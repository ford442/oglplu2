/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_MESSAGE_BUS_SUBSCRIBER_HPP
#define EAGINE_MESSAGE_BUS_SUBSCRIBER_HPP

#include "../interface.hpp"
#include "../maybe_unused.hpp"
#include "../span.hpp"
#include "endpoint.hpp"
#include "handler_map.hpp"
#include "verification.hpp"
#include <array>
#include <type_traits>
#include <vector>

namespace eagine::msgbus {
//------------------------------------------------------------------------------
/// @brief Base class for message bus subscribers.
/// @ingroup msgbus
/// @see static_subscriber
/// @see subscriber
/// @see endpoint
class subscriber_base {
public:
    /// @brief Tests if this subscribes has an associated endpoint and is usable.
    explicit operator bool() noexcept {
        return _endpoint != nullptr;
    }

    /// @brief Returns a reference to the associated endpoint.
    /// @pre bool(*this)
    auto bus() noexcept -> auto& {
        EAGINE_ASSERT(_endpoint != nullptr);
        return *_endpoint;
    }

    /// @brief Returns a const reference to the associated endpoint.
    /// @pre bool(*this)
    auto bus() const noexcept -> auto& {
        EAGINE_ASSERT(_endpoint != nullptr);
        return *_endpoint;
    }

    /// @brief Updates the internal endpoint state (should be called repeatedly).
    auto update() const noexcept -> bool {
        if(EAGINE_LIKELY(_endpoint)) {
            return _endpoint->update();
        }
        return false;
    }

    /// @brief Uses the associated endpoint to verify the specified message.
    auto verify_bits(const stored_message& message) noexcept
      -> verification_bits {
        if(EAGINE_LIKELY(_endpoint)) {
            return message.verify_bits(_endpoint->ctx(), *_endpoint);
        }
        return {};
    }

    /// @brief Queries the subscriptions of the remote endpoint with the specified id.
    /// @see query_subscribers_of
    void query_subscriptions_of(identifier_t target_id) {
        if(EAGINE_LIKELY(_endpoint)) {
            _endpoint->query_subscriptions_of(target_id);
        }
    }

    /// @brief Queries remote nodes subscribing to the specified message.
    /// @see query_subscriptions_of
    void query_subscribers_of(message_id sub_msg) {
        if(EAGINE_LIKELY(_endpoint)) {
            _endpoint->query_subscribers_of(sub_msg);
        }
    }

    /// @brief Not copy assignable.
    subscriber_base(const subscriber_base&) = delete;

    /// @brief Not move assignable.
    auto operator=(subscriber_base&&) = delete;

    /// @brief Not copy constructible.
    auto operator=(const subscriber_base&) = delete;

protected:
    using method_handler = typename endpoint::method_handler;
    struct handler_entry {
        message_id msg_id{};
        method_handler handler{};

        constexpr handler_entry() noexcept = default;

        constexpr handler_entry(message_id id, method_handler hndlr) noexcept
          : msg_id{std::move(id)}
          , handler{std::move(hndlr)} {}

        template <
          identifier_t ClassId,
          identifier_t MethodId,
          typename Class,
          bool (Class::*HandlerFunc)(const message_context&, stored_message&)>
        handler_entry(
          Class* instance,
          static_message_handler_map<
            static_message_id<ClassId, MethodId>,
            member_function_constant<
              bool (Class::*)(const message_context&, stored_message&),
              HandlerFunc>> msg_map) noexcept
          : msg_id{ClassId, MethodId}
          , handler{instance, msg_map.method()} {}

        template <
          identifier_t ClassId,
          identifier_t MethodId,
          typename Class,
          bool (Class::*HandlerFunc)(const message_context&, stored_message&)>
        handler_entry(
          Class* instance,
          static_message_handler_map<
            static_message_id<ClassId, MethodId>,
            member_function_constant<
              bool (Class::*)(const message_context&, stored_message&) const,
              HandlerFunc>> msg_map) noexcept
          : msg_id{ClassId, MethodId}
          , handler{instance, msg_map.method()} {}
    };

    ~subscriber_base() noexcept = default;
    constexpr subscriber_base() noexcept = default;
    constexpr subscriber_base(endpoint& bus) noexcept
      : _endpoint{&bus} {}
    subscriber_base(subscriber_base&& temp) noexcept
      : _endpoint{temp._endpoint} {
        temp._endpoint = nullptr;
    }

    void _subscribe_to(span<const handler_entry> msg_handlers) const {
        if(EAGINE_LIKELY(_endpoint)) {
            for(auto& entry : msg_handlers) {
                _endpoint->subscribe(entry.msg_id);
            }
        }
    }

    void
    _unsubscribe_from(span<const handler_entry> msg_handlers) const noexcept {
        if(_endpoint) {
            for(auto& entry : msg_handlers) {
                try {
                    _endpoint->unsubscribe(entry.msg_id);
                } catch(...) {
                }
            }
        }
    }

    void _announce_subscriptions(span<const handler_entry> msg_handlers) const {
        if(EAGINE_LIKELY(_endpoint)) {
            for(auto& entry : msg_handlers) {
                _endpoint->say_subscribes_to(entry.msg_id);
            }
        }
    }

    void _allow_subscriptions(span<const handler_entry> msg_handlers) const {
        if(EAGINE_LIKELY(_endpoint)) {
            for(auto& entry : msg_handlers) {
                _endpoint->allow_message_type(entry.msg_id);
            }
        }
    }

    void _retract_subscriptions(
      span<const handler_entry> msg_handlers) const noexcept {
        if(EAGINE_LIKELY(_endpoint)) {
            for(auto& entry : msg_handlers) {
                try {
                    _endpoint->say_unsubscribes_from(entry.msg_id);
                } catch(...) {
                }
            }
        }
    }

    void _respond_to_subscription_query(
      identifier_t source_id,
      span<const handler_entry> msg_handlers) const {
        if(EAGINE_LIKELY(_endpoint)) {
            for(auto& entry : msg_handlers) {
                _endpoint->say_subscribes_to(source_id, entry.msg_id);
            }
        }
    }

    void _respond_to_subscription_query(
      identifier_t source_id,
      message_id sub_msg,
      span<const handler_entry> msg_handlers) const {
        if(EAGINE_LIKELY(_endpoint)) {
            for(auto& entry : msg_handlers) {
                if(entry.msg_id == sub_msg) {
                    _endpoint->say_subscribes_to(source_id, sub_msg);
                    return;
                }
            }
            _endpoint->say_not_subscribed_to(source_id, sub_msg);
        }
    }

    auto _process_one(span<const handler_entry> msg_handlers) -> bool {
        for(auto& entry : msg_handlers) {
            if(bus().process_one(entry.msg_id, entry.handler)) {
                return true;
            }
        }
        return false;
    }

    auto _process_all(span<const handler_entry> msg_handlers) -> span_size_t {
        span_size_t result{0};
        for(auto& entry : msg_handlers) {
            result += bus().process_all(entry.msg_id, entry.handler);
        }
        return result;
    }

    void _finish() noexcept {
        if(EAGINE_LIKELY(_endpoint)) {
            try {
                _endpoint->finish();
            } catch(...) {
            }
        }
    }

private:
    endpoint* _endpoint{nullptr};
};
//------------------------------------------------------------------------------
/// @brief Template for subscribers with predefined count of handled message types.
/// @ingroup msgbus
template <std::size_t N>
class static_subscriber : public subscriber_base {
public:
    /// @brief Alias for the method/message handler callable reference.
    using method_handler = typename endpoint::method_handler;

    using handler_entry = typename subscriber_base::handler_entry;

    template <
      typename... MsgHandlers,
      typename = std::enable_if_t<sizeof...(MsgHandlers) == N>>
    static_subscriber(endpoint& bus, MsgHandlers&&... msg_handlers)
      : subscriber_base{bus}
      , _msg_handlers{{std::forward<MsgHandlers>(msg_handlers)...}} {
        this->_subscribe_to(view(_msg_handlers));
    }

    /// @brief Construction from a reference to endpoint and some message maps.
    template <
      typename Class,
      typename... MsgMaps,
      typename = std::enable_if_t<sizeof...(MsgMaps) == N>>
    static_subscriber(endpoint& bus, Class* instance, MsgMaps... msg_maps)
      : static_subscriber(bus, handler_entry(instance, msg_maps)...) {}

    /// @brief Not move constructible.
    static_subscriber(static_subscriber&& temp) = delete;

    /// @brief Not copy constructible.
    static_subscriber(const static_subscriber&) = delete;

    /// @brief Not move assignable.
    auto operator=(static_subscriber&&) = delete;

    /// @brief Not copy assignable.
    auto operator=(const static_subscriber&) = delete;

    ~static_subscriber() noexcept {
        this->_unsubscribe_from(view(_msg_handlers));
    }

    /// @brief Processes one pending enqueued message.
    auto process_one() -> bool {
        return this->_process_one(view(_msg_handlers));
    }

    /// @brief Processes all pending enqueued messages.
    auto process_all() -> span_size_t {
        return this->_process_all(view(_msg_handlers));
    }

    /// @brief Sends messages to the bus saying which messages this can handle.
    /// @see retract_subscriptions
    /// @see respond_to_subscription_query
    void announce_subscriptions() const {
        this->_announce_subscriptions(view(_msg_handlers));
    }

    void allow_subscriptions() const {
        this->_allow_subscriptions(view(_msg_handlers));
    }

    /// @brief Sends messages to the bus saying which messages this cannot handle.
    /// @see announce_subscriptions
    /// @see respond_to_subscription_query
    void retract_subscriptions() const noexcept {
        this->_retract_subscriptions(view(_msg_handlers));
    }

    /// @brief Sends messages responding to a subscription query.
    /// @see retract_subscriptions
    /// @see announce_subscriptions
    void respond_to_subscription_query(identifier_t source_id) const noexcept {
        this->_respond_to_subscription_query(source_id, view(_msg_handlers));
    }

    /// @brief Sends messages responding to a subscription query.
    /// @see retract_subscriptions
    /// @see announce_subscriptions
    void respond_to_subscription_query(
      identifier_t source_id,
      message_id sub_msg) const noexcept {
        this->_respond_to_subscription_query(
          source_id, sub_msg, view(_msg_handlers));
    }

private:
    std::array<handler_entry, N> _msg_handlers;
};
//------------------------------------------------------------------------------
class subscriber
  : public interface<subscriber>
  , public subscriber_base {
public:
    using handler_entry = subscriber_base::handler_entry;
    using method_handler =
      callable_ref<bool(const message_context&, stored_message&)>;

    subscriber(endpoint& bus) noexcept
      : subscriber_base{bus} {}

    template <
      typename Class,
      bool (Class::*Method)(const message_context&, stored_message&)>
    void add_method(
      Class* instance,
      message_id msg_id,
      member_function_constant<
        bool (Class::*)(const message_context&, stored_message&),
        Method> method) {
        _msg_handlers.emplace_back(msg_id, method_handler{instance, method});
    }

    template <
      typename Class,
      bool (Class::*Method)(const message_context&, stored_message&)>
    void add_method(
      Class* instance,
      message_handler_map<member_function_constant<
        bool (Class::*)(const message_context&, stored_message&),
        Method>> msg_map) noexcept {
        add_method(instance, msg_map.msg_id(), msg_map.method());
    }

    template <
      typename Class,
      bool (Class::*Method)(const message_context&, stored_message&)>
    void add_method(std::tuple<
                    Class*,
                    message_handler_map<member_function_constant<
                      bool (Class::*)(const message_context&, stored_message&),
                      Method>>> imm) noexcept {
        add_method(
          std::get<0>(imm),
          std::get<1>(imm).msg_id(),
          std::get<1>(imm).method());
    }

    template <
      typename Class,
      bool (Class::*Method)(const message_context&, stored_message&),
      identifier_t ClassId,
      identifier_t MethodId>
    void add_method(
      Class* instance,
      static_message_handler_map<
        static_message_id<ClassId, MethodId>,
        member_function_constant<
          bool (Class::*)(const message_context&, stored_message&),
          Method>> msg_map) noexcept {
        add_method(instance, msg_map.msg_id(), msg_map.method());
    }

    auto process_one() -> bool {
        return this->_process_one(view(_msg_handlers));
    }

    auto process_all() -> span_size_t {
        return this->_process_all(view(_msg_handlers));
    }

    void announce_subscriptions() const {
        this->_announce_subscriptions(view(_msg_handlers));
    }

    void allow_subscriptions() const {
        this->_allow_subscriptions(view(_msg_handlers));
    }

    void retract_subscriptions() const noexcept {
        this->_retract_subscriptions(view(_msg_handlers));
    }

    void respond_to_subscription_query(identifier_t source_id) const noexcept {
        this->_respond_to_subscription_query(source_id, view(_msg_handlers));
    }

    void respond_to_subscription_query(
      identifier_t source_id,
      message_id sub_msg) const noexcept {
        this->_respond_to_subscription_query(
          source_id, sub_msg, view(_msg_handlers));
    }

protected:
    void add_methods() {}

    void init() {
        this->_subscribe_to(view(_msg_handlers));
    }

    void finish() noexcept {
        this->_unsubscribe_from(view(_msg_handlers));
        this->_finish();
    }

private:
    std::vector<handler_entry> _msg_handlers;
};
//------------------------------------------------------------------------------
} // namespace eagine::msgbus

#endif // EAGINE_MESSAGE_BUS_SUBSCRIBER_HPP
