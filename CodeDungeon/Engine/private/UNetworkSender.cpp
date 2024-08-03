#include "EngineDefine.h"
#include "UNetworkSender.h"
#include "UNetworkBaseController.h"
#include "ServerUtility.h"

UNetworkSender::UNetworkSender(SHPTR<UNetworkBaseController> _spNetworkBaseController) :
	m_pClientTcpSocket{NULL}, m_SendQuery{}, m_pOverExp{nullptr}
{
	m_pClientTcpSocket = _spNetworkBaseController->GetClientSocketPointer();
	m_pOverExp = Make::xnew<UOverExp>();
}

void UNetworkSender::InsertSendProcessPacketInQuery(const UProcessedData& _ProcceedData)
{
	m_SendQuery.push_back(_ProcceedData);
}

void UNetworkSender::InsertSendTcpPacketInQuery(_char* _pPacket, _short _PacketType, _short _PacketSize)
{
	m_SendQuery.push_back(UProcessedData(_pPacket, (size_t)_PacketSize, (_int)_PacketType));
}

void UNetworkSender::SendDataInQuery()
{
	for (auto& iter : m_SendQuery)
	{
		m_pOverExp->SendBufferReady(& iter.GetData()[0], iter.GetDataType(), iter.GetDataSize());
		UServerMethods::SendTcpPacket(*m_pClientTcpSocket, m_pOverExp);
	}
	m_SendQuery.clear();
}

void UNetworkSender::Free()
{
}
