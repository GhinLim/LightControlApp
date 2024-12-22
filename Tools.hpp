#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <QCoreApplication>
#include <QSettings>
#include <QVariant>
#include <QString>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
#include <QTextStream>

// 使用 C++17 的 inline 变量特性
inline QMutex* mutex = nullptr;
inline bool isConfigured = false;
inline QFile* logFile = nullptr;  // 全局日志文件

// 日志处理函数，定义为内联函数
inline void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    // 获取应用程序名称
    QString appName = QCoreApplication::applicationName();

    // 获取当前时间，并格式化为字符串
    QString timeStamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

    // 构建日志文件名：应用名_时间戳.log,并保存在用户主目录下
    QString logDir = QDir::homePath() + "/logs";
    QDir dir;
    if (!dir.exists(logDir)) {
        dir.mkpath(logDir);  // 如果目录不存在，创建目录
    }

    QString logFileName = logDir + "/" + appName + "_" + timeStamp + ".log";

    // 在程序启动时，只创建一次文件
    if (logFile == nullptr) {
        logFile = new QFile(logFileName);
        if (!logFile->open(QIODevice::Append | QIODevice::Text)) {
            qDebug() << "Failed to open log file: " << logFile->errorString();  // 打印错误信息
            return;
        }
    }

    // 使用互斥锁来确保线程安全
    if (mutex) {
        QMutexLocker locker(mutex);  // 自动加锁和解锁
    }

    QTextStream out(logFile);
    QString logMessage;

    logMessage = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " ";

    // 根据不同的消息类型，输出不同的日志级别
    switch (type) {
    case QtDebugMsg:
        logMessage += "DEBUG: ";
        break;
    case QtInfoMsg:
        logMessage += "INFO: ";
        break;
    case QtWarningMsg:
        logMessage += "WARNING: ";
        break;
    case QtCriticalMsg:
        logMessage += "CRITICAL: ";
        break;
    case QtFatalMsg:
        logMessage += "FATAL: ";
        break;
    }

    // 输出函数名和日志信息
    logMessage += QString("[%1] ").arg(QString(context.function));
    logMessage += msg;

    // 输出到日志文件
    out << logMessage << "\n";

    // 同时输出到控制台
    // 确保只在正确的级别调用对应的日志函数
    switch (type) {
    case QtDebugMsg:
        // 确保使用 qDebug() 输出绿色的调试信息
        qDebug() << logMessage;
        break;
    case QtInfoMsg:
        qInfo() << logMessage;
        break;
    case QtWarningMsg:
        qWarning() << logMessage;
        break;
    case QtCriticalMsg:
        qCritical() << logMessage;
        break;
    case QtFatalMsg:
        qFatal("%s", qPrintable(logMessage));
        break;
    }
}


// 配置日志函数，定义为内联函数
inline void configureLogging(bool isMultithreaded) {
    if (isConfigured) {
        return;  // 如果已经配置过，直接返回
    }

    // 设置日志是否使用互斥锁
    if (isMultithreaded) {
        mutex = new QMutex();
        qDebug() << "Mutex initialized.";
    } else {
        mutex = nullptr;
    }

    qInstallMessageHandler(logHandler);

    isConfigured = true;
}

inline void clearInput()
{
    QSettings settings("Julian Wong","Julian Wong's App");
    settings.clear();
}

inline void saveInput (const QVariant target,const QString &key)
{
    QSettings settings("Julian Wong","Julian Wong's App");
    settings.setValue(key,target);
}
inline void saveInput (const int target,const QString &key)
{
    QSettings settings("Julian Wong","Julian Wong's App");
    settings.setValue(key,target);
}
inline void saveInput (const float target,const QString &key)
{
    QSettings settings("Julian Wong","Julian Wong's App");
    settings.setValue(key,target);
}
inline void saveInput (const bool target,const QString &key)
{
    QSettings settings("Julian Wong","Julian Wong's App");
    settings.setValue(key,target);
}
inline void saveInput (const QString target,const QString &key)
{
    QSettings settings("Julian Wong","Julian Wong's App");
    settings.setValue(key,target);
}

inline void restoreInput(QVariant &target,const QString &key,QVariant defaultValue = 0)
{
    QSettings settings("Julian Wong","Julian Wong's App");
    target = settings.value(key,defaultValue);
}
inline void restoreInput(int &target,const QString &key,int defaultValue = 0)
{
    QSettings settings("Julian Wong","Julian Wong's App");
    target = settings.value(key,defaultValue).toInt();
}
inline void restoreInput(float &target,const QString &key,float defaultValue = 0)
{
    QSettings settings("Julian Wong","Julian Wong's App");
    target = settings.value(key,defaultValue).toFloat();
}
inline void restoreInput(bool &target,const QString &key, bool defaultValue = false)
{
    QSettings settings("Julian Wong","Julian Wong's App");
    target = settings.value(key,defaultValue).toBool();
}
inline void restoreInput(QString &target,const QString &key, QString defaultValue = "")
{
    QSettings settings("Julian Wong","Julian Wong's App");
    target = settings.value(key,defaultValue).toString();
}

#endif //TOOL_H
