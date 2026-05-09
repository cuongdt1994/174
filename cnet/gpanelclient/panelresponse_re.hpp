
#ifndef __GNET_PANELRESPONSE_RE_HPP
#define __GNET_PANELRESPONSE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "mysqlstorage.hpp"
#include "gpanelclient.hpp"

namespace GNET
{

class PanelResponse_Re : public GNET::Protocol
{
	#include "panelresponse_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (result == ERR_SUCCESS)
		{
		
			static const char * ConstStr = "SELECT COUNT(*) FROM users";
			
			Octets str;
			str.replace(ConstStr, strlen(ConstStr));
			
			MySQLStorage MySqlSrt(1,0,0);
			MySqlSrt.input_str.reserve(1);
			
			MySqlSrt.input_str.push_back(str);
			
			
			GPanelClient::GetInstance()->Send(sid, MySqlSrt);
			printf("PanelResponse_Re::Process: result = %d \n", result);
		}
		// TODO
	}
};

};

#endif
