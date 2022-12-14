#include "Linux_Implementation/Socket_Implementation.h"

using namespace LNet;


//	Debug stuff

void LNet::___debug_linux_socket_log_func()
{
	if(errno == 0)
		return;

	int error_code = errno;
	const char* error_message = strerror(errno);

	std::string error_log_message;
	error_log_message = "\nLinux Socket Error:\nError code: " + std::to_string(error_code) + "\nError description:\n" + error_message + "\n";

	LNET_LOG("LINUX_SOCKET_LOG_LEVEL", error_log_message);

	errno = 0;
}



//	Client_Socket_Impl

Client_Socket_Impl::Client_Socket_Impl() : Client_Socket()
{

}

Client_Socket_Ptr Client_Socket_Impl::create(const std::string &_connect_to, int _port)
{
	int sockfd;
	sockaddr_in serv_addr;
	hostent *server;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		LNET_DEBUG_FUNC_NOARG(___debug_linux_socket_log_func);
		return Client_Socket_Ptr(nullptr);
	}
	server = gethostbyname(_connect_to.c_str());
	if (!server)
	{
		close(sockfd);
//		LNET_DEBUG_FUNC_NOARG(___debug_linux_socket_log_func);
		LNET_LOG("LINUX_SOCKET_LOG_LEVEL", "Error: server with ip " + _connect_to + " not found\n");
		return Client_Socket_Ptr(nullptr);
	}

	for(unsigned int i = 0; i < (unsigned int)server->h_length; ++i)
		((char*)&serv_addr.sin_addr.s_addr)[i] = server->h_addr[i];
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(_port);

//	delete server;	//	TODO: this may cause memory leak: hostent contains raw pointers
	endhostent();

	if (connect(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		close(sockfd);
		LNET_DEBUG_FUNC_NOARG(___debug_linux_socket_log_func);
		return Client_Socket_Ptr(nullptr);
	}

	Client_Socket_Impl* raw_result = new Client_Socket_Impl;
	raw_result->m_raw_socket = sockfd;

	return Client_Socket_Ptr(raw_result);
}

Client_Socket_Impl::~Client_Socket_Impl()
{
	close(m_raw_socket);
}



void Client_Socket_Impl::send_message(const std::string &_msg) const
{
	int result = write(m_raw_socket, _msg.c_str(), _msg.size());
//	return result >= 0;
}

void Client_Socket_Impl::start_listening_to_message(void(*_on_message)(Message)) const
{
	LNET_ASSERT(!m_is_listening);

	std::thread listening_thread([_on_message, this]()
	{
		m_is_listening = true;

		bool need_to_stop = false;
		while(!need_to_stop)
		{
			char buffer[1024];
			unsigned int real_length = read(m_raw_socket, buffer, 1024);

			Message result;
			if(real_length < 0)
				result.type = Message::Type::error;
			if(real_length == 0)
				result.type = Message::Type::disconnect;
			if(real_length > 0)
			{
				result.type = Message::Type::message;
				result.message = buffer;
			}

		}

	})





//	return result;
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
	close(m_raw_socket);
}



void Connected_Client::send_message(const std::string &_msg) const
{
	int result = write(m_raw_socket, _msg.c_str(), _msg.size());
//	return result >= 0;
}

Message Connected_Client::listen_to_message() const
{
	char buffer[1024];
	unsigned int real_length = read(m_raw_socket, buffer, 1024);

	Message result;
	if(real_length < 0)
		result.type = Message::Type::error;
	if(real_length == 0)
		result.type = Message::Type::disconnect;
	if(real_length > 0)
	{
		buffer[real_length] = 0;
		result.type = Message::Type::message;
		result.message = buffer;
	}

	return result;
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

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		LNET_DEBUG_FUNC_NOARG(___debug_linux_socket_log_func);
		return Pointer_Wrapper<Server_Socket>(nullptr);
	}
	//	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_addr.s_addr = 0;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(_port);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		LNET_DEBUG_FUNC_NOARG(___debug_linux_socket_log_func);
		return Pointer_Wrapper<Server_Socket>(nullptr);
	}

	Server_Socket_Impl* raw_result = new Server_Socket_Impl;
	raw_result->m_raw_socket = sockfd;

	return Pointer_Wrapper<Server_Socket>(raw_result);
}

Server_Socket_Impl::~Server_Socket_Impl()
{
	stop_waiting_for_connection();
	close(m_raw_socket);
}



Client_Socket_Ptr Server_Socket_Impl::wait_for_connection()
{
	m_is_listening = true;

	listen(m_raw_socket, 1);

	sockaddr_in cli_addr;
	unsigned int clilen = sizeof(cli_addr);

	int raw_client_socket = accept(m_raw_socket, (sockaddr*)&cli_addr, &clilen);

	m_is_listening = false;

	if(raw_client_socket < 0 && !m_shut_down_manually)
	{
		LNET_DEBUG_FUNC_NOARG(___debug_linux_socket_log_func);
		return Client_Socket_Ptr(nullptr);
	}

	m_shut_down_manually = false;

	return Connected_Client::create(raw_client_socket, std::to_string(cli_addr.sin_addr.s_addr), cli_addr.sin_port);
}

void Server_Socket_Impl::stop_waiting_for_connection()
{
	LNET_ASSERT(m_is_listening);

	m_shut_down_manually = true;
	shutdown(m_raw_socket, SHUT_RD);
}



