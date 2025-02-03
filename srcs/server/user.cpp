#include "../includes/irc.hpp"

#define BUF_SIZE 4096

int	Server::parseCommand( Client &client, std::string s )
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
	
	std::map<std::string, int(Server::*)( Client & )>::iterator itMap = this->commands.find(client.args.at(0));
	if (itMap != this->commands.end())
		ret = (this->*itMap->second)(client);
	else
		Server::privmsg(client);//unkown command
	
	client.args.clear();
	return (ret);
}

int	Server::clientRead( Client &c )
{
	char		buffer[BUF_SIZE] = {0};
	std::string	command;
	int			bytesRecv;
	size_t		found;

	bytesRecv = recv(c.getFd(), &buffer, BUF_SIZE, 0);//todo
	if (bytesRecv < 1)
	{
		if (bytesRecv < 0)
			throw(FileException("recv error"));
		close(c.getFd());
		FD_CLR(c.getFd(), &this->fdList);
		return (1);
	}
	command.append(buffer, bytesRecv);
	found = command.find('\r', 0);
	if (found == std::string::npos)
		throw(FileException("weird cmd"));
	
	std::cout << "\ncmd >"<< command.substr(0, found) << ENDL;
	return (parseCommand(c, command.substr(0, found)));
}

void	clientWrite( Client &c )
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
			bytesToSend -= send(c.getFd(), buffer, std::strlen(buffer), 0);
			if (bytesToSend < 0)
				throw (FileException("send went wrong!"));
		}
		std::cout << "sent [" << c.buffer << "] to fd " << c.getFd() << ENDL;
		c.buffer.clear();
	}
}