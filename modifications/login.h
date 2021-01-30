#ifndef LOGIN_H
#define LOGIN_H

#include "customerdata.h"
#include "QGCApplication.h"
#include "LinkManager.h"

class Login: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString otp READ otp WRITE setOTP NOTIFY otpChanged)
    Q_PROPERTY(QString passWord READ passWord WRITE setPassWord NOTIFY passWordChanged)
    Q_PROPERTY(bool okButton READ okButton WRITE checkDataBase NOTIFY verifyCredentials)
    Q_PROPERTY(bool otpButton READ otpButton WRITE checkOTP NOTIFY verifyOTP)
    Q_PROPERTY(QString droneNo READ droneNo WRITE setDroneNo NOTIFY droneNoChanged)

    Q_PROPERTY(bool logoutButton READ logoutButton WRITE custLogout NOTIFY logoutCustomer)
public:
    explicit Login(QObject *parent = nullptr);
    Q_INVOKABLE bool getLoginStatus(){return loginStatus;}
    Q_INVOKABLE bool getOTPStatus(){return OTPStatus;}

    QString userName();
    QString otp();
    QString passWord();
    QString droneNo(); //storing drone number

    bool okButton();
    bool otpButton();

    void setUserName(const QString &userName);
    void setOTP(const QString &otp);
    void setPassWord(const QString &passWord);
    void setDroneNo(const QString &droneNo); // writting droneNo value

    void checkDataBase(bool okButton);
    void checkOTP(bool otpButton);

    void custLogout(bool logoutButton);

signals:
    void userNameChanged();
    void otpChanged();
    void passWordChanged();
    void verifyCredentials();
    void verifyOTP();
    void droneNoChanged(); // notifying  when droneNo changed

    void loggedInStatusChange();
    void logoutCustomer();

private slots:
    void LoginSuccessful();
    void LoginFailed();

    void OTPSuccessful();
    void OTPFailed();

    void logoutSuccessful();
    void logoutFailed();

private:
    void changeLoginStatus(bool status){
        loginStatus = status;
        emit loggedInStatusChange();
    }
    void changeOTPStatus(bool status){
        OTPStatus = status;
        emit loggedInStatusChange();
    }
    bool loginStatus, OTPStatus;
    QString m_userName;
    QString m_passWord;
    QString m_otp;
    bool checkNow;
    bool m_otpButton;
    bool mutex;
    QString myURL;
    QString m_droneNo;//drone number value
};

#endif // LOGIN_H
