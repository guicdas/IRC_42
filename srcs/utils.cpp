#include "../includes/irc.hpp"

std::vector<std::string>	ircSplit(std::string str, char c)
{
	std::vector< std::string >	args;
	std::stringstream			input(str);
	std::string					arg;

	while (std::getline(input, arg, c))
		args.push_back(arg);

	return (args);
}