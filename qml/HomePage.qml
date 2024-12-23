import QtQuick
import QtQuick.Controls

Item {
    property var lightController
    signal pushSettingPage()

    function setDrawerOpen(isOpened){
        if(isOpened)
            drawer.open()
        else
            drawer.close()
    }

    function refreshData(){
        controlPanel.refreshData()
    }

    id:root
    Rectangle {
        id:background
        anchors.fill: parent
        color: "#595973"
        MouseArea{
            anchors.fill: parent
            onClicked: background.forceActiveFocus()
        }
    }

    Button {
        id: settingLogo
        width: 20
        height: 20
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 30
        anchors.topMargin: 30

        background: Item {
            id: imageContainer
            width: parent.width
            height: parent.height
            scale: 1.0
            opacity: 1.0

            Image {
                anchors.fill: parent
                source: "../image/setting.png"
                layer.enabled: true
                layer.smooth: true
                layer.samplerName: "linear"
            }

            states: [
                State {
                    name: "hovered"
                    when: settingLogo.hovered && !settingLogo.pressed
                    PropertyChanges { target: imageContainer; opacity: 0.7; scale: 1.2 }
                },
                State {
                    name: "pressed"
                    when: settingLogo.pressed
                    PropertyChanges { target: imageContainer; opacity: 0.5; scale: 0.9 }
                },
                State {
                    name: "normal"
                    when: !settingLogo.hovered && !settingLogo.pressed
                    PropertyChanges { target: imageContainer; opacity: 1.0; scale: 1.0 }
                }
            ]

            transitions: [
                Transition {
                    NumberAnimation { properties: "opacity, scale"; duration: 200 }
                }
            ]
        }

        onClicked:{
            pushSettingPage()
        }
    }

    Label{
        text: "Lighting Controller"
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 30
        font.bold: true
        color: "white"
    }

    Label{
        text: "CL-200A Online Parameters"
        anchors.bottom: onlineParamBoard.top
        anchors.bottomMargin: -5
        anchors.left: onlineParamBoard.left
        anchors.leftMargin: -20
        font.pixelSize: 20
        font.bold: true
        color: "white"
        font.family: "Consolas"
    }

    OnlineParamBoard {
        onlineParam: lightController.onlineParam
        id: onlineParamBoard
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false

        Connections {
            target: drawer
            function onOpenedChanged() {
                if (drawer.opened) {
                    // console.log("drawer is opened!");
                    // 清除所有 anchors 设置，避免冲突
                    onlineParamBoard.anchors.centerIn = undefined;

                    // 设置顶部位置
                    onlineParamBoard.anchors.top = root.top;
                    onlineParamBoard.anchors.topMargin = 100;
                } else {
                    // console.log("drawer is closed!");
                    // 清除所有 anchors 设置
                    onlineParamBoard.anchors.top = undefined;
                    onlineParamBoard.anchors.topMargin = undefined;

                    // 恢复居中
                    onlineParamBoard.anchors.centerIn = root;
                }
            }
        }
    }

    Image{
        id:up1
        source: "../image/up1.png"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Image{
        id:up2
        source: "../image/up2.png"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Image{
        id:up3
        source: "../image/up3.png"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    MouseArea{
        anchors.fill: up1
        onClicked: drawer.open()
    }

    Timer{
        property int count: 0
        running: true
        interval: 250
        repeat: true
        onTriggered: {
            if(count % 4 == 0){
                up1.visible = false
                up2.visible = false
                up3.visible = false
                count = 0
            }else if(count % 4 ==1){
                up3.visible = true
            }else if(count % 4 ==2){
                up2.visible = true
            }else if(count % 4 ==3){
                up1.visible = true
            }
            count++;
        }
    }

    Drawer{
        id:drawer
        edge: Qt.BottomEdge
        interactive: false
        modal: false
        width: parent.width
        height: controlPanel.height - 140
        dragMargin: height
        background: Rectangle{
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

        MouseArea {
            anchors.fill: parent
            onClicked: {
                controlPanel.clearTextFieldFocus()
            }
        }

        ControlPanel {
            // channelSetterList: lightController.channelSetterList
            id:controlPanel
            scale: 0.8
            anchors.top: parent.top
            anchors.topMargin: -90
            anchors.horizontalCenter: parent.horizontalCenter
        }

        onOpened: {
            onlineParamBoard.visible = true
        }

        Component.onCompleted: {
            open()
        }
    }
}
