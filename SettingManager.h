#ifndef SETTINGMANAGER_H
#define SETTINGMANAGER_H

#include <QObject>
#include <QStringList>

class SettingManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ControlMode controlMode READ controlMode WRITE setControlMode NOTIFY controlModeChanged FINAL)
    Q_PROPERTY(QStringList controlModeList READ controlModeList NOTIFY controlModeListChanged FINAL)
public:
    enum ControlMode {
        singleChannel,
        multichannel
    };
    Q_ENUM(ControlMode)
    explicit SettingManager(QObject *parent = nullptr);
    void init();
    ControlMode controlMode() const;
    void setControlMode(ControlMode newControlMode);

    QStringList controlModeList() const;

signals:
    void controlModeChanged();
    void controlModeListChanged();

private:
    ControlMode m_controlMode;
    QStringList m_controlModeList;
    QString controlModeKey = "controlMode";
};

#endif // SETTINGMANAGER_H
