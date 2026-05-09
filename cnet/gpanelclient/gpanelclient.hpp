#ifndef __GNET_GPANELCLIENT_HPP
#define __GNET_GPANELCLIENT_HPP

#include "protocol.h"
#include "thread.h"

namespace GNET
{

class GPanelClient : public Protocol::Manager
{
	static GPanelClient instance;
	size_t		accumulate_limit;
	Session::ID	sid;
	bool		conn_state;
	Thread::Mutex	locker_state;
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);
	void OnAbortSession(const SockAddr &sa);
	void OnCheckAddress(SockAddr &) const;
public:
	static GPanelClient *GetInstance() { return &instance; }
	std::string Identification() const { return "GPanelClient"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	GPanelClient() : accumulate_limit(0), conn_state(false), locker_state("GPanelClient::locker_state") { }

	bool SendProtocol(const Protocol &protocol) { return conn_state && Send(sid, protocol); }
	bool SendProtocol(const Protocol *protocol) { return conn_state && Send(sid, protocol); }
	Octets client_key;
};

};
#endif
