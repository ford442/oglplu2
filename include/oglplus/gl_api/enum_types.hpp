/**
 *  @file oglplus/gl_api/enum_types.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef OGLPLUS_GL_API_ENUM_TYPES_HPP
#define OGLPLUS_GL_API_ENUM_TYPES_HPP

#include "enum_class.hpp"

namespace eagine {
namespace oglp {
//------------------------------------------------------------------------------
struct error_code : gl_enum_class<error_code, EAGINE_ID_V(ErrorCode)> {
    using enum_class::enum_class;
};

struct capability : gl_enum_class<capability, EAGINE_ID_V(Capability)> {
    using enum_class::enum_class;
};

struct graphics_reset_status
  : gl_enum_class<graphics_reset_status, EAGINE_ID_V(GrphRstStt)> {
    using enum_class::enum_class;
};

struct memory_barrier_bit
  : gl_enum_class<memory_barrier_bit, EAGINE_ID_V(MemBrirBit)> {
    using enum_class::enum_class;
};

struct shader_type : gl_enum_class<shader_type, EAGINE_ID_V(ShaderType)> {
    using enum_class::enum_class;
};

struct buffer_target : gl_enum_class<buffer_target, EAGINE_ID_V(BufferTgt)> {
    using enum_class::enum_class;
};

struct buffer_usage : gl_enum_class<buffer_usage, EAGINE_ID_V(BufferUsge)> {
    using enum_class::enum_class;
};

struct buffer_map_access_bit
  : gl_enum_class<buffer_map_access_bit, EAGINE_ID_V(BufMapAccB)> {
    using enum_class::enum_class;
};

struct program_interface
  : gl_enum_class<program_interface, EAGINE_ID_V(ProgrmIntf)> {
    using enum_class::enum_class;
};

struct program_property
  : gl_enum_class<program_property, EAGINE_ID_V(ProgrmProp)> {
    using enum_class::enum_class;
};

struct buffer_clear_bit
  : gl_enum_class<buffer_clear_bit, EAGINE_ID_V(BuffClrBit)> {
    using enum_class::enum_class;
};

struct texture_compare_mode
  : gl_enum_class<texture_compare_mode, EAGINE_ID_V(TexCmpMode)> {
    using enum_class::enum_class;
};

struct integer_query : gl_enum_class<integer_query, EAGINE_ID_V(IntQuery)> {
    using enum_class::enum_class;
};

struct string_query : gl_enum_class<string_query, EAGINE_ID_V(StrQuery)> {
    using enum_class::enum_class;
};
//------------------------------------------------------------------------------
} // namespace oglp
} // namespace eagine

#endif // OGLPLUS_GL_API_ENUM_TYPES_HPP

