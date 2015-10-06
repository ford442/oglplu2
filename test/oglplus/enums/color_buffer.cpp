//  File test/oglplus/enums/color_buffer.cpp
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
#define BOOST_TEST_MODULE OGLPLUS_color_buffer

#include <boost/test/unit_test.hpp>
#include "common.hpp"
#include <cstring>

BOOST_AUTO_TEST_SUITE(enum_color_buffer)

BOOST_AUTO_TEST_CASE(enum_color_buffer_values)
{
	using namespace oglplus;
	enum_values ev;
	(void)ev;
	color_buffer x;
	(void)x;

#ifdef GL_BACK
	x = ev.back;
	BOOST_CHECK(x == ev.back);
# ifdef GL_BACK_LEFT
	BOOST_CHECK(x != ev.back_left);
# endif
# ifdef GL_BACK_RIGHT
	BOOST_CHECK(x != ev.back_right);
# endif
# ifdef GL_FRONT
	BOOST_CHECK(x != ev.front);
# endif
# ifdef GL_FRONT_AND_BACK
	BOOST_CHECK(x != ev.front_and_back);
# endif
# ifdef GL_FRONT_LEFT
	BOOST_CHECK(x != ev.front_left);
# endif
# ifdef GL_FRONT_RIGHT
	BOOST_CHECK(x != ev.front_right);
# endif
# ifdef GL_LEFT
	BOOST_CHECK(x != ev.left);
# endif
# ifdef GL_NONE
	BOOST_CHECK(x != ev.none);
# endif
# ifdef GL_RIGHT
	BOOST_CHECK(x != ev.right);
# endif
#endif

#ifdef GL_BACK_LEFT
	x = ev.back_left;
# ifdef GL_BACK
	BOOST_CHECK(x != ev.back);
# endif
	BOOST_CHECK(x == ev.back_left);
# ifdef GL_BACK_RIGHT
	BOOST_CHECK(x != ev.back_right);
# endif
# ifdef GL_FRONT
	BOOST_CHECK(x != ev.front);
# endif
# ifdef GL_FRONT_AND_BACK
	BOOST_CHECK(x != ev.front_and_back);
# endif
# ifdef GL_FRONT_LEFT
	BOOST_CHECK(x != ev.front_left);
# endif
# ifdef GL_FRONT_RIGHT
	BOOST_CHECK(x != ev.front_right);
# endif
# ifdef GL_LEFT
	BOOST_CHECK(x != ev.left);
# endif
# ifdef GL_NONE
	BOOST_CHECK(x != ev.none);
# endif
# ifdef GL_RIGHT
	BOOST_CHECK(x != ev.right);
# endif
#endif

#ifdef GL_BACK_RIGHT
	x = ev.back_right;
# ifdef GL_BACK
	BOOST_CHECK(x != ev.back);
# endif
# ifdef GL_BACK_LEFT
	BOOST_CHECK(x != ev.back_left);
# endif
	BOOST_CHECK(x == ev.back_right);
# ifdef GL_FRONT
	BOOST_CHECK(x != ev.front);
# endif
# ifdef GL_FRONT_AND_BACK
	BOOST_CHECK(x != ev.front_and_back);
# endif
# ifdef GL_FRONT_LEFT
	BOOST_CHECK(x != ev.front_left);
# endif
# ifdef GL_FRONT_RIGHT
	BOOST_CHECK(x != ev.front_right);
# endif
# ifdef GL_LEFT
	BOOST_CHECK(x != ev.left);
# endif
# ifdef GL_NONE
	BOOST_CHECK(x != ev.none);
# endif
# ifdef GL_RIGHT
	BOOST_CHECK(x != ev.right);
# endif
#endif

#ifdef GL_FRONT
	x = ev.front;
# ifdef GL_BACK
	BOOST_CHECK(x != ev.back);
# endif
# ifdef GL_BACK_LEFT
	BOOST_CHECK(x != ev.back_left);
# endif
# ifdef GL_BACK_RIGHT
	BOOST_CHECK(x != ev.back_right);
# endif
	BOOST_CHECK(x == ev.front);
# ifdef GL_FRONT_AND_BACK
	BOOST_CHECK(x != ev.front_and_back);
# endif
# ifdef GL_FRONT_LEFT
	BOOST_CHECK(x != ev.front_left);
# endif
# ifdef GL_FRONT_RIGHT
	BOOST_CHECK(x != ev.front_right);
# endif
# ifdef GL_LEFT
	BOOST_CHECK(x != ev.left);
# endif
# ifdef GL_NONE
	BOOST_CHECK(x != ev.none);
# endif
# ifdef GL_RIGHT
	BOOST_CHECK(x != ev.right);
# endif
#endif

#ifdef GL_FRONT_AND_BACK
	x = ev.front_and_back;
# ifdef GL_BACK
	BOOST_CHECK(x != ev.back);
# endif
# ifdef GL_BACK_LEFT
	BOOST_CHECK(x != ev.back_left);
# endif
# ifdef GL_BACK_RIGHT
	BOOST_CHECK(x != ev.back_right);
# endif
# ifdef GL_FRONT
	BOOST_CHECK(x != ev.front);
# endif
	BOOST_CHECK(x == ev.front_and_back);
# ifdef GL_FRONT_LEFT
	BOOST_CHECK(x != ev.front_left);
# endif
# ifdef GL_FRONT_RIGHT
	BOOST_CHECK(x != ev.front_right);
# endif
# ifdef GL_LEFT
	BOOST_CHECK(x != ev.left);
# endif
# ifdef GL_NONE
	BOOST_CHECK(x != ev.none);
# endif
# ifdef GL_RIGHT
	BOOST_CHECK(x != ev.right);
# endif
#endif

#ifdef GL_FRONT_LEFT
	x = ev.front_left;
# ifdef GL_BACK
	BOOST_CHECK(x != ev.back);
# endif
# ifdef GL_BACK_LEFT
	BOOST_CHECK(x != ev.back_left);
# endif
# ifdef GL_BACK_RIGHT
	BOOST_CHECK(x != ev.back_right);
# endif
# ifdef GL_FRONT
	BOOST_CHECK(x != ev.front);
# endif
# ifdef GL_FRONT_AND_BACK
	BOOST_CHECK(x != ev.front_and_back);
# endif
	BOOST_CHECK(x == ev.front_left);
# ifdef GL_FRONT_RIGHT
	BOOST_CHECK(x != ev.front_right);
# endif
# ifdef GL_LEFT
	BOOST_CHECK(x != ev.left);
# endif
# ifdef GL_NONE
	BOOST_CHECK(x != ev.none);
# endif
# ifdef GL_RIGHT
	BOOST_CHECK(x != ev.right);
# endif
#endif

#ifdef GL_FRONT_RIGHT
	x = ev.front_right;
# ifdef GL_BACK
	BOOST_CHECK(x != ev.back);
# endif
# ifdef GL_BACK_LEFT
	BOOST_CHECK(x != ev.back_left);
# endif
# ifdef GL_BACK_RIGHT
	BOOST_CHECK(x != ev.back_right);
# endif
# ifdef GL_FRONT
	BOOST_CHECK(x != ev.front);
# endif
# ifdef GL_FRONT_AND_BACK
	BOOST_CHECK(x != ev.front_and_back);
# endif
# ifdef GL_FRONT_LEFT
	BOOST_CHECK(x != ev.front_left);
# endif
	BOOST_CHECK(x == ev.front_right);
# ifdef GL_LEFT
	BOOST_CHECK(x != ev.left);
# endif
# ifdef GL_NONE
	BOOST_CHECK(x != ev.none);
# endif
# ifdef GL_RIGHT
	BOOST_CHECK(x != ev.right);
# endif
#endif

#ifdef GL_LEFT
	x = ev.left;
# ifdef GL_BACK
	BOOST_CHECK(x != ev.back);
# endif
# ifdef GL_BACK_LEFT
	BOOST_CHECK(x != ev.back_left);
# endif
# ifdef GL_BACK_RIGHT
	BOOST_CHECK(x != ev.back_right);
# endif
# ifdef GL_FRONT
	BOOST_CHECK(x != ev.front);
# endif
# ifdef GL_FRONT_AND_BACK
	BOOST_CHECK(x != ev.front_and_back);
# endif
# ifdef GL_FRONT_LEFT
	BOOST_CHECK(x != ev.front_left);
# endif
# ifdef GL_FRONT_RIGHT
	BOOST_CHECK(x != ev.front_right);
# endif
	BOOST_CHECK(x == ev.left);
# ifdef GL_NONE
	BOOST_CHECK(x != ev.none);
# endif
# ifdef GL_RIGHT
	BOOST_CHECK(x != ev.right);
# endif
#endif

#ifdef GL_NONE
	x = ev.none;
# ifdef GL_BACK
	BOOST_CHECK(x != ev.back);
# endif
# ifdef GL_BACK_LEFT
	BOOST_CHECK(x != ev.back_left);
# endif
# ifdef GL_BACK_RIGHT
	BOOST_CHECK(x != ev.back_right);
# endif
# ifdef GL_FRONT
	BOOST_CHECK(x != ev.front);
# endif
# ifdef GL_FRONT_AND_BACK
	BOOST_CHECK(x != ev.front_and_back);
# endif
# ifdef GL_FRONT_LEFT
	BOOST_CHECK(x != ev.front_left);
# endif
# ifdef GL_FRONT_RIGHT
	BOOST_CHECK(x != ev.front_right);
# endif
# ifdef GL_LEFT
	BOOST_CHECK(x != ev.left);
# endif
	BOOST_CHECK(x == ev.none);
# ifdef GL_RIGHT
	BOOST_CHECK(x != ev.right);
# endif
#endif

#ifdef GL_RIGHT
	x = ev.right;
# ifdef GL_BACK
	BOOST_CHECK(x != ev.back);
# endif
# ifdef GL_BACK_LEFT
	BOOST_CHECK(x != ev.back_left);
# endif
# ifdef GL_BACK_RIGHT
	BOOST_CHECK(x != ev.back_right);
# endif
# ifdef GL_FRONT
	BOOST_CHECK(x != ev.front);
# endif
# ifdef GL_FRONT_AND_BACK
	BOOST_CHECK(x != ev.front_and_back);
# endif
# ifdef GL_FRONT_LEFT
	BOOST_CHECK(x != ev.front_left);
# endif
# ifdef GL_FRONT_RIGHT
	BOOST_CHECK(x != ev.front_right);
# endif
# ifdef GL_LEFT
	BOOST_CHECK(x != ev.left);
# endif
# ifdef GL_NONE
	BOOST_CHECK(x != ev.none);
# endif
	BOOST_CHECK(x == ev.right);
#endif
}

BOOST_AUTO_TEST_CASE(enum_color_buffer_names)
{
	using namespace oglplus;
	enum_values ev;
	(void)ev;
	color_buffer x;
	(void)x;

#ifdef GL_BACK
	x = ev.back;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"BACK"
	) == 0);
#endif

#ifdef GL_BACK_LEFT
	x = ev.back_left;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"BACK_LEFT"
	) == 0);
#endif

#ifdef GL_BACK_RIGHT
	x = ev.back_right;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"BACK_RIGHT"
	) == 0);
#endif

#ifdef GL_FRONT
	x = ev.front;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"FRONT"
	) == 0);
#endif

#ifdef GL_FRONT_AND_BACK
	x = ev.front_and_back;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"FRONT_AND_BACK"
	) == 0);
#endif

#ifdef GL_FRONT_LEFT
	x = ev.front_left;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"FRONT_LEFT"
	) == 0);
#endif

#ifdef GL_FRONT_RIGHT
	x = ev.front_right;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"FRONT_RIGHT"
	) == 0);
#endif

#ifdef GL_LEFT
	x = ev.left;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"LEFT"
	) == 0);
#endif

#ifdef GL_NONE
	x = ev.none;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"NONE"
	) == 0);
#endif

#ifdef GL_RIGHT
	x = ev.right;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"RIGHT"
	) == 0);
#endif
}

BOOST_AUTO_TEST_CASE(enum_color_buffer_range)
{
	using namespace oglplus;
	color_buffer x;
	(void)x;
	auto count = enum_value_range(x).size();

#ifdef GL_BACK
{
	--count;
	array_view<const GLenum> r = enum_value_range(x);
	BOOST_CHECK(std::find(
		r.begin(), r.end(),
		GL_BACK
	) != r.end());
}
#endif

#ifdef GL_BACK_LEFT
{
	--count;
	array_view<const GLenum> r = enum_value_range(x);
	BOOST_CHECK(std::find(
		r.begin(), r.end(),
		GL_BACK_LEFT
	) != r.end());
}
#endif

#ifdef GL_BACK_RIGHT
{
	--count;
	array_view<const GLenum> r = enum_value_range(x);
	BOOST_CHECK(std::find(
		r.begin(), r.end(),
		GL_BACK_RIGHT
	) != r.end());
}
#endif

#ifdef GL_FRONT
{
	--count;
	array_view<const GLenum> r = enum_value_range(x);
	BOOST_CHECK(std::find(
		r.begin(), r.end(),
		GL_FRONT
	) != r.end());
}
#endif

#ifdef GL_FRONT_AND_BACK
{
	--count;
	array_view<const GLenum> r = enum_value_range(x);
	BOOST_CHECK(std::find(
		r.begin(), r.end(),
		GL_FRONT_AND_BACK
	) != r.end());
}
#endif

#ifdef GL_FRONT_LEFT
{
	--count;
	array_view<const GLenum> r = enum_value_range(x);
	BOOST_CHECK(std::find(
		r.begin(), r.end(),
		GL_FRONT_LEFT
	) != r.end());
}
#endif

#ifdef GL_FRONT_RIGHT
{
	--count;
	array_view<const GLenum> r = enum_value_range(x);
	BOOST_CHECK(std::find(
		r.begin(), r.end(),
		GL_FRONT_RIGHT
	) != r.end());
}
#endif

#ifdef GL_LEFT
{
	--count;
	array_view<const GLenum> r = enum_value_range(x);
	BOOST_CHECK(std::find(
		r.begin(), r.end(),
		GL_LEFT
	) != r.end());
}
#endif

#ifdef GL_NONE
{
	--count;
	array_view<const GLenum> r = enum_value_range(x);
	BOOST_CHECK(std::find(
		r.begin(), r.end(),
		GL_NONE
	) != r.end());
}
#endif

#ifdef GL_RIGHT
{
	--count;
	array_view<const GLenum> r = enum_value_range(x);
	BOOST_CHECK(std::find(
		r.begin(), r.end(),
		GL_RIGHT
	) != r.end());
}
#endif
	BOOST_CHECK_EQUAL(count, 0);
}

BOOST_AUTO_TEST_SUITE_END()
