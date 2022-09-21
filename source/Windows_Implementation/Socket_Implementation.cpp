#include "Windows_Implementation/Socket_Implementation.h"

using namespace LNet;


//	Debug stuff

void LNet::___debug_windows_socket_log_func()
{
	if(errno == 0)
		return;

	int error_code = errno;
	const char* error_message = strerror(errno);

	std::string error_log_message;
	error_log_message = "\nLinux Socket Error:\nError code: " + std::to_string(error_code) + "\nError description:\n" + error_message + "\n";

	LNET_LOG("WINDOWS_SOCKET_LOG_LEVEL", error_log_message);

	errno = 0;
}



//	Client_Socket_Impl

Client_Socket_Impl::Client_Socket_Impl() : Client_Socket()
{

}

Client_Socket_Ptr Client_Socket_Impl::create(const std::string &_connect_to, int _port)
{
	int raw_socket = INVALID_SOCKET;

	int error;

	addrinfo* result = nullptr;
	addrinfo hints;

	hints.ai_flags = 0;
	hints.ai_addrlen = 0;
	hints.ai_canonname = nullptr;
	hints.ai_addr = nullptr;
	hints.ai_next = nullptr;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	error = getaddrinfo(_connect_to.c_str(), std::to_string(_port).c_str(), &hints, &result);

	if (error != 0)
	{
		LNET_LOG("WINDOWS_SOCKET_LOG_LEVEL", "getaddrinfo error: " + std::to_string(error));
		return Client_Socket_Ptr(nullptr);
	}

	raw_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (raw_socket == INVALID_SOCKET)
	{
		LNET_LOG("WINDOWS_SOCKET_LOG_LEVEL", "getaddrinfo error: " + std::to_string(error));
		return Client_Socket_Ptr(nullptr);
	}

	error = connect(raw_socket, result->ai_addr, result->ai_addrlen);
	if (error == SOCKET_ERROR)
	{
		closesocket(raw_socket);
		raw_socket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (raw_socket == INVALID_SOCKET)
	{
		LNET_LOG("WINDOWS_SOCKET_LOG_LEVEL", "getaddrinfo error: " + std::to_string(error));
		return Client_Socket_Ptr(nullptr);
	}

	Client_Socket_Impl* raw_client_socket = new Client_Socket_Impl;
	raw_client_socket->m_raw_socket = raw_socket;

	return Client_Socket_Ptr(raw_client_socket);
}

Client_Socket_Impl::~Client_Socket_Impl()
{
	closesocket(m_raw_socket);
}



void Client_Socket_Impl::send_message(const std::string &_msg)
{
	int result = send(m_raw_socket, _msg.c_str(), _msg.size(), 0);
//	return result >= 0;
}

Message Client_Socket_Impl::listen_to_message()
{
	char buffer[1024];
	unsigned int real_length = recv(m_raw_socket, buffer, 1024, 0);

	Message result;
	if(real_length < 0)
		result.type = Message::Type::error;
	else if(real_length == 0)
		result.type = Message::Type::disconnect;
	else if(real_length > 0)
	{
		buffer[real_length] = 0;
		result.type = Message::Type::message;
		result.message = buffer;
	}
	else if(m_shut_down_manually)
	{
		result.type = Message::Type::stop_listening;
	}

	m_is_listening = false;
	m_shut_down_manually = false;
	return result;
}

void Client_Socket_Impl::stop_listening_to_message()
{
	LNET_ASSERT(m_is_listening);

	m_shut_down_manually = true;
	shutdown(m_raw_socket, SD_BOTH);
}



//	Connected_Client

Connected_Client::Connected_Client() : Client_Socket()
{

}

Client_Socket_Ptr Connected_Client::create(raw_socket_t _client_connection_socket,
										   std::string&& _ip_adress,
										   int _port)
{
	Connected_Client* raw_result = new Connected_Client;
	raw_result->m_raw_socket = _client_connection_socket;
	raw_result->m_port = _port;
	raw_result->m_ip_adress = std::move(_ip_adress);

	return Client_Socket_Ptr(raw_result);
}

Connected_Client::~Connected_Client()
{
	closesocket(m_raw_socket);
}



void Connected_Client::send_message(const std::string &_msg)
{
	int result = send(m_raw_socket, _msg.c_str(), _msg.size(), 0);
//	return result >= 0;
}

Message Connected_Client::listen_to_message()
{
	char buffer[1024];
	unsigned int real_length = recv(m_raw_socket, buffer, 1024, 0);

	Message result;
	if(real_length < 0)
		result.type = Message::Type::error;
	else if(real_length == 0)
		result.type = Message::Type::disconnect;
	else if(real_length > 0)
	{
		buffer[real_length] = 0;
		result.type = Message::Type::message;
		result.message = buffer;
	}
	else if(m_shut_down_manually)
	{
		result.type = Message::Type::stop_listening;
	}

	m_is_listening = false;
	m_shut_down_manually = false;
	return result;
}

void Connected_Client::stop_listening_to_message()
{
	LNET_ASSERT(m_is_listening);

	m_shut_down_manually = true;
	shutdown(m_raw_socket, SD_BOTH);
}



//	Server_Socket_Impl

Server_Socket_Impl::Server_Socket_Impl() : Server_Socket()
{

}

//Server_Socket_Impl::Server_Socket_Impl(Server_Socket_Impl&& _other) : Server_Socket(std::move(_other))
//{
//	//	move smth here
//}


Server_Socket_Ptr Server_Socket_Impl::create(int _port)
{
	int sockfd;
	sockaddr_in serv_addr;

	addrinfo *result = nullptr, hints;

	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	int error = getaddrinfo(NULL, "25565", &hints, &result);
	if (error != 0)
	{
		LNET_LOG("WINDOWS_SOCKET_LOG_LEVEL", "getaddrinfo failed: " + std::to_string(error) + " " + strerror(error));
		return Server_Socket_Ptr(nullptr);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		LNET_DEBUG_FUNC_NOARG(___debug_windows_socket_log_func);
		return Pointer_Wrapper<Server_Socket>(nullptr);
	}
	//	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_addr.s_addr = 0;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(_port);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		LNET_DEBUG_FUNC_NOARG(___debug_windows_socket_log_func);
		return Pointer_Wrapper<Server_Socket>(nullptr);
	}

	Server_Socket_Impl* raw_result = new Server_Socket_Impl;
	raw_result->m_raw_socket = sockfd;

	return Pointer_Wrapper<Server_Socket>(raw_result);
}

Server_Socket_Impl::~Server_Socket_Impl()
{
	stop_waiting_for_connection();
	closesocket(m_raw_socket);
}



Client_Socket_Ptr Server_Socket_Impl::wait_for_connection()
{
	m_is_listening = true;

	listen(m_raw_socket, 1);

	sockaddr_in cli_addr;
	int clilen = sizeof(cli_addr);

	int raw_client_socket = accept(m_raw_socket, (sockaddr*)&cli_addr, &clilen);

	m_is_listening = false;

	if(raw_client_socket < 0 && !m_shut_down_manually)
	{
		LNET_DEBUG_FUNC_NOARG(___debug_windows_socket_log_func);
		return Client_Socket_Ptr(nullptr);
	}

	m_shut_down_manually = false;

	return Connected_Client::create(raw_client_socket, std::to_string(cli_addr.sin_addr.s_addr), cli_addr.sin_port);
}

void Server_Socket_Impl::stop_waiting_for_connection()
{
	LNET_ASSERT(m_is_listening);

	m_shut_down_manually = true;
	shutdown(m_raw_socket, SD_BOTH);
}



