/// @file oglplus/texgen/sphere_coord_node.hpp
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#ifndef OGLPLUS_TEXGEN_SPHERE_COORD_NODE_HPP
#define OGLPLUS_TEXGEN_SPHERE_COORD_NODE_HPP

#include "base_node.hpp"

namespace eagine::oglp::texgen {

class sphere_coord_output : public base_output {
public:
    sphere_coord_output(node_intf& parent);

    string_view type_name() override;

    slot_data_type value_type() override;

    std::ostream& definitions(std::ostream& out, compile_context& ctxt) override;
};

using sphere_coord_node = single_output_node<sphere_coord_output>;

} // namespace eagine::oglp::texgen

#if !OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)
#include <oglplus/texgen/sphere_coord_node.inl>
#endif

#endif // OGLPLUS_TEXGEN_SPHERE_COORD_NODE_HPP
