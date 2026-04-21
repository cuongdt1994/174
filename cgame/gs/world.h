#ifndef __ONLINEGAME_GS_WORLD_H__
#define __ONLINEGAME_GS_WORLD_H__

#include <map>
#include <hashtab.h>
#include <amemory.h>
#include <timer.h>
#include <threadpool.h>
#include <common/types.h>
#include <set>
#include <glog.h>

#include "gimp.h"
#include "grid.h"
#include "msgqueue.h"
#include "io/msgio.h"
#include "terrain.h"
#include "template/itemdataman.h"
#include "worldmanager.h"
#include "npcgenerator.h"
#include "commondata.h"
#include "actobject.h"
#include "usermsg.h"
#include "staticmap.h"

namespace NPCMoveMap
{
	class CMap;
}
class trace_manager2;

class MsgDispatcher;
class GSvrPool;
class CNPCGenMan;

class  world_data_ctrl
{
//�����������������ݿ��ƣ�������һϵ�е��麯��
	public:
	virtual ~world_data_ctrl() {}
	virtual world_data_ctrl * Clone() = 0;
	virtual void Reset() = 0;
	virtual void Tick(world * pPlane) = 0;
	virtual void BattleFactionSay(int faction, const void * buf, unsigned int size, char emote_id, const void * aux_data, unsigned int dsize, int self_id, int self_level){}
	virtual void BattleSay(const void * buf, unsigned int size){};
	virtual void OnSetCommonValue(int key, int value){}
	virtual void OnTriggerSpawn(int controller_id){}
	virtual void OnClearSpawn(int controller_id){}
	virtual void OnServerShutDown(){}
	//���ɻ������
	virtual int GetFactionId(){ return 0; }
	virtual bool LevelUp(){ return false; }
	virtual bool SetTechPoint(unsigned int tech_index){ return false; }
	virtual bool ResetTechPoint(world * pPlane, unsigned int tech_index){ return false; }
	virtual bool Construct(world * pPlane, int id, int accelerate){ return false; }
	virtual bool HandInMaterial(int id, unsigned int count){ return false; }
	virtual bool HandInContrib(int contrib){ return false; }
	virtual bool MaterialExchange(unsigned int src_index,unsigned int dst_index,int material){ return false; }
	virtual bool Dismantle(world * pPlane, int id){ return false; }
	virtual bool GetInfo(int roleid, int cs_index, int cs_sid){ return false; }
	//��սս�����
	virtual void UpdatePersonalScore(bool offense, int roleid, int combat_time, int attend_time, int dmg_output, int dmg_output_weighted, int dmg_endure, int dmg_output_npc){}
	virtual void OnPlayerDeath(gplayer * pPlayer, const XID & killer, int player_soulpower, const A3DVECTOR & pos){}
	virtual bool PickUpFlag(gplayer * pPlayer){ return false;}
	virtual bool HandInFlag(gplayer * pPlayer){ return false;}
	virtual void UpdateFlagCarrier(int roleid, const A3DVECTOR & pos){}
	virtual void OnTowerDestroyed(world * pPlane, bool offense, int tid){}
	virtual void OccupyStrongHold(int mine_tid, gplayer* pPlayer){};
	virtual bool GetStrongholdNearby(bool offense, const A3DVECTOR &opos, A3DVECTOR &pos, int & tag){return false;}
	virtual bool GetPersonalScore(bool offense, int roleid, int& combat_time, int& attend_time, int& kill_count, int& death_count, int& country_kill_count,int& country_death_count){ return false; }
	virtual bool GetCountryBattleInfo(int & attacker_count, int & defender_count){ return false; }
	virtual void GetStongholdState(int roleid, int cs_index, int cs_sid){}
	virtual bool GetLiveShowResult(int roleid, int cs_index, int cs_sid, world* pPlane){ return false; }

	//ս��ս�����
	virtual void UpdatePersonalScore(int roleid, int kill, int death, int score){}
	virtual void AddChariot(int type, int chariot) {}
	virtual void DelChariot(int type, int chariot) {}
	virtual void GetChariots(int type, std::unordered_map<int, int> & chariot_map) {}

	//�������ս�����
	virtual int PlayerTransmitInMNFaction(gplayer_imp * pImp, int index, A3DVECTOR &pos){return S2C::ERR_MNFACTION_NOT_IN_BATTLE;}
};

class world
{
	typedef abase::hashtab<int,int,abase::_hash_function,abase::fast_alloc<> >	query_map;//�û��Ĳ�ѯ��
	extern_object_manager w_ext_man;	//���������������϶�����Ϣ�Ĺ�����

	grid 	w_grid;
	int 	w_index;	//�������������
	query_map w_player_map;	//��ҵĲ��ұ�
	query_map w_npc_map;	//�ڱ��ص��ⲿnpc��ѯ��
	int	w_pmap_lock;	//player ��userid -> index id��map��
	int	w_nmap_lock;	//npc �ⲿnpc-->�������ӵĲ�ѯ��
	int 	w_message_counter[GM_MSG_MAX];	//��Ϣ��Ŀ�ļ�����
	npc_generator 	w_npc_gen; //�������ɹ�����
	
	map_generator * w_map_generator;
	CTerrain * w_terrain;
	NPCMoveMap::CMap * w_movemap;
	trace_manager2 * w_traceman;
public:
	struct off_node_t{
		int idx_off;
		int x_off;
		int z_off;
		off_node_t(grid & grid,int offset_x,int offset_y):x_off(offset_x),z_off(offset_y)
		{
			idx_off = offset_y*grid.reg_column + offset_x;
		}
		bool operator==(const off_node_t & rhs) const 
		{
			return rhs.idx_off == idx_off;
		}
	};
	abase::vector<off_node_t,abase::fast_alloc<> > w_off_list;
	int	w_near_vision;		//��ʱû��ʹ�ã����������
	int	w_far_vision;		//��Զ�ľ��������ǵķ�Χ
	int	w_true_vision;		//��ȫ���ӵķ�Χ,��ʱû��ʹ��
	int	w_plane_index;		//�������е�λ������
	int	w_player_count;		//����������ҵ���Ŀ
	float	w_vision;		//��Ұ��Χ����far_vision��Ӧ�ľ���
	world_manager * w_world_man;
	int 	w_obsolete;		//��managerʹ���������
	int 	w_life_time;	//worldʣ������ʱ��,-1��������,0�����Ѿ�����,��managerʹ���������
	instance_hash_key w_ins_key;	//����ʹ�õ�instance_key �� ��managerʹ���������
	int	w_activestate;		//����״̬ 0:δ���� 1:���� 2:��ȴ  ��manager������
	int	w_index_in_man;		//�ڹ������е�������ΪMsgQueue2��manager��ʹ��
	int	w_create_timestamp;	//������ʱ�������managerʹ�� 
	int	w_destroy_timestamp;	//ɾ����ʱ�������managerʹ�� �������еĸ����������Ч��
	int	w_ins_kick;		//�Ƿ��߳�instance key�����ϵ����
	int	w_battle_result;	//��ս���õ� ս�����
	int	w_offense_goal;		//ս������Ŀ��
	int	w_offense_cur_score;	//ս�������÷�
	int	w_defence_goal;		//ս���ط�Ŀ��
	int	w_defence_cur_score;	//ս���ط��÷�
	int 	w_end_timestamp; 	//ս������ʱ�䣬����ʱ��ʱ��������Ҷ������Զ��߳� 
					//ֻ��battle_result��Ч��,��ֵ�Żᱻʹ��

	int 	w_player_node_lock;	//��������ݣ����ڸ��������е�����б�ʱ����
	cs_user_map w_player_node_list;	//�������
	common_data w_common_data;	//������ͨ������
	abase::vector<char>	w_collision_flags;	//npc dyn mine ��ײ�Ƿ񼤻�ı�־
	int w_scene_service_npcs_lock;
	abase::static_multimap<int, int> w_scene_service_npcs;	//ȫ��������npc��ʹ�÷���ʱ�޾�������
public:
	void SetCommonValue(int key,int value, bool notify_world_ctrl = true);
	int GetCommonValue(int key);
	int ModifyCommonValue(int key, int offset);
	void AddPlayerNode(gplayer * pPlayer);
	void DelPlayerNode(gplayer * pPlayer);
	void SyncPlayerWorldGen(gplayer* pPlayer);
	void AddSceneServiceNpc(int tid, int id);
	void DelSceneServiceNpc(int tid, int id);
	void GetSceneServiceNpc(abase::vector<int> & list);
private:	
	void CommonDataNotify(int key, int value);
public:
//��ʼ������
	world();
	~world();
	bool 	Init(int world_index);
	void 	InitManager(world_manager * man) { w_world_man = man;}
	bool 	InitNPCGenerator(CNPCGenMan & npcgen);
	bool    InitNPCGenerator(CNPCGenMan & ctrldata, npcgen_data_list& npcgen_list);
	bool    InitNPCGeneratorByClone(CNPCGenMan & ctrldata, npcgen_data_list& npcgen_list);
	bool	TriggerSpawn(int condition, bool notify_world_ctrl = true);
	bool 	ClearSpawn(int condition, bool notify_world_ctrl = true);
	void 	InitTimerTick();
	bool 	CreateGrid(int row,int column,float step,float startX,float startY);
	int	BuildSliceMask(float near,float far);			//���������ѯ����Ҫ��mask

	void 	DuplicateWorld(world * dest) const;	//�������磬����NPC������ .....
	
	inline world_manager * GetWorldManager() { return w_world_man;}
	//����һ��NPC���ݣ�����һ����������NPC�ṹ
	inline gnpc 	*AllocNPC() 
	{ 
		gnpc *pNPC = w_world_man->AllocNPC(); 
		if(pNPC) pNPC->plane = this;
		return pNPC;
	}
	inline void 	FreeNPC(gnpc* pNPC) 
	{ 
		ASSERT(pNPC->plane == this);
		pNPC->plane = NULL;
		return w_world_man->FreeNPC(pNPC); 
	}

	inline bool CheckPlayerDropCondition()
	{
		return w_world_man->CheckPlayerDropCondition();
	}

	//���ô�npcΪ�ⲿnpc
	inline void 	SetNPCExtern(gnpc * pNPC)
	{
		spin_autolock alock(w_nmap_lock); 
		w_npc_map.put(pNPC->ID.id,GetNPCIndex(pNPC));
	}

	inline int 	GetNPCExternID(int id)
	{
		spin_autolock alock(w_nmap_lock);
		query_map::pair_type p = w_npc_map.get(id);
		if(!p.second) return -1;
		return *p.first;
	}

	inline void 	EraseExternNPC(int id)
	{
		spin_autolock alock(w_nmap_lock);
		w_npc_map.erase(id);
	}


	//����һ��Matter���ݣ�����һ����������Matter�ṹ
	inline gmatter *AllocMatter() 
	{ 
		gmatter * pMatter = w_world_man->AllocMatter(); 
		if(pMatter) pMatter->plane = this;
		return pMatter;
	}
	inline void 	FreeMatter(gmatter *pMatter) 
	{ 
		ASSERT(pMatter->plane == this);
		pMatter->plane = NULL;
		return w_world_man->FreeMatter(pMatter); 
	}

	//	����/ɾ����ҺͶ���ĺ���
	//����һ��������ݣ�������һ����������ҽṹ
	inline gplayer *AllocPlayer() 
	{ 
		gplayer * pPlayer = w_world_man->AllocPlayer(); 
		if(pPlayer) 
		{
			pPlayer->plane = this;
			interlocked_increment(&w_player_count);
		}
		return pPlayer;
	}

	inline void 	FreePlayer(gplayer * pPlayer)
	{
		w_world_man->PlayerLeaveThisWorld(w_plane_index,pPlayer->ID.id);
		ASSERT(pPlayer->plane == this);
		interlocked_decrement(&w_player_count);
		pPlayer->plane = NULL;
		return w_world_man->FreePlayer(pPlayer);
	}

	//AttachPlayer,DetachPlayer ��������ڱ�gs���л��߲���ʱʹ��
	inline void AttachPlayer(gplayer * pPlayer)
	{
		ASSERT(pPlayer->plane == NULL);
		pPlayer->plane = this;
		interlocked_increment(&w_player_count);
	}

	inline void DetachPlayer(gplayer * pPlayer)
	{
		w_world_man->PlayerLeaveThisWorld(w_plane_index,pPlayer->ID.id);
		ASSERT(pPlayer->plane == this);
		interlocked_decrement(&w_player_count);
		pPlayer->plane = NULL;
	}

	inline int GetPlayerInWorld() 
	{
		return w_player_count; 
	}

	inline void InsertPlayerToMan(gplayer *pPlayer) 
	{ 
		w_world_man->InsertPlayerToMan(pPlayer);
	}
	inline void RemovePlayerToMan(gplayer *pPlayer) 
	{ 	
		w_world_man->RemovePlayerToMan(pPlayer);
	}

	int InsertPlayer(gplayer *);		//����λ�ã�����һ�����������У����ز��������������
	int InsertNPC(gnpc*);			//����λ�ã�����һ�����������У����ز��������������
	int InsertMatter(gmatter *);		//����λ�ã�����һ�����������У����ز��������������
	
	void RemovePlayer(gplayer *pPlayer); 	//���������Ƴ�һ�����󣬲�free
	void RemoveNPC(gnpc *pNPC);		//���������Ƴ�һ�����󣬲�free
	void RemoveMatter(gmatter *pMatter);	//���������Ƴ�һ�����󣬲�free

	//�ӹ��������Ƴ�NPC�����ڲ��ڳ����е�npc
	inline void RemoveNPCFromMan(gnpc * pNPC)
	{
		w_world_man->RemoveNPCFromMan(pNPC);
	}

	inline void RemoveMatterFromMan(gmatter * pMatter)
	{
		w_world_man->RemoveMatterFromMan(pMatter);
	}

	bool IsPlayerExist(int player_id);	//��ѯ����Ƿ��������У������������������У�

	void Release();
	
	int GetMineMatterType(int id);
	int GetNpcType(int id);

public:
//	ȡ�����Ե�inline����
	inline gmatter * GetMatterByIndex(unsigned int index) const  { return w_world_man->GetMatterByIndex(index);}
	inline gplayer*  GetPlayerByIndex(unsigned int index) const   {return w_world_man->GetPlayerByIndex(index);}
	inline gnpc* 	 GetNPCByIndex(unsigned int index) const   { return w_world_man->GetNPCByIndex(index);}
	inline unsigned int GetPlayerIndex(gplayer *pPlayer)  const  { return w_world_man->GetPlayerIndex(pPlayer);}
	inline unsigned int GetMatterIndex(gmatter *pMatter)  const  { return w_world_man->GetMatterIndex(pMatter);}
	inline unsigned int GetNPCIndex(gnpc *pNPC)  const  { return w_world_man->GetNPCIndex(pNPC);}
	inline grid&	 GetGrid() { return w_grid;}
	inline extern_object_manager & GetExtObjMan() { return  w_ext_man;}
	inline const rect & GetLocalWorld() { return w_grid.local_region;}
	
	//inline unsigned int GetObjCountPool() { return w_world_man->GetCapacity(); }
	
	inline bool PosInWorld(const A3DVECTOR & pos)
	{
		return w_grid.IsLocal(pos.x,pos.z);
	}

	inline bool MapPlayer(int uid,int index) { 
		spin_autolock alock(w_pmap_lock); 
		return w_player_map.put(uid,index);
	}
	
	inline int UnmapPlayer(int uid) {
		spin_autolock alock(w_pmap_lock);
		return w_player_map.erase(uid);
	}
	
	inline int FindPlayer(int uid) {
		spin_autolock alock(w_pmap_lock);
		query_map::pair_type p = w_player_map.get(uid);
		if(!p.second) return -1;
		return *p.first;
	}

	inline gplayer * GetPlayerByID(int uid)
	{
		int index = FindPlayer(uid);
		if(index < 0) return NULL;
		return GetPlayerByIndex(index);
	}

	inline void ExtManRefresh(int id, const A3DVECTOR &pos, const extern_object_manager::object_appear & obj)
	{
		w_ext_man.Refresh(id,pos,obj);
	}

	inline void ExtManRefreshHP(int id, const A3DVECTOR &pos, int hp)
	{
		w_ext_man.RefreshHP(id,pos,hp);
	}

	inline void ExtManRemoveObject(int id)
	{
		w_ext_man.RemoveObject(id);
	}
	
	inline int GetPlayerCount()
	{
		spin_autolock alock(w_pmap_lock); 
		return w_player_map.size();
	}

	//
	enum
	{
		QUERY_OBJECT_STATE_ACTIVE = 0x01,
		QUERY_OBJECT_STATE_ZOMBIE = 0x02,
		QUERY_OBJECT_STATE_DISCONNECT = 0x04,
	};
	struct object_info
	{
		int state;
		A3DVECTOR pos;
		float body_size;
		int race;
		int faction;
		int level;
		int hp;
		int mp;
		int max_hp;
		int invisible_degree;
		int anti_invisible_degree;
		unsigned int object_state;		//��player npc��Ч
		unsigned int object_state2;		//��player npc��Ч
		int mafia_id;
		int master_id;//���������id
		// Novos
		int realm_level;
		int soulpower;
	};

	bool QueryObject(const XID & id,object_info & info);	//��ѯһ�����������״̬

public:
	int RebuildMapRes();
	inline float GetHeightAt(float x, float z)
	{
		if(w_terrain) return w_terrain->GetHeightAt(x, z);
		return w_world_man->GetMapRes().GetUniqueTerrain()->GetHeightAt(x, z);	
	}

	inline NPCMoveMap::CMap * GetMoveMap()
	{
		if(w_movemap) return w_movemap;
		return w_world_man->GetMapRes().GetUniqueMoveMap();
	}

	inline trace_manager2 * GetTraceMan()
	{
		if(w_traceman) return w_traceman;
		return w_world_man->GetMapRes().GetUniqueTraceMan();
	}
	
	inline const map_generator* GetMapGen() const { return w_map_generator; }

	inline int GetBlockID(float x, float z) 
	{
		return w_map_generator ? w_map_generator->GetBlockID(x,z,this) : 0;
	}
	inline int GetRoomIndex(float x, float z) const { return w_map_generator ? w_map_generator->GetRoomIndex(x,z) : 0;}
	inline bool GetTownPosition(gplayer_imp *pImp, const A3DVECTOR &opos, A3DVECTOR &pos, int & tag) const { return w_map_generator ? w_map_generator->GetTownPosition(pImp,opos,pos,tag) : false; }
	inline bool SetIncomingPlayerPos(gplayer * pPlayer, const A3DVECTOR & origin_pos) const { return w_map_generator ? w_map_generator->SetIncomingPlayerPos(pPlayer,origin_pos) : false; }	
private:
	//������Ϣ���ڲ�����
	gobject * locate_object_from_msg(const MSG & msg);		//������Ϣ��λ����
	void 	try_dispatch_extern_msg(const MSG & msg);
public:
	void RunTick();		//��manager���Ƶ��û����ڲ��Զ�����
	void ResetWorld();	//�������磬ֻ�и����Ż�������������������������еĹ���ȣ�
	void DumpMessageCount();

	void SetWorldCtrl(world_data_ctrl * ctrl);
	world_data_ctrl * w_ctrl;
private:
	friend class MsgQueue;
public:

//������Ϣ�ĺ���
	/*
	 *	ָ��Ŀ��ķ�����Ϣ���Ѿ�����˶���ȷ������˲�����Ҫ�ٴν��в���
	 */
	int DispatchMessage(gobject * obj, const MSG & message);

	/*
	 *	��ʱ����һ����Ϣ����һ����Ϣ
	 *	delay_tick��50msΪ��λ
	 *	��config.h�ﶨ��������delay_tick MAX_MESSAGE_DELAY
	 *	PostMessageQueue ����SendLazyMessage(message,0)��ʵ�ֵ�
	 *	���delay_tick������ô��ʱ���͸���Ϣ��δ�����
	 */
	inline void PostLazyMessage(const MSG & message, unsigned int delay_tick)
	{
		w_world_man->PostMessage(this,message,delay_tick);
	}

	/*
	 *	����һ����Ϣ�����ܻ��ӳٷ��ͣ�Ҳ���ܻ�Ͽ�ķ���
	 *	��ÿ��SendMessage֮�󣬶������Ƿ�����Ϣ�ȴ�����
	 */
	inline void PostLazyMessage(const MSG & message)
	{
		w_world_man->PostMessage(this,message);
	}

	/*
	 *	��һ��player������Ϣ
	 */
	inline void SendPlayerMessage(unsigned int count, int * player_list, const MSG & msg)
	{
		w_world_man->PostPlayerMessage(this,player_list,count,msg);
	}

	/*
	 *	��һ��ID������Ϣ
	 */
	void SendMessage(const XID * first, const XID * last, const MSG & msg)
	{
		w_world_man->PostMessage(this,first,last,msg);
	}

	/*
	 *	����һ����Ϣ��Զ�̷�����
	 *	�ַ�������Զ�̷��������
	 */
	void SendRemoteMessage(int id,const MSG & msg)
	{
		w_world_man->SendRemoteMessage(id, msg);
	}

	/*
	
	*  ��ȡplayer_list�Ĵ�С��Ҳ���Ǳ��㲥������
	*/
	int GetSpherePlayerListSize(const A3DVECTOR& target,float fRadius);
	
	/*
	 *	��һ����Ϣ���͵��͸�����rect�����ཻ��Զ�̷�������
	 */
	int BroadcastSvrMessage(const rect & rt,const MSG & message,float extend_size)
	{
		return w_world_man->BroadcastSvrMessage(rt,message,extend_size);
	}

	/*
	 *	�㲥��Ϣ�����վ��뽫��ת������Χ�����ж�?
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	mask���ڹ�����Ϣ���ն���
	 *	��������㲥ʱ�����Զ��ж��Ƿ�Ҫת���������ķ�������
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastMessage(const MSG & message,float fRadius,int mask); 		

	/*
	 *	�㲥������Ϣ�����վ��뽫��ת������Χ�����ж���
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastLocalBoxMessage(const MSG & message,const rect & rt);

	// Novo Fulano 

	//int GetSpherePlayerListSize(const MSG & message,const rect & rt);
	int GetBoxPlayerListSize(const A3DVECTOR & target, const rect & rt);


	// Movo Fulano

	/*
	 *	�㲥������Ϣ�����վ��뽫��ת������Χ�����ж���
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	mask���ڹ�����Ϣ���ն���(�Ѿ���ȡ��)
	 *	��������㲥ʱ�����Զ��ж��Ƿ�Ҫת���������ķ�������
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastSphereMessage(const MSG & message,const A3DVECTOR & target, float fRadius);

	/*
	 *	�㲥������Ϣ�����վ��뽫��ת������Χ�����ж���,�ö������������
	 *	������ʼ������message�У��յ���target
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	mask���ڹ�����Ϣ���ն���
	 *	��������㲥ʱ�����Զ��ж��Ƿ�Ҫת���������ķ�������
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastCylinderMessage(const MSG & message,const A3DVECTOR & target, float fRadius);

	/*
	 *	�㲥׵����Ϣ����׵�еĶ�����յ������Ϣ
	 *	Բ׶��Բ�ļ�Ϊ��Ϣ�ķ�����
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	mask���ڹ�����Ϣ���ն���
	 *	��������㲥ʱ�����Զ��ж��Ƿ�Ҫת���������ķ�������
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastTaperMessage(const MSG & message,const A3DVECTOR & target,float fRadius,float cos_halfangle);


	/*
	 *	ͬBroadcastMessage��Ψһ��������ֻ�ڱ�������Ӧ��ת������
	 *	BroadcastMessage�ı��ط�����ͨ�����ñ�������ɵ�
	 */
	int BroadcastLocalMessage(const MSG & message,float fRadius,int mask);

	/*
	 *	�㲥������Ϣ�����վ��뽫��ת������Χ�����ж���
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	mask���ڹ�����Ϣ���ն���
	 *	��������㲥ʱ�����Զ��ж��Ƿ�Ҫת���������ķ�������
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastLocalSphereMessage(const MSG & message,const A3DVECTOR & target, float fRadius);

	/*
	 *	�㲥������Ϣ�����վ��뽫��ת������Χ�����ж���,�ö������������
	 *	������ʼ������message�У��յ���target
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	mask���ڹ�����Ϣ���ն���
	 *	��������㲥ʱ�����Զ��ж��Ƿ�Ҫת���������ķ�������
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastLocalCylinderMessage(const MSG & message,const A3DVECTOR & target, float fRadius);

	/*
	 *	�㲥׵����Ϣ����׵�еĶ�����յ������Ϣ
	 *	Բ׶��Բ�ļ�Ϊ��Ϣ�ķ�����
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	mask���ڹ�����Ϣ���ն���
	 *	��������㲥ʱ�����Զ��ж��Ƿ�Ҫת���������ķ�������
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastLocalTaperMessage(const MSG & message,const A3DVECTOR & target,float fRadius,float cos_halfangle);

	/*
	 *	�ַ���Ϣ�����������Ӧ�����û�����
	 *	�ַ���Ϣ�Ĳ�����SendMessage�ﱻ�Զ�����
	 */
	int DispatchMessage(const MSG & message);

	/*
	 *	����ĳ��λ��Ӧ�������ĸ������� ,�����ҵ��ĵ�һ��������
	 *	����-1��ʾû���ҵ�
	 */
	int GetSvrNear(const A3DVECTOR & pos) const
	{
		return w_world_man->GetServerNear(pos);
	}

	/*
	 *	����ĳ��λ��Ӧ�����ڴ��ͼ�е��Ǹ����������������������ڴ���
	 *	����-1��ʾû���ҵ�
	 */
	int GetGlobalServer(const A3DVECTOR & pos) const
	{
		return w_world_man->GetServerGlobal(pos);
	}
	
	/*
	 *	���һ�������ֵ���Ƿ���Ȼ����ȷ��Χ��,�ڲ�ʹ�õĺ���.
	 */
	inline static bool check_index(const grid * g,int x,int z, const world::off_node_t &node)
	{
		int nx = x + node.x_off;
		if(nx < 0 || nx >= g->reg_column) return false;
		int nz = z + node.z_off;
		if(nz < 0 || nz >= g->reg_row) return false;
		return true;
	}

	void BattleFactionSay(int faction, const void * msg, unsigned int size, char emote_id=0, const void * aux_data=NULL, unsigned int dsize=0, int self_id=0, int self_level=0);
	void BattleSay(const void * msg, unsigned int size);
	void InstanceSay(const void * msg, unsigned int size, bool middle, const void* data=NULL, unsigned int dsize = 0);
	void InstanceFarCryChat(char channel, const void * msg, unsigned int size, bool middle, const void* data, unsigned int dsize);

public:
//ģ�庯���ӿ�
	/*
	 *	��һ���������������Ӽ��ƶ�,�жϸö����뿪����Щ���ӵ���Ұ,�������Ӧ��enter��leave��������
	 */
	template <typename ENTER,typename LEAVE>
	inline void MoveBetweenSlice(slice * pPiece, slice * pNewPiece,ENTER enter,LEAVE leave)
	{
		int i;
		grid * pGrid = &GetGrid();
		int ox,oy,nx,ny;
		pGrid->GetSlicePos(pPiece,ox,oy);
		pGrid->GetSlicePos(pNewPiece,nx,ny);
		float vision = w_vision + pGrid->slice_step - 1e-3;
		float dis = pNewPiece->Distance(pPiece);
		if(dis > vision)
		{
			//������޷����� ����Ҫ�����뿪����Ĳ���,�����ѭ����û���жϱ���
			leave(pPiece);
			enter(pNewPiece);
			if(dis > vision*2)
			{
				for(i = 0; i < w_far_vision; i ++)
				{
					const world::off_node_t &node = w_off_list[i];
					slice * pTmpPiece = pPiece + node.idx_off;
					leave(pTmpPiece);
				}

				for(i = 0; i < w_far_vision; i ++)
				{
					const world::off_node_t &node = w_off_list[i];
					slice * pTmpPiece = pNewPiece + node.idx_off;
					enter(pTmpPiece);
				}
				return ;
			}
		}

		for(i = 0; i < w_far_vision; i ++)
		{
			const world::off_node_t &node = w_off_list[i];
			if(check_index(pGrid,ox,oy,node)) 
			{
				slice * pTmpPiece = pPiece + node.idx_off;
				if(pTmpPiece->Distance(pNewPiece) > vision && pTmpPiece->IsInWorld())
				{
					leave(pTmpPiece);
					//�뿪�����slice
				}
			}

			if(check_index(pGrid,nx,ny,node))
			{
				slice * pTmpPiece = pNewPiece + node.idx_off;
				if(pTmpPiece->Distance(pPiece) > vision && pTmpPiece->IsInWorld())
				{
					enter(pTmpPiece);
				}
			}
		}
	}

	/*
	 *	ɨ�踽�����е�С����,����Ԥ���ķ�Χ��ɨ��,���ﲻ���и����Ƿ��ڱ����������ж� 
	 */
	template <typename FUNC >
	inline void ForEachSlice(slice * pStart, FUNC func,int vlevel = 0)
	{
		int total = vlevel?w_near_vision:w_far_vision;
		int slice_x,slice_z;
		GetGrid().GetSlicePos(pStart,slice_x,slice_z);
		for(int i = 0; i <total; i ++)
		{
			off_node_t &node = w_off_list[i]; 
			int nx = slice_x + node.x_off; 
			int nz = slice_z + node.z_off; 
			if(nx < 0 || nz < 0 || nx >= GetGrid().reg_column || nz >= GetGrid().reg_row) continue;
			slice * pNewPiece = pStart+ node.idx_off;
			func(i,pNewPiece);
		}
	}

	/*
	 * ����λ�úͷ�Χɨ�踽�����е�С��,�������ε�����Ӧ�Ĵ�����������
	 * ����������жϸ����Ƿ��ڵ�ǰ��������,���򲻻ᷢ�͵�func������
	 */
	template <typename FUNC>
	inline void ForEachSlice(const A3DVECTOR &pos, float fRadius, FUNC func)
	{
		grid * pGrid = &GetGrid();
		float fx = pos.x - pGrid->grid_region.left;
		float fz = pos.z - pGrid->grid_region.top;
		float inv_step = pGrid->inv_step;
		int ofx1 = (int)((fx - fRadius) * inv_step);
		int ofx2 = (int)((fx + fRadius) * inv_step);
		int ofz1 = (int)((fz - fRadius) * inv_step);
		int ofz2 = (int)((fz + fRadius) * inv_step);
		if(ofx1 < 0) ofx1 = 0;
		if(ofx2 >= pGrid->reg_column) ofx2 = pGrid->reg_column -1;
		if(ofz1 < 0) ofz1 = 0;
		if(ofz2 >= pGrid->reg_row) ofz2 = pGrid->reg_row - 1;

		slice * pPiece = pGrid->GetSlice(ofx1,ofz1);
		for(int i = ofz1;i <= ofz2; i ++,pPiece += pGrid->reg_column)
		{
			slice * pStart = pPiece;
			for(int j = ofx1; j <= ofx2; j ++, pStart++)
			{
				if(pStart->IsInWorld()) func(pStart,pos);
			}
		}
	}

	template <typename FUNC>
	inline void ForEachSlice(const A3DVECTOR &pos, const rect & rt, FUNC func)
	{
		grid * pGrid = &GetGrid();
		
		float inv_step = pGrid->inv_step;
		int ofx1 = (int)((rt.left   - pGrid->grid_region.left) * inv_step);
		int ofx2 = (int)((rt.right  - pGrid->grid_region.left) * inv_step);
		int ofz1 = (int)((rt.top    - pGrid->grid_region.top ) * inv_step);
		int ofz2 = (int)((rt.bottom - pGrid->grid_region.top ) * inv_step);
		if(ofx1 < 0) ofx1 = 0;
		if(ofx2 >= pGrid->reg_column) ofx2 = pGrid->reg_column -1;
		if(ofz1 < 0) ofz1 = 0;
		if(ofz2 >= pGrid->reg_row) ofz2 = pGrid->reg_row - 1;

		slice * pPiece = pGrid->GetSlice(ofx1,ofz1);
		for(int i = ofz1;i <= ofz2; i ++,pPiece += pGrid->reg_column)
		{
			slice * pStart = pPiece;
			for(int j = ofx1; j <= ofx2; j ++, pStart++)
			{
				if(pStart->IsInWorld()) func(pStart,pos);
			}
		}
	}

	template <typename FUNC>
	inline void ForEachSlice(const A3DVECTOR &pos1, const A3DVECTOR & pos2, FUNC func)
	{
		rect rt(pos1,pos2);
		ForEachSlice(pos1,rt,func);
	}

	template <int foo>
	inline static void InspirePieceNPC(slice * pPiece,int tick)
	{
		int timestamp = pPiece->idle_timestamp;
		if(tick - timestamp < 40)
		{
			return;
		}
		pPiece->Lock();
		if(tick - pPiece->idle_timestamp < 40)	//��piece����ÿ��������һ��
		{
			pPiece->Unlock();
			return;
		}
		pPiece->idle_timestamp = tick;
		gnpc * pNPC = (gnpc*)(pPiece->npc_list);
		while(pNPC)
		{
			pNPC->idle_timer = NPC_IDLE_TIMER;
			pNPC = (gnpc*)(pNPC->pNext);
		}
		pPiece->Unlock();
	}
	/*
	 *	ɨ�踽�����е�С����,����Ԥ���ķ�Χ��ɨ��,���ﲻ���и����Ƿ��ڱ����������ж� 
	 */
	template <int foo>
	inline void InspireNPC(slice * pStart, int vlevel = 0)
	{
		int total = vlevel?w_near_vision:w_far_vision;
		int slice_x,slice_z;
		GetGrid().GetSlicePos(pStart,slice_x,slice_z);
		int tick = g_timer.get_tick();
		InspirePieceNPC<0>(pStart,tick);

		for(int i = 0; i <total; i ++)
		{
			off_node_t &node = w_off_list[i]; 
			int nx = slice_x + node.x_off; 
			int nz = slice_z + node.z_off; 
			if(nx < 0 || nz < 0 || nx >= GetGrid().reg_column || nz >= GetGrid().reg_row) continue;
			slice * pNewPiece = pStart+ node.idx_off;
			InspirePieceNPC<0>(pNewPiece,tick);
		}
	}

private:
	void CheckGSvrPoolUpdate();						//��鵱ǰ���ߵ���Ϸ�������б�
	void ConnectGSvr(int index, const char * ipaddr, const char * unixaddr);	//��������һ̨������ 

	/*
	 * ������Ƕ���ˢ����Ϸ����������,������Ϊ�̳߳ص�һ������������
	 * �������ĵط�����صĶ�ʱ������
	 */

	int _message_handle_count;				//��¼��ǰmessage������Ƕ�� 

private:
	//��Ϣ�������ĺ���������������ֻ����Ϣ�����������ϺͶϿ�ʱ����
};

template <typename WRAPPER>
inline int WrapObject(WRAPPER & wrapper,controller * ctrl, gobject_imp * imp, dispatcher * runner)
{
	ctrl->SaveInstance(wrapper);
	imp->SaveInstance(wrapper);
	runner->SaveInstance(wrapper);
	return 0;
}

template <typename WRAPPER,typename OBJECT>
inline int RestoreObject(WRAPPER & wrapper,OBJECT *obj,world * pPlane)
{
	controller * ctrl =NULL;
	gobject_imp * imp = NULL;
	dispatcher * runner = NULL;

	ctrl = substance::DynamicCast<controller>(substance::LoadInstance(wrapper));
	if(ctrl) imp = substance::DynamicCast<gobject_imp>(substance::LoadInstance(wrapper));
	if(imp) runner = substance::DynamicCast<dispatcher>(substance::LoadInstance(wrapper));
	if(!ctrl || !runner ||!imp) 
	{
		delete imp;
		delete ctrl;
		return -1;
	}
	obj->imp = imp;
	imp->_runner = runner;
	imp->_commander = ctrl;
	imp->Init(pPlane,obj);
	ctrl->Init(imp);
	runner->init(imp);
	//�Ժ���Ҫ����ReInit
	return 0;
}

extern abase::timer	g_timer;

#endif

