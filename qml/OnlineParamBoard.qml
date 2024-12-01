import QtQuick
import QtQuick.Controls

Row {
    property var onlineParam
    width: childrenRect.width
    height: childrenRect.height
    spacing: 32

    ParamLabel{
        value: onlineParam.colorTemp
        title:"Color Temperature"
        unit:"(K)"
        onParamTextChanged: onlineParam.colorTemp = Number(text)
    }
    ParamLabel{
        value:onlineParam.brightness
        title:"Brightness"
        unit:"(lux)"
        onParamTextChanged: onlineParam.brightness = Number(text)
    }
    ParamLabel{
        value:onlineParam.xColorCoord
        title:"X Color Coord"
        onParamTextChanged: onlineParam.xColorCoord = Number(text)
    }
    ParamLabel{
        value:onlineParam.yColorCoord
        title:"Y Color Coord"
        onParamTextChanged: onlineParam.yColorCoord = Number(text)
    }
    ParamLabel{
        value:onlineParam.uvValue
        title:"UV Value"
        onParamTextChanged: onlineParam.uvValue = Number(text)
    }
}
