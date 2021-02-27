import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.1

import QGroundControl 1.0
import com.NPNT_CONTROL 1.0

 Rectangle{
        id:myrect
        property bool check1:false
        property bool check2:false
        property bool check3:false
        property bool check4:false
        property bool vis: false
        anchors.fill: parent
        color: "grey"
        signal changePage(int page)
        signal npntComplete()
        visible: true
        z:1
        Component.onCompleted: {
            //start NPNT process
            npntController.restartConnection();
            npntController.deviceConnected();

        }
        NpntControl{
            id : npntController;
            onCheck1: myrect.check1 = true;
            onCheck2: myrect.check2 = true;
            onCheck3: myrect.check3 = true;
            onCheck4: myrect.check4 = true;
            onFirmwareUpgradeInit: {
                changePage(4);
            }

        }
        function resetChecks(){
            check1 = false;
            check2 = false;
            check3 = false;
            check4 = false;
        }

    Rectangle{
        id: rect1
        width:parent.width/2.5
        height:parent.height/1.5
        anchors.centerIn: parent
        radius: 80
        color:"lightgrey"
        border.color:"black"
        border.width: 3
        MouseArea{
            anchors.fill:parent
            z:2
        }

        Column{
            spacing: rect1.height/10
            anchors.centerIn: rect1

           //START NPNT CHECKLIST
            Column{
            Text {
                id: init
                text: qsTr("STARTING INIT PROCESS..")
                font.bold: true
                font.pixelSize:(rect1.width/25 + rect1.height/25 )/2
            }
            }

            Column{
                    CheckBox {
                        id: c1
                        checked:myrect.check1
                        text: qsTr("Hardware Connected")
                        font.pixelSize: (rect1.width/25 + rect1.height/25 )/2
                        checkable:false
                        indicator.width:rect1.width/20
                        indicator.height:rect1.height/20

                    }
                    CheckBox {
                        id:c2
                        checked: myrect.check2
                        text: qsTr("Check if Drone is Registered")
                        font.pixelSize: (rect1.width/25 + rect1.height/25 )/2
                        checkable:false
                        indicator.width:rect1.width/20
                        indicator.height:rect1.height/20
                    }
                    CheckBox {
                        id:c3
                        checked: myrect.check3
                        text: qsTr("Check for Firmware Upgrades")
                        font.pixelSize: (rect1.width/25 + rect1.height/25 )/2
                        checkable:false
                        indicator.width:rect1.width/20
                        indicator.height:rect1.height/20
                    }
                    CheckBox {
                        id:c4
                        checked: myrect.check4
                        text: qsTr("Starting Key Rotation")
                        checkable:false
                        font.pixelSize: (rect1.width/25 + rect1.height/25 )/2
                        indicator.width:rect1.width/20
                        indicator.height:rect1.height/20
                        onCheckedChanged: {
                            if(checkable){
                                //resetChecks();
                                npntComplete();
                                tmr.start();
                            }
                        }
                    }
                    Timer{
                        id: tmr
                        interval: 2000
                        running: false
                        repeat: false
                        onTriggered: {
                            myrect.changePage(5);
                        }

                    }
            }

        }

    }
    Image {
        id: logo
        source: "/qmlimages/modifications/cropped-SpaceJamUAV-Logo-2048x663.png"
        fillMode: Image.PreserveAspectFit
        width: parent.width/3
        height: parent.height/6
    }

 }

