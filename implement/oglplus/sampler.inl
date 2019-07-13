/**
 *  @file oglplus/sampler.inl
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#include <eagine/assert.hpp>
#include <oglplus/oper/numeric_queries.hpp>
#include <oglplus/utils/gl_func.hpp>

namespace oglplus {
//------------------------------------------------------------------------------
namespace oper {
//------------------------------------------------------------------------------
inline outcome<void> sampler_ops::bind_sampler(
  texture_unit unit, sampler_name sam) noexcept {
    OGLPLUS_GLFUNC(BindSampler)(GLenum(unit), get_raw_name(sam));
    OGLPLUS_VERIFY(BindSampler, gl_enum_value(unit).gl_object(sam), debug);
    return {};
}
//------------------------------------------------------------------------------
#if defined(GL_VERSION_4_4)
template <typename S>
inline outcome<void> sampler_ops::bind_samplers(
  texture_unit first, const object_names<tag::sampler, S>& samplers) noexcept {
    OGLPLUS_GLFUNC(BindSamplers)
    (GLuint(first.index()),
     GLsizei(samplers.size()),
     get_raw_names(samplers).data());
    OGLPLUS_VERIFY(
      BindSamplers, gl_enum_value(first).gl_object(samplers[0]), debug);
    return {};
}
#endif
//------------------------------------------------------------------------------
inline outcome<sampler_name> sampler_ops::sampler_binding(
  texture_unit unit) noexcept {
    OGLPLUS_GLFUNC(ActiveTexture)(GLenum(unit));
    OGLPLUS_VERIFY(ActiveTexture, gl_enum_value(unit), always);
    GLint result = 0;
    return numeric_queries::get_integer_v(
             binding_query(GL_SAMPLER_BINDING), {&result, 1})
      .add(sampler_name(GLuint(result)));
}
//------------------------------------------------------------------------------
inline outcome<void> sampler_ops::sampler_parameter_i(
  sampler_name sam, oglplus::sampler_parameter param, GLint value) noexcept {
    OGLPLUS_GLFUNC(SamplerParameteri)(get_raw_name(sam), GLenum(param), value);
    OGLPLUS_VERIFY(
      SamplerParameteri, gl_object(sam).gl_enum_value(param), always);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> sampler_ops::get_sampler_parameter_iv(
  sampler_name sam,
  oglplus::sampler_parameter param,
  span<GLint> values) noexcept {
    EAGINE_ASSERT(values.size() > 0);
    OGLPLUS_GLFUNC(GetSamplerParameteriv)
    (get_raw_name(sam), GLenum(param), values.data());
    OGLPLUS_VERIFY(
      GetSamplerParameteriv, gl_object(sam).gl_enum_value(param), always);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> sampler_ops::sampler_parameter_f(
  sampler_name sam, oglplus::sampler_parameter param, GLfloat value) noexcept {
    OGLPLUS_GLFUNC(SamplerParameterf)(get_raw_name(sam), GLenum(param), value);
    OGLPLUS_VERIFY(
      SamplerParameterf, gl_object(sam).gl_enum_value(param), always);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> sampler_ops::get_sampler_parameter_fv(
  sampler_name sam,
  oglplus::sampler_parameter param,
  span<GLfloat> values) noexcept {
    EAGINE_ASSERT(values.size() > 0);
    OGLPLUS_GLFUNC(GetSamplerParameterfv)
    (get_raw_name(sam), GLenum(param), values.data());
    OGLPLUS_VERIFY(
      GetSamplerParameterfv, gl_object(sam).gl_enum_value(param), always);
    return {};
}
//------------------------------------------------------------------------------
template <typename R, typename T>
inline outcome<R> sampler_ops::return_sampler_parameter_i(
  sampler_name sam, sampler_parameter parameter) noexcept {
    GLint result = 0;
    return get_sampler_parameter_iv(sam, parameter, {&result, 1}), R(T(result));
}
//------------------------------------------------------------------------------
template <typename R>
inline outcome<R> sampler_ops::return_sampler_parameter_f(
  sampler_name sam, sampler_parameter parameter) noexcept {
    GLfloat result = 0;
    return get_sampler_parameter_fv(sam, parameter, {&result, 1}), R(result);
}
//------------------------------------------------------------------------------
inline outcome<void> sampler_ops::sampler_min_filter(
  sampler_name sam, texture_min_filter value) noexcept {
    return sampler_parameter_i(
      sam, sampler_parameter(GL_TEXTURE_MIN_FILTER), GLint(GLenum(value)));
}
//------------------------------------------------------------------------------
inline outcome<texture_min_filter> sampler_ops::get_sampler_min_filter(
  sampler_name sam) noexcept {
    return return_sampler_parameter_i<texture_min_filter, GLenum>(
      sam, sampler_parameter(GL_TEXTURE_MIN_FILTER));
}
//------------------------------------------------------------------------------
inline outcome<void> sampler_ops::sampler_mag_filter(
  sampler_name sam, texture_mag_filter value) noexcept {
    return sampler_parameter_i(
      sam, sampler_parameter(GL_TEXTURE_MAG_FILTER), GLint(GLenum(value)));
}
//------------------------------------------------------------------------------
inline outcome<texture_mag_filter> sampler_ops::get_sampler_mag_filter(
  sampler_name sam) noexcept {
    return return_sampler_parameter_i<texture_mag_filter, GLenum>(
      sam, sampler_parameter(GL_TEXTURE_MAG_FILTER));
}
//------------------------------------------------------------------------------
inline outcome<void> sampler_ops::sampler_compare_mode(
  sampler_name sam, texture_compare_mode value) noexcept {
    return sampler_parameter_i(
      sam, sampler_parameter(GL_TEXTURE_COMPARE_MODE), GLint(GLenum(value)));
}
//------------------------------------------------------------------------------
inline outcome<texture_compare_mode> sampler_ops::get_sampler_compare_mode(
  sampler_name sam) noexcept {
    return return_sampler_parameter_i<texture_compare_mode, GLenum>(
      sam, sampler_parameter(GL_TEXTURE_COMPARE_MODE));
}
//------------------------------------------------------------------------------
inline outcome<void> sampler_ops::sampler_compare_func(
  sampler_name sam, compare_function value) noexcept {
    return sampler_parameter_i(
      sam, sampler_parameter(GL_TEXTURE_COMPARE_FUNC), GLint(GLenum(value)));
}
//------------------------------------------------------------------------------
inline outcome<compare_function> sampler_ops::get_sampler_compare_func(
  sampler_name sam) noexcept {
    return return_sampler_parameter_i<compare_function, GLenum>(
      sam, sampler_parameter(GL_TEXTURE_COMPARE_FUNC));
}
//------------------------------------------------------------------------------
inline outcome<void> sampler_ops::sampler_wrap(
  sampler_name sam,
  texture_wrap_coord coord,
  texture_wrap_mode value) noexcept {
    return sampler_parameter_i(
      sam, sampler_parameter(GLenum(coord)), GLint(GLenum(value)));
}
//------------------------------------------------------------------------------
inline outcome<texture_wrap_mode> sampler_ops::get_sampler_wrap(
  sampler_name sam, texture_wrap_coord coord) noexcept {
    return return_sampler_parameter_i<texture_wrap_mode, GLenum>(
      sam, sampler_parameter(GLenum(coord)));
}
//------------------------------------------------------------------------------
inline outcome<void> sampler_ops::sampler_wrap_s(
  sampler_name sam, texture_wrap_mode value) noexcept {
    return sampler_parameter_i(
      sam, sampler_parameter(GL_TEXTURE_WRAP_S), GLint(GLenum(value)));
}
//------------------------------------------------------------------------------
inline outcome<texture_wrap_mode> sampler_ops::get_sampler_wrap_s(
  sampler_name sam) noexcept {
    return return_sampler_parameter_i<texture_wrap_mode, GLenum>(
      sam, sampler_parameter(GL_TEXTURE_WRAP_S));
}
//------------------------------------------------------------------------------
inline outcome<void> sampler_ops::sampler_wrap_t(
  sampler_name sam, texture_wrap_mode value) noexcept {
    return sampler_parameter_i(
      sam, sampler_parameter(GL_TEXTURE_WRAP_T), GLint(GLenum(value)));
}
//------------------------------------------------------------------------------
inline outcome<texture_wrap_mode> sampler_ops::get_sampler_wrap_t(
  sampler_name sam) noexcept {
    return return_sampler_parameter_i<texture_wrap_mode, GLenum>(
      sam, sampler_parameter(GL_TEXTURE_WRAP_T));
}
//------------------------------------------------------------------------------
inline outcome<void> sampler_ops::sampler_wrap_r(
  sampler_name sam, texture_wrap_mode value) noexcept {
    return sampler_parameter_i(
      sam, sampler_parameter(GL_TEXTURE_WRAP_R), GLint(GLenum(value)));
}
//------------------------------------------------------------------------------
inline outcome<texture_wrap_mode> sampler_ops::get_sampler_wrap_r(
  sampler_name sam) noexcept {
    return return_sampler_parameter_i<texture_wrap_mode, GLenum>(
      sam, sampler_parameter(GL_TEXTURE_WRAP_R));
}
//------------------------------------------------------------------------------
#if defined(GL_TEXTURE_LOD_BIAS)
inline outcome<void> sampler_ops::sampler_lod_bias(
  sampler_name sam, GLfloat value) noexcept {
    return sampler_parameter_f(
      sam, sampler_parameter(GL_TEXTURE_LOD_BIAS), value);
}

//------------------------------------------------------------------------------
inline outcome<GLfloat> sampler_ops::get_sampler_lod_bias(
  sampler_name sam) noexcept {
    return return_sampler_parameter_f<GLfloat>(
      sam, sampler_parameter(GL_TEXTURE_LOD_BIAS));
}
#endif
//------------------------------------------------------------------------------
inline outcome<void> sampler_ops::sampler_min_lod(
  sampler_name sam, GLfloat value) noexcept {
    return sampler_parameter_f(
      sam, sampler_parameter(GL_TEXTURE_MIN_LOD), value);
}

//------------------------------------------------------------------------------
inline outcome<GLfloat> sampler_ops::get_sampler_min_lod(
  sampler_name sam) noexcept {
    return return_sampler_parameter_f<GLfloat>(
      sam, sampler_parameter(GL_TEXTURE_MIN_LOD));
}

//------------------------------------------------------------------------------
inline outcome<void> sampler_ops::sampler_max_lod(
  sampler_name sam, GLfloat value) noexcept {
    return sampler_parameter_f(
      sam, sampler_parameter(GL_TEXTURE_MAX_LOD), value);
}

//------------------------------------------------------------------------------
inline outcome<GLfloat> sampler_ops::get_sampler_max_lod(
  sampler_name sam) noexcept {
    return return_sampler_parameter_f<GLfloat>(
      sam, sampler_parameter(GL_TEXTURE_MAX_LOD));
}
//------------------------------------------------------------------------------
} // namespace oper
//------------------------------------------------------------------------------
// obj_gen_del_ops::_gen
//------------------------------------------------------------------------------
inline deferred_error_handler obj_gen_del_ops<tag::sampler>::_gen(
  span<GLuint> names) noexcept {
    OGLPLUS_GLFUNC(GenSamplers)(GLsizei(names.size()), names.data());
    OGLPLUS_VERIFY_SIMPLE(GenSamplers, debug);
    return {};
}
//------------------------------------------------------------------------------
// obj_gen_del_ops::_create
//------------------------------------------------------------------------------
#if defined(GL_VERSION_4_5)
inline deferred_error_handler obj_gen_del_ops<tag::sampler>::_create(
  span<GLuint> names) noexcept {
    OGLPLUS_GLFUNC(CreateSamplers)(GLsizei(names.size()), names.data());
    OGLPLUS_VERIFY_SIMPLE(CreateSamplers, debug);
    return {};
}
#endif
//------------------------------------------------------------------------------
// obj_gen_del_ops::_delete
//------------------------------------------------------------------------------
inline deferred_error_handler obj_gen_del_ops<tag::sampler>::_delete(
  span<GLuint> names) noexcept {
    OGLPLUS_GLFUNC(DeleteSamplers)(GLsizei(names.size()), names.data());
    OGLPLUS_VERIFY_SIMPLE(DeleteSamplers, debug);
    return {};
}
//------------------------------------------------------------------------------
// obj_gen_del_ops::_is_a
//------------------------------------------------------------------------------
inline outcome<boolean> obj_gen_del_ops<tag::sampler>::_is_a(
  GLuint name) noexcept {
    GLboolean res = OGLPLUS_GLFUNC(IsSampler)(name);
    OGLPLUS_VERIFY_SIMPLE(IsSampler, debug);
    return boolean(res);
}
//------------------------------------------------------------------------------
} // namespace oglplus
