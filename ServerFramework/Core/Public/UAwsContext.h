#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UAWSCONTEXT_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UAWSCONTEXT_H

#include "UBase.h"
#include "aws/gamelift/model/GameSession.h"

BEGIN(Core)
/*
@ Date: 2023-12-31
@ Writer: ������
@ Explain
- Aws�� �̿��ϱ� ���� Context ��ü�̴�. 
�ش� ��ü�� �θ���ؼ� Server ������Ʈ���� �ڽ� Ŭ������ �������Ͽ� ����Ѵ�. 
*/
class CACHE_ALGIN_CORE_DLL UAwsContext abstract : public UBase {

};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UAWSCONTEXT_H