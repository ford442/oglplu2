/**
 *  @file oglplus/utils/limited_value.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef OGLPLUS_UTILS_LIMITED_VALUE_1509260923_HPP
#define OGLPLUS_UTILS_LIMITED_VALUE_1509260923_HPP

#include "gl_func.hpp"
#include "indexed_enum.hpp"
#include "identity.hpp"
#include "../error/handling.hpp"
#include "../error/outcome.hpp"
#include "../enum/types.hpp"
#include <eagine/iterator.hpp>

namespace oglplus {

template <GLenum Query, typename T>
struct limited_value
{
	T _value;

	limited_value(void) = default;

	constexpr
	limited_value(T value)
	noexcept
	 : _value(value)
	{ }

	explicit constexpr
	operator T (void) const
	noexcept
	{
		return _value;
	}
};

template <GLenum Query, GLenum Base>
struct limited_value<Query, indexed_enum_value<Base>>
 : indexed_enum_value<Base>
{
	limited_value(void) = default;

	constexpr
	limited_value(indexed_enum_value<Base> iev)
	noexcept
	 : indexed_enum_value<Base>(iev)
	{ }

	constexpr
	limited_value(indexed_enum_base<Base> ieb)
	noexcept
	 : indexed_enum_value<Base>(ieb)
	{ }

	constexpr explicit
	limited_value(GLenum value)
	noexcept
	 : indexed_enum_value<Base>(value)
	{ }
};

template <typename X>
struct is_limited_value
 : std::false_type
{ };

template <GLenum Query, typename T>
struct is_limited_value<limited_value<Query, T>>
 : std::true_type
{ };

template <
	GLenum Query,
	typename T,
	typename = typename std::enable_if<
		std::is_integral<T>::value
	>::type
>
static inline
outcome<T>
get_limit(identity<limited_value<Query, T>>)
noexcept
{
	GLint value = 0;
	OGLPLUS_GLFUNC(GetIntegerv)(Query, &value);
	OGLPLUS_VERIFY(
		GetIntegerv,
		gl_enum_value(limit_query(Query)),
		debug
	);
	return {T(value)};
}

template <GLenum Query, GLenum Base>
static inline
outcome<indexed_enum_value<Base>>
get_limit(identity<limited_value<Query, indexed_enum_value<Base>>>)
noexcept
{
	return outcome_cast<indexed_enum_value<Base>>(
		get_limit(identity<limited_value<Query, GLenum>>())|
		[](GLenum& val) { val += Base; }
	);
}

template <
	typename LimitedValue,
	typename = typename std::enable_if<
		is_limited_value<LimitedValue>::value
	>::type
>
static inline
auto limit(void)
noexcept
{
	return get_limit(identity<LimitedValue>());
}

template <GLenum Query, typename T>
static inline
bool exceeds_limit(limited_value<Query, T> lv, T limit)
noexcept
{
	return T(lv) >= limit;
}

template <GLenum Query, GLenum Base>
static inline
bool exceeds_limit(
	limited_value<Query, indexed_enum_value<Base>> lv,
	indexed_enum_value<Base> limit
) noexcept
{
	return lv.index() >= limit.index();
}

template <GLenum Query, typename T>
static inline
outcome<void>
check_below_limit(limited_value<Query, T> lv)
noexcept
{
	outcome<T> lim = get_limit(make_identity(lv));
	if(lim.failed())
	{
		return lim.release_handler();
	}
	if(exceeds_limit(lv, lim.get()))
	{
		return deferred_error_handler(std::move(
			error_info(GL_INVALID_VALUE)
				.gl_enum_value(limit_query(Query))
				.source_file(__FILE__)
				.source_line(__LINE__)
		));
	}
	return {};
}

template <typename LimitedValue>
class limited_value_range;

template <GLenum Query, GLenum Base>
class limited_value_range<limited_value<Query, indexed_enum_value<Base>>>
{
private:
	GLenum _limit;

	typedef limited_value<Query, indexed_enum_value<Base>> _lv_t;

	static
	_lv_t _wrap_enum(GLenum e)
	noexcept
	{
		return _lv_t{e};
	}
	
public:
	limited_value_range(_lv_t limit)
	noexcept
	 : _limit(GLenum(limit))
	{ }

	limited_value_range(void)
	 : limited_value_range(get_limit(identity<_lv_t>()).get())
	{ }

	typedef eagine::transforming_iterator<
		eagine::selfref_iterator<GLenum>,
		_lv_t, _lv_t(*)(GLenum) noexcept
	> iterator;

	iterator begin(void) const
	noexcept
	{
		return iterator(Base, &_wrap_enum);
	}

	iterator end(void) const
	noexcept
	{
		return iterator(_limit, &_wrap_enum);
	}
};

} // namespace oglplus

#endif // include guard
