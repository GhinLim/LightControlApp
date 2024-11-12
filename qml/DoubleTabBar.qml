import QtQuick
import QtQuick.Controls

Item {
    id:root
    // 定义 model 属性，接受整数或模型数据
    width: childrenRect.width
    height: childrenRect.height
    property int selectedIndex: 6
    property var model

    property var firstTabBarModel
    property var secondTabBarModel

    function splitModel(){
        if (typeof model === "number") {
            console.log("model is number!")
            // 如果 model 是整数，简单分成两部分
            firstTabBarModel = model / 2
            secondTabBarModel = model / 2

        }
        else if (Array.isArray(model)) {
            console.log("model is array!")
            // 如果 model 是数组，将数组分成两部分
            let half = Math.ceil(model.length / 2)
            firstTabBarModel = model.slice(0, half)
            secondTabBarModel = model.slice(half)

        }
        else if (model instanceof ListModel) {
            console.log("model is ListModel!")
            // 如果 model 是 ListModel，将内容分割并存入两个新 ListModel 中
            let half = Math.ceil(model.count / 2)

            // 创建两个新的 ListModel 用于存储前半部分和后半部分
            firstTabBarModel = Qt.createQmlObject("import QtQuick; ListModel {}", this)
            secondTabBarModel = Qt.createQmlObject("import QtQuick; ListModel {}", this)

            for (let i = 0; i < model.count; i++) {
                if (i < half) {
                    firstTabBarModel.append(model.get(i))
                } else {
                    secondTabBarModel.append(model.get(i))
                }
            }
        }
    }

    function controlSelectLine(){
        if (typeof model === "number"){
            if(selectedIndex<model/2){
                line1.visible = true
                line2.visible = false
                line2.x = 0
                line1.x = channelBar1.width/model*2*selectedIndex
            }else{
                line1.visible = false
                line1.x = 0
                line2.visible = true
                line2.x = channelBar2.width/model*2*(selectedIndex-model/2)
            }
        }else if (Array.isArray(model)){
            if(selectedIndex<model.length/2){
                line1.visible = true
                line2.visible = false
                line2.x = 0
                line1.x = channelBar1.width/model.length*2*selectedIndex
            }else{
                line1.visible = false
                line1.x = 0
                line2.visible = true
                line2.x = channelBar2.width/model.length*2*(selectedIndex-model.length/2)
            }

        }else if (model instanceof ListModel){
            if(selectedIndex<model.count/2){
                line1.visible = true
                line2.visible = false
                line2.x = 0
                line1.x = channelBar1.width/model.count*2*selectedIndex
            }else{
                line1.visible = false
                line1.x = 0
                line2.visible = true
                line2.x = channelBar2.width/model.count*2*(selectedIndex-model.count/2)
            }
        }
    }

    onSelectedIndexChanged: {
        controlSelectLine()
    }

    onModelChanged: {
        splitModel()
    }

    Component.onCompleted: {
        splitModel()
        controlSelectLine()

    }

    TabBar {
        id: channelBar1
        height: 60
        background: Rectangle{
            anchors.fill: parent
            border.color: "transparent"
        }

        Repeater {
            model: firstTabBarModel
            ChannelButton {
                // 示例：这里可以设置 ChannelButton 的属性
                neonText: "CH-" + (index<9?"0":"") + String(index+1)
                checked: selectedIndex === index
                onClicked: selectedIndex = index
            }
        }


    }
    Rectangle{
        id:line1
        width: 144
        height: 2
        anchors.bottom: channelBar1.bottom
        radius: 5
        color: "black"
        Behavior on x{
            NumberAnimation{
                duration: 200
            }
        }
    }

    TabBar {
        id: channelBar2
        height: 60
        background: Rectangle{
            anchors.fill: parent
            border.color: "transparent"
        }
        anchors.top: channelBar1.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: channelBar1.horizontalCenter
        Repeater {
            model: secondTabBarModel
            ChannelButton {
                // 示例：这里可以设置 ChannelButton 的属性
                checked: {
                    if (typeof root.model === "number"){
                        return  root.selectedIndex === index+root.model/2
                    }else if (Array.isArray(root.model)) {
                        return root.selectedIndex === index+root.model.length/2
                    }else if (root.model instanceof ListModel) {
                        return root.selectedIndex === index+root.model.count/2
                    }
                }
                onClicked: {
                    if (typeof root.model === "number"){
                        root.selectedIndex = index+root.model/2
                    }else if (Array.isArray(root.model)) {
                        root.selectedIndex = index+root.model.length/2
                    }else if (root.model instanceof ListModel) {
                        root.selectedIndex = index+root.model.count/2
                    }
                    console.log("onClicked:index=",index)
                }
                neonText: {
                    if (typeof root.model === "number"){
                        return "CH-" + (index+root.model/2<9?"0":"") + String(index+1+ root.model/2)
                    }else if (Array.isArray(root.model)) {
                        return "CH-" + (index+root.model.length/2<9?"0":"") + String(index+1+root.model.length/2)
                    }else if (root.model instanceof ListModel) {
                        return "CH-" + (index+root.model.count/2<9?"0":"") + String(index+1+root.model.count/2)
                    }
                }
            }
        }

        Component.onCompleted: {
            if (typeof root.model === "number"){
                 if(root.selectedIndex != root.model/2){
                     itemAt(0).checked = false
                 }
            }else if (Array.isArray(root.model)) {
                if(root.selectedIndex != root.model.length/2){
                    itemAt(0).checked = false
                }
            }else if (root.model instanceof ListModel) {
                if(root.selectedIndex != root.model.count/2){
                    itemAt(0).checked = false
                }
            }
        }
    }

    Rectangle{
        id:line2
        width: 144
        height: 2
        anchors.bottom: channelBar2.bottom
        radius: 5
        color: "black"
        Behavior on x{
            NumberAnimation{
                duration: 200
            }
        }
    }
}
