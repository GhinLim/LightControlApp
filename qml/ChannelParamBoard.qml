import QtQuick
import QtQuick.Layouts

Row {
    spacing: 33
    ChannelParamSetter{
        title:"Color Temperature"
        source:"../image/colorTemp.png"
        value: "123456"
    }
    ChannelParamSetter{
        title: "Total Brightness"
        source: "../image/totalBrightness.png"
        value: "656544"
    }
    ChannelParamSetter{
        title:"Brightness"
        source:"../image/brightness.png"
        value: "776"
    }
}
