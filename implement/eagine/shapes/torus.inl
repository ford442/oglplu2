/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#include <eagine/assert.hpp>
#include <eagine/math/constants.hpp>
#include <eagine/math/functions.hpp>
#include <cmath>
#include <random>

#ifdef __clang__
EAGINE_DIAG_PUSH()
EAGINE_DIAG_OFF(double-promotion)
#endif

namespace eagine {
namespace shapes {
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto unit_torus_gen::_attr_mask() noexcept -> vertex_attrib_bits {
    return vertex_attrib_kind::position | vertex_attrib_kind::normal |
           vertex_attrib_kind::tangential | vertex_attrib_kind::bitangential |
           vertex_attrib_kind::pivot | vertex_attrib_kind::pivot_pivot |
           vertex_attrib_kind::vertex_pivot | vertex_attrib_kind::box_coord |
           vertex_attrib_kind::wrap_coord;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
unit_torus_gen::unit_torus_gen(
  vertex_attrib_bits attr_bits,
  valid_if_greater_than<int, 4> rings,
  valid_if_greater_than<int, 3> sections,
  valid_if_ge0_lt1<float> radius_ratio) noexcept
  : _base(attr_bits & _attr_mask())
  , _rings(span_size_t(rings.value()))
  , _sections(span_size_t(sections.value()))
  , _radius_ratio(radius_ratio.value()) {}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto unit_torus_gen::vertex_count() -> span_size_t {
    return (_rings + 1) * (_sections + 1);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_torus_gen::vertex_pivots(span<float> dest) noexcept {
    EAGINE_ASSERT(has(vertex_attrib_kind::position));
    EAGINE_ASSERT(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;

    const auto ro = 0.50;
    const auto ri = ro * _radius_ratio;
    const auto rc = (ro + ri) / 2;

    const auto s_step = 2 * math::pi / _sections;

    for(const auto s : integer_range(_sections + 1)) {
        const auto vx = std::cos(s * s_step) * rc;
        const auto vz = -std::sin(s * s_step) * rc;

        for(const auto r : integer_range(_rings + 1)) {
            EAGINE_MAYBE_UNUSED(r);
            dest[k++] = float(vx);
            dest[k++] = float(0);
            dest[k++] = float(vz);
        }
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_torus_gen::positions(
  span<float> dest,
  unit_torus_gen::offset_getter get_offs) noexcept {
    EAGINE_ASSERT(has(vertex_attrib_kind::position));
    EAGINE_ASSERT(dest.size() >= vertex_count() * 3);

    const auto ro = 0.50;
    const auto ri = ro * _radius_ratio;
    const auto r1 = ri;
    const auto r2 = ro - ri;

    const auto s_step = 2 * math::pi / _sections;
    const auto r_step = 2 * math::pi / _rings;

    auto k = [this](span_size_t s, span_size_t r, span_size_t c) {
        return 3 * (s * (_rings + 1) + r) + c;
    };

    for(const auto s : integer_range(_sections)) {
        for(const auto r : integer_range(_rings)) {
            const auto [rd, sd, td] = get_offs(s, r);

            const auto vr = -std::cos((r + rd) * r_step);
            const auto vx = std::cos((s + sd) * s_step);
            const auto vy = std::sin((r + rd) * r_step);
            const auto vz = -std::sin((s + sd) * s_step);
            const auto rt = r2 * (1 + td);

            dest[k(s, r, 0)] = float(vx * (r1 + rt * (1 + vr)));
            dest[k(s, r, 1)] = float(vy * rt);
            dest[k(s, r, 2)] = float(vz * (r1 + rt * (1 + vr)));
        }
        dest[k(s, _rings, 0)] = dest[k(s, 0, 0)];
        dest[k(s, _rings, 1)] = dest[k(s, 0, 1)];
        dest[k(s, _rings, 2)] = dest[k(s, 0, 2)];
    }
    for(const auto r : integer_range(_rings + 1)) {
        dest[k(_sections, r, 0)] = dest[k(0, r, 0)];
        dest[k(_sections, r, 1)] = dest[k(0, r, 1)];
        dest[k(_sections, r, 2)] = dest[k(0, r, 2)];
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_torus_gen::normals(
  span<float> dest,
  unit_torus_gen::offset_getter get_offs) noexcept {
    EAGINE_ASSERT(has(vertex_attrib_kind::normal));
    EAGINE_ASSERT(dest.size() >= vertex_count() * 3);

    const auto s_step = 2 * math::pi / _sections;
    const auto r_step = 2 * math::pi / _rings;

    auto k = [this](span_size_t s, span_size_t r, span_size_t c) {
        return 3 * (s * (_rings + 1) + r) + c;
    };

    for(const auto s : integer_range(_sections)) {
        for(const auto r : integer_range(_rings)) {
            const auto [rd, sd, td] = get_offs(s, r);
            EAGINE_MAYBE_UNUSED(td);

            const auto nr = -std::cos((r + rd) * r_step);
            const auto nx = std::cos((s + sd) * s_step);
            const auto ny = std::sin((r + rd) * r_step);
            const auto nz = -std::sin((s + sd) * s_step);

            dest[k(s, r, 0)] = float(nx * nr);
            dest[k(s, r, 1)] = float(ny);
            dest[k(s, r, 2)] = float(nz * nr);
        }
        dest[k(s, _rings, 0)] = dest[k(s, 0, 0)];
        dest[k(s, _rings, 1)] = dest[k(s, 0, 1)];
        dest[k(s, _rings, 2)] = dest[k(s, 0, 2)];
    }
    for(const auto r : integer_range(_rings + 1)) {
        dest[k(_sections, r, 0)] = dest[k(0, r, 0)];
        dest[k(_sections, r, 1)] = dest[k(0, r, 1)];
        dest[k(_sections, r, 2)] = dest[k(0, r, 2)];
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_torus_gen::tangentials(
  span<float> dest,
  unit_torus_gen::offset_getter get_offs) noexcept {
    EAGINE_ASSERT(has(vertex_attrib_kind::tangential));
    EAGINE_ASSERT(dest.size() >= vertex_count() * 3);

    auto k = [this](span_size_t s, span_size_t r, span_size_t c) {
        return 3 * (s * (_rings + 1) + r) + c;
    };

    const auto s_step = 2 * math::pi / _sections;

    for(const auto s : integer_range(_sections)) {
        for(const auto r : integer_range(_rings)) {
            const auto [rd, sd, td] = get_offs(s, r);
            EAGINE_MAYBE_UNUSED(rd);
            EAGINE_MAYBE_UNUSED(td);

            const auto tx = -std::sin((s + sd) * s_step);
            const auto tz = -std::cos((s + sd) * s_step);

            dest[k(s, r, 0)] = float(tx);
            dest[k(s, r, 1)] = float(0);
            dest[k(s, r, 2)] = float(tz);
        }
        dest[k(s, _rings, 0)] = dest[k(s, 0, 0)];
        dest[k(s, _rings, 1)] = dest[k(s, 0, 1)];
        dest[k(s, _rings, 2)] = dest[k(s, 0, 2)];
    }
    for(const auto r : integer_range(_rings + 1)) {
        dest[k(_sections, r, 0)] = dest[k(0, r, 0)];
        dest[k(_sections, r, 1)] = dest[k(0, r, 1)];
        dest[k(_sections, r, 2)] = dest[k(0, r, 2)];
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_torus_gen::bitangentials(
  span<float> dest,
  unit_torus_gen::offset_getter get_offs) noexcept {
    EAGINE_ASSERT(has(vertex_attrib_kind::bitangential));
    EAGINE_ASSERT(dest.size() >= vertex_count() * 3);

    auto k = [this](span_size_t s, span_size_t r, span_size_t c) {
        return 3 * (s * (_rings + 1) + r) + c;
    };

    const auto s_step = 2 * math::pi / _sections;
    const auto r_step = 2 * math::pi / _rings;

    const auto ty = 0;

    for(const auto s : integer_range(_sections)) {
        for(const auto r : integer_range(_rings)) {
            const auto [rd, sd, td] = get_offs(s, r);
            EAGINE_MAYBE_UNUSED(td);

            const auto tx = -std::sin((s + sd) * s_step);
            const auto tz = -std::cos((s + sd) * s_step);
            const auto nr = -std::cos((r + rd) * r_step);
            const auto ny = std::sin((r + rd) * r_step);
            const auto nx = -tz * nr;
            const auto nz = tx * nr;

            dest[k(s, r, 0)] = float(ny * tz - nz * ty);
            dest[k(s, r, 1)] = float(nz * tx - nx * tz);
            dest[k(s, r, 2)] = float(nx * ty - ny * tx);
        }
        dest[k(s, _rings, 0)] = dest[k(s, 0, 0)];
        dest[k(s, _rings, 1)] = dest[k(s, 0, 1)];
        dest[k(s, _rings, 2)] = dest[k(s, 0, 2)];
    }
    for(const auto r : integer_range(_rings + 1)) {
        dest[k(_sections, r, 0)] = dest[k(0, r, 0)];
        dest[k(_sections, r, 1)] = dest[k(0, r, 1)];
        dest[k(_sections, r, 2)] = dest[k(0, r, 2)];
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_torus_gen::wrap_coords(span<float> dest) noexcept {
    EAGINE_ASSERT(has(vertex_attrib_kind::wrap_coord));
    EAGINE_ASSERT(dest.size() >= vertex_count() * 2);

    span_size_t k = 0;

    const auto s_step = 1.F / _sections;
    const auto r_step = 1.F / _rings;

    for(const auto s : integer_range(_sections + 1)) {
        for(const auto r : integer_range(_rings + 1)) {
            dest[k++] = s * s_step;
            dest[k++] = r * r_step;
        }
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto unit_torus_gen::special_variant_name(span_size_t index) -> string_view {
    if(index == 1) {
        return {"random"};
    }
    return {};
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_torus_gen::make_special_attrib_values(
  void (unit_torus_gen::*function)(span<float>, unit_torus_gen::offset_getter),
  span_size_t variant_index,
  span<float> dest) {
    if(variant_index == 1) {
        auto get_offs = [rrg{std::mt19937{_r_seed}},
                         srg{std::mt19937{_s_seed}},
                         rnd{std::normal_distribution<float>{0.F, 0.15F}},
                         snd{std::normal_distribution<float>{0.F, 0.15F}}](
                          span_size_t,
                          span_size_t) mutable -> std::array<float, 3> {
            return {{rnd(rrg), snd(srg), 0.F}};
        };
        (this->*function)(dest, {construct_from, get_offs});
    } else if(variant_index == 2) {
        auto get_offs = [](span_size_t s, span_size_t) -> std::array<float, 3> {
            return {{float(s), 0.F, 0.F}};
        };
        (this->*function)(dest, {construct_from, get_offs});
    } else if(variant_index == 3) {
        auto get_offs =
          [this](span_size_t s, span_size_t r) -> std::array<float, 3> {
            const auto x = float(s) / float(this->_sections);
            const auto y = float(r) / float(this->_rings);
            return {
              {float(math::sine_wave01(x)), float(math::sine_wave01(y)), 0.F}};
        };
        (this->*function)(dest, {construct_from, get_offs});
    } else {
        const auto no_offs =
          [](span_size_t, span_size_t) -> std::array<float, 3> {
            return {{0.F, 0.F, 0.F}};
        };
        (this->*function)(dest, offset_getter{no_offs});
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto unit_torus_gen::attribute_variants(vertex_attrib_kind attrib)
  -> span_size_t {
    switch(attrib) {
        case vertex_attrib_kind::pivot_pivot:
        case vertex_attrib_kind::vertex_pivot:
            break;
        case vertex_attrib_kind::position:
        case vertex_attrib_kind::normal:
        case vertex_attrib_kind::tangential:
        case vertex_attrib_kind::bitangential:
            return 4;
        case vertex_attrib_kind::wrap_coord:
        case vertex_attrib_kind::pivot:
        case vertex_attrib_kind::box_coord:
        case vertex_attrib_kind::face_coord:
        case vertex_attrib_kind::object_id:
        case vertex_attrib_kind::polygon_id:
        case vertex_attrib_kind::material_id:
        case vertex_attrib_kind::weight:
        case vertex_attrib_kind::color:
        case vertex_attrib_kind::occlusion:
            break;
    }
    return _base::attribute_variants(attrib);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto unit_torus_gen::variant_name(vertex_attrib_variant vav) -> string_view {
    switch(vav.attribute()) {
        case vertex_attrib_kind::position:
        case vertex_attrib_kind::normal:
        case vertex_attrib_kind::tangential:
        case vertex_attrib_kind::bitangential:
        case vertex_attrib_kind::wrap_coord:
            return special_variant_name(vav.index());
        case vertex_attrib_kind::vertex_pivot:
        case vertex_attrib_kind::pivot:
        case vertex_attrib_kind::pivot_pivot:
        case vertex_attrib_kind::box_coord:
        case vertex_attrib_kind::face_coord:
        case vertex_attrib_kind::object_id:
        case vertex_attrib_kind::polygon_id:
        case vertex_attrib_kind::material_id:
        case vertex_attrib_kind::weight:
        case vertex_attrib_kind::color:
        case vertex_attrib_kind::occlusion:
            break;
    }
    return centered_unit_shape_generator_base::variant_name(vav);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_torus_gen::attrib_values(vertex_attrib_variant vav, span<float> dest) {
    switch(vav.attribute()) {
        case vertex_attrib_kind::vertex_pivot:
            vertex_pivots(dest);
            break;
        case vertex_attrib_kind::position:
            make_special_attrib_values(
              &unit_torus_gen::positions, vav.index(), dest);
            break;
        case vertex_attrib_kind::normal:
            make_special_attrib_values(
              &unit_torus_gen::normals, vav.index(), dest);
            break;
        case vertex_attrib_kind::tangential:
            make_special_attrib_values(
              &unit_torus_gen::tangentials, vav.index(), dest);
            break;
        case vertex_attrib_kind::bitangential:
            make_special_attrib_values(
              &unit_torus_gen::bitangentials, vav.index(), dest);
            break;
        case vertex_attrib_kind::wrap_coord:
            wrap_coords(dest);
            break;
        case vertex_attrib_kind::pivot:
        case vertex_attrib_kind::pivot_pivot:
        case vertex_attrib_kind::box_coord:
        case vertex_attrib_kind::face_coord:
        case vertex_attrib_kind::object_id:
        case vertex_attrib_kind::polygon_id:
        case vertex_attrib_kind::material_id:
        case vertex_attrib_kind::weight:
        case vertex_attrib_kind::color:
        case vertex_attrib_kind::occlusion:
            centered_unit_shape_generator_base::attrib_values(vav, dest);
            break;
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto unit_torus_gen::draw_variant_count() -> span_size_t {
    return 2;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto unit_torus_gen::index_type(drawing_variant v) -> index_data_type {
    if(index_count(v) < span_size(std::numeric_limits<std::uint8_t>::max())) {
        return index_data_type::unsigned_8;
    }
    if(index_count(v) < span_size(std::numeric_limits<std::uint16_t>::max())) {
        return index_data_type::unsigned_16;
    }
    return index_data_type::unsigned_32;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto unit_torus_gen::index_count(drawing_variant var) -> span_size_t {
    if(var == 0) {
        return _sections * ((_rings + 1) * 2 + (primitive_restart() ? 1 : 0));
    }
    if(var == 1) {
        return (_sections * (_rings + (primitive_restart() ? 1 : 0))) +
               (_rings * (_sections + (primitive_restart() ? 1 : 0)));
    }
    return 0;
}
//------------------------------------------------------------------------------
template <typename T>
void unit_torus_gen::_indices(drawing_variant var, span<T> dest) noexcept {
    EAGINE_ASSERT(dest.size() >= index_count(var));

    const auto pri = limit_cast<T>(vertex_count());
    span_size_t k = 0;

    if(var == 0) {
        span_size_t step = _rings + 1;
        for(const auto s : integer_range(_sections)) {
            for(const auto r : integer_range(step)) {
                dest[k++] = limit_cast<T>((s + 0) * step + r);
                dest[k++] = limit_cast<T>((s + 1) * step + r);
            }

            if(primitive_restart()) {
                dest[k++] = pri;
            }
        }
    } else if(var == 1) {
        for(const auto s : integer_range(_sections)) {
            for(const auto r : integer_range(_rings)) {
                dest[k++] = limit_cast<T>(s * (_rings + 1) + r);
            }

            if(primitive_restart()) {
                dest[k++] = pri;
            }
        }
        for(const auto r : integer_range(_rings)) {
            for(const auto s : integer_range(_sections)) {
                dest[k++] = limit_cast<T>(s * (_rings + 1) + r);
            }

            if(primitive_restart()) {
                dest[k++] = pri;
            }
        }
    }
    EAGINE_ASSERT(k == index_count(var));
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_torus_gen::indices(drawing_variant var, span<std::uint8_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_torus_gen::indices(drawing_variant var, span<std::uint16_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_torus_gen::indices(drawing_variant var, span<std::uint32_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto unit_torus_gen::operation_count(drawing_variant var) -> span_size_t {
    if(var == 0) {
        if(primitive_restart()) {
            return 1;
        } else {
            return _sections;
        }
    }
    if(var == 1) {
        if(primitive_restart()) {
            return 1;
        } else {
            return _sections + _rings;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_torus_gen::instructions(
  drawing_variant var,
  span<draw_operation> ops) {
    EAGINE_ASSERT(ops.size() >= operation_count(var));

    if(var == 0) {
        if(primitive_restart()) {
            draw_operation& op = ops[0];
            op.mode = primitive_type::triangle_strip;
            op.idx_type = index_type(var);
            op.first = 0;
            op.count = index_count(var);
            op.primitive_restart_index = unsigned(vertex_count());
            op.primitive_restart = true;
            op.cw_face_winding = true;
        } else {
            span_size_t step = 2 * (_rings + 1);
            for(const auto s : integer_range(_sections)) {
                draw_operation& op = ops[s];
                op.mode = primitive_type::triangle_strip;
                op.idx_type = index_type(var);
                op.first = s * step;
                op.count = step;
                op.primitive_restart = false;
                op.cw_face_winding = true;
            }
        }
    } else if(var == 1) {
        if(primitive_restart()) {
            draw_operation& op = ops[0];
            op.mode = primitive_type::line_loop;
            op.idx_type = index_type(var);
            op.first = 0;
            op.count = index_count(var);
            op.primitive_restart_index = unsigned(vertex_count());
            op.primitive_restart = true;
        } else {
            span_size_t k = 0;
            for(const auto s : integer_range(_sections)) {
                draw_operation& op = ops[k++];
                op.mode = primitive_type::line_loop;
                op.idx_type = index_type(var);
                op.first = s * _rings;
                op.count = _rings;
                op.primitive_restart = false;
            }
            for(const auto r : integer_range(_rings)) {
                draw_operation& op = ops[k++];
                op.mode = primitive_type::line_loop;
                op.idx_type = index_type(var);
                op.first = r * _sections;
                op.count = _sections;
                op.primitive_restart = false;
            }
        }
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto unit_torus_gen::bounding_sphere() -> math::sphere<float, true> {
    return {{0.0F}, 0.5F};
}
//------------------------------------------------------------------------------
} // namespace shapes
} // namespace eagine

#ifdef __clang__
EAGINE_DIAG_POP()
#endif
