/**
 *  .file oglplus/example/wrapper.cpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef OGLPLUS_EXAMPLE_WRAPPER_HPP
#define OGLPLUS_EXAMPLE_WRAPPER_HPP

#include "../example.hpp"
#include <cassert>
#include <chrono>
#include <vector>

namespace oglplus {

class example_wrapper {
private:
    example_context _context;

    std::unique_ptr<example> _example;

    bool _screenshot_done;

    using clock_type = std::chrono::system_clock;
    const std::chrono::time_point<clock_type> _start;
    std::chrono::time_point<clock_type> _now;

    std::vector<char> _pixel_data;
    std::vector<char>& pixels();
    std::vector<char> _textbuf;
    std::vector<char>& textbuf(std::size_t);

public:
    example_wrapper(example_args&, example_params&, example_state&);

    void destroy();

    bool next_frame();

    void update();

    void render();

    void set_size(int width, int height);

    void set_mouse_btn(int i, bool pressed);

    void set_mouse_pos(int x, int y);

    void set_mouse_wheel(int w);
};

} // namespace oglplus

#endif // OGLPLUS_EXAMPLE_WRAPPER_HPP
