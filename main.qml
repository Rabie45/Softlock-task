import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Softlock task")
FileDialog {
            id: fileDialog
            title: "Select File"
            fileMode: FileDialog.OpenFile
            folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)

            onAccepted: {
                fileHandler.setFilePath(fileDialog.file)
            }
        }
    GridLayout {
        anchors.fill: parent
        anchors.margins: 16
        columnSpacing: 8

        DButton {
            id: encButtonFile
            Layout.row: 0
            Layout.fillWidth: true
            buttonText: "Encrypt File"
            onClicked: {
                if (passwordInput.isPasswordValid > 0) {
                    fileHandler.selectPassword(passwordInput.inputPassword)
                }
            }
        }
        DButton {
            id: encButtonFolder
            Layout.row: 1
            Layout.fillWidth: true
            buttonText: "Encrypt Folder"
            isFolderDialog: true
            dialogTitle: "Choose folder to encrypt"
            onClicked: {
                if (passwordInput.isPasswordValid> 0) {
                    fileHandler.selectPassword(passwordInput.inputPassword)
                }
            }
        }

        DButton {
            id: decButtonFolder
            Layout.row: 2
            Layout.fillWidth: true
            buttonText: "Decrypt Folder"
            isFolderDialog: true
            dialogTitle: "Choose folder to decrypt"
            isDecrypt: true
            onClicked: {
                if (passwordInput.isPasswordValid > 0) {
                    fileHandler.selectPassword(passwordInput.inputPassword)
                }
            }
        }

        Text{
            Layout.row: 3
            text:fileHandler.fileSize
            font.family: "Body"
            color: "Black"
        }


        InputItems {
            id: passwordInput
            labelText: "Password"
            Layout.row: 4
            Layout.fillWidth: true
            onPasswordChanged:{
                if(passwordInput.isPasswordValid>0){
                encButtonFile.isEnabled=true
                encButtonFolder.isEnabled=true
                decButtonFolder.isEnabled=true
                }
                else{
                    encButtonFile.isEnabled=false
                    encButtonFolder.isEnabled=false
                    decButtonFolder.isEnabled=false
                }
            }


        }
        InputItems {
            id: fileNameInput
            labelText: "File name"
            helperText:" "
            placeholderText: "Choose the file name"
            isPassword: false
            onFileNameChanged: {
              fileHandler.selecFileName(fileName)
            }
            Layout.row: 5
            Layout.fillWidth: true
        }
    }

}
