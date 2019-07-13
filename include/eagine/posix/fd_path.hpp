/**
 *  @file eagine/posix/fd_path.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef EAGINE_POSIX_FD_PATH_HPP
#define EAGINE_POSIX_FD_PATH_HPP

#include "dir.hpp"
#include "file.hpp"
#include "file_stat.hpp"
#include <string>
#include <utility>

namespace eagine {
namespace posix {

static inline outcome<std::string> fd_abs_path(file_descriptor_owner curr_fd) {
    using inode_id = std::pair<dev_t, ino_t>;
    struct stat fst {};

    if(auto res = failure(fstat(file_descriptor_owner(open("/", 0)), fst))) {
        return res.release_handler();
    }
    const inode_id root_id(fst.st_dev, fst.st_ino);

    if(auto res = failure(fstat(curr_fd, fst))) {
        return res.release_handler();
    }
    inode_id curr_id(fst.st_dev, fst.st_ino);

    std::string path;

    while(curr_id != root_id) {
        {
            outcome<owned_file_descriptor> res = openat(curr_fd, "..", 0);

            if(res.failed()) {
                return res.release_handler();
            }
            curr_fd = file_descriptor_owner(res.rvalue());
        }
        {
            outcome<owned_dir_descriptor> res = fdopendir(dup(curr_fd));

            if(res.failed()) {
                return res.release_handler();
            }

            dir_descriptor_owner(res.rvalue())
              .get()
              .for_each_entry([&path, &curr_id](const ::dirent& entry) -> bool {
                  if(entry.d_ino == curr_id.second) {
                      path.insert(0, static_cast<const char*>(entry.d_name));
                      path.insert(0, "/");
                      return false;
                  }
                  return true;
              });
        }
        if(auto res = failure(fstat(curr_fd, fst))) {
            return res.release_handler();
        }
        curr_id = inode_id(fst.st_dev, fst.st_ino);
    }

    return {path};
}

static inline outcome<std::string> safe_getcwd() {
    outcome<owned_file_descriptor> res = open(".", 0);
    if(res.failed()) {
        return res.release_handler();
    }
    return fd_abs_path(file_descriptor_owner(res.rvalue()));
}

} // namespace posix
} // namespace eagine

#endif // EAGINE_POSIX_FD_PATH_HPP
