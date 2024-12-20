#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTimer>
#include <QString>
#include <QStringList>

class SerialCom : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentPortName READ currentPortName WRITE setCurrentPortName NOTIFY currentPortNameChanged FINAL)
    Q_PROPERTY(qint32 baudRate READ baudRate WRITE setBaudRate NOTIFY baudRateChanged FINAL)
    Q_PROPERTY(QStringList portNameList READ portNameList NOTIFY portNameListChanged FINAL)
    Q_PROPERTY(bool isOpened READ isOpened WRITE setIsOpened NOTIFY isOpenedChanged FINAL)
public:
    explicit SerialCom(QObject *parent = nullptr);

    QStringList portNameList() const;

    QString currentPortName() const;
    void setCurrentPortName(const QString &newCurrentPortName);

    qint32 baudRate() const;
    void setBaudRate(qint32 newBaudRate);

    bool isOpened() const;

    void setIsOpened(bool newIsOpened);

public slots:

    Q_INVOKABLE void openSerialPort();
    Q_INVOKABLE void closeSerialPort();
    Q_INVOKABLE void updatePortNameList();
signals:

    void portNameListChanged();

    void currentPortNameChanged();

    void baudRateChanged();

    void isOpenedChanged();

private slots:


    void writeData(const QByteArray &data);
    void readData();

    void handleError(QSerialPort::SerialPortError error);
    void handleBytesWritten(qint64 bytes);
    void handleWriteTimeout();

private:
    QTimer *m_timer = nullptr;
    QSerialPort *m_serial = nullptr;
    QStringList m_portNameList;
    qint64 m_bytesToWrite = 0;
    QString m_currentPortName;
    qint32 m_baudRate;
    bool m_isOpened = false;
};

#endif // SERIALCOM_H
