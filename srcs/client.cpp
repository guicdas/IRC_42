#include "../includes/irc.hpp"

#define BUF_SIZE 4096

int	Server::parseCommand( t_client &client, std::string s )
{
	std::istringstream	input(s);
	std::string			arg;
	int					ret = 0;

	while (std::getline(input, arg, ' '))
		client.args.push_back(arg);

	for (int i = 0; client.args.at(0)[i] ;i++)
	{
		if (std::islower(client.args.at(0)[i]))
			client.args.at(0)[i] = toupper(client.args.at(0)[i]);
	}	
	
	std::map<std::string, int(Server::*)( t_client & )>::iterator itMap = this->commands.find(client.args.at(0));
	if (itMap != this->commands.end())
		ret = (this->*itMap->second)(client);
	else
		Server::privmsg(client);
	
	client.args.clear();
	return (ret);
}

int	Server::clientRead( t_client &c )
{
	char		buffer[BUF_SIZE] = {0};
	std::string	command;
	int			bytesRecv;
	size_t		found;

	bytesRecv = recv(c.fd, &buffer, BUF_SIZE, 0);
	if (bytesRecv < 1)
	{
		if (bytesRecv < 0)
			throw(FileException("recv error"));
		close(c.fd);
		FD_CLR(c.fd, &this->fdList);
		return (1);
	}
	command.append(buffer, bytesRecv);
	found = command.find('\r', 0);
	if (found == std::string::npos)
		throw(FileException("weird cmd"));
	
	std::cout << "\ncmd >"<< command.substr(0, found) << ENDL;
	return (parseCommand(c, command.substr(0, found)));
}

void	clientWrite( t_client &c )
{
	char	buffer[BUF_SIZE];
	int		bytesToSend;

	bytesToSend = c.buffer.size();
	std::strcpy(buffer, c.buffer.c_str());

	if (bytesToSend > 0)
	{
		std::memcpy(buffer, c.buffer.c_str(), bytesToSend);

		while (bytesToSend > 0)
		{
			bytesToSend -= send(c.fd, buffer, std::strlen(buffer), 0);
			if (bytesToSend < 0)
				throw (FileException("send went wrong!"));
		}
		std::cout << "sent [" << c.buffer << "] to fd " << c.fd << ENDL;
		c.buffer.clear();
	}
}