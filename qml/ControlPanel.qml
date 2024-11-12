import QtQuick
import QtQuick.Controls
Item{
    id:root
    width: childrenRect.width
    height: childrenRect.height
    property var model: 12

    DoubleTabBar{
        id:doubleTabBar
        model:root.model
        onSelectedIndexChanged: swipeView.currentIndex = selectedIndex
        anchors.horizontalCenter: swipeView.horizontalCenter
    }

    SwipeView {
        id:swipeView
        clip: true
        width: 950
        height: 780+30
        anchors.top: doubleTabBar.bottom

        onFocusChanged: {
            if(!focus){
                currentItem.clearTextFieldFocus()
            }
        }

        Repeater{
            model:root.model
            PwmBoard{
                model:12 /*modelData.xxmodel*/
            }
        }

        onCurrentIndexChanged: doubleTabBar.selectedIndex = currentIndex
    }
}


