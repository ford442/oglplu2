///
/// Copyright Matus Chochlik.
/// Distributed under the GNU GENERAL PUBLIC LICENSE version 3.
/// See http://www.gnu.org/licenses/gpl-3.0.txt
///
import QtQuick 2.2
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.2
import "qrc:///views"

ApplicationWindow {
    id: root
    visible: true
    width: 1200
    height: 800
    Material.theme: backend.theme.light ? Material.Light : Material.Dark
    Material.accent: Material.Blue

    Action {
        id: saveAction
        text: qsTr("&Save")
        shortcut: StandardKey.Save
        onTriggered: {
            saveDialog.open()
        }
    }

    Action {
        id: quitAction
        text: qsTr("&Quit")
        shortcut: StandardKey.Quit
        onTriggered: {
            Qt.callLater(Qt.quit)
        }
    }

    Action {
        id: lightThemeToggleAction
        text: qsTr("&Light")
        checkable: true
        checked: backend.theme.light
        onToggled: {
            backend.theme.light = checked
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Menu {
                title: qsTr("&New tiling")
                Repeater {
                    model: [16, 32, 64, 128, 256, 512, 1024]
                    MenuItem {
                        text: qsTr("%1x%1").arg(modelData)
                        onTriggered: {
                            backend.tiling.reinitialize(modelData, modelData)
                        }
                    }
                }
            }
            MenuItem {
                action: saveAction
            }
            MenuSeparator { }
            MenuItem {
                action: quitAction
            }
        }
        Menu {
            title: qsTr("&Window")
            Menu {
                title: qsTr("&Theme")
                MenuItem {
                    action: lightThemeToggleAction
                }
            }
        }
    }

    contentData: ColumnLayout {
        anchors.fill: parent

		TilingView {
			Layout.fillWidth: true
			Layout.fillHeight: true

        	model: backend.tiling
		}
		ProgressBar {
			Layout.fillWidth: true
			Layout.preferredHeight: 25

			property real progress: backend.tiling.progress
				? backend.tiling.progress
				: 0.0

			from: 0
			to: 1
			value: progress
			indeterminate: !backend.tiling.progress

			Behavior on progress {
				NumberAnimation {
					duration: 1000
				}
			}
		}
    }

    FileDialog {
        id: saveDialog
        title: qsTr("Save tiling into file")
        folder: shortcuts.home
        selectExisting: false
        selectMultiple: false
        selectFolder: false
        onAccepted: {
            backend.tiling.saveAs(saveDialog.fileUrl)
        }

        Component.onCompleted: visible = false
    }
}
