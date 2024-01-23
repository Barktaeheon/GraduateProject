#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USESSION_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USESSION_H

#include "UObject.h"


BEGIN(Core)
class UService;
class UTransform;
class UCollider;

/*
@ Date: 2023-01-22, Writer: ������
@ Explain
- Ŭ���̾�Ʈ�� ����ϱ� ���� Session Ŭ�����̰� �����Լ��� Server �ַ�ǿ��� �ش� Ŭ������ �θ�� �ڽ� Ŭ������ 
�������־�� �Ѵ�.
*/
class CORE_DLL USession abstract : public UObject  {
public:
	using BUFFER = ARRAY<_char, MAX_BUFFER_LENGTH>;
	using TOTALBUFFER = ARRAY<_char, MAX_PROCESSBUF_LENGTH>;
public:
	USession(OBJCON_CONSTRUCTOR, MOVE TCPSOCKET _TcpSocket, SHPTR<UService> _spService,  SESSIONID _ID, SESSIONTYPE _SessionType);
	DESTRUCTOR(USession)
public: 
	virtual _bool Start() PURE;
	// Ŭ���̾�Ʈ���� ���۵� ���۸� �д� �Լ�
	virtual void ReadData() PURE;
	// Ŭ���̾�Ʈ���� ���۸� �����Ͽ� �����ϴ� �Լ�
	virtual _bool WriteData(_char* _pPacket, const PACKETHEAD& _PacketHead) PURE;
	virtual void Disconnect() PURE;
	virtual void ConnectTcpSocket() PURE;

	/*
	@ Date: 2023-01-04, Writer: ������
	@ Explain
	- ProtocolBuffer�� �����ϱ� ���� �Լ��̴�. 
	*/
	template<class T>
	void CombineProto(REF_IN BUFFER& _Buffer, REF_IN PACKETHEAD& _PacketHead, const T& _data, short _tag)
	{
		_data.SerializePartialToArray((void*)&_Buffer[0], static_cast<int>(_data.ByteSizeLong()));
		short size = static_cast<short>(_data.ByteSizeLong());
		_PacketHead = PACKETHEAD{ size, _tag };
	}
	/*
	@ Date: 2023-01-05, Writer: ������
	@ Explain
	- ProtocolBuffer�� TCP�� ����� ��뿡�� ������ ���� ���ø� �Լ��̴�. 
	*/
	template<class T>
	requires CheckProtoType<T>
	void SendProtoData(const T& _data, short _tag)
	{
		static thread_local BUFFER Buffer;
		Core::PACKETHEAD PacketHead;
		CombineProto<T>(REF_OUT Buffer, REF_OUT PacketHead, _data, _tag);
		WriteData(&Buffer[0], PacketHead);
	}
public: /*Get Set */
	const SESSIONID GetSessionID() const { return m_SessionID; }
	const SESSIONTYPE GetSessionType() const { return m_SessionType; }
	TCPSOCKET& GetTcpSocket(REF_RETURN) { return m_TcpSocket; }
	SHPTR<UService> GetService() const { return m_wpService.lock(); }

	SHPTR<UTransform> GetTransform() const { return m_spTransform; }
	SHPTR<UCollider> GetCollider() const { return m_spCollider; }

	const _bool IsConnected() const { return m_isConnected.load(); }
protected:
	void PacketCombine(_char* _pPacket, _llong _Size);
	virtual _bool ProcessPacket(_char* _pPacket, const PACKETHEAD& _PacketHead) PURE;
	void CombineSendBuffer(_char* _pPacket, const PACKETHEAD& _PacketHead);
	void Leave();

	void CreateCollider(COLLIDERTYPE _ColliderType, const Vector3& _vCenter, const Vector3& _vScale);

	BUFFER& GetSendBuff(REF_RETURN) { return m_SendBuffer; }
private:
	virtual void Free() override;

private:
	SESSIONID						m_SessionID;
	SESSIONTYPE				m_SessionType;
	TCPSOCKET					m_TcpSocket;
	_llong								m_CurBuffuerLocation;
	// Buffer ����
	TOTALBUFFER				m_TotalBuffer;
	BUFFER							m_SendBuffer;
	BUFFER							m_RecvBuffer;
	// Service
	WKPTR<UService>		m_wpService;

	SHPTR<UTransform> m_spTransform;
	SHPTR<UCollider>		m_spCollider;

	ATOMIC<_bool>			m_isConnected;
};

END

#endif //  _SERVERFRAMEWORK_CORE_PUBLIC_USESSION_H
