/**
 *  @file oglplus/texgen/base_output.inl
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#include <eagine/assert.hpp>
#include <iostream>
#include <set>
#include <string>

namespace eagine::oglp::texgen {
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::ostream& base_output::input_defs(std::ostream& out, compile_context& ctxt) {
    return parent().input_definitions(out, ctxt);
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::ostream&
base_output::opening_expr(std::ostream& out, compile_context& ctxt) {
    out << data_type_name(value_type()) << " ";
    out << expr::output_id{*this, ctxt};
    out << expr::render_param_decl{*this} << "{" << std::endl;
    return out;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::ostream& base_output::closing_expr(std::ostream& out, compile_context&) {
    return out << "}" << std::endl << std::endl;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
bool base_output::already_defined(compile_context& context) {
    if(!context.remembers_output(*this)) {
        context.remember_output(*this);
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
string_view base_output::name() noexcept {
    return string_view("Output");
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
render_param_bits base_output::required_params() {
    return all_render_params();
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::ostream& base_output::definitions(std::ostream& out, compile_context&) {
    return out;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::ostream& base_output::expression(std::ostream& out, compile_context&) {
    append_id(out);
    return out;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
bool base_output::is_connected(input_intf& input) {
    return _inputs.find(std::addressof(input)) != _inputs.end();
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
bool base_output::connect(input_intf& input) {
    if(input.accepts_value_type(value_type())) {
        EAGINE_ASSERT(!is_connected(input));
        _inputs.insert(std::addressof(input));
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
bool base_output::disconnect(input_intf& input) {
    EAGINE_ASSERT(is_connected(input));
    _inputs.erase(std::addressof(input));
    return true;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
void base_output::disconnect_all() {
    _inputs.clear();
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
void base_output::notify_connected() {
    for(auto* input : _inputs) {
        EAGINE_ASSERT(input != nullptr);
        input->update_needed();
    }
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
void base_output::prepare_parent() {
    parent().prepare();
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
bool base_output::render_parent(const render_params& params) {
    return parent().render(params);
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::intptr_t base_output::get_id() const noexcept {
    return reinterpret_cast<std::intptr_t>(this);
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
void base_output::append_id(std::ostream& out, string_view name) {
    out << "oglptg" << name << get_id();
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
void base_output::append_id(std::ostream& out) {
    append_id(out, type_name());
}
//------------------------------------------------------------------------------
namespace expr {
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::ostream& operator<<(std::ostream& out, const output_id& e) {
    e.output.expression(out, e.context);
    return out;
}
//------------------------------------------------------------------------------
} // namespace expr
} // namespace eagine::oglp::texgen
//------------------------------------------------------------------------------
