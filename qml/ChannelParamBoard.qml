import QtQuick
import QtQuick.Layouts

Row {
    spacing: 33
    ParamSetter{
        title:"Color Temperature"
        source:"../image/colorTemp.png"
        value: "123456"
    }
    ParamSetter{
        title: "Total Brightness"
        source: "../image/totalBrightness.png"
        value: "656544"
    }
    ParamSetter{
        title:"Brightness"
        source:"../image/brightness.png"
        value: "776"
    }
}
