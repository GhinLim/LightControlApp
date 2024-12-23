#ifndef PWMSETTER_H
#define PWMSETTER_H

#include <QObject>
#include <QString>

class PwmSetter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(bool isOpened READ isOpened WRITE setIsOpened NOTIFY isOpenedChanged FINAL)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged FINAL)
public:
    explicit PwmSetter(int channelIndex, int index, QObject *parent = nullptr);

    bool isOpened() const;
    void setIsOpened(bool newIsOpened);

    int value() const;
    void setValue(int newValue);

    bool enabled() const;
    void setEnabled(bool newEnabled);

signals:
    void isOpenedChanged();
    void valueChanged();
    void enabledChanged();
    void maxDetect();
    void updatePwmOfOtherChannels(int channelIndex, int index, bool enabled);

private:
    bool m_isOpened = false;
    int m_value = 0;
    bool m_enabled = true;
    int m_channelIndex;
    int m_index;
    QString isOpenedKey = "PwmSetter_isOpened_";
    QString valueKey = "PwmSetter_value_";
    QString enabledKey = "PwmSetter_enabled_";
};

#endif // PWMSETTER_H
