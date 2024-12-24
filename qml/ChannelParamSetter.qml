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
    signal multiplierChanged(real multiplier)
    width: 260
    height: 160

    onValueChanged: param.text = String(value)

    function refreshData(){
        param.text = value
    }

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

        RoundButton {
            visible: hasCtrlBtn
            id: minusBtn
            x: 5
            width: 50
            height: 50
            text: "-"
            font.pixelSize: 30
            font.bold: true
            anchors.verticalCenter: param.verticalCenter
            Material.background: Material.Indigo
            Material.foreground: "white"

            // 定义一个 Timer 用来持续减少数字
            Timer {
                id: minusTimer
                interval: 100 // 每100毫秒触发一次
                running: false // 初始时 Timer 不运行
                repeat: true // 设置为重复模式
                onTriggered: {
                    var num = textField.visible?Number(textField.text):Number(param.text)
                    if (num > 0) {
                        num--
                        if(textField.visible){
                            textField.text = num
                        }else{
                            var multiplier = num / value
                            if (multiplier !== Infinity && !isNaN(multiplier)){
                                multiplierChanged(multiplier)
                            }
                            param.text = String(num)
                        }
                    }
                }
            }

            MouseArea {
                id: minusMouseArea
                anchors.fill: parent
                onPressed: {
                    minusTimer.start() // 按下时启动计时器
                }
                onReleased: {
                    minusTimer.stop() // 松开时停止计时器
                }
                onCanceled: {
                    minusTimer.stop() // 如果按下的鼠标被取消，停止计时器
                }
                onClicked: {
                    var num = textField.visible?Number(textField.text):Number(param.text)
                    if (num > 0) {
                        num--
                        if(textField.visible){
                            textField.text = num
                        }else{
                            var multiplier = num / value
                            if (multiplier !== Infinity && !isNaN(multiplier)){
                                multiplierChanged(multiplier)
                            }
                            param.text = String(num)
                        }
                    }
                }
            }
        }

        RoundButton {
            visible: hasCtrlBtn
            id: addBtn
            width: 50
            height: 50
            x: 205
            text: "+"
            font.pixelSize: 30
            font.bold: true
            anchors.verticalCenter: param.verticalCenter
            Material.background: Material.Indigo
            Material.foreground: "white"

            // 定义一个 Timer 用来持续增加数字
            Timer {
                id: addTimer
                interval: 100 // 每100毫秒触发一次
                running: false // 初始时 Timer 不运行
                repeat: true // 设置为重复模式
                onTriggered: {
                    var num = textField.visible?Number(textField.text):Number(param.text)
                    if (num < 9999) {
                        num++
                        if(textField.visible){
                            textField.text = num
                        }else{
                            var multiplier = num / value
                            if (multiplier !== Infinity && !isNaN(multiplier)){
                                multiplierChanged(multiplier)
                            }
                            param.text = String(num)
                        }
                    }
                }
            }

            MouseArea {
                id: addMouseArea
                anchors.fill: parent
                onPressed: {
                    addTimer.start() // 按下时启动计时器
                }
                onReleased: {
                    addTimer.stop() // 松开时停止计时器
                }
                onCanceled: {
                    addTimer.stop() // 如果按下的鼠标被取消，停止计时器
                }
                onClicked: {
                    var num = textField.visible?Number(textField.text):Number(param.text)
                    if (num < 9999) {
                        num++
                        if(textField.visible){
                            textField.text = num
                        }else{
                            var multiplier = num / value
                            if (multiplier !== Infinity && !isNaN(multiplier)){
                                multiplierChanged(multiplier)
                            }
                            param.text = String(num)
                        }
                    }
                }
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
                    textField.visible = false
                }
            }

            validator: IntValidator {
                bottom: 0
                top: 9999
            }

            onTextChanged: {
                // 移除前导零
                if (text.startsWith("00")) {
                    text = "0";
                }
                if (text.startsWith("0") && text.length > 1) {
                    text = text.replace(/^0+/, "");
                }
                if(text!==""){
                    param.text = textField.text
                }

                var newValue = Number(text)
                var multiplier = newValue / value
                if (multiplier !== Infinity && !isNaN(multiplier)){
                    multiplierChanged(multiplier)
                }
                // console.log("value=",value,"newValue=",newValue,"multiplier:",multiplier)
            }

            onVisibleChanged: {
                if(!visible){
                    var newValue = Number(param.text)
                    var multiplier = newValue / value
                    if (multiplier !== Infinity && !isNaN(multiplier)){
                        multiplierChanged(multiplier)
                    }
                    // console.log("value=",value,"newValue=",newValue,"multiplier:",multiplier)
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
