#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

#include <QObject>
#include <QList>
#include <QQmlListProperty>
#include <QJSEngine>
#include "ChannelSetter.h"
#include "OnlineParam.h"
#include "SerialCom.h"
#include "Protocol.h"

class LightController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(OnlineParam* onlineParam READ onlineParam NOTIFY onlineParamChanged FINAL)
    Q_PROPERTY(QQmlListProperty<ChannelSetter> channelSetterList READ channelSetterList NOTIFY channelSetterListChanged FINAL)
    Q_PROPERTY(SerialCom* pcOnlineCom READ pcOnlineCom NOTIFY pcOnlineComChanged FINAL)
    Q_PROPERTY(SerialCom* a200OnlineCom READ a200OnlineCom NOTIFY a200OnlineComChanged FINAL)
    Q_PROPERTY(int pwmHz READ pwmHz WRITE setPwmHz NOTIFY pwmHzChanged FINAL)
public:
    static QObject* instance(QQmlEngine *engine = nullptr, QJSEngine *scriptEngine = nullptr);

    QQmlListProperty<ChannelSetter> channelSetterList();

    OnlineParam* onlineParam() const;

    SerialCom *pcOnlineCom() const;

    SerialCom *a200OnlineCom() const;

    Q_INVOKABLE void sendPwmHz();

    int pwmHz() const;
    void setPwmHz(int newPwmHz);


signals:
    void channelSetterListChanged();
    void onlineParamChanged();

    void pcOnlineComChanged();

    void a200OnlineComChanged();

    void pwmHzChanged();

private:
    explicit LightController(QObject *parent = nullptr);
    ~LightController();
    static void appendChannelSetter(QQmlListProperty<ChannelSetter> *list, ChannelSetter *channelSetter);
    static qsizetype countChannelSetters(QQmlListProperty<ChannelSetter> *list);
    static ChannelSetter* atChannelSetter(QQmlListProperty<ChannelSetter> *list, qsizetype index);
    static void clearChannelSetters(QQmlListProperty<ChannelSetter> *list);

    int m_pwmHz;
    QString pwmHzKey = "pwmHzKey";

    //一级模块
    QList<ChannelSetter*> m_channelSetterList;
    OnlineParam *m_onlineParam = nullptr;
    SerialCom *m_pcOnlineCom = nullptr;
    SerialCom *m_a200OnlineCom = nullptr;

    //二级模块(依赖一级模块)

    //三级模块 (依赖一、二级模块)
    Protocol * m_protocol = nullptr;

};

#endif // LIGHTCONTROLLER_H
