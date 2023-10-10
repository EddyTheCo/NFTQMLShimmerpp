#pragma once

#include<QObject>
#include<QString>

// https://doc.qt.io/qt-6/qtqml-cppintegration-definetypes.html#preconditions
#include<QtQml/qqmlregistration.h>
// include https://eddytheco.github.io/Qclient-IOTA/
#include"client/qclient.hpp"
// include https://eddytheco.github.io/QclientMqtt-IOTA/
#include"client/qclientMQTT.hpp"



// https://doc.qt.io/qt-6/sharedlibrary.html
#include <QtCore/QtGlobal>
#if defined(WINDOWS_OMONI)
# define OMONI_EXPORT Q_DECL_EXPORT
#else
#define OMONI_EXPORT Q_DECL_IMPORT
#endif



/*!
* QML Type using the Shimmer++ libraries to show information about NFTs in certain address.
* The type expose to QML the properties 
* - address   (in) => The address to look for the last NFT.
* - nodeAddr  (in) => The URL of the node to connect
* - issuer    (out)=> The issuer (hex string) of the NFT found on the address.
* - imgSource (out)=> The `uri` inside the immutable metadata of the NFT found on the address
* - attack    (out)=> Attack coefficient calculated from the NFTId of the NFT found on the address 
* - name      (out)=> The `name` inside the metadata of the NFT found on the address
*
*/
//https://doc.qt.io/qt-6/qtqml-cppintegration-exposecppattributes.html

class OMONI_EXPORT CPPMonitor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString  address MEMBER m_address NOTIFY addressChanged)
    Q_PROPERTY(QString  issuer MEMBER m_issuer NOTIFY issuerChanged)
    Q_PROPERTY(QString  imgSource READ getImgSource NOTIFY imgSourceChanged)
    Q_PROPERTY(quint16  attack READ getAttack NOTIFY attackChanged)
    Q_PROPERTY(QString  name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QUrl  nodeAddr MEMBER m_nodeAddr NOTIFY nodeAddrChanged)

    QML_ELEMENT

public:
    CPPMonitor(QObject *parent = nullptr);

    QString getImgSource()const{return m_imgSource;}
    QString getName()const{return m_name;}
    quint16 getAttack()const{return m_attack;}


signals:
    void addressChanged();
    void imgSourceChanged();
    void attackChanged();
    void nameChanged();
    void nodeAddrChanged();
    void issuerChanged();

private:
    void restart();
    void updateValues(qiota::Node_output out);
    QString m_address,m_imgSource,m_name,m_issuer;
    quint16 m_attack;
    qiota::Client * rest_client;
    qiota::ClientMqtt* event_client;
    QUrl m_nodeAddr;
    qiota::ResponseMqtt* resp;
    qiota::Node_outputs* node_outputs_;

};
