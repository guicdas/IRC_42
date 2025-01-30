#include "../includes/irc.hpp"

void	putInBuf( t_client &c, int code, std::string extra , std::string command )
{
	c.buffer += "ircserv "; 
	switch (code){
		case 324:
			c.buffer += "324 " + c.nickname + " " + extra;							break;
		case 401:
			c.buffer += "401 " + c.nickname + ERR_NOSUCHNICK;						break;
		case 403:
			c.buffer += "403 " + c.nickname + ERR_NOSUCHCHANNEL;					break;
		case 404:
			c.buffer += "404 " + c.nickname + " :Cannot send to nick/channel";		break;
		case 411:
			c.buffer += "411 " + c.nickname + " :No recipient given (PRIVMSG)";		break;
		case 412:
			c.buffer += "412 " + c.nickname + " :No text to send";					break;
		case 431:
			c.buffer += "431 " + c.nickname + " :No nickname given";				break;
		case 432:
			c.buffer += "432 " + c.nickname + " :Erroneous nickname";				break;
		case 433:
			c.buffer += "433 " + c.nickname + " :Nickname is already in use";		break; //client.args.at(1)
		case 441:
			c.buffer += "442 " + c.nickname + ERR_USERNOTINCHANNEL;					break;
		case 442:
			c.buffer += "441 " + c.nickname + ERR_NOTONCHANNEL; 					break;
		case 461:
			c.buffer += "461 " + c.nickname + " " + command + ERR_NEEDMOREPARAMS;	break;
		case 462:
			c.buffer += "462 " + c.nickname + ERR_ALREADYREGISTERED;				break;
		case 464:
			c.buffer += "464 " + c.nickname + ERR_PASSWDMISMATCH;					break;
		case 482:
			c.buffer += "482 " + c.nickname + ERR_CHANOPRIVSNEEDED;					break;
		default:
			if (c.nickname.size() < 1)
				c.buffer += command + " setting nick to: " + extra;
			else
				c.buffer += c.nickname + " " + command + " " + extra;
			break;
	}
	c.buffer += "\n";
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

t_client	&Server::getClient( std::string client )
{
	for (std::vector< t_client >::iterator itC = this->clients.begin(); itC != this->clients.end(); itC++)
	{
		t_client &c = *itC;
		if (c.nickname == client)
			return (c);
	}
	throw (FileException("Client not found"));
}


void	Server::eraseClientFromAllChannels( t_client &client )
{
	for (std::vector< t_channel >::iterator itCh = this->channels.begin(); itCh != this->channels.end(); itCh++)
	{
		t_channel &c = *itCh;
		eraseClientFromChannel(client, getChannel(c.name));
	}
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
