#include "sjloginpagecontroller.h"
#include "customerdata.h"

SJLoginPageController::SJLoginPageController(QObject *parent): QObject(parent){
    mutex = false;
    m_userName = "user_namer";
    myURL = "https://drone-management-api-ankit1998.herokuapp.com/customer";
    qgcApp()->toolbox()->linkManager()->shutdown();
    loginStatus = false;
    connect(qgcApp()->getCust(), &CustomerData::correctDetails,         this, &SJLoginPageController::LoginSuccessful);
    connect(qgcApp()->getCust(), &CustomerData::wrongDetails,           this, &SJLoginPageController::LoginFailed);


}

QString SJLoginPageController::userName(){
    return m_userName;
}


QString SJLoginPageController::passWord(){
    return m_passWord;
}

bool SJLoginPageController::okButton(){
    return checkNow;
}

void SJLoginPageController::setUserName(const QString &userName){
    if (userName == m_userName)
        return;
    m_userName = userName;
    emit userNameChanged();
}

void SJLoginPageController::setPassWord(const QString &passWord){
    if (passWord == m_passWord)
        return;

    m_passWord = passWord;
    emit passWordChanged();
}


void SJLoginPageController::checkDataBase(bool okButton)
{
    if(okButton && !checkNow){
        qInfo() << m_userName << " " << m_passWord;
        checkNow = true;
        // Tries to log in
        QByteArray jsonString;                      // pack data
        jsonString.append("{\"email\":\"");
        jsonString.append(m_userName);
        jsonString.append("\",\"password\":\"");
        jsonString.append(m_passWord);
        jsonString.append("\"}");
        qgcApp()->getCust()->postEmailPass(myURL+"/login", jsonString);
        emit verifyCredentials();
    }
}


void SJLoginPageController::LoginSuccessful()
{
    checkNow = false;
    //m_userName.clear();
    //m_passWord.clear();
    changeLoginStatus(true);
    emit userNameChanged();
    emit passWordChanged();
    emit loginSuccessfull();
}

void SJLoginPageController::LoginFailed(QString error)
{
    checkNow = false;
    ErrorMessageBox(error);
    m_userName.clear();
    m_passWord.clear();
    emit userNameChanged();
    emit passWordChanged();

}

int SJLoginPageController::ErrorMessageBox(QString errorMessage)
{
    QMessageBox msgBox;
    msgBox.setText("Error: ");
    msgBox.setInformativeText(errorMessage);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    return msgBox.exec();
}
