/**
 *  @file oglplus/error/info.inl
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#include <eagine/maybe_unused.hpp>
#include <oglplus/enum/types.hpp>
#include <oglplus/utils/span.hpp>
#include <string>
#include <vector>

#ifndef GL_SHADER
#define GL_SHADER 0x82E1
#endif

#ifndef GL_PROGRAM
#define GL_PROGRAM 0x82E2
#endif

#ifndef GL_PROGRAM_PIPELINE
#define GL_PROGRAM_PIPELINE 0x82E4
#endif

namespace oglplus {
//------------------------------------------------------------------------------
#if !OGLPLUS_ERROR_NO_EXTENDED_INFO
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
extended_error_info& error_info::_ext_info() const noexcept {
    if(!_ext_info_ptr) {
        try {
            _ext_info_ptr = std::make_unique<extended_error_info>();
        } catch(...) {
        }
    }
    if(!_ext_info_ptr) {
        static extended_error_info fbk;
        return fbk;
    }
    return *_ext_info_ptr;
}
//------------------------------------------------------------------------------
#endif // NO_EXTENDED_INFO
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info::error_info(GLenum gl_err_code) noexcept
  : _gl_err_code(gl_err_code)
  , _ext_info_ptr()
#if !OGLPLUS_ERROR_NO_GL_LIB
  , _gl_lb_name(nullptr)
#endif
#if !OGLPLUS_ERROR_NO_GL_FUNC
  , _gl_fn_name(nullptr)
#endif
#if !OGLPLUS_ERROR_NO_SRC_FUNC
  , _src_func(nullptr)
#endif
#if !OGLPLUS_ERROR_NO_SRC_FILE
  , _src_file(nullptr)
#endif
#if !OGLPLUS_ERROR_NO_SRC_LINE
  , _src_line(0)
#endif
#if !OGLPLUS_ERROR_NO_OBJECT
  , _obj_name()
#endif
#if !OGLPLUS_ERROR_NO_INDEX
  , _index(invalid_index())
#endif
#if !OGLPLUS_ERROR_NO_ENUM_VALUE
  , _enum_val()
#endif
{
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::gl_error_code(GLenum gl_err_code) noexcept {
    _gl_err_code = gl_err_code;
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
GLenum error_info::gl_error_code() const noexcept {
    return _gl_err_code;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::gl_library_name(const char* gl_lb_name) noexcept {
#if !OGLPLUS_ERROR_NO_GL_LIB
    _gl_lb_name = gl_lb_name;
#else
    EAGINE_MAYBE_UNUSED(gl_lb_name);
#endif
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
const char* error_info::gl_library_name() const noexcept {
#if !OGLPLUS_ERROR_NO_GL_LIB
    return _gl_lb_name;
#else
    return nullptr;
#endif
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::gl_function_name(const char* gl_fn_name) noexcept {
#if !OGLPLUS_ERROR_NO_GL_FUNC
    _gl_fn_name = gl_fn_name;
#else
    EAGINE_MAYBE_UNUSED(gl_fn_name);
#endif
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
const char* error_info::gl_function_name() const noexcept {
#if !OGLPLUS_ERROR_NO_GL_FUNC
    return _gl_fn_name;
#else
    return nullptr;
#endif
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::source_function(const char* src_func) noexcept {
#if !OGLPLUS_ERROR_NO_SRC_FUNC
    _src_func = src_func;
#else
    EAGINE_MAYBE_UNUSED(src_func);
#endif
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
const char* error_info::source_function() const noexcept {
#if !OGLPLUS_ERROR_NO_SRC_FUNC
    return _src_func;
#else
    return nullptr;
#endif
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::source_file(const char* src_file) noexcept {
#if !OGLPLUS_ERROR_NO_SRC_FILE
    _src_file = src_file;
#else
    EAGINE_MAYBE_UNUSED(src_file);
#endif
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
const char* error_info::source_file() const noexcept {
#if !OGLPLUS_ERROR_NO_SRC_FILE
    return _src_file;
#else
    return nullptr;
#endif
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::source_line(unsigned src_line) noexcept {
#if !OGLPLUS_ERROR_NO_SRC_LINE
    _src_line = src_line;
#else
    EAGINE_MAYBE_UNUSED(src_line);
#endif
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
unsigned error_info::source_line() const noexcept {
#if !OGLPLUS_ERROR_NO_SRC_LINE
    return _src_line;
#else
    return 0u;
#endif
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::gl_object(const any_object_name& obj_name) noexcept {
#if !OGLPLUS_ERROR_NO_OBJECT
    _obj_name = obj_name;
#else
    EAGINE_MAYBE_UNUSED(obj_name);
#endif
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::gl_object_binding(
  const any_object_type&, const any_enum_value&) noexcept {
    // TODO
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
any_object_name error_info::gl_object() const noexcept {
#if !OGLPLUS_ERROR_NO_OBJECT
    return _obj_name;
#else
    return any_object_name();
#endif
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::gl_subject_binding(
  const any_object_type&, const any_enum_value&) noexcept {
    // TODO
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::gl_subject(const any_object_name& sub_name) noexcept {
#if !OGLPLUS_ERROR_NO_SUBJECT
    _ext_info()._sub_name = sub_name;
#else
    EAGINE_MAYBE_UNUSED(sub_name);
#endif
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
any_object_name error_info::gl_subject() const noexcept {
#if !OGLPLUS_ERROR_NO_SUBJECT
    return _ext_info()._sub_name;
#else
    return any_object_name();
#endif
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::gl_index(GLuint idx) noexcept {
#if !OGLPLUS_ERROR_NO_INDEX
    _index = idx;
#else
    EAGINE_MAYBE_UNUSED(idx);
#endif
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
GLuint error_info::gl_index() const noexcept {
#if !OGLPLUS_ERROR_NO_INDEX
    return _index;
#else
    return invalid_index();
#endif
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::gl_enum_value(const any_enum_value& enum_val) noexcept {
#if !OGLPLUS_ERROR_NO_ENUM_VALUE
    _enum_val = enum_val;
#else
    EAGINE_MAYBE_UNUSED(enum_val);
#endif
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::gl_enum_value(
  const any_indexed_enum_value& enum_val) noexcept {
#if !OGLPLUS_ERROR_NO_ENUM_VALUE
    _enum_val = indexed_value_base(enum_val.base());
#endif
#if !OGLPLUS_ERROR_NO_INDEX
    _index = GLuint(enum_val.index());
#endif
    EAGINE_MAYBE_UNUSED(enum_val);
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
const any_enum_value& error_info::gl_enum_value() const noexcept {
#if !OGLPLUS_ERROR_NO_ENUM_VALUE
    return _enum_val;
#else
    static any_enum_value enum_val;
    return enum_val;
#endif
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::identifier(string_view ident) noexcept {
#if !OGLPLUS_ERROR_NO_IDENTIFIER
    try {
        _ext_info()._identifier.assign(ident.begin(), ident.end());
    } catch(...) {
    }
#else
    EAGINE_MAYBE_UNUSED(ident);
#endif
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
string_view error_info::identifier() const noexcept {
#if !OGLPLUS_ERROR_NO_IDENTIFIER
    return _ext_info()._identifier;
#else
    return {};
#endif
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::info_log(string_view log) noexcept {
#if !OGLPLUS_ERROR_NO_INFO_LOG
    try {
        _ext_info()._info_log.assign(log.begin(), log.end());
    } catch(...) {
    }
#else
    EAGINE_MAYBE_UNUSED(log);
#endif
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
error_info& error_info::info_log_of(const any_object_name& obj) noexcept {
#if !OGLPLUS_ERROR_NO_INFO_LOG
    PFNGLGETPROGRAMIVPROC _GetObjectiv = nullptr;
    PFNGLGETPROGRAMINFOLOGPROC _GetObjectInfoLog = nullptr;

    if(obj._type == GL_SHADER) {
        _GetObjectiv = glGetShaderiv;
        _GetObjectInfoLog = glGetShaderInfoLog;
    } else if(obj._type == GL_PROGRAM) {
        _GetObjectiv = glGetProgramiv;
        _GetObjectInfoLog = glGetProgramInfoLog;
    } else if(obj._type == GL_PROGRAM_PIPELINE) {
#if defined(GL_VERSION_4_1) || defined(GL_ARB_separate_shader_objects)
        _GetObjectiv = glGetProgramPipelineiv;
        _GetObjectInfoLog = glGetProgramPipelineInfoLog;
#endif
    }

    if(_GetObjectiv && _GetObjectInfoLog) {
        GLint len = 0;
        _GetObjectiv(get_raw_name(obj), GL_INFO_LOG_LENGTH, &len);
        if(len > 0) {
            std::vector<GLchar> tmp(std_size(len + 1), '\0');
            _GetObjectInfoLog(get_raw_name(obj), len, nullptr, tmp.data());
            if(glGetError() == GL_NO_ERROR && tmp.front() != '\0') {
                info_log(tmp);
            }
        }
    }
#else
    EAGINE_MAYBE_UNUSED(log);
#endif
    return *this;
}
//------------------------------------------------------------------------------
OGLPLUS_LIB_FUNC
string_view error_info::info_log() const noexcept {
#if !OGLPLUS_ERROR_NO_INFO_LOG
    return {_ext_info()._info_log};
#else
    return {};
#endif
}
//------------------------------------------------------------------------------
} // namespace oglplus
