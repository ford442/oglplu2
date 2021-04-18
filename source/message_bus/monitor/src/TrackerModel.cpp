///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#include "TrackerModel.hpp"
#include "MonitorBackend.hpp"
#include <eagine/message_bus/conn_setup.hpp>
#include <eagine/message_bus/router_address.hpp>
//------------------------------------------------------------------------------
TrackerModel::TrackerModel(MonitorBackend& backend)
  : QObject{nullptr}
  , eagine::main_ctx_object{EAGINE_ID(Tracker), backend}
  , _backend{backend}
  , _bus{EAGINE_ID(TrckrEndpt), *this}
  , _tracker{_bus} {
    eagine::msgbus::router_address address{*this};
    eagine::msgbus::connection_setup conn_setup{*this};
    conn_setup.setup_connectors(_tracker, address);

    _tracker.node_changed.connect(EAGINE_THIS_MEM_FUNC_REF(handleNodeChanged));
}
//------------------------------------------------------------------------------
void TrackerModel::handleNodeChanged(
  eagine::msgbus::remote_node& node,
  eagine::msgbus::remote_node_changes changes) {
    using eagine::msgbus::remote_node_change;

    if(changes.has(remote_node_change::kind)) {
        emit nodeAppeared(node);
    }

    if(changes.has(remote_node_change::host_id)) {
    }

    if(changes.has(remote_node_change::host_info)) {
    }

    if(changes.has(remote_node_change::build_info)) {
    }

    if(changes.has(remote_node_change::endpoint_info)) {
    }

    if(changes.has(remote_node_change::methods_added)) {
    }

    if(changes.has(remote_node_change::methods_removed)) {
    }

    if(changes.has(remote_node_change::started_responding)) {
    }

    if(changes.has(remote_node_change::stopped_responding)) {
    }

    if(changes.has(remote_node_change::hardware_config)) {
    }

    if(changes.has(remote_node_change::sensor_values)) {
    }

    if(changes.has(remote_node_change::connection_info)) {
    }

    if(changes.has(remote_node_change::instance_id)) {
    }
}
//------------------------------------------------------------------------------
void TrackerModel::update() {
    _tracker.process_all();
    _tracker.update();
}
//------------------------------------------------------------------------------