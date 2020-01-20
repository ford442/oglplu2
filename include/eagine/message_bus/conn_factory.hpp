/**
 *  @file eagine/message_bus/conn_factory.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_MESSAGE_BUS_CONN_FACTORY_HPP
#define EAGINE_MESSAGE_BUS_CONN_FACTORY_HPP

#include "acceptor.hpp"
#include "connection.hpp"
#include <memory>

namespace eagine {
namespace msgbus {
//------------------------------------------------------------------------------
struct connection_factory {
    connection_factory() noexcept = default;
    connection_factory(connection_factory&&) noexcept = default;
    connection_factory(const connection_factory&) = delete;
    connection_factory& operator=(connection_factory&&) = delete;
    connection_factory& operator=(const connection_factory&) = delete;
    virtual ~connection_factory() noexcept = default;

    virtual identifier type_id() = 0;

    virtual std::unique_ptr<acceptor> make_acceptor(string_view address) = 0;
    virtual std::unique_ptr<connection> make_connector(string_view address) = 0;

    auto make_acceptor() {
        return make_acceptor(string_view{});
    }

    auto make_connector() {
        return make_connector(string_view{});
    }

    auto make_acceptor(identifier id) {
        return make_acceptor(id.name());
    }

    auto make_connector(identifier id) {
        return make_connector(id.name());
    }
};
//------------------------------------------------------------------------------
} // namespace msgbus
} // namespace eagine

#endif // EAGINE_MESSAGE_BUS_CONN_FACTORY_HPP

