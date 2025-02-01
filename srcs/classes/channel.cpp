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