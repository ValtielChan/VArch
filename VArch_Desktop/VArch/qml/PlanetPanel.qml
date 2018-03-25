import QtQuick 2.0

Item {

    id: rootPlanetPanel
    width: 200
    height: parent.height

    Rectangle {
        color: "#303030"
        anchors.fill: parent

        Rectangle {
            id: rectangle
            x: 8
            y: 8
            width: 184
            height: 200
            color: "#494949"

            Text {
                id: text1
                x: 8
                y: 8
                color: "#ffffff"
                text: qsTr("Perlin Noise")
                font.bold: true
                font.pixelSize: 12
            }

            Column {
                id: column
                x: 8
                y: 28
                width: 168
                height: 164
                spacing: 6

                Text {
                    id: text2
                    color: "#ffffff"
                    text: qsTr("Frequency")
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 12
                }

                TextInput {
                    id: textInput
                    width: 80
                    height: 20
                    color: "#bfbfbf"
                    text: qsTr("Text Input")
                    renderType: Text.QtRendering
                    font.family: "Verdana"
                    font.bold: false
                    selectionColor: "#229b00"
                    font.pixelSize: 12
                }

            }
        }
    }
}
