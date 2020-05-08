/**
 *  @file eagine/message_bus/context_fwd.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_MESSAGE_BUS_CONTEXT_FWD_HPP
#define EAGINE_MESSAGE_BUS_CONTEXT_FWD_HPP

#include "../logging/logger.hpp"
#include "../program_args.hpp"
#include <memory>

namespace eagine {
namespace msgbus {
//------------------------------------------------------------------------------
class context;
//------------------------------------------------------------------------------
using shared_context = std::shared_ptr<context>;
//------------------------------------------------------------------------------
std::unique_ptr<context> make_context(logger&);
std::unique_ptr<context> make_context(logger&, const program_args&);
//------------------------------------------------------------------------------
} // namespace msgbus
} // namespace eagine

#endif // EAGINE_MESSAGE_BUS_CONTEXT_FWD_HPP
