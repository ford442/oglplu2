/**
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef UTILS_OGLPLUS_GLX_DRAWABLE_1107121519_HPP
#define UTILS_OGLPLUS_GLX_DRAWABLE_1107121519_HPP

#include "../x11/display.hpp"

#include <GL/glx.h>
#include <stdexcept>

namespace eagine {
namespace oglp {
namespace glx {

using Drawable = x11::BaseDisplayObject<::GLXDrawable>;

} // namespace glx
} // namespace oglp
} // namespace eagine

#endif // include guard
