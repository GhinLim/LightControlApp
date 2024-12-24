import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    property var lightController
    property int _value: lightController.pwmHz
    on_ValueChanged: textField.text = String(lightController.pwmHz)
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

    TextField{
        id:textField
        width: 150
        height: 53
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: unitLabel.left
        anchors.rightMargin: 9
        font.pixelSize: 18

        validator: IntValidator {
            bottom: 1
            top: 24000
        }

        // 监听文本变化
        onTextChanged: {
            if(text === "0"){
                text = "1"
            }else if(Number(text)> 24000){
                text = "24000"
            }
            if(text!=="" && Number(text)!==lightController.pwmHz){
                // console.log("lightController.pwmHz:",lightController.pwmHz,"text:",Number(text))
                button.enabled = true
            }else{
                // console.log("lightController.pwmHz:",lightController.pwmHz,"text:",Number(text))
                button.enabled = false
            }
        }

        onFocusChanged: {
            if(!focus){
                if(text === ""){
                    text = String(_value)
                }
            }
        }

        Component.onCompleted: text = String(_value)
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
        onClicked: {
            lightController.pwmHz = Number(textField.text)
            enabled = false
        }
    }
}
