#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>

class SerialCom : public QObject
{
    Q_OBJECT
public:
    explicit SerialCom(QObject *parent = nullptr);

signals:

private slots:
    void openSerialPort();
    void closeSerialPort();

    void writeData(const QByteArray &data);
    void readData();

    void handleError(QSerialPort::SerialPortError error);
    void handleBytesWritten(qint64 bytes);
    void handleWriteTimeout();
};

#endif // SERIALCOM_H
