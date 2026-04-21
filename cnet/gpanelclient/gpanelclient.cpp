
#include "gpanelclient.hpp"
#include "state.hxx"
namespace GNET
{

GPanelClient GPanelClient::instance;

const Protocol::Manager::Session::State* GPanelClient::GetInitState() const
{
	return &state_GPanelClient;
}

void GPanelClient::OnAddSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	if (conn_state)
	{
		Close(sid);
		return;
	}
	conn_state = true;
	this->sid = sid;
	//TODO
	
	printf("GPanelClient::OnAddSession: sid = %d \n", sid);
}

void GPanelClient::OnDelSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	//TODO
	
	printf("GPanelClient::OnDelSession: sid = %d \n", sid);
}

void GPanelClient::OnAbortSession(const SockAddr &sa)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	//TODO
	printf("GPanelClient::OnAbortSession: sid = %d \n", sid);
}

void GPanelClient::OnCheckAddress(SockAddr &sa) const
{
	//TODO
	//printf("GPanelClient::OnCheckAddress: sid = %d \n", sid);
}

};
