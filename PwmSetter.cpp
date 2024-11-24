#include "PwmSetter.h"

PwmSetter::PwmSetter(QObject *parent)
    : QObject{parent}
{}

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
