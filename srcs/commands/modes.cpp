#include "../includes/irc.hpp"

int	Server::mode( Client &client ){
	try{
		if (client.args.at(1)[0] == '#')
		{
			std::string ChannelName = client.args.at(1);
			Channel channel = getChannel(ChannelName); //throws 403

			if (client.args.size() < 2)//If <modestring> is not given, the RPL_CHANNELMODEIS (324)
				throw (324);
			if (channel.isOperatorInChannel(client) == 0)
       			throw (482);
			
		}
		/*else if (doesClientNickExist(client.args.at(1)) == 1)
			if (c.args.at(1) != c.getNick())
				throw (502);
			//If <modestring> is not given, 
    		if (c.args.size() > 1)
        		throw (221); //the RPL_UMODEIS (221) numeric is sent back containing the current modes of the target user.
    		
		else
           throw (401);*/
	}
	catch(int e){
		buf(client, e, "", "MODE");
	}
	return (0);
}