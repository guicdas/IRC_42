#include "../includes/irc.hpp"

void	Channel::checkClientInChannel( Client &client )
{
	for (std::vector< Client >::iterator itC = this->clients.begin(); itC != this->clients.end(); itC++)
	{
		Client &c = *itC;
		if (c.getNick() == client.getNick())
			throw (441); //args 1
	}
}

void	checkClientOp( Client &c, Channel &channel )
{
	for (std::vector< Client >::iterator itC = channel.operators.begin(); itC != channel.operators.end(); itC++)
	{
		Client &operat = *itC;
		if (c.getNick() == operat.getNick())
			throw (482); //args 1
	}
}

