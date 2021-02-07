#ifndef SJ_NPNTCONTROL_H
#define SJ_NPNTCONTROL_H

#include <QObject>
#include "sjfirmwarecontrol.h"
#include "sjkeyrotation.h"
#include "QGCApplication.h"
#include "customerdata.h"

class SJ_NPNTControl : public QObject
{
    Q_OBJECT
public:
    explicit SJ_NPNTControl(QObject *parent = nullptr);
signals:
    void check1();
    void check2();
    void check3();
    void check4();

public slots:
    void boardIsRegistered();
    void boardNotRegistered();
    Q_INVOKABLE bool deviceConnected();
private slots:

    bool boardRegistered();
    bool firmwareCheck();
    bool keyRotated();

    void firmwareOK();
    void firmwareUpgradeRequired();

    void keyRotatedOK();
    void KeyRotateFailed();


private:
    SJFirmwareControl* m_firmwareController;
    SJKeyRotation* m_keyController;

    QTimer *timer1, *timer2, *timer3, *timer4;
    CustomerData* cust;
    QString m_url;
};

#endif // SJ_NPNTCONTROL_H
