#include "../includes/irc.hpp"


void	Server::nick( t_client *client, std::vector<std::string> args ){
	std::string clientName;
	std::size_t	pos;

	if (args.at(1).size() < 1)
		client->buffer = "Erroneous nickname"; //432  -> ""
	else
	{
		pos = args.at(1).find('\n', 0);
		clientName = args.at(1).substr(0, pos);
		std::cout << "setting client's name as " << clientName << std::endl;
		std::cout << "(previous: " << client->name << ")" <<  std::endl;
		if (std::strcmp(client->name.c_str(), clientName.c_str()) != 0)
		{
			client->buffer = "@time : GUIrcserv " + client->name + " !... NICK :" +clientName;
			client->name = clientName;
		}
	}
}

void	Server::join( t_client *client, std::vector<std::string> args ){
	std::string	channelName;
	std::size_t	pos;

	pos = args.at(1).find('\n', 0);
	channelName = args.at(1).substr(0, pos);
	std::cout << "setting channel's name as " << channelName << std::endl;
	if (channelName[0] != '#' && channelName[0] != '&')
		client->buffer = channelName + " :No such channel\n";
	else
	{
		t_channel ch;
		ch.name = channelName;
		ch.operators.push_back(*client);
		ch.clients.push_back(*client);
		this->channels.push_back(ch);

		std::cout << "client " << client->name << " joined " << channelName << std::endl;
		client->buffer = "now talking " + channelName + "\n";
	}
}

void	Server::list( t_client *client, std::vector<std::string> args ){
	std::vector< t_channel >::iterator it = this->channels.begin();

	std::cout << "client " << client->name << " listing all channels: " << std::endl;

	for (int i = 0; it != this->channels.end(); it++, i++)
	{
		t_channel &ch = *it;
		std::cout << ch.name << "\t\t" << i << ch.topic << std::endl;
	}
}

void	Server::quit( t_client *client, std::vector<std::string> args ){
	(void) args;

	FD_CLR(client->fd, &this->fdRead);
	std::cout << "client " << client->name << " gone" << std::endl;
}