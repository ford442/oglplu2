/**
 *  @file eglplus/egl_api/objects.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef EGLPLUS_EGL_API_OBJECTS_HPP
#define EGLPLUS_EGL_API_OBJECTS_HPP

#include "config.hpp"
#include <eagine/handle.hpp>
#include <eagine/message_id.hpp>

namespace eagine::eglp {
//------------------------------------------------------------------------------
using display_tag = EAGINE_MSG_TYPE(egl, Display);
using surface_tag = EAGINE_MSG_TYPE(egl, Surface);
//------------------------------------------------------------------------------
#if defined(EGL_NO_DISPLAY)
using display_handle =
  basic_handle<display_tag, egl_types::display_type, EGL_NO_DISPLAY>;
#else
using display_handle = basic_handle<display_tag, nothing_t>;
#endif
//------------------------------------------------------------------------------
#if defined(EGL_NO_SURFACE)
using surface_handle =
  basic_handle<surface_tag, egl_types::surface_type, EGL_NO_SURFACE>;
#else
using surface_handle = basic_handle<surface_tag, nothing_t>;
#endif
//------------------------------------------------------------------------------
} // namespace eagine::eglp

#endif // EGLPLUS_EGL_API_OBJECTS_HPP
