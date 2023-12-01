import QtQuick 2.15
import QtQuick.Controls 2.15
import QtLocation 5.15
import QtPositioning 5.15


ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Map Application"

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(45.000, 20.000)
        zoomLevel: 14
    }

    Connections {
        target: mainWindowObject
        onSetCenterHelper: {
            console.log("setCenter received:", latitude, longitude);
            // Add your logic here
        }
        onAddMarkerHelper: {
            console.log("addMarker received:", latitude, longitude);
            // Add your logic here
        }
    }
}
