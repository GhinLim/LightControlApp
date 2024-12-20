#include "ChannelSetter.h"

ChannelSetter::ChannelSetter(int index, QObject *parent)
    : QObject{parent},
    m_index(index)
{
    for(int i=0;i<12;i++)
    {
        PwmSetter* pwmSetter = new PwmSetter(this);
        pwmSetter->setValue((i+1)*1000);
        // pwmSetter->setIsOpened(i%2);
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
    emit totalBrightChanged();
}
