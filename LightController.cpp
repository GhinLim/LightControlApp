#include "LightController.h"
#include "OnlineParam.h"
#include "SerialCom.h"
#include "Protocol.h"
#include "ChannelSetter.h"
#include "Tools.hpp"

QObject *LightController::instance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    static LightController instance;
    return &instance;
}

LightController::LightController(QObject *parent)
    : QObject{parent}
{
    restoreInput(m_pwmHz,pwmHzKey);
    m_onlineParam = new OnlineParam(this);
    m_onlineParam->setBrightness(0);
    m_onlineParam->setColorTemp(0);
    m_onlineParam->setXColorCoord(0);
    m_onlineParam->setYColorCoord(0);
    m_onlineParam->setUvValue(0);

    for(int i=0;i<12;i++)
    {
        ChannelSetter *channelSetter = new ChannelSetter(i,this);
        connect(channelSetter,&ChannelSetter::updateOtherChannels,this,[=](int index,bool enabled){
            // qDebug()<<"来自channel["<<index<<"]:"<<enabled;
            ChannelSetter * channelSetter = m_channelSetterList[index]; //取得开关发生改变的channelSetter
            for(int i=0;i<12;i++)   //修改剩余的channnelSetter
            {
                if(i == index){
                    continue;
                }
                ChannelSetter* updatingChannelSetter = m_channelSetterList[i];  //取得需要修改的ChannelSetter

                for(int j= 0;j<12;j++)  //遍历ChannelSetter的pwmSetterList
                {
                    if(channelSetter->pwmSetterList().toList<QList<PwmSetter*>>().at(j)->isOpened()&&channelSetter->pwmSetterList().toList<QList<PwmSetter*>>().at(j)->enabled()){
                        updatingChannelSetter->pwmSetterList().toList<QList<PwmSetter*>>().at(j)->setEnabled(enabled);
                    }

                }
            }
        });
        m_channelSetterList.append(channelSetter);

        for(int j=0;j<12;j++)
        {
            PwmSetter* pwmSetter = channelSetter->pwmSetterList().toList<QList<PwmSetter*>>().at(j);
            connect(pwmSetter,&PwmSetter::updatePwmOfOtherChannels,this,[=](int channelIndex,int index,bool enabled){
                for(int i=0;i<12;i++)
                {
                    if(i == channelIndex){
                        continue;
                    }
                    ChannelSetter* updatingChannelSetter = m_channelSetterList[i];  //取得需要修改的ChannelSetter
                    updatingChannelSetter->pwmSetterList().toList<QList<PwmSetter*>>().at(index)->setEnabled(enabled);
                    // qDebug()<<"channel:"<<i<<" Pwm:"<<index<<" is set to "<<enabled;
                }
            });
        }
    }

    m_pcOnlineCom = new SerialCom("pcOnlineCom",this);
    m_a200OnlineCom = new SerialCom("200AOnlineCom",this);

    m_protocol = new Protocol(this);

    m_protocol->init();
}

LightController::~LightController()
{

}

QQmlListProperty<ChannelSetter> LightController::channelSetterList()
{
    return QQmlListProperty<ChannelSetter>(this,this,
                                           &LightController::appendChannelSetter,
                                           &LightController::countChannelSetters,
                                           &LightController::atChannelSetter,
                                           &LightController::clearChannelSetters);
}

void LightController::appendChannelSetter(QQmlListProperty<ChannelSetter> *list, ChannelSetter *channelSetter)
{
    LightController * lightController = qobject_cast<LightController*>(list->object);
    if(lightController)
    {
        channelSetter->setParent(lightController);
        lightController->m_channelSetterList.append(channelSetter);
        emit lightController->channelSetterListChanged();
    }
}

qsizetype LightController::countChannelSetters(QQmlListProperty<ChannelSetter> *list)
{
    LightController * lightController = qobject_cast<LightController*>(list->object);
    if(lightController)
    {
        return lightController->m_channelSetterList.count();
    }
    return 0;
}

ChannelSetter *LightController::atChannelSetter(QQmlListProperty<ChannelSetter> *list, qsizetype index)
{
    LightController * lightController = qobject_cast<LightController*>(list->object);
    if(lightController)
    {
        return lightController->m_channelSetterList.at(index);
    }
    return nullptr;
}

void LightController::clearChannelSetters(QQmlListProperty<ChannelSetter> *list)
{
    LightController * lightController = qobject_cast<LightController*>(list->object);
    if(lightController)
    {
        qDeleteAll(lightController->m_channelSetterList);
        lightController->m_channelSetterList.clear();
        emit lightController->channelSetterListChanged();
    }
}

OnlineParam* LightController::onlineParam() const
{
    return m_onlineParam;
}

SerialCom *LightController::pcOnlineCom() const
{
    return m_pcOnlineCom;
}

SerialCom *LightController::a200OnlineCom() const
{
    return m_a200OnlineCom;
}

void LightController::sendPwmHz()
{
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);

    stream<<static_cast<qint8>(0xFA);
    stream<<static_cast<qint8>(0x1B);

    QString frameInfo;
    frameInfo += "PWM频率设定数据帧结构：[帧头:0xFA] ";
    frameInfo += "[帧长度:0x1B] ";

    stream << static_cast<qint32>(m_pwmHz);
    frameInfo += QString("[PWM频率:%1] ").arg(QString::asprintf("0x%02X 0x%02X 0x%02X 0x%02X", (static_cast<qint32>(m_pwmHz) >> 24) & 0xFF, (static_cast<qint32>(m_pwmHz) >> 16) & 0xFF, (static_cast<qint32>(m_pwmHz) >> 8) & 0xFF, static_cast<qint32>(m_pwmHz) & 0xFF));

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
}

int LightController::pwmHz() const
{
    return m_pwmHz;
}

void LightController::setPwmHz(int newPwmHz)
{
    if (m_pwmHz == newPwmHz)
        return;
    m_pwmHz = newPwmHz;
    saveInput(m_pwmHz,pwmHzKey);
    qDebug()<<"'pwmHz' is set:"<<m_pwmHz;
    emit pwmHzChanged();
}
