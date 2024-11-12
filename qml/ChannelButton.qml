import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

TabButton {

    property alias neonText: neonText.text
    property alias textNormalColor: neonText.normalColor
    property alias textNeonColor: neonText.neonColor
    property alias isTextNeonEffect: neonText.isNeonEffect
    signal swToggled(bool checked)

    function triggerNeonEffect(color)
    {
        neonText.triggerNeonEffect(color)
    }

    id: tabBtn
    width: /*neonText.width + sw.width*/ 150
    height: 60
    anchors.top: parent.top

    background: Rectangle{
        id:bg
        anchors.fill: parent
        radius: 10
    }

    onHoveredChanged: {
        if(hovered)
            bg.color = "#efefef"
        else
            bg.color = "white"
    }

    onCheckedChanged: {
        if (checked) {
            neonText.normalColor = "black"
        } else {
            neonText.normalColor = "gray"
        }
    }

    contentItem: Item {
        Switch {
            id: sw
            rotation: -90
            scale: 0.45
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: neonText.left
            anchors.rightMargin: -25
            onToggled: {
                tabBtn.clicked()
                // if (checked) {
                //     console.log("Switch is turned ON");
                // } else {
                //     console.log("Switch is turned OFF");
                // }
                swToggled(checked)
            }
        }

        NeonText{
            id:neonText
            text: "neonText"
            normalColor: "gray"
            font.pixelSize: 22
            anchors.centerIn: parent
        }
    }

}
