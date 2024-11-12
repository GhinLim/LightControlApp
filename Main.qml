import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material
import "qml"

Window {
    id: root
    visible: true
    width: drawer.width
    height: 1080
    Material.accent: Material.Indigo

    // 将 Item 改为 FocusScope
    // Item {
    //     id: item
    //     width: childrenRect.width
    //     height: childrenRect.height
    //     anchors.horizontalCenter: parent.horizontalCenter

        Drawer{
            id:drawer
            edge: Qt.BottomEdge
            interactive: true
            width: controlPanel.width
            height: controlPanel.height - 100

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    drawer.forceActiveFocus()
                    console.log("clicked")
                }
            }

            ControlPanel {
                id:controlPanel
                scale: 0.8
                anchors.top: parent.top
                anchors.topMargin: -50
                // anchors.bottom: parent.bottom
                // anchors.bottom: parent.bottom
                // anchors.horizontalCenter: parent.horizontalCenter
            }
        }


    // }
}

