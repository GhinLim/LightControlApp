import QtQuick

Item {
    property alias value: param.text
    property alias title: title.text
    width: 140
    height: 80

    NeonText{
        id:param
        normalColor: "white"
        text:"123456"
        font.pixelSize: 35
        anchors.centerIn: parent
    }

    NeonText{
        id:title
        normalColor: "white"
        text: "Color Temperature"
        font.pixelSize: 15
        anchors.top: param.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
