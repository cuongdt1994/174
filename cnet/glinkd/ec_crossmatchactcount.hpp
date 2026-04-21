
#ifndef __GNET_EC_CROSSMATCHACTCOUNT_HPP
#define __GNET_EC_CROSSMATCHACTCOUNT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_CrossMatchActCount : public GNET::Protocol
{
	#include "ec_crossmatchactcount"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
