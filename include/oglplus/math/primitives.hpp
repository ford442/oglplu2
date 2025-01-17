/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#ifndef OGLPLUS_MATH_PRIMITIVES_HPP
#define OGLPLUS_MATH_PRIMITIVES_HPP

#include "config.hpp"
#include <eagine/math/primitives.hpp>
#include <oglplus/gl_api/config.hpp>

namespace eagine::oglp {

/// @brief Alias for math::line instantiation.
/// @ingroup gl_utils
using line = math::line<gl_types::float_type, math_use_simd>;

/// @brief Alias for math::sphere instantiation.
/// @ingroup gl_utils
using sphere = math::sphere<gl_types::float_type, math_use_simd>;

/// @brief Alias for math::triangle instantiation.
/// @ingroup gl_utils
using triangle = math::triangle<gl_types::float_type, math_use_simd>;

} // namespace eagine::oglp

#endif // OGLPLUS_MATH_PRIMITIVES_HPP
