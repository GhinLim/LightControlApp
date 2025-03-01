#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QSurfaceFormat>
#include "LightController.h"
#include "ChannelSetter.h"
#include "PwmSetter.h"
#include "OnlineParam.h"
#include "SettingManager.h"
#include "Tools.hpp"

int main(int argc, char *argv[])
{
    // 设置 OpenGL 格式
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    QSurfaceFormat::setDefaultFormat(format);

    QGuiApplication app(argc, argv);
    configureLogging(false);
    // clearInput();
    app.setWindowIcon(QIcon(":/LightControlApp/image/appLogo.png"));
    qDebug()<<"GLHF";

    LightController* lightController =qobject_cast<LightController*>(LightController::instance());
    lightController->init();

    qmlRegisterSingletonType<LightController>("LightController", 1, 0, "LightController",LightController::instance);
    qmlRegisterType         <ChannelSetter>  ("LightController", 1, 0, "ChannelSetter");
    qmlRegisterType         <PwmSetter>      ("LightController", 1, 0, "PwmSetter");
    qmlRegisterType         <OnlineParam>    ("LightController", 1, 0, "OnlineParam");
    qmlRegisterType         <SettingManager> ("LightController", 1, 0, "SettingManager");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/LightControlApp/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
