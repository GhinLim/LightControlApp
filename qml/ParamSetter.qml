import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import QtQuick.Controls.Material

Item {
    property alias source: logo.source
    property alias value: param.text
    property alias title: title.text
    width: 260
    height: 160
    Rectangle{
        id:background
        anchors.fill: parent
        color: "white"
        radius: 30

        Image{
            id:logo
            width: 40
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20
        }

        NeonText{
            id:param
            anchors.top: logo.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 8
            font.pixelSize: 40
            normalColor: "black"
            font.bold: true
        }

        NeonText{
            id:title
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 17
            font.pixelSize: 20
            normalColor: "black"
        }
    }



    DropShadow{
        anchors.fill: background
        source: background
        radius:20
        samples: 16
        color:"lightGray"
        spread: 0.2
    }
}
