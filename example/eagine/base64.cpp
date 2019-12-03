/**
 *  @example eagine/base64.cpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#include <eagine/base64.hpp>
#include <eagine/file_contents.hpp>
#include <eagine/program_args.hpp>
#include <iostream>

int main(int argc, const char** argv) {
    using namespace eagine;

    program_args args(argc, argv);

    std::string temp;
    for(auto& arg : args) {
        if(!arg.starts_with("-")) {
            if(arg.prev().is_tag("-f", "--file")) {
                file_contents fc(arg.get());
                std::cout << arg << '|'
                          << base64_encode(fc.block(), temp).value_or("-")
                          << std::endl;
            } else if(arg.prev().is_tag("-s", "--string")) {
                std::cout << arg << '|'
                          << base64_encode(arg.block(), temp).value_or("-")
                          << std::endl;
            }
        }
    }
    return 0;
}