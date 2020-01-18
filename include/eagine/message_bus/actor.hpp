/**
 *  @file eagine/message_bus/actor.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_MESSAGE_BUS_ACTOR_HPP
#define EAGINE_MESSAGE_BUS_ACTOR_HPP

#include "subscriber.hpp"

namespace eagine {
namespace msgbus {
//------------------------------------------------------------------------------
template <std::size_t N>
class actor {
protected:
    template <
      typename Class,
      typename... MsgMaps,
      typename = std::enable_if_t<sizeof...(MsgMaps) == N>>
    actor(Class* instance, MsgMaps... msg_maps)
      : _endpoint{}
      , _subscriber{_endpoint, instance, msg_maps...} {
        _endpoint.say_not_a_router();
    }

public:
    endpoint& bus() noexcept {
        return _endpoint;
    }

    bool add_connection(std::unique_ptr<connection> conn) {
        return _endpoint.add_connection(std::move(conn));
    }

    void process_one() {
        _endpoint.update();
        _subscriber.process_one();
    }

    void process_all() {
        _endpoint.update();
        _subscriber.process_all();
    }

private:
    endpoint _endpoint;
    subscriber<N> _subscriber;
};
//------------------------------------------------------------------------------
} // namespace msgbus
} // namespace eagine

#endif // EAGINE_MESSAGE_BUS_ACTOR_HPP

