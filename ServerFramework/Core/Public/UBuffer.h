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
	using BUFF = ARRAY<_char, MAX_BUFFER_LENGTH>;

public:
	UBuffer();
	UBuffer(MOVE UBuffer&& _Other);
	UBuffer(const UBuffer& _rhs);
	UBuffer(_char* const _Buffer, const _llong& _TotalBuffer);
	~UBuffer();

	UBuffer& operator=(UBuffer&& _Other);
public:
	// ���۸� �ٽ� ��� ���´�. 
	void BufferReceive( _char* const _Buffer,  const _llong& _TotalBuffer);
	// Combines Buffer 
	void CombineBuffers( _char* const _DefiniationBuffer, REF_OUT _llong& _CurrentBufferLocation);
public:
	 BUFF& GetBuffer() { return m_Buff; }
	const _llong GetBufferLength() const { return m_BufferLength; }
private:
	BUFF		m_Buff;
	_llong		m_BufferLength;
};


END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UBUFFER_H