/**
 *  @example eagine/message_bus/007_ping.cpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#include <eagine/main.hpp>
#include <eagine/message_bus/conn_setup.hpp>
#include <eagine/message_bus/router_address.hpp>
#include <eagine/message_bus/service.hpp>
#include <eagine/message_bus/service/discovery.hpp>
#include <eagine/message_bus/service/ping_pong.hpp>
#include <eagine/message_bus/service/shutdown.hpp>
#include <eagine/timeout.hpp>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <map>
#include <thread>

namespace eagine {
namespace msgbus {
//------------------------------------------------------------------------------
struct ping_stats {
    std::chrono::microseconds min_time{std::chrono::microseconds::max()};
    std::chrono::microseconds max_time{std::chrono::microseconds::zero()};
    std::intmax_t responded{0};
    std::intmax_t timeouted{0};
};
//------------------------------------------------------------------------------
using ping_base =
  service_composition<pinger<subscriber_discovery<shutdown_invoker<>>>>;

class ping_example : public ping_base {
    using base = ping_base;

public:
    ping_example(endpoint& bus, valid_if_positive<std::intmax_t> max)
      : base{bus}
      , _log{EAGINE_ID(PingExampl), bus.log()}
      , _max{extract_or(max, 100000)} {
    }

    void on_subscribed(identifier_t id, const message_id_tuple& sub_msg) final {
        if(EAGINE_MSG_ID(eagiPing, ping).matches(sub_msg)) {
            _log.info("pingable ${id} appeared").arg(EAGINE_ID(id), id);
            _targets.try_emplace(id, ping_stats{});
        }
    }

    void on_unsubscribed(
      identifier_t id, const message_id_tuple& sub_msg) final {
        if(EAGINE_MSG_ID(eagiPing, ping).matches(sub_msg)) {
            _log.info("pingable ${id} disappeared").arg(EAGINE_ID(id), id);
        }
    }

    void on_ping_response(
      identifier_t pinger_id,
      message_sequence_t,
      std::chrono::microseconds age,
      verification_bits) final {
        auto& stats = _targets[pinger_id];
        stats.responded++;
        stats.min_time = std::min(stats.min_time, age);
        stats.max_time = std::max(stats.max_time, age);
        if(EAGINE_UNLIKELY((++_rcvd % _mod) == 0)) {
            _log.info("received ${rcvd} pongs").arg(EAGINE_ID(rcvd), _rcvd);
        }
    }

    void on_ping_timeout(
      identifier_t pinger_id,
      message_sequence_t,
      std::chrono::microseconds) final {
        auto& stats = _targets[pinger_id];
        stats.timeouted++;
        if(EAGINE_UNLIKELY((++_tout % _mod) == 0)) {
            _log.info("${tout} pongs timeouted").arg(EAGINE_ID(tout), _tout);
        }
    }

    bool is_done() const noexcept {
        return !(((_rcvd + _tout + _mod) < _max) || this->has_pending_pings());
    }

    bool update() {
        some_true something_done{};
        something_done(base::update());
        for(auto& entry : _targets) {
            const auto pingable_id = std::get<0>(entry);
            if(_rcvd < _max) {
                if(_sent < (_rcvd + _tout + _mod)) {
                    this->ping(pingable_id, std::chrono::seconds(5));
                    if(EAGINE_UNLIKELY((++_sent % _mod) == 0)) {
                        _log.info("sent ${sent} pings")
                          .arg(EAGINE_ID(sent), _sent);
                    }
                    something_done();
                }
            } else {
                break;
            }
        }
        something_done(base::process_all() > 0);
        return something_done;
    }

    void shutdown() {
        for(auto& entry : _targets) {
            this->shutdown_one(std::get<0>(entry));
        }
        base::update();
    }

private:
    logger _log{};
    std::map<identifier_t, ping_stats> _targets{};
    std::intmax_t _mod{10000};
    std::intmax_t _max{100000};
    std::intmax_t _sent{0};
    std::intmax_t _rcvd{0};
    std::intmax_t _tout{0};
};
//------------------------------------------------------------------------------
} // namespace msgbus

int main(main_ctx& ctx) {

    msgbus::router_address address{ctx.log(), ctx.args()};
    msgbus::connection_setup conn_setup(ctx.log(), ctx.args());

    msgbus::endpoint bus{logger{EAGINE_ID(PingExampl), ctx.log()}};

    valid_if_positive<std::intmax_t> ping_count{};
    if(auto arg{ctx.args().find("--ping-count")}) {
        arg.next().parse(ping_count, ctx.log().error_stream());
    }

    msgbus::ping_example the_pinger{bus, ping_count};
    conn_setup.setup_connectors(the_pinger, address);

    while(!the_pinger.is_done()) {
        the_pinger.process_all();
        if(!the_pinger.update()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    the_pinger.shutdown();

    return 0;
}
//------------------------------------------------------------------------------
} // namespace eagine

