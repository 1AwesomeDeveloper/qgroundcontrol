#include "customerdata.h"
#include "Vehicle.h"

#include "MultiVehicleManager.h"
#include "QGCApplication.h"
#include "MockLink.h"
#include "FTPManager.h"
#include "string"

// Singleton

CustomerData::CustomerData(QObject* parent): QObject(parent)

{
    serverURL = QString(SERVER_URL) + "/customer";
    pilot = new SJPilotData(QString(SERVER_URL) + "/pilot");
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

void CustomerData::getLatestGCSversion()
{
    qInfo() << "Posting to Server.....";
    QNetworkRequest request = QNetworkRequest(serverURL + "/GCSVersion");
    request.setRawHeader("Content-Type", "application/json");
    QNetworkReply* reply = manager.get(request);
    connect(reply, &QNetworkReply::finished, this, &CustomerData::readyReadGCSVersion);
}

void CustomerData::postEmailPass(QString location, QByteArray data)
{    if(loggedIN){
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
    request.setRawHeader("loginauth", authCode);
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
    clearVehicleData();
    //
}

void CustomerData::clearVehicleData()
{
    vehicleData.npntCheck = false;
    vehicleData.vehicleSerialId.clear();
    vehicleData.modalID.clear();
}

void CustomerData::getLatestFirmwareInfo(QString location)
{
    qInfo() <<"Requesting latest firmware...";
    location += QString("/" + vehicleData.modalID);
    QUrl url(location);
    QNetworkRequest request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader , "application/json");
    request.setRawHeader("auth",tokenDroneNo);

    QNetworkReply* reply = manager.get(request);
    connect(reply,&QNetworkReply::readyRead, this, &CustomerData::readyReadGetLatestFirmwareInfo);
}

void CustomerData::uploadKey(QString location, QString pathOfKey)
{
    QString fileName;
    for(auto it = pathOfKey.rbegin(); it!=pathOfKey.rend(); it++)
        if(*it == '/') break;
        else fileName.push_front(*it);
    qInfo() << fileName;

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QHttpPart imagePart;
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-pem-file"));
        imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data ; name=\"keyFile\" ; filename=\"(%1)\"").arg(fileName)) );

        //UNDER CONSTRUCTION
        QString apkLocation = pathOfKey;
        //
        QFile *file = new QFile(apkLocation);
        if(!file->open(QIODevice::ReadOnly)){
            qInfo() << "File Not Open";
            return;
        }
        imagePart.setBodyDevice(file);
        file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

        multiPart->append(imagePart);

//        QUrlQuery params;
//        //UNDER CONSTRUCTION
//            QString _id = this->vehicleData.vehicleSerialId;
//            params.addQueryItem("id", _id);
//        //
        QUrl url(location + "/" + vehicleData.vehicleServerID);
//        url.setQuery(params.query());

        QNetworkRequest request(url);

        request.setRawHeader("auth", tokenDroneNo);
        QNetworkReply* reply = manager.post(request, multiPart);
        multiPart->setParent(reply);

         connect(reply, SIGNAL(finished()),
                 this, SLOT  (readyReadUploadKey()));
}

void CustomerData::firmwareDownload(QString location)
{
    location += "/downloadLatestFirmware";
    qInfo() <<"Downloading latest firmware...";
    location += QString("/" + vehicleData.modalID);
    QUrl url(location);
    QNetworkRequest request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader , "application/json");
    request.setRawHeader("auth",tokenDroneNo);
    QNetworkReply* reply = manager.get(request);
    connect(reply,&QNetworkReply::finished, this, &CustomerData::readyReadDownload);
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

void CustomerData::readyReadGCSVersion()
{
    qInfo() << "Ready Read OTP ";
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QString replyStr = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(replyStr.toUtf8());
    qInfo() << replyStr;
    QJsonObject jsonObject = jsonResponse.object();
    if(jsonObject.find("error") != jsonObject.end()){
        emit wrongOTP(jsonObject["error"].toObject()["message"].toString());
        return;
    }
    if(QString(SOFTWARE_VERSION) < jsonObject["GCSVersion"].toString()){
        qInfo()<<"UPGRADE REQUIRED";
    }
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
        emit wrongOTP(jsonObject["error"].toObject()["message"].toString());
        return;
    }
    if(jsonObject.find("accessToken") == jsonObject.end()){
        emit wrongOTP(replyStr);
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
    QString replyStr = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(replyStr.toUtf8());
    qInfo() << replyStr;
    QJsonObject jsonObject = jsonResponse.object();
    if(jsonObject.find("error") != jsonObject.end()){
            emit droneNotRegistered();
    }
    if(jsonObject.find("modalId") != jsonObject.end() && jsonObject.find("id") != jsonObject.end()){
        vehicleData.modalID.clear();
        vehicleData.vehicleServerID.clear();
        vehicleData.vehicleServerID = jsonObject["id"].toString();
        vehicleData.modalID = jsonObject["modalId"].toString();
            droneStatusCheck = true;
            emit droneRegistered();
     }
    else emit droneNotRegistered();

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
        emit getFirmwareInfoFailed(false);
    }
    else{
        QString latestVersion = jsonObject["version"].toString();
        int version[3], idx=0;
        memset(version, 0, sizeof(version));
        for(auto c: latestVersion){
            if(c.digitValue() == -1){
                if(c.toLatin1() == QChar('.').toLatin1()){
                    idx++;
                }
            }
            else if(idx < 3){
                version[idx] *= 10;
                version[idx] += c.digitValue();
            }
        }
        qInfo() << "Latest Firmware Info : "<< version[0] <<"." <<version[1]<<"."<<version[2];
        int res = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->versionCompare(version[0], version[1], version[2]);
        if(res!=0) emit getFirmwareInfoFailed(true);
        else emit getFirmwareInfoSuccessfull();
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
        vehicleData.npntCheck = true;
    }
}

void CustomerData::readyReadDownload()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(reply->error())
        {
            qDebug() << "ERROR!";
            qDebug() << reply->errorString();
            emit firmwareDownloadFailed(reply->errorString());
        }

        else
        {
            qDebug() << "1) " << reply->header(QNetworkRequest::ContentTypeHeader).toString();
            qDebug() << "2) " << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
            qDebug() << "3) " << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
            qDebug() << "4) " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << "5) " << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

            QString path(QStandardPaths::writableLocation(QStandardPaths::TempLocation)), path2;
            path += "/space_jam_custom_firmware.bin";
            path2 = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+"/space_jam_custom_firmware.bin";
            QFile file(path);
            if(file.open(QFile::WriteOnly | QIODevice::Truncate))
            {
                qDebug() <<"The text file is open";
                file.write(reply->readAll());
                file.close();
                QDir dir;
                dir.rename(path, path2);
                emit firmwareDownloadComplete(path2);

            }
            else{
                qDebug() << "file not open";
                emit firmwareDownloadFailed("Cannot Load Firmware File");
            }
        }

        reply->deleteLater();
}

