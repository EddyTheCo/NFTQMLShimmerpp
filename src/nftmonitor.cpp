#include"nftmonitor.hpp"

//include https://eddytheco.github.io/Qbech32/
#include"encoding/qbech32.hpp"

#include"QJsonDocument"
#include"QJsonObject"
#include <limits>
using namespace qiota;


/*
 * Constructor of the QML Type.
 */
CPPMonitor::CPPMonitor(QObject *parent):QObject(parent),rest_client(new Client(this)),
	event_client(new ClientMqtt(this)),m_attack(0),m_nodeAddr("https://api.testnet.shimmer.network"),
		resp(nullptr),node_outputs_(nullptr)
{
	//  Will use the public endpoint of the IOTA Foundation if no nodeAddr is set.
	rest_client->set_node_address(m_nodeAddr);
	event_client->set_node_address(m_nodeAddr);
	// If the nodeAddr change set the node address of the clients.
	connect(this,&CPPMonitor::nodeAddrChanged,this,[=](){
			if(m_nodeAddr.isValid())
			{
			rest_client->set_node_address(m_nodeAddr);
			event_client->set_node_address(m_nodeAddr);
			}
			});
	// If the address change or the client found a healthy node restart the properties of the type
	connect(this,&CPPMonitor::addressChanged,this,&CPPMonitor::restart);
	connect(rest_client,&qiota::Client::stateChanged,this,&CPPMonitor::restart);
}

/*!
 *  Check again for the last NFT output on the m_address from the node m_nodeAddr.
 */
void CPPMonitor::restart(void)
{
	// If the node is healthy and the m_address is set
	if(!m_address.isNull()&&rest_client->state())
	{
		// get the hrp and data of the bech32 encoded address
		const auto addr_pair=qencoding::qbech32::Iota::decode(m_address);

		// if the is a valid bech32 encoded address
		if(addr_pair.second.size())
		{

			// Get the node info https://wiki.iota.org/tips/tips/TIP-0025/
			auto info=rest_client->get_api_core_v2_info();

			// When the info is returned run the lambda function
			connect(info,&Node_info::finished,this,[=]( ){

					//Check that the hrp of the address match the hrp of the node
					if(info->bech32Hrp==addr_pair.first)
					{

					// Deleting previous objects remove the connections
					// So if the QML type is restarted because the address to monitor changed
					// We stop receiving events from the Event API using the previous address.
					if(resp)resp->deleteLater();
					if(node_outputs_)node_outputs_->deleteLater();

					// Create and object that holds the NFT outputs returned by the node
					node_outputs_=new Node_outputs();

					// When the node returns the NFT outputs execute this
					connect(node_outputs_,&Node_outputs::finished,this,[=]( ){
							// If there are NFT outputs on the address update the values of the QML type
							if(node_outputs_->outs_.size())updateValues(node_outputs_->outs_.front());
							node_outputs_->deleteLater();
							});

					// Get the NFT outputs according to '/api/indexer/v1/outputs/nft' https://wiki.iota.org/tips/tips/TIP-0026/
					rest_client->get_outputs<qblocks::Output::NFT_typ>(node_outputs_,"address="+m_address);


					// Subscribe to `outputs/unlock/{condition}/{address}` https://wiki.iota.org/tips/tips/TIP-0028/
					// When new outputs are created with m_address as address unlock condition execute this
					resp=event_client->get_subscription("outputs/unlock/address/"+m_address);
					connect(resp,&ResponseMqtt::returned,this,[=](auto data){
							//Update the values of the QML type.
							updateValues(Node_output(data));
							});
					}
					info->deleteLater();
			});
		}

	}

}
/*!
 *
 * Update the values of the QML Type by analyzing the NFT output
 *
 */
void CPPMonitor::updateValues(qiota::Node_output out)
{

	// Check if is a NFT output because the event API can be triggered by any output
	if(out.output()->type()==qblocks::Output::NFT_typ)
	{
		//Now that we are sure it is a NFT output cast the pointer from Output to NFT_Output 
		auto nft_output=std::static_pointer_cast<qblocks::NFT_Output>(out.output());

		//If the NFT id is zeroed out set the implicit NFTid by using the OutputId.
		if(nft_output->get_id()==qblocks::c_array(32,0))
		{
			nft_output->set_id(out.metadata().outputid_);
		}
		// From the NFTID calculate an attack coefficient.
		auto NFTID=nft_output->get_id();
		auto buffer=QDataStream(&NFTID,QIODevice::ReadOnly);
		buffer.setByteOrder(QDataStream::LittleEndian);
		quint64 atackInd;
		buffer>>atackInd;
		m_attack=atackInd*1000.0/std::numeric_limits<quint64>::max();
		emit attackChanged();

		// Get the metadata feature of the Output, if there is a name field update the name property of the QObject.
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
		// Get the immutable metadata feature of the Output, if there is a uri field update the imgSource property of the QObject.
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
		// Get the issuer feature of the Output, if set update the issuer property of the QObject.
		auto IssuerFea=nft_output->get_immutable_feature_(qblocks::Feature::Issuer_typ);
		if(IssuerFea)
		{
			m_issuer=std::static_pointer_cast<const qblocks::Issuer_Feature>(IssuerFea)
				->issuer()->addr().toHexString();
		}
		//emit the signal in order for QML side to know that the property of the element has changed.
		emit issuerChanged();
	}
}



