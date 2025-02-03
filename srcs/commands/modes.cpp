#include "../includes/irc.hpp"

int	Server::mode( Client &client ){
	try{
		if (client.args.at(1)[0] == '#')
		{
			std::string ChannelName = client.args.at(1);
			Channel channel = getChannel(ChannelName); //throws 403

			if (client.args.size() < 2)//If <modestring> is not given, the RPL_CHANNELMODEIS (324)
				throw (324);
			if (channel.isOperatorInChannel(client.getNick()) == 0)
       			throw (482);
			
		}
		else
		{
			Client &c = getClient(client.args.at(1)); //throws 401
			if (c.getNick() != client.getNick())
				throw (501);

    		if (c.args.size() < 2)
			{}//If <modestring> is not given numeric is sent back containing the current modes of the target user.
				// mandar os modes do user ??? 

			/*If <modestring> is given, the supplied modes will be applied, 
			and a MODE message will be sent to the user containing the changed modes.
			If one or more modes sent are not implemented on the server, the server MUST apply the modes that are implemented, 
			and then send the ERR_UMODEUNKNOWNFLAG (501) in reply along with the MODE message.*/
		}
	}
	catch(int e){
		buf(client, e, "", "MODE");
	}
	return (0);
}

int	Server::invite( Client &client ){
	try
	{
		if (client.args.size() < 2)
			throw (336);
		std::string invitedName = client.args.at(1);
		std::string ChannelName = client.args.at(2);

		Channel &channel = getChannel(ChannelName);// throws 403
		Client	invited = getClient(invitedName);// throws 401
		if (channel.isClientInChannel(client.getNick()) == 0)
			throw(442);
		if (channel.isInviteOnly(channel))
			checkClientOp(client, channel);//ver se funciona so com o client (em vez de comparar o nome)
		if (channel.isClientInChannel(invited.getNick()) == 1)
			throw(443);
		
		buf(client, 341, invitedName + " " + ChannelName, "INVITE");
		buf(invited, 341, client.getNick() + " " + ChannelName, " INVITE");
	}
	catch(int e){
		buf(client, e, "", "INVITE");
	}
	return (0);
}
