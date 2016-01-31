/**
 *  @file eagine/shapes/torus.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_SHAPES_TORUS_1509260923_HPP
#define EAGINE_SHAPES_TORUS_1509260923_HPP

#include "gen_base.hpp"
#include "../config/basic.hpp"
#include "../valid_if.hpp"
#include <cassert>

namespace eagine {
namespace shapes {

class unit_torus_gen
 : public generator_base
{
private:
	unsigned _rings;
	unsigned _sections;
	float _radius_ratio;

	static
	vertex_attrib_bits _attr_mask(void)
	noexcept;
public:
	unit_torus_gen(
		vertex_attrib_bits attr_bits,
		valid_if_greater_than<int, 4> rings,
		valid_if_greater_than<int, 3> sections,
		valid_if_ge0_lt1<float> radius_ratio
	) noexcept;

	unit_torus_gen(
		vertex_attrib_bits attr_bits,
		valid_if_greater_than<int, 4> rings,
		valid_if_greater_than<int, 3> sections
	) noexcept
	 : unit_torus_gen(attr_bits, rings, sections, 0.5f)
	{ }

	unit_torus_gen(vertex_attrib_bits attr_bits)
	noexcept
	 : unit_torus_gen(attr_bits, 24, 36, 0.5f)
	{ }

	bool cw_face_winding(void)
	override;

	unsigned vertex_count(void)
	override;

	void positions(span<float> dest)
	noexcept;

	void normals(span<float> dest)
	noexcept;

	void tangentials(span<float> dest)
	noexcept;

	void bitangentials(span<float> dest)
	noexcept;

	void box_coords(span<float> dest)
	noexcept;

	void wrap_coords(span<float> dest)
	noexcept;

	void attrib_values(vertex_attrib_kind attr, span<float> dest)
	override;

	index_data_type index_type(void)
	override;

	unsigned index_count(void)
	override;

	void indices(span<unsigned> dest)
	override;

	unsigned operation_count(void)
	override;

	void instructions(span<draw_operation> ops)
	override;
};

} // namespace shapes
} // namespace eagine

#if !EAGINE_LINK_LIBRARY || defined(EAGINE_IMPLEMENTING_LIBRARY)
#include <eagine/shapes/torus.inl>
#endif

#endif // include guard
