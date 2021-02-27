#ifndef SJLOGINPAGECONTROLLER_H
#define SJLOGINPAGECONTROLLER_H

#include <QObject>
#include "customerdata.h"
#include "QGCApplication.h"
#include "LinkManager.h"
#include <QMessageBox>

class SJLoginPageController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString passWord READ passWord WRITE setPassWord NOTIFY passWordChanged)
    Q_PROPERTY(bool okButton READ okButton WRITE checkDataBase NOTIFY verifyCredentials)

public:
    explicit SJLoginPageController(QObject *parent = nullptr);
    Q_INVOKABLE bool getLoginStatus(){return loginStatus;}

    QString userName();
    QString passWord();

    bool okButton();

    void setUserName(const QString &userName);
    void setPassWord(const QString &passWord);

    void checkDataBase(bool okButton);


signals:
    void userNameChanged();
    void passWordChanged();
    void verifyCredentials();
    void loggedInStatusChange();
    void loginSuccessfull();

private slots:
    void LoginSuccessful();
    void LoginFailed(QString error);

private:
    int ErrorMessageBox(QString errorMessage);
    void changeLoginStatus(bool status){
        loginStatus = status;
        emit loggedInStatusChange();
    }
    QTimer *clk;
    bool loginStatus;
    QString m_userName;
    QString m_passWord;
    bool checkNow;
    bool mutex;
    QString myURL;
};

#endif // SJLOGINPAGECONTROLLER_H
