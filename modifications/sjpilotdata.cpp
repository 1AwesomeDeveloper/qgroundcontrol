#include "sjpilotdata.h"

SJPilotData::SJPilotData(QString url, QObject *parent): QObject(parent), serverURL(url)
{

}

void SJPilotData::get(QString location)
{
    qInfo() << "Getting from Server.....";
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(location)));
    connect(reply, &QNetworkReply::readyRead, this, &SJPilotData::readyRead);
}

void SJPilotData::postEmailPass(QByteArray data)
{
        qInfo() << "Posting to Server.....";
        QNetworkRequest request = QNetworkRequest(QUrl(serverURL + "/login"));
        request.setRawHeader("Content-Type", "application/json");
        QNetworkReply* reply = manager.post(request, data);
        connect(reply, &QNetworkReply::finished, this, &SJPilotData::readyRead);
}

void SJPilotData::readyRead()
{
    qInfo() << "Ready Read";
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QString replyStr = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(replyStr.toUtf8());
    qInfo() << replyStr;
    QJsonObject jsonObject = jsonResponse.object();

    if(jsonObject.find("error") != jsonObject.end()){
        emit wrongDetails(jsonObject["error"].toObject()["message"].toString());
        return;
    }
    if(jsonObject.find("loginToken") == jsonObject.end()){
        emit wrongDetails(replyStr);
        return;
    }
    authCode.clear();
    QString x = jsonObject["loginToken"].toString();
    authCode.append(x);
    qInfo() << "authcode  = " << authCode;
    emit correctDetails();

}
