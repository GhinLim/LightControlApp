#include "PwmSetter.h"

PwmSetter::PwmSetter(QObject *parent)
    : QObject{parent}
{
    m_enabled = true;
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
    emit isOpenedChanged();
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
    emit enabledChanged();
}
