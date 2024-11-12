// DButton.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs
Rectangle {
    id: root


    property string buttonText: "Default"
    property string leftIconSource: ""
    property string rightIconSource: ""
    property string spaceToSave: ""
    property bool isEnabled: false
    property bool isFolderDialog: false
    property var nameFilters: ["All files (*)"]
    property string dialogTitle: "Please choose a file"
    property bool isDecrypt: false
    // Signals
    signal clicked()
    signal fileSelected(string filePath)
    signal folderSelected(string folderPath)


    Layout.fillWidth: true
    height: 56
    radius: 14

    // Color states
    color: {
        if (!isEnabled) return "#E0E0E0"
        if (mouseArea.pressed) return "#0052CC"
        return "#0066FF"
    }


    border.color: "#FF6B00" // Orange border
    border.width: mouseArea.containsMouse ? 2 : 0


    RowLayout {
        anchors.centerIn: parent
        spacing: 8

        // Left Icon
        Image {
            id: leftIcon
            source: root.leftIconSource
            width: 18
            height: 18
            visible: root.leftIconSource !== ""
            Layout.alignment: Qt.AlignVCenter
        }

        // Button text
        Text {
            text: root.buttonText
            font.family: "Body"
            font.weight: Font.Medium
            color: root.isEnabled ? "white" : "#999999"
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignVCenter
            leftPadding: leftIcon.visible ? 0 : 16
            rightPadding: rightIcon.visible ? 0 : 16
        }

        // Right Icon
        Image {
            id: rightIcon
            source: root.rightIconSource
            width: 18
            height: 18
            visible: root.rightIconSource !== ""
            Layout.alignment: Qt.AlignVCenter
        }
    }
    FileDialog {
        id: fileDialog
        title: root.dialogTitle
        nameFilters: root.nameFilters
        fileMode: isFolderDialog ? FileDialog.OpenFolder : FileDialog.OpenFile
        currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]

        onAccepted: {
            if (isFolderDialog) {
                const folderPath = currentFolder.toString()
                root.spaceToSave = folderPath
                root.folderSelected(folderPath)
            } else {
                const filePath = selectedFile.toString()
                root.greeting = filePath
                root.fileSelected(filePath)
                fileHandler.getFileSize(filePath)
            }
        }
    }

    // Mouse area for handling interactions
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        enabled: root.isEnabled

        onClicked: {
                if (root.isEnabled) {
                    if (isFolderDialog) {
                        if (isDecrypt) {
                            fileHandler.selectFolderDecrypt()
                        } else {
                            fileHandler.selectFolder()
                        }
                    } else {
                        if (isDecrypt) {
                            fileHandler.selectFileDecrypt()
                        } else {
                            fileHandler.selectFile()
                        }
                    }
                    root.clicked()
                }

            }
    }

    Behavior on border.width {
        NumberAnimation { duration: 150 }
    }
}
