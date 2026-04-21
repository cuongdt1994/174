
#include "glicenseserver.hpp"
#include "state.hxx"

namespace GNET
{

GLicenseServer GLicenseServer::instance;

const Protocol::Manager::Session::State* GLicenseServer::GetInitState() const
{
	return &state_LicChallenge;
}

void GLicenseServer::OnAddSession(Session::ID sid)
{
	//TODO
}

void GLicenseServer::OnDelSession(Session::ID sid)
{
	//TODO
}

};
