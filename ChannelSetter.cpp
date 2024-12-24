#include "ChannelSetter.h"
#include "Tools.hpp"

ChannelSetter::ChannelSetter(int index, QObject *parent)
    : QObject{parent},
    m_index(index)
{
    isOpenedKey += QString::number(index);
    colorTempKey += QString::number(index);
    brightnessKey += QString::number(index);
    totalBrightKey += QString::number(index);

    restoreInput(m_isOpened,isOpenedKey);
    restoreInput(m_colorTemp,colorTempKey);
    restoreInput(m_brightness,brightnessKey);
    restoreInput(m_totalBright,totalBrightKey);

    for(int i=0;i<12;i++)
    {
        PwmSetter* pwmSetter = new PwmSetter(m_index,i,this);
        connect(pwmSetter,&PwmSetter::maxDetect,this,[this](){
            int maxValue = 0;
            for(int i=0;i<12;i++)
            {
                if(m_pwmSetterList.at(i)){
                    if(m_pwmSetterList.at(i)->enabled() && m_pwmSetterList.at(i)->isOpened() && m_pwmSetterList.at(i)->value()>maxValue)
                    {
                        maxValue = m_pwmSetterList.at(i)->value();
                    }
                }
            }
            setTotalBright(maxValue);
        });
        m_pwmSetterList.append(pwmSetter);
    }
}

QQmlListProperty<PwmSetter> ChannelSetter::pwmSetterList()
{
    return QQmlListProperty<PwmSetter>(this,this,
                                       &ChannelSetter::appendPwmSetter,
                                       &ChannelSetter::countPwmSetters,
                                       &ChannelSetter::atPwmSetter,
                                       &ChannelSetter::clearPwmSetters);
}

void ChannelSetter::appendPwmSetter(QQmlListProperty<PwmSetter> *list, PwmSetter *pwmSetter)
{
    ChannelSetter* channelSetter = qobject_cast<ChannelSetter*>(list->object);
    if(channelSetter)
    {
        pwmSetter->setParent(channelSetter);
        channelSetter->m_pwmSetterList.append(pwmSetter);
        emit channelSetter->pwmSetterListChanged();
    }
}

qsizetype ChannelSetter::countPwmSetters(QQmlListProperty<PwmSetter> *list)
{
    ChannelSetter* channelSetter = qobject_cast<ChannelSetter*>(list->object);
    if(channelSetter)
    {
        return channelSetter->m_pwmSetterList.count();
    }
    return 0;
}

PwmSetter *ChannelSetter::atPwmSetter(QQmlListProperty<PwmSetter> *list, qsizetype index)
{
    ChannelSetter* channelSetter = qobject_cast<ChannelSetter*>(list->object);
    if(channelSetter)
    {
        return channelSetter->m_pwmSetterList.at(index);
    }
    return nullptr;
}

void ChannelSetter::clearPwmSetters(QQmlListProperty<PwmSetter> *list)
{
    ChannelSetter* channelSetter = qobject_cast<ChannelSetter*>(list->object);
    if(channelSetter)
    {
        qDeleteAll(channelSetter->m_pwmSetterList);
        channelSetter->m_pwmSetterList.clear();
        emit channelSetter->pwmSetterListChanged();
    }

}

bool ChannelSetter::isOpened() const
{
    return m_isOpened;
}

void ChannelSetter::setIsOpened(bool newIsOpened)
{
    if (m_isOpened == newIsOpened)
        return;
    m_isOpened = newIsOpened;
    qDebug()<<"channel"<<m_index+1<<" 's 'isOpened' is set:"<<m_isOpened;
    saveInput(m_isOpened,isOpenedKey);
    emit isOpenedChanged();
    emit updateOtherChannels(m_index,!m_isOpened);
}

int ChannelSetter::colorTemp() const
{
    return m_colorTemp;
}

void ChannelSetter::setColorTemp(int newColorTemp)
{
    if (m_colorTemp == newColorTemp)
        return;
    m_colorTemp = newColorTemp;
    qDebug()<<"channel"<<m_index+1<<" 's 'colorTemp' is set:"<<m_colorTemp;
    saveInput(m_colorTemp,colorTempKey);
    emit colorTempChanged();
}

int ChannelSetter::brightness() const
{
    return m_brightness;
}

void ChannelSetter::setBrightness(int newBrightness)
{
    if (m_brightness == newBrightness)
        return;
    m_brightness = newBrightness;
    qDebug()<<"channel"<<m_index+1<<" 's 'brightness' is set:"<<m_brightness;
    saveInput(m_brightness,brightnessKey);
    emit brightnessChanged();
}

int ChannelSetter::totalBright() const
{
    return m_totalBright;
}

void ChannelSetter::setTotalBright(int newTotalBright)
{
    if (m_totalBright == newTotalBright)
        return;
    m_totalBright = newTotalBright;
    qDebug()<<"channel"<<m_index+1<<" 's 'totalBright' is set:"<<m_totalBright;
    saveInput(m_totalBright,totalBrightKey);
    emit totalBrightChanged();
}
