import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import LightController 1.0

Item{
    id:root
    width: childrenRect.width
    height: childrenRect.height
    property var channelSetterList:LightController.channelSetterList
    signal channelFound(int channelIndex)
    function clearTextFieldFocus(){
        swipeView.currentItem.clearTextFieldFocus()
    }

    function refreshData(){
        for (var i = 0; i < repeater.count; i++) {
            repeater.itemAt(i).refreshData()
        }
    }

    onFocusChanged: {
        if(!focus){
            console.log("controlPanel.focus = false")
        }
    }

    DoubleTabBar{
        id:doubleTabBar
        model:channelSetterList
        onSelectedIndexChanged: swipeView.currentIndex = selectedIndex
        anchors.horizontalCenter: swipeView.horizontalCenter
        anchors.horizontalCenterOffset: 20
    }

    SwipeView {
        id: swipeView
        clip: true
        width: 950
        height: 780 + 80
        anchors.top: doubleTabBar.bottom
        anchors.topMargin: 30

        onFocusChanged: {
            if (!focus) {
                currentItem.clearTextFieldFocus()
            }
        }

        Repeater {
            id: repeater
            model: channelSetterList
            ParamBoard {
                channelSetter: modelData
                onDataEqual: {
                    // 当前 ParamBoard 的 dataEqual 信号
                    // if (currentItem === parent) {  // 确保只有当前显示的 ParamBoard 会改变 saveBtn 的状态
                        saveBtn.enabled = !isEqual
                    // }
                }
                onInquireOccupation: {
                    for (var i = 0; i < repeater.count; i++) {
                        if(i===index){
                            continue
                        }

                        if(repeater.itemAt(i).isPwmSetterOpened(pwmSetterIndex)!== -1){
                            doubleTabBar.glitterChannelText(i)
                            break
                        }
                    }
                }
            }
        }

        onCurrentIndexChanged: {
            currentItem.refreshData()
            doubleTabBar.selectedIndex = currentIndex
            currentItem.checkDataEquality()
        }
    }


    Label{
        id:title1
        text:"Channel Parameters"
        anchors.left: swipeView.left
        anchors.leftMargin: 46
        anchors.top:swipeView.top
        anchors.topMargin: -11
        font.pixelSize: 25
        font.family: "Consolas"
        font.bold: true
        color: "black"
    }

    Button {
        id: saveBtn
        anchors.right: swipeView.right
        anchors.rightMargin: 50
        anchors.verticalCenter: title1.verticalCenter
        width: 56
        height: 38
        Material.background: Material.Indigo
        // layer.enabled: true
        // layer.smooth: true
        contentItem: Image {
            scale: 1.6
            anchors.centerIn: parent
            source: "../image/save.png"
            fillMode: Image.PreserveAspectFit
            smooth: true // 启用抗锯齿
        }
        onClicked: swipeView.currentItem.save()
    }

    Label{
        id:title2
        text:"PWMs "
        anchors.left: swipeView.left
        anchors.leftMargin: 46
        anchors.top:swipeView.top
        anchors.topMargin: 223
        font.pixelSize: 25
        color: "black"
        font.family: "Consolas"
        font.bold: true
    }

    Timer{
        id:timer
        interval: 400
        repeat: true
        onTriggered: {
            doubleTabBar.selectedIndex++
            if(doubleTabBar.selectedIndex >= 11){
                doubleTabBar.selectedIndex = 0
                timer.stop()
            }
        }
    }

    Component.onCompleted: {
        doubleTabBar.selectedIndex = 0
        timer.start()
    }
}


