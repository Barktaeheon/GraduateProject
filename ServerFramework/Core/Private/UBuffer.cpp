#include "CoreDefines.h"
#include "UBuffer.h"

namespace Core {

	UBuffer::UBuffer() : m_Buff{}, m_BufferLength{0}
	{
		ZeroMemory(&m_Buff, MAX_BUFFER_LENGTH);
	}

	UBuffer::UBuffer(MOVE UBuffer&& _Other) : m_Buff{ _Other.m_Buff}, m_BufferLength{ _Other.m_BufferLength}
	{
	}

	UBuffer::UBuffer(const UBuffer& _rhs) : m_Buff{ _rhs.m_Buff }, m_BufferLength{ _rhs.m_BufferLength }
	{
	}

	UBuffer::UBuffer(_char* const _Buffer, const _llong& _TotalBuffer) : m_Buff{}, m_BufferLength{ _TotalBuffer }
	{
		ZeroMemory(&m_Buff, MAX_BUFFER_LENGTH);
		::memcpy((_char*)&m_Buff, _Buffer, static_cast<size_t>(m_BufferLength));
	}

	UBuffer::~UBuffer()
	{
	}

	UBuffer& UBuffer::operator=(UBuffer&& _Other)
	{
		m_Buff = _Other.m_Buff;
		m_BufferLength = _Other.m_BufferLength;
		return *this;
	}

	void UBuffer::BufferReceive(_char* const _Buffer, const _llong& _TotalBuffer)
	{
		ZeroMemory(&m_Buff, MAX_BUFFER_LENGTH);
		m_BufferLength = _TotalBuffer;
		::memcpy((_char*)&m_Buff, _Buffer, static_cast<size_t>(m_BufferLength));
	}

	void UBuffer::CombineBuffers(_char* const _pDefiniationBuffer, REF_OUT _llong& _CurrentBufferLocation)
	{
		// ����  ���ۿ� ���� ������ ũ�⸸ŭ �̵��� ��, �޾ƿ� packet �����͸� �̾���δ�. 
		::memcpy(_pDefiniationBuffer, m_Buff.data(), m_BufferLength);
		// ���� ������ ��ġ�� �̵���Ų��. 
		_CurrentBufferLocation += m_BufferLength;
	}

}