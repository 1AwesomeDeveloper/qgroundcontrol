#include "sjpilotpagecontroller.h"
#include "customerdata.h"

SJPilotPageController::SJPilotPageController(QObject *parent) : QObject(parent)
{
    connect(qgcApp()->getCust()->getPilot(), &SJPilotData::correctDetails, this, &SJPilotPageController::LoginSuccessful);
    connect(qgcApp()->getCust()->getPilot(), &SJPilotData::wrongDetails, this, &SJPilotPageController::LoginFailed);
}

void SJPilotPageController::checkDataBase(bool okButton)
{
    if(okButton && !checkNow){
        checkNow = true;
        QByteArray jsonString;                      // pack data
        jsonString.append("{\"email\":\"");
        jsonString.append(m_userName);
        jsonString.append("\",\"password\":\"");
        jsonString.append(m_passWord);
        jsonString.append("\",\"droneId\":\"");
        jsonString.append(qgcApp()->getCust()->getDroneServerID());
        jsonString.append("\"}");
        qgcApp()->getCust()->getPilot()->postEmailPass(jsonString);
    }
}

void SJPilotPageController::LoginSuccessful()
{
    checkNow = false;
    emit loginSuccessfull();
}

void SJPilotPageController::LoginFailed(QString error)
{
    checkNow = false;
    qDebug() << error;
}

int SJPilotPageController::ErrorMessageBox(QString errorMessage)
{
    qDebug() << errorMessage;
    return 0;
}
