#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_CORETLS_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_CORETLS_H

namespace Core
{
	namespace TLS
	{
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain
		- ���� ó���� MAX_THRED�� �� 
		*/
		constexpr static _uint									 MAX_THREAD{ 16 };
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain
		- Thread_Local���� Thread�� ID�� �����Ѵ�. 
		*/
		static  thread_local _uint							g_ThreadID;

		static thread_local _char							g_LogTextBuffer[512];
	}
}


#endif 