#include "../includes/irc.hpp"

void	putInBuf( Client &c, int code, std::string extra , std::string command )
{
	c.buffer += "ircserv "; 
	switch (code){
		case 324:
			c.buffer += "324 " + c.getNick() + " " + extra;							break;
		case 331:
			c.buffer += "331 " + c.getNick() + " " + extra;							break;
		case 336:
			c.buffer += "336 " + c.getNick() + RPL_INVITELIST;						break;
		case 341:
			c.buffer += "341 " + c.getNick() + " " + extra + " " + command;			break;
		case 401:
			c.buffer += "401 " + c.getNick() + ERR_NOSUCHNICK;						break;
		case 404:
			c.buffer += "404 " + c.getNick() + ERR_NOSUCHNICK;						break;
		case 411:
			c.buffer += "411 " + c.getNick() + ERR_NORECIPIENT;						break;
		case 412:
			c.buffer += "412 " + c.getNick() + ERR_NOTEXTTOSEND;					break;
		case 431:
			c.buffer += "431 " + c.getNick() + ERR_NONICKNAMEGIVEN;					break;
		case 432:
			c.buffer += "432 " + c.getNick() + ERR_ERRONEUSNICKNAME;				break;
		case 433:
			c.buffer += "433 " + c.getNick() + ERR_NICKNAMEINUSE;					break; //
		case 441:
			c.buffer += "442 " + c.getNick() + ERR_USERNOTINCHANNEL;				break;
		case 442:
			c.buffer += "441 " + c.getNick() + ERR_NOTONCHANNEL; 					break;
		case 443:
			c.buffer += "443 " + c.getNick() + ERR_USERONCHANNEL; 					break;
		case 461:
			c.buffer += "461 " + c.getNick() + ERR_NEEDMOREPARAMS;					break;
		case 462:
			c.buffer += "462 " + c.getNick() + ERR_ALREADYREGISTERED;				break;
		case 464:
			c.buffer += "464 " + c.getNick() + ERR_PASSWDMISMATCH;					break;
		case 482:
			c.buffer += "482 " + c.getNick() + ERR_CHANOPRIVSNEEDED;				break;
		case 501:
			c.buffer += "501 " + c.getNick() + ERR_USERSDONTMATCH;					break;
		case 1001:
			c.buffer = ":" + c.getId() + " " + command + " :" + extra; 				break;
		case 1003:
			c.buffer = ":" + c.getId() + " " + command + " " + extra; 				break;
		case 1004:
			c.buffer = "Wrong characters in channel name!" ; 						break;

		default:
			if (c.getNick().size() < 1)
				c.buffer += command + " setting nick to: " + extra;
			else
				c.buffer += c.getNick() + " " + command + " " + extra;
			break;
	}
	c.buffer += "\n";
}

Channel	&Server::getChannel( std::string channel )
{
	for (std::vector< Channel >::iterator itCh = this->channels.begin(); itCh != this->channels.end(); itCh++)
	{
		Channel &c = *itCh;
		if (c.getName() == channel)
			return (c);
	}
	throw (403);
}

Client	&Server::getClient( std::string nick )
{
	for (std::vector< Client >::iterator itC = this->clients.begin(); itC != this->clients.end(); itC++)
	{
		Client &c = *itC;
		if (c.getNick() == nick)
			return (c);
	}
	throw (401);
}


void	Server::eraseClientFromAllChannels( Client &client )
{
	for (std::vector< Channel >::iterator itCh = this->channels.begin(); itCh != this->channels.end(); itCh++)
	{
		Channel &c = *itCh;
		c.eraseClientFromChannel(client);
	}
}

int		Server::doesChannelNameExist( std::string arg )
{
	for (std::vector< Channel >::iterator itCh = this->channels.begin(); itCh != this->channels.end(); itCh++)
	{
		Channel &channel = *itCh;
		if (channel.getName() == arg)
			return (1);
	}
	return (0);
}

