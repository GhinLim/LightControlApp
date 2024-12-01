import QtQuick
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Rectangle{
    id:background
    color: "white"
    radius: 10
    height: childrenRect.height
    width: 800

    Column{
        anchors.horizontalCenter: parent.horizontalCenter
        ComSettingItem{
            title: qsTr("PC-Online")
        }

        ComSettingItem{
            title: qsTr("200A-Online")
        }

        PwmHzSetting{
        }
    }


}
