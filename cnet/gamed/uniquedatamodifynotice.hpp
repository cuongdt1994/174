
#ifndef __GNET_UNIQUEDATAMODIFYNOTICE_HPP
#define __GNET_UNIQUEDATAMODIFYNOTICE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void OnUniqueDataModify(int worldtag, int key, int type, const void* val, unsigned int sz,const void* oldval, unsigned int osz, int retcode, int version);

namespace GNET
{

class UniqueDataModifyNotice : public GNET::Protocol
{
	#include "uniquedatamodifynotice"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		OnUniqueDataModify(worldtag, key, vtype, value.begin(), value.size(), oldvalue.begin(), oldvalue.size(), retcode, version);
	}
};

};

#endif
