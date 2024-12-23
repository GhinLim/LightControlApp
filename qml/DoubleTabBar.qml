import QtQuick
import QtQuick.Controls

Item {
    id:root
    // 定义 model 属性，接受整数或模型数据
    width: childrenRect.width
    height: childrenRect.height
    property int selectedIndex: 0
    property var model

    property var firstTabBarModel
    property var secondTabBarModel

    function glitterChannelText(channelIndex){
        if(channelIndex<model.length/2){
            channelBar1.itemAt(channelIndex).triggerNeonEffect("red")
        }else{
            channelBar2.itemAt(channelIndex-model.length/2).triggerNeonEffect("red")
        }
    }

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
        else /*if (model instanceof ListModel)*/ {
            console.log("model is ListModel!")
            if (!model || model.length === 0) {
                        firstTabBarModel = [];
                        secondTabBarModel = [];
                        return;
                    }

                    let mid = Math.floor(model.length / 2); // 中间索引
                    firstTabBarModel = model.slice(0, mid); // 前半部分
                    secondTabBarModel = model.slice(mid);  // 后半部分
        }
    }

    onSelectedIndexChanged: {
        if (typeof root.model === "number"){
            if( root.selectedIndex < root.model/2){
                channelBar2.currentIndex = -1
                channelBar1.currentIndex = root.selectedIndex
            }else{
                channelBar1.currentIndex = -1
                channelBar2.currentIndex = root.selectedIndex - root.model/2
            }
        }else if (Array.isArray(root.model)) {
            if(root.selectedIndex < root.model.length/2){
                channelBar2.currentIndex = -1
                channelBar1.currentIndex = root.selectedIndex
            }else{
                channelBar1.currentIndex = -1
                channelBar2.currentIndex = root.selectedIndex - root.model.length/2
            }
        }else{
            if(root.selectedIndex < root.model.length/2){
                channelBar2.currentIndex = -1
                channelBar1.currentIndex = root.selectedIndex
            }else{
                channelBar1.currentIndex = -1
                channelBar2.currentIndex = root.selectedIndex - root.model.length/2
            }
        }
    }

    onModelChanged: {
        splitModel()
    }

    Component.onCompleted: {
        splitModel()
        channelBar2.currentIndex = -1

    }

    TabBar {
        id: channelBar1
        height: 60
        background: Rectangle{
            anchors.fill: parent
            border.color: "transparent"
        }

        onCurrentIndexChanged: {
            if(currentIndex === -1){
                for(var i=0;i<contentChildren.length;i++){
                    var tabButton = itemAt(i);
                    tabButton.checked = false
                }
            }
        }

        Repeater {
            model: firstTabBarModel
            ChannelButton {
                // 示例：这里可以设置 ChannelButton 的属性
                neonText: "CH-" + (index<9?"0":"") + String(index+1)
                onClicked: selectedIndex = index
                onSwCheckedChanged: modelData.isOpened = swChecked
                Component.onCompleted: swChecked = modelData.isOpened
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

        onCurrentIndexChanged: {
            if(currentIndex === -1){
                for(var i=0;i<contentChildren.length;i++){
                    var tabButton = itemAt(i);
                    tabButton.checked = false
                }
            }
        }

        Repeater {
            model: secondTabBarModel
            ChannelButton {
                // 示例：这里可以设置 ChannelButton 的属性
                onClicked: {
                    if (typeof root.model === "number"){
                        root.selectedIndex = index+root.model/2
                    }else if (Array.isArray(root.model)) {
                        root.selectedIndex = index+root.model.length/2
                    }else{
                        root.selectedIndex = index+root.model.length/2
                    }
                }
                neonText: {
                    if (typeof root.model === "number"){
                        return "CH-" + (index+root.model/2<9?"0":"") + String(index+1+ root.model/2)
                    }else if (Array.isArray(root.model)) {
                        return "CH-" + (index+root.model.length/2<9?"0":"") + String(index+1+root.model.length/2)
                    }else{
                        return "CH-" + (index+root.model.length/2<9?"0":"") + String(index+1+root.model.length/2)
                    }
                }
                onSwCheckedChanged: modelData.isOpened = swChecked
                Component.onCompleted: swChecked = modelData.isOpened
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
            }else {
                if(root.selectedIndex != root.model.length/2){
                    itemAt(0).checked = false
                }
            }
        }
    }
}
