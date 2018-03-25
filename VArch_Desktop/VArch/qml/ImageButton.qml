import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.2

import "./"

Item {

    id: imageButton

    width: parent.width
    height: width

    property string sourceImage : "../icons/worlwide.svg"
    property string label : "Planet"
    property color backgroundColor : "#464646"
    property color hoverColor : VColors.darkGreen

    Rectangle {
        id: buttonRectangle
        anchors.fill: parent

        color: backgroundColor
    }

    Image {
        id: buttonImage
        x: 72
        y: 7
        width: parent.width * 0.6
        height: width
        clip: false
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 7
        source: sourceImage
    }

    Text {
        id: buttonLabel
        x: 72
        y: 280
        width: parent.width * 0.6
        height: parent.height * 0.2
        color: "#ffffff"
        text: qsTr(label)
        font.family: "Verdana"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        font.pixelSize: height
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onEntered: {
            buttonRectangle.color = hoverColor;
        }

        onExited: {
            buttonRectangle.color = backgroundColor;
        }
    }
}
