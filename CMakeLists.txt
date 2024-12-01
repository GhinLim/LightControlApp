cmake_minimum_required(VERSION 3.16)

project(LightControlApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Quick SerialPort)


qt_add_executable(appLightControlApp
    main.cpp
)

qt_add_qml_module(appLightControlApp
    URI LightControlApp
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES qml/ChannelButton.qml
        QML_FILES qml/ChannelBar.qml
        QML_FILES qml/NeonText.qml
        QML_FILES qml/PwmSetter.qml
        RESOURCES image/save.png
        QML_FILES qml/ParamBoard.qml
        QML_FILES qml/DoubleTabBar.qml
        QML_FILES qml/ControlPanel.qml
        QML_FILES qml/ChannelParamSetter.qml
        RESOURCES image/brightness.png image/colorTemp.png image/totalBrightness.png
        QML_FILES qml/ChannelParamBoard.qml
        QML_FILES qml/ParamLabel.qml
        QML_FILES qml/OnlineParamBoard.qml
        QML_FILES qml/HomePage.qml
        RESOURCES image/appLogo.png
        RESOURCES image/up1.png image/up2.png image/up3.png
        SOURCES PwmSetter.h PwmSetter.cpp
        SOURCES ChannelSetter.h ChannelSetter.cpp
        SOURCES LightController.h LightController.cpp
        SOURCES OnlineParam.h OnlineParam.cpp
        RESOURCES image/setting.png
        SOURCES SettingManager.h SettingManager.cpp
        SOURCES SerialCom.h SerialCom.cpp
        QML_FILES qml/SettingPage.qml
        RESOURCES image/back.png
        QML_FILES qml/ComSettingItem.qml
        QML_FILES qml/SettingBoard.qml
        QML_FILES qml/PwmHzSetting.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appLightControlApp PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appLightControlApp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appLightControlApp
    PRIVATE Qt6::Quick Qt6::SerialPort
)

include(GNUInstallDirs)
install(TARGETS appLightControlApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
