import QtQuick          2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts  1.11
import QtQuick.Dialogs  1.3

import QGroundControl                       1.0
import QGroundControl.Controls              1.0
import QGroundControl.Palette               1.0
import QGroundControl.MultiVehicleManager   1.0
import QGroundControl.ScreenTools           1.0
import QGroundControl.Controllers           1.0
import com.SJFTPUploadButton                1.0

import "qrc:/qml/modifications"
import "qrc:/qmlimages/modifications/"
Rectangle{
    id : abc
    property bool vis:false
    Layout.fillHeight: true
    height: parent.height
    width: height
    visible: vis

    QGCFileDialog {
        id:                 filePathDialog
        title:              qsTr("Select Permission Artifact File")
        nameFilters:        [qsTr("Permission Artifact File (*.xml *.json)"), qsTr("All Files (*)")]
        selectExisting:     true
        folder:             QGroundControl.settingsManager.appSettings.logSavePath

        onAcceptedForLoad: {
            controller.uploadPA(file)
            close()
        }
    }
    SJFTPUploadController{
        id: controller
    }

    QGCButton {
        height: parent.height
        width: height
        id:                 uploadButton
        text:               qsTr("Upload")
        onClicked:          filePathDialog.openForLoad();
        visible:           parent.visible
    }
}
