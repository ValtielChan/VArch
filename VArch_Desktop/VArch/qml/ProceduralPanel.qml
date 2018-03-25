import QtQuick 2.2

Item {
    id: item1
    
    width: 80
    height: parent.height

    Rectangle {
        id: rectangle
        color: "#545454"
        anchors.fill: parent
    }

    ImageButton {
        id: planetButton
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 6
        sourceImage: "../icons/worlwide.svg"

        MouseArea {
	        anchors.fill: parent
	        
	        onClicked: {
	            planetPanel.visible = true;
	        }
	    }
    }

    ImageButton {
        id: terrainButton
        anchors.top: planetButton.bottom
        anchors.topMargin: 6
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 0
        label: "Terrain"
        sourceImage: "../icons/terrain.svg"
    }

    ImageButton {
        id: treeButon
        x: 0
        y: 172
        anchors.topMargin: 6
        anchors.right: parent.right
        sourceImage: "../icons/tree.svg"
        label: "Tree"
        anchors.top: terrainButton.bottom
        anchors.left: parent.left
    }


}
