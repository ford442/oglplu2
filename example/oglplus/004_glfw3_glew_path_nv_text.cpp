/**
 *  @example standalone/004_glfw3_glew_path_nv.cpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#include <GL/glew.h>

#include <oglplus/constant_defs.hpp>
#include <oglplus/constants.hpp>
#include <oglplus/error/format.hpp>
#include <oglplus/glsl/string_ref.hpp>
#include <oglplus/math/vector.hpp>
#include <oglplus/operations.hpp>

#include <eagine/memory/span_algo.hpp>
#include <eagine/scope_exit.hpp>

#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

static oglplus::constants GL;
static oglplus::operations gl;

static void handle_resize(
  int width, int height, float l, float r, float b, float t) {
    gl.viewport(0, 0, width, height);

    gl.matrix_mode(GL.projection);
    gl.load_identity();
    gl.ortho(double(l), double(r), double(b), double(t), 0, 1);
}

static void run_loop(GLFWwindow* window, int width, int height) {
    using namespace oglplus;

    std::string text = "OpenGL®";

    paths_nv text_path(span_size(text.length()));

    text_path.glyphs(
      GL.standard_font_name_nv,
      "Sans",
      GL.bold_bit_nv,
      string_view(text),
      GL.use_missing_glyph_nv,
      ~0u,
      64);

    GLuint indices[8] = {0, 1, 2, 3, 4, 5, 6, 0};
    GLfloat spacings[9] = {0.f};

    text_path.get_spacing(
      GL.accum_adjacent_pairs_nv,
      view(indices),
      1.0f,
      1.0f,
      GL.translate_x_nv,
      skip(cover(spacings), 1));

    const auto glyph_indices = head(view(indices), 7);
    const auto glyph_spacings = head(view(spacings), 8);

    const GLfloat text_left = *glyph_spacings.begin();
    const GLfloat text_right = *glyph_spacings.rbegin();

    GLfloat font_min_max[2];

    text_path.get_metric_range(
      GL.font_x_min_bounds_bit_nv | GL.font_y_max_bounds_bit_nv,
      1,
      0,
      cover(font_min_max));

    gl.clear_color(vec4(1.0f, 1.0f, 1.0f, 0.0f));
    gl.clear_stencil(0);
    gl.stencil_mask(~0u);
    gl.stencil_func(GL.notequal, 0, 0x1F);
    gl.stencil_op(GL.keep, GL.keep, GL.zero);

    gl.enable(GL.stencil_test);

    handle_resize(
      width,
      height,
      text_left - 10,
      text_right + 10,
      font_min_max[0] + 10,
      font_min_max[1] - 10);

    while(true) {
        glfwPollEvents();

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
            break;
        }

        if(glfwWindowShouldClose(window)) {
            break;
        }

        int new_width, new_height;
        glfwGetWindowSize(window, &new_width, &new_height);

        if((width != new_width) || (height != new_height)) {
            width = new_width;
            height = new_height;

            handle_resize(
              width,
              height,
              text_left - 10,
              text_right + 10,
              font_min_max[0] + 10,
              font_min_max[1] - 10);
        }

        gl.clear(GL.color_buffer_bit | GL.stencil_buffer_bit);

        gl.matrix_mode(GL.modelview);
        gl.load_identity();

        gl.color_f(0.2f, 0.2f, 1.0f);

        text_path.stencil_fill_instanced(
          glyph_indices,
          GL.count_up_nv,
          0xFF,
          GL.translate_x_nv,
          glyph_spacings);
        text_path.cover_fill_instanced(
          glyph_indices,
          GL.bounding_box_of_bounding_boxes_nv,
          GL.translate_x_nv,
          glyph_spacings);

        gl.color_f(0.0f, 0.0f, 0.0f);

        text_path.stencil_stroke_instanced(
          glyph_indices, 1, ~0u, GL.translate_x_nv, glyph_spacings);
        text_path.cover_stroke_instanced(
          glyph_indices, GL.convex_hull_nv, GL.translate_x_nv, glyph_spacings);

        glfwSwapBuffers(window);
    }
}

static void init_and_run() {
    if(!glfwInit()) {
        throw std::runtime_error("GLFW initialization error");
    } else {
        auto ensure_glfw_cleanup = eagine::finally(glfwTerminate);

        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 0);
        glfwWindowHint(GLFW_DEPTH_BITS, 0);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);

        glfwWindowHint(GLFW_SAMPLES, GLFW_DONT_CARE);

        int width = 800, height = 600;

        GLFWwindow* window =
          glfwCreateWindow(width, height, "OGLplus example", nullptr, nullptr);

        if(!window) {
            throw std::runtime_error("Error creating GLFW window");
        } else {
            glfwMakeContextCurrent(window);
            glewExperimental = GL_TRUE;
            GLenum init_result = glewInit();
            glGetError();
            if(init_result != GLEW_OK) {
                throw std::runtime_error("OpenGL/GLEW initialization error.");
            } else {
                run_loop(window, width, height);
            }
        }
    }
}

int main() {
    try {
        init_and_run();
        return 0;
    } catch(oglplus::error& gle) {
        oglplus::format_error(
          gle,
          "OpenGL error\n"
          "in GL function: %(gl_function_name)\n"
          "with enum parameter: %(gl_enum_value)\n"
          "from source file: %(source_file)\n"
          "from source line: %(source_line)\n"
          "%(message)",
          std::cerr)
          << std::endl;
    } catch(std::runtime_error& sre) {
        std::cerr << "Runtime error: " << sre.what() << std::endl;
    } catch(std::exception& se) {
        std::cerr << "Unknown error: " << se.what() << std::endl;
    }
    return 1;
}
