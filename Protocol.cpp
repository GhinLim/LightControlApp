#include "Protocol.h"
#include "LightController.h"

Protocol::Protocol(QObject *parent)
    : QObject{parent}
{

}

void Protocol::init()
{
    LightController* lightController = qobject_cast<LightController*>(parent());
    m_pcOnlineCom = lightController->pcOnlineCom();
    m_a200OnlineCom = lightController->a200OnlineCom();
    m_onlineParam = lightController->onlineParam();

    m_pcOnlineTimer.setInterval(500);
    m_a200OnlineTimer.setInterval(500);
    m_pwmHzTimer.setInterval(500);

    connect(m_pcOnlineCom, &SerialCom::isOpenedChanged, &m_pcOnlineTimer,
            [this]() {
                if (m_pcOnlineCom->isOpened()) {
                    m_pcOnlineTimer.start();
                } else {
                    m_pcOnlineTimer.stop();
                }
            });

    connect(m_a200OnlineCom, &SerialCom::isOpenedChanged, &m_a200OnlineTimer,
            [this]() {
                if (m_a200OnlineCom->isOpened()) {
                    m_a200OnlineTimer.start();
                } else {
                    m_a200OnlineTimer.stop();
                }
            });

    connect(&m_pcOnlineTimer,&QTimer::timeout,this,[this](){
        LightController* lightController = qobject_cast<LightController*>(parent());
        QByteArray byteArray;
        QDataStream stream(&byteArray, QIODevice::WriteOnly);

        stream<<static_cast<qint8>(0xFA);
        stream<<static_cast<qint8>(0x1B);

        QString frameInfo;
        frameInfo += "主面板控制数据帧结构：[帧头:0xFA] ";
        frameInfo += "[帧长度:0x1B] ";

        QList<ChannelSetter*> channelSetterList = lightController->channelSetterList().toList<QList<ChannelSetter*>>();
        for (int i = 0; i < 12; i++) {
            bool isAnyOpened = false; // 用于跟踪是否有任何pwmSetter(i)被打开

            for (int j = 0; j < 12; j++) {
                ChannelSetter* channelSetter = channelSetterList.at(j);
                if(!channelSetter->isOpened())
                {
                    continue;
                }
                QList<PwmSetter*> pwmSetterList = channelSetter->pwmSetterList().toList<QList<PwmSetter*>>();

                if (pwmSetterList.at(i)->isOpened()) {
                    stream << static_cast<qint16>(pwmSetterList.at(i)->value());
                    isAnyOpened = true; // 标记为true，表示找到了一个打开的pwmSetter
                    frameInfo += QString("[PWM细分%1:CH-%2:%3] ").arg(i+1).arg(j+1).arg(QString::asprintf("0x%02X 0x%02X", (static_cast<qint16>(pwmSetterList.at(i)->value()) >> 8) & 0xFF, static_cast<qint16>(pwmSetterList.at(i)->value()) & 0xFF));
                    break;
                }
            }

            // 如果没有任何pwmSetter(i)被打开，则追加两个为0的字节
            if (!isAnyOpened) {
                stream << static_cast<qint16>(0);
                frameInfo += QString("[PWM细分%1:0x00 0x00] ").arg(i+1);
            }
        }

        frameInfo += "[帧尾:0xFF]";
        stream<<static_cast<qint8>(0xFF);
        m_pcOnlineCom->writeData(byteArray);
        qDebug()<<frameInfo;
        QStringList hexList;
        for (const auto& byte : byteArray) {
            // 使用QString::asprintf格式化每个字节为0x的十六进制形式
            hexList.append(QString::asprintf("0x%02X", static_cast<unsigned char>(byte)));
        }

        // 使用qDebug()输出
        qDebug() << hexList.join(" ");
    });

    connect(&m_pwmHzTimer,&QTimer::timeout,this,[this](){
        static int sendCount = 0;
        LightController* lightController = qobject_cast<LightController*>(parent());
        QByteArray byteArray;
        QDataStream stream(&byteArray, QIODevice::WriteOnly);

        stream<<static_cast<qint8>(0xFB);
        stream<<static_cast<qint8>(0x1B);

        QString frameInfo;
        frameInfo += "PWM频率设定数据帧结构：[帧头:0xFB] ";
        frameInfo += "[帧长度:0x1B] ";

        stream << static_cast<qint32>(lightController->pwmHz());
        frameInfo += QString("[PWM频率:%1] ").arg(QString::asprintf("0x%02X 0x%02X 0x%02X 0x%02X", (static_cast<qint32>(lightController->pwmHz()) >> 24) & 0xFF, (static_cast<qint32>(lightController->pwmHz()) >> 16) & 0xFF, (static_cast<qint32>(lightController->pwmHz()) >> 8) & 0xFF, static_cast<qint32>(lightController->pwmHz()) & 0xFF));

        stream<<static_cast<qint8>(0x01);
        frameInfo += "[PWM开关:0x01] ";

        for(int i=0;i<19;i++)
        {
            stream << static_cast<qint16>(0);
            frameInfo += QString("[空值%1:0x00] ").arg(i+1);
        }

        frameInfo += "[帧尾:0xFF]";
        stream<<static_cast<qint8>(0xFF);
        m_pcOnlineCom->writeData(byteArray);

        qDebug()<<frameInfo;
        QStringList hexList;
        for (const auto& byte : byteArray) {
            // 使用QString::asprintf格式化每个字节为0x的十六进制形式
            hexList.append(QString::asprintf("0x%02X", static_cast<unsigned char>(byte)));
        }

        // 使用qDebug()输出
        qDebug() << hexList.join(" ");
        sendCount++;
        if(sendCount == 3){
            m_pwmHzTimer.stop();
            sendCount = 0;
            if(m_pcOnlineCom->isOpened()){
                m_pcOnlineTimer.start();
            }
        }
    });

}

void Protocol::sendPwmHz()
{
    if(m_pcOnlineCom->isOpened()){
        m_pcOnlineTimer.stop();
        m_pwmHzTimer.start();
    }
}
