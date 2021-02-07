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

bool CustomerData::vehicleIDChanged(QString vehicleID)
{
    if(vehicleData.vehicleSerialId.size() == 0){
        //new key detected
        vehicleData.vehicleSerialId = vehicleID;
        return false;
    }
    if(!DroneStatusCheck()){
        vehicleData.vehicleSerialId = vehicleID;
        return false;
    }
    if(vehicleID == vehicleData.vehicleSerialId) return false;
    return true;
}

void CustomerData::get(QString location)
{
    qInfo() << "Getting from Server.....";
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(location)));
    connect(reply, &QNetworkReply::readyRead, this, &CustomerData::readyRead);
}

void CustomerData::postEmailPass(QString location, QByteArray data)
{

    if(loggedIN){
        qInfo() << "Customer already logged in";
        return;
    }
    qInfo() << "Posting to Server.....";
    QNetworkRequest request = QNetworkRequest(QUrl(location));
    request.setRawHeader("Content-Type", "application/json");
    QNetworkReply* reply = manager.post(request, data);
    connect(reply, &QNetworkReply::finished, this, &CustomerData::readyRead);
}

void CustomerData::postOTP(QString location, QByteArray data)
{
    if(loggedIN){
        qInfo() << "Customer already logged in";
        return;
    }
    qInfo() << "Posting OTP to Server.....";
    QNetworkRequest request = QNetworkRequest(QUrl(location));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("loingauth", authCode);
    QNetworkReply* reply = manager.post(request, data);
    connect(reply, &QNetworkReply::readyRead, this, &CustomerData::readyReadOTP);
}

// for posting drone number
void CustomerData::postDroneNo(QString location)
{
    qInfo() <<"Posting drone number to Server...";
    if(vehicleData.vehicleSerialId.size() == 0){
        qInfo() << "Vehicle Id not Retrieved";
        return;
    }
    QByteArray n;
    n.append("{\"flightControllerNumber\":\"");
    n.append(vehicleData.vehicleSerialId);
    n.append("\"}");
    qInfo() << vehicleData.vehicleSerialId;
    QNetworkRequest request = QNetworkRequest(QUrl(location));
    request.setHeader(QNetworkRequest::ContentTypeHeader , "application/json");
    request.setRawHeader("auth",tokenDroneNo);
    QNetworkReply* reply = manager.post(request,n);
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
    //NOT FINISHED
    tokenDroneNo.clear();
    loggedIN = false;
    droneStatusCheck = false;
    authCode.clear();
    token.clear();
    tokenDroneReply.clear();
    //
}

void CustomerData::getLatestFirmwareInfo(QString location)
{
    qInfo() <<"Requesting latest firmware...";
    QUrlQuery params;
        params.addQueryItem("token", tokenDroneNo);
        params.addQueryItem("id", "601bd6bbaf54850017f2ce5b");
    QUrl url(location);
    url.setQuery(params.query());
    QNetworkRequest request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader , "application/json");
    //request.setRawHeader("auth",tokenDroneNo);

    QNetworkReply* reply = manager.get(request);
    connect(reply,&QNetworkReply::readyRead, this, &CustomerData::readyReadGetLatestFirmwareInfo);
}

void CustomerData::uploadKey(QString location)
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QHttpPart imagePart;
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-pem-file"));
        imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data ; name=\"keyFile\" ; filename=\"a.pem\"") );

        //UNDER CONSTRUCTION
        QString apkLocation = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        //
        QFile *file = new QFile(apkLocation);
        if(!file->open(QIODevice::ReadOnly)){
            qInfo() << "File Not Open";
            return;
        }
        imagePart.setBodyDevice(file);
        file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

        multiPart->append(imagePart);

        QUrlQuery params;
        //UNDER CONSTRUCTION
            QString _id = ""; // yet to be decided
            params.addQueryItem("id", _id);
        //
        QUrl url(location);
        url.setQuery(params.query());

        QNetworkRequest request(url);

        request.setRawHeader("auth",tokenDroneNo);
        QNetworkReply* reply = manager.post(request, multiPart);
        multiPart->setParent(reply);

         connect(reply, SIGNAL(finished()),
                  this, SLOT  (readyReadUploadKey()));
}


void CustomerData::readyRead()
{
    qInfo() << "Ready Read";
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QString replyStr = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(replyStr.toUtf8());
    qInfo() << replyStr;
    QJsonObject jsonObject = jsonResponse.object();

    if(jsonObject.find("error") != jsonObject.end()){
        emit wrongDetails();
        return;
    }
    if(jsonObject.find("loginToken") == jsonObject.end()){
        emit wrongDetails();
        return;
    }
    authCode.clear();
    QString x = jsonObject["loginToken"].toString();
    authCode.append(x);
    qInfo() << "authcode  = " << authCode;
    emit correctDetails();

}

void CustomerData::readyReadOTP()
{
    qInfo() << "Ready Read OTP ";
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QString replyStr = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(replyStr.toUtf8());
    qInfo() << replyStr;
    QJsonObject jsonObject = jsonResponse.object();
    if(jsonObject.find("error") != jsonObject.end()){
        emit wrongOTP();
        return;
    }
    if(jsonObject.find("accessToken") == jsonObject.end()){
        emit wrongOTP();
        return;
    }
    tokenDroneNo.clear();
    QString x = jsonObject["accessToken"].toString();
    tokenDroneNo.append(x);
    qInfo() << "accessToken  = " << tokenDroneNo;
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
    if(jsonObject.find("error") != jsonObject.end()){
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
    if(jsonObject.find("error") != jsonObject.end()){
        emit getFirmwareInfoFailed();
    }
    else{
        //qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->versionCompare();
        emit getFirmwareInfoSuccessfull();
    }
}

void CustomerData::readyReadUploadKey()
{
    qInfo() <<"Ready Read Upload Key";
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QString replyStr = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(replyStr.toUtf8());
    qInfo() << replyStr;
    QJsonObject jsonObject = jsonResponse.object();
    // response is parsed
    if(jsonObject.find("error") != jsonObject.end()){
        emit keyUploadFailed();
    }
    else{
        emit keyUploadSuccessful();
    }
}


//extern QObject *singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
//{
//    Q_UNUSED(engine)
//    Q_UNUSED(scriptEngine)
//    return getInstance();
//}


