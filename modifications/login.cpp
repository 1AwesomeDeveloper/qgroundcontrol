#include "login.h"
#include "customerdata.h"

Login::Login(QObject *parent): QObject(parent){
    mutex = false;
    myURL = "https://drone-management-api-ankit1998.herokuapp.com/customer";
    qgcApp()->toolbox()->linkManager()->shutdown();
    loginStatus = false;
    OTPStatus   = false;
    connect(qgcApp()->getCust(), &CustomerData::correctDetails,         this, &Login::LoginSuccessful);
    connect(qgcApp()->getCust(), &CustomerData::wrongDetails,           this, &Login::LoginFailed);

    connect(qgcApp()->getCust(), &CustomerData::correctOTP,             this, &Login::OTPSuccessful);
    connect(qgcApp()->getCust(), &CustomerData::wrongOTP,               this, &Login::OTPFailed);

    connect(qgcApp()->getCust(), &CustomerData::loggedOutSuccessfully,  this, &Login::logoutSuccessful);
    connect(qgcApp()->getCust(), &CustomerData::loggedOutFailed,        this, &Login::logoutFailed);

    connect(this, &Login::logoutCustomer, qgcApp()->getCust(), &CustomerData::clearData);

}

QString Login::userName(){
    return m_userName;
}

QString Login::otp(){
    return m_otp;
}

QString Login::passWord(){
    return m_passWord;
}

bool Login::okButton(){
    return checkNow;
}

bool Login::otpButton(){
    return m_otpButton;
}


void Login::setUserName(const QString &userName){
    if (userName == m_userName)
        return;
    m_userName = userName;
    emit userNameChanged();
}

void Login::setOTP(const QString &otp)
{
    if (otp == m_otp)
        return;
    m_otp = otp;
    emit otpChanged();
}
void Login::setPassWord(const QString &passWord){
    if (passWord == m_passWord)
        return;

    m_passWord = passWord;
    emit passWordChanged();
}


void Login::checkDataBase(bool okButton)
{
    if(okButton){
        if(mutex){
            //customer already logged in
        }
        else{
            // Tries to log in
            QByteArray jsonString;                      // pack data
            jsonString.append("{\"email\":\"");
            jsonString.append(m_userName);
            jsonString.append("\",\"password\":\"");
            jsonString.append(m_passWord);
            jsonString.append("\"}");
            qgcApp()->getCust()->postEmailPass(myURL+"/login", jsonString);
        }
        emit verifyCredentials();
    }
}

void Login::checkOTP(bool otpButton)
{
    if(otpButton){
        if(mutex){
            //customer already logged in
        }
        else{
            QByteArray s;
            s.append("{\"otp\":\"");
            s.append(m_otp);
            s.append("\"}");
            qgcApp()->getCust()->postOTP(myURL+"/otpValidation", s);

        }
        emit verifyOTP();
    }
}

void Login::custLogout(bool logoutButton)
{
    if(logoutButton){
        if(loginStatus && OTPStatus){
            qgcApp()->getCust()->get(myURL+"/logout");
            emit logoutCustomer();
        }
    }
}

void Login::LoginSuccessful()
{
    m_userName.clear();
    m_passWord.clear();
    changeLoginStatus(true);
    emit userNameChanged();
    emit passWordChanged();
}

void Login::LoginFailed()
{
    m_userName.clear();
    m_passWord.clear();
    emit userNameChanged();
    emit passWordChanged();
}

void Login::OTPSuccessful()
{
    m_otp.clear();
    changeOTPStatus(true);
    qgcApp()->toolbox()->linkManager()->restart();
    emit otpChanged();
}

void Login::OTPFailed()
{
    m_otp.clear();
    emit otpChanged();
}

void Login::logoutSuccessful()
{
    changeOTPStatus(false);
    changeLoginStatus(false);
    emit logoutCustomer();
}

void Login::logoutFailed()
{

}
