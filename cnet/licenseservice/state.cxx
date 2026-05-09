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

static GNET::Protocol::Type _state_LicChallenge[] = 
{
	PROTOCOL_LICENSECHALLENGE,
	PROTOCOL_LICENSELOGIN,
	PROTOCOL_LICENSEQUIT,
};

GNET::Protocol::Manager::Session::State state_LicChallenge(_state_LicChallenge,
						sizeof(_state_LicChallenge)/sizeof(GNET::Protocol::Type), 3600);

static GNET::Protocol::Type _state_LicValidate[] = 
{
	PROTOCOL_LICENSELOGIN_RE,
	PROTOCOL_LICENSEDATA,
	PROTOCOL_LICENSEQUIT,
};

GNET::Protocol::Manager::Session::State state_LicValidate(_state_LicValidate,
						sizeof(_state_LicValidate)/sizeof(GNET::Protocol::Type), 3600);

static GNET::Protocol::Type _state_LicGame[] = 
{
	PROTOCOL_LICENSEDATA,
	PROTOCOL_LICENSEDATA_RE,
	PROTOCOL_LICENSEQUIT,
};

GNET::Protocol::Manager::Session::State state_LicGame(_state_LicGame,
						sizeof(_state_LicGame)/sizeof(GNET::Protocol::Type), 3600);


};

