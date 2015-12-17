/**
 *  @file eagine/tagged_quantity.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_TAGGED_QUANTITY_1509260923_HPP
#define EAGINE_TAGGED_QUANTITY_1509260923_HPP

#include "type_traits.hpp"
#include <cmath>

namespace eagine {

template <typename T, typename U>
class tagged_quantity
{
private:
	static_assert(std::is_arithmetic<T>::value, "");
	T _v;

	template <typename, typename>
	friend class tagged_quantity;
public:
	tagged_quantity(void) = default;

	explicit constexpr inline
	tagged_quantity(T v)
	noexcept
	 : _v(v)
	{ }

	template <
		typename X,
		typename = std::enable_if_t<std::is_convertible<X, T>::value>
	>
	constexpr inline
	tagged_quantity(tagged_quantity<X, U> tq)
	noexcept
	 : _v(tq._v)
	{ }

	T value(void) const
	noexcept
	{
		return _v;
	}

	template <
		typename X,
		typename = std::enable_if_t<std::is_convertible<T, X>::value>
	>
	explicit constexpr inline
	operator X (void) const
	noexcept
	{
		return X(_v);
	}

	friend
	tagged_quantity operator + (tagged_quantity a, tagged_quantity b)
	noexcept
	{
		return tagged_quantity{a._v + b._v};
	}

	friend
	tagged_quantity operator - (tagged_quantity a, tagged_quantity b)
	noexcept
	{
		return tagged_quantity{a._v - b._v};
	}

	template <
		typename X,
		typename = std::enable_if_t<std::is_convertible<X, T>::value>
	>
	constexpr inline
	bool operator == (tagged_quantity<X, U> q) const
	noexcept
	{
		return _v == q._v;
	}

	template <
		typename X,
		typename = std::enable_if_t<std::is_convertible<X, T>::value>
	>
	constexpr inline
	bool operator != (tagged_quantity<X, U> q) const
	noexcept
	{
		return _v != q._v;
	}

	template <
		typename X,
		typename = std::enable_if_t<std::is_convertible<X, T>::value>
	>
	constexpr inline
	bool operator <  (tagged_quantity<X, U> q) const
	noexcept
	{
		return _v <  q._v;
	}

	template <
		typename X,
		typename = std::enable_if_t<std::is_convertible<X, T>::value>
	>
	constexpr inline
	bool operator <= (tagged_quantity<X, U> q) const
	noexcept
	{
		return _v <= q._v;
	}

	template <
		typename X,
		typename = std::enable_if_t<std::is_convertible<X, T>::value>
	>
	constexpr inline
	bool operator >  (tagged_quantity<X, U> q) const
	noexcept
	{
		return _v >  q._v;
	}

	template <
		typename X,
		typename = std::enable_if_t<std::is_convertible<X, T>::value>
	>
	constexpr inline
	bool operator >= (tagged_quantity<X, U> q) const
	noexcept
	{
		return _v >= q._v;
	}

};

} // namespace eagine

#endif // include guard
