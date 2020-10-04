/**
 *  @file eagine/message_bus/endpoint.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_MESSAGE_BUS_ENDPOINT_HPP
#define EAGINE_MESSAGE_BUS_ENDPOINT_HPP

#include "../flat_map.hpp"
#include "../logging/logger.hpp"
#include "../timeout.hpp"
#include "blobs.hpp"
#include "connection.hpp"
#include "context_fwd.hpp"
#include "serialize.hpp"
#include <tuple>
#include <vector>

namespace eagine::msgbus {
//------------------------------------------------------------------------------
class friend_of_endpoint;
//------------------------------------------------------------------------------
class endpoint : public connection_user {
public:
    static constexpr auto invalid_id() noexcept -> identifier_t {
        return 0U;
    }

    static constexpr auto is_valid_id(identifier_t id) noexcept -> bool {
        return id != invalid_id();
    }

    using fetch_handler = connection::fetch_handler;
    using blob_filter_function = blob_manipulator::filter_function;

private:
    friend class friend_of_endpoint;

    logger _log{};

    shared_context _context{make_context(_log)};

    identifier_t _id{invalid_id()};
    timeout _no_id_timeout{std::chrono::seconds{2}, nothing};

    std::vector<std::unique_ptr<connection>> _connections{};

    message_storage _outgoing{};

    flat_map<message_id, std::tuple<span_size_t, message_priority_queue>>
      _incoming{};

    template <typename Entry>
    static inline auto _get_counter(Entry& entry) -> auto& {
        return std::get<0>(std::get<1>(entry));
    }

    template <typename Entry>
    static inline auto _get_queue(Entry& entry) -> auto& {
        return std::get<1>(std::get<1>(entry));
    }

    blob_manipulator _blobs{_log};
    blob_manipulator::filter_function _allow_blob{};

    auto _cleanup_blobs() -> bool;
    auto _process_blobs() -> bool;
    auto _do_allow_blob(message_id) -> bool;

    auto _default_store_handler() noexcept -> fetch_handler {
        return fetch_handler{this, EAGINE_MEM_FUNC_C(endpoint, _store_message)};
    }

    fetch_handler _store_handler{_default_store_handler()};

    auto _do_send(message_id msg_id, message_view) -> bool;

    auto
    _handle_send(message_id msg_id, message_age, const message_view& message)
      -> bool {
        // TODO: use message age
        return _do_send(msg_id, message);
    }

    auto _handle_post(message_id msg_id, const message_view& message) -> bool {
        return post(msg_id, message);
    }

    auto _handle_special(message_id msg_id, const message_view&) noexcept
      -> bool;

    auto _store_message(message_id msg_id, message_age, const message_view&)
      -> bool;

    auto _accept_message(message_id msg_id, const message_view&) -> bool;

    explicit endpoint(logger log, fetch_handler store_message) noexcept
      : _log{std::move(log)}
      , _store_handler{std::move(store_message)} {}

    explicit endpoint(
      logger log,
      blob_filter_function allow_blob,
      fetch_handler store_message) noexcept
      : _log{std::move(log)}
      , _allow_blob{std::move(allow_blob)}
      , _store_handler{std::move(store_message)} {}

    endpoint(endpoint&& temp) noexcept
      : _log{std::move(temp._log)}
      , _context{std::move(temp._context)}
      , _id{temp._id}
      , _connections{std::move(temp._connections)}
      , _outgoing{std::move(temp._outgoing)}
      , _incoming{std::move(temp._incoming)}
      , _blobs{std::move(temp._blobs)} {
        temp._id = invalid_id();
    }

    endpoint(
      endpoint&& temp,
      blob_filter_function allow_blob,
      fetch_handler store_message) noexcept
      : _log{std::move(temp._log)}
      , _context{std::move(temp._context)}
      , _id{temp._id}
      , _connections{std::move(temp._connections)}
      , _outgoing{std::move(temp._outgoing)}
      , _incoming{std::move(temp._incoming)}
      , _blobs{std::move(temp._blobs)}
      , _allow_blob{std::move(allow_blob)}
      , _store_handler{std::move(store_message)} {
        temp._id = invalid_id();
    }

public:
    endpoint() = default;

    endpoint(logger log) noexcept
      : _log{std::move(log)} {}

    explicit endpoint(logger log, blob_filter_function allow_blob) noexcept
      : _log{std::move(log)}
      , _allow_blob{std::move(allow_blob)} {}

    endpoint(logger log, const program_args& args) noexcept
      : _log{std::move(log)}
      , _context{make_context(_log, args)} {}

    endpoint(const endpoint&) = delete;
    auto operator=(endpoint&&) = delete;
    auto operator=(const endpoint&) = delete;

    auto ctx() noexcept -> context& {
        EAGINE_ASSERT(_context);
        return *_context;
    }

    auto log() noexcept -> logger& {
        return _log;
    }

    ~endpoint() noexcept override = default;

    auto set_id(identifier id) -> auto& {
        _id = id.value();
        return *this;
    }

    auto has_id() const noexcept -> bool {
        return is_valid_id(_id);
    }

    auto get_id() const noexcept {
        return _id;
    }

    void add_certificate_pem(memory::const_block blk);
    void add_ca_certificate_pem(memory::const_block blk);

    auto add_connection(std::unique_ptr<connection> conn) -> bool final;

    auto is_usable() const -> bool;

    auto max_data_size() const -> valid_if_positive<span_size_t>;

    void flush_outbox();

    auto update() -> bool;

    void finish() {
        say_bye();
        flush_outbox();
    }

    void subscribe(message_id);
    void unsubscribe(message_id);

    auto set_next_sequence_id(message_id, message_info&) -> bool;

    auto post(message_id msg_id, message_view message) -> bool {
        _outgoing.push(msg_id, message);
        return true;
    }

    auto post_signed(message_id, message_view message) -> bool;

    template <typename T>
    auto post_value(message_id msg_id, T& value, const message_info& info = {})
      -> bool {
        if(const auto opt_size = max_data_size()) {
            const auto max_size = extract(opt_size);
            return _outgoing.push_if(
              [this, msg_id, &info, &value, max_size](
                message_id& dst_msg_id, stored_message& message) {
                  if(message.store_value(value, max_size)) {
                      message.assign(info);
                      dst_msg_id = msg_id;
                      return true;
                  }
                  return false;
              },
              max_size);
        }
        return false;
    }

    auto post_blob(
      message_id msg_id,
      identifier_t target_id,
      memory::const_block blob,
      std::chrono::seconds max_time,
      message_priority priority) -> bool {
        _blobs.push_outgoing(msg_id, _id, target_id, blob, max_time, priority);
        return true;
    }

    auto broadcast_blob(
      message_id msg_id,
      memory::const_block blob,
      std::chrono::seconds max_time,
      message_priority priority) -> bool {
        return post_blob(
          msg_id, broadcast_endpoint_id(), blob, max_time, priority);
    }

    auto broadcast_blob(
      message_id msg_id,
      memory::const_block blob,
      std::chrono::seconds max_time) -> bool {
        return broadcast_blob(msg_id, blob, max_time, message_priority::normal);
    }

    auto post_certificate(identifier_t target_id) -> bool;
    auto broadcast_certificate() -> bool;

    auto send(message_id msg_id, message_view message) -> bool {
        if(has_id()) {
            return _do_send(msg_id, message);
        } else {
            post(msg_id, message);
        }
        return false;
    }

    auto send(message_id msg_id) -> bool {
        return send(msg_id, {});
    }

    auto say_not_a_router() -> bool;
    auto say_bye() -> bool;

    void post_meta_message(message_id meta_msg_id, message_id msg_id);
    void post_meta_message_to(
      identifier_t target_id,
      message_id meta_msg_id,
      message_id msg_id);

    void say_subscribes_to(message_id);
    void say_subscribes_to(identifier_t target_id, message_id);
    void say_unsubscribes_from(message_id);
    void query_subscribers_of(message_id);

    void clear_block_list();
    void block_message_type(message_id);

    void clear_allow_list();
    void allow_message_type(message_id);

    void query_certificate_of(identifier_t endpoint_id);

    auto respond_to(
      const message_info& info,
      message_id msg_id,
      message_view message) -> bool {
        message.setup_response(info);
        return send(msg_id, message);
    }

    auto respond_to(const message_info& info, message_id msg_id) -> bool {
        return respond_to(info, msg_id, {});
    }

    using method_handler = callable_ref<bool(stored_message&)>;

    auto process_one(message_id msg_id, method_handler handler) -> bool;

    template <typename Class, bool (Class::*MemFnPtr)(stored_message&)>
    auto process_one(
      message_id msg_id,
      member_function_constant<bool (Class::*)(stored_message&), MemFnPtr>
        method,
      Class* instance) -> bool {
        return process_one(msg_id, {instance, method});
    }

    auto process_all(message_id msg_id, method_handler handler) -> span_size_t;

    using generic_handler = callable_ref<bool(message_id, stored_message&)>;

    auto process_everything(generic_handler handler) -> span_size_t;
};
//------------------------------------------------------------------------------
class friend_of_endpoint {
protected:
    static auto
    _make_endpoint(logger log, endpoint::fetch_handler store_message) noexcept {
        return endpoint{std::move(log), store_message};
    }

    static auto _make_endpoint(
      logger log,
      endpoint::blob_filter_function allow_blob,
      endpoint::fetch_handler store_message) noexcept {
        return endpoint{std::move(log), allow_blob, store_message};
    }

    static auto _move_endpoint(
      endpoint&& bus,
      endpoint::fetch_handler store_message) noexcept {
        return endpoint{std::move(bus), {}, store_message};
    }

    static auto _move_endpoint(
      endpoint&& bus,
      endpoint::blob_filter_function allow_blob,
      endpoint::fetch_handler store_message) noexcept {
        return endpoint{std::move(bus), allow_blob, store_message};
    }

    inline auto _accept_message(
      endpoint& ep,
      message_id msg_id,
      const message_view& message) -> bool {
        return ep._accept_message(msg_id, message);
    }
};
//------------------------------------------------------------------------------
} // namespace eagine::msgbus

#if !EAGINE_LINK_LIBRARY || defined(EAGINE_IMPLEMENTING_LIBRARY)
#include <eagine/message_bus/endpoint.inl>
#endif

#endif // EAGINE_MESSAGE_BUS_ENDPOINT_HPP
