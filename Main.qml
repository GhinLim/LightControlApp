import QtQuick 2.15
import QtQuick.Controls.Material
import LightController 1.0
import "qml"

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 950
    height: 1080
    minimumWidth:950
    minimumHeight: 1080
    Material.accent: Material.Indigo
    title: "Lighting Controller"

    StackView{
        id:stackView
        anchors.fill: parent
        initialItem:     HomePage{
            lightController: LightController
            id:homePage
            onPushSettingPage: stackView.push(settingPageCpnt)
        }

        onCurrentItemChanged: {
            if(stackView.currentItem === homePage){
                homePage.setDrawerOpen(true)
            }else{
                homePage.setDrawerOpen(false)
            }
        }

        Component {
            id:settingPageCpnt
            SettingPage{
                Component.onCompleted: back.connect(stackView.pop)
            }
        }
    }
}

