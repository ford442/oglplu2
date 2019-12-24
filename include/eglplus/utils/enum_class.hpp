/**
 *  @file eglplus/utils/enum_class.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EGLPLUS_UTILS_ENUM_CLASS_HPP
#define EGLPLUS_UTILS_ENUM_CLASS_HPP

#include <eagine/enum_bitfield.hpp>
#include <eagine/enum_class.hpp>

namespace eglplus {

using eagine::enum_value;

template <typename Self, typename T, eagine::identifier_t Id>
using enum_class = eagine::enum_class<Self, T, 100, Id>;

using eagine::is_enum_class;

using eagine::enum_bitfield;

using any_enum_class = eagine::any_enum_class<100>;
using any_enum_value = eagine::any_enum_value<100>;

static constexpr inline bool same_enum_class(
  any_enum_class a, any_enum_class b) noexcept {
    return ::eagine::same_enum_class(a, b);
}

} // namespace eglplus

#endif // EGLPLUS_UTILS_ENUM_CLASS_HPP
