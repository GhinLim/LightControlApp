#include "SerialCom.h"

SerialCom::SerialCom(QObject *parent)
    : QObject{parent}
{}

void SerialCom::openSerialPort()
{

}

void SerialCom::closeSerialPort()
{

}

void SerialCom::writeData(const QByteArray &data)
{

}

void SerialCom::readData()
{

}

void SerialCom::handleError(QSerialPort::SerialPortError error)
{

}

void SerialCom::handleBytesWritten(qint64 bytes)
{

}

void SerialCom::handleWriteTimeout()
{

}
