import QtQuick 2.2
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import "qrc:///views"

Control {
	id: nodeListItem
	property variant view: nodeListItemLayout.view

	width: 250
	height: 56
	leftPadding: 0
	topPadding: 0
	rightPadding: 0
	bottomPadding: 0
	clip: true

	background: Rectangle {
		id: nodeListItemBg
		color: "gray"
		opacity: 0.05
	}

	function isCurrent() {
		return nodeListItem.view.currentIndex == index
	}

	function makeCurrent() {
		nodeListItem.view.currentIndex = index
	}

	state: isCurrent() ? "Selected" : "Unselected"
	states: [
		State {
			name: "Selected"
			PropertyChanges {
				target: nodeListItemBg
				opacity: 0.5
			}
		},
		State {
			name: "Unselected"
			PropertyChanges {
				target: nodeListItemBg
				opacity: 0.05
			}
		}
	]

	transitions: [
		Transition {
			from: "Unselected"
			to: "Selected"
			PropertyAnimation {
				target: nodeListItemBg
				properties: "opacity"
				duration: 100
			}
		},
		Transition {
			from: "Selected"
			to: "Unselected"
			PropertyAnimation {
				target: nodeListItemBg
				properties: "opacity"
				duration: 300
			}
		}
	]

	MouseArea {
		anchors.fill: parent
		onClicked: makeCurrent()
	}
}
