import QtQuick.Controls
import QtQuick
import QtQuick.Layouts

import NFTMonitor

ApplicationWindow {
    visible: true
    id:window
    background: Rectangle
    {
        color:"#10141c"
    }
    ColumnLayout
    {
        anchors.fill:parent
        TextField {
            id:nodeText
            Layout.fillWidth: true
            Layout.maximumWidth: 300
            Layout.alignment: Qt.AlignHCenter
            placeholderText: qsTr("Enter the node URL")
        }

        RowLayout
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            QMLMonitor
            {
                id:playerA
                Layout.fillHeight: true
                Layout.fillWidth:true
                nodeAddr:nodeText.text
                Label
                {
                    anchors.centerIn: parent
                    width:parent.width*0.3
                    height:parent.height*0.3
                    horizontalAlignment:Text.AlignHCenter
                    verticalAlignment:Text.AlignVCenter
                    text:qsTr("Winner!")
                    fontSizeMode:Text.Fit
                    font.pointSize : 20.0
                    visible:(playerA.attack>playerB.attack)
                    background: Rectangle
                    {
                        color:"black"
                        border.width:1
                        border.color:"white"
                        radius:Math.min(width,height)*0.05
                    }
                }
            }
            QMLMonitor
            {
                id:playerB
                Layout.fillHeight: true
                Layout.fillWidth:true
                nodeAddr:nodeText.text
                Label
                {
                    anchors.centerIn: parent
                    width:parent.width*0.3
                    height:parent.height*0.3
                    horizontalAlignment:Text.AlignHCenter
                    verticalAlignment:Text.AlignVCenter
                    font.pointSize : 20.0
                    text:qsTr("Winner!")
                    visible:(playerB.attack>playerA.attack)
                    background: Rectangle
                    {
                        color:"black"
                        border.width:1
                        border.color:"white"
                        radius:Math.min(width,height)*0.05
                    }
                }
            }
        }
    }



}
