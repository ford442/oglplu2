import QtQuick 2.2
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import "qrc:///views"

Pane {
	id: hostView
    property variant model: null
	Layout.fillWidth: true
	leftPadding: 0

	ColumnLayout {
		anchors.fill: parent
		ItemViewHeading {
			itemKind: "Host"
			iconName: "HostItem"
			itemModel: hostView.model.host
		}
		Item {
			Layout.fillHeight: true
		}
		GridLayout {
			Layout.fillWidth: true
			columns: 6
			rowSpacing: 20
			columnSpacing: 20

			Label {
				text: qsTr("Load")
			}
			ProgressBar {
				Layout.columnSpan: 5
				Layout.fillWidth: true
				from: 0.0
				to: 1.0
				value: hostView.model.host.shortLoad
					? hostView.model.host.shortLoad
					: 0.0
				indeterminate: !hostView.model.host.shortLoad
			}

			Label {
				text: qsTr("CPU threads")
			}
			Label {
				Layout.fillWidth: true
				text: hostView.model.host.cpuThreads
					? hostView.model.host.cpuThreads
					: "-"
			}

			Label {
				text: qsTr("Short average load")
			}
			Label {
				Layout.fillWidth: true
				text: hostView.model.host.shortLoad
					? "%1 %".arg(hostView.model.host.shortLoad*100.0)
					: "-"
			}

			Label {
				text: qsTr("Long average load")
			}
			Label {
				Layout.fillWidth: true
				text: hostView.model.host.longLoad
					? "%1 %".arg(hostView.model.host.longLoad*100.0)
					: "-"
			}

			Label {
				text: qsTr("Total RAM size")
			}
			Label {
				text: hostView.model.host.ramTotal
					? "%1 MB".arg(hostView.model.host.ramTotal/1000000)
					: "-"
			}

			Label {
				text: qsTr("Free RAM size")
			}
			Label {
				text: hostView.model.host.ramFree
					? "%1 MB".arg(hostView.model.host.ramFree/1000000)
					: "-"
			}

			Label {
				text: qsTr("RAM usage")
			}
			Label {
				text: hostView.model.host.ramUsage
					? "%1 %".arg(hostView.model.host.ramUsage*100.0)
					: "-"
			}

			Label {
				text: qsTr("Total swap size")
			}
			Label {
				text: hostView.model.host.swapTotal
					? "%1 MB".arg(hostView.model.host.swapTotal/1000000)
					: "-"
			}

			Label {
				text: qsTr("Free swap size")
			}
			Label {
				text: hostView.model.host.swapFree
					? "%1 MB".arg(hostView.model.host.swapFree/1000000)
					: "-"
			}

			Label {
				text: qsTr("Swap usage")
			}
			Label {
				text: hostView.model.host.swapUsage
					? "%1 %".arg(hostView.model.host.swapUsage*100.0)
					: "-"
			}
		}
	}
}

