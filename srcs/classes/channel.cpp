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
		Client &c = *itC;
		if (c.getNick() == client.getNick())
		{
			/*if (operator)
				channel.operators*/
			this->clients.erase(itC);
		}
		else
		{
			buf(c, 0, c.getNick() + " :Leaving", "PART");
			clientWrite(c);
		}
	}
}

int	Channel::sendMsgToChannel( std::string client, std::string msg )
{
	std::cout << "channel " << this->name << " receiving \"" << msg << "\"" << std::endl;
	
	for (std::vector< Client >::iterator itC = this->clients.begin(); itC != this->clients.end(); itC++)
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

int		Channel::isClientInChannel( std::string nick )
{
	for (std::vector<Client>::iterator itClient = this->clients.begin(); itClient != this->clients.end(); ++itClient)
	{
		if (itClient->getNick() == nick) //possivel erro
			return (1);
	}
	return (0);
}

int		Channel::isOperatorInChannel( Client &c )
{
	for (std::vector<Client>::iterator itClient = this->operators.begin(); itClient != this->operators.end(); ++itClient)
	{
		Client &Client = *itClient;
		if (c.getNick() == Client.getNick())
			return (1);
	}
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

void	Channel::listAllMembersInChannel( Client &c )
{
	for (std::vector< Client >::iterator itC = this->clients.begin(); itC != this->clients.end(); itC++)
	{
		Client &client = *itC;
		buf(c, 0, "", client.getNick());// e preciso limpar a cada vez? 
		clientWrite(c);
	}
}
