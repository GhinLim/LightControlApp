import QtQuick
import QtQuick.Controls

Item{
    property alias model: repeater.model

    function clearTextFieldFocus(){
        for (var i = 0; i < repeater.count; i++) {
            if (repeater.itemAt(i).textFieldFocus) {
                repeater.itemAt(i).textFieldFocus  = false
                break
            }
        }
    }

    width: childrenRect.width
    height: childrenRect.height

    ChannelParamBoard{
        id:channelParamBoard
        anchors.horizontalCenter: grid.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
    }

    Grid {
        id:grid
        columns: 2
        columnSpacing: -50
        anchors.top: channelParamBoard.bottom
        anchors.topMargin: 20

        Repeater{
            id:repeater
            model:12
            PwmSetter{
                scale: 0.8
                titleText: "W-"+String(index+1)
            }
        }
    }
}


