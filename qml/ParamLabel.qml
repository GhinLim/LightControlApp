import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import QtQuick.Controls.Material

Item {
    property int value
    property alias title: title.text
    property alias unit: unit.text
    signal paramTextChanged(string text)
    width: 140
    height: 80

    onValueChanged: param.text = String(value)

    MouseArea{
        anchors.fill: parent
        onClicked: parent.forceActiveFocus()
    }

    NeonText{
        id:unit
        normalColor: "white"
        anchors.left: param.right
        anchors.top: param.top
        anchors.topMargin: -5
        font.pixelSize: 15
    }

    NeonText{
        id:param
        normalColor: "white"
        font.pixelSize: 35
        anchors.centerIn: parent

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
        font.pixelSize: 30
        anchors.centerIn: param
        width: 140
        height: 48
        visible: false
        selectByMouse: true
        color:"white"

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        validator: IntValidator {
            bottom: 0
            top: 999999
        }

        onTextChanged: {
            // 移除前导零
            if (text.startsWith("00")) {
                text = "0";
            }
            if (text.startsWith("0") && text.length > 1) {
                text = text.replace(/^0+/, "");
            }
        }

        // 当 TextField 失去焦点时隐藏它
        onFocusChanged: {
            if (!focus) {
                param.visible = true
                param.text = textField.text
                textField.visible = false
            }
        }

        onVisibleChanged: {
            if(visible)
                unit.anchors.left = textField.right
            else
                unit.anchors.left = param.right
        }
    }

    NeonText{
        id:title
        normalColor: "white"
        font.pixelSize: 15
        anchors.top: param.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
