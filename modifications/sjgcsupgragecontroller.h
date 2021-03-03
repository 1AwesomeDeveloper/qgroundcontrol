#ifndef SJGCSUPGRAGECONTROLLER_H
#define SJGCSUPGRAGECONTROLLER_H

#include <QObject>
#include "customerdata.h"
#include "QGCApplication.h"

class SJGCSUpgrageController : public QObject
{
    Q_OBJECT
public:
    explicit SJGCSUpgrageController(QObject *parent = nullptr);
    Q_INVOKABLE void UpgrageCheckINIT();

signals:

public slots:
};

#endif // SJGCSUPGRAGECONTROLLER_H
