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

        void logoutSignal(QString error);


    public slots:
        void postEmailPass(QByteArray data);
        void logout();

    private slots:
        void readyRead();
        void readyReadLogout();

    private:
        QNetworkAccessManager manager;
        QString serverURL;
        QByteArray authCode;
    };


#endif // SJPILOTDATA_H
