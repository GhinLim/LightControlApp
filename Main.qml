import QtQuick 2.15
import QtQuick.Controls.Material
import LightController 1.0
import "qml"

ApplicationWindow {
    id: mainWindow
    visible: true
    width: /*950*/750
    height: /*960*/950
    minimumWidth:/*950*/750
    minimumHeight: /*1080*/950
    Material.accent: Material.Indigo
    title: "Lighting Controller 1.0.5"

    Component.onCompleted: {
        stackView.refreshData()
    }

    StackView{
        function refreshData(){
            homePage.refreshData()
        }

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
                lightController: LightController
                Component.onCompleted: back.connect(stackView.pop)
            }
        }
    }
}

