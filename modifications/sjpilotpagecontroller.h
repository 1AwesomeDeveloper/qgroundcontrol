#ifndef SJPILOTPAGECONTROLLER_H
#define SJPILOTPAGECONTROLLER_H

#include <QObject>
#include "customerdata.h"
#include "QGCApplication.h"
#include "LinkManager.h"
#include <QMessageBox>

class SJPilotPageController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString passWord READ passWord WRITE setPassWord NOTIFY passWordChanged)
    Q_PROPERTY(bool okButton READ okButton WRITE checkDataBase NOTIFY verifyCredentials)

public:
    explicit SJPilotPageController(QObject *parent = nullptr);

    QString userName(){return m_userName;}
    QString passWord(){return m_passWord;}

    bool okButton(){return checkNow;}

    void setUserName(const QString &userName){m_userName = userName;emit userNameChanged();}
    void setPassWord(const QString &passWord){m_passWord = passWord;emit passWordChanged();}

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

#endif // SJPILOTPAGECONTROLLER_H
