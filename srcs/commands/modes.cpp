#include "../includes/irc.hpp"

static void usermode( Client &c ){
    if (c.args.at(1) != c.getNick())
		throw (502);
	//If <modestring> is not given, 
    if (c.args.size() > 1)
        throw (221); //the RPL_UMODEIS (221) numeric is sent back containing the current modes of the target user.
    //execute mode
}

static int  checkPrivileges( Client &c ){
    //std::iterator :: operators
	(void)c;
    return (1);
}

static void channelmode( Client &c ){

   // if (checkChannelNameExists(c.args.at(1)) == 1)
	//	throw (403); //client at arg 1
     //If <modestring> is not given, 
    if (c.args.size() > 1)
        throw (324); //client at arg 1
    if (checkPrivileges(c))
        throw (482);
    //execute mode
}

int	Server::mode( Client &client ){
	try{
		//if (checkClientNickExists(client.args.at(1)) == 1)
			usermode(client);
		//else if (checkChannelNameExists(client.args.at(1)) == 1)
			channelmode(client);
        //else
            throw (401); //ou 403?
	}
	catch (std::exception &e){
		buf(client, 2, "", "NICK");
		//buf(client, (int)e.what(), "", "MODE");
	}
	return (0);
}