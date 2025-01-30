#include "../includes/irc.hpp"

void	putInBuf( Client &c, int code, std::string extra , std::string command )
{
	c.buffer += "ircserv "; 
	switch (code){
		case 324:
			c.buffer += "324 " + c.getNick() + " " + extra;							break;
		case 331:
			c.buffer += "331 " + c.getNick() + " " + extra;							break;
		case 401:
			c.buffer += "401 " + c.getNick() + ERR_NOSUCHNICK;						break;
		case 403:
			c.buffer += "403 " + c.getNick() + ERR_NOSUCHCHANNEL;					break;
		case 404:
			c.buffer += "404 " + c.getNick() + " :Cannot send to nick/channel";		break;
		case 411:
			c.buffer += "411 " + c.getNick() + " :No recipient given (PRIVMSG)";	break;
		case 412:
			c.buffer += "412 " + c.getNick() + " :No text to send";					break;
		case 431:
			c.buffer += "431 " + c.getNick() + " :No nickname given";				break;
		case 432:
			c.buffer += "432 " + c.getNick() + " :Erroneous nickname";				break;
		case 433:
			c.buffer += "433 " + c.getNick() + " :Nickname is already in use";		break; //client.args.at(1)
		case 441:
			c.buffer += "442 " + c.getNick() + ERR_USERNOTINCHANNEL;					break;
		case 442:
			c.buffer += "441 " + c.getNick() + ERR_NOTONCHANNEL; 					break;
		case 461:
			c.buffer += "461 " + c.getNick() + " " + command + ERR_NEEDMOREPARAMS;	break;
		case 462:
			c.buffer += "462 " + c.getNick() + ERR_ALREADYREGISTERED;				break;
		case 464:
			c.buffer += "464 " + c.getNick() + ERR_PASSWDMISMATCH;					break;
		case 482:
			c.buffer += "482 " + c.getNick() + ERR_CHANOPRIVSNEEDED;					break;
		default:
			if (c.getNick().size() < 1)
				c.buffer += command + " setting nick to: " + extra;
			else
				c.buffer += c.getNick() + " " + command + " " + extra;
			break;
	}
	c.buffer += "\n";
}

void	Server::addUserToChannel( Client &client, Channel *channel )
{
	std::cout << "Adding user " << client.getNick() << " to channel " << channel->name << ENDL;
	channel->operators.push_back(client);
	channel->clients.push_back(client);
	this->channels.push_back(*channel);
}

Channel	*Server::getChannel( std::string channel )
{
	for (std::vector< Channel >::iterator itCh = this->channels.begin(); itCh != this->channels.end(); itCh++)
	{
		Channel &c = *itCh;
		if (c.name == channel)
			return (&c);
	}
	return (NULL);
}

Client	&Server::getClient( std::string client )
{
	for (std::vector< Client >::iterator itC = this->clients.begin(); itC != this->clients.end(); itC++)
	{
		Client &c = *itC;
		if (c.getNick() == client)
			return (c);
	}
	throw (FileException("Client not found"));
}


void	Server::eraseClientFromAllChannels( Client &client )
{
	for (std::vector< Channel >::iterator itCh = this->channels.begin(); itCh != this->channels.end(); itCh++)
	{
		Channel &c = *itCh;
		eraseClientFromChannel(client, getChannel(c.name));
	}
}

int	Server::sendMsgToUser( std::string clientName, std::string msg )
{
	std::cout << "client " << clientName << " receiving \"" << msg << "\"" << std::endl;
	
	for (std::vector< Client >::iterator itC = this->clients.begin(); itC != this->clients.end(); itC++)
	{
		Client &c = *itC;
		if (c.getNick() == clientName)
		{
			c.buffer = clientName + " :" + msg + "\n";
			clientWrite(c);
		}
	}
	return (0);
}

void	listChannelMembers( Client &c, Channel *ch)
{
	for (std::vector< Client >::iterator itC = ch->clients.begin(); itC != ch->clients.end(); itC++)
	{
		Client &client = *itC;
		buf(client, 0, "", ch->name); // verif se e client
		clientWrite(c);
	}
}


void	eraseClientFromChannel( Client &client, Channel *channel )
{
	for (std::vector< Client >::iterator itC = channel->clients.begin(); itC != channel->clients.end(); itC++)
	{
		Client &c = *itC;
		if (c.getNick() == client.getNick())
		{
			/*if (operator)
				channel.operators*/
			channel->clients.erase(itC);
		}
		else
		{
			buf(c, 0, c.getNick() + " :Leaving", "PART");
			clientWrite(c);
		}
	}
}

int	sendMsgToChannel( Channel &channel, std::string client, std::string msg )
{
	std::cout << "channel " << channel.name << " receiving \"" << msg << "\"" << std::endl;
	
	for (std::vector< Client >::iterator itC = channel.clients.begin(); itC != channel.clients.end(); itC++)
	{
		Client &c = *itC;
		if (c.getNick() != client)
		{
			buf(c, 0, c.getNick() + " :" + msg + ".\n", "PRIVMSG");
			clientWrite(c);
		}
	}
	return (0);
}