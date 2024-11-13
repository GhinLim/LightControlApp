import QtQuick
import QtQuick.Controls
Item{
    id:root
    width: childrenRect.width
    height: childrenRect.height
    property var model: 12

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
        model:root.model
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
            model:root.model
            ParamBoard{
                model:12 /*modelData.xxmodel*/
            }
        }

        onCurrentIndexChanged: doubleTabBar.selectedIndex = currentIndex
    }

    Label{
        id:title1
        text:"Channel parameter setting"
        anchors.left: swipeView.left
        anchors.leftMargin: 46
        anchors.top:swipeView.top
        anchors.topMargin: -11
        font.pixelSize: 25
        font.family: "Consolas"
        font.bold: true
        color: "black"
    }

    Label{
        id:title2
        text:"PWM setting"
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


