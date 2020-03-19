/**
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef UTILS_OGLPLUS_OS_CONTEXT_GLFW3_1107121519_HPP
#define UTILS_OGLPLUS_OS_CONTEXT_GLFW3_1107121519_HPP

#include "os_context_common.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace eagine {
namespace oglp {

class offscreen_context_glfw3 {
private:
    ::GLFWwindow* window;

public:
    offscreen_context_glfw3(const offscreen_context_params& p) {
        if(!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW3.");
        }

        window = glfwCreateWindow(p.width, p.height, p.title, nullptr, nullptr);

        if(!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW3 window.");
        }
        glfwMakeContextCurrent(window);
    }

    ~offscreen_context_glfw3() noexcept {
        try {
            glfwTerminate();
        } catch(...) {
        }
    }
};

} // namespace oglp
} // namespace eagine

#endif
