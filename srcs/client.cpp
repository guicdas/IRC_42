#include "../includes/irc.hpp"

#define BUF_SIZE 4096

void	Server::parseCommand( t_client *client, std::string s )
{
	std::string::iterator itStr = s.begin();		// feio, fica so por enquanto
	if (*itStr == '\n')
		s = s.substr(1, s.size());

	std::istringstream	input(s);
	std::string			arg;
	
	while (std::getline(input, arg, ' '))
		client->args.push_back(arg);
	std::cout << "\nline received: " << s << ENDL;
	
	std::vector<std::string> :: iterator itve = client->args.begin();

	std::map<std::string, void(Server::*)( t_client *, std::vector< std::string >)>::iterator itMap = this->commands.find(client->args.at(0));
	if (itMap != this->commands.end())
		(this->*itMap->second)(client, client->args);

}

int	Server::clientRead( t_client *c )
{
	char		buffer[BUF_SIZE] = {0};
	std::string	command;
	int 		i = 0;

	if (recv(c->fd, &buffer, BUF_SIZE, 0) < 1)
		return (0);

	std::istringstream	commandLine(buffer);
	while (std::getline(commandLine, command, '\r'))
	{
		std::string::iterator itStr = command.begin();
		if (*itStr == '\n')
			std::advance(itStr, 1);
		if (std::distance(itStr,command.end()) > 1)
		{
			parseCommand(c, command);
			clientWrite(c);
			command.clear();
		}
		std::cout << "i: "<< ++i << " BUFFER: " << c->buffer;
	}
	return (1);
}

void	Server::clientWrite( t_client *client )
{
	char		buffer[BUF_SIZE];

	std::strcpy(buffer, client->buffer.c_str());
	std::cout << "buf: ->" << buffer;
	if (send(client->fd, &buffer, std::strlen(buffer), 0) < 1)
		std::cout << "client " << client->name << " DEUMERDA" << ENDL;
}

/*
/nick    gui  => (nick ja existe como channel) You are now known as gui
-NickServ- This nickname is registered. Please choose a different nickname, or identify via /msg NickServ IDENTIFY gui <password>

/join #gui,other => other => no such channel (gui ja existe)
/user  =>  You are already connected and cannot handshake again

(/list  =>  LIST :This command could not be completed because it has been used recently, and is rate-limited.)
*/
