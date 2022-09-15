#ifndef __LINUX_SOCKET_IMPLEMENTATION
#define __LINUX_SOCKET_IMPLEMENTATION

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <unistd.h>
#include <netdb.h>

#include "Socket.h"


namespace LNet
{

	class Client_Socket_Impl : public Client_Socket
	{
	private:
		using raw_socket_t = int;
		raw_socket_t m_raw_socket = 0;

	private:
		Client_Socket_Impl();

	public:
		static Client_Socket_Ptr create(const std::string& _connect_to, int _port);

	public:
		void send_message(const std::string &_msg) const override;
		Message listen_to_message() const override;

	};



	class Connected_Client : public Client_Socket
	{
	private:
		using raw_socket_t = int;
		raw_socket_t m_raw_socket = 0;

		std::string m_ip_adress;
		int m_port = 0;

	private:
		Connected_Client();

	public:
		static Pointer_Wrapper<Client_Socket> create(raw_socket_t _client_connection_socket,
													 std::string&& _ip_adress,
													 int _port);

	public:
		void send_message(const std::string &_msg) const override;
		Message listen_to_message() const override;

	};



	class Server_Socket_Impl : public Server_Socket
	{
	private:
		using raw_socket_t = int;
		raw_socket_t m_raw_socket = 0;

	private:
		Server_Socket_Impl();
		Server_Socket_Impl(const Server_Socket_Impl& _other) = delete;

	public:
//		Server_Socket_Impl(Server_Socket_Impl&& _other);

		static Server_Socket_Ptr create(int _port);

	public:
		Client_Socket_Ptr wait_for_connection() override;
		void stop_waiting_for_connection() override;

	};

}


#endif // __LINUX_SOCKET_IMPLEMENTATION
