#include "EngineDefines.h"
#include "UTimer.h"
#include "UMethod.h"

UTimer::UTimer()  :
	m_pRunningFunc{ &UTimer::TickActive},
	m_dTimeDelta{ 0.0},
	m_isStop{ false}
{
	UMethod::ClearToMemory(m_stCurTimer);
	UMethod::ClearToMemory(m_stOldLTime);
	UMethod::ClearToMemory(m_stOriginLTime);
	UMethod::ClearToMemory(m_stStopLTime);
	UMethod::ClearToMemory(m_stCPULTime);
}

void UTimer::Free()
{ 
}

HRESULT UTimer::NativeConstruct()
{
	//���κ��尡 ���� �ִ� ���ػ� Ÿ�̸��� �������� ������ �Լ�
	QueryPerformanceCounter(&m_stCurTimer);		// 1000
	QueryPerformanceCounter(&m_stOldLTime);			// 1020	
	QueryPerformanceCounter(&m_stOriginLTime);			// 1030	
	QueryPerformanceCounter(&m_stStopLTime);			// 1030	
	// ���ػ� Ÿ�̸��� ���ļ��� ������ �Լ�, ���ļ��� cpu �ʴ� Ŭ���� �ֱ⸦ ����
	QueryPerformanceFrequency(&m_stCPULTime);			// 1600000
	return S_OK;
}

void UTimer::StopTimer()
{
	m_pRunningFunc = &UTimer::Stop;
}

void UTimer::RunTimer()
{
	m_pRunningFunc = &UTimer::TickActive;
}

void UTimer::TickActive()
{
	QueryPerformanceCounter(&m_stCurTimer);
	if (m_stCurTimer.QuadPart - m_stOriginLTime.QuadPart > m_stCPULTime.QuadPart)
	{
		QueryPerformanceFrequency(&m_stCPULTime);
		m_stOriginLTime = m_stCurTimer;
	}
	m_dTimeDelta = static_cast<_double>(m_stCurTimer.QuadPart - m_stOldLTime.QuadPart)
		/ static_cast<_double>(m_stCPULTime.QuadPart);
	m_stOldLTime = m_stCurTimer;
}

void UTimer::Stop()
{
	QueryPerformanceCounter(&m_stStopLTime);
	if (m_stStopLTime.QuadPart - m_stOriginLTime.QuadPart > m_stCPULTime.QuadPart)
	{
		QueryPerformanceFrequency(&m_stCPULTime);
		m_stOriginLTime = m_stStopLTime;
	}
	m_dTimeDelta = static_cast<_double>(m_stStopLTime.QuadPart - m_stOldLTime.QuadPart) / m_stCPULTime.QuadPart;
}
