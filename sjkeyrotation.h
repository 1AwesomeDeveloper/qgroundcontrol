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
    void startKeyRotation();

private slots:
    bool downloadKey();
    bool uploadKey();

private:
    QString pathOnBoard;
    QString keyFileName;
};

#endif // SJKEYROTATION_H
