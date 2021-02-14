/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#include <iostream>

namespace eagine::oglp::texgen {
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
string_view data_type_name(slot_data_type type) noexcept {
    switch(type) {
        case slot_data_type::bool_:
            return string_view("bool");
        case slot_data_type::bool_2:
            return string_view("bvec2");
        case slot_data_type::bool_3:
            return string_view("bvec3");
        case slot_data_type::bool_4:
            return string_view("bvec4");
        case slot_data_type::int_:
            return string_view("int");
        case slot_data_type::int_2:
            return string_view("ivec2");
        case slot_data_type::int_3:
            return string_view("ivec3");
        case slot_data_type::int_4:
            return string_view("ivec4");
        case slot_data_type::float_:
            return string_view("float");
        case slot_data_type::float_2:
            return string_view("vec2");
        case slot_data_type::float_3:
            return string_view("vec3");
        case slot_data_type::float_4:
            return string_view("vec4");
    }
    return string_view("_unknown_type_");
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
scalar_data_type elem_data_type(slot_data_type type) noexcept {
    switch(type) {
        case slot_data_type::bool_:
        case slot_data_type::bool_2:
        case slot_data_type::bool_3:
        case slot_data_type::bool_4:
            return scalar_data_type::bool_;
        case slot_data_type::int_:
        case slot_data_type::int_2:
        case slot_data_type::int_3:
        case slot_data_type::int_4:
            return scalar_data_type::int_;
        case slot_data_type::float_:
        case slot_data_type::float_2:
        case slot_data_type::float_3:
        case slot_data_type::float_4:
            return scalar_data_type::float_;
    }
    return scalar_data_type();
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
span_size_t data_type_dims(slot_data_type type) noexcept {
    switch(type) {
        case slot_data_type::bool_:
        case slot_data_type::int_:
        case slot_data_type::float_:
            return 1;
        case slot_data_type::bool_2:
        case slot_data_type::int_2:
        case slot_data_type::float_2:
            return 2;
        case slot_data_type::bool_3:
        case slot_data_type::int_3:
        case slot_data_type::float_3:
            return 3;
        case slot_data_type::bool_4:
        case slot_data_type::int_4:
        case slot_data_type::float_4:
            return 4;
    }
    return 0;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
slot_data_type make_data_type(
  scalar_data_type type,
  const valid_if_between<span_size_t, 1, 4>& dims) noexcept {
    if(dims) {
        switch(type) {
            case scalar_data_type::bool_:
                switch(dims.value_anyway()) {
                    case 1:
                        return slot_data_type::bool_;
                    case 2:
                        return slot_data_type::bool_2;
                    case 3:
                        return slot_data_type::bool_3;
                    case 4:
                        return slot_data_type::bool_4;
                }
            case scalar_data_type::int_:
                switch(dims.value_anyway()) {
                    case 1:
                        return slot_data_type::int_;
                    case 2:
                        return slot_data_type::int_2;
                    case 3:
                        return slot_data_type::int_3;
                    case 4:
                        return slot_data_type::int_4;
                }
            case scalar_data_type::float_:
                switch(dims.value_anyway()) {
                    case 1:
                        return slot_data_type::float_;
                    case 2:
                        return slot_data_type::float_2;
                    case 3:
                        return slot_data_type::float_3;
                    case 4:
                        return slot_data_type::float_4;
                }
        }
    }
    return slot_data_type();
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
scalar_data_type common_elem_type(slot_data_type a, slot_data_type b) noexcept {
    scalar_data_type ta = elem_data_type(a);
    scalar_data_type tb = elem_data_type(b);
    return int(ta) > int(tb) ? ta : tb;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
span_size_t common_dims(slot_data_type a, slot_data_type b) noexcept {
    span_size_t da = data_type_dims(a);
    span_size_t db = data_type_dims(b);
    return da > db ? da : db;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
slot_data_type common_data_type(slot_data_type a, slot_data_type b) noexcept {
    if(a == b) {
        return a;
    }

    return make_data_type(common_elem_type(a, b), common_dims(a, b));
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::ostream&
conversion_prefix(std::ostream& out, slot_data_type from, slot_data_type to) {
    if(from != to) {
        out << data_type_name(to) << "(";
    }
    return out;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::ostream&
conversion_suffix(std::ostream& out, slot_data_type from, slot_data_type to) {
    if(from != to) {
        span_size_t df = data_type_dims(from);
        span_size_t dt = data_type_dims(to);

        for(span_size_t i = df; i < dt; ++i) {
            out << ",0";
        }

        out << ")";
    }
    return out;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::ostream& conversion_suffix(
  std::ostream& out,
  slot_data_type from,
  slot_data_type to,
  string_view x,
  string_view y,
  string_view z,
  string_view w) {
    if(from != to) {
        span_size_t df = data_type_dims(from);
        span_size_t dt = data_type_dims(to);

        if((df < 1) && (dt >= 1)) {
            out << "," << x;
        }
        if((df < 2) && (dt >= 2)) {
            out << "," << y;
        }
        if((df < 3) && (dt >= 3)) {
            out << "," << z;
        }
        if((df < 4) && (dt >= 4)) {
            out << "," << w;
        }

        out << ")";
    }
    return out;
}
//------------------------------------------------------------------------------
namespace expr {
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::ostream& operator<<(std::ostream& out, conversion_prefix e) {
    return oglp::texgen::conversion_prefix(out, e.from, e.to);
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::ostream& operator<<(std::ostream& out, conversion_suffix e) {
    return oglp::texgen::conversion_suffix(out, e.from, e.to);
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
std::ostream& operator<<(std::ostream& out, conversion_suffix_v e) {
    return oglp::texgen::conversion_suffix(
      out, e.from, e.to, e.val[0], e.val[1], e.val[2], e.val[3]);
}
//------------------------------------------------------------------------------
} // namespace expr
} // namespace eagine::oglp::texgen
//------------------------------------------------------------------------------
