import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import nftMonitor

Item
{
    id:root
    property int attack:0;

    CPPMonitor
    {
        id:monitor
        onIssuerChanged:
        {
            root.attack=monitor.attack
            if(monitor.issuer==="0xqsert")
            {
                root.attack=1000;
            }
        }
        address: addrText.text
        nodeAddr: nodeText.text
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
            text:(monitor.name!=="")?monitor.name:"Player"
        }

        Image {
            id:image
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 20
            source: monitor.imgSource

            Label
            {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                text:"Attack\n "+ root.attack
                width:parent.width*0.2
                height:parent.height*0.1
                fontSizeMode:Text.Fit
            }
        }
        TextField {
            id:addrText
            Layout.fillWidth: true
            Layout.maximumWidth: 300
            Layout.alignment: Qt.AlignHCenter
            placeholderText: qsTr("Enter the address")
        }
        TextField {
            id:nodeText
            Layout.fillWidth: true
            Layout.maximumWidth: 300
            Layout.alignment: Qt.AlignHCenter
            placeholderText: qsTr("Enter the node URL")
        }

    }


}


