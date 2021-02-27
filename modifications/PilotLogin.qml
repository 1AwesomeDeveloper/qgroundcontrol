import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.1
import QGroundControl 1.0
import SpaceJam 1.0
Rectangle{

        id:myRect
        color:"grey"
        anchors.fill: parent
        z:1
        visible: true;
        signal changePage(int page)
        SJPilotPageController{
            id: controller

            onLoginSuccessfull: {
                changePage(6);
            }
        }

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
                   text: qsTr("Remote-Pilot Email")
                   font.pixelSize: (rect1.width/25 + rect1.height/25 )/2
                   font.bold: true

               }

               TextField{
                   id:pilotusername
                   text: controller.userName
                   placeholderText: qsTr("Enter Email")
                   width: rect1.width/2
                   height:rect1.height/10
                   onTextChanged:{
                       controller.userName = pilotusername.text

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
                   id:pilotpassword
                   placeholderText: qsTr("Enter Password")
                   echoMode:"Password"
                   text: controller.passWord
                   width: rect1.width/2
                   height:rect1.height/10
                   onTextChanged:{
                       controller.passWord = pilotpassword.text
                   }

           }
               }

               Rectangle{
                   id:mybtn
                   width:pilotusername.width/3
                   height:pilotusername.height
                   color:"lightgray"
                   visible: true
                   anchors.centerIn:rect1
                   anchors.verticalCenterOffset: rect1.height/2
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
                           controller.okButton = true

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


