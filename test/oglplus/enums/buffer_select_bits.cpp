//  File test/oglplus/enums/buffer_select_bits.cpp
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
#define BOOST_TEST_MODULE OGLPLUS_buffer_select_bits

#include <boost/test/unit_test.hpp>
#include "common.hpp"
#include <cstring>

BOOST_AUTO_TEST_SUITE(enum_buffer_select_bits)

BOOST_AUTO_TEST_CASE(enum_buffer_select_bits_values)
{
	using namespace oglplus;
	enum_values ev;
	(void)ev;
	buffer_select_bits x;
	(void)x;

#ifdef GL_COLOR_BUFFER_BIT
	x = ev.color_buffer_bit;
	BOOST_CHECK(x == ev.color_buffer_bit);
# ifdef GL_DEPTH_BUFFER_BIT
	BOOST_CHECK(x != ev.depth_buffer_bit);
# endif
# ifdef GL_STENCIL_BUFFER_BIT
	BOOST_CHECK(x != ev.stencil_buffer_bit);
# endif
#endif

#ifdef GL_DEPTH_BUFFER_BIT
	x = ev.depth_buffer_bit;
# ifdef GL_COLOR_BUFFER_BIT
	BOOST_CHECK(x != ev.color_buffer_bit);
# endif
	BOOST_CHECK(x == ev.depth_buffer_bit);
# ifdef GL_STENCIL_BUFFER_BIT
	BOOST_CHECK(x != ev.stencil_buffer_bit);
# endif
#endif

#ifdef GL_STENCIL_BUFFER_BIT
	x = ev.stencil_buffer_bit;
# ifdef GL_COLOR_BUFFER_BIT
	BOOST_CHECK(x != ev.color_buffer_bit);
# endif
# ifdef GL_DEPTH_BUFFER_BIT
	BOOST_CHECK(x != ev.depth_buffer_bit);
# endif
	BOOST_CHECK(x == ev.stencil_buffer_bit);
#endif
}

BOOST_AUTO_TEST_CASE(enum_buffer_select_bits_names)
{
	using namespace oglplus;
	enum_values ev;
	(void)ev;
	buffer_select_bits x;
	(void)x;

#ifdef GL_COLOR_BUFFER_BIT
	x = ev.color_buffer_bit;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"COLOR_BUFFER_BIT"
	) == 0);
#endif

#ifdef GL_DEPTH_BUFFER_BIT
	x = ev.depth_buffer_bit;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"DEPTH_BUFFER_BIT"
	) == 0);
#endif

#ifdef GL_STENCIL_BUFFER_BIT
	x = ev.stencil_buffer_bit;
	BOOST_ASSERT(enum_value_name(x).data() != nullptr);
	BOOST_CHECK(std::strcmp(
		enum_value_name(x).data(),
		"STENCIL_BUFFER_BIT"
	) == 0);
#endif
}

BOOST_AUTO_TEST_SUITE_END()
