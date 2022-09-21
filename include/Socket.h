#ifndef __SOCKET
#define __SOCKET

#include <string>

#include "Pointer_Wrapper.h"


namespace LNet
{

	struct Message
	{
		enum class Type
		{
			error = 0,
			disconnect,
			stop_listening,
			message
		};
		Type type;
		std::string message;
	};



	class Client_Socket
	{
	protected:
		Client_Socket();
		Client_Socket(const Client_Socket& _other) = delete;

	public:
//		Client_Socket(Client_Socket&& _other);
		virtual ~Client_Socket();

	public:
		virtual void send_message(const std::string& _msg) = 0;
		virtual Message listen_to_message() = 0;
		virtual void stop_listening_to_message() = 0;


	};
	typedef Pointer_Wrapper<Client_Socket> Client_Socket_Ptr;




	class Server_Socket
	{
	protected:
		Server_Socket();
		Server_Socket(const Server_Socket& _other) = delete;

	public:
//		Server_Socket(Server_Socket&& _other);
		virtual ~Server_Socket();

	public:
		virtual Client_Socket_Ptr wait_for_connection() = 0;
		virtual void stop_waiting_for_connection() = 0;

	};
	typedef Pointer_Wrapper<Server_Socket> Server_Socket_Ptr;

}


#endif // __SOCKET
