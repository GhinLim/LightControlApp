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
    title: "Lighting Controller 1.0.8"

    Component.onCompleted: {
        homePage.refreshData()
    }

    HomePage{
        anchors.fill: parent
        lightController: LightController
        id:homePage
        onPushSettingPage: {
            homePage.setDrawerOpen(false)
            settingPage.visible = true
        }
    }

    SettingPage{
        id:settingPage
        visible: false
        anchors.fill: parent
        lightController: LightController
        onBack: {
            visible = false
            homePage.setDrawerOpen(true)
        }
    }

    Message{
        id:messageTip
        z: 1
        parent: Overlay.overlay
    }

    Connections{
        target: LightController
        function onMessage(type,msg){
            message(type,msg)
        }
    }

    function message(type, message) {
        if(type!=='success'&&type!=='error'&&type!=='info'){
            return false
        }
        messageTip.open(type, message)
    }
}

