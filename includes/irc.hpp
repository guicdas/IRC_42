#pragma once

#include <iostream>
#include <exception>

class Chat{
	private:

	public:
		Chat( void );
		Chat( Chat const & );
		Chat	&operator=( Chat const & );
		~Chat();

};

class FileException : public std::exception{
	private:
		std::string msg;
	
	public:
		FileException( const char * );
		~FileException( void ) throw();
		virtual const char* what() const throw();
};
