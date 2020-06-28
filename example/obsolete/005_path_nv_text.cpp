/**
 *  example oglplus/005_path_nv_text.cpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

// clang-format off
#include <oglplus/gl.hpp>
#include <oglplus/constants.hpp>
#include <oglplus/operations.hpp>

#include "example.hpp"

#include <string>
// clang-format on

namespace oglplus {

class example_clear : public example {
private:
    constants GL;
    operations gl;

    std::string text;
    paths_nv text_paths;

    span<GLuint> glyph_indices;
    span<GLfloat> glyph_spacings;

public:
    example_clear()
      : text("OpenGL®")
      , text_paths(span_size(text.length())) {
        text_paths.glyphs(
          GL.standard_font_name_nv,
          "Sans",
          GL.bold_bit_nv,
          string_view(text),
          GL.use_missing_glyph_nv,
          ~0u,
          64);

        static GLuint indices[8] = {0, 1, 2, 3, 4, 5, 6, 0};
        static GLfloat spacings[9] = {0.f};

        text_paths.get_spacing(
          GL.accum_adjacent_pairs_nv,
          cover(indices),
          1.0f,
          1.0f,
          GL.translate_x_nv,
          skip(cover(spacings), 1));

        glyph_indices = head(cover(indices), 7);
        glyph_spacings = head(cover(spacings), 8);

        GLfloat text_left = *glyph_spacings.begin();
        GLfloat text_right = *glyph_spacings.rbegin();

        GLfloat font_min_max[2];

        text_paths.get_metric_range(
          GL.font_x_min_bounds_bit_nv | GL.font_y_max_bounds_bit_nv,
          1,
          0,
          cover(font_min_max));

        gl.clear_color(1.0f, 1.0f, 1.0f, 0.0f);
        gl.clear_stencil(0);
        gl.stencil_mask(~0u);
        gl.stencil_func(GL.notequal, 0, 0x1F);
        gl.stencil_op(GL.keep, GL.keep, GL.zero);

        gl.enable(GL.stencil_test);

        gl.load_identity(GL.projection);
        gl.ortho(
          GL.projection,
          GLdouble(text_left - 10),
          GLdouble(text_right + 10),
          GLdouble(font_min_max[0] + 10),
          GLdouble(font_min_max[1] - 10),
          0,
          1);

        gl.load_identity(GL.modelview);
    }

    void resize(const example_context& ctx) final {
        const auto& state = ctx.state();
        gl.viewport(0, 0, state.width(), state.height());
    }

    void render(const example_context&) final {
        gl.clear(GL.color_buffer_bit | GL.stencil_buffer_bit);

        gl.color_f(0.2f, 0.2f, 1.0f);

        text_paths.stencil_fill_instanced(
          glyph_indices,
          GL.count_up_nv,
          0xFF,
          GL.translate_x_nv,
          glyph_spacings);
        text_paths.cover_fill_instanced(
          glyph_indices,
          GL.bounding_box_of_bounding_boxes_nv,
          GL.translate_x_nv,
          glyph_spacings);

        gl.color_f(0.0f, 0.0f, 0.0f);

        text_paths.stencil_stroke_instanced(
          glyph_indices, 1, ~0u, GL.translate_x_nv, glyph_spacings);
        text_paths.cover_stroke_instanced(
          glyph_indices, GL.convex_hull_nv, GL.translate_x_nv, glyph_spacings);
    }
};

std::unique_ptr<example> make_example(
  const example_args&, const example_context&) {
    return std::unique_ptr<example>(new example_clear());
}

void adjust_params(example_params& params) {
    params.compatibility_context(true);
    params.depth_buffer(false);
    params.stencil_buffer(true);
}

bool is_example_param(const example_arg&) {
    return false;
}

} // namespace oglplus
