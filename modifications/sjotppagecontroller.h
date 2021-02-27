#ifndef SJOTPPAGECONTROLLER_H
#define SJOTPPAGECONTROLLER_H

#include <QObject>
#include "customerdata.h"
#include "QGCApplication.h"
#include "LinkManager.h"
#include <QMessageBox>

class SJOTPPageController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString otp READ otp WRITE setOTP NOTIFY otpChanged)
    Q_PROPERTY(bool otpButton READ otpButton WRITE checkOTP NOTIFY verifyOTP)
public:
    explicit SJOTPPageController(QObject *parent = nullptr);
    Q_INVOKABLE bool getOTPStatus() {return OTPStatus;}

    QString otp();
    bool otpButton();
    void setOTP(const QString otp);
    void checkOTP(bool otpButton);

signals:
    void otpChanged();
    void verifyOTP();
    void otpSuccessfull();
    void loggedInStatusChange();

private slots:
    void OTPSuccessful();
    void OTPFailed(QString error);
    int ErrorMessageBox(QString errorMessage);
private:
    void changeOTPStatus(bool status);
    bool OTPStatus;
    QString m_otp;
    bool m_otpButton;
    QString myURL;
};

#endif // SJOTPPAGECONTROLLER_H
