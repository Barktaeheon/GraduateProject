#include "ServerDefines.h"
#include "CServerApp.h"

#include "UDBConnector.h"

int main()
{
	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Server::CServerApp* Server = CreateAndRegisterApp<Server::CServerApp>();
	Server->Start();
	std::cout << "End Server\n";
}