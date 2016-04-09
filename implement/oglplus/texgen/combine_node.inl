/**
 *  @file oglplus/texgen/combine_node.inl
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>

namespace oglplus {
namespace texgen {
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
combine_output::combine_output(node_intf& parent)
 : base_output(parent)
 , input_r(parent, cstr_ref("Red"))
 , input_g(parent, cstr_ref("Green"))
 , input_b(parent, cstr_ref("Blee"))
 , input_a(parent, cstr_ref("Alpha"))
{ }
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
cstr_ref
combine_output::type_name(void)
{
	return cstr_ref("Combine");
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
scalar_data_type
combine_output::component_type(void)
{
	return elem_data_type(
		common_data_type(
			common_data_type(
				input_r.output().value_type(),
				input_g.output().value_type()
			),
			common_data_type(
				input_b.output().value_type(),
				input_a.output().value_type()
			)
		)
	);
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
slot_data_type
combine_output::value_type(void)
{
	return make_data_type(component_type(), 4);
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::string
combine_output::_input_expr(
	_input_t& input,
	slot_data_type type,
	compile_context& ctxt
)
{
	std::stringstream res;

	res << expr::conversion_prefix{input.value_type(), type};
	res << expr::output_id{input.output(), ctxt};
	res << expr::render_param_pass{input.output()};
	res << expr::conversion_suffix{input.value_type(), type};
	return res.str();
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::ostream&
combine_output::definitions(std::ostream& out, compile_context& ctxt)
{
	if(already_defined(ctxt)) return out;

	input_defs(out, ctxt);
	opening_expr(out, ctxt);

	slot_data_type dt = make_data_type(component_type(), 1);

	std::map<std::string, std::set<unsigned>> expr_to_inputs;

	expr_to_inputs[_input_expr(input_r, dt, ctxt)].insert(0);
	expr_to_inputs[_input_expr(input_g, dt, ctxt)].insert(1);
	expr_to_inputs[_input_expr(input_b, dt, ctxt)].insert(2);
	expr_to_inputs[_input_expr(input_a, dt, ctxt)].insert(3);

	std::map<unsigned, unsigned> component_to_expr;

	unsigned v=0;

	for(const auto& ei : expr_to_inputs)
	{
		out << "\t" << data_type_name(dt);
		out << " value" << v << " = " << ei.first << ";" << std::endl;

		for(unsigned cmpn : ei.second)
		{
			component_to_expr[cmpn] = v;
		}
		++v;
	}

	out << "\treturn " << data_type_name(value_type()) << "(";
	for(unsigned i=0; i<4; ++i)
	{
		if(i > 0) out << ", ";
		out << "value" << component_to_expr[i];
	}
	out << ");" << std::endl;

	return closing_expr(out, ctxt);
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
combine_node::combine_node(void)
 : _output(*this)
{ }
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
base_output&
combine_node::single_output(void)
{
	return _output;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::size_t
combine_node::input_count(void)
{
	return 4u;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
input_intf&
combine_node::input(std::size_t index)
{
	if(index == 0) return _output.input_r;
	if(index == 1) return _output.input_g;
	if(index == 2) return _output.input_b;
	assert(index < input_count());
	return _output.input_a;
}
//------------------------------------------------------------------------------
} // namespace texgen
} // namespace oglplus
//------------------------------------------------------------------------------

