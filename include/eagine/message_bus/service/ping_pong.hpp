/// @file eagine/message_bus/service/ping_pong.hpp
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_MESSAGE_BUS_SERVICE_PING_PONG_HPP
#define EAGINE_MESSAGE_BUS_SERVICE_PING_PONG_HPP

#include "../../bool_aggregate.hpp"
#include "../../maybe_unused.hpp"
#include "../serialize.hpp"
#include "../subscriber.hpp"
#include <chrono>
#include <vector>

namespace eagine::msgbus {
//------------------------------------------------------------------------------
template <typename Base = subscriber>
class pingable : public Base {
    using This = pingable;

protected:
    using Base::Base;

    void add_methods() {
        Base::add_methods();
        Base::add_method(
          this, EAGINE_MSG_MAP(eagiMsgBus, ping, This, _handle_ping));
    }

public:
    virtual auto respond_to_ping(
      identifier_t pinger_id,
      message_sequence_t,
      verification_bits) -> bool {
        EAGINE_MAYBE_UNUSED(pinger_id);
        return true;
    }

private:
    auto _handle_ping(const message_context&, stored_message& message) -> bool {
        if(respond_to_ping(
             message.source_id,
             message.sequence_no,
             this->verify_bits(message))) {
            this->bus().respond_to(message, EAGINE_MSGBUS_ID(pong), {});
        }
        return true;
    }
};
//------------------------------------------------------------------------------
template <typename Base = subscriber>
class pinger
  : public Base
  , protected std::chrono::steady_clock {

    using This = pinger;

    std::vector<std::tuple<identifier_t, message_sequence_t, timeout>>
      _pending{};

protected:
    using Base::Base;

    void add_methods() {
        Base::add_methods();
        Base::add_method(
          this, EAGINE_MSG_MAP(eagiMsgBus, pong, This, _handle_pong));
    }

public:
    static constexpr auto ping_msg_id() noexcept {
        return EAGINE_MSGBUS_ID(ping);
    }

    void query_pingables() {
        this->bus().query_subscribers_of(ping_msg_id());
    }

    void ping(identifier_t pingable_id, std::chrono::milliseconds max_time) {
        message_view message{};
        auto msg_id{EAGINE_MSGBUS_ID(ping)};
        message.set_target_id(pingable_id);
        this->bus().set_next_sequence_id(msg_id, message);
        this->bus().post(msg_id, message);
        _pending.emplace_back(message.target_id, message.sequence_no, max_time);
    }

    void ping(identifier_t pingable_id) {
        ping(pingable_id, std::chrono::milliseconds{5000});
    }

    auto update() -> bool {
        some_true something_done{};
        something_done(Base::update());

        _pending.erase(
          std::remove_if(
            _pending.begin(),
            _pending.end(),
            [this, &something_done](auto& entry) {
                auto& [pingable_id, sequence_no, ping_time] = entry;
                if(ping_time.is_expired()) {
                    on_ping_timeout(
                      pingable_id,
                      sequence_no,
                      std::chrono::duration_cast<std::chrono::microseconds>(
                        ping_time.elapsed_time()));
                    something_done();
                    return true;
                }
                return false;
            }),
          _pending.end());
        return something_done;
    }

    auto has_pending_pings() const noexcept -> bool {
        return !_pending.empty();
    }

    virtual void on_ping_response(
      identifier_t pingable_id,
      message_sequence_t sequence_no,
      std::chrono::microseconds age,
      verification_bits) = 0;

    virtual void on_ping_timeout(
      identifier_t pingable_id,
      message_sequence_t sequence_no,
      std::chrono::microseconds age) = 0;

private:
    auto _handle_pong(const message_context&, stored_message& message) -> bool {
        _pending.erase(
          std::remove_if(
            _pending.begin(),
            _pending.end(),
            [this, &message](auto& entry) {
                auto& [pingable_id, sequence_no, ping_time] = entry;
                const bool is_response = (message.source_id == pingable_id) &&
                                         (message.sequence_no == sequence_no);
                if(is_response) {
                    on_ping_response(
                      message.source_id,
                      message.sequence_no,
                      std::chrono::duration_cast<std::chrono::microseconds>(
                        ping_time.elapsed_time()),
                      this->verify_bits(message));
                    return true;
                }
                return false;
            }),
          _pending.end());
        return true;
    }
};
//------------------------------------------------------------------------------
} // namespace eagine::msgbus

#endif // EAGINE_MESSAGE_BUS_SERVICE_PING_PONG_HPP
