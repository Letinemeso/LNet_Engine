#ifndef __SOCKET
#define __SOCKET

#include <string>

#include "Pointer_Wrapper.h"


namespace LNet
{

	class Client_Socket
	{
	private:
		Client_Socket() { }
		Client_Socket(const Client_Socket& _other) = delete;

	public:
		Client_Socket(Client_Socket&& _other);
		virtual ~Client_Socket();

	public:


	};
	typedef Pointer_Wrapper<Client_Socket> Client_Socket_Ptr;



	class Server_Socket
	{
	private:
		Server_Socket() { }
		Server_Socket(const Server_Socket& _other) = delete;

	public:
		Server_Socket(Server_Socket&& _other);
		virtual ~Server_Socket();

	public:
		Client_Socket_Ptr accept_connection() const;


	};
	typedef Pointer_Wrapper<Server_Socket> Server_Socket_Ptr;

}


#endif // __SOCKET
