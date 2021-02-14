/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_TYPES_HPP
#define EAGINE_TYPES_HPP

#include "is_within_limits.hpp"
#include "type_identity.hpp"
#include "valid_if/nonnegative.hpp"
#include <cassert>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace eagine {

// byte type
using byte = unsigned char;

// alignment type used by std
using std_align_t = std::size_t;
// size type used by std
using std_size_t = std::size_t;
// signed span size type used by eagine
using span_size_t = std::ptrdiff_t;
// valid size type
using valid_span_size = valid_if_nonnegative<span_size_t>;

// convert to std alignment type
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
static constexpr auto std_align(T v) noexcept {
    return limit_cast<std_align_t>(v);
}

// convert to std size type
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
static constexpr auto std_size(T v) noexcept {
    return limit_cast<std_size_t>(v);
}

// convert to span size type
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
static constexpr auto span_size(T v) noexcept {
    return limit_cast<span_size_t>(v);
}

template <typename T>
static constexpr auto span_align_of(type_identity<T> = {}) noexcept {
    return span_size(alignof(T));
}

template <typename T>
static constexpr auto span_size_of(type_identity<T> = {}) noexcept {
    return span_size(sizeof(T));
}

template <typename T, typename S>
static constexpr auto span_size_of(S n, type_identity<T> = {}) noexcept {
    return span_size(sizeof(T)) * span_size(n);
}

} // namespace eagine

#endif // EAGINE_TYPES_HPP
