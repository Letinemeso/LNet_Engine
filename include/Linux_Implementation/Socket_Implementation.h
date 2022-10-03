#ifndef __SOCKET_IMPLEMENTATION
#define __SOCKET_IMPLEMENTATION

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <string.h>
#include <thread>

#include "Socket.h"

#include "Debug.h"


namespace LNet
{

	void ___debug_linux_socket_log_func();



	class Client_Socket_Impl : public Client_Socket
	{
	private:
		using raw_socket_t = int;
		raw_socket_t m_raw_socket = 0;

	private:
		mutable bool m_is_listening = false;
		mutable bool m_shut_down_manually = false;

	private:
		Client_Socket_Impl();

	public:
		static Client_Socket_Ptr create(const std::string& _connect_to, int _port);
		~Client_Socket_Impl();

	public:
		void send_message(const std::string &_msg) override;
		Message listen_to_message() override;
		void stop_listening_to_message() override;

	};



	class Connected_Client : public Client_Socket
	{
	private:
		using raw_socket_t = int;
		raw_socket_t m_raw_socket = 0;

		std::string m_ip_adress;
		int m_port = 0;

	private:
		bool m_is_listening = false;
		bool m_shut_down_manually = false;

	private:
		Connected_Client();

	public:
		static Pointer_Wrapper<Client_Socket> create(raw_socket_t _client_connection_socket,
													 std::string&& _ip_adress,
													 int _port);
		~Connected_Client();

	public:
		void send_message(const std::string &_msg) override;
		Message listen_to_message() override;
		void stop_listening_to_message() override;

	};



	class Server_Socket_Impl : public Server_Socket
	{
	private:
		using raw_socket_t = int;
		raw_socket_t m_raw_socket = 0;

	private:
		bool m_is_listening = false;
		bool m_shut_down_manually = false;

	private:
		Server_Socket_Impl();
		Server_Socket_Impl(const Server_Socket_Impl& _other) = delete;

	public:
		static Server_Socket_Ptr create(int _port);
		~Server_Socket_Impl();

	public:
		Client_Socket_Ptr wait_for_connection() override;
		void stop_waiting_for_connection() override;

	};

}


#endif // __LINUX_SOCKET_IMPLEMENTATION
