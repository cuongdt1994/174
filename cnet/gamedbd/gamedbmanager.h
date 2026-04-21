#ifndef __GNET_GAMEDBMANAGER_H
#define __GNET_GAMEDBMANAGER_H
/*		this class provide management tools for gamedb, such as:
 *	   	1. initialize tab and data
 *		2. query data
 *		3. show statistic informations
 */

#include <vector>
#include <map>

#include "thread.h"
#include "conv_charset.h"
#include "statistic.h"

#include "groleforbid"
#include "grolebase"
#include "grolestatus"
#include "groleinventory"
#include "gshoplog"
#include "groledetail"
#include "roleid"
#include "stocklog"
#include "user"
#include "userid"
#include "dbconfig"
#include "map.h"
#include <storage.h>
#include "dbconfig2"

#define MAGIC_CLASS		5
#define ROLELIST_DEFAULT	0x80000000
#define MAX_ROLE_COUNT		16
#define UNAUTHORIZED_SERVER	0x90
#define CDS_MAX_USERNAME_LENGTH (40-2)
#define ZONE_NAME_MAX_LEN_DEFAULT 	10
#define ZONE_NAME_MAX_LEN_UPLIMIT	16

#define ROLE_NAME_STAT_USED	1	//�������ڱ�ʹ��
#define ROLE_NAME_STAT_OBSOLETE 2	//�����Ѿ���������

namespace GNET
{
	struct extend_prop
	{
		/* �������� */
		int vitality;           //��
		int energy;             //��
		int strength;           //��
		int agility;            //��
		int max_hp;             //���hp
		int max_mp;             //���mp
		int hp_gen;             //hp�ָ��ٶ�
		int mp_gen;             //mp�ָ��ٶ� 

		/* �˶��ٶ�*/ 
		float walk_speed;       //�����ٶ� ��λ  m/s
		float run_speed;        //�����ٶ� ��λ  m/s
		float swim_speed;       //��Ӿ�ٶ� ��λ  m/s
		float flight_speed;     //�����ٶ� ��λ  m/s

		/* ��������*/
		int attack;             //������ attack rate
		int damage_low;         //���damage
		int damage_high;        //�������damage
		int attack_speed;       //����ʱ���� ��tickΪ��λ
		float attack_range;     //������Χ
		struct                  //������������ħ���˺�
		{       
			int damage_low;
			int damage_high;
		} addon_damage[MAGIC_CLASS];

		/* ħ����� */
		int damage_magic_low;           //ħ����͹�����
		int damage_magic_high;          //ħ����߹�����
		int resistance[MAGIC_CLASS];    //ħ������

		/* �������� */  
		int defense;            //������
		int armor;              //�����ʣ�װ�׵ȼ���
		int max_ap;				//���ŭ��ֵ
	};
	struct point_t
	{
		float x;
		float y;
		float z;
		point_t() { }
		point_t(float _x,float _y,float _z) : x(_x),y(_y),z(_z) { }
	};
	struct zone_t
	{
		int group_id;
		point_t pos;
		zone_t():group_id(-1) {}
		zone_t(int gid,point_t& p):group_id(gid),pos(p) {}
	};
	struct region_t
	{
		int		id;
		float	left;
		float	right;
		float	top;
		float 	bottom;
		region_t() { }
		region_t(char _id,float _l,float _r,float _t,float _b) : id(_id),left(_l),right(_r),top(_t),bottom(_b) { }	
		bool IsEnclosed(point_t pt) { return (pt.x>left && pt.x<right) && (pt.z>bottom && pt.z<top ); }
	};
	class RoleList
	{
		unsigned int rolelist;
		int count;
		int cur_role;
	public:
		RoleList() : rolelist(0),count(0),cur_role(0) { }
		RoleList(unsigned int r) : rolelist(r),count(0),cur_role(0) { }
		~RoleList() { }
		void operator=(const RoleList& rhs) { rolelist=rhs.rolelist; }
		bool IsRoleListInitialed()
		{
			return (rolelist & ROLELIST_DEFAULT) != 0;
		}
		bool IsRoleExist(int roleid)
		{
			return (rolelist & (1<<(roleid % MAX_ROLE_COUNT))) != 0;
		}
		void InitialRoleList()
		{
			rolelist = ROLELIST_DEFAULT;
		}
		unsigned int GetRoleList() 
		{
			return rolelist;
		}
		int GetRoleCount()
		{
			if (!IsRoleListInitialed()) return -1;  //rolelist is not initialized
			count=0;
			for (int id=0;id<MAX_ROLE_COUNT;id++)
			{
				if (IsRoleExist(id)) count++;
			}
			return count;
		}
		int AddRole()
		{
			if (!IsRoleListInitialed()) { return -1; }
			if (GetRoleCount()==MAX_ROLE_COUNT) { return -1; }
			int id=0;
			for (;id<MAX_ROLE_COUNT && IsRoleExist(id);id++);
			rolelist +=(1<<id);
			return id;	
		}		
		int AddRole(int roleid)
		{
			if (!IsRoleListInitialed()) { return -1; }
			if (IsRoleExist(roleid)) { return roleid; } //the role will be overlayed
			if (GetRoleCount()==MAX_ROLE_COUNT) { return -1; }
			if (roleid<0 || roleid >MAX_ROLE_COUNT-1) { return -1;}
			rolelist +=(1<<roleid);  
			return roleid;
		}
		bool DelRole(int roleid)
		{
			if (!IsRoleListInitialed()) return false;
			if (!IsRoleExist(roleid)) return false;
			return (rolelist -= 1<<(roleid % MAX_ROLE_COUNT)) != 0;
		}
		int GetNextRole()
		{
			while (cur_role<MAX_ROLE_COUNT)
				if (IsRoleExist(cur_role++)) return cur_role-1;
			return -1;
		}
		void SeekToBegin()
		{
			cur_role=0;
		}
	};

	#define	OBJECT_COUNT	1024
	#define	MATERIAL_COUNT	8
	class GameDBManager
	{
		int areaid;
		int zoneid;
		int deleterole_timeout;
		int max_delta;
		int64_t guid;
		int adduser_on_charge;
		std::vector<region_t> gameserver_region;
		Thread::Mutex	locker_region;
		DBConfig db_config;	// ��¼��config��100�ż�¼�ķ��������ݿⴴ��ʱ��Ϳ���ʱ��
		int create_count;	// ��¼�����������󴴽���ɫ������
		DBConfig2 db_config2; //config��101�� ��¼��DB�Ƿ�Ϊ���DB������
		bool is_central_db;
		bool is_recall_server;
		bool japan_version;
		int zone_name_max_len;
		typedef std::map<int/*zoneid*/, Octets> ZoneNameMap;
		ZoneNameMap zone_name_map;
	
		typedef std::map<int, zone_t> ZONE_INFO_MAP;
		ZONE_INFO_MAP zone_pos_map;
		int zone_group_count;

		GameDBManager() 
		{
			deleterole_timeout = 604800;	// a week
			max_delta = 0;
			guid = 0;
			areaid = 0;
			zoneid = 0;
			adduser_on_charge = 1;
			create_count = 0;
			is_central_db = false;
			is_recall_server = false;
			japan_version = false;
			zone_name_max_len = ZONE_NAME_MAX_LEN_DEFAULT;
			zone_group_count = 0;
		}
	public:
		~GameDBManager() { }
		static GameDBManager* GetInstance() { static GameDBManager instance; return &instance;}
		//initialize role base information when they are created
		bool InitGameDB();
		int GetDeleteRole_Timeout() {return deleterole_timeout;}
		void InitGUID();
		int64_t GetGUID() { return guid++; }
		static int Zoneid() { return GetInstance()->zoneid; }
		static int Areaid() { return GetInstance()->areaid; }
		void SetAddUserOnChange(int value) { adduser_on_charge = value; }
		int  GetAddUserOnChange() { return adduser_on_charge; }
		bool IsFreshServer() { return db_config.open_time == 0 || time(0) - db_config.open_time < 86400; }
		//list 
		void ListUser() { }
		void ListUser(const UserID& userid) { }
		void ListRole() { }
		void ListRole(const RoleId& roleid) { }
		//find 
		int FindGameServer(const point_t& pt);
		//rase details
		bool SaveUserDefault( );
		bool SaveClsDetail(unsigned int id, char gender, GRoleDetail &role);
		bool GetClsDetail(unsigned int id, char gender,
					GRoleBase &base, GRoleStatus &status,
					GRolePocket &pocket, GRoleInventoryVector &equipment,
					GRoleStorehouse &storehouse);
		bool GetClsDetail(unsigned int id, char gender,
					GRoleBase &base, GRoleStatus &status,
					GRolePocket &pocket, GRoleInventoryVector &equipment,
					GRoleStorehouse &storehouse,
					StorageEnv::Storage * pbase,
					StorageEnv::Storage * pstatus,
					StorageEnv::Storage * pinventory,
					StorageEnv::Storage * pequipment,
					StorageEnv::Storage * pstorehouse,
					StorageEnv::Transaction& txn);
		bool GetClsPos(unsigned int id, char gender, float &posx, float &posy, float &posz);

		void UpdateTradeMoney( int money1, int money2 )
		{
			STAT_MIN5("TradeMoney", (int64_t)money1 + money2 );
		}
		void UpdateMoney(int roleid, int delta)
		{
			if(delta>max_delta)
			{
				LOG_TRACE("UpdateMoney, update max delta roleid=%d, delta=%d", roleid,delta);
				max_delta = delta;
			}
			STAT_MIN5("Money",delta);
		}
		void UpdateCash( int delta )
		{
			STAT_MIN5("Cash",delta);
		}

		void ReadDBConfig();
		void SaveDBConfig();
		void OnCreateRole()
		{
			if (++create_count >= 256) {
				create_count = 256;
				if (db_config.open_time == 0)
				{
					db_config.open_time = time(0);
					SaveDBConfig();
				}
			}
			LOG_TRACE("gamedbmanager::OnCreateRole() create_count=%d", create_count);
		}
		bool GetZoneName(int zoneid, Octets& zone_name)
		{
			ZoneNameMap::iterator it = zone_name_map.find(zoneid);
			if (it != zone_name_map.end()) {
				zone_name = it->second;
				return true;
			} else {
				return false;
			}
		}

		void SetZoneNameMaxLen(int value) { zone_name_max_len = value; }
		int LoadZoneNameConfig();
		bool IsCentralDB() { return is_central_db; }
		void SetCentralDB(bool b) { is_central_db = b; }
		bool IsRecallServer() { return is_recall_server; }
		void SetRecallServer(bool b) { is_recall_server = b; }
		bool IsJapanVersion(){ return japan_version; }

		bool IsZoneAccepted(int zoneid) { return zone_pos_map.find(zoneid) != zone_pos_map.end(); }
		bool IsAcceptedZoneListMatch(const std::vector<int>& zones)
		{
			if(zones.size() != zone_pos_map.size()) return false;
			for(unsigned int i = 0; i < zones.size(); ++i)
			{
				if(!IsZoneAccepted(zones[i]))
				{
					Log::log(LOG_ERR, "CrossRelated AnnounceCentralDelivery, zoneid %d is not accpeted, please check the gamesys.conf", zones[i]);
					return false;
				}
			}

			return true;
		}
		void GetAcceptedZoneList(std::map<int,int>& zones,int& count)
		{
			ZONE_INFO_MAP::iterator it = zone_pos_map.begin();
			ZONE_INFO_MAP::iterator ie = zone_pos_map.end();
			for(; it != ie; ++it)
			{
				zones[it->first] = it->second.group_id;
			}
			count = zone_group_count;
		}
		
		bool GetZonePos(int zoneid, point_t& pos)
		{
			ZONE_INFO_MAP::iterator it = zone_pos_map.find(zoneid);
			if(it != zone_pos_map.end()) {
				pos = it->second.pos;
				return true;
			} else {
				return false;
			}
		}
		bool InitAcceptedZoneCrossPos(const std::string accepted_zones, const std::string cross_zone_pos);
	};

	void GRoleBaseToDetail( const GRoleBase & base, GRoleDetail & detail );
	void GRoleDetailToBase( const GRoleDetail & detail, GRoleBase & base );
    unsigned int GetDataRoleId(unsigned int cls);
};
#endif

