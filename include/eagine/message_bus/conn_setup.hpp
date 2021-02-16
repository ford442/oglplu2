/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_MESSAGE_BUS_CONN_SETUP_HPP
#define EAGINE_MESSAGE_BUS_CONN_SETUP_HPP

#include "../enum_map.hpp"
#include "../main_ctx_object.hpp"
#include "conn_factory.hpp"
#include <memory>
#include <mutex>
#include <vector>

namespace eagine {
class application_config;
namespace msgbus {
//------------------------------------------------------------------------------
class connection_setup;
void connection_setup_default_init(connection_setup&);
//------------------------------------------------------------------------------
static inline auto adapt_log_entry_arg(
  identifier name,
  const std::unique_ptr<connection_factory>& value) {
    return [name, &value](logger_backend& backend) {
        if(value) {
            backend.add_identifier(
              name, EAGINE_ID(ConnFactry), value->type_id());
        } else {
            backend.add_nothing(name, EAGINE_ID(ConnFactry));
        }
    };
}
//------------------------------------------------------------------------------
class connection_setup : public main_ctx_object {
    std::mutex _mutex{};

    using _factory_list = std::vector<std::unique_ptr<connection_factory>>;

    template <connection_kind Kind>
    using _enum_map_unit = _factory_list;

    static_enum_map<
      connection_kind,
      _enum_map_unit,
      connection_kind::in_process,
      connection_kind::local_interprocess,
      connection_kind::remote_interprocess>
      _factory_map{};

    void _do_setup_acceptors(
      acceptor_user& target,
      string_view address,
      _factory_list& factories);

    void _do_setup_connectors(
      connection_user& target,
      string_view address,
      _factory_list& factories);

    auto _make_call_setup_acceptors(acceptor_user&, string_view address);
    auto _make_call_setup_connectors(connection_user&, string_view address);

public:
    connection_setup(main_ctx_parent parent) noexcept
      : main_ctx_object{EAGINE_ID(ConnSetup), parent} {
        default_init();
    }

    void setup_acceptors(acceptor_user& target, string_view address);

    void setup_acceptors(acceptor_user& target, identifier address) {
        setup_acceptors(target, address.name());
    }

    void setup_acceptors(acceptor_user& target) {
        setup_acceptors(target, string_view{});
    }

    void setup_acceptors(
      acceptor_user& target,
      connection_kinds kinds,
      string_view address);

    void setup_acceptors(
      acceptor_user& target,
      connection_kinds kinds,
      identifier address) {
        setup_acceptors(target, kinds, address.name());
    }

    void setup_acceptors(acceptor_user& target, connection_kinds kinds) {
        setup_acceptors(target, kinds, string_view{});
    }

    void setup_acceptors(
      acceptor_user& target,
      connection_kind kind,
      string_view address);

    void setup_acceptors(
      acceptor_user& target,
      connection_kind kind,
      identifier address) {
        setup_acceptors(target, kind, address.name());
    }

    void setup_acceptors(acceptor_user& target, connection_kind kind) {
        setup_acceptors(target, kind, string_view{});
    }

    void setup_connectors(connection_user& target, string_view address);

    void setup_connectors(connection_user& target, identifier address) {
        setup_connectors(target, address.name());
    }

    void setup_connectors(connection_user& target) {
        setup_connectors(target, string_view{});
    }

    void setup_connectors(
      connection_user& target,
      connection_kinds kinds,
      string_view address);

    void setup_connectors(
      connection_user& target,
      connection_kinds kinds,
      identifier address) {
        setup_connectors(target, kinds, address.name());
    }

    void setup_connectors(connection_user& target, connection_kinds kinds) {
        setup_connectors(target, kinds, string_view{});
    }

    void setup_connectors(
      connection_user& target,
      connection_kind kind,
      string_view address);

    void setup_connectors(
      connection_user& target,
      connection_kind kind,
      identifier address) {
        setup_connectors(target, kind, address.name());
    }

    void setup_connectors(connection_user& target, connection_kind kind) {
        setup_connectors(target, kind, string_view{});
    }

    void add_factory(std::unique_ptr<connection_factory> factory);

    template <typename Factory, typename... Args>
    auto make_factory(Args&&... args)
      -> std::enable_if_t<std::is_base_of_v<connection_factory, Factory>> {
        add_factory(
          std::make_unique<Factory>(*this, std::forward<Args>(args)...));
    }

    void default_init() {
        connection_setup_default_init(*this);
    }
};
//------------------------------------------------------------------------------
} // namespace msgbus
} // namespace eagine

#if !EAGINE_LINK_LIBRARY || defined(EAGINE_IMPLEMENTING_LIBRARY)
#include <eagine/message_bus/conn_setup.inl>
#endif

#endif // EAGINE_MESSAGE_BUS_CONN_SETUP_HPP
