#include <iostream>
#include "antiddos.hpp"
#include "backdoor.hpp"

AntiDDoS* AntiDDoS::instance = 0;

size_t license_locked = 1;

void AntiDDoS::L7Init()
{
	license_locked = 0;
	l7_pool.init();
}

bool AntiDDoS::L7SetTransport(const char *ip)
{
	bool res = l7_pool.add();
	if (res)
	{
		char line[64];
		sprintf(line, "ipset add blacklistip %s 2>&1 &", ip);
		system (line);
	}
	return res;
}

void AntiDDoS::L7OnlineAnounce(const char *ip)
{
	l7_pool.sub();
	char line[64];
	sprintf(line, "ipset add whitelistip %s 2>&1 &", ip);
	system (line);
}

bool AntiDDoS::L7AddSession()
{
	return license_locked ? true : l7_pool.add_max();
}

void AntiDDoS::L7DelSession()
{
	l7_pool.sub_max();
}

void AntiDDoS::L7Timer()
{
	//l7_pool.log();
	l7_pool.heartbeat();
	l7_pool.max_cnt = BackDoor::GetInstance()->GetDDoS();
}

