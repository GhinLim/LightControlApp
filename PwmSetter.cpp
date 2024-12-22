#include "PwmSetter.h"
#include "Tools.hpp"
#include "ChannelSetter.h"
PwmSetter::PwmSetter(int channelIndex, int index, QObject *parent)
    : QObject{parent},
    m_channelIndex(channelIndex),
    m_index(index)
{
    isOpenedKey += QString::number(channelIndex) + "_" + QString::number(index);
    valueKey += QString::number(channelIndex) + "_" + QString::number(index);
    enabledKey += QString::number(channelIndex) + "_" + QString::number(index);

    restoreInput(m_isOpened,isOpenedKey);
    restoreInput(m_value,valueKey);
    restoreInput(m_enabled,enabledKey,true);
}

bool PwmSetter::isOpened() const
{
    return m_isOpened;
}

void PwmSetter::setIsOpened(bool newIsOpened)
{
    if (m_isOpened == newIsOpened)
        return;
    m_isOpened = newIsOpened;
    saveInput(m_isOpened,isOpenedKey);
    emit isOpenedChanged();

    ChannelSetter* channelSetter = qobject_cast<ChannelSetter*>(parent());
    if(channelSetter->isOpened())
    {
        emit updatePwmOfOtherChannels(m_channelIndex,m_index,!m_isOpened);
    }
}

int PwmSetter::value() const
{
    return m_value;
}

void PwmSetter::setValue(int newValue)
{
    if (m_value == newValue)
        return;
    m_value = newValue;
    saveInput(m_value,valueKey);
    emit valueChanged();
}

bool PwmSetter::enabled() const
{
    return m_enabled;
}

void PwmSetter::setEnabled(bool newEnabled)
{
    if (m_enabled == newEnabled)
        return;
    m_enabled = newEnabled;
    saveInput(m_enabled,enabledKey);
    emit enabledChanged();

    if(!m_enabled)
    {
        setIsOpened(false);
    }
}
