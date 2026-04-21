#include "player_money.h"


void gplayer_money::Init()
{ 
	ll_gold = 0; 
	ll_silver = 0; 
}

void gplayer_money::HeartBeat()
{
	
}

bool gplayer_money::ConvertMoney( unsigned int value )
{
	if ( value && value <= MAX_GOLD && ll_silver <= MAX_SILVER )
	{
		if ( (ll_silver + value) > MAX_SILVER )
		{
			value = MAX_SILVER - ll_silver;
		}
		
		if ( value <= MAX_GOLD )
		{
			unsigned int m_gold = GetGold();
			unsigned int m_silver = GetSilver();				

			m_gold -= value;
			m_silver += value;

			// Correção de bug de gold negativo
			if(m_gold < 0 || m_gold > MAX_GOLD)
			m_gold = 0;
	
			
			if ( (m_gold+m_silver) == (ll_gold+ll_silver) )
			{
				SetGold(m_gold);
				SetSilver(m_silver);
				return true;
			}
		}
	}
	return false;
}

bool gplayer_money::SpendMoney( unsigned int value )
{
	return (value <= (ll_gold+ll_silver) && (SpendSilver(value) || (ConvertMoney(value - ll_silver) && SpendSilver(value))));
}

bool gplayer_money::SyncMoney( unsigned int value )
{
	unsigned int ll_val = (ll_gold+ll_silver);
	if ( ll_val >= value )
	{
		ll_val -= value;
		return SpendMoney( ll_val );
	}
	return false;
}

unsigned long long gplayer_money::GetMoney()
{
	return ( GetGold() + GetSilver() );
}

