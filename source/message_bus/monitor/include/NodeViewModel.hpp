///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_MESSAGE_BUS_MONITOR_NODE_VIEW_MODEL
#define EAGINE_MESSAGE_BUS_MONITOR_NODE_VIEW_MODEL

#include <eagine/main_ctx_object.hpp>
#include <eagine/message_bus/remote_node.hpp>
#include <QObject>
#include <QVariant>

class MonitorBackend;
class SelectedItemViewModel;
//------------------------------------------------------------------------------
class NodeViewModel
  : public QObject
  , public eagine::main_ctx_object {
    Q_OBJECT

    Q_PROPERTY(QVariant identifier READ getIdentifier NOTIFY infoChanged)
    Q_PROPERTY(QVariant displayName READ getDisplayName NOTIFY infoChanged)

public:
    NodeViewModel(MonitorBackend&, SelectedItemViewModel&);

    explicit operator bool() const noexcept {
        return bool(_node);
    }

    auto getItemKind() -> QString;
    auto getIdentifier() -> QVariant;
    auto getDisplayName() -> QVariant;

signals:
    void infoChanged();

public slots:
    void onNodeInfoChanged(eagine::identifier_t nodeId);

private:
    MonitorBackend& _backend;
    eagine::msgbus::remote_node _node;
};
//------------------------------------------------------------------------------
#endif
