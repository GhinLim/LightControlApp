import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item{
    property alias model: repeater.model

    function clearTextFieldFocus(){
        for (var i = 0; i < repeater.count; i++) {
            if (repeater.itemAt(i).textFieldFocus) {
                repeater.itemAt(i).textFieldFocus  = false
                break
            }
        }
    }

    width: childrenRect.width
    height: childrenRect.height+30

    /*Label{
        id:title1
        text:"Channel parameter setting"
        anchors.left: channelParamBoard.left
        font.pixelSize: 25
        font.bold: true
        color: "black"
        anchors.top: parent.top
        Component.onCompleted: {
            console.log("title1.x",x)
            console.log("title1.y",y)
        }
    }*/

    // Button{
    //     id:saveBtn
    //     width: 80
    //     height: 40
    //     anchors.right: channelParamBoard.right
    //     Material.background: Material.Indigo
    //     layer.enabled: true
    //     layer.smooth: true
    //     contentItem: Image {
    //         scale: 1.6
    //         anchors.centerIn: parent
    //         source: "../image/save.png"
    //         fillMode: Image.PreserveAspectFit
    //     }
    // }

    ChannelParamBoard{
        id:channelParamBoard
        anchors.horizontalCenter: grid.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 35
    }

    /*Label{
        id:title2
        text:"PWM setting"
        anchors.left: channelParamBoard.left
        font.pixelSize: 25
        font.bold: true
        color: "black"
        anchors.top: channelParamBoard.bottom
        anchors.topMargin: 20
        Component.onCompleted: {
            console.log("title2.x",x)
            console.log("title2.y",y)
        }
    }*/

    Grid {
        id:grid
        columns: 2
        columnSpacing: -50
        anchors.top: /*title2.bottom*/channelParamBoard.bottom
        anchors.topMargin: 64

        Repeater{
            id:repeater
            model:12
            PwmSetter{
                scale: 0.8
                titleText: "W-"+String(index+1)
            }
        }
    }
}


