#include "ServerDefines.h"
#include "CAwsServerConnector.h"

namespace Server
{
	CAwsServerConnector::CAwsServerConnector(OBJCON_CONSTRUCTOR)
		: UAwsConnector(OBJCON_CONDATA)
	{
	}

	_bool CAwsServerConnector::Start()
	{
		std::cout << "AWS Start\n";
		return __super::Start();
	}

	void CAwsServerConnector::StartGameSession()
	{
		/* AWS�� ���۵� �� */

	}

	void CAwsServerConnector::TerminateGameSession(_int _exitCode)
	{
		/* �ش� AWS�� ����� �� */
		__super::TerminateGameSession(_exitCode);
	}

	void CAwsServerConnector::Free()
	{
	}
}