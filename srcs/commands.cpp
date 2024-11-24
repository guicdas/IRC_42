#include "../includes/irc.hpp"


void	Server::nick( t_client client, std::string string ){
	std::size_t findFirstChar, findEnd;
	std::string clientName;
	
	findFirstChar = string.find_first_not_of(' ');
	clientName = string.substr(findFirstChar, string.size());
	findEnd = clientName.find(' ', 0);
	clientName = clientName.substr(0, findEnd);
	//std::cout << "setting client's name to "<< clientName << "." <<std::endl;
	if (findEnd == std::string::npos)
	{
		if (clientName.size() < 2)
			std::cout << "Usage: NICK <nickname>, sets your nick" << std::endl;
		else
		{
			//std::cout << "client's name is "<< client.name << "." <<std::endl;
			if (std::strcmp(client.name.c_str(), clientName.c_str()) != 0)
			{
				std::cout << "client nickname set to " << clientName << "."<< std::endl;
				client.name = clientName;
			}
		}
	}
}

void	Server::join( t_client client, std::string string ){
	std::size_t found;

	found = string.find(' ', 0);
	if (found == std::string::npos)
		std::cout << "Usage: JOIN <channel>, joins the channel" << std::endl;
	else
	{
		if (string[0] != '#')
			std::cout << string << " :No such channel" << std::endl;
		else if (string[0] != '&')
			std::cout << string << " :No such channel" << std::endl;
		else
		{
			//may not contain any spaces, a control G / BELL ('^G', 0x07), or a comma
			this->channels[string] = "password";
			std::cout << "client " << client.name <<  " joined" << std::endl;
		}
	}
}

void	Server::list( t_client client, std::string string ){
	(void) client; //?
	std::map<std::string, std::string>::iterator it = this->channels.begin();

	std::cout << "listing all channels: " << std::endl;
	for (; it != this->channels.end(); it++)
	{
		std::cout << it->first << std::endl;
	}
}