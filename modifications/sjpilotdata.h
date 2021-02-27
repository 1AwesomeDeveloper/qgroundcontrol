#ifndef SJPILOTDATA_H
#define SJPILOTDATA_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QNetworkProxy>
#include <QByteArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>
#include "Vehicle.h"

class SJPilotData : public QObject
{
        Q_OBJECT

    public:
        explicit SJPilotData(QString url, QObject* parent=nullptr);
        QString getURL(){return serverURL;}

    signals:
        void correctDetails();
        void wrongDetails(QString error);


    public slots:
        void get(QString location);
        void postEmailPass(QByteArray data);

    private slots:
        void readyRead();

    private:
        QNetworkAccessManager manager;
        QString serverURL;
        QByteArray authCode;
    };


#endif // SJPILOTDATA_H
