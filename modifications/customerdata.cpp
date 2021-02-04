#include "customerdata.h"
#include "Vehicle.h"

#include "MultiVehicleManager.h"
#include "QGCApplication.h"
#include "MockLink.h"
#include "FTPManager.h"


// Singleton

CustomerData::CustomerData(QObject* parent): QObject(parent)
{
    loggedIN = false;
    droneStatusCheck = false;
}

void CustomerData::get(QString location)
{
    qInfo() << "Getting from Server.....";
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(location)));
    connect(reply, &QNetworkReply::readyRead, this, &CustomerData::readyRead);
}

void CustomerData::postEmailPass(QString location, QByteArray data)
{

    qInfo() << "Posting to Server.....";
    QNetworkRequest request = QNetworkRequest(QUrl(location));
    request.setRawHeader("Content-Type", "application/json");
    QNetworkReply* reply = manager.post(request, data);
    connect(reply, &QNetworkReply::finished, this, &CustomerData::readyRead);
}

void CustomerData::postOTP(QString location, QByteArray data)
{
    qInfo() << "Posting OTP to Server.....";
    QNetworkRequest request = QNetworkRequest(QUrl(location));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("loingauth", authCode);
    QNetworkReply* reply = manager.post(request, data);
    connect(reply, &QNetworkReply::readyRead, this, &CustomerData::readyReadOTP);
}

// for posting drone number
void CustomerData::postDroneNo(QString location, QByteArray data)
{
    qInfo() <<"Posting drone number to Server...";
    QNetworkRequest request = QNetworkRequest(QUrl(location));
    request.setHeader(QNetworkRequest::ContentTypeHeader , "application/json");
    request.setRawHeader("auth",tokenDroneNo);
    QNetworkReply* reply = manager.post(request,data);
    connect(reply,&QNetworkReply::readyRead, this, &CustomerData::readyReadDroneNo);

}

void CustomerData::logOutCustomer(QString location, QByteArray data)
{
    qInfo() <<"Requesting to Log Out...";
    QNetworkRequest request = QNetworkRequest(QUrl(location));
    request.setHeader(QNetworkRequest::ContentTypeHeader , "application/json");
    request.setRawHeader("auth",tokenDroneNo);
    QNetworkReply* reply = manager.post(request,data);
    connect(reply,&QNetworkReply::readyRead, this, &CustomerData::readyReadDroneNo);
}

void CustomerData::clearData()
{
    tokenDroneNo.clear();
    loggedIN = false;
    droneStatusCheck = false;
    authCode.clear();
    token.clear();
    tokenDroneReply.clear();
}

void CustomerData::getLatestFirmwareInfo(QString location)
{
    qInfo() <<"Requesting latest firmware...";
    QUrlQuery params;
        params.addQueryItem("token", tokenDroneNo);
    QUrl url(location);
    url.setQuery(params.query());
    QNetworkRequest request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader , "application/json");
    //request.setRawHeader("auth",tokenDroneNo);

    QNetworkReply* reply = manager.get(request);
    connect(reply,&QNetworkReply::readyRead, this, &CustomerData::readyReadGetLatestFirmwareInfo);
}


void CustomerData::readyRead()
{
    qInfo() << "Ready Read";
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray replyArray = reply->readAll(), m_authCode;
    qInfo() << replyArray;
    int idx = replyArray.lastIndexOf("loginToken");
    int idx1 = replyArray.lastIndexOf("error");
    if(loggedIN){
        qInfo() << "Customer already logged in";
        return;
    }
    if(idx == -1 || idx1 != -1){
        emit wrongDetails();
        return;
    }
    idx += 13;
    for(int i=idx; i < replyArray.size()-2; i++){
        m_authCode.append(replyArray[i]);
    }
    this->authCode = m_authCode;
    emit correctDetails();

}

void CustomerData::readyReadOTP()
{
    qInfo() << "Ready Read OTP ";
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray replyArray = reply->readAll(), m_token;
    int idx = replyArray.lastIndexOf("error");
    int idx1 = replyArray.lastIndexOf("accessToken");
    if(idx != -1){
        emit wrongOTP();
        return;
    }
    if(loggedIN){
        qInfo() << "Customer already logged in";
        return;
    }
    idx1 += 14;
    for(int i=idx1; i<replyArray.size()-1; i++){
        m_token.append(replyArray[i]);
    }
    qInfo() << m_token;
    // extracting tokenDroneNo for "auth":header session value
    tokenDroneNo.clear();
    for(int i =0;i<m_token.size()-1;i++){
        tokenDroneNo.append(m_token[i]);
    }
    this->token = m_token;
    this->authCode.clear();
    loggedIN = true;
    emit correctOTP();
}

//extracting reply after validating drone number
void CustomerData::readyReadDroneNo()
{
    qInfo() <<"Ready Read Drone";
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray replyArray = reply->readAll();
    qInfo() << replyArray;
    if(replyArray.contains("Your drone is not registered")){
            emit droneNotRegistered();
        }
    if(replyArray.contains("modal")){
            droneStatusCheck = true;
            emit droneRegistered();
        }

}

void CustomerData::readyReadLogOut()
{
    qInfo() <<"Ready Read Logout";
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QString replyStr = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(replyStr.toUtf8());
    qInfo() << replyStr;
    QJsonObject jsonObject = jsonResponse.object();
    // response is parsed
    if(jsonObject.find("error") == jsonObject.end()){
        emit loggedOutFailed();
    }
    else{
        emit loggedOutSuccessfully();
        tokenDroneNo.clear();
    }
}

void CustomerData::readyReadGetLatestFirmwareInfo()
{
    qInfo() <<"Ready Read Firmware Info";
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QString replyStr = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(replyStr.toUtf8());
    qInfo() << replyStr;
    QJsonObject jsonObject = jsonResponse.object();
    // response is parsed
    if(jsonObject.find("error") == jsonObject.end()){
        emit getFirmwareInfoFailed();
    }
    else{
        //qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->versionCompare();
        emit getFirmwareInfoSuccessfull();
    }
}


//extern QObject *singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
//{
//    Q_UNUSED(engine)
//    Q_UNUSED(scriptEngine)
//    return getInstance();
//}


