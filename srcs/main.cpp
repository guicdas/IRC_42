#include "../includes/irc.hpp"

int main( int ac, char **av ){
	try
	{
		if (ac != 3)
			throw(FileException("Wrong number of arguments!"));
		
		Chat chat(av);

		int				clientSocket;
		sockaddr_in6	clientAddr;
		/* 
		create an unbound socket in a communications domain, and return a file descriptor 
		that can be used in later function calls that operate on sockets.*/
		chat.createServerSocket();

		//if (fcntl(SerSocketFd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		//	throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
		chat.prepareServerSocket();
		std::cout << "Server listening on port " << chat.getServerPort() << std::endl;

		/* extract the first connection on the queue of pending connections, create a 
		new socket with the same socket type protocol and address family as the specified 
		socket, and allocate a new file descriptor for that socket */
		//	const int* nullptr_equiv = reinterpret_cast<const int*>(0);  // Create a 'null pointer' constant
		//	int* ptr = nullptr_equiv;  // Equivalent to nullptr
		clientSocket = accept(chat.getServerFd(), (struct sockaddr*)&clientAddr, (socklen_t *)sizeof(clientAddr));
		if (clientSocket == -1)
		{
			close(chat.getServerFd());
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
		if (close(chat.getServerPort()) == -1)
		{
			std::cerr << "Error: closing sockets: ";
			throw (FileException(strerror(errno)));
		}
	}
	catch(const std::exception &e){std::cerr << e.what() << std::endl;}
}