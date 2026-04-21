
#ifndef __GNET_COUNTRYBATTLEGETMAP_RE_HPP
#define __GNET_COUNTRYBATTLEGETMAP_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gcountrybattledomain"

extern int country_battle_counter;

namespace GNET
{

class CountryBattleGetMap_Re : public GNET::Protocol
{
	#include "countrybattlegetmap_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(country_battle_counter)
		{
			for ( int i = 0;i < domains.size(); ++i )
				for ( int j = 0;j < domains[i].country_playercnt.size(); ++j )
					if( domains[i].country_playercnt[j] )
						domains[i].country_playercnt[j] += domains[i].country_playercnt[j] / country_battle_counter;
		}
		GLinkServer::GetInstance()->Send(localsid, this);
	}
};

};

#endif
