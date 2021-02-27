#ifndef SJFIRMWARECONTROL_H
#define SJFIRMWARECONTROL_H

#include <QObject>

class SJFirmwareControl : public QObject
{
    Q_OBJECT
public:
    explicit SJFirmwareControl(QObject *parent = nullptr);

signals:
    void firmwareReady(QString path);

public slots:
    Q_INVOKABLE bool getLatestFirmware();
    Q_INVOKABLE void deleteFirmwareFile(QString path);

private slots:
    void DownloadFailed(QString error);
    void DownloadSuccessfull(QString path);

private:

};

#endif // SJFIRMWARECONTROL_H
