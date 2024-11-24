#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

#include <QObject>
#include <QList>
#include <QQmlListProperty>
#include <QJSEngine>
#include "ChannelSetter.h"
#include "OnlineParam.h"

class LightController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(OnlineParam* onlineParam READ onlineParam NOTIFY onlineParamChanged FINAL)
    Q_PROPERTY(QQmlListProperty<ChannelSetter> channelSetterList READ channelSetterList NOTIFY channelSetterListChanged FINAL)
public:
    static QObject* instance(QQmlEngine *engine = nullptr, QJSEngine *scriptEngine = nullptr);

    QQmlListProperty<ChannelSetter> channelSetterList();

    OnlineParam* onlineParam() const;

signals:
    void channelSetterListChanged();
    void onlineParamChanged();

private:
    explicit LightController(QObject *parent = nullptr);
    ~LightController();
    static void appendChannelSetter(QQmlListProperty<ChannelSetter> *list, ChannelSetter *channelSetter);
    static qsizetype countChannelSetters(QQmlListProperty<ChannelSetter> *list);
    static ChannelSetter* atChannelSetter(QQmlListProperty<ChannelSetter> *list, qsizetype index);
    static void clearChannelSetters(QQmlListProperty<ChannelSetter> *list);
    QList<ChannelSetter*> m_channelSetterList;
    OnlineParam *m_onlineParam;
};

#endif // LIGHTCONTROLLER_H
