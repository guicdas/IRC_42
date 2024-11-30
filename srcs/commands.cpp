#include "../includes/irc.hpp"


void	Server::nick( t_client *client, std::vector<std::string> args ){
	std::string clientName;

	if (std::strcmp(args.at(1).c_str(), "\"") == 0)
		client->buffer = "Erroneous nickname"; //432  -> ""
	else
	{  
        clientName = args.at(1);
		std::cout << "changing client's name from " << client->name << " to " << clientName <<  std::endl;
		//if (std::strcmp(client->name.c_str(), clientName.c_str()) != 0)
		//{
			client->buffer = "@time : GUIrcserv " + client->name + " !... NICK :" + clientName + "\n";
			client->name = clientName;
		//}
	}
}

void	Server::join( t_client *client, std::vector<std::string> args ){
	std::string	channelName;

	channelName = args.at(1);
	std::cout << "creating channel " << channelName << ENDL;
	if (channelName[0] != '#' && channelName[0] != '&')
		client->buffer = "@time : GUIrcserv 403" +  client->name + channelName + " :No such channel\n";
	else
	{
		t_channel ch;
		ch.name = channelName;
		ch.operators.push_back(*client);
		ch.clients.push_back(*client);
		this->channels.push_back(ch);

		std::cout << "client " << client->name << " joined " << channelName << std::endl;
		client->buffer = "@time :" + client->name + "!~" + client->name + " JOIN " + channelName + "* :realname\n";
        /*
        MODE #canalasd
        << WHO #canalasd %chtsunfra,152
        */
	}
}

void	Server::list( t_client *client, std::vector<std::string> args ){
	std::vector< t_channel >::iterator it = this->channels.begin();

	std::cout << "client " << client->name << " listing all channels: " << std::endl;

	for (; it != this->channels.end(); it++)
	{
		t_channel &ch = *it;
		client->buffer = ch.name + "\t\t" + ch.topic + ".\n";
	}
}

void	Server::quit( t_client *client, std::vector<std::string> args ){
	(void) args;

    close(client->fd);
	FD_CLR(client->fd, &this->fdList);
    // this->clients.erase(*client);
	std::cout << "client " << client->name << " gone" << std::endl;
}