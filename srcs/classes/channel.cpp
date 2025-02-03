#include "../includes/channel.hpp"

Channel::Channel( std::string name ) : 
	name(name), 
	topic(""), 
	operators(),
	clients()
{}


Channel::Channel( Channel const &c ) {
    this->name = c.name;
    this->topic = c.topic;
    this->operators = c.operators;
    this->clients = c.clients;
}


Channel	&Channel::operator=( Channel const &c ){
	if (this != &c)
		return (*this);
	return (*this);
}

Channel::~Channel( void ){}

std::string	Channel::getTopic( void ){
	return (this->topic);
}

std::string	Channel::getName( void ){
	return (this->name);
}

bool	Channel::isInviteOnly(Channel &c){
	return (c.InviteMode);
}

void Channel::setTopic( std::string str ){
	this->topic = str;
}

void Channel::setName( std::string str ){
	this->name = str;
}

void	Channel::eraseClientFromChannel( Client &client )
{
	for (std::vector< Client >::iterator itC = this->clients.begin(); itC != this->clients.end(); itC++)
	{
		if ((*itC).getNick() == client.getNick())
		{
			/*if (operator)
				channel.operators*/
			this->clients.erase(itC);
		}
		else
		{
			buf(*itC, 0, (*itC).getNick() + " :Leaving", "PART");
			clientWrite(*itC);
		}
	}
}

int	Channel::sendMsgToChannel( std::string client, std::string msg )
{
	std::cout << "channel " << this->name << " receiving \"" << msg << "\"" << std::endl;
	
	for (std::vector< Client >::iterator itC = this->clients.begin(); itC != this->clients.end(); itC++)
	{
		if ((*itC).getNick() != client)
		{
			buf(*itC, 0, (*itC).getNick() + " :" + msg + ".\n", "PRIVMSG");
			clientWrite(*itC);
		}
	}
	return (0);
}

int		Channel::isClientInChannel( std::string nick )
{
	if (this->findNick(nick, this->clients) != this->clients.end())
		return (1);
	return (0);
}

int		Channel::isOperatorInChannel( std::string nick )
{
	if (this->findNick(nick, this->operators) != this->operators.end())
		return (1);
	return (0);
}

void	Channel::addClientToChannel( Client &client )
{
	PRINT_COLOR(MAGENTA, "Adding user " + client.getNick() + " to channel " + this->name);
	this->clients.push_back(client);
}

void	Channel::addOperatorToChannel( Client &client )
{
	PRINT_COLOR(MAGENTA, "Giving Op to " + client.getNick() + " in channel " + this->name);
	this->operators.push_back(client);
}

void	Channel::listAllMembersInChannel( Client &client )
{
	for (std::vector< Client >::iterator itC = this->clients.begin(); itC != this->clients.end(); itC++)
	{
		buf(client, 0, "", (*itC).getNick());
		clientWrite(client);
	}
}

std::vector< Client >::iterator	Channel::findNick( std::string nick, std::vector< Client > itVec)
{
	std::vector< Client >::iterator itClient = itVec.begin();

	while (itClient != itVec.end() && (*itClient).getNick() != nick)
		itClient++;
	return (itClient);
}
