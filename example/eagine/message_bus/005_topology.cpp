/// @example eagine/message_bus/005_topology.cpp
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#include "lib_common_pki.hpp"
#include <eagine/main.hpp>
#include <eagine/message_bus.hpp>
#include <eagine/message_bus/service.hpp>
#include <eagine/message_bus/service/shutdown.hpp>
#include <eagine/message_bus/service/topology.hpp>
#include <eagine/message_bus/service_requirements.hpp>
#include <eagine/signal_switch.hpp>
#include <iostream>
#include <set>
#include <thread>

namespace eagine {
namespace msgbus {
//------------------------------------------------------------------------------
using topology_printer_base = service_composition<
  require_services<subscriber, network_topology, shutdown_target>>;

class topology_printer
  : public main_ctx_object
  , public topology_printer_base {
    using base = topology_printer_base;

public:
    topology_printer(endpoint& bus)
      : main_ctx_object{EAGINE_ID(TopoPrint), bus}
      , base{bus} {
        router_appeared.connect(EAGINE_THIS_MEM_FUNC_REF(on_router_appeared));
        bridge_appeared.connect(EAGINE_THIS_MEM_FUNC_REF(on_bridge_appeared));
        endpoint_appeared.connect(
          EAGINE_THIS_MEM_FUNC_REF(on_endpoint_appeared));
        shutdown_requested.connect(EAGINE_THIS_MEM_FUNC_REF(on_shutdown));
    }

    void print_topology() {
        std::cout << "graph EMB {\n";

        std::cout << "	overlap=false\n";
        std::cout << "	splines=true\n";
        std::cout << "	node [style=filled]\n";
        std::cout << "	node [shape=egg;color=\"#B0D0B0\"]\n";
        for(auto id : _routers) {
            std::cout << "	n" << id << "[label=\"Router-" << id << "\"]\n";
        }
        std::cout << "\n";

        std::cout << "	node [shape=parallelogram;color=\"#80B080\"]\n";
        for(auto id : _bridges) {
            std::cout << "	n" << id << " [label=\"Bridge-" << id << "\"]\n";
        }
        std::cout << "\n";

        std::cout << "	node [shape=box;color=\"#B0E0B0\"]\n";
        std::cout << "	n" << this->bus_node().get_id()
                  << "[label=\"Self\\nEndpoint-" << this->bus_node().get_id()
                  << "\"]\n";

        for(auto id : _endpoints) {
            std::cout << "	n" << id << "[label=\"Endpoint-" << id << "\"]\n";
        }
        std::cout << "\n";

        std::cout << "	edge [style=solid,penwidth=2]\n";
        for(auto [l, r] : _connections) {
            std::cout << "	n" << l << " -- n" << r << "\n";
        }

        std::cout << "}\n";
    }

    void on_router_appeared(const router_topology_info& info) {
        log_info("found router connection ${router} - ${remote}")
          .arg(EAGINE_ID(remote), info.remote_id)
          .arg(EAGINE_ID(router), info.router_id);

        _routers.emplace(info.router_id);
        _connections.emplace(info.router_id, info.remote_id);
    }

    void on_bridge_appeared(const bridge_topology_info& info) {
        if(info.opposite_id) {
            log_info("found bridge connection ${bridge} - ${remote}")
              .arg(EAGINE_ID(remote), info.opposite_id)
              .arg(EAGINE_ID(bridge), info.bridge_id);

            _bridges.emplace(info.opposite_id);
            _connections.emplace(info.bridge_id, info.opposite_id);
        } else {
            _log.info("found bridge ${bridge}")
              .arg(EAGINE_ID(bridge), info.bridge_id);
        }

        _bridges.emplace(info.bridge_id);
    }

    void on_endpoint_appeared(const endpoint_topology_info& info) {
        log_info("found endpoint ${endpoint}")
          .arg(EAGINE_ID(endpoint), info.endpoint_id);

        _endpoints.emplace(info.endpoint_id);
    }

    void on_shutdown(
      std::chrono::milliseconds age,
      identifier_t subscriber_id,
      verification_bits verified) {
        _log.info("received ${age} old shutdown request from ${subscrbr}")
          .arg(EAGINE_ID(age), age)
          .arg(EAGINE_ID(subscrbr), subscriber_id)
          .arg(EAGINE_ID(verified), verified);
    }

private:
    std::set<identifier_t> _routers;
    std::set<identifier_t> _bridges;
    std::set<identifier_t> _endpoints;
    std::set<std::pair<identifier_t, identifier_t>> _connections;
    logger _log{};
};
//------------------------------------------------------------------------------
} // namespace msgbus

auto main(main_ctx& ctx) -> int {

    signal_switch interrupted;

    msgbus::endpoint bus{EAGINE_ID(TopologyEx), ctx};
    bus.add_ca_certificate_pem(ca_certificate_pem(ctx));
    bus.add_certificate_pem(msgbus_endpoint_certificate_pem(ctx));

    msgbus::topology_printer topo_prn{bus};

    ctx.bus().setup_connectors(topo_prn);
    timeout waited_enough{std::chrono::seconds(30)};
    resetting_timeout resend_query{std::chrono::seconds(5), nothing};

    while(!(interrupted || waited_enough)) {
        if(resend_query) {
            topo_prn.discover_topology();
        }
        topo_prn.update();
        if(!topo_prn.process_all()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    }

    topo_prn.print_topology();

    return 0;
}
//------------------------------------------------------------------------------
} // namespace eagine
