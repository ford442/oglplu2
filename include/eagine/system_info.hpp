/**
 *  @file eagine/system_info.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef EAGINE_SYSTEM_INFO_HPP
#define EAGINE_SYSTEM_INFO_HPP

#include "config/basic.hpp"
#include "config/platform.hpp"
#include "types.hpp"
#include "valid_if/positive.hpp"
#include <thread>

#if EAGINE_POSIX
#include <unistd.h>
#endif

namespace eagine {

class system_info {
public:
    valid_if_positive<span_size_t> cpu_concurrent_threads() const noexcept {
        return {span_size_t(std::thread::hardware_concurrency())};
    }

    valid_if_positive<span_size_t> memory_page_size() const noexcept {
#if EAGINE_POSIX
#if defined(_SC_PAGESIZE)
        return {span_size(::sysconf(_SC_PAGESIZE))};
#endif
#endif
        return {0};
    }
};

} // namespace eagine

#endif // EAGINE_SYSTEM_INFO_HPP
