import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root
    width: 140
    height: 80

    // Properties
    property string labelText: "Password"
    property string placeholderText: "Enter password"
    property string helperText: "At least 8 characters long, 1 upper and lower case letter, 1 number and 1 special character (like !@#)"
    property bool isPassword: true
    property alias inputPassword: textField.text
    property bool isPasswordValid: false

    // Signals
    signal passwordEntered(string newPassword)
    signal passwordChanged(string newPassword)
    signal fileNameChanged(string fileName)
    Layout.fillWidth: true

    function getStrengthColor(password) {
        if (!password) return "#E0E0E0"
        if (password.length < 8) return "#FF0000"
        if (!root.isPasswordValid) return "#FFA500"
        return "#4CAF50"
    }

    function getHelperText(password) {
        if (!password) return root.helperText

        var missing = []
        if (!/[a-z]/.test(password)) missing.push("lowercase letter")
        if (!/[A-Z]/.test(password)) missing.push("uppercase letter")
        if (!/\d/.test(password)) missing.push("number")
        if (!/[@$!%*?&]/.test(password)) missing.push("special character")
        if (password.length < 8) missing.push("8 characters")

        if (missing.length === 0) return "Password meets all requirements"
        return "Missing: " + missing.join(", ")
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 24

        Rectangle {
            id: inputFieldContainer
            Layout.fillWidth: true
            height: 56
            color: "transparent"

            Rectangle {
                id: inputField
                anchors.fill: parent
                border.width: textField.focus ? 2 : 0.5
                border.color: textField.focus ? "#0066FF" : "#E0E0E0"
                radius: 8
                color: "white"

                Text {
                    id: floatingLabel
                    text: root.labelText
                    color: textField.focus ? "#0066FF" : "#666666"
                    font.pixelSize: 12
                    anchors.bottom: parent.top
                    anchors.bottomMargin: 9
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                }

                TextField {
                    id: textField
                    anchors.fill: parent
                    anchors.margins: 0
                    leftPadding: 20
                    rightPadding: 46
                    topPadding: 17
                    bottomPadding: 17
                    font.family: "Body"
                    font.pixelSize: 16
                    placeholderText: root.placeholderText
                    echoMode: root.isPassword ? TextInput.Password : TextInput.Normal

                    validator: RegularExpressionValidator {
                        regularExpression: root.isPassword ?
                            /^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[@$!%*?&])[A-Za-z\d@$!%*?&]{8,}$/ :
                            /.*/
                    }

                    onTextChanged: {
                        if(isPassword) {
                            var hasLowerCase = /[a-z]/.test(text)
                            var hasUpperCase = /[A-Z]/.test(text)
                            var hasNumber = /\d/.test(text)
                            var hasSpecial = /[@$!%*?&]/.test(text)
                            var isLongEnough = text.length >= 8

                            root.isPasswordValid = hasLowerCase && hasUpperCase &&
                                                 hasNumber && hasSpecial && isLongEnough

                            strengthIndicator.color = root.getStrengthColor(text)
                            root.passwordChanged(text)
                        } else {
                            root.fileNameChanged(text)

                            console.log(text)
                        }
                    }

                    background: Item {}
                }

                Image {
                    id: visibilityIcon
                    width: 18
                    height: 18
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                    anchors.verticalCenter: parent.verticalCenter
                    visible: root.isPassword
                }

                Rectangle {
                    id: strengthIndicator
                    height: 4
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: helperText.top
                    anchors.bottomMargin: 4
                    color: "#E0E0E0"
                    visible: root.isPassword
                }

                Text {
                    id: helperText
                    text: {
                        if(isPassword)
                        root.getHelperText(textField.text)

                    }
                    color: root.isPasswordValid ? "#4CAF50" : "#666666"
                    font.pixelSize: 12
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.top: parent.bottom
                    anchors.topMargin: 8
                }
            }
        }
    }
}
