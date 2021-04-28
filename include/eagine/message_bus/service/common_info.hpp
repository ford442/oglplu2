/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_MESSAGE_BUS_SERVICE_COMMON_INFO_HPP
#define EAGINE_MESSAGE_BUS_SERVICE_COMMON_INFO_HPP

#include "application_info.hpp"
#include "build_info.hpp"
#include "compiler_info.hpp"
#include "endpoint_info.hpp"
#include "host_info.hpp"

namespace eagine::msgbus {

template <typename Base = subscriber>
using common_info_providers = compiler_info_provider<build_info_provider<
  host_info_provider<application_info_provider<endpoint_info_provider<Base>>>>>;

template <typename Base = subscriber>
using common_info_consumers = compiler_info_consumer<build_info_consumer<
  host_info_consumer<application_info_consumer<endpoint_info_consumer<Base>>>>>;

} // namespace eagine::msgbus

#endif // EAGINE_MESSAGE_BUS_SERVICE_COMMON_INFO_HPP