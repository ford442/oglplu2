/**
 *  @file eagine/shapes/drawing.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_SHAPES_DRAWING_HPP
#define EAGINE_SHAPES_DRAWING_HPP

#include "../reflect/map_enumerators.hpp"
#include "../types.hpp"

namespace eagine {
namespace shapes {
//------------------------------------------------------------------------------
enum class primitive_type {
    points,
    lines,
    line_strip,
    line_loop,
    triangles,
    triangle_strip,
    triangle_fan,
    quads,
    tetrahedrons,
    patches
};
//------------------------------------------------------------------------------
template <typename Selector>
constexpr auto enumerator_mapping(identity<primitive_type>, Selector) noexcept {
    return enumerator_map_type<primitive_type, 10>{
      {{"points", primitive_type::points},
       {"lines", primitive_type::lines},
       {"line_strip", primitive_type::line_strip},
       {"line_loop", primitive_type::line_loop},
       {"triangles", primitive_type::triangles},
       {"triangle_strip", primitive_type::triangle_strip},
       {"triangle_fan", primitive_type::triangle_fan},
       {"quads", primitive_type::quads},
       {"tetrahedrons", primitive_type::tetrahedrons},
       {"patches", primitive_type::patches}}};
}
//------------------------------------------------------------------------------
enum class attrib_data_type { none, float_ };
//------------------------------------------------------------------------------
enum class index_data_type {
    none = 0,
    unsigned_8 = 8,
    unsigned_16 = 16,
    unsigned_32 = 32
};
//------------------------------------------------------------------------------
static inline bool operator<(index_data_type l, index_data_type r) noexcept {
    using UT = std::underlying_type_t<index_data_type>;
    return UT(l) < UT(r);
}
//------------------------------------------------------------------------------
static inline bool operator>(index_data_type l, index_data_type r) noexcept {
    using UT = std::underlying_type_t<index_data_type>;
    return UT(l) > UT(r);
}
//------------------------------------------------------------------------------
static inline bool operator<=(index_data_type l, index_data_type r) noexcept {
    using UT = std::underlying_type_t<index_data_type>;
    return UT(l) <= UT(r);
}
//------------------------------------------------------------------------------
static inline bool operator>=(index_data_type l, index_data_type r) noexcept {
    using UT = std::underlying_type_t<index_data_type>;
    return UT(l) >= UT(r);
}
//------------------------------------------------------------------------------
struct draw_operation {
    primitive_type mode{primitive_type::points};
    index_data_type idx_type{index_data_type::none};
    span_size_t first{0};
    span_size_t count{0};
    std::uint32_t phase{0};
    std::uint32_t primitive_restart_index{0};
    std::uint16_t patch_vertices{3};
    bool primitive_restart : 1;
    bool cw_face_winding : 1;

    constexpr draw_operation() noexcept
      : primitive_restart(false)
      , cw_face_winding(false) {
    }
};
//------------------------------------------------------------------------------
} // namespace shapes
} // namespace eagine

#endif // EAGINE_SHAPES_DRAWING_HPP
