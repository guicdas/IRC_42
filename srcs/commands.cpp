#include "../includes/irc.hpp"


void	Server::nick( t_client *client, std::string string ){
	std::size_t	findFirstChar;
	std::string clientName;

	findFirstChar = string.find(' ', 0);
	if (findFirstChar == std::string::npos)
		client->buffer = "Usage: NICK <nickname>, sets your nick\n";
	else
	{
		clientName = string.substr(++findFirstChar, string.size() - 1);
		/*
			- "/nick       " e "/nick gui (...)" dao merda
			- por enquanto ta size() - 1 pa tirar o \n final, mas n pode ser assim
		*/
		
		if (std::strcmp(client->name.c_str(), clientName.c_str()) != 0)
		{
			client->buffer = "You are now known as " + clientName + ".\n";
			client->name = clientName;
		}
	}
}

void	Server::join( t_client *client, std::string string ){
	std::size_t found;
	std::string	channelName;

	found = string.find(' ', 0);
	if (found == std::string::npos)
		client->buffer = "Usage: JOIN <channel>, joins the channel\n";
	else
	{
		channelName = string.substr(++found, string.size() - 1);
		/* string n tem o que deve ter ainda*/

		if (string[0] != '#' && string[0] != '&')
		{
			client->buffer = string + " :No such channel\n";
		}
		else
		{
			/*std::vector< t_channel >::const_iterator it = this->channels.begin();
	
			for ()
			{

			}*/
			t_channel chan;
			chan.name = channelName;
			//chan.operators.push_back(client);		// If the channel doesn’t exist prior to joining, the channel is created and the creating user becomes a channel operator.
			//chan.clients.push_back(client);
			this->channels.push_back(chan);
			
			/*The channel is created implicitly when the first client joins it, and the channel ceases to exist when the last client leaves it.
		While the channel exists, any client can reference the channel using the name of the channel.*/
	
			//this->channels[string] = "password";
			client->buffer = "client " + client->name + " joined\n";
		}
	}
}

void	Server::list( t_client *client, std::string string ){
	(void) client; //?
//	std::map<std::string, std::string>::iterator it = this->channels.begin();

	std::cout << "listing all channels: " << std::endl;

	for (; it != this->channels.end(); it++)
	{
		std::cout << it->first << std::endl;
	}
	
}

void	Server::quit( t_client *client, std::string string ){
	(void) string;

	
	FD_CLR(client->fd, &this->fdRead);
	std::cout << "client " << client->name << " gone" << std::endl;
}