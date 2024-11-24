import QtQuick
import QtQuick.Layouts

Row {
    property var channelParameters
    spacing: 33
    ChannelParamSetter{
        title:"Color Temperature"
        source:"../image/colorTemp.png"
        value: channelParameters.colorTemp
        onParamTextChanged: channelParameters.colorTemp = Number(text)
    }
    ChannelParamSetter{
        title: "Total Brightness"
        source: "../image/totalBrightness.png"
        value: channelParameters.totalBright
        hasCtrlBtn: true
        onParamTextChanged: channelParameters.totalBright = Number(text)
    }
    ChannelParamSetter{
        title:"Brightness"
        source:"../image/brightness.png"
        value: channelParameters.brightness
        onParamTextChanged: channelParameters.brightness = Number(text)
    }
}
