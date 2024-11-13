import QtQuick
import QtQuick.Controls

Item {

    Rectangle {
        id:background
        anchors.fill: parent
        color: "#595973"
    }

    Label{
        text: "Lighting Control System"
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 30
        font.bold: true
        color: "white"
    }

    OnlineParamBoard{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 120
    }

    Drawer{
        id:drawer
        edge: Qt.BottomEdge
        interactive: false
        modal: false
        width: /*controlPanel.width*/parent.width
        height: controlPanel.height - 170
        dragMargin: 100
        background: Rectangle{
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    controlPanel
                    console.log("clicked")
                    controlPanel.clearTextFieldFocus()
                }
            }
            color: "#595973"
            Rectangle{
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: 30
                radius: 20
                color:"white"
            }

            Rectangle{
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.topMargin: 15
                color:"white"
            }
        }

        ControlPanel {
            id:controlPanel
            scale: 0.8
            anchors.top: parent.top
            anchors.topMargin: -80
            // anchors.bottom: parent.bottom
            // anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Component.onCompleted: {
            open()
        }
    }
}
