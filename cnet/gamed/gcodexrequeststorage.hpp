#ifndef __GNET_GCODEXREQUESTSTORAGE_HPP
#define __GNET_GCODEXREQUESTSTORAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gcodexrequeststorage_re.hpp"

namespace GNET
{

class GCodexRequestStorage : public GNET::Protocol
{
	#include "gcodexrequeststorage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//TODO
	}
};

};

#endif
