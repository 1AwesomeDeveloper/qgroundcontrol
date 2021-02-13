#ifndef SJFTPUPLOADBUTTON_H
#define SJFTPUPLOADBUTTON_H

#include <QObject>
#include <QDebug>

#include "MultiVehicleManager.h"
#include "QGCApplication.h"
#include "MockLink.h"
#include "FTPManager.h"

class SJFTPUploadButton : public QObject
{
    Q_OBJECT
public:
    explicit SJFTPUploadButton(QObject *parent = nullptr);
    Q_INVOKABLE void uploadPA(QString path);

signals:

public slots:
};

#endif // SJFTPUPLOADBUTTON_H
