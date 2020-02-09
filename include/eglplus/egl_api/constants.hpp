/**
 *  @file eglplus/egl_api/constants.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef EGLPLUS_EGL_API_CONSTANTS_HPP
#define EGLPLUS_EGL_API_CONSTANTS_HPP

#include "c_api.hpp"
#include "enum_types.hpp"

namespace eagine {
namespace eglp {
//------------------------------------------------------------------------------
template <typename ApiTraits>
struct basic_egl_constants {
public:
    using enum_type = typename egl_types::enum_type;
    using enum_type_i = identity<enum_type>;
    template <enum_type value>
    using enum_type_c = std::integral_constant<enum_type, value>;

    opt_c_api_constant<
      mp_list<egl_error_code>,
#ifdef EGL_SUCCESS
      enum_type_c<EGL_SUCCESS>>
#else
      enum_type_i>
#endif
      success;

    opt_c_api_constant<
      mp_list<string_query>,
#ifdef EGL_CLIENT_APIS
      enum_type_c<EGL_CLIENT_APIS>>
#else
      enum_type_i>
#endif
      client_apis;

    opt_c_api_constant<
      mp_list<string_query>,
#ifdef EGL_VENDOR
      enum_type_c<EGL_VENDOR>>
#else
      enum_type_i>
#endif
      vendor;

    opt_c_api_constant<
      mp_list<string_query>,
#ifdef EGL_VERSION
      enum_type_c<EGL_VERSION>>
#else
      enum_type_i>
#endif
      version;

    opt_c_api_constant<
      mp_list<string_query>,
#ifdef EGL_EXTENSIONS
      enum_type_c<EGL_EXTENSIONS>>
#else
      enum_type_i>
#endif
      extensions;

    opt_c_api_constant<
      mp_list<eglp::config_caveat>,
#ifdef EGL_NONE
      enum_type_c<EGL_NONE>>
#else
      enum_type_i>
#endif
      none;

    basic_egl_constants(ApiTraits& traits, basic_egl_c_api<ApiTraits>& api)
      : success("SUCCESS", traits, api)
      , client_apis("CLIENT_APIS", traits, api)
      , vendor("VENDOR", traits, api)
      , version("VERSION", traits, api)
      , extensions("EXTENSIONS", traits, api)
      , none("NONE", traits, api) {
    }
};
//------------------------------------------------------------------------------
} // namespace eglp
} // namespace eagine

#endif // EGLPLUS_EGL_API_CONSTANTS_HPP

