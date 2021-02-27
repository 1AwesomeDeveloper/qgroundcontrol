#include "sj_npntcontrol.h"


SJ_NPNTControl::SJ_NPNTControl(QObject *parent) : QObject(parent)
{
    keyRotating = false;
    m_keyController         = new SJKeyRotation();
    cust                    = qgcApp()->getCust();
    m_url                   = qgcApp()->getCust()->getURL();
    timer1                  = new QTimer(this);
    timer2                  = new QTimer(this);
    timer3                  = new QTimer(this);
    timer4                  = new QTimer(this);
    connect(timer1  , SIGNAL(timeout())                  , this, SLOT(deviceConnected())         );
    connect(timer2  , SIGNAL(timeout())                  , this, SLOT(boardRegistered())         );
    connect(timer3  , SIGNAL(timeout())                  , this, SLOT(firmwareCheck())           );
    //connect(timer4  , SIGNAL(timeout())                  , this, SLOT(boardRegistered())         );

    connect(cust, SIGNAL(droneNotRegistered()), this, SLOT(boardNotRegistered()));
    connect(cust, SIGNAL(droneRegistered()), this, SLOT(boardIsRegistered()));

    connect(cust    , &CustomerData::getFirmwareInfoFailed     , this, &SJ_NPNTControl::firmwareUpgradeRequired  );
    connect(cust    , SIGNAL(getFirmwareInfoSuccessfull()), this, SLOT(firmwareOK())                );
    connect(cust    , SIGNAL(keyUploadFailed())           , this, SLOT(KeyRotateFailed())   );
    connect(cust    , SIGNAL(keyUploadSuccessful())       , this, SLOT(keyRotatedOK())                );

}

bool SJ_NPNTControl::deviceConnected()
{
    if(!qgcApp()->toolbox()->multiVehicleManager()->activeVehicleAvailable()){
        // No active Board Connected
        qInfo() << "Trying To find the board";
        timer1->start(500);
        return false;
    }
    timer1->stop();
    emit check1();
    boardRegistered();
    return true;
}

bool SJ_NPNTControl::boardRegistered()
{
    qInfo() << "Trying To Communicate with the board";
    cust->postDroneNo(m_url+"/checkMyDrone");
    timer2->start(5000);
    return true;
}

bool SJ_NPNTControl::firmwareCheck()
{
    qgcApp()->getCust()->getLatestFirmwareInfo(m_url+"/latestFirmwareVersion");
    timer3->start(2000);
    return true;
}

bool SJ_NPNTControl::keyRotated()
{
    if(!keyRotating)
    {
        keyRotating = true;
        m_keyController->startKeyRotation(m_url+"/flyup");
        return true;
    }
    return false;
}

void SJ_NPNTControl::firmwareOK()
{
    timer3->stop();
    emit check3();
    keyRotated();
}

void SJ_NPNTControl::firmwareUpgradeRequired(bool res)
{
    m_firmwareController = new SJFirmwareControl();
    //m_firmwareController->getLatestFirmware(m_url);
    timer3->stop();
    if(res){
        int RES = ErrorMessageBox("!Firmware is not Updated!\n"
                        "\n"
                        "Would You like to Upgrade firmware Now?");
        switch (RES) {
            case QMessageBox::Ok:{
                emit firmwareUpgradeInit();
                break;
            }
            case QMessageBox::Cancel:{
                emit check3();
                keyRotated();
                break;
            }
        }
    }
}

void SJ_NPNTControl::keyRotatedOK()
{
    keyRotating = false;
    emit check4();
}

void SJ_NPNTControl::KeyRotateFailed()
{
    keyRotating = false;
    return;
}

int SJ_NPNTControl::ErrorMessageBox(QString errorMessage)
{
    QMessageBox msgBox;
    msgBox.setText("Error: ");
    msgBox.setInformativeText(errorMessage);
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    return msgBox.exec();
}

void SJ_NPNTControl::boardIsRegistered()
{
    timer2->stop();
    emit check2();
    // Initiate Next Step
    firmwareCheck();
}

void SJ_NPNTControl::boardNotRegistered()
{
    timer2->stop();
    ErrorMessageBox("Your Drone is not Registered");
    // emit signal or dialog box popup to inform to try different board
}
