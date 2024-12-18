#include "LightController.h"

QObject *LightController::instance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    static LightController instance;
    return &instance;
}

LightController::LightController(QObject *parent)
    : QObject{parent}
{
    m_onlineParam = new OnlineParam(this);
    m_onlineParam->setColorTemp(1000);
    m_onlineParam->setBrightness(2000);
    m_onlineParam->setXColorCoord(3000);
    m_onlineParam->setYColorCoord(4000);
    m_onlineParam->setUvValue(5000);

    for(int i=0;i<12;i++)
    {
        ChannelSetter *channelSetter = new ChannelSetter(this);
        channelSetter->setTotalBright((i+1)*1000);
        m_channelSetterList.append(channelSetter);

    }
}

LightController::~LightController()
{

}

QQmlListProperty<ChannelSetter> LightController::channelSetterList()
{
    return QQmlListProperty<ChannelSetter>(this,this,
                                           &LightController::appendChannelSetter,
                                           &LightController::countChannelSetters,
                                           &LightController::atChannelSetter,
                                           &LightController::clearChannelSetters);
}

void LightController::appendChannelSetter(QQmlListProperty<ChannelSetter> *list, ChannelSetter *channelSetter)
{
    LightController * lightController = qobject_cast<LightController*>(list->object);
    if(lightController)
    {
        channelSetter->setParent(lightController);
        lightController->m_channelSetterList.append(channelSetter);
        emit lightController->channelSetterListChanged();
    }
}

qsizetype LightController::countChannelSetters(QQmlListProperty<ChannelSetter> *list)
{
    LightController * lightController = qobject_cast<LightController*>(list->object);
    if(lightController)
    {
        return lightController->m_channelSetterList.count();
    }
    return 0;
}

ChannelSetter *LightController::atChannelSetter(QQmlListProperty<ChannelSetter> *list, qsizetype index)
{
    LightController * lightController = qobject_cast<LightController*>(list->object);
    if(lightController)
    {
        return lightController->m_channelSetterList.at(index);
    }
    return nullptr;
}

void LightController::clearChannelSetters(QQmlListProperty<ChannelSetter> *list)
{
    LightController * lightController = qobject_cast<LightController*>(list->object);
    if(lightController)
    {
        qDeleteAll(lightController->m_channelSetterList);
        lightController->m_channelSetterList.clear();
        emit lightController->channelSetterListChanged();
    }
}

OnlineParam* LightController::onlineParam() const
{
    return m_onlineParam;
}
