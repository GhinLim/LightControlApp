import QtQuick
import QtQuick.Layouts

Row {
    id:root
    spacing: 33

    property var channelParameters
    property bool isColorTempEqual: true //用于判断前后端数据是否相等
    property bool isTotalBrightEqual: true
    property bool isBrightEqual: true
    signal multiplierChanged(real multiplier)
    signal dataEuqal(bool isEqual)

    function setTotalBright(value){
        channelParameters.totalBright = Number(value)
    }

    function checkDataEquality(){
        if(isColorTempEqual && isTotalBrightEqual && isBrightEqual){
            dataEuqal(true)
        }else{
            dataEuqal(false)
        }
    }

    function refreshData(){
        colorTempCPS.refreshData()
        totalBrightCPS.refreshData()
        brightCPS.refreshData()
    }

    function save(){
        colorTempCPS.save()
        totalBrightCPS.save()
        brightCPS.save()
        root.dataEuqal(true)
    }

    ChannelParamSetter{
        property string setterText: channelParameters.colorTemp
        function save(){
            channelParameters.colorTemp = Number(setterText)
            isColorTempEqual = true
        }

        id:colorTempCPS
        title:"Color Temperature"
        source:"../image/colorTemp.png"
        value: channelParameters.colorTemp
        onParamTextChanged: {
            if(channelParameters.colorTemp === Number(text)){
                isColorTempEqual = true
            }else{
                isColorTempEqual = false
            }
            checkDataEquality()
            setterText = text
        }
    }

    ChannelParamSetter{
        property string setterText: channelParameters.brightness
        function save(){
            channelParameters.brightness = Number(setterText)
            isBrightEqual = true
        }

        id:brightCPS
        title:"Brightness"
        source:"../image/brightness.png"
        value: channelParameters.brightness
        onParamTextChanged: {
            if(channelParameters.brightness === Number(text)){
                isBrightEqual = true
            }else{
                isBrightEqual = false
            }
            checkDataEquality()
            setterText = text
        }
    }

    ChannelParamSetter{
        property string setterText: channelParameters.totalBright
        function save(){
            channelParameters.totalBright = Number(setterText)
            isTotalBrightEqual = true
        }

        id:totalBrightCPS
        title: "Total Brightness"
        source: "../image/totalBrightness.png"
        value: channelParameters.totalBright
        hasCtrlBtn: true
        onParamTextChanged:{
            if(channelParameters.totalBright === Number(text)){
                isTotalBrightEqual = true
            }else{
                isTotalBrightEqual = false
            }
            checkDataEquality()
            setterText = text
        }

        onMultiplierChanged:{
            root.multiplierChanged(multiplier)
        }
    }
}
