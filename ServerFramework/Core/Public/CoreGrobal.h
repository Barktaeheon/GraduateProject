#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREGROBAL_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREGROBAL_H

namespace Core
{
	// �� �������� �����Ǿ���ϴ� App
	CORE_DLL extern class  ACoreApp* g_RegisterApp;

#ifdef USE_DEBUG
	CORE_DLL		extern class UDeadLockProfiler*		g_pDeadLockProfiler;
#endif

	CORE_DLL 	extern class AMemoryAdiminstor*	g_pMemoryAdminster;

	CORE_DLL		extern class ALogger*				g_LogManager;
}

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREGROBAL_H