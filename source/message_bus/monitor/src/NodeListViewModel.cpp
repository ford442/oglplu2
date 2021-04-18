///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#include "NodeListViewModel.hpp"
#include "MonitorBackend.hpp"
#include "TrackerModel.hpp"
//------------------------------------------------------------------------------
NodeListViewModel::NodeListViewModel(MonitorBackend& backend)
  : QAbstractItemModel{nullptr}
  , eagine::main_ctx_object{EAGINE_ID(NodeListVM), backend}
  , _backend{backend} {
    connect(
      &_backend,
      &MonitorBackend::trackerModelChanged,
      this,
      &NodeListViewModel::onTrackerModelChanged);
}
//------------------------------------------------------------------------------
void NodeListViewModel::onTrackerModelChanged() {
    if(auto trackerModel{_backend.trackerModel()}) {
        connect(
          trackerModel,
          &TrackerModel::nodeAppeared,
          this,
          &NodeListViewModel::onNodeAppeared);
    }
}
//------------------------------------------------------------------------------
void NodeListViewModel::onNodeAppeared(eagine::msgbus::remote_node& node) {
    if(node) {
        auto [pos, added] = _nodes.try_emplace(extract(node.id()));
        if(added) {
            pos->second = node;
            emit modelReset({});
        }
    }
}
//------------------------------------------------------------------------------
auto NodeListViewModel::roleNames() const -> QHash<int, QByteArray> {
    QHash<int, QByteArray> result;
    result.insert(NodeListViewModel::identifierRole, "identifier");
    result.insert(NodeListViewModel::itemKindRole, "itemKind");
    result.insert(NodeListViewModel::displayNameRole, "displayName");
    result.insert(NodeListViewModel::descriptionRole, "description");
    return result;
}
//------------------------------------------------------------------------------
auto NodeListViewModel::index(int row, int, const QModelIndex&) const
  -> QModelIndex {
    return QAbstractItemModel::createIndex(row, 0);
}
//------------------------------------------------------------------------------
auto NodeListViewModel::parent(const QModelIndex&) const -> QModelIndex {
    return {};
}
//------------------------------------------------------------------------------
auto NodeListViewModel::columnCount(const QModelIndex&) const -> int {
    return 1;
}
//------------------------------------------------------------------------------
auto NodeListViewModel::rowCount(const QModelIndex&) const -> int {
    return eagine::limit_cast<int>(_nodes.size());
}
//------------------------------------------------------------------------------
auto NodeListViewModel::data(const QModelIndex& index, int role) const
  -> QVariant {
    const auto r = index.row();
    if((r >= 0) && (r < eagine::limit_cast<int>(_nodes.size()))) {
        auto pos = _nodes.begin() + r;
        if(role == NodeListViewModel::identifierRole) {
            return {QString::number(pos->first)};
        } else if(role == NodeListViewModel::itemKindRole) {
            switch(pos->second.kind()) {
                case eagine::msgbus::node_kind::router:
                    return {"Router"};
                case eagine::msgbus::node_kind::bridge:
                    return {"Bridge"};
                case eagine::msgbus::node_kind::endpoint:
                    return {"Endpoint"};
                case eagine::msgbus::node_kind::unknown:
                    break;
            }
            return {"Node"};
        } else if(role == NodeListViewModel::displayNameRole) {
            if(auto optStr{pos->second.display_name()}) {
                return {c_str(extract(optStr))};
            }
        } else if(role == NodeListViewModel::descriptionRole) {
            if(auto optStr{pos->second.description()}) {
                return {c_str(extract(optStr))};
            }
        }
    }
    return {};
}
//------------------------------------------------------------------------------