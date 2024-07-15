#include "CoreDefines.h"
#include "AMainLoop.h"
#include "AService.h"
#include "AGameTimer.h"
#include "ACoreInstance.h"
#include "AMonster.h"
#include "AAnimator.h"
#include "AAnimController.h"

namespace Core {

	AMainLoop::AMainLoop(OBJCON_CONSTRUCTOR, Asio::io_context& _context, SHPTR<AService> _spServerService) :
		ACoreObject(OBJCON_CONDATA), m_spGameTimer{ Create<AGameTimer>() }, 
		m_SteadyTimer{ _context }, m_wpServerService{_spServerService}
	{

	}

	void AMainLoop::RegisterTimer()
	{
		m_SteadyTimer.expires_from_now(std::chrono::microseconds(10));
		// Ÿ�̸��� �񵿱� ��� ����
		m_SteadyTimer.async_wait(std::bind(&AMainLoop::TimerThread, this, std::placeholders::_1));
	}

	void AMainLoop::TimerThread(const boost::system::error_code& _error)
	{
		m_spGameTimer->Tick();
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		SHPTR<AService> spServerService = m_wpServerService.lock();
		{
			_double dTimeDelta = m_spGameTimer->GetDeltaTime();
			LIST<SHPTR<AMonster>> aliveMonster;
			{
				// Mob�� ã�� 
				for (auto& iter : spServerService->GetMobObjContainer())
				{
					SHPTR<AMonster> spMonster = iter.second;

					if (true == spMonster->IsPermanentDisable())
						continue;

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
					iter->Tick(dTimeDelta);
				}
				// Collision 
				spCoreInstance->CollisionSituation(dTimeDelta);
			}
		}
		RegisterTimer();
	}

	void AMainLoop::Free()
	{
	}

}