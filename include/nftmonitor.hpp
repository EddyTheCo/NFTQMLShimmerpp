#pragma once

#include<QObject>
#include<QString>
#include<QtQml/qqmlregistration.h>
#include"client/qclient.hpp"
#include"client/qclientMQTT.hpp"

#include <QtCore/QtGlobal>

#if defined(WINDOWS_OMONI)
# define OMONI_EXPORT Q_DECL_EXPORT
#else
#define OMONI_EXPORT Q_DECL_IMPORT
#endif

class OMONI_EXPORT CPPMonitor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString  address MEMBER m_address NOTIFY addressChanged)
    Q_PROPERTY(QString  issuer MEMBER m_issuer NOTIFY issuerChanged)
    Q_PROPERTY(QString  imgSource READ getImgSource NOTIFY imgSourceChanged)
    Q_PROPERTY(quint16  attack READ getAttack NOTIFY AttackChanged)
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
    void AttackChanged();
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
