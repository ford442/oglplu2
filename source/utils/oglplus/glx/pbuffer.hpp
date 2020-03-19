/**
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef UTILS_OGLPLUS_GLX_PBUFFER_1107121519_HPP
#define UTILS_OGLPLUS_GLX_PBUFFER_1107121519_HPP

#include "../x11/display.hpp"
#include "fb_config.hpp"

#include <GL/glx.h>
#include <stdexcept>

namespace eagine {
namespace oglp {
namespace glx {

class Pbuffer
  : public x11::DisplayObject<::GLXPbuffer, void(::Display*, ::GLXPbuffer)> {
public:
    Pbuffer(
      const x11::Display& display,
      const glx::FBConfig& fbc,
      const int* attrib_list)
      : x11::DisplayObject<::GLXPbuffer, void(::Display*, ::GLXPbuffer)>(
          display,
          ::glXCreatePbuffer(display, fbc, attrib_list),
          ::glXDestroyPbuffer,
          "Error creating glX Pbuffer") {
    }
};

} // namespace glx
} // namespace oglp
} // namespace eagine

#endif // include guard
