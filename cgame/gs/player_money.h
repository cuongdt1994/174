#ifndef __ONLINEGAME_GS_PLAYER_MONEY_H__
#define __ONLINEGAME_GS_PLAYER_MONEY_H__

class gplayer_money
{
public:
	enum 
	{
		MAX_GOLD = 2000000000,
		MAX_SILVER = 2000000000,
	};
	
public:
	long long ll_gold;
	long long ll_silver;
	
public:
	gplayer_money() : ll_gold(0) , ll_silver(0) {}
	~gplayer_money() {}
	
	void Init();
	void HeartBeat();
	
	bool ConvertMoney( unsigned int value );
	bool SpendMoney( unsigned int value );
	bool SyncMoney( unsigned int value );
	
	unsigned long long GetMoney( );

public:	
// gold begin
	inline void UpdateGold() 
	{ 
		if (ll_gold > MAX_GOLD) 
		{
			ll_gold = MAX_GOLD; 
		}
		
		if (ll_gold < 0)
		{
			ll_gold = 0;
		}
	}
	
	inline bool AddGold( unsigned int value )
	{
		if ( (ll_gold + value) <= MAX_GOLD )
		{
			ll_gold += value;
			UpdateGold();
			return true;
		}
		return false;
	}
	
	inline bool SpendGold( unsigned int value )
	{
		if ( (ll_gold - value) >= 0 )
		{
			ll_gold -= value;
			UpdateGold();
			return true;
		}
		return false;
	}
	
	inline void SetGold( unsigned int value ) 
	{ 
		ll_gold = value; 
		UpdateGold(); 
	}
	
	inline unsigned int GetGold() 
	{ 
		UpdateGold(); 
		unsigned int v = ll_gold;
		return v;
	}
// gold end
public:
// silver begin
	inline void UpdateSilver() 
	{ 
		if (ll_silver > MAX_SILVER) 
		{
			ll_silver = MAX_SILVER; 
		}
		
		if (ll_silver < 0)
		{
			ll_silver = 0;
		}
	}
	
	inline bool AddSilver( unsigned int value )
	{
		if ( (ll_silver + value) <= MAX_SILVER )
		{
			ll_silver += value;
			UpdateSilver();
			return true;
		}
		return false;
	}
	
	inline bool SpendSilver( unsigned int value )
	{
		if ( (ll_silver - value) >= 0 )
		{
			ll_silver -= value;
			UpdateSilver();
			return true;
		}
		return false;
	}
	
	inline void SetSilver( unsigned int value ) 
	{ 
		ll_silver = value; 
		UpdateSilver(); 
	}
	
	inline unsigned int GetSilver() 
	{ 
		UpdateSilver();
		unsigned int v = ll_silver;
		return v;
	}
// silver end
public:


};














#endif