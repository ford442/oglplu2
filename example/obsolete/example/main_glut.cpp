/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#include "main.hpp"
#include <oglplus/config/basic.hpp>
#if OGLPLUS_GLUT_FOUND
#include "args.hpp"
#include "state.hpp"
#include "wrapper.hpp"
#include <oglplus/gl.hpp>

#include <eagine/assert.hpp>
#include <eagine/program_args.hpp>
#include <eagine/scope_exit.hpp>

#if OGLPLUS_FREEGLUT_FOUND
#include <GL/freeglut.h>
#elif defined(__APPLE__) && __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cassert>
#include <iostream>
#endif // OGLPLUS_GLUT_FOUND

#if defined(__APPLE__) && __APPLE__ && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

namespace eagine {
//------------------------------------------------------------------------------
#if OGLPLUS_GLUT_FOUND

class single_glut_context {
private:
    static auto instance_ptr() -> single_glut_context*& {
        static single_glut_context* pinst = nullptr;
        return pinst;
    }

    static auto instance() -> single_glut_context& {
        EAGINE_ASSERT(instance_ptr());
        return *instance_ptr();
    }

    example_wrapper example;
    int _height;
    int _wheel;

public:
    single_glut_context(example_run_context& erc)
      : example(erc)
      , _height(erc.state.height())
      , _wheel(0) {
        EAGINE_ASSERT(!instance_ptr());
        instance_ptr() = this;
        if(example.is_ready()) {
            glutDisplayFunc(&display_func);
            glutIdleFunc(&display_func);
            glutReshapeFunc(&reshape_func);

            glutMotionFunc(&motion_func);
            glutPassiveMotionFunc(&motion_func);

            glutMouseFunc(&mouse_func);
            glutKeyboardFunc(&keyboard_func);

#ifdef FREEGLUT
            glutSetOption(
              GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
            glutCloseFunc(&close_func);
            example.context().log().info("using FreeGLUT context");
#else
            example.context().log().info("using GLUT context");
#endif
        }
    }

    auto is_ready() const -> bool {
        return example.is_ready();
    }

    single_glut_context(single_glut_context&&) = delete;
    single_glut_context(const single_glut_context&) = delete;
    auto operator=(single_glut_context&&) = delete;
    auto operator=(const single_glut_context&) = delete;

    ~single_glut_context() noexcept {
        EAGINE_ASSERT(instance_ptr());
        instance_ptr() = nullptr;
    }

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif

#ifdef FREEGLUT
    void quit() {
        glutLeaveMainLoop();
    }
#else
    [[noreturn]] void quit() {
        exit(0);
    }
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

private:
    void close() {
        example.destroy();
    }

    static void close_func() {
        instance().close();
    }

    void display() {
        example.update();

        example.render();
        glutSwapBuffers();

        if(!example.next_frame()) {
            quit();
        }
    }

    static void display_func() {
        instance().display();
    }

    void reshape(int width, int height) {
        _height = height;
        example.set_size(width, height);
    }

    static void reshape_func(int width, int height) {
        instance().reshape(width, height);
    }

    void motion(int x, int y) {
        example.set_mouse_pos(x, _height - y);
    }

    static void motion_func(int x, int y) {
        instance().motion(x, y);
    }

    void mouse_press(int button, int state, int x, int y) {
        example.set_mouse_pos(x, _height - y);
        if(button == GLUT_LEFT_BUTTON) {
            example.set_mouse_btn(1, state == GLUT_DOWN);
        } else if(button == 3) {
            if(state == GLUT_DOWN) {
                example.set_mouse_wheel(++_wheel);
            }
        } else if(button == 4) {
            if(state == GLUT_DOWN) {
                example.set_mouse_wheel(--_wheel);
            }
        }
    }

    static void mouse_func(int button, int state, int x, int y) {
        instance().mouse_press(button, state, x, y);
    }

    void key_press(unsigned char k) {
        if(k == 0x1B) {
            quit();
        }
        // TODO
    }

    static void keyboard_func(unsigned char k, int, int) {
        instance().key_press(k);
    }
};

#endif // OGLPLUS_GLUT_FOUND
//------------------------------------------------------------------------------
class example_main_glut : public example_main_intf {
public:
    auto is_implemented() -> bool final {
        return (OGLPLUS_GLUT_FOUND != 0);
    }

    auto implementation_name() -> string_view final {
#if OGLPLUS_FREEGLUT_FOUND
        return {"FREEGLUT"};
#else
        return {"GLUT"};
#endif
    }

    auto run(example_run_context& erc) -> int final {
        EAGINE_MAYBE_UNUSED(erc);
#if OGLPLUS_GLUT_FOUND

        int argc = erc.args.argc();
        char** argv = const_cast<char**>(erc.args.argv());

        glutInit(&argc, argv);
        glutInitDisplayMode(
#if defined(__APPLE__) && __APPLE__
          GLUT_3_2_CORE_PROFILE |
#endif
          // NOLINTNEXTLINE(hicpp-signed-bitwise)
          GLUT_DOUBLE | GLUT_RGBA |
          (erc.params.depth_buffer() ? GLUT_DEPTH : 0) |
          (erc.params.stencil_buffer() ? GLUT_STENCIL : 0));
#ifdef FREEGLUT
        glutInitContextVersion(3, 0);
#endif
        glutInitWindowSize(erc.state.width(), erc.state.height());
        glutInitWindowPosition(
          erc.params.window_x_pos(), erc.params.window_y_pos());
#ifdef FREEGLUT
        glutCreateWindow("OGLplus example (FREEGLUT)");
#else
        glutCreateWindow("OGLplus example (GLUT)");
#endif

        oglp::api_initializer gl_api_init;

        erc.state.set_depth(16);

        single_glut_context ctx(erc);
        if(!ctx.is_ready()) {
            return 2;
        }
        glutMainLoop();

        return 0;
#else
        return 1;
#endif
    }
};
//------------------------------------------------------------------------------
auto make_example_main_glut() -> std::unique_ptr<example_main_intf> {
    return {std::make_unique<example_main_glut>()};
}
//------------------------------------------------------------------------------
} // namespace eagine

#if defined(__APPLE__) && __APPLE__ && defined(__clang__)
#pragma clang diagnostic pop
#endif
