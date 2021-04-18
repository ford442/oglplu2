import QtQuick 2.2
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import "qrc:///views"

NodeListItem {
	id: instListItem

	ColumnLayout {
		Label {
			text: "Instance: %1".arg(identifier ? identifier : "?")
		}
	}

}
