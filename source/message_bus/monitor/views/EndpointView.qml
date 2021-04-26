import QtQuick 2.2
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import "qrc:///views"

Pane {
	id: endpointView
    property variant model: null
	leftPadding: 0

	ColumnLayout {
		anchors.fill: parent
		ItemViewHeading {
			itemKind: "Endpoint"
			iconName: "EndpointNode"
			itemModel: endpointView.model.node
		}
		ParentInstanceView {
			Layout.fillWidth: true
			model: endpointView.model
		}
		Item {
			Layout.fillHeight: true
		}
	}
}
