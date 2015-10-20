/**
 *  @file oalplus/enum_value_range_alut.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef OALPLUS_ENUM_VALUE_RANGE_ALUT_1509260923_HPP
#define OALPLUS_ENUM_VALUE_RANGE_ALUT_1509260923_HPP

#include "enum_value_range.hpp"

namespace oalplus {

std::pair<const void*, std::size_t>
get_enum_value_range(const any_enum_class_alut&)
noexcept;

template <typename EnumClass, typename T, unsigned EnumId>
static inline
array_view<const T>
enum_value_range(enum_class_alut<EnumClass, T, EnumId> cls)
noexcept
{
	auto p = ::oalplus::get_enum_value_range(cls);
	return {static_cast<const T*>(p.first), p.second};
}

} // namespace oalplus

#if !OALPLUS_LINK_LIBRARY || defined(OALPLUS_IMPLEMENTING_LIBRARY)
#include <oalplus/enum_value_range_alut.inl>
#endif

#endif // include guard
