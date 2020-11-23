/**
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_INTEROP_GLX_CONTEXT_HPP
#define EAGINE_INTEROP_GLX_CONTEXT_HPP

#include "../x11/display.hpp"
#include "../x11/window.hpp"
#include "drawable.hpp"
#include "fb_config.hpp"

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <stdexcept>

namespace eagine::glx {

class Context
  : public x11::DisplayObject<::GLXContext, void(::Display*, ::GLXContext)> {
private:
    static auto make_context(
      const x11::Display& display,
      const FBConfig& fbc,
      int version_major,
      int version_minor,
      bool debugging,
      bool compatibility,
      ::GLXContext share_context = ::GLXContext(nullptr)) -> ::GLXContext {
        using glXCreateContextAttribsARBProc = GLXContext (*)(
          ::Display*, ::GLXFBConfig, ::GLXContext, Bool, const int*);

        glXCreateContextAttribsARBProc glXCreateContextAttribsARB = nullptr;
        glXCreateContextAttribsARB =
          reinterpret_cast<glXCreateContextAttribsARBProc>(glXGetProcAddressARB(
            reinterpret_cast<const GLubyte*>("glXCreateContextAttribsARB")));

        const int CONTEXT_MAJOR_VERSION_ARB = 0x2091;
        const int CONTEXT_MINOR_VERSION_ARB = 0x2092;
        const int CONTEXT_FLAGS_ARB = 0x2094;
        const int CONTEXT_PROFILE_MASK_ARB = 0x9126;
        const int CONTEXT_DEBUG_BIT_ARB = 0x0001;
        const int CONTEXT_CORE_PROFILE_BIT_ARB = 0x00000001;
        const int CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB = 0x00000002;

        ::GLXContext res{};

        if(compatibility) {
            int context_attribs[] = {
              CONTEXT_MAJOR_VERSION_ARB,
              version_major,
              CONTEXT_MINOR_VERSION_ARB,
              version_minor,
              CONTEXT_FLAGS_ARB,
              (debugging ? CONTEXT_DEBUG_BIT_ARB : 0),
              CONTEXT_PROFILE_MASK_ARB,
              (compatibility ? CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
                             : CONTEXT_CORE_PROFILE_BIT_ARB),
              None};
            res = glXCreateContextAttribsARB(
              display,
              fbc.Handle(),
              share_context,
              True,
              static_cast<const int*>(context_attribs));
        } else {
            int context_attribs[] = {
              CONTEXT_MAJOR_VERSION_ARB,
              version_major,
              CONTEXT_MINOR_VERSION_ARB,
              version_minor,
              CONTEXT_FLAGS_ARB,
              (debugging ? CONTEXT_DEBUG_BIT_ARB : 0),
              None};
            res = glXCreateContextAttribsARB(
              display,
              fbc.Handle(),
              share_context,
              True,
              static_cast<const int*>(context_attribs));
        }
        ::XSync(display, False);
        return res;
    }

public:
    Context(
      const x11::Display& display,
      const FBConfig& fbc,
      int version_major,
      int version_minor,
      bool debugging = true,
      bool compatibility = false)
      : x11::DisplayObject<::GLXContext, void(::Display*, ::GLXContext)>(
          display,
          make_context(
            display,
            fbc,
            version_major,
            version_minor,
            debugging,
            compatibility),
          ::glXDestroyContext,
          "Error creating glX context") {}

    Context(
      const x11::Display& display,
      const FBConfig& fbc,
      const Context& share_context,
      int version_major,
      int version_minor,
      bool debugging = true,
      bool compatibility = false)
      : x11::DisplayObject<::GLXContext, void(::Display*, ::GLXContext)>(
          display,
          make_context(
            display,
            fbc,
            version_major,
            version_minor,
            debugging,
            compatibility,
            share_context.Handle()),
          ::glXDestroyContext,
          "Error creating sharing glX context") {}

    void MakeCurrent(const Drawable& drawable) const {
        ::glXMakeCurrent(this->DisplayRef(), drawable, this->Handle());
    }

    static void Release(const x11::Display& display) {
        ::glXMakeCurrent(display, 0, nullptr);
    }

    void SwapBuffers(const Drawable& drawable) const {
        ::glXSwapBuffers(this->DisplayRef(), drawable);
    }
};

} // namespace eagine::glx

#endif
