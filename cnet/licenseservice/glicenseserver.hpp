#ifndef __GNET_GLICENSESERVER_HPP
#define __GNET_GLICENSESERVER_HPP

#include "protocol.h"

namespace GNET
{

class GLicenseServer : public Protocol::Manager
{
	static GLicenseServer instance;
	size_t		accumulate_limit;
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);
public:
	static GLicenseServer *GetInstance() { return &instance; }
	std::string Identification() const { return "GLicenseServer"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	GLicenseServer() : accumulate_limit(0) { }
};

};
#endif
