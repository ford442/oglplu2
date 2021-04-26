import QtQuick 2.2
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import "qrc:///views"

Pane {
	id: bridgeView
    property variant model: null
	leftPadding: 0

	ColumnLayout {
		anchors.fill: parent
		ItemViewHeading {
			itemKind: "Bridge"
			iconName: "BridgeNode"
			itemModel: bridgeView.model.node
		}
		ParentInstanceView {
			Layout.fillWidth: true
			model: bridgeView.model
		}
		Item {
			Layout.fillHeight: true
		}
	}
}
