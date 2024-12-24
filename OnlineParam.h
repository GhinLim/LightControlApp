#ifndef ONLINEPARAM_H
#define ONLINEPARAM_H

#include <QObject>

class OnlineParam : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int colorTemp READ colorTemp WRITE setColorTemp NOTIFY colorTempChanged FINAL)
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged FINAL)
    Q_PROPERTY(int xColorCoord READ xColorCoord WRITE setXColorCoord NOTIFY xColorCoordChanged FINAL)
    Q_PROPERTY(int yColorCoord READ yColorCoord WRITE setYColorCoord NOTIFY yColorCoordChanged FINAL)
    Q_PROPERTY(int uvValue READ uvValue WRITE setUvValue NOTIFY uvValueChanged FINAL)
public:
    explicit OnlineParam(QObject *parent = nullptr);

    int colorTemp() const;
    void setColorTemp(int newColorTemp);

    int brightness() const;
    void setBrightness(int newBrightness);

    int xColorCoord() const;
    void setXColorCoord(int newXColorCoord);

    int yColorCoord() const;
    void setYColorCoord(int newYColorCoord);

    int uvValue() const;
    void setUvValue(int newUvValue);

signals:
    void colorTempChanged();

    void brightnessChanged();

    void xColorCoordChanged();

    void yColorCoordChanged();

    void uvValueChanged();

private:
    int m_colorTemp;
    int m_brightness;
    int m_xColorCoord;
    int m_yColorCoord;
    int m_uvValue;
};

#endif // ONLINEPARAM_H
