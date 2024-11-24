import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import QtQuick.Controls.Material

Item {
    property alias source: logo.source
    property int value
    property alias title: title.text
    property bool hasCtrlBtn:false
    signal paramTextChanged(string text)
    width: 260
    height: 160

    onValueChanged: param.text = String(value)

    MouseArea{
        anchors.fill: parent
        onClicked: parent.forceActiveFocus()
    }


    Rectangle{
        id:background
        anchors.fill: parent
        color: "white"
        radius: 30

        Image{
            id:logo
            width: 40
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20
        }

        RoundButton{
            visible: hasCtrlBtn
            id:minusBtn
            x:5
            width: 50
            height: 50
            text: "-"
            font.pixelSize: 30
            font.bold: true
            anchors.verticalCenter: param.verticalCenter
            Material.background: Material.Indigo
            Material.foreground: "white"
            onClicked: {
                var num = Number(param.text)
                num--
                param.text = String(num)
            }
        }

        RoundButton{
            visible: hasCtrlBtn
            id:addBtn
            width: 50
            height: 50
            x:205
            text: "+"
            font.pixelSize: 30
            font.bold: true
            anchors.verticalCenter: param.verticalCenter
            Material.background: Material.Indigo
            Material.foreground: "white"
            onClicked: {
                var num = Number(param.text)
                num++
                param.text = String(num)
            }
        }

        NeonText{
            id:param
            anchors.top: logo.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 8
            font.pixelSize: 40
            normalColor: "black"
            font.bold: true

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    textField.visible = true
                    textField.text = param.text
                    textField.forceActiveFocus()  // 给 TextField 焦点
                    param.visible = false
                }
            }
            onTextChanged: paramTextChanged(text)
        }

        TextField {
            id: textField
            font.pixelSize: 34
            anchors.centerIn: param
            width: 156
            height: 54
            visible: false
            selectByMouse: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            // 当 TextField 失去焦点时隐藏它
            onFocusChanged: {
                if (!focus) {
                    param.visible = true
                    param.text = textField.text
                    textField.visible = false
                }
            }
        }

        NeonText{
            id:title
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 17
            font.pixelSize: 20
            normalColor: "black"
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
