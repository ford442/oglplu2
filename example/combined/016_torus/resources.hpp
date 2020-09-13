/**
 *  example combined/016_torus/resources.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#include <oglplus/gl.hpp>
#include <oglplus/gl_api.hpp>

#include <oglplus/shapes/drawing.hpp>

namespace eagine {
class example_context;
namespace oglp {
class example_orbiting_camera;
//------------------------------------------------------------------------------
// program
//------------------------------------------------------------------------------
class torus_program {
private:
    owned_program_name prog;
    uniform_location camera_loc;

public:
    void init(example_context& ctx);
    void set_projection(
      const example_context& ctx,
      const example_orbiting_camera& camera);

    void bind_position_location(
      const example_context& ctx,
      vertex_attrib_location loc);
    void bind_normal_location(
      const example_context& ctx,
      vertex_attrib_location loc);
    void bind_texcoord_location(
      const example_context& ctx,
      vertex_attrib_location loc);
};
//------------------------------------------------------------------------------
// geometry
//------------------------------------------------------------------------------
class torus_geometry {
private:
    owned_vertex_array_name vao;

    owned_buffer_name positions;
    owned_buffer_name normals;
    owned_buffer_name texcoords;
    owned_buffer_name indices;

    std::vector<shape_draw_operation> ops;

public:
    void init(example_context& ctx);
    void draw(const example_context& ctx);

    static auto position_loc() noexcept {
        return vertex_attrib_location{0};
    }

    static auto normal_loc() noexcept {
        return vertex_attrib_location{1};
    }

    static auto texcoord_loc() noexcept {
        return vertex_attrib_location{2};
    }
};
//------------------------------------------------------------------------------
} // namespace oglp
} // namespace eagine
