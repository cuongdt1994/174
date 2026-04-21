
#ifndef __GNET_MYSQLSTORAGE_HPP
#define __GNET_MYSQLSTORAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class MySQLStorage : public GNET::Protocol
{
	#include "mysqlstorage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
