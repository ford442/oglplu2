/**
 *  @file oglplus/math/vector.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef OGLPLUS_MATH_VECTOR_HPP
#define OGLPLUS_MATH_VECTOR_HPP

#include "config.hpp"
#include <eagine/math/tvec.hpp>

namespace eagine {
namespace oglp {

using math::vector;

template <typename T, unsigned N, bool V = math_use_simd>
using tvec = math::tvec<T, N, V>;

using vec2 = tvec<GLfloat, 2>;

using vec3 = tvec<GLfloat, 3>;

using vec4 = tvec<GLfloat, 4>;

} // namespace oglp
} // namespace eagine

#endif // OGLPLUS_MATH_VECTOR_HPP
