#include "../includes/irc.hpp"

void	putInBuf( t_client &c, int code, std::string extra , std::string command )
{
	switch (code){
		case 324:
			c.buffer = "ircserv 324 " + c.nickname + " " + extra;										break;
		case 401:
			c.buffer = "ircserv 401 " + c.nickname + " :No such nick/channel";						break;
		case 403:
			c.buffer = "ircserv 403 " + c.nickname + " " + extra + " :No such channel";				break;
		case 404:
			c.buffer = "ircserv 404 " + c.nickname + " " + extra + " :Cannot send to nick/channel";	break;
		case 411:
			c.buffer = "ircserv 411 " + c.nickname + " :No recipient given (PRIVMSG)";				break;
		case 412:
			c.buffer = "ircserv 412 " + c.nickname + " :No text to send";							break;
		case 431:
			c.buffer = "ircserv 431 " + c.nickname + " :No nickname given";							break;
		case 432:
			c.buffer = "ircserv 432 " + c.nickname + " :Erroneous nickname";						break;
		case 433:
			c.buffer = "ircserv 433 " + c.nickname + " " + extra + " :Nickname is already in use";	break;
		case 461:
			c.buffer = "ircserv 461 " + c.nickname + " " + command + ERR_NEEDMOREPARAMS;				break;
		case 462:
			c.buffer = "ircserv 462 " + c.nickname + ERR_ALREADYREGISTERED;							break;
		default:
			if (c.nickname.size() < 1)
				c.buffer = "ircserv " + command + " setting nick to: " + extra;
			else
				c.buffer = "ircserv " + c.nickname + " " + command + " " + extra;
			break;
	}
}

int	Server::checkChannelNameExists( std::string arg )
{
	for (std::vector< t_channel >::iterator itCh = this->channels.begin(); itCh != this->channels.end(); itCh++)
	{
		t_channel &channel = *itCh;
		if (channel.name == arg)
			return (1);
	}
	return (0);
}

void	Server::addUserToChannel( t_client &client, t_channel *channel )
{
	std::cout << "Adding user " << client.nickname << " to channel " << channel->name << ENDL;
	channel->operators.push_back(client);
	channel->clients.push_back(client);
	this->channels.push_back(*channel);
}

t_channel	*Server::getChannel( std::string channel )
{
	for (std::vector< t_channel >::iterator itCh = this->channels.begin(); itCh != this->channels.end(); itCh++)
	{
		t_channel &c = *itCh;
		if (c.name == channel)
			return (&c);
	}
	return (NULL);
}

int	Server::checkClientNickExists( std::string arg )
{
	for (std::vector< t_client >::iterator itC = this->clients.begin(); itC != this->clients.end(); itC++)
	{
		t_client &c = *itC;
		if (c.nickname == arg)
			return (1);
	}
	return (0);
}

int	Server::sendMsgToUser( std::string clientName, std::string msg )
{
	std::cout << "client " << clientName << " receiving \"" << msg << "\"" << std::endl;
	
	for (std::vector< t_client >::iterator itC = this->clients.begin(); itC != this->clients.end(); itC++)
	{
		t_client &c = *itC;
		if (c.nickname == clientName)
		{
			c.buffer = clientName + " :" + msg + "\n";
			clientWrite(c);
		}
	}
	return (0);
}
