///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#include "lib_common_pki.hpp"
#include <eagine/logging/root_logger.hpp>
#include <eagine/main_ctx.hpp>
#include <eagine/math/functions.hpp>
#include <eagine/message_bus/bridge.hpp>
#include <eagine/message_bus/conn_setup.hpp>
#include <eagine/message_bus/router_address.hpp>
#include <eagine/signal_switch.hpp>

namespace eagine {
//------------------------------------------------------------------------------
auto main(main_ctx& ctx) -> int {
    signal_switch interrupted;

    auto& log = ctx.log();

    log.info("message bus bridge starting up");

    msgbus::router_address address(ctx);

    msgbus::connection_setup conn_setup(ctx);

    msgbus::bridge bridge(ctx);
    bridge.add_ca_certificate_pem(ca_certificate_pem(ctx));
    bridge.add_certificate_pem(msgbus_bridge_certificate_pem(ctx));
    conn_setup.setup_connectors(bridge, address);

    std::uintmax_t cycles_work{0};
    std::uintmax_t cycles_idle{0};
    int idle_streak{0};
    int max_idle_streak{0};

    while(EAGINE_LIKELY(!(interrupted || bridge.is_done()))) {
        if(EAGINE_LIKELY(bridge.update())) {
            ++cycles_work;
            idle_streak = 0;
        } else {
            ++cycles_idle;
            max_idle_streak = math::maximum(max_idle_streak, ++idle_streak);
            std::this_thread::sleep_for(
              std::chrono::milliseconds(math::minimum(idle_streak / 8, 8)));
        }
    }

    log.stat("message bus bridge finishing")
      .arg(EAGINE_ID(working), cycles_work)
      .arg(EAGINE_ID(idling), cycles_idle)
      .arg(
        EAGINE_ID(workRate),
        EAGINE_ID(Ratio),
        float(cycles_work) / (float(cycles_idle) + float(cycles_work)))
      .arg(
        EAGINE_ID(idleRate),
        EAGINE_ID(Ratio),
        float(cycles_idle) / (float(cycles_idle) + float(cycles_work)))
      .arg(EAGINE_ID(maxIdlStrk), max_idle_streak);

    return 0;
}
//------------------------------------------------------------------------------
} // namespace eagine

auto main(int argc, const char** argv) -> int {
    eagine::main_ctx_options options;
    options.app_id = EAGINE_ID(BridgeExe);
    return eagine::main_impl(argc, argv, options);
}