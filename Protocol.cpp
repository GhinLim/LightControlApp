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

    });



}
