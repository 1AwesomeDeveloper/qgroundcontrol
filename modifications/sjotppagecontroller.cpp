#include "sjotppagecontroller.h"
#include "customerdata.h"
#include "sjotppagecontroller.h"

SJOTPPageController::SJOTPPageController(QObject *parent): QObject(parent){
    myURL = "https://drone-management-api-ankit1998.herokuapp.com/customer";
    qgcApp()->toolbox()->linkManager()->shutdown();
    OTPStatus   = false;
    connect(qgcApp()->getCust(), &CustomerData::correctOTP,             this, &SJOTPPageController::OTPSuccessful);
    connect(qgcApp()->getCust(), &CustomerData::wrongOTP,               this, &SJOTPPageController::OTPFailed);
}


QString SJOTPPageController::otp(){
    return m_otp;
}


bool SJOTPPageController::otpButton(){
    return m_otpButton;
}


void SJOTPPageController::setOTP(const QString otp)
{
    if (otp == m_otp)
        return;
    m_otp = otp;
    emit otpChanged();
}


void SJOTPPageController::checkOTP(bool otpButton)
{
    if(otpButton){

        QByteArray s;
        s.append("{\"otp\":\"");
        s.append(m_otp);
        s.append("\"}");
        qgcApp()->getCust()->postOTP(myURL+"/otpValidation", s);


        emit verifyOTP();
    }
}



void SJOTPPageController::OTPSuccessful()
{
    m_otp.clear();
    changeOTPStatus(true);
    qgcApp()->toolbox()->linkManager()->restart();
    emit otpChanged();
    emit otpSuccessfull();
}

void SJOTPPageController::OTPFailed(QString error)
{
    ErrorMessageBox(error);
    m_otp.clear();
    emit otpChanged();
}

void SJOTPPageController::changeOTPStatus(bool status)
{
    OTPStatus = status;
    emit loggedInStatusChange();
}

int SJOTPPageController::ErrorMessageBox(QString errorMessage)
{
    QMessageBox msgBox;
    msgBox.setText("Error: ");
    msgBox.setInformativeText(errorMessage);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    return msgBox.exec();
}
