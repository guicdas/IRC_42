#include "../includes/irc.hpp"

void	putInBuf(t_client *c, int code, std::string extra)
{
	switch (code){
		case 432:
			c->buffer = "@time : GUIrcserv 432" + c->nickname + " :Erroneous nickname"; break;
		case 403:
			c->buffer = "@time : GUIrcserv 403" +  c->nickname + extra + " :No such channel\n"; break;
		case 324:
			c->buffer = "@time : GUIrcserv 324" + c->nickname + extra;
		case 412:
			c->buffer = "@time : GUIrcserv 412" + c->nickname + " :No text to send"; break;
		case 411:
			c->buffer = "@time : GUIrcserv 411" + c->nickname + " :No recipient given (PRIVMSG)"; break;
		case 401:
			c->buffer = "@time : GUIrcserv 401" + c->nickname + " :No such nick/channel"; break;
		default:
			c->buffer = "@time : GUIrcserv " + c->nickname + " !... NICK :" + extra + "\n"; break;
	}
}
