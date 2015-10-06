//  File test/oglplus/enums/tess_gen_primitive_spacing.cpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the enumeration source files in 'source/enums/oglplus'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE OGLPLUS_tess_gen_primitive_spacing

#include <boost/test/unit_test.hpp>
#include "common.hpp"
#include <cstring>

BOOST_AUTO_TEST_SUITE(enum_tess_gen_primitive_spacing)

BOOST_AUTO_TEST_CASE(enum_tess_gen_primitive_spacing_values)
{
	using namespace oglplus;
	enum_values ev;
	(void)ev;
	tess_gen_primitive_spacing x;
	(void)x;

#ifdef GL_EQUAL
	x = ev.equal;
	BOOST_CHECK(x == ev.equal);
# ifdef GL_FRACTIONAL_EVEN
	BOOST_CHECK(x != ev.fractional_even);
# endif
# ifdef GL_FRACTIONAL_ODD
	BOOST_CHECK(x != ev.fractional_odd);
# endif
#endif

#ifdef GL_FRACTIONAL_EVEN
	x = ev.fractional_even;
# ifdef GL_EQUAL
	BOOST_CHECK(x != ev.equal);
# endif
	BOOST_CHECK(x == ev.fractional_even);
# ifdef GL_FRACTIONAL_ODD
	BOOST_CHECK(x != ev.fractional_odd);
# endif
#endif

#ifdef GL_FRACTIONAL_ODD
	x = ev.fractional_odd;
# ifdef GL_EQUAL
	BOOST_CHECK(x != ev.equal);
# endif
# ifdef GL_FRACTIONAL_EVEN
	BOOST_CHECK(x != ev.fractional_even);
# endif
	BOOST_CHECK(x == ev.fractional_odd);
#endif
}

BOOST_AUTO_TEST_CASE(enum_tess_gen_primitive_spacing_names)
{
	using namespace oglplus;
	enum_values ev;
	(void)ev;
	tess_gen_primitive_spacing x;
	(void)x;

#ifdef GL_EQUAL
	x = ev.equal;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"EQUAL"
	) == 0);
#endif

#ifdef GL_FRACTIONAL_EVEN
	x = ev.fractional_even;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"FRACTIONAL_EVEN"
	) == 0);
#endif

#ifdef GL_FRACTIONAL_ODD
	x = ev.fractional_odd;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"FRACTIONAL_ODD"
	) == 0);
#endif
}

BOOST_AUTO_TEST_CASE(enum_tess_gen_primitive_spacing_range)
{
	using namespace oglplus;
	tess_gen_primitive_spacing x;
	(void)x;
	auto count = enum_value_range(x).size();

#ifdef GL_EQUAL
{
	--count;
	array_view<const GLenum> r = enum_value_range(x);
	BOOST_CHECK(std::find(
		r.begin(), r.end(),
		GL_EQUAL
	) != r.end());
}
#endif

#ifdef GL_FRACTIONAL_EVEN
{
	--count;
	array_view<const GLenum> r = enum_value_range(x);
	BOOST_CHECK(std::find(
		r.begin(), r.end(),
		GL_FRACTIONAL_EVEN
	) != r.end());
}
#endif

#ifdef GL_FRACTIONAL_ODD
{
	--count;
	array_view<const GLenum> r = enum_value_range(x);
	BOOST_CHECK(std::find(
		r.begin(), r.end(),
		GL_FRACTIONAL_ODD
	) != r.end());
}
#endif
	BOOST_CHECK_EQUAL(count, 0);
}

BOOST_AUTO_TEST_SUITE_END()
