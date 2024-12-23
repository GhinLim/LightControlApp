import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item{
    id:root
    property var channelSetter
    property bool isChannelParamEqual: true
    property bool isPwmParamEqual: true
    signal dataEqual(bool isEqual)

    function checkDataEquality(){
        if(isChannelParamEqual && isPwmParamEqual){
            dataEqual(true)
            console.log("总体：true")
        }else{
            dataEqual(false)
            console.log("总体：false")
        }
    }

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
        grid.refreshData()
    }

    function save(){
        channelParamBoard.save()
        grid.save()
    }

    width: childrenRect.width
    height: childrenRect.height+30

    ChannelParamBoard{
        channelParameters: channelSetter
        id:channelParamBoard
        anchors.horizontalCenter: grid.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 35
        onDataEuqal: {
            isChannelParamEqual = isEqual
            root.checkDataEquality()
        }
        onMultiplierChanged: {
            grid.updatePwmsByMultiplier(multiplier)
        }
    }

    Grid {
        id:grid
        function updatePwmsByMultiplier(multiplier){
            for (var i = 0; i < repeater.count; i++) {
                if(repeater.itemAt(i).enabled && repeater.itemAt(i).innerIsOpened){
                    repeater.itemAt(i).setValueByMultiplier(multiplier)
                }
            }
        }

        function refreshData(){
            for (var i = 0; i < repeater.count; i++) {
                repeater.itemAt(i).refreshData()
            }
        }

        function checkDataEquality(){
            for (var i = 0; i < repeater.count; i++) {
                if(!repeater.itemAt(i).checkDataEquality()){
                    console.log("PwmSetter:",i,"不相等")
                    isPwmParamEqual = false
                    root.checkDataEquality()
                    return
                }
            }
            isPwmParamEqual = true
            root.checkDataEquality()
        }

        function save(){
            for (var i = 0; i < repeater.count; i++) {
                repeater.itemAt(i).save()
            }
            isPwmParamEqual = true
            root.checkDataEquality()
        }

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
                onRequestCheckDataEquality: {
                    console.log("PwmSetter:",index,"requestCheckDataEquality")
                    grid.checkDataEquality()
                }
            }
        }
    }
}


