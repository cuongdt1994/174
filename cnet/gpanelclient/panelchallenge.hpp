
#ifndef __GNET_PANELCHALLENGE_HPP
#define __GNET_PANELCHALLENGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gpanelclient.hpp"
#include "panelresponse.hpp"

namespace GNET
{

class PanelChallenge : public GNET::Protocol
{
	#include "panelchallenge"

	void Process(Manager *manager, Manager::Session::ID sid)
	{		
		GPanelClient * gpc = GPanelClient::GetInstance();
		
		Octets key = nonce;
		Octets client_key = gpc->client_key;
		
		MD5Hash hmd5;
		HMAC_MD5Hash hash;
		
		// key = md5(key)
		// key = hmac_md5(key, client_key)
		
		hmd5.Digest(key); 
		hash.SetParameter(key);
		hash.Update(client_key);
		hash.Final(key);
		
		PanelResponse pr(key);
		GPanelClient::GetInstance()->Send(sid, pr);
		
		printf("PanelChallenge: Process --OK!! \n");
		return;
	}
};

};

#endif
