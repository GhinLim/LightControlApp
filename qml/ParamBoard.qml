import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item{
    property var channelSetter

    function clearTextFieldFocus(){
        for (var i = 0; i < repeater.count; i++) {
            if (repeater.itemAt(i).textFieldFocus) {
                repeater.itemAt(i).textFieldFocus  = false
                break
            }
        }
    }

    function refreshData()
    {
        channelParamBoard.refreshData()
        for (var i = 0; i < repeater.count; i++) {
            repeater.itemAt(i).refreshData()
        }
    }

    width: childrenRect.width
    height: childrenRect.height+30

    ChannelParamBoard{
        channelParameters: channelSetter
        id:channelParamBoard
        anchors.horizontalCenter: grid.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 35
    }

    Grid {
        id:grid
        columns: 2
        columnSpacing: -50
        anchors.top: channelParamBoard.bottom
        anchors.topMargin: 64

        Repeater{
            id:repeater
            model:channelSetter.pwmSetterList
            PwmSetter{
                innerModelData: modelData
                scale: 0.8
                titleText: "W-"+String(index+1)
            }
        }
    }
}


