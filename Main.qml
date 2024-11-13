import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material
import "qml"

Window {
    id: mainWindow
    visible: true
    width: 950
    height: 1080
    minimumWidth:950
    minimumHeight: 1080
    Material.accent: Material.Indigo
    title: "Lighting Control System"

    HomePage{
        id:homePage
        anchors.fill: parent
    }
}

