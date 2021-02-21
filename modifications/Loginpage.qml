import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.1

import QGroundControl 1.0
import QGroundControl.Controls      1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FactControls  1.0
import QGroundControl.Palette       1.0
import QGroundControl.Controllers   1.0
import QGroundControl.ScreenTools   1.0

Rectangle{

        id:myrect
        color:"grey"
        anchors.fill: parent
        z:1
        property var    _activeVehicle:     QGroundControl.multiVehicleManager.activeVehicle
        property bool   _communicationLost: _activeVehicle ? _activeVehicle.vehicleLinkManager.communicationLost : false
        property string usrname
        property string passwrd
        //signal firmwarebtn()
        signal getUsername()
        signal getPassword()
        signal changeUsername(string a)
        signal changePassword(string b)
        signal loginButton()
        property bool vis:true
        visible: myrect.vis

Rectangle{
           id: rect1
           visible: true
           width:parent.width/2.5
           height:parent.height/1.5
           anchors.centerIn: parent
           radius: 80
           color:"lightgrey"
           border.color:"black"
           border.width: 3



           Column{
               spacing: rect1.height/20
               anchors.centerIn: rect1

               Column{
               Text {
                   id: username
                   text: qsTr("Customer Email")
                   font.pixelSize: (rect1.width/25 + rect1.height/25 )/2
                   font.bold: true

               }

               TextField{
                   id:customerusername
                   text: usrname
                   placeholderText: qsTr("Enter Email")
                   width: rect1.width/2
                   height:rect1.height/10
                   anchors.topMargin: customerusername.height/2
                   onTextChanged:{

                       usrname = text
                       changeUsername(text);
                       getUsername()

               }
               }

               }
               Column{

               Text {
                   id: password
                   text: qsTr("Password")
                   font.bold: true
                   font.pixelSize: (rect1.width/25 + rect1.height/25 )/2

               }

               TextField{
                   id:customerpassword
                   placeholderText: qsTr("Enter Password")
                   echoMode:"Password"
                   text: passwrd
                   width: rect1.width/2
                   height:rect1.height/10
                   anchors.topMargin: customerusername.height/2
                   onTextChanged:{
                       passwrd = text
                       changePassword(text)
                       getPassword()    
                   }

           }
               }

               Rectangle{
                   id:mybtn
                   width:customerusername.width/3
                   height:customerusername.height
                   color:"lightgray"
                   visible: true
                   border.color: "black"
                   border.width: 1

                   Text {
                       id: text
                       text: qsTr("Login")
                       font.bold: true
                       font.pixelSize: ((mybtn.width/5)+(mybtn.height/5))/2
                       anchors.centerIn:mybtn
                   }


                   MouseArea{
                       id:btnArea
                       anchors.fill: mybtn
                       hoverEnabled: true
                       onClicked:{
                           loginButton()

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

//Button{
//    id:firmwareUpgradebutton
//    text: "Upgrade Firmware"
//    anchors.right: myrect.right
//    onClicked: firmwarebtn()

//}


}


