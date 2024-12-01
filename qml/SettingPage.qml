import QtQuick
import QtQuick.Controls

Item {
    signal back()
    Rectangle {
        id:background
        anchors.fill: parent
        color: "#595973"
        MouseArea{
            anchors.fill: parent
            onClicked: background.forceActiveFocus()
        }
    }



    Button {
        id: backLogo
        width: 30
        height: 30
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 30
        anchors.topMargin: 30

        background: Item {
            id: imageContainer
            width: parent.width
            height: parent.height
            scale: 1.0
            opacity: 1.0

            Image {
                anchors.fill: parent
                source: "../image/back.png"
                layer.enabled: true
                layer.smooth: true
                layer.samplerName: "linear"
            }

            states: [
                State {
                    name: "hovered"
                    when: backLogo.hovered && !backLogo.pressed
                    PropertyChanges { target: imageContainer; opacity: 0.7; scale: 1.2 }
                },
                State {
                    name: "pressed"
                    when: backLogo.pressed
                    PropertyChanges { target: imageContainer; opacity: 0.5; scale: 0.9 }
                },
                State {
                    name: "normal"
                    when: !backLogo.hovered && !backLogo.pressed
                    PropertyChanges { target: imageContainer; opacity: 1.0; scale: 1.0 }
                }
            ]

            transitions: [
                Transition {
                    NumberAnimation { properties: "opacity, scale"; duration: 200 }
                }
            ]
        }

        onClicked:back()
    }

    Label{
        id:title
        text: "Settings"
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 30
        font.bold: true
        color: "white"
    }

    SettingBoard{
        anchors.top: title.bottom
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
