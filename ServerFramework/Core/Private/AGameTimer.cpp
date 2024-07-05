#include "CoreDefines.h"
#include "AGameTimer.h"

namespace Core {
	
	AGameTimer::AGameTimer() :
		m_pRunningFunc{ &AGameTimer::TickActive },
		m_dTimeDelta{ 0.0 },
		m_isStop{ false }
	{
		MemoryInitialization(m_stCurTimer);
		MemoryInitialization(m_stOldLTime);
		MemoryInitialization(m_stOriginLTime);
		MemoryInitialization(m_stStopLTime);
		MemoryInitialization(m_stCPULTime);

		//���κ��尡 ���� �ִ� ���ػ� Ÿ�̸��� �������� ������ �Լ�
		QueryPerformanceCounter(&m_stCurTimer);		// 1000
		QueryPerformanceCounter(&m_stOldLTime);			// 1020	
		QueryPerformanceCounter(&m_stOriginLTime);			// 1030	
		QueryPerformanceCounter(&m_stStopLTime);			// 1030	
		// ���ػ� Ÿ�̸��� ���ļ��� ������ �Լ�, ���ļ��� cpu �ʴ� Ŭ���� �ֱ⸦ ����
		QueryPerformanceFrequency(&m_stCPULTime);			// 1600000
	}

	void AGameTimer::StopTimer()
	{
		m_pRunningFunc = &AGameTimer::Stop;
	}

	void AGameTimer::RunTimer()
	{
		m_pRunningFunc = &AGameTimer::TickActive;
	}

	void AGameTimer::TickActive()
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

	void AGameTimer::Stop()
	{
		QueryPerformanceCounter(&m_stStopLTime);
		if (m_stStopLTime.QuadPart - m_stOriginLTime.QuadPart > m_stCPULTime.QuadPart)
		{
			QueryPerformanceFrequency(&m_stCPULTime);
			m_stOriginLTime = m_stStopLTime;
		}
		m_dTimeDelta = static_cast<_double>(m_stStopLTime.QuadPart - m_stOldLTime.QuadPart) / m_stCPULTime.QuadPart;
	}

	void AGameTimer::Free()
	{
	}
}