import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    width: 600
    height:80

    Label{
        id:titleLabel
        font.pixelSize: 20
        font.bold: true
        color: "black"
        font.family: "Consolas"
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        text:"PWM frequency setting"
    }

    Rectangle{
        id:inputRect
        width: 150
        height: 50
        border.color: "gray"
        radius: 5
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: unitLabel.left
        anchors.rightMargin: 9
        onFocusChanged: {
            if(focus){
                border.color = "#3F51B5"
                border.width = 2
            }
            else{
                border.color = "gray"
                border.width = 1
            }
        }

        MouseArea{
            hoverEnabled: true
            anchors.fill: parent
            onEntered: {
                if(inputRect.focus === false)
                    inputRect.border.color = "black"
            }
            onExited: {
                if(inputRect.focus === false)
                    inputRect.border.color = "gray"
            }
            onPressed: inputRect.forceActiveFocus()
        }

    }

    Label{
        id:unitLabel
        font.pixelSize: 20
        font.bold: true
        color: "black"
        font.family: "Consolas"
        anchors.right:button.left
        anchors.rightMargin: 9
        anchors.verticalCenter: parent.verticalCenter
        text:"Hz"
    }

    Button {
        id: button
        width: 150
        Material.background: Material.Indigo
        Material.foreground: "white"
        Image {
            source:  "../image/save.png"
            width: 15
            height: 15
            anchors.left: parent.left
            anchors.leftMargin: 15
            anchors.verticalCenter: parent.verticalCenter
        }
        // 按钮文本
        text: qsTr("Save")


        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
    }
}
