//  File test/oglplus/enums/transform_feedback_primitive_type.cpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the enumeration source files in 'source/enums/oglplus'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE OGLPLUS_transform_feedback_primitive_type

#include <boost/test/unit_test.hpp>
#include "common.hpp"
#include <cstring>

BOOST_AUTO_TEST_SUITE(enum_transform_feedback_primitive_type)

BOOST_AUTO_TEST_CASE(enum_transform_feedback_primitive_type_values)
{
	using namespace oglplus;
	enum_values ev;
	(void)ev;
	transform_feedback_primitive_type x;
	(void)x;

#ifdef GL_LINES
	x = ev.lines;
	BOOST_CHECK(x == ev.lines);
# ifdef GL_POINTS
	BOOST_CHECK(x != ev.points);
# endif
# ifdef GL_TRIANGLES
	BOOST_CHECK(x != ev.triangles);
# endif
#endif

#ifdef GL_POINTS
	x = ev.points;
# ifdef GL_LINES
	BOOST_CHECK(x != ev.lines);
# endif
	BOOST_CHECK(x == ev.points);
# ifdef GL_TRIANGLES
	BOOST_CHECK(x != ev.triangles);
# endif
#endif

#ifdef GL_TRIANGLES
	x = ev.triangles;
# ifdef GL_LINES
	BOOST_CHECK(x != ev.lines);
# endif
# ifdef GL_POINTS
	BOOST_CHECK(x != ev.points);
# endif
	BOOST_CHECK(x == ev.triangles);
#endif
}

BOOST_AUTO_TEST_CASE(enum_transform_feedback_primitive_type_names)
{
	using namespace oglplus;
	enum_values ev;
	(void)ev;
	transform_feedback_primitive_type x;
	(void)x;

#ifdef GL_LINES
	x = ev.lines;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"LINES"
	) == 0);
#endif

#ifdef GL_POINTS
	x = ev.points;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"POINTS"
	) == 0);
#endif

#ifdef GL_TRIANGLES
	x = ev.triangles;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"TRIANGLES"
	) == 0);
#endif
}

BOOST_AUTO_TEST_SUITE_END()
