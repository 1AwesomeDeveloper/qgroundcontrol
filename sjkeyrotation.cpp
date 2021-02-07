#include "sjkeyrotation.h"

SJKeyRotation::SJKeyRotation(QObject *parent) : QObject(parent)
{
    pathOnBoard = "/key.pem";
}

void SJKeyRotation::startKeyRotation()
{
    if(!downloadKey()){
        return;
    }
    uploadKey();
}

bool SJKeyRotation::downloadKey()
{
    FTPManager* ftpManager = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->ftpManager();
    if(!ftpManager->download(pathOnBoard, QStandardPaths::writableLocation(QStandardPaths::TempLocation))){
        qInfo() << " Cannot Download Key.pem";
            //UNDER CONSTRUCTION
            // emit signal to qml??
            //
        return false;
    }
    return true;
}

bool SJKeyRotation::uploadKey()
{
    CustomerData* customer = qgcApp()->getCust();
    customer->uploadKey(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + keyFileName);
    return true;
}
