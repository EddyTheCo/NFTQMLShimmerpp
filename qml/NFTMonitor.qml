import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import nftMonitor

Item
{
    id:root
    property int attack:0;
    property alias nodeAddr:monitor.nodeAddr
    CPPMonitor
    {
        id:monitor
        onIssuerChanged:
        {
            root.attack=monitor.attack
            if(monitor.issuer==="0x000b22cdeed839e4df23def46f7c2e8d04d3b66aab30b2695c7e9cbf21e9ef93cb")
            {
                root.attack=1000;
            }
            console.log(monitor.issuer);
            console.log(monitor.imgSource);
            console.log(monitor.name);
            console.log(monitor.attack);
        }
        address: addrText.text

    }
    ColumnLayout
    {
        anchors.fill: parent
        Label
        {
            Layout.fillWidth: true
            Layout.maximumWidth: 200
            Layout.alignment: Qt.AlignHCenter
            horizontalAlignment:Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide:Text.ElideRight
            text:((monitor.name!=="")?monitor.name:qsTr("Player"))+"\n Attack:" + root.attack
            color:"white"
            background: Rectangle
            {
                color:"black"
                border.width:1
                border.color:"white"
                radius:Math.min(width,height)*0.05
            }
        }

        Image {
            id:image
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 20
            source: monitor.imgSource
            fillMode: Image.PreserveAspectFit
        }
        TextField {
            id:addrText
            Layout.fillWidth: true
            Layout.maximumWidth: 300
            Layout.alignment: Qt.AlignHCenter
            placeholderText: qsTr("Enter the address")
        }

    }


}


