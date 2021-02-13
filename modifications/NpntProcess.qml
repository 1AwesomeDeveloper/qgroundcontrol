import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.1

import QGroundControl 1.0
import com.NPNT_CONTROL 1.0

 Rectangle{
     id:myrect
        Component.onCompleted: {
            //start NPNT process
            npntController.deviceConnected();
        }
        NpntControl{
            id : npntController;
            onCheck1: myrect.check1 = true;
            onCheck2: myrect.check2 = true;
            onCheck3: myrect.check3 = true;
            onCheck4:{
                myrect.check4 = true;
            }
        }
        function npntComplete(){
            return check1 & check2 & check3 & check4;
        }

        function resetChecks(){
            check1 = false;
            check2 = false;
            check3 = false;
            check4 = false;
        }

        property bool complete: false
        property bool check1:false
        property bool check2:false
        property bool check3:false
        property bool check4:false
        property bool vis: false
        anchors.fill: parent
        color: "grey"
        visible: myrect.vis
        z:1

    Rectangle{
        id: rect2
        width:parent.width/3
        height:parent.height/2
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

            spacing: 20
            anchors.centerIn: rect2

           //START NPNT CHECKLIST
            Text {
                id: init
                text: qsTr("<h1>STARTING INIT PROCESS..<\h1>")
            }

            Column{
                    CheckBox {
                        id: c1
                        checked:myrect.check1
                        text: qsTr("Hardware Connected")
                        checkable:false

                    }
                    CheckBox {
                        id:c2
                        checked: myrect.check2
                        text: qsTr("Check if Drone is Registered")
                        checkable:false
                    }
                    CheckBox {
                        id:c3
                        checked: myrect.check3
                        text: qsTr("Check for Firmware Upgrades")
                        checkable:false
                    }
                    CheckBox {
                        id:c4
                        checked: myrect.check4
                        text: qsTr("Starting Key Rotation")
                        checkable:false
                        onCheckedChanged: {
                            if(npntComplete()){
                                vis = false;
                                resetChecks();
                                complete = true;
                            }
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

