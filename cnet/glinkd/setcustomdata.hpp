
#ifndef __GNET_SETCUSTOMDATA_HPP
#define __GNET_SETCUSTOMDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
namespace GNET
{

class SetCustomData : public GNET::Protocol
{
	#include "setcustomdata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// Send to delivery, check localsid by delivery
		if ( custom_data.size() < 176 || custom_data.size() > 1024 ) return;
		this->localsid=sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);		

	}
};

};

#endif
