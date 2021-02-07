#ifndef SJFIRMWARECONTROL_H
#define SJFIRMWARECONTROL_H

#include <QObject>

class SJFirmwareControl : public QObject
{
    Q_OBJECT
public:
    explicit SJFirmwareControl(QObject *parent = nullptr);

signals:


public slots:
    bool getLatestFirmwareInfo(QString m_url);


private:

};

#endif // SJFIRMWARECONTROL_H
