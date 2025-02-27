#include <QtEndian>
#include "Protocol.h"
#include "LightController.h"
#include "ChannelSetter.h"
#include <QEventLoop>

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

    connect(m_pcOnlineCom, &SerialCom::isOpenedChanged, this, [this]() {
        if (m_pcOnlineCom->isOpened())
        {

            //初始化成功后，开启测量定时器
            m_pcOnlineTimer.start();
        }
        else
        {
            m_pcOnlineTimer.stop();
        }
    });

    connect(m_a200OnlineCom, &SerialCom::isOpenedChanged, &m_a200OnlineTimer,
            [this]() {
                if (m_a200OnlineCom->isOpened()) {
                    //初始化
                    QEventLoop loop;
                    QTimer sendTimer;
                    QTimer timeoutTimer;
                    QMetaObject::Connection sendConnection;
                    QMetaObject::Connection readConnection;
                    bool replyReceived = false;

                    // 设置超时定时器
                    timeoutTimer.setInterval(5000);
                    timeoutTimer.setSingleShot(true);

                    // 发送定时器每500ms触发一次
                    sendTimer.setInterval(500);

                    connect(&timeoutTimer, &QTimer::timeout, [&](){
                        qDebug() << "Timeout occurred!";
                        loop.quit(); // 超时，退出事件循环
                    });

                    /*------------------------------*/
                    /*Step 2 发送命令54 (设置PC连接模式)*/
                    /*------------------------------*/

                    sendConnection = connect(&sendTimer, &QTimer::timeout, [&]() {
                        QByteArray byteArray;
                        QDataStream stream(&byteArray, QIODevice::WriteOnly);

                        // 1. 添加 STX (0x02)
                        stream << (quint8)0x02;

                        // 2. 发送命令字符串 "00541    "
                        QByteArray command = "00541   ";
                        stream.writeRawData(command.constData(), command.size());

                        // 3. 计算 BCC（从 "00" 开始到 ETX 之前）
                        quint8 bcc = 0;
                        for (char c : command) {
                            bcc ^= c;
                        }
                        bcc ^= 0x03; // 异或 ETX

                        // 4. 追加 ETX (0x03)
                        stream << (quint8)0x03;

                        // 5. 追加 BCC（以 ASCII 形式）
                        QString bccStr = QString("%1").arg(bcc, 2, 16, QChar('0')).toUpper(); // 转换为2位16进制大写字符串
                        stream.writeRawData(bccStr.toUtf8().constData(), bccStr.size());

                        // 6. 追加 CR + LF (0x0D, 0x0A)
                        stream << (quint8)0x0D << (quint8)0x0A;
                        m_a200OnlineCom->writeData(byteArray);
                        QStringList hexList;
                        for (const auto& byte : byteArray) {
                            // 使用QString::asprintf格式化每个字节为0x的十六进制形式
                            hexList.append(QString::asprintf("0x%02X", static_cast<unsigned char>(byte)));
                        }

                        // 使用qDebug()输出
                        qDebug() << "Sent command 54:"<< hexList.join(" ");
                    });

                    readConnection = connect(m_a200OnlineCom, &SerialCom::readReady, [&]() {
                        QByteArray receivedData = m_a200OnlineCom->readAll();
                        qDebug() << "Data received:" << receivedData;
                        // 1. 检查最小长度 (STX + 命令 + ETX + BCC + CR + LF)
                        if (receivedData.size() < 14) {
                            qDebug() << "Invalid response: Too short";
                            return;
                        }

                        // 2. 检查 STX (0x02) 和 ETX (0x03)
                        if (receivedData[0] != 0x02 || receivedData[receivedData.size() - 5] != 0x03) {
                            qDebug() << "Invalid response: Incorrect STX or ETX";
                            return;
                        }

                        // 3. 提取命令和设备编号
                        QByteArray commandData = receivedData.mid(1, 6);  // "0054  "
                        if (!commandData.startsWith("0054  ")) {
                            qDebug() << "Unexpected command response:" << commandData;
                            return;
                        }

                        // 4. 提取 BCC（倒数第三、第四个字节）
                        QByteArray receivedBCC = receivedData.mid(receivedData.size() - 4, 2); // BCC 是两个字符的 ASCII
                        QByteArray expectedBCC;

                        // 计算 BCC（从 "00" 到 ETX）
                        quint8 calculatedBCC = 0;
                        for (int i = 1; i < receivedData.size() - 4; i++) {
                            calculatedBCC ^= receivedData[i];
                        }

                        // 转换 BCC 为 ASCII 形式
                        expectedBCC = QString("%1").arg(calculatedBCC, 2, 16, QChar('0')).toUpper().toUtf8();

                        if (receivedBCC != expectedBCC) {
                            qDebug() << "BCC mismatch: Expected" << expectedBCC << "but got" << receivedBCC;
                            return;
                        }

                        // 5. 确保 CR + LF 结尾 (0x0D 0x0A)
                        if (receivedData[receivedData.size() - 2] != 0x0D || receivedData[receivedData.size() - 1] != 0x0A) {
                            qDebug() << "Invalid response: Incorrect CR+LF termination";
                            return;
                        }

                        qDebug() << "Valid response received from CL-200A!";

                        replyReceived = true;
                        loop.quit(); // 收到期望的回复，退出事件循环
                    });

                    // 启动定时器
                    sendTimer.start();
                    timeoutTimer.start();

                    // 进入事件循环，等待信号或超时
                    showToast(info,"正在设置PC连接模式");
                    loop.exec();

                    // 停止定时器
                    sendTimer.stop();
                    timeoutTimer.stop();

                    if (replyReceived) {
                        qDebug() << "Expected reply received!";
                        disconnect(sendConnection);
                        disconnect(readConnection);
                    } else {
                        qDebug() << "Exited without receiving expected reply.";
                        showToast(error,"设置PC连接模式失败，请检查电缆是否连接正确");
                        m_a200OnlineCom->closeSerialPort();
                        return;
                    }

                    //等待500ms
                    timeoutTimer.setInterval(500);
                    timeoutTimer.start();
                    loop.exec();

                    /*------------------------------*/
                    /*Step 3 发送命令55 (设置为保持状态)*/
                    /*------------------------------*/

                    showToast(info,"正在设置保持状态");

                    QByteArray byteArray;
                    QDataStream stream(&byteArray, QIODevice::WriteOnly);

                    // 1. 添加 STX (0x02)
                    stream << (quint8)0x02;

                    // 2. 发送命令字符串 "99551  0"
                    QByteArray command = "99551  0";
                    stream.writeRawData(command.constData(), command.size());

                    // 3. 计算 BCC（从 "99" 开始到 ETX 之前）
                    quint8 bcc = 0;
                    for (char c : command) {
                        bcc ^= c;
                    }
                    bcc ^= 0x03; // 异或 ETX

                    // 4. 追加 ETX (0x03)
                    stream << (quint8)0x03;

                    // 5. 追加 BCC（以 ASCII 形式）
                    QString bccStr = QString("%1").arg(bcc, 2, 16, QChar('0')).toUpper(); // 转换为2位16进制大写字符串
                    stream.writeRawData(bccStr.toUtf8().constData(), bccStr.size());

                    // 6. 追加 CR + LF (0x0D, 0x0A)
                    stream << (quint8)0x0D << (quint8)0x0A;
                    m_a200OnlineCom->writeData(byteArray);
                    QStringList hexList;
                    for (const auto& byte : byteArray) {
                        // 使用QString::asprintf格式化每个字节为0x的十六进制形式
                        hexList.append(QString::asprintf("0x%02X", static_cast<unsigned char>(byte)));
                    }

                    // 使用qDebug()输出
                    qDebug() << "Sent command 55:"<< hexList.join(" ");

                    //命令55发送完毕后需要等待500ms
                    timeoutTimer.start();
                    loop.exec();

                    /*------------------------------*/
                    /*Step 4 发送命令40 (设置为EXT模式)*/
                    /*------------------------------*/

                    showToast(info,"正在设置EXT模式");

                    replyReceived = false;
                    timeoutTimer.setInterval(5000);

                    sendConnection = connect(&sendTimer, &QTimer::timeout, [&]() {
                        QByteArray byteArray;
                        QDataStream stream(&byteArray, QIODevice::WriteOnly);

                        // 1. 添加 STX (0x02)
                        stream << (quint8)0x02;

                        // 2. 发送命令字符串 "004010  "
                        QByteArray command = "004010  ";
                        stream.writeRawData(command.constData(), command.size());

                        // 3. 计算 BCC（从 "00" 开始到 ETX 之前）
                        quint8 bcc = 0;
                        for (char c : command) {
                            bcc ^= c;
                        }
                        bcc ^= 0x03; // 异或 ETX

                        // 4. 追加 ETX (0x03)
                        stream << (quint8)0x03;

                        // 5. 追加 BCC（以 ASCII 形式）
                        QString bccStr = QString("%1").arg(bcc, 2, 16, QChar('0')).toUpper(); // 转换为2位16进制大写字符串
                        stream.writeRawData(bccStr.toUtf8().constData(), bccStr.size());

                        // 6. 追加 CR + LF (0x0D, 0x0A)
                        stream << (quint8)0x0D << (quint8)0x0A;
                        m_a200OnlineCom->writeData(byteArray);
                        QStringList hexList;
                        for (const auto& byte : byteArray) {
                            // 使用QString::asprintf格式化每个字节为0x的十六进制形式
                            hexList.append(QString::asprintf("0x%02X", static_cast<unsigned char>(byte)));
                        }

                        // 使用qDebug()输出
                        qDebug() << "Sent command 40:"<< hexList.join(" ");
                    });

                    readConnection = connect(m_a200OnlineCom, &SerialCom::readReady, [&]() {
                        QByteArray receivedData = m_a200OnlineCom->readAll();
                        qDebug() << "Data received:" << receivedData;
                        if (receivedData.compare("...") == 0) {
                            replyReceived = true;
                            loop.quit(); // 收到期望的回复，退出事件循环
                        }
                    });

                    // 启动定时器
                    sendTimer.start();
                    timeoutTimer.start();

                    // 进入事件循环，等待信号或超时
                    showToast(info,"正在设置EXT模式");
                    loop.exec();

                    // 停止定时器
                    sendTimer.stop();
                    timeoutTimer.stop();

                    if (replyReceived) {
                        qDebug() << "Expected reply received!";
                        disconnect(sendConnection);
                        disconnect(readConnection);
                    } else {
                        qDebug() << "Exited without receiving expected reply.";
                        showToast(error,"设置EXT模式失败");
                        m_a200OnlineCom->closeSerialPort();
                        return;
                    }

                    m_a200OnlineTimer.start();
                } else {
                    m_a200OnlineTimer.stop();
                }
            });
    connect(m_a200OnlineCom,&SerialCom::readReady,this,[this](){

    });

    connect(&m_a200OnlineTimer,&QTimer::timeout,this,[this](){
        //只在初始化成功后用来测量
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

        stream.setByteOrder(QDataStream::LittleEndian);  // 设置为小端字节序
        stream << static_cast<qint32>(lightController->pwmHz());
        // frameInfo += QString("[PWM频率:%1] ").arg(QString::asprintf("0x%02X 0x%02X 0x%02X 0x%02X", (static_cast<qint32>(lightController->pwmHz()) >> 24) & 0xFF, (static_cast<qint32>(lightController->pwmHz()) >> 16) & 0xFF, (static_cast<qint32>(lightController->pwmHz()) >> 8) & 0xFF, static_cast<qint32>(lightController->pwmHz()) & 0xFF));
        frameInfo += QString("[PWM频率:%1] ").arg(QString::asprintf("0x%02X 0x%02X 0x%02X 0x%02X", static_cast<qint32>(lightController->pwmHz())  & 0xFF, (static_cast<qint32>(lightController->pwmHz()) >> 8) & 0xFF, (static_cast<qint32>(lightController->pwmHz()) >> 16) & 0xFF, (static_cast<qint32>(lightController->pwmHz())>> 24) & 0xFF));

        stream<<static_cast<qint8>(0x01);
        frameInfo += "[PWM开关:0x01] ";

        for(int i=0;i<19;i++)
        {
            stream << static_cast<qint8>(0);
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

void Protocol::sendCommand(int num)
{
    bool isMatched;
    QByteArray command;

    switch (num) {
    case 54:
        command = "00541   ";
        isMatched = true;
        break;
    case 55:
        command = "99551  0";
        isMatched = true;
        break;
    case 40:
        command = "004010  ";
        isMatched = true;
        break;
    default:
        isMatched = false;
        break;
    }

    if(isMatched){
        QByteArray byteArray;
        QDataStream stream(&byteArray, QIODevice::WriteOnly);

        // 1. 添加 STX (0x02)
        stream << (quint8)0x02;

        // 2. 发送命令字符串

        stream.writeRawData(command.constData(), command.size());

        // 3. 计算 BCC（从 "00" 开始到 ETX 之前）
        quint8 bcc = 0;
        for (char c : std::as_const(command)) {
            bcc ^= c;
        }
        bcc ^= 0x03; // 异或 ETX

        // 4. 追加 ETX (0x03)
        stream << (quint8)0x03;

        // 5. 追加 BCC（以 ASCII 形式）
        QString bccStr = QString("%1").arg(bcc, 2, 16, QChar('0')).toUpper(); // 转换为2位16进制大写字符串
        stream.writeRawData(bccStr.toUtf8().constData(), bccStr.size());

        // 6. 追加 CR + LF (0x0D, 0x0A)
        stream << (quint8)0x0D << (quint8)0x0A;
        m_a200OnlineCom->writeData(byteArray);
        QStringList hexList;
        for (const auto& byte : std::as_const(byteArray)) {
            // 使用QString::asprintf格式化每个字节为0x的十六进制形式
            hexList.append(QString::asprintf("0x%02X", static_cast<unsigned char>(byte)));
        }

        // 使用qDebug()输出
        qDebug() << "Sent command "<<num<<":"<< hexList.join(" ");
    }else{
        qDebug() << "No commands were sent,becasue no command is matched!";
    }




}
