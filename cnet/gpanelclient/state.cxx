#include "callid.hxx"

#ifdef WIN32
#include <winsock2.h>
#include "gnproto.h"
#include "gncompress.h"
#else
#include "protocol.h"
#include "binder.h"
#endif

namespace GNET
{

static GNET::Protocol::Type _state_GPanelClient[] = 
{
	PROTOCOL_PANELCHALLENGE,
	PROTOCOL_PANELRESPONSE_RE,
	PROTOCOL_MYSQLSTORAGE_RE,
	RPC_GMYSQLSTORAGE,
};

GNET::Protocol::Manager::Session::State state_GPanelClient(_state_GPanelClient,
						sizeof(_state_GPanelClient)/sizeof(GNET::Protocol::Type), 3600);


};

