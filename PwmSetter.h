#ifndef PWMSETTER_H
#define PWMSETTER_H

#include <QObject>

class PwmSetter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isOpened READ isOpened WRITE setIsOpened NOTIFY isOpenedChanged FINAL)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged FINAL)
public:
    explicit PwmSetter(QObject *parent = nullptr);

    bool isOpened() const;
    void setIsOpened(bool newIsOpened);

    int value() const;
    void setValue(int newValue);

signals:
    void isOpenedChanged();
    void valueChanged();

private:
    bool m_isOpened;
    int m_value;
};

#endif // PWMSETTER_H
