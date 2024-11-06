#include "../includes/irc.hpp"

int main( int ac, char **av ){
	try
	{
		int				serverSocket, clientSocket, reuse;
		sockaddr_in6	serverAddr, clientAddr;
		std::string		password;
		unsigned int	port;

		if (ac != 3)
			throw(FileException("Wrong number of arguments!"));
		password = av[2];
		port = std::atoi(av[1]);
		/* 
		create an unbound socket in a communications domain, and return a file descriptor 
		that can be used in later function calls that operate on sockets.*/
		serverSocket = socket(AF_INET6, SOCK_STREAM, 0);
		if (serverSocket == -1)
		{
			std::cerr << "Error: creating socket: ";
			throw(FileException(strerror(errno)));
		}

		reuse = 1;
		if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
		{
			close(serverSocket);
			std::cerr << "Error: setsockopt(): ";
			throw(FileException(strerror(errno)));
		}

		std::memset(&serverAddr, 0, sizeof(serverAddr));
		serverAddr.sin6_family = AF_INET6;
		serverAddr.sin6_port = htons(port);
		serverAddr.sin6_addr = in6addr_any;

		/* The bind() function binds a unique local name 
		to the socket with descriptor socket. After calling socket(), a 
		descriptor does not have a name associated with it. However, 
		it does belong to a particular address family as specified when socket() is called.
		The exact format of a name depends on the address family. */
		if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
		{
			close(serverSocket);
			std::cerr << "Error: bind(): ";
			throw (FileException(strerror(errno)));
		}
		/* transforms an active socket into a passive socket, Once called, 
		socket can never be used as an active socket to initiate connection requests
		It indicates a readiness to accept client connection requests, 
		and creates a connection request queue of length backlog to queue incoming 
		connection requests. Once full, additional connection requests are rejected.*/
		if (listen(serverSocket, 5) == -1)
		{
			close(serverSocket);
			std::cerr << "Error: listen(): ";
			throw (FileException(strerror(errno)));
		}
		std::cout << "Server listening on port " << port << std::endl;

		/* extract the first connection on the queue of pending connections, create a 
		new socket with the same socket type protocol and address family as the specified 
		socket, and allocate a new file descriptor for that socket */
		//	const int* nullptr_equiv = reinterpret_cast<const int*>(0);  // Create a 'null pointer' constant
		//	int* ptr = nullptr_equiv;  // Equivalent to nullptr
		clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t *)sizeof(clientAddr));
		if (clientSocket == -1)
		{
			close(serverSocket);
			std::cerr << "Error: accepting client: ";
			throw (FileException(strerror(errno)));
		}



		char buffer[1024] = {0};
		recv(clientSocket, buffer, sizeof(buffer), 0);
		std::cout << "Message from client: " << buffer << std::endl;

		/* Closes a file descriptor, fildes. close() call shuts down the socket associated with 
		the socket descriptor socket, and frees resources allocated to the socket. If socket refers 
		to an open TCP connection, the connection is closed. If a stream socket is closed when there is input data queued, 
		the TCP connection is reset rather than being cleanly closed. */
		if (close(serverSocket) == -1)
		{
			std::cerr << "Error: closing sockets: ";
			throw (FileException(strerror(errno)));
		}
	}
	catch(const std::exception &e){std::cerr << e.what() << std::endl;}
}