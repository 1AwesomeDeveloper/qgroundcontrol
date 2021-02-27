/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/
import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

import QGroundControl               1.0
import QGroundControl.Controls      1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FactControls  1.0
import QGroundControl.Palette       1.0
import QGroundControl.Controllers   1.0
import QGroundControl.ScreenTools   1.0
import SpaceJam 1.0

Rectangle{
    id:myrect
    color:"grey"
    anchors.fill: parent
//    z:1
    signal changePage(int page)
    Component.onCompleted: {
        display.append("Please wait while connecting to servers...")
        downloadController.getLatestFirmware()
    }
    property bool vis:false

    readonly property int _defaultFimwareTypePX4:   12
    readonly property int _defaultFimwareTypeAPM:   3
    property var    _firmwareUpgradeSettings:   QGroundControl.settingsManager.firmwareUpgradeSettings
    property var    _defaultFirmwareFact: _firmwareUpgradeSettings.defaultFirmwareType
    property bool   _defaultFirmwareIsPX4:      true
    property string firmwareWarningMessage
    property bool   firmwareWarningMessageVisible:  false
    property bool   initialBoardSearch:             true
    property string firmwareName
    property bool _singleFirmwareMode:          QGroundControl.corePlugin.options.firmwareUpgradeSingleURL.length != 0   ///< true: running in special single firmware download mode
    property string _path
    visible: myrect.vis
    SJFirmwareController{
        id: downloadController
        onFirmwareReady: {
            _path = path
            controller.startBoardSearch()
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


       TextArea{
           id: display
           width: parent.width - 50
           height: parent.height - 50
           x: parent.width/10
           y: parent.width/20
           font.bold: true
           font.pixelSize: (parent.width/20 + parent.height/20 )/2
           wrapMode: Text.WordWrap
           readOnly: true
           backgroundVisible:false
           frameVisible:false
       }

       ProgressBar {
           id:                     progressBar
           width:                  parent.width - parent.width/10
           visible:                true
           anchors.centerIn:       parent
           anchors.verticalCenterOffset: 50
       }

    }

    Image {
        id: logo
        source: "/qmlimages/modifications/cropped-SpaceJamUAV-Logo-2048x663.png"
        fillMode: Image.PreserveAspectFit
        width: parent.width/3
        height: parent.height/6
    }

    function cancelFlash() {
      display.text = "Upgrade cancelled !!"
      controller.cancel()
    }


    FirmwareUpgradeController {
        id:             controller
        onError: display.text = "Upgrade Failed"
        progressBar:    progressBar
        //statusLog:      display

        property var activeVehicle: QGroundControl.multiVehicleManager.activeVehicle

        onActiveVehicleChanged: {
            if (!globals.activeVehicle) {
                display.text = "Please unplug your board and plug back in"
            }
        }

        onNoBoardFound: {
            initialBoardSearch = false
            if (!QGroundControl.multiVehicleManager.activeVehicleAvailable) {
                display.text = "Plug in your device via USB to start firmware upgrade"
            }
        }

        onBoardGone: {
            initialBoardSearch = false
            if (!QGroundControl.multiVehicleManager.activeVehicleAvailable) {
               display.text = "Plug in your device via USB to start firmware upgrade"
            }
        }

        onBoardFound: {
            if (initialBoardSearch) {
            // Board was found right away, so something is already plugged in before we've started upgrade
            display.text = "Please unplug your device and then plug back in to start firmware upgrade"
            var availableDevices = controller.availableBoardsName()
            if (availableDevices.length > 1) {
                display.text = "Multiple devices detected !!"
            }
            if (QGroundControl.multiVehicleManager.activeVehicle) {
                QGroundControl.multiVehicleManager.activeVehicle.vehicleLinkManager.autoDisconnect = true
            }
        } else {
            // We end up here when we detect a board plugged in after we've started upgrade
            display.text = "Found device !!"
        }
        }

        onShowFirmwareSelectDlg: {
            console.log("Firmware path: " + _path)
            //changePage(3)
            controller.flashFirmwareUrl(_path)
        }

        onEraseStarted:{
            display.append("")
            display.append("Erasing Old Firmware...")
        }

        onEraseComplete: {
            display.append("Flashing New Firmware...")
        }

        onFlashComplete: {
            display.append("")
            display.append("Flash Complete!")
            downloadController.deleteFirmwareFile(_path)
            changePage(3)
        }
    }

}


