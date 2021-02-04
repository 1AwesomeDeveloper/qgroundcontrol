#include "sjfirmwarecontrol.h"
#include "QGCApplication.h"
SJFirmwareControl::SJFirmwareControl(QObject *parent) : QObject(parent)
{
    m_url = "https://drone-management-api-ankit1998.herokuapp.com/customer";
}

bool SJFirmwareControl::getLatestFirmwareInfo()
{
    // Check if Logged in
    if(!qgcApp()->getCust()->loggedIn()){
        qInfo() << "Customer not logged in";
        return false;
    }

    // Check if Drone is Registered
    if(!qgcApp()->getCust()->DroneStatusCheck()){
        qInfo() << "Drone Not Registered";
        return false;
    }

    qgcApp()->getCust()->getLatestFirmwareInfo(m_url+"/allFirmware");
    return true;
}
