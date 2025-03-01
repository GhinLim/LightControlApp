#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <QTimer>
#include "SerialCom.h"
#include "OnlineParam.h"

class Protocol : public QObject
{
    Q_OBJECT
public:
    explicit Protocol(QObject *parent = nullptr);
    void init();
    void sendPwmHz();
    void sendCommand(int num);
signals:

private:
    SerialCom* m_pcOnlineCom = nullptr;
    SerialCom* m_a200OnlineCom = nullptr;
    OnlineParam* m_onlineParam = nullptr;
    QTimer m_pcOnlineTimer;
    QTimer m_a200OnlineTimer;
    QTimer m_pwmHzTimer;
};

#endif // PROTOCOL_H
