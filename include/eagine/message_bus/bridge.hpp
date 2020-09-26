/**
 *  @file eagine/message_bus/bridge.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_MESSAGE_BUS_BRIDGE_HPP
#define EAGINE_MESSAGE_BUS_BRIDGE_HPP

#include "../logging/logger.hpp"
#include "../program_args.hpp"
#include "../timeout.hpp"
#include "connection.hpp"
#include "context_fwd.hpp"
#include <memory>

namespace eagine::msgbus {
//------------------------------------------------------------------------------
class bridge_state;
class bridge : public connection_user {

    static constexpr auto invalid_id() noexcept -> identifier_t {
        return 0U;
    }

    static constexpr auto is_valid_id(identifier_t id) noexcept -> bool {
        return id != invalid_id();
    }

public:
    bridge(logger& parent) noexcept
      : _log(EAGINE_ID(MsgBusBrdg), parent)
      , _context{make_context(_log)} {}

    bridge(logger& parent, const program_args& args) noexcept
      : _log(EAGINE_ID(MsgBusBrdg), parent)
      , _context{make_context(_log, args)} {
        _setup_from_args(args);
    }

    void add_certificate_pem(memory::const_block blk);
    void add_ca_certificate_pem(memory::const_block blk);

    auto add_connection(std::unique_ptr<connection>) -> bool final;

    auto has_id() const noexcept -> bool {
        return is_valid_id(_id);
    }

    auto update() -> bool;

    auto no_connection_timeout() const noexcept -> auto& {
        return _no_connection_timeout;
    }

    auto is_done() const noexcept -> bool {
        return bool(no_connection_timeout());
    }

private:
    void _setup_from_args(const program_args&);

    auto _check_state() -> bool;
    auto _update_connections() -> bool;

    auto _do_send(message_id, message_view) -> bool;
    auto _send(message_id, message_view) -> bool;
    auto _handle_special(message_id, message_view, bool) -> bool;
    auto _do_push(message_id, message_view) -> bool;
    auto _forward_messages() -> bool;

    logger _log{};
    shared_context _context{};

    identifier_t _id{invalid_id()};
    timeout _no_id_timeout{std::chrono::seconds{2}, nothing};

    std::intmax_t _forwarded_messages_i2c{0};
    std::intmax_t _forwarded_messages_c2o{0};

    std::shared_ptr<bridge_state> _state{};
    timeout _no_connection_timeout{std::chrono::seconds{30}};
    std::vector<std::unique_ptr<connection>> _connections{};
};
//------------------------------------------------------------------------------
} // namespace eagine::msgbus

#if !EAGINE_LINK_LIBRARY || defined(EAGINE_IMPLEMENTING_LIBRARY)
#include <eagine/message_bus/bridge.inl>
#endif

#endif // EAGINE_MESSAGE_BUS_BRIDGE_HPP
