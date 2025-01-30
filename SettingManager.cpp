#include "SettingManager.h"
#include <QMetaEnum>
#include "Tools.hpp"
#include "LightController.h"

SettingManager::SettingManager(QObject *parent)
    : QObject{parent}
{

}

void SettingManager::init()
{
    // const QMetaObject metaObject = SettingManager::staticMetaObject;
    // QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("ControlMode"));
    // for (int i = 0; i < metaEnum.keyCount(); ++i) {
    //     m_controlModeList << QString(metaEnum.key(i));
    // }
    m_controlModeList << QString("Single-CH");
    m_controlModeList << QString("Multi-CH");
    emit controlModeListChanged();

    connect(this,SettingManager::controlModeChanged,this,[this](){
        LightController *lightController = qobject_cast<LightController*>(parent());
        if(lightController != nullptr){
            QList<ChannelSetter*> channelSetterList = lightController->channelSetterList().toList<QList<ChannelSetter*>>();
            if(m_controlMode == singleChannel){
                bool isSet = false;
                for(int i=0;i<channelSetterList.count();i++){
                    if(!isSet && channelSetterList.at(i)->isOpened()){
                        channelSetterList.at(i)->setIsOpened(false);
                        channelSetterList.at(i)->setIsOpened(true);
                        isSet = true;
                    }
                    QList<PwmSetter*> pwmSetterList = channelSetterList.at(i)->pwmSetterList().toList<QList<PwmSetter*>>();
                    for(int j=0;j<pwmSetterList.count();j++){
                        pwmSetterList.at(j)->setEnabled(true);
                    }
                }
            }else if(m_controlMode == multichannel){
                for(int i=0;i<channelSetterList.count();i++){
                    if(channelSetterList.at(i)->isOpened()){
                        channelSetterList.at(i)->setIsOpened(false);
                        channelSetterList.at(i)->setIsOpened(true);
                        break;
                    }
                }
            }
        }
    });
}

SettingManager::ControlMode SettingManager::controlMode() const
{
    return m_controlMode;
}

void SettingManager::setControlMode(ControlMode newControlMode)
{
    if (m_controlMode == newControlMode)
        return;
    m_controlMode = newControlMode;
    qDebug()<<"m_controlMode="<<m_controlMode;
    showToast(success,"666");
    emit controlModeChanged();
}

QStringList SettingManager::controlModeList() const
{
    return m_controlModeList;
}
