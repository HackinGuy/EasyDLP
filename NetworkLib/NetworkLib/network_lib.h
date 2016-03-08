#ifdef NETWORKLIB_EXPORTS
	#define NETWORKLIB_API __declspec(dllexport)
#else
	#define NETWORKLIB_API __declspec(dllimport)
#endif


#pragma once
#include <winsock2.h>
#include <Windows.h>
#include <string>
#include "stdafx.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
using namespace std;


class NETWORKLIB_API NetworkBase {

private:
	SOCKET socket;
	string serverAddress;
	struct addrinfo hints;
	int serverPort;
	typedef enum log_levels {error, warning, info, debug } log_level;
	void logMessages(const char *message, log_level level) { MessageBoxA(NULL, message, NULL, NULL); }; // TODO Create a logging module and implement here.
	void logMessages(const string message, log_level level) { MessageBoxA(NULL, message.c_str(), NULL, NULL); }; // TODO Create a logging module and implement here.

public:
	typedef enum return_codes { success = 0, wsastrartup_error = 1, getaddrinfo_error, socket_create_error, connect_error, send_error, bind_error, listen_error, accept_error, nullptr_error } return_code;
	NetworkBase(void);
	NetworkBase(SOCKET &sock);
	return_code socketInit();
	return_code connect(const string serverAddress, const char * port);
	return_code send(const string data);
	return_code send(SOCKET clientSocket, const string data);
	return_code bind(const char * port);
	return_code listen(int backlog);
	NetworkBase* accept();
	string recv(const int buf_size);

};

extern "C"{

	typedef int return_code;
	NETWORKLIB_API NetworkBase* New_NetworkBase()
	{
		NetworkBase* netBase = new NetworkBase();
		netBase->socketInit();
		return netBase;
	}

	NETWORKLIB_API return_code net_connect(NetworkBase* netBase, char* ip, char* port)
	{
		if (netBase == nullptr) return NetworkBase::return_code::nullptr_error;

		return netBase->connect(ip, port);
	}

	NETWORKLIB_API return_code net_send(NetworkBase* netBase, char* data)
	{
		if (netBase == nullptr) return NetworkBase::return_code::nullptr_error;
		return netBase->send(data);
	}
	
	NETWORKLIB_API char* net_recv(NetworkBase* netBase, int buf_size)
	{
		if (netBase == nullptr || netBase == 0) return nullptr;
		return _strdup(netBase->recv(buf_size).c_str());
	}
	


	NETWORKLIB_API return_code net_bind(NetworkBase* netBase, const char * port)
	{
		if (netBase == nullptr) return NetworkBase::return_code::nullptr_error;
		return netBase->bind(port);
	}

	NETWORKLIB_API return_code net_listen(NetworkBase* netBase, int backlog)
	{
		if (netBase == nullptr) return NetworkBase::return_code::nullptr_error;
		return netBase->listen(backlog);
	}

	NETWORKLIB_API NetworkBase* net_accept(NetworkBase* netBase)
	{
		if (netBase == nullptr || netBase == 0) return nullptr;
		return netBase->accept();
	}
}




