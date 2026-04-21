
#ifndef __GNET_GCODEXREQUESTSTORAGE_RE_HPP
#define __GNET_GCODEXREQUESTSTORAGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GCodexRequestStorage_Re : public GNET::Protocol
{
	#include "gcodexrequeststorage_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
