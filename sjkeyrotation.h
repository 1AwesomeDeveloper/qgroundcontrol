#ifndef SJKEYROTATION_H
#define SJKEYROTATION_H

#include <QObject>
#include <FTPManager.h>
#include <QGCApplication.h>
#include <customerdata.h>

class SJKeyRotation : public QObject
{
    Q_OBJECT
public:
    explicit SJKeyRotation(QObject *parent = nullptr);

signals:

public slots:
    void startKeyRotation(QString URL);

private slots:
    bool downloadKey();
    bool uploadKey();
    void downloadComplete(const QString &path, const QString &error);

private:
    QString pathOnBoard;
    QString keyFileName;
    QString pathOnSystem;
    QString url;
    bool uploading;
};

#endif // SJKEYROTATION_H
