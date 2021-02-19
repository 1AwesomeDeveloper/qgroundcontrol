#include "sjkeyrotation.h"

SJKeyRotation::SJKeyRotation(QObject *parent) : QObject(parent)
{
    pathOnBoard = "APM";
    keyFileName = "1.pem";
    pathOnSystem = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    uploading = false;
}

void SJKeyRotation::startKeyRotation(QString URL)
{
    url = URL;
    if(!downloadKey()){
        return;
    }
}

bool SJKeyRotation::downloadKey()
{

    FTPManager* ftpManager = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->ftpManager();
    QString a, b;
    connect(ftpManager, SIGNAL(downloadComplete(const QString& , const QString& )), this, SLOT(downloadComplete(const QString&, const QString&)));

    if(!ftpManager->download(pathOnBoard + "/" + keyFileName, pathOnSystem)){
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
    qInfo() << "Uploading key" << pathOnSystem + "/" + keyFileName;
    CustomerData* customer = qgcApp()->getCust();
    customer->uploadKey(url, pathOnSystem + "/" + keyFileName);
    return true;
}

void SJKeyRotation::downloadComplete(const QString &path, const QString &error)
{
    if(error.size() == 0 && !uploading){
        uploadKey();
    }
}
