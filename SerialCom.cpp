#include "SerialCom.h"
#include <QDebug>
#include <chrono>

static constexpr std::chrono::seconds kWriteTimeout = std::chrono::seconds{5};

SerialCom::SerialCom(QString name, QObject *parent)
    : QObject{parent},
    m_name(name),
    m_serial(new QSerialPort(this)),
    m_timer(new QTimer(this))
{
    m_baudRate = QSerialPort::Baud115200;
    updatePortNameList();
    connect(m_serial, &QSerialPort::errorOccurred, this, &SerialCom::handleError);
    connect(m_timer, &QTimer::timeout, this, &SerialCom::handleWriteTimeout);
    m_timer->setSingleShot(true);

    connect(m_serial, &QSerialPort::readyRead, this, &SerialCom::readData);
    connect(m_serial, &QSerialPort::bytesWritten, this, &SerialCom::handleBytesWritten);
}

void SerialCom::openSerialPort()
{
    m_serial->setPortName(m_currentPortName);
    m_serial->setBaudRate(m_baudRate);

    if (m_serial->open(QIODevice::ReadWrite)) {
        qDebug()<<m_name<<":open succeed!";
        setIsOpened(true);
    } else {
        setIsOpened(false);
        qDebug()<<m_name<<":"<<m_serial->errorString();
    }
}

void SerialCom::closeSerialPort()
{
    if (m_serial->isOpen())
    {
        m_serial->close();
        qDebug()<<m_name<<":close!";
        setIsOpened(false);
    }else{
        qDebug()<<m_name<<":"<<m_serial->errorString();
    }
}

void SerialCom::updatePortNameList()
{
    m_portNameList.clear();
    const auto infos = QSerialPortInfo::availablePorts();

    for(const QSerialPortInfo &info : infos){
        m_portNameList<<info.portName();
    }

    qDebug()<<m_name<<":availablePorts:"<<m_portNameList;
    emit portNameListChanged();
}

void SerialCom::writeData(const QByteArray &data)
{
    const qint64 written = m_serial->write(data);
    if (written == data.size()) {
        m_bytesToWrite += written;
        m_timer->start(kWriteTimeout);
    } else {
        const QString error = tr(":Failed to write all data to port %1.\n"
                                 "Error: %2").arg(m_serial->portName(),
                                       m_serial->errorString());
        qDebug()<<m_name<<error;
    }
}

void SerialCom::readData()
{
    const QByteArray data = m_serial->readAll();
    qDebug()<<m_name<<"recv:"<<data;
}

void SerialCom::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qDebug()<<m_name<<":"<<m_serial->errorString();
        closeSerialPort();
        updatePortNameList();
    }
}

void SerialCom::handleBytesWritten(qint64 bytes)
{
    m_bytesToWrite -= bytes;
    if (m_bytesToWrite == 0)
        m_timer->stop();
}

void SerialCom::handleWriteTimeout()
{
    const QString error = tr("Write operation timed out for port %1.\n"
                             "Error: %2").arg(m_serial->portName(),
                                   m_serial->errorString());
    // showWriteError(error);
    qDebug()<<m_name<<":"<<error;
}


QStringList SerialCom::portNameList() const
{
    return m_portNameList;
}

QString SerialCom::currentPortName() const
{
    return m_currentPortName;
}

void SerialCom::setCurrentPortName(const QString &newCurrentPortName)
{
    if (m_currentPortName == newCurrentPortName)
        return;
    m_currentPortName = newCurrentPortName;
    qDebug()<<m_name<<" current port name:"<<m_currentPortName;
    emit currentPortNameChanged();
}

qint32 SerialCom::baudRate() const
{
    return m_baudRate;
}

void SerialCom::setBaudRate(qint32 newBaudRate)
{
    if (m_baudRate == newBaudRate)
        return;
    m_baudRate = newBaudRate;
    emit baudRateChanged();
}

bool SerialCom::isOpened() const
{
    return m_isOpened;
}

void SerialCom::setIsOpened(bool newIsOpened)
{
    if (m_isOpened == newIsOpened)
        return;
    m_isOpened = newIsOpened;
    emit isOpenedChanged();
}
