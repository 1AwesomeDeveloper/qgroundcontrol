#include "sjfirmwarecontrol.h"
#include "QGCApplication.h"
SJFirmwareControl::SJFirmwareControl(QObject *parent) : QObject(parent)
{

}

bool SJFirmwareControl::getLatestFirmware()
{
    connect(qgcApp()->getCust(), &CustomerData::firmwareDownloadFailed, this, &SJFirmwareControl::DownloadFailed);
    connect(qgcApp()->getCust(), &CustomerData::firmwareDownloadComplete, this, &SJFirmwareControl::DownloadSuccessfull);
    qgcApp()->getCust()->firmwareDownload(qgcApp()->getCust()->getURL());
    return true;
}

void SJFirmwareControl::deleteFirmwareFile(QString path)
{
    QDir dir;
    dir.remove(path);
}

void SJFirmwareControl::DownloadFailed(QString error)
{
    qInfo() << error;
}

void SJFirmwareControl::DownloadSuccessfull(QString path)
{
    emit firmwareReady(path);
}
