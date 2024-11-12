import Qt5Compat.GraphicalEffects
import QtQuick

Item {
    id: root
    height: textItem.height
    width: textItem.width
    property alias text: textItem.text
    property alias font: textItem.font

    // 正常文字颜色
    property color normalColor: "white"
    // 霓虹灯效果颜色
    property color neonColor: "cyan"
    // 当前是否启用霓虹灯效果
    property bool isNeonEffect: false

    // 内部属性，用于动画过渡
    property color _currentColor: normalColor
    property real _currentGlow: 0.0

    // 外部调用的函数，触发霓虹灯动画效果
    function triggerNeonEffect(color) {
        neonEffectColor = color
        neonAnimation.start()
    }

    // 动画过程中使用的临时属性
    property color neonEffectColor: "cyan"
    property color _originalColor: normalColor
    property bool _originalIsNeonEffect: isNeonEffect

    // 文字控件
    Text {
        id: textItem
        color: root._currentColor
        font: root.font
        text: root.text
    }

    // 第一层光晕（最内层）
    FastBlur {
        id: innerGlow
        anchors.fill: textItem
        source: textItem
        radius: root._currentGlow * 0.5
        transparentBorder: true
        visible: root.isNeonEffect || neonAnimation.running
    }

    // 第二层光晕（中间层）
    ColorOverlay {
        anchors.fill: textItem
        source: innerGlow
        color: Qt.lighter(root._currentColor, 1.2)
    }

    FastBlur {
        id: middleGlow
        anchors.fill: textItem
        source: innerGlow
        radius: root._currentGlow
        transparentBorder: true
        visible: root.isNeonEffect || neonAnimation.running
    }

    // 第三层光晕（最外层）
    ColorOverlay {
        anchors.fill: textItem
        source: middleGlow
        color: Qt.lighter(root._currentColor, 1.4)
    }

    FastBlur {
        id: outerGlow
        anchors.fill: textItem
        source: middleGlow
        radius: root._currentGlow * 1.5
        transparentBorder: true
        visible: root.isNeonEffect || neonAnimation.running
    }

    // 启动动画（增加闪烁效果）
    SequentialAnimation {
        id: neonAnimation
        running: false
        loops: 1

        ScriptAction {
            script: {
                // 保存原始状态
                root._originalColor = root._currentColor
                root._originalIsNeonEffect = root.isNeonEffect
            }
        }

        // **在动画开始时，将 _currentColor 设置为 neonEffectColor**
        PropertyAction {
            target: root
            property: "_currentColor"
            value: neonEffectColor
        }

        // 闪烁效果（使用指定的 neonEffectColor）
        PropertyAnimation { target: root; property: "_currentGlow"; to: 10; duration: 100 }
        PropertyAnimation { target: root; property: "_currentGlow"; to: 0; duration: 100 }
        PropertyAnimation { target: root; property: "_currentGlow"; to: 30; duration: 100 }

        // 渐变到指定的霓虹灯效果（颜色已设置，只需要调整光晕）
        PropertyAnimation { target: root; property: "_currentGlow"; to: 40; duration: 300 }

        // 保持一段时间
        PauseAnimation { duration: 500 }

        // **修改渐变回原始状态的动画**
        // 渐变回原始状态
        ParallelAnimation {
            // **仅当 _originalIsNeonEffect 为 true 时，才过渡 _currentColor**
            PropertyAnimation {
                target: root
                property: "_currentColor"
                to: root._originalIsNeonEffect ? root._originalColor : neonEffectColor
                duration: 500
            }
            PropertyAnimation {
                target: root
                property: "_currentGlow"
                to: root._originalIsNeonEffect ? 30 : 0
                duration: 500
            }
        }

        onStopped: {
            // 恢复到原始的霓虹灯效果状态
            root.isNeonEffect = root._originalIsNeonEffect

            // **当 _originalIsNeonEffect 为 false 时，将 _currentColor 设置回 normalColor**
            if (!root._originalIsNeonEffect) {
                root._currentColor = root.normalColor
            }
        }
    }

    // 当 isNeonEffect 为真时，始终显示霓虹灯效果
    onIsNeonEffectChanged: {
        if (isNeonEffect) {
            _currentGlow = 30
            _currentColor = neonColor
        } else {
            _currentGlow = 0
            _currentColor = normalColor
        }
    }

    // 当 neonColor 改变时，更新 _currentColor
    onNeonColorChanged: {
        if (isNeonEffect) {
            _currentColor = neonColor
        }
    }
}
