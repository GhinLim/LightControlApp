#ifndef CHANNELSETTER_H
#define CHANNELSETTER_H

#include <QObject>
#include <QQmlListProperty>
#include <QList>
#include "PwmSetter.h"

class ChannelSetter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isOpened READ isOpened WRITE setIsOpened NOTIFY isOpenedChanged FINAL)
    Q_PROPERTY(int colorTemp READ colorTemp WRITE setColorTemp NOTIFY colorTempChanged FINAL)
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged FINAL)
    Q_PROPERTY(int totalBright READ totalBright WRITE setTotalBright NOTIFY totalBrightChanged FINAL)
    Q_PROPERTY(QQmlListProperty<PwmSetter> pwmSetterList READ pwmSetterList NOTIFY pwmSetterListChanged FINAL)
public:
    explicit ChannelSetter(int index, QObject *parent = nullptr);
    QQmlListProperty<PwmSetter> pwmSetterList();
    bool isOpened() const;
    void setIsOpened(bool newIsOpened);

    int colorTemp() const;
    void setColorTemp(int newColorTemp);

    int brightness() const;
    void setBrightness(int newBrightness);

    int totalBright() const;
    void setTotalBright(int newTotalBright);

signals:
    void pwmSetterListChanged();

    void isOpenedChanged();

    void colorTempChanged();

    void brightnessChanged();

    void totalBrightChanged();

    void updateOtherChannels(int index,bool enabled);

private:
    static void appendPwmSetter(QQmlListProperty<PwmSetter> *list, PwmSetter *pwmSetter);
    static qsizetype countPwmSetters(QQmlListProperty<PwmSetter>* list);
    static PwmSetter *atPwmSetter(QQmlListProperty<PwmSetter> *list ,qsizetype index);
    static void clearPwmSetters(QQmlListProperty<PwmSetter> *list);
    QList<PwmSetter*> m_pwmSetterList;
    bool m_isOpened = false;
    int m_colorTemp = 0;
    int m_brightness = 0;
    int m_totalBright = 0;
    int m_index;

    QString isOpenedKey = "channel_isOpened_";
    QString colorTempKey = "channel_colorTemp_";
    QString brightnessKey = "channel_brightness_";
    QString totalBrightKey = "channel_totalBright_";
};

#endif // CHANNELSETTER_H
