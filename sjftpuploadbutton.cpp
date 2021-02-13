#include "sjftpuploadbutton.h"

SJFTPUploadButton::SJFTPUploadButton(QObject *parent) : QObject(parent)
{

}

void SJFTPUploadButton::uploadPA(QString path)
{
    QString fileName ;
    for(auto it=path.rbegin(); it!= path.rend(); it++)
        if(*it == '/') break;
        else fileName.push_front(*it);
    qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->ftpManager()->upload(QString("/APM/(%1)").arg(fileName), path);
}
