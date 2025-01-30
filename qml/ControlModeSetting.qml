import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    property alias model: combox.model
    property alias currentIndex:combox.currentIndex
    property alias currentText: combox.currentText
    width: 600
    height:80
    Label{
        id:label
        font.pixelSize: 20
        font.bold: true
        color: "black"
        font.family: "Consolas"
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        text:"Control Mode"
    }

    ComboBox{
        id:combox
        width: 150
        anchors.left: label.right
        anchors.leftMargin: 128
        anchors.verticalCenter: parent.verticalCenter
    }
}
