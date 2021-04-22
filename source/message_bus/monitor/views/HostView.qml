import QtQuick 2.2
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import "qrc:///views"

Pane {
	id: hostView
    property variant model: null
	leftPadding: 0

	ColumnLayout {
		ItemViewHeading {
			itemKind: "Host"
			iconName: "HostItem"
			itemId: hostView.model.host.identifier
		}
		Item {
			Layout.fillHeight: true
		}
	}
}

