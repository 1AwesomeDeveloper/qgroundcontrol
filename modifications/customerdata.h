#ifndef CUSTOMERDATA_H
#define CUSTOMERDATA_H

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
class CustomerData: public QObject
{
    Q_OBJECT

public:
       explicit CustomerData(QObject* parent=nullptr);
    bool loggedIn(){return loggedIN;}
    bool DroneStatusCheck(){return droneStatusCheck;}
    bool vehicleIDChanged(QString vehicleID);
    bool getNpntStatus(){return vehicleData.npntCheck;}

signals:
    void correctDetails();
    void wrongDetails(QString error);
    void correctOTP();
    void wrongOTP(QString error);
    void droneRegistered();
    void droneNotRegistered();
    //...............Edited..............//
        void loggedOutSuccessfully();
        void loggedOutFailed();
    //...................................//
    void getFirmwareInfoFailed();
    void getFirmwareInfoSuccessfull();

    void keyUploadFailed();
    void keyUploadSuccessful();

public slots:
    void get(QString location);
    void postEmailPass(QString location, QByteArray data);
    void postOTP(QString location, QByteArray data);
    void postDroneNo(QString location); // post droneNo to check whether it's registered or not
    void logOutCustomer(QString location, QByteArray data);
    void clearData();
    void getLatestFirmwareInfo(QString location);
    void uploadKey(QString location, QString pathOfKey);

private slots:
    void readyRead();
    void readyReadOTP();
    void readyReadDroneNo();//getting droneNo validation reply
    void readyReadLogOut();
    void readyReadGetLatestFirmwareInfo();
    void readyReadUploadKey();

//    void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
//    void encrypted(QNetworkReply *reply);
//    void finished(QNetworkReply *reply);
//    void preSharedKeyAuthenticationRequired(QNetworkReply *reply, QSslPreSharedKeyAuthenticator *authenticator);
//    void proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
//    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

private:
    struct customerVehicleData
    {
        QString vehicleSerialId;
        bool npntCheck;
    } vehicleData;

    QNetworkAccessManager manager;
    QByteArray authCode;
    QByteArray token;
    QByteArray tokenDroneNo;//storing 'auth': header session
    QByteArray tokenDroneReply;
    bool loggedIN;
    bool droneStatusCheck;
};

extern QObject *singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine);
extern CustomerData *getInstance();
#endif // CUSTOMERDATA_H
