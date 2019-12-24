/**
 *  @file eglplus/enum/value_names.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef EGLPLUS_ENUM_VALUE_NAMES_HPP
#define EGLPLUS_ENUM_VALUE_NAMES_HPP

#include "../config/basic.hpp"
#include "../egl.hpp"
#include "../utils/enum_class.hpp"
#include "../utils/string_span.hpp"

namespace eglplus {

string_view get_enum_value_name(const any_enum_value&) noexcept;

template <typename EnumClass, typename T, eagine::identifier_t EnumId>
static inline string_view enum_value_name(
  enum_class<EnumClass, T, EnumId> val) noexcept {
    return ::eglplus::get_enum_value_name(val);
}

} // namespace eglplus

#if !OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)
#include <eglplus/enum/value_names.inl>
#endif

#endif // EGLPLUS_ENUM_VALUE_NAMES_HPP
