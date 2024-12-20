import QtQuick
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Rectangle{
    property var lightController
    id:background
    color: "white"
    radius: 10
    height: childrenRect.height
    width: 800

    Column{
        anchors.horizontalCenter: parent.horizontalCenter
        ComSettingItem{
            model: lightController.pcOnlineCom.portNameList
            title: qsTr("PC-Online")
            onCurrentTextChanged: lightController.pcOnlineCom.currentPortName = currentText

            onSpread:  {
                lightController.pcOnlineCom.updatePortNameList()
            }
            onBtnClicked: {
                if(state === "connected")
                    lightController.pcOnlineCom.closeSerialPort()
                else if(state === "disconnected")
                    lightController.pcOnlineCom.openSerialPort()
            }
            Component.onCompleted: {
                lightController.pcOnlineCom.currentPortName = currentText
                if(lightController.pcOnlineCom.isOpened)
                    state = "connected"
                else
                    state = "disconnected"
            }
            Connections{
                target: lightController.pcOnlineCom
                function onIsOpenedChanged(){
                    console.log("recv isOpenedChanged!")
                    if(lightController.pcOnlineCom.isOpened)
                        state = "connected"
                    else
                        state = "disconnected"
                }
            }
        }

        ComSettingItem{
            model: lightController.a200OnlineCom.portNameList
            title: qsTr("200A-Online")
            onCurrentTextChanged: lightController.a200OnlineCom.currentPortName = currentText

            onSpread: {
                lightController.a200OnlineCom.updatePortNameList()
            }
            onBtnClicked: {
                if(state === "connected")
                    lightController.a200OnlineCom.closeSerialPort()
                else if(state === "disconnected")
                    lightController.a200OnlineCom.openSerialPort()
            }
            Component.onCompleted: {
                lightController.a200OnlineCom.currentPortName = currentText
                if(lightController.a200OnlineCom.isOpened)
                    state = "connected"
                else
                    state = "disconnected"
            }
            Connections{
                target: lightController.a200OnlineCom
                function onIsOpenedChanged(){
                    console.log("recv isOpenedChanged!")
                    if(lightController.a200OnlineCom.isOpened)
                        state = "connected"
                    else
                        state = "disconnected"
                }
            }
        }

        PwmHzSetting{
        }
    }


}
