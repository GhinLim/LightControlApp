import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import LightController 1.0

Item{
    id:root
    width: childrenRect.width
    height: childrenRect.height
    property var channelSetterList:LightController.channelSetterList

    function clearTextFieldFocus(){
        swipeView.currentItem.clearTextFieldFocus()
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
        id:swipeView
        clip: true
        width: 950
        height: 780+80
        anchors.top: doubleTabBar.bottom
        anchors.topMargin: 30

        onFocusChanged: {
            if(!focus){
                currentItem.clearTextFieldFocus()
            }
        }

        Repeater{
            model:channelSetterList
            ParamBoard{
                channelSetter: modelData
            }
        }

        onCurrentIndexChanged: {
            currentItem.refreshData()
            doubleTabBar.selectedIndex = currentIndex
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
        layer.enabled: true
        layer.smooth: true
        contentItem: Image {
            scale: 1.6
            anchors.centerIn: parent
            source: "../image/save.png"
            fillMode: Image.PreserveAspectFit
            smooth: true // 启用抗锯齿
        }
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
}


