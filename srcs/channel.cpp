#include "../includes/irc.hpp"

void	eraseClientFromChannel( t_client &client, t_channel *channel )
{
	for (std::vector< t_client >::iterator itC = channel->clients.begin(); itC != channel->clients.end(); itC++)
	{
		t_client &c = *itC;
		if (c.nickname == client.nickname)
		{
			/*if (operator)
				channel.operators*/
			channel->clients.erase(itC);
		}
		else
		{
			buf(c, 0, c.nickname + " :Leaving", "PART");
			clientWrite(c);
		}
	}
}

int	isClientInChannel( t_client &client, t_channel *channel )
{
	for (std::vector< t_client >::iterator itC = channel->clients.begin(); itC != channel->clients.end(); itC++)
	{
		t_client &c = *itC;
		if (c.nickname == client.nickname)
			return (1);
	}
	return (0);
}

int	sendMsgToChannel( t_channel &channel, std::string client, std::string msg )
{
	std::cout << "channel " << channel.name << " receiving \"" << msg << "\"" << std::endl;
	
	for (std::vector< t_client >::iterator itC = channel.clients.begin(); itC != channel.clients.end(); itC++)
	{
		t_client &c = *itC;
		if (c.nickname != client)
		{
			buf(c, 0, c.nickname + " :" + msg + ".\n", "PRIVMSG");
			clientWrite(c);
		}
	}
	return (0);
}

int	isClientOp( t_client &c, t_channel *channel )
{
	for (std::vector< t_client >::iterator itC = channel->operators.begin(); itC != channel->operators.end(); itC++)
	{
		t_client &client = *itC;
		if (client.nickname == c.nickname)
			return (1);
	}
	return (0);
}

void	listChannelMembers( t_client &c, t_channel *ch)
{
	for (std::vector< t_client >::iterator itC = ch->clients.begin(); itC != ch->clients.end(); itC++)
	{
		t_client &client = *itC;
		buf(c, 0, "", ch->name);
		clientWrite(c);
	}
}