#ifndef __ONLINEGAME_GS_ARENAOFAURORA_MANAGER_H__
#define __ONLINEGAME_GS_ARENAOFAURORA_MANAGER_H__

#include "instance_manager.h"

/*
struct arena_of_aurora_param
{
	int arena_id;

	int red_count;
	int red[6];

	int blue_count;
	int blue[6];

	int end_timestamp;
};
*/

class arenaofaurora_world_manager : public instance_world_manager 
{
private:

public:
	arenaofaurora_world_manager():instance_world_manager()
	{
		_idle_time = 600;
		_life_time = -1;
	}
	virtual int GetWorldType(){ return WORLD_TYPE_ARENAOFAURORA; }
	virtual world * CreateWorldTemplate();
	virtual world_message_handler * CreateMessageHandler();
	virtual void Heartbeat();
	virtual void FinalInit(const char * servername);
	virtual void OnDeliveryConnected();

	virtual void UserLogin(int cs_index,int cs_sid,int uid,const void * auth_data, unsigned int auth_size, bool isshielduser, char flag);
	virtual void SetFilterWhenLogin(gplayer_imp * pImp, instance_key * ikey);
	virtual void PlayerAfterSwitch(gplayer_imp * pImp);
	virtual void GetLogoutPos(gplayer_imp * pImp, int & world_tag ,A3DVECTOR & pos);
	virtual bool GetTownPosition(gplayer_imp *pImp, const A3DVECTOR &opos, A3DVECTOR &pos, int & tag);
	virtual void RecordTownPos(const A3DVECTOR &pos,int faction);
	virtual void SetIncomingPlayerPos(gplayer * pPlayer, const A3DVECTOR & origin_pos, int special_mask);
	
	virtual void TransformInstanceKey(const instance_key::key_essence & key, instance_hash_key & hkey)
	{
		hkey.key1 = key.key_level4;
		hkey.key2 = 0;
	}
	virtual int CheckPlayerSwitchRequest(const XID & who,const instance_key * key,const A3DVECTOR & pos,int ins_timer);
	virtual world * GetWorldInSwitch(const instance_hash_key & ikey,int & world_index,int );
	virtual bool CreateArenaOfAurora(const GMSV::EC_GSArena &);
};

class arenaofaurora_world_message_handler : public instance_world_message_handler
{
protected:
	virtual ~arenaofaurora_world_message_handler(){}
	virtual void PlayerPreEnterServer(gplayer * pPlayer, gplayer_imp * pimp,instance_key &  ikey);
public:
	arenaofaurora_world_message_handler(instance_world_manager * man):instance_world_message_handler(man) {}
};

#endif
