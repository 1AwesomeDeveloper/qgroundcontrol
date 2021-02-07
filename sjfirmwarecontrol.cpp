#include "sjfirmwarecontrol.h"
#include "QGCApplication.h"
SJFirmwareControl::SJFirmwareControl(QObject *parent) : QObject(parent)
{

}

bool SJFirmwareControl::getLatestFirmwareInfo(QString m_url)
{
    qgcApp()->getCust()->getLatestFirmwareInfo(m_url+"/latestFirmwareDownload");
    return true;
}
