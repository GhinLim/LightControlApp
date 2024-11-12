import QtQuick
import QtQuick.Controls
TabBar {
    height: 50
    background: Rectangle{
        anchors.fill: parent
        border.color: "transparent"
    }

    Repeater{
        model:12
        ChannelButton{
            neonText:"CH-"+(index<9?"0":"")+String(index+1)
        }
    }


}
