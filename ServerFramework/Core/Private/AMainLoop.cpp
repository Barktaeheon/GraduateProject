#include "CoreDefines.h"
#include "AMainLoop.h"
#include "AServerService.h"
#include "AGameTimer.h"
#include "ACoreInstance.h"
#include "AMonster.h"
#include "AAnimator.h"

namespace Core {

	AMainLoop::AMainLoop(OBJCON_CONSTRUCTOR, Asio::io_context& _context) :
		ACoreObject(OBJCON_CONDATA), m_spGameTimer{ Create<AGameTimer>() }, 
		m_DeadLineEvent{ _context,  boost::posix_time::microseconds(100) }
	{

	}

	void AMainLoop::RegisterTimer(const _double& _dTimeDelta)
	{
		// Ÿ�̸��� �񵿱� ��� ����
		m_DeadLineEvent.async_wait(std::bind(&AMainLoop::TimerThread, this, std::placeholders::_1));
	}

	void AMainLoop::TimerThread(const boost::system::error_code& _error)
	{
		m_spGameTimer->Tick();
		SHPTR<AServerService> spServerService = m_wpServerService.lock();
		{
			_double dTimeDelta = m_spGameTimer->GetDeltaTime();
			LIST<SHPTR<AMonster>> aliveMonster;
			{
				// Mob�� ã�� 
				for (auto& iter : spServerService->GetMobObjContainer())
				{
					SHPTR<AMonster> spMonster = iter.second;
					if (true == spMonster->IsActive())
					{
						aliveMonster.push_back(spMonster);
					}
				}
			}
			// Animation Ȱ�� 
			{
				for (auto& iter : aliveMonster)
				{
					iter->AnimTick(dTimeDelta);
				}
			}
		}
	}

	void AMainLoop::Free()
	{
	}

}