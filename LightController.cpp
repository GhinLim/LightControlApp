#include "LightController.h"
#include "Tools.hpp"

QObject *LightController::instance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    static LightController instance;
    return &instance;
}

void LightController::init()
{
    for(int i=0;i<12;i++)
    {
        ChannelSetter *channelSetter = new ChannelSetter(i,this);
        connect(channelSetter,&ChannelSetter::updateOtherChannels,this,[=](int index,bool enabled){
            if(settingManager()->controlMode() == SettingManager::singleChannel){
                if(!enabled){
                    for(int i=0;i<m_channelSetterList.count();i++){
                        if(i == index){
                            continue;
                        }
                        ChannelSetter* updatingChannelSetter = m_channelSetterList[i];
                        updatingChannelSetter->setIsOpened(enabled);
                    }
                }

            }else if(settingManager()->controlMode() == SettingManager::multichannel){
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
                            if(!enabled){
                                updatingChannelSetter->pwmSetterList().toList<QList<PwmSetter*>>().at(j)->setValue(channelSetter->pwmSetterList().toList<QList<PwmSetter*>>().at(j)->value());
                            }
                        }

                    }
                }
            }

        });
        m_channelSetterList.append(channelSetter);

        for(int j=0;j<12;j++)
        {
            PwmSetter* pwmSetter = channelSetter->pwmSetterList().toList<QList<PwmSetter*>>().at(j);
            connect(pwmSetter,&PwmSetter::updatePwmOfOtherChannels,this,[=](int channelIndex,int index,bool enabled){
                if(m_settingManager->controlMode() == SettingManager::multichannel){
                    for(int i=0;i<12;i++)
                    {
                        if(i == channelIndex){
                            continue;
                        }
                        ChannelSetter* updatingChannelSetter = m_channelSetterList[i];  //取得需要修改的ChannelSetter
                        updatingChannelSetter->pwmSetterList().toList<QList<PwmSetter*>>().at(index)->setEnabled(enabled);
                        // qDebug()<<"channel:"<<i<<" Pwm:"<<index<<" is set to "<<enabled;
                        if(!enabled){
                            updatingChannelSetter->pwmSetterList().toList<QList<PwmSetter*>>().at(index)->setValue(pwmSetter->value());
                        }
                    }
                }
            });

            connect(pwmSetter,&PwmSetter::valueChanged,this,[=](int channelIndex,int index){
                if(m_settingManager->controlMode() == SettingManager::multichannel){
                    if(pwmSetter->isOpened() && channelSetter->isOpened()){
                        for(int i=0;i<12;i++){
                            if(i == channelIndex){
                                continue;
                            }
                            ChannelSetter* updatingChannelSetter = m_channelSetterList[i];  //取得需要修改的ChannelSetter
                            updatingChannelSetter->pwmSetterList().toList<QList<PwmSetter*>>().at(index)->setValue(pwmSetter->value());
                        }
                    }
                }
            });
        }
    }

    m_protocol->init();
    m_settingManager->init();
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

    m_settingManager = new SettingManager(this);
    m_pcOnlineCom = new SerialCom("pcOnlineCom",this);
    m_a200OnlineCom = new SerialCom("200AOnlineCom",this);
    m_protocol = new Protocol(this);
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
    m_protocol->sendPwmHz();
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

SettingManager *LightController::settingManager() const
{
    return m_settingManager;
}
