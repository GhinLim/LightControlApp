import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    property alias model: combox.model
    property alias title: label.text
    property alias state: button.state
    property alias currentText: combox.currentText
    signal btnClicked()
    signal spread()

    width: 600
    height:80
    Label{
        id:label
        font.pixelSize: 20
        font.bold: true
        color: "black"
        font.family: "Consolas"
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        text:"PC-Online"
    }

    ComboBox{
        id:combox
        width: 150
        anchors.right: button.left
        anchors.rightMargin: 40
        model: ListModel {
                    ListElement { name: "COM 1" }
                    ListElement { name: "COM 2" }
                    ListElement { name: "COM 3" }
                }
        anchors.verticalCenter: parent.verticalCenter
        onPressedChanged: spread()
    }

    Button {
        property alias iconColor: iconRect.color
        id: button
        width: 150
        Material.background: Material.Indigo
        Material.foreground: "white"

        // 将 Rectangle 放入按钮中作为子项
        Rectangle {
            id: iconRect
            width: 10
            height: 10
            radius: width / 2
            anchors.left: button.left
            anchors.leftMargin: 15
            anchors.verticalCenter: button.verticalCenter
            color: "#ff1919"
        }

        // 按钮文本
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        states: [
            State {
                name: "connected"
                PropertyChanges { target: button; text: "Disconnect"; iconColor: "#00ff66" }
            },
            State {
                name: "disconnected"
                PropertyChanges { target: button; text: "Connect"; iconColor: "#ff1919" }
            }
        ]

        onClicked: btnClicked()
    }

}
