#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H


namespace Core
{
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain:  LOGTYPE�� enum 
	*/
	enum class LOGTYPE : unsigned int
	{
		NONE,
		INFOLOG,						// �Ϲ� ���� �α�
		WARNNINGLOG,			// ��� �α�
		ERRORLOG,					// ���� �α�
		DEBUGLOG,			// ����׿� �α�
		CRITICALLOG,			// ġ������ ��Ȳ�� ���� �α�
		// End 
		LOG_END,
	};
}



#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H