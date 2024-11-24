import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import QtQuick.Controls.Material

Item {
    id:root
    property var innerModelData
    property int value: innerModelData.value
    property alias titleText: titleText.text
    property alias textFieldFocus: textField.focus
    property alias pwmValue: pwmValue.text

    function refreshData()
    {
        pwmValue.text = String(value)
    }

    width: 500
    height: 100

    onValueChanged: refreshData()

    MouseArea{
        anchors.fill: parent
        onClicked: parent.forceActiveFocus()
    }

    Rectangle {
        id:background
        anchors.fill: parent
        color: "white"
        radius: 30

        NeonText {
            id:titleText
            text: "W-7"
            font.pixelSize: 25
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 20
            normalColor: "black"
        }

        Button{
            id:saveBtn
            height: 38
            width: 56
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            Material.background: Material.Indigo
            enabled: value !== Number(pwmValue.text)
            contentItem: Image {
                scale: 1.6
                anchors.centerIn: parent
                source: "../image/save.png"
            }
            onClicked:  innerModelData.value = Number(pwmValue.text)
        }

        RoundButton{
            id:minusBtn
            x:112
            width: 50
            height: 50
            text: "-"
            font.pixelSize: 30
            font.bold: true
            anchors.verticalCenter: parent.verticalCenter
            Material.background: Material.Indigo
            Material.foreground: "white"
            onClicked: {
                var num = Number(pwmValue.text)
                num--
                pwmValue.text = String(num)
            }
        }

        RoundButton{
            id:addBtn
            width: 50
            height: 50
            x:338
            text: "+"
            font.pixelSize: 30
            font.bold: true
            anchors.verticalCenter: parent.verticalCenter
            Material.background: Material.Indigo
            Material.foreground: "white"
            onClicked: {
                var num = Number(pwmValue.text)
                num++
                pwmValue.text = String(num)
            }
        }

        TextField {
            id: textField
            font.pixelSize: 34
            anchors.centerIn: parent
            width: 156
            height: 57
            visible: false
            selectByMouse: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            onTextChanged: {
                pwmValue.text = textField.text
            }

            // 当 TextField 失去焦点时隐藏它
            onFocusChanged: {
                if (!focus) {
                    pwmValue.visible = true
                    textField.visible = false
                }
            }
        }

        NeonText {
            id: pwmValue
            normalColor: "black"
            font.pixelSize: 45
            anchors.centerIn: parent

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    textField.visible = true
                    textField.text = pwmValue.text
                    textField.forceActiveFocus()  // 给 TextField 焦点
                    pwmValue.visible = false
                }
            }
        }

        Switch{
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 20
            scale: 1.2
        }
    }

    DropShadow{
        anchors.fill: background
        source: background
        radius:20
        samples: 16
        color:"lightGray"
        spread: 0.2
    }
}
