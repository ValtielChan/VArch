import QtQuick 2.0

Rectangle {
    id: page
    width: 1080
    height: 720
    color: "#303030"

    Text {
        id: helloText
        text: "Lourde 3D par là"
        anchors.horizontalCenterOffset: -265
        y: 226
        anchors.horizontalCenter: page.horizontalCenter
        font.pointSize: 24; font.bold: true
    }

    ProceduralPanel {
        id: proceduralPanel
        x: 1000
        y: 0
        width: 80
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.verticalCenter: parent.verticalCenter
        anchors.top: parent.top
    }

    PlanetPanel {
        id: planetPanel
        x: 799
        y: 0
        anchors.right: proceduralPanel.left
        anchors.rightMargin: 0
        visible: true
    }
}
