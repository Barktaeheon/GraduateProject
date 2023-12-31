#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UBUFFER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UBUFFER_H

BEGIN(Core)

/*
@ Date: 2023-12-31
@ Writer: ������
@ Explain
- Ŭ���̾�Ʈ���� ���۸� �޾Ƽ� �����ϴ� Ŭ�����̴�. 
*/
class CACHE_ALGIN_CORE_DLL UBuffer {
	using RECVBUFF = ARRAY<_char, MAX_BUFFER_LENGTH>;

public:
	UBuffer();
	UBuffer(MOVE UBuffer&& _Other);
	UBuffer(_char* const _Buffer, const _uint& _TotalBuffer);
	~UBuffer();

	UBuffer& operator=(UBuffer&& _Other);
public:
	// ���۸� �ٽ� ��� ���´�. 
	void BufferReceive( _char* const _Buffer,  const _uint& _TotalBuffer);
	// Combines Buffer 
	void CombineBuffers( _char* const _DefiniationBuffer, REF_OUT _int& _CurrentBufferLocation);
public:
	const RECVBUFF& GetRecvBuffer() const { return m_RecvBuff; }
	const _uint GetBufferLength() const { return m_BufferLength; }
public:
	RECVBUFF	m_RecvBuff;
	_uint				m_BufferLength;
};


END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UBUFFER_H