#include "OnlineParam.h"
#include <QDebug>

OnlineParam::OnlineParam(QObject *parent)
    : QObject{parent}
{}

int OnlineParam::colorTemp() const
{
    return m_colorTemp;
}

void OnlineParam::setColorTemp(int newColorTemp)
{
    if (m_colorTemp == newColorTemp)
        return;
    m_colorTemp = newColorTemp;
    qDebug()<<m_colorTemp;
    emit colorTempChanged();
}

int OnlineParam::brightness() const
{
    return m_brightness;
}

void OnlineParam::setBrightness(int newBrightness)
{
    if (m_brightness == newBrightness)
        return;
    m_brightness = newBrightness;
    qDebug()<<m_brightness;
    emit brightnessChanged();
}

int OnlineParam::xColorCoord() const
{
    return m_xColorCoord;
}

void OnlineParam::setXColorCoord(int newXColorCoord)
{
    if (m_xColorCoord == newXColorCoord)
        return;
    m_xColorCoord = newXColorCoord;
    qDebug()<<m_xColorCoord;
    emit xColorCoordChanged();
}

int OnlineParam::yColorCoord() const
{
    return m_yColorCoord;
}

void OnlineParam::setYColorCoord(int newYColorCoord)
{
    if (m_yColorCoord == newYColorCoord)
        return;
    m_yColorCoord = newYColorCoord;
    qDebug()<<m_yColorCoord;
    emit yColorCoordChanged();
}

int OnlineParam::uvValue() const
{
    return m_uvValue;
}

void OnlineParam::setUvValue(int newUvValue)
{
    if (m_uvValue == newUvValue)
        return;
    m_uvValue = newUvValue;
    qDebug()<<m_uvValue;
    emit uvValueChanged();
}
