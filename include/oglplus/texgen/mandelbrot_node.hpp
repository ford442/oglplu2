/**
 *  @file oglplus/texgen/mandelbrot_node.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef OGLPLUS_TEXGEN_MANDELBROT_NODE_1509260923_HPP
#define OGLPLUS_TEXGEN_MANDELBROT_NODE_1509260923_HPP

#include "base_node.hpp"

namespace oglplus {
namespace texgen {

class mandelbrot_output
 : public base_output
{
public:
	mandelbrot_output(node_intf& parent);

	cstr_ref type_name(void)
	override;

	slot_data_type value_type(void)
	override;

	std::ostream& definitions(std::ostream& out, compile_context& ctxt)
	override;
};

class mandelbrot_node
 : public base_single_output_node
{
private:
	mandelbrot_output _output;
public:
	mandelbrot_node(void);

	base_output& single_output(void)
	override;
};

} // namespace texgen
} // namespace oglplus

#if !OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)
#include <oglplus/texgen/mandelbrot_node.inl>
#endif

#endif // include guard
