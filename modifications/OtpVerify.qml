import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1

import QGroundControl 1.0

Rectangle{

    property var    _activeVehicle:     QGroundControl.multiVehicleManager.activeVehicle
    property bool   _communicationLost: _activeVehicle ? _activeVehicle.vehicleLinkManager.communicationLost : false
    property bool vis: false
    signal backButtonClicked()
    property string otpVal
    signal getOtpVal()
    signal setOtp(string a)
    signal verifyButton()
    id:myrect
    anchors.fill: parent
    color: "grey"
    z:1
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
                   spacing: rect1.height/5
                   anchors.centerIn: rect1

                   Column{

                   Text {
                       id: otptxt
                       text: qsTr("Enter the OTP sent to your Registered Email")
                       font.pixelSize: (rect1.width/25 + rect1.height/25 )/2
                       font.bold: true

                   }

                   }

                   Column{
                   anchors.horizontalCenter: parent.horizontalCenter
                   TextField{
                       id:otpedit
                       text: otpVal
                       placeholderText: qsTr("Enter OTP")
                       width: rect1.width/2
                       height:rect1.height/10
                       anchors.topMargin: otptxt.height/2
                       onTextChanged: {
                               otpVal = text;
                               setOtp(text);
                               getOtpVal();
                       }
                   }

                   }


                   Column{

                       anchors.horizontalCenterOffset: rect1.width/2

                       Row{
                           spacing: rect1.width/4

                           Rectangle{
                               id:mybtn1
                               width:otptxt.width/3
                               height:otptxt.height
                               color:"lightgray"
                               visible: true
                               border.color: "black"
                               border.width: 1

                               Text {
                                   id: text1
                                   text: qsTr("Verify")
                                   font.bold: true
                                   font.pixelSize: ((mybtn1.width/5)+(mybtn1.height/5))/2
                                   anchors.centerIn:mybtn1
                               }


                               MouseArea{
                                   id:btn1Area
                                   anchors.fill: mybtn1
                                   hoverEnabled: true
                                   onClicked:{
                                       verifyButton()

                                   }

                               }

                           }

                           Rectangle{
                               id:mybtn2
                               width:otptxt.width/3
                               height:otptxt.height
                               color:"lightgray"
                               visible: true
                               border.color: "black"
                               border.width: 1

                               Text {
                                   id: text2
                                   text: qsTr("Back")
                                   font.bold: true
                                   font.pixelSize: ((mybtn2.width/5)+(mybtn2.height/5))/2
                                   anchors.centerIn:mybtn2
                               }


                               MouseArea{
                                   id:btn2Area
                                   anchors.fill: mybtn2
                                   hoverEnabled: true
                                   onClicked:{
                                       backButtonClicked()

                                   }

                               }

                           }


                       }


                   }

        }
    }




//    TextField{
//        id:otpedit
//        placeholderText: qsTr("Enter OTP")
//        text : otpVal
//        onTextChanged: {
//            otpVal = text;
//            setOtp(text);
//            getOtpVal();
//        }
//    }
//   }

//   Column{
//        spacing: 10

//    RowLayout{
//    id: btnrow
//    spacing:rect3.width/10

//    Button{
//        id: verifyOTP
//        text: "Verify"
//        font.bold: true
//        onClicked: verifyButton()
//        width: rect3.width/5

//}

//    Button{
//        id: backbtn
//        text: "Back"
//        font.bold:true
//        onClicked: backButtonClicked()
//    }

//       }
//   }

//}
//}

Image {
    id: logo
    source: "/qmlimages/modifications/cropped-SpaceJamUAV-Logo-2048x663.png"
    fillMode: Image.PreserveAspectFit
    width: parent.width/3
    height: parent.height/6
}
}
