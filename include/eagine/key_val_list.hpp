/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_KEY_VAL_LIST_HPP
#define EAGINE_KEY_VAL_LIST_HPP

#include "span.hpp"
#include <array>
#include <type_traits>
#include <utility>
#include <vector>

namespace eagine {

template <typename Traits, std::size_t N = 0>
class key_value_list;

template <typename Traits>
struct key_value_list_element {
    using key_type = typename Traits::key_type;
    using value_type = typename Traits::value_type;

    key_type _key;
    value_type _value;

    constexpr key_value_list_element(key_type key, value_type value) noexcept
      : _key{key}
      , _value{value} {}
};

template <typename Traits, std::size_t N>
struct key_value_list_base;

template <typename Traits>
struct key_value_list_base<Traits, 0> {
    using value_type = typename Traits::value_type;

    key_value_list_base() = default;

    static auto data() noexcept -> const value_type* {
        static const value_type term = Traits::terminator();
        return &term;
    }
};

template <typename Traits>
struct key_value_list_base<Traits, 2> {
    std::array<typename Traits::value_type, 3> _elements;

    using key_type = typename Traits::key_type;
    using conv_type = typename Traits::conv_type;
    using value_type = typename Traits::value_type;

    constexpr key_value_list_base(key_type key, value_type value) noexcept
      : _elements{{value_type(conv_type(key)), value, Traits::terminator()}} {}

    constexpr key_value_list_base(
      const key_value_list_base<Traits, 0>&,
      key_type key,
      value_type value,
      std::index_sequence<>) noexcept
      : _elements{{value_type(conv_type(key)), value, Traits::terminator()}} {}

    auto data() const noexcept -> const value_type* {
        return _elements.data();
    }
};

template <typename Traits, std::size_t N>
struct key_value_list_base {
    std::array<typename Traits::value_type, N + 1> _elements;

    using key_type = typename Traits::key_type;
    using conv_type = typename Traits::conv_type;
    using value_type = typename Traits::value_type;

    template <
      std::size_t M,
      std::size_t... I,
      typename = std::enable_if_t<(M + 2 == N) && (sizeof...(I) == M)>>
    constexpr key_value_list_base(
      const key_value_list_base<Traits, M>& head,
      key_type key,
      value_type value,
      std::index_sequence<I...>) noexcept
      : _elements{
          {head._elements[I]...,
           value_type(conv_type(key)),
           value,
           Traits::terminator()}} {}

    auto data() const noexcept -> const value_type* {
        return _elements.data();
    }
};

template <typename Traits, std::size_t N>
class key_value_list {
public:
    using key_type = typename Traits::key_type;
    using value_type = typename Traits::value_type;

private:
    key_value_list_base<Traits, N> _base;

public:
    key_value_list() = default;

    template <std::size_t M, typename = std::enable_if_t<M + 2 == N>>
    constexpr key_value_list(
      const key_value_list_base<Traits, M>& head,
      key_type key,
      value_type value) noexcept
      : _base(head, key, value, std::make_index_sequence<M>()) {}

    key_value_list(const key_value_list_element<Traits>& head) noexcept
      : _base(head._key, head._value) {}

    static constexpr auto size() noexcept -> span_size_t {
        return span_size(N + 1);
    }

    auto data() const noexcept -> const value_type* {
        return _base.data();
    }

    auto copy() const noexcept -> std::vector<value_type> {
        return {data(), data() + size()};
    }

    auto get() const noexcept -> span<const value_type> {
        return {data(), size()};
    }

    constexpr auto
    append(const key_value_list_element<Traits>& key_val) const noexcept
      -> key_value_list<Traits, N + 2> {
        return {_base, key_val._key, key_val._value};
    }
};

template <typename Traits>
static constexpr auto operator+(
  const key_value_list_element<Traits>& l,
  const key_value_list_element<Traits>& r) noexcept
  -> key_value_list<Traits, 4> {
    return key_value_list<Traits, 2>(l).append(r);
}

template <typename Traits, std::size_t N>
static constexpr auto operator+(
  const key_value_list<Traits, N>& l,
  const key_value_list_element<Traits>& r) noexcept
  -> key_value_list<Traits, N + 2> {
    return l.append(r);
}

} // namespace eagine

#endif // EAGINE_KEY_VAL_LIST_HPP
