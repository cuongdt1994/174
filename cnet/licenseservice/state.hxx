#ifndef __GNET_LICENSESERVICE_STATE
#define __GNET_LICENSESERVICE_STATE

#ifdef WIN32
#include "gnproto.h"
#else
#include "protocol.h"
#endif

namespace GNET
{

extern GNET::Protocol::Manager::Session::State state_LicChallenge;

extern GNET::Protocol::Manager::Session::State state_LicValidate;

extern GNET::Protocol::Manager::Session::State state_LicGame;

};

#endif
