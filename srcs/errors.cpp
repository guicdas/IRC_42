#include "../includes/irc.hpp"

void	Server::checkChannelNameExists( std::string arg )
{
	for (std::vector< Channel >::iterator itCh = this->channels.begin(); itCh != this->channels.end(); itCh++)
	{
		Channel &channel = *itCh;
		if (channel.name == arg)
			throw (403); //client.args.at(1)
	}
}

int		Server::doesChannelNameExist( std::string arg )
{
	for (std::vector< Channel >::iterator itCh = this->channels.begin(); itCh != this->channels.end(); itCh++)
	{
		Channel &channel = *itCh;
		if (channel.name == arg)
			return (1);
	}
	return (0);
}

int		Server::isClientInChannel(std::string nick, std::string channelName)
{
	for (std::vector<Channel>::iterator itCh = this->channels.begin(); itCh != this->channels.end(); ++itCh)
	{
		if (itCh->name == channelName)
		{
			for (std::vector<Client>::iterator itClient = itCh->clients.begin(); itClient != itCh->clients.end(); ++itClient)
			{
				if (itClient->getNick() == nick)
					return 1;
			}
		}
	}
	return 0;
}


void	Server::checkClientNickExists( std::string arg )
{
	for (std::vector< Client >::iterator itC = this->clients.begin(); itC != this->clients.end(); itC++)
	{
		Client &c = *itC;
		if (c.getNick() == arg)
			throw (404); //client.args.at(2)
	}
}

void	checkClientInChannel( Client &client, Channel *channel )
{
	for (std::vector< Client >::iterator itC = channel->clients.begin(); itC != channel->clients.end(); itC++)
	{
		Client &c = *itC;
		if (c.getNick() == client.getNick())
			throw (441); //args 1
	}
}

void	checkClientOp( Client &c, Channel *channel )
{
	for (std::vector< Client >::iterator itC = channel->operators.begin(); itC != channel->operators.end(); itC++)
	{
		Client &operat = *itC;
		if (c.getNick() == operat.getNick())
			throw (482); //args 1
	}
}

