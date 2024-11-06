#include "../includes/irc.hpp"

int main( int ac, char **av ){
	try
	{
		if (ac != 3)
			throw(FileException("Wrong number of arguments!"));
		std::cout << "port: " << av[1] << "\npassword: " << av[2] << std::endl;
		/* 
		create an unbound socket in a communications domain, and return a file descriptor 
		that can be used in later function calls that operate on sockets.*/
		int	serverSocket = socket(AF_INET6, SOCK_STREAM, 0);
		if (serverSocket == -1)
			throw(FileException("Error: Could not create socket!"));

		sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(8080);
		serverAddress.sin_addr.s_addr = INADDR_ANY;

		/* The bind() function binds a unique local name 
		to the socket with descriptor socket. After calling socket(), a 
		descriptor does not have a name associated with it. However, 
		it does belong to a particular address family as specified when socket() is called.
		The exact format of a name depends on the address family. */
		if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
			throw (FileException(strerror(errno)));

		/* transforms an active socket into a passive socket, Once called, 
		socket can never be used as an active socket to initiate connection requests
		It indicates a readiness to accept client connection requests, 
		and creates a connection request queue of length backlog to queue incoming 
		connection requests. Once full, additional connection requests are rejected.*/
		if (listen(serverSocket, 5) == -1)
			throw (FileException(strerror(errno)));

		/* extract the first connection on the queue of pending connections, create a 
		new socket with the same socket type protocol and address family as the specified 
		socket, and allocate a new file descriptor for that socket */
		//	const int* nullptr_equiv = reinterpret_cast<const int*>(0);  // Create a 'null pointer' constant
		//	int* ptr = nullptr_equiv;  // Equivalent to nullptr
		int clientSocket = accept(serverSocket, NULL, NULL);
		if (clientSocket == -1)
			throw (FileException(strerror(errno)));

		char buffer[1024] = {0};
		recv(clientSocket, buffer, sizeof(buffer), 0);
		std::cout << "Message from client: " << buffer << std::endl;
		/* Closes a file descriptor, fildes. close() call shuts down the socket associated with 
		the socket descriptor socket, and frees resources allocated to the socket. If socket refers 
		to an open TCP connection, the connection is closed. If a stream socket is closed when there is input data queued, 
		the TCP connection is reset rather than being cleanly closed. */
		if (close(serverSocket) == -1)
			throw (FileException(strerror(errno)));
	}
	catch(const std::exception &e){std::cerr << e.what() << std::endl;}
}