/**
 *  @file oglplus/framebuffer.inl
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#include <oglplus/oper/numeric_queries.hpp>
#include <oglplus/utils/gl_func.hpp>

#if !OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)
#include "enum/framebuffer_target_bq.inl"
#endif

namespace oglplus {
//------------------------------------------------------------------------------
namespace oper {
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::bind_framebuffer(
  framebuffer_target target, framebuffer_name fbo) noexcept {
    OGLPLUS_GLFUNC(BindFramebuffer)(GLenum(target), get_raw_name(fbo));
    OGLPLUS_VERIFY(
      BindFramebuffer, gl_enum_value(target).gl_object(fbo), debug);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<framebuffer_name> framebuffer_ops::framebuffer_binding(
  framebuffer_target target) noexcept {
    GLint result = 0;
    return numeric_queries::get_integer_v(
             get_binding_query(target), {&result, 1})
      .add(framebuffer_name(GLuint(result)));
}
//------------------------------------------------------------------------------
inline outcome<framebuffer_status> framebuffer_ops::check_framebuffer_status(
  framebuffer_target target) noexcept {
    GLenum result = OGLPLUS_GLFUNC(CheckFramebufferStatus)(GLenum(target));
    if(result == 0) {
        OGLPLUS_VERIFY(CheckFramebufferStatus, gl_enum_value(target), debug);
    }
    return {framebuffer_status(result)};
}
//------------------------------------------------------------------------------
inline outcome<bool> framebuffer_ops::is_framebuffer_complete(
  framebuffer_target target) noexcept {
    return outcome_conversion<bool, framebuffer_status>(
      check_framebuffer_status(target),
      [](framebuffer_status status) noexcept->bool {
          return status == framebuffer_status(GL_FRAMEBUFFER_COMPLETE);
      });
}
//------------------------------------------------------------------------------
#if defined(GL_VERSION_4_5) || defined(GL_EXT_direct_state_access)
inline outcome<framebuffer_status> framebuffer_ops::check_framebuffer_status(
  framebuffer_name fbo, framebuffer_target target) noexcept {
#ifdef GL_VERSION_4_5
    GLenum result = OGLPLUS_GLFUNC(CheckNamedFramebufferStatus)(
#else
    GLenum result = OGLPLUS_GLFUNC(CheckNamedFramebufferStatusEXT)(
#endif
      get_raw_name(fbo), GLenum(target));
    if(result == 0) {
        OGLPLUS_VERIFY_STR(
          OGLPLUS_GL_DSA_FUNC_NAME(CheckNamedFramebufferStatus),
          gl_object(fbo).gl_enum_value(target),
          debug);
    }
    return {framebuffer_status(result)};
}
//------------------------------------------------------------------------------
inline outcome<bool> framebuffer_ops::is_framebuffer_complete(
  framebuffer_name fb, framebuffer_target target) noexcept {
    return outcome_conversion<bool, framebuffer_status>(
      check_framebuffer_status(fb, target),
      [](framebuffer_status status) noexcept->bool {
          return status == framebuffer_status(GL_FRAMEBUFFER_COMPLETE);
      });
}
#endif
//------------------------------------------------------------------------------
#if defined(GL_VERSION_4_3)
inline outcome<void> framebuffer_ops::framebuffer_parameter_i(
  framebuffer_target fb_target,
  framebuffer_parameter param,
  GLint value) noexcept {
    OGLPLUS_GLFUNC(FramebufferParameteri)
    (GLenum(fb_target), GLenum(param), value);
    OGLPLUS_VERIFY(FramebufferParameteri, gl_enum_value(param), debug);
    return {};
}
#endif
//------------------------------------------------------------------------------
#if defined(GL_VERSION_4_5)
inline outcome<void> framebuffer_ops::framebuffer_parameter_i(
  framebuffer_name fbo, framebuffer_parameter param, GLint value) noexcept {
    OGLPLUS_GLFUNC(NamedFramebufferParameteri)
    (get_raw_name(fbo), GLenum(param), value);
    OGLPLUS_VERIFY(
      NamedFramebufferParameteri, gl_enum_value(param).gl_object(fbo), debug);
    return {};
}
#endif
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::framebuffer_renderbuffer(
  framebuffer_target fb_target,
  framebuffer_attachment fb_attach,
  renderbuffer_target rb_target,
  renderbuffer_name rbo) noexcept {
    OGLPLUS_GLFUNC(FramebufferRenderbuffer)
    (GLenum(fb_target),
     GLenum(fb_attach),
     GLenum(rb_target),
     get_raw_name(rbo));
    OGLPLUS_VERIFY(
      FramebufferRenderbuffer, gl_enum_value(fb_target).gl_subject(rbo), debug);
    return {};
}
//------------------------------------------------------------------------------
#if defined(GL_VERSION_4_5) || defined(GL_EXT_direct_state_access)
inline outcome<void> framebuffer_ops::framebuffer_renderbuffer(
  framebuffer_name fbo,
  framebuffer_attachment fb_attach,
  renderbuffer_target rb_target,
  renderbuffer_name rbo) noexcept {
#ifdef GL_VERSION_4_5
    OGLPLUS_GLFUNC(NamedFramebufferRenderbuffer)
    (
#else
    OGLPLUS_GLFUNC(NamedFramebufferRenderbufferEXT)
    (
#endif
      get_raw_name(fbo),
      GLenum(fb_attach),
      GLenum(rb_target),
      get_raw_name(rbo));
    OGLPLUS_VERIFY_STR(
      OGLPLUS_GL_DSA_FUNC_NAME(NamedFramebufferRenderbuffer),
      gl_object(fbo).
      // gl_enum_value(fb_target).
      gl_subject(rbo),
      debug);
    return {};
}
#endif
//------------------------------------------------------------------------------
#if defined(GL_VERSION_3_0)
inline outcome<void> framebuffer_ops::framebuffer_texture_1d(
  framebuffer_target fb_target,
  framebuffer_attachment fb_attach,
  texture_target tx_target,
  texture_name tex,
  GLint level) noexcept {
    OGLPLUS_GLFUNC(FramebufferTexture1D)
    (GLenum(fb_target),
     GLenum(fb_attach),
     GLenum(tx_target),
     get_raw_name(tex),
     level);
    OGLPLUS_VERIFY(
      FramebufferTexture1D, gl_enum_value(fb_target).gl_subject(tex), debug);
    return {};
}
//------------------------------------------------------------------------------
#if defined(GL_EXT_direct_state_access)
inline outcome<void> framebuffer_ops::framebuffer_texture_1d(
  framebuffer_name fbo,
  framebuffer_attachment fb_attach,
  texture_target tx_target,
  texture_name tex,
  GLint level) noexcept {
    OGLPLUS_GLFUNC(NamedFramebufferTexture1DEXT)
    (get_raw_name(fbo),
     GLenum(fb_attach),
     GLenum(tx_target),
     get_raw_name(tex),
     level);
    OGLPLUS_VERIFY(
      NamedFramebufferTexture1DEXT,
      gl_object(fbo).
      // gl_enum_value(fb_target).
      gl_subject(tex),
      debug);
    return {};
}
#endif
#endif // GL_VERSION_3_0
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::framebuffer_texture_2d(
  framebuffer_target fb_target,
  framebuffer_attachment fb_attach,
  texture_target tx_target,
  texture_name tex,
  GLint level) noexcept {
    OGLPLUS_GLFUNC(FramebufferTexture2D)
    (GLenum(fb_target),
     GLenum(fb_attach),
     GLenum(tx_target),
     get_raw_name(tex),
     level);
    OGLPLUS_VERIFY(
      FramebufferTexture2D, gl_enum_value(fb_target).gl_subject(tex), debug);
    return {};
}
//------------------------------------------------------------------------------
#if defined(GL_EXT_direct_state_access)
inline outcome<void> framebuffer_ops::framebuffer_texture_2d(
  framebuffer_name fbo,
  framebuffer_attachment fb_attach,
  texture_target tx_target,
  texture_name tex,
  GLint level) noexcept {
    OGLPLUS_GLFUNC(NamedFramebufferTexture2DEXT)
    (get_raw_name(fbo),
     GLenum(fb_attach),
     GLenum(tx_target),
     get_raw_name(tex),
     level);
    OGLPLUS_VERIFY(
      NamedFramebufferTexture2DEXT,
      gl_object(fbo).
      // gl_enum_value(fb_target).
      gl_subject(tex),
      debug);
    return {};
}
#endif
//------------------------------------------------------------------------------
#if defined(GL_VERSION_3_0)
inline outcome<void> framebuffer_ops::framebuffer_texture_3d(
  framebuffer_target fb_target,
  framebuffer_attachment fb_attach,
  texture_target tx_target,
  texture_name tex,
  GLint level,
  GLint layer) noexcept {
    OGLPLUS_GLFUNC(FramebufferTexture3D)
    (GLenum(fb_target),
     GLenum(fb_attach),
     GLenum(tx_target),
     get_raw_name(tex),
     level,
     layer);
    OGLPLUS_VERIFY(
      FramebufferTexture3D, gl_enum_value(fb_target).gl_subject(tex), debug);
    return {};
}
//------------------------------------------------------------------------------
#if defined(GL_EXT_direct_state_access)
inline outcome<void> framebuffer_ops::framebuffer_texture_3d(
  framebuffer_name fbo,
  framebuffer_attachment fb_attach,
  texture_target tx_target,
  texture_name tex,
  GLint level,
  GLint layer) noexcept {
    OGLPLUS_GLFUNC(NamedFramebufferTexture3DEXT)
    (get_raw_name(fbo),
     GLenum(fb_attach),
     GLenum(tx_target),
     get_raw_name(tex),
     level,
     layer);
    OGLPLUS_VERIFY(
      NamedFramebufferTexture3DEXT,
      gl_object(fbo).
      // gl_enum_value(fb_target).
      gl_subject(tex),
      debug);
    return {};
}
#endif
#endif // GL_VERSION_3_0
//------------------------------------------------------------------------------
#if defined(GL_VERSION_3_0)
inline outcome<void> framebuffer_ops::framebuffer_texture_layer(
  framebuffer_target fb_target,
  framebuffer_attachment fb_attach,
  texture_name tex,
  GLint level,
  GLint layer) noexcept {
    OGLPLUS_GLFUNC(FramebufferTextureLayer)
    (GLenum(fb_target), GLenum(fb_attach), get_raw_name(tex), level, layer);
    OGLPLUS_VERIFY(
      FramebufferTextureLayer, gl_enum_value(fb_target).gl_subject(tex), debug);
    return {};
}
//------------------------------------------------------------------------------
#ifdef OGLPLUS_DSA_FRAMEBUFFER
inline outcome<void> framebuffer_ops::framebuffer_texture_layer(
  framebuffer_name fbo,
  framebuffer_attachment fb_attach,
  texture_name tex,
  GLint level,
  GLint layer) noexcept {
#ifdef GL_VERSION_4_5
    OGLPLUS_GLFUNC(NamedFramebufferTextureLayer)
    (
#else
    OGLPLUS_GLFUNC(NamedFramebufferTextureLayerEXT)
    (
#endif
      get_raw_name(fbo), GLenum(fb_attach), get_raw_name(tex), level, layer);
    OGLPLUS_VERIFY_STR(
      OGLPLUS_GL_DSA_FUNC_NAME(NamedFramebufferTextureLayer),
      gl_object(fbo).
      // gl_enum_value(fb_target).
      gl_subject(tex),
      debug);
    return {};
}
#endif
#endif // GL_VERSION_3_0
//------------------------------------------------------------------------------
#if defined(GL_VERSION_3_2)
inline outcome<void> framebuffer_ops::framebuffer_texture(
  framebuffer_target fb_target,
  framebuffer_attachment fb_attach,
  texture_name tex,
  GLint level) noexcept {
    OGLPLUS_GLFUNC(FramebufferTexture)
    (GLenum(fb_target), GLenum(fb_attach), get_raw_name(tex), level);
    OGLPLUS_VERIFY(
      FramebufferTexture, gl_enum_value(fb_target).gl_subject(tex), debug);
    return {};
}
//------------------------------------------------------------------------------
#if defined(GL_VERSION_4_5) || defined(GL_EXT_direct_state_access)
inline outcome<void> framebuffer_ops::framebuffer_texture(
  framebuffer_name fbo,
  framebuffer_attachment fb_attach,
  texture_name tex,
  GLint level) noexcept {
#ifdef GL_VERSION_4_5
    OGLPLUS_GLFUNC(NamedFramebufferTexture)
    (
#else
    OGLPLUS_GLFUNC(NamedFramebufferTextureEXT)
    (
#endif
      get_raw_name(fbo), GLenum(fb_attach), get_raw_name(tex), level);
    OGLPLUS_VERIFY_STR(
      OGLPLUS_GL_DSA_FUNC_NAME(NamedFramebufferTexture),
      gl_object(fbo).
      // gl_enum_value(fb_target).
      gl_subject(tex),
      debug);
    return {};
}
#endif
#endif // GL_VERSION_3_2
//------------------------------------------------------------------------------
#if defined(GL_VERSION_3_0)
inline outcome<void> framebuffer_ops::draw_buffer(color_buffer buf) noexcept {
    OGLPLUS_GLFUNC(DrawBuffer)(GLenum(buf));
    OGLPLUS_VERIFY(DrawBuffer, gl_enum_value(buf), debug);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::draw_buffers(
  enum_span<color_buffer> bufs) noexcept {
    OGLPLUS_GLFUNC(DrawBuffers)(GLsizei(bufs.size()), bufs.data());
    OGLPLUS_VERIFY_SIMPLE(DrawBuffers, debug);
    return {};
}
//------------------------------------------------------------------------------
#if defined(GL_VERSION_4_5) || defined(GL_EXT_direct_state_access)
inline outcome<void> framebuffer_ops::framebuffer_draw_buffer(
  framebuffer_name fbo, framebuffer_color_attachment buf) noexcept {
#ifdef GL_VERSION_4_5
    OGLPLUS_GLFUNC(NamedFramebufferDrawBuffer)
    (
#else
    OGLPLUS_GLFUNC(FramebufferDrawBufferEXT)
    (
#endif
      get_raw_name(fbo), GLenum(buf));
    OGLPLUS_VERIFY_STR(
      OGLPLUS_GL_DSA_FUNC_NAME(NamedFramebufferDrawBuffer),
      gl_object(fbo).gl_enum_value(buf),
      debug);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::framebuffer_draw_buffers(
  framebuffer_name fbo, enum_span<framebuffer_attachment> bufs) noexcept {
#ifdef GL_VERSION_4_5
    OGLPLUS_GLFUNC(NamedFramebufferDrawBuffers)
    (
#else
    OGLPLUS_GLFUNC(FramebufferDrawBuffersEXT)
    (
#endif
      get_raw_name(fbo), GLsizei(bufs.size()), bufs.data());
    OGLPLUS_VERIFY_STR(
      OGLPLUS_GL_DSA_FUNC_NAME(NamedFramebufferDrawBuffers),
      gl_object(fbo),
      debug);
    return {};
}
#endif
#endif // GL_VERSION_3_0
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::read_buffer(color_buffer buf) noexcept {
    OGLPLUS_GLFUNC(ReadBuffer)(GLenum(buf));
    OGLPLUS_VERIFY(ReadBuffer, gl_enum_value(buf), debug);
    return {};
}
//------------------------------------------------------------------------------
#if defined(GL_VERSION_4_5) || defined(GL_EXT_direct_state_access)
inline outcome<void> framebuffer_ops::framebuffer_read_buffer(
  framebuffer_name fbo, framebuffer_color_attachment buf) noexcept {
#ifdef GL_VERSION_4_5
    OGLPLUS_GLFUNC(NamedFramebufferReadBuffer)
    (
#else
    OGLPLUS_GLFUNC(FramebufferReadBufferEXT)
    (
#endif
      get_raw_name(fbo), GLenum(buf));
    OGLPLUS_VERIFY_STR(
      OGLPLUS_GL_DSA_FUNC_NAME(NamedFramebufferReadBuffer),
      gl_object(fbo).gl_enum_value(buf),
      debug);
    return {};
}
#endif
//------------------------------------------------------------------------------
#if defined(GL_VERSION_3_0)
inline outcome<void> framebuffer_ops::clear_buffer(
  framebuffer_buffer buf,
  GLint draw_buffer,
  span<const GLint> values) noexcept {
    OGLPLUS_GLFUNC(ClearBufferiv)(GLenum(buf), draw_buffer, values.data());
    OGLPLUS_VERIFY(
      ClearBufferiv, gl_enum_value(buf).gl_index(draw_buffer), debug);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::clear_buffer(
  framebuffer_buffer buf,
  GLint draw_buffer,
  span<const GLuint> values) noexcept {
    OGLPLUS_GLFUNC(ClearBufferuiv)(GLenum(buf), draw_buffer, values.data());
    OGLPLUS_VERIFY(
      ClearBufferuiv, gl_enum_value(buf).gl_index(draw_buffer), debug);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::clear_buffer(
  framebuffer_buffer buf,
  GLint draw_buffer,
  span<const GLfloat> values) noexcept {
    OGLPLUS_GLFUNC(ClearBufferfv)(GLenum(buf), draw_buffer, values.data());
    OGLPLUS_VERIFY(
      ClearBufferfv, gl_enum_value(buf).gl_index(draw_buffer), debug);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::clear_buffer(
  framebuffer_buffer buf,
  GLint draw_buffer,
  GLfloat depth,
  GLint stencil) noexcept {
    OGLPLUS_GLFUNC(ClearBufferfi)(GLenum(buf), draw_buffer, depth, stencil);
    OGLPLUS_VERIFY(
      ClearBufferfi, gl_enum_value(buf).gl_index(draw_buffer), debug);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::blit_framebuffer(
  GLint srcX0,
  GLint srcY0,
  GLint srcX1,
  GLint srcY1,
  GLint dstX0,
  GLint dstY0,
  GLint dstX1,
  GLint dstY1,
  enum_bitfield<buffer_select_bits> mask,
  blit_filter filter) noexcept {
    OGLPLUS_GLFUNC(BlitFramebuffer)
    (srcX0,
     srcY0,
     srcX1,
     srcY1,
     dstX0,
     dstY0,
     dstX1,
     dstY1,
     GLbitfield(mask),
     GLenum(filter));
    OGLPLUS_VERIFY(BlitFramebuffer, gl_enum_value(filter), debug);
    return {};
}
#endif
//------------------------------------------------------------------------------
#if defined(GL_VERSION_4_3)
inline outcome<void> framebuffer_ops::invalidate_framebuffer_data(
  framebuffer_target tgt,
  enum_span<framebuffer_attachment> attachments) noexcept {
    OGLPLUS_GLFUNC(InvalidateFramebuffer)
    (GLenum(tgt), GLsizei(attachments.size()), attachments.data());
    OGLPLUS_VERIFY(InvalidateFramebuffer, gl_enum_value(tgt), debug);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::invalidate_framebuffer_sub_data(
  framebuffer_target tgt,
  enum_span<framebuffer_attachment> attachments,
  GLint x,
  GLint y,
  GLint width,
  GLint height) noexcept {
    OGLPLUS_GLFUNC(InvalidateSubFramebuffer)
    (GLenum(tgt),
     GLsizei(attachments.size()),
     attachments.data(),
     x,
     y,
     width,
     height);
    OGLPLUS_VERIFY(InvalidateSubFramebuffer, gl_enum_value(tgt), debug);
    return {};
}
#endif
//------------------------------------------------------------------------------
#if defined(GL_VERSION_4_5)
inline outcome<void> framebuffer_ops::clear_framebuffer(
  framebuffer_name fbo,
  framebuffer_buffer buf,
  GLint draw_buffer,
  span<const GLint> values) noexcept {
    OGLPLUS_GLFUNC(ClearNamedFramebufferiv)
    (get_raw_name(fbo), GLenum(buf), draw_buffer, values.data());
    OGLPLUS_VERIFY_STR(
      OGLPLUS_GL_DSA_FUNC_NAME(NamedFramebufferReadBufferiv),
      gl_object(fbo).gl_enum_value(buf).gl_index(draw_buffer),
      debug);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::clear_framebuffer(
  framebuffer_name fbo,
  framebuffer_buffer buf,
  GLint draw_buffer,
  span<const GLuint> values) noexcept {
    OGLPLUS_GLFUNC(ClearNamedFramebufferuiv)
    (get_raw_name(fbo), GLenum(buf), draw_buffer, values.data());
    OGLPLUS_VERIFY_STR(
      OGLPLUS_GL_DSA_FUNC_NAME(NamedFramebufferReadBufferuiv),
      gl_object(fbo).gl_enum_value(buf).gl_index(draw_buffer),
      debug);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::clear_framebuffer(
  framebuffer_name fbo,
  framebuffer_buffer buf,
  GLint draw_buffer,
  span<const GLfloat> values) noexcept {
    OGLPLUS_GLFUNC(ClearNamedFramebufferfv)
    (get_raw_name(fbo),
     GLenum(buf),
     draw_buffer,
     const_cast<GLfloat*>(values.data()) // TODO:remove when fixed in GL APIs
    );
    OGLPLUS_VERIFY_STR(
      OGLPLUS_GL_DSA_FUNC_NAME(NamedFramebufferReadBufferfv),
      gl_object(fbo).gl_enum_value(buf).gl_index(draw_buffer),
      debug);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::clear_framebuffer(
  framebuffer_name fbo,
  framebuffer_buffer buf,
  GLint draw_buffer,
  GLfloat depth,
  GLint stencil) noexcept {
    OGLPLUS_GLFUNC(ClearNamedFramebufferfi)
    (get_raw_name(fbo), GLenum(buf), draw_buffer, depth, stencil);
    OGLPLUS_VERIFY_STR(
      OGLPLUS_GL_DSA_FUNC_NAME(NamedFramebufferReadBufferfi),
      gl_object(fbo).gl_enum_value(buf).gl_index(draw_buffer),
      debug);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::invalidate_framebuffer_data(
  framebuffer_name fbo,
  enum_span<framebuffer_attachment> attachments) noexcept {
    OGLPLUS_GLFUNC(InvalidateNamedFramebufferData)
    (get_raw_name(fbo), GLsizei(attachments.size()), attachments.data());
    OGLPLUS_VERIFY(InvalidateNamedFramebufferData, gl_object(fbo), debug);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::invalidate_framebuffer_sub_data(
  framebuffer_name fbo,
  enum_span<framebuffer_attachment> attachments,
  GLint x,
  GLint y,
  GLint width,
  GLint height) noexcept {
    OGLPLUS_GLFUNC(InvalidateNamedFramebufferSubData)
    (get_raw_name(fbo),
     GLsizei(attachments.size()),
     attachments.data(),
     x,
     y,
     width,
     height);
    OGLPLUS_VERIFY(InvalidateNamedFramebufferSubData, gl_object(fbo), debug);
    return {};
}
//------------------------------------------------------------------------------
inline outcome<void> framebuffer_ops::blit_framebuffer(
  framebuffer_name srcfbo,
  framebuffer_name dstfbo,
  GLint srcX0,
  GLint srcY0,
  GLint srcX1,
  GLint srcY1,
  GLint dstX0,
  GLint dstY0,
  GLint dstX1,
  GLint dstY1,
  enum_bitfield<buffer_select_bits> mask,
  blit_filter filter) noexcept {
    OGLPLUS_GLFUNC(BlitNamedFramebuffer)
    (get_raw_name(srcfbo),
     get_raw_name(dstfbo),
     srcX0,
     srcY0,
     srcX1,
     srcY1,
     dstX0,
     dstY0,
     dstX1,
     dstY1,
     GLbitfield(mask),
     GLenum(filter));
    OGLPLUS_VERIFY(
      BlitNamedFramebuffer,
      gl_subject(dstfbo).gl_object(srcfbo).gl_enum_value(filter),
      debug);
    return {};
}
#endif
//------------------------------------------------------------------------------
} // namespace oper
//------------------------------------------------------------------------------
// obj_gen_del_ops::_gen
//------------------------------------------------------------------------------
inline deferred_error_handler obj_gen_del_ops<tag::framebuffer>::_gen(
  span<GLuint> names) noexcept {
    OGLPLUS_GLFUNC(GenFramebuffers)(GLsizei(names.size()), names.data());
    OGLPLUS_VERIFY_SIMPLE(GenFramebuffers, debug);
    return {};
}
//------------------------------------------------------------------------------
// obj_gen_del_ops::_create
//------------------------------------------------------------------------------
#if defined(GL_VERSION_4_5)
inline deferred_error_handler obj_gen_del_ops<tag::framebuffer>::_create(
  span<GLuint> names) noexcept {
    OGLPLUS_GLFUNC(CreateFramebuffers)(GLsizei(names.size()), names.data());
    OGLPLUS_VERIFY_SIMPLE(CreateFramebuffers, debug);
    return {};
}
#endif
//------------------------------------------------------------------------------
// obj_gen_del_ops::_delete
//------------------------------------------------------------------------------
inline deferred_error_handler obj_gen_del_ops<tag::framebuffer>::_delete(
  span<GLuint> names) noexcept {
    OGLPLUS_GLFUNC(DeleteFramebuffers)(GLsizei(names.size()), names.data());
    OGLPLUS_VERIFY_SIMPLE(DeleteFramebuffers, debug);
    return {};
}
//------------------------------------------------------------------------------
// obj_gen_del_ops::_is_a
//------------------------------------------------------------------------------
inline outcome<boolean> obj_gen_del_ops<tag::framebuffer>::_is_a(
  GLuint name) noexcept {
    GLboolean res = OGLPLUS_GLFUNC(IsFramebuffer)(name);
    OGLPLUS_VERIFY_SIMPLE(IsFramebuffer, debug);
    return boolean(res);
}
//------------------------------------------------------------------------------
} // namespace oglplus
