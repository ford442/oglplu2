import QtQuick 2.2
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2

ListView {
	id: nodeListView
	Layout.preferredWidth: 200

	delegate: Loader {
		id: itemLoader
		Layout.fillWidth: true

		function itemViewUrl() {
			return "qrc:///views/%1ListItem.qml".arg(itemKind)
		}

		source: itemViewUrl()
		onLoaded: {
			itemLoader.item.view = nodeListView
		}
	}
	focus: true
	clip: true
}

