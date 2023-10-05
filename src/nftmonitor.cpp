#include"nftmonitor.hpp"
#include"encoding/qbech32.hpp"

#include"QJsonDocument"
#include"QJsonObject"
#include <limits>
using namespace qiota;

CPPMonitor::CPPMonitor(QObject *parent):QObject(parent),rest_client(new Client(this)),
    event_client(new ClientMqtt(this)),m_attack(0),m_nodeAddr("https://api.testnet.shimmer.network")
{

    connect(this,&CPPMonitor::nodeAddrChanged,this,[=](){
        if(m_nodeAddr.isValid())
        {
            rest_client->set_node_address(m_nodeAddr);
            event_client->set_node_address(m_nodeAddr);
        }
    });
    connect(this,&CPPMonitor::addressChanged,this,[=](){
        restart();
    });
    restart();
}
void CPPMonitor::restart(void)
{
    static ResponseMqtt* resp=nullptr;
    static Node_outputs* node_outputs_=nullptr;

    if(!m_address.isNull()&&!m_nodeAddr.isEmpty())
    {
        const auto addr_pair=qencoding::qbech32::Iota::decode(m_address);

        if(addr_pair.second.size()&&rest_client->state())
        {
            auto info=rest_client->get_api_core_v2_info();
            connect(info,&Node_info::finished,this,[=]( ){
                if(info->bech32Hrp==addr_pair.first)
                {

                    if(resp)resp->deleteLater();
                    if(node_outputs_)node_outputs_->deleteLater();

                    node_outputs_=new Node_outputs();

                    connect(node_outputs_,&Node_outputs::finished,this,[=]( ){
                        if(node_outputs_->outs_.size())updateValues(node_outputs_->outs_.front());
                        node_outputs_->deleteLater();
                    });

                    rest_client->get_outputs<qblocks::Output::NFT_typ>(node_outputs_,"address="+m_address);

                    auto resp=event_client->get_subscription("outputs/unlock/address/"+m_address);
                    connect(resp,&ResponseMqtt::returned,this,[=](auto data){
                        updateValues(Node_output(data));
                    });
                }
                info->deleteLater();
            });
        }

    }

}

void CPPMonitor::updateValues(qiota::Node_output out)
{

    if(out.output()->type()==qblocks::Output::NFT_typ)
    {
        auto nft_output=std::static_pointer_cast<const qblocks::NFT_Output>(out.output());
        auto NFTID=nft_output->get_id();
        auto buffer=QDataStream(&NFTID,QIODevice::ReadOnly);
        buffer.setByteOrder(QDataStream::LittleEndian);
        quint64 atackInd;
        buffer>>atackInd;
        m_attack=atackInd*1000.0/std::numeric_limits<quint64>::max();
        emit AttackChanged();

        auto metdataFeat=nft_output->get_feature_(qblocks::Feature::Metadata_typ);
        if(metdataFeat)
        {
            auto metdata=std::static_pointer_cast<const qblocks::Metadata_Feature>(metdataFeat);
            auto data=metdata->data();

            QJsonDocument document = QJsonDocument::fromJson(metdata->data());
            if(!document.isNull())
            {
                QJsonObject rootObj = document.object();
                if(!rootObj["name"].isUndefined()&&rootObj["name"].isString())
                {
                    m_name=rootObj["name"].toString();
                    emit nameChanged();
                }
            }

        }
        auto ImmetdataFeat=nft_output->get_immutable_feature_(qblocks::Feature::Metadata_typ);
        if(ImmetdataFeat)
        {
            auto metdata=std::static_pointer_cast<const qblocks::Metadata_Feature>(ImmetdataFeat);
            auto data=metdata->data();

            QJsonDocument document = QJsonDocument::fromJson(metdata->data());
            if(!document.isNull())
            {
                QJsonObject rootObj = document.object();
                if(!rootObj["uri"].isUndefined()&&rootObj["uri"].isString())
                {
                    m_imgSource=rootObj["uri"].toString();
                    emit imgSourceChanged();
                }
            }

        }
        auto IssuerFea=nft_output->get_immutable_feature_(qblocks::Feature::Issuer_typ);
        if(IssuerFea)
        {
            auto m_issuer=std::static_pointer_cast<const qblocks::Issuer_Feature>(IssuerFea)
                                ->issuer()->addr().toHexString();
        }
        emit issuerChanged();
    }
}



