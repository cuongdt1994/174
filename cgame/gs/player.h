#ifndef __ONLINEGAME_GS_PLAYER_H__
#define __ONLINEGAME_GS_PLAYER_H__

#include <iconv.h>
#include <time.h>
#include "object.h"
#include <common/packetwrapper.h>

struct gplayer  : public gactive_object
{
	//base_info.race
	//player��ְҵ/����/�Ա� ���λ��ʾ�Ա𣬵��ֽڱ�ʾְҵ��Ŀǰû�������趨��
	enum{
		EMPTY 		= 0,
		WAITING_LOGIN 	= 1,
		WAITING_ENTER	= 2,
		LOGIN_OK	= 3,	//��½������״̬
		WAITING_LOGOUT	= 4,
		DISCONNECTED 	= 5,
		WAITING_SWITCH  = 6,
	};
	inline gplayer * get_next() { return (gplayer*)pNext;}
	inline gplayer * get_prev() { return (gplayer*)pPrev;}
	int	cs_index;		//���ӷ���������
	int 	cs_sid;			//���ӷ�����session ID
	int	login_state;		//��ǰ��¼״̬����δ����impʱʹ��
	int 	id_mafia;		//����id
	short	effect_list[MAX_PLAYER_EFFECT_COUNT];		//�����ǵ���Ŀ��gactive_object��������
	unsigned char rank_mafia;	//�����ڽ�ɫ
	unsigned char market_id;	//��ǰ̯��
	unsigned char vehicle;		//���е�ʱ�Ľ�ͨ����
	unsigned char gm_invisible;	//�Ƿ�GM����,������ӵ����ȫ��Ұ
	unsigned short  custom_crc;	//��ǰ�ĸ��Ի�����crc
	int 	adv_data1;
	int 	adv_data2;
	int 	pariah_state;		//����״̬
	char	sec_level;		//cultivate 1-8 , 20-22 , 30-32 , 40-41 
	char 	bind_type;		//������ 1���� 2 ���� 
	unsigned short mount_color;		//���������ɫ
	int	bind_target;		//�󶨵��� ��˭
	int 	mount_id;		//�������ʲô����
	int 	spouse_id;
	int 	team_id;	//���ڶ���id(�ӳ�id),Ӧ����player_team�е�leader����һ��,��������״̬�¹㲥��Ϣ�Ƿ��͵��ж�
	int64_t	disabled_equip_mask;	//ʧЧ��װ��mask,�����Ӧλ����λ,����Ӧ��װ�������Ƿ����㼤��������������
	int		force_id;	//�������id
	int		country_id;	//����id
	unsigned short title_id;	// ��ǰ�ƺţ�Ӧ����player_title�е�curr_title_id ����һ��
	unsigned char reincarnation_times;	//ת������
	unsigned char realmlevel;	// ����ȼ�	 	
	unsigned char mafia_pvp_mask; // ����pvp Mask
	int64_t  mnfaction_id; //���Ψһ����id
	int      cash_vip_level;
	int      cash_vip_score;
	float 	 body_scale;

	bool	is_waitting_login() { return login_state == WAITING_LOGIN;}
	void Clear()
	{
		cs_sid = -1;
		cs_index = -1;
		login_state =  EMPTY;
		id_mafia = 0;
		rank_mafia = 0;
		market_id = 0;
		ID.id = 0;
		base_info.race = -1;
		custom_crc = 0;
		vehicle = gm_invisible = 0;
		adv_data1 = adv_data2 = 0;
		bind_type  =0;
		sec_level = 0;
		mount_color = 0;
		bind_target = 0;
		mount_id = 0;
		spouse_id = 0;
		team_id = 0;
		disabled_equip_mask = 0;
		force_id = 0;
		country_id = 0;
		title_id = 0;
		reincarnation_times = 0;
		realmlevel = 0;
		mafia_pvp_mask = 0;
		mnfaction_id = 0;
		cash_vip_level = 0;
		cash_vip_score      = 0;
		body_scale          = 1.0f;
		gactive_object::Clear();
	}

	template <typename WRAPPER>
	WRAPPER &Export(WRAPPER & wrapper)
	{
		gactive_object::Export(wrapper);
		wrapper << ID << pos << msg_mask << crc << 
		id_mafia << rank_mafia << market_id << 
		vehicle << gm_invisible << custom_crc <<
		base_info.race << base_info.faction <<
		base_info.level << base_info.hp <<
		base_info.max_hp << body_size << 
		cs_sid << cs_index << login_state << 
		adv_data1 << adv_data2 << pariah_state << sec_level << 
		bind_type << mount_color <<
		bind_target << mount_id << spouse_id << team_id << disabled_equip_mask << 
		force_id << country_id << title_id << reincarnation_times << realmlevel << mafia_pvp_mask << mnfaction_id << cash_vip_level << cash_vip_score << body_scale;
		return wrapper.push_back(effect_list,sizeof(effect_list));
	}

	template <typename WRAPPER>
	WRAPPER & Import(WRAPPER & wrapper)
	{
		gactive_object::Import(wrapper);
		wrapper >> ID >> pos >> msg_mask >> crc >> 
		id_mafia >> rank_mafia >> market_id >>
		vehicle >> gm_invisible >> custom_crc >> 
		base_info.race >> base_info.faction >>
		base_info.level >> base_info.hp >>
		base_info.max_hp >> body_size >>
		cs_sid >> cs_index >> login_state >> 
		adv_data1 >> adv_data2 >> pariah_state >> sec_level >>
		bind_type >> mount_color >>
		bind_target >> mount_id >> spouse_id >> team_id >> disabled_equip_mask >> 
		force_id >> country_id >> title_id >> reincarnation_times >> realmlevel >> mafia_pvp_mask >> mnfaction_id >> cash_vip_level >> cash_vip_score >> body_scale;
		return wrapper.pop_back(effect_list,sizeof(effect_list));
	}
public:
	int GetCountryId() { return country_id &0xffff; } 
	char GetEnablefashionWeapon() { return 0; } 
};

inline bool make_link_sid(gplayer * dest, link_sid & id)
{
	id.cs_id = dest->cs_index;
	id.cs_sid = dest->cs_sid;
	id.user_id = dest->ID.id;
	return id.cs_id >= 0;
}

//�����������������key���ڽ��븱������ʱ��Ҫ����key
struct  instance_key
{
	struct key_essence
	{
		int key_level1;			//һ��key�� ��Щ������Ҫ��� ��׼key���Լ���ID
		struct 
		{
			int first;
			int second;
		}key_level2;			//����key����Щ������Ҫ���  ��׼key���Լ��Ķ���ID��seq
		int key_level3;			//����key����Щ������Ҫ���  ��׼key���Լ��İ���ID
		int key_level4;			//�ļ�key����Щ������Ҫ���������Ϊר�и���׼���ģ������ս��
		int key_level5;			//�弶key����Щ������Ҫ�������׼key���Լ��Ĺ���ID
	}; 
	
	key_essence essence;
	key_essence target;
	int control_id;				//����Ŀ��ƺţ�����������ʱ��ĳЩ������������id������ĳЩ����0��ʾ�ǿ���
	int special_mask;			//0x01 - GM  IK_SPECIAL_MASKG
};

enum IK_SPECIAL_MASK
{
	IKSM_GM 		= 0x1,
	IKSM_REENTER 	= 0x2,
};

template <typename WRAPPER >
WRAPPER & operator << (WRAPPER & wrapper ,const instance_key & rhs)
{
	return wrapper.push_back(&rhs, sizeof(rhs));
}

template <typename WRAPPER >
WRAPPER & operator >>  (WRAPPER & wrapper ,instance_key & rhs)
{
	return wrapper.pop_back(&rhs, sizeof(rhs));
}

struct instance_hash_key
{
	int key1;
	int key2;
	instance_hash_key(int k1 = 0, int k2 = 0) : key1(k1), key2(k2){}
	bool operator ==(const instance_hash_key & rhs) const
	{
		return key1 == rhs.key1 && key2 == rhs.key2;
	}
};

struct instance_hash_function
{
	inline int operator()(const instance_hash_key & key) const
	{
		return key.key1 ^ key.key2;
	}
};

struct player_var_data
{
	enum 
	{
		BASIC_VAR_DATA_SIZE = 16,
		VERSION1	= 1,
		VERSION2	= 2,
		VERSION3	= 3,
		VERSION4	= 4,
		VERSION5	= 5,
		VERSION6	= 6,
		VERSION7	= 7,
		CUR_VERSION
	};
	int version;
	int pk_count;		//ɱ����Ŀ 
	int pvp_cooldown;	//pvp��ȴʱ��
	bool pvp_flag;		//�Ƿ񼤻���pvp
	char dead_flag;		//�Ƿ�����״̬(3��)
	bool is_drop;		//�Ƿ��ڿ���
	bool resurrect_state;	//����״̬(�Ƿ��ԭ�ظ���)
	float resurrect_exp_reduce; //����״̬ʱ��ʧ��exp����ԭ�ظ�������Ч)
	instance_hash_key ins_key;  //�ϴ��˳�ʱ���ڵĸ���hash key
	int  trashbox_size;	    //�ֿ�Ĵ�С
	int last_instance_timestamp;//���һ�ν��븱����ʱ���
	int last_instance_tag;	    //���һ�ν��븱����ID
	A3DVECTOR last_instance_pos;//���һ�ν��븱��������
	int dir;		    //�������ʱ�ķ���
	float 	resurrect_hp_factor;		//����״̬���Ѫ��������ԭ�ظ�������Ч)
	float 	resurrect_mp_factor;		//����״̬ʱ������������ԭ�ظ�������Ч)
	int instance_reenter_timeout;		// ��ʾ���������½����ʱ���  0��ʾ���ܽ���
	int 		last_world_type;		// ���һ���˳�ʱ����������
	A3DVECTOR	last_logout_pos;		// ���һ���˳�ʱ������λ�� (�Ǵ���λ��)

	template <typename PLAYER_IMP>
	void MakeData(gplayer * pPlayer, PLAYER_IMP * pImp)
	{
		version = CUR_VERSION;
		pk_count = pImp->GetPKCount();
		pvp_cooldown = pImp->GetPVPCoolDown();
		pvp_flag = pImp->GetPVPFlag();
		dead_flag =  pImp->GetDeadFlag();
		is_drop =  pImp->_layer_ctrl.IsOnAir();
		resurrect_state = pImp->GetResurrectState(resurrect_exp_reduce,resurrect_hp_factor,resurrect_mp_factor);
	//	int tag = world_manager::GetWorldTag();
		ins_key = pImp->GetLogoutInstanceKey();
		trashbox_size = pImp->_trashbox.GetTrashBoxSize(0);
		pImp->GetLastInstancePos( last_instance_tag, last_instance_pos,last_instance_timestamp);
		dir = pPlayer->dir;
		
		instance_reenter_timeout = pImp->GetInstanceReenterTimeout();
		last_world_type = pImp->GetWorldType();
		last_logout_pos = pPlayer->pos;
	}

	template <typename PLAYER_IMP>
	static void SetData(gplayer * pPlayer, PLAYER_IMP * pImp, const void * buf, unsigned int size)
	{
		player_var_data * pVar = (player_var_data*)buf;
		switch(pVar->version)
		{
			case VERSION1:
				{
					pImp->SetPVPState(pVar->pk_count,pVar->pvp_cooldown,pVar->pvp_flag);
					pImp->SetDeadFlag(pVar->dead_flag);
					if(pVar->is_drop) pImp->_layer_ctrl.Landing();
				}
				break;
			case VERSION2:
				{
					pImp->SetPVPState(pVar->pk_count,pVar->pvp_cooldown,pVar->pvp_flag);
					pImp->SetDeadFlag(pVar->dead_flag);
					if(pVar->is_drop) pImp->_layer_ctrl.Landing();
					pImp->SetResurrectState(pVar->resurrect_state,pVar->resurrect_exp_reduce,DEFAULT_RESURRECT_HP_FACTOR,DEFAULT_RESURRECT_MP_FACTOR);
				}
				break;
			case VERSION3:
			case VERSION4:
				{
					pImp->SetPVPState(pVar->pk_count,pVar->pvp_cooldown,pVar->pvp_flag);
					pImp->SetDeadFlag(pVar->dead_flag);
					if(pVar->is_drop) pImp->_layer_ctrl.Landing();
					pImp->SetResurrectState(pVar->resurrect_state,pVar->resurrect_exp_reduce,DEFAULT_RESURRECT_HP_FACTOR,DEFAULT_RESURRECT_MP_FACTOR);
				}
				break;
			case VERSION5:
				{
					pImp->SetPVPState(pVar->pk_count,pVar->pvp_cooldown,pVar->pvp_flag);
					pImp->SetDeadFlag(pVar->dead_flag);
					if(pVar->is_drop) pImp->_layer_ctrl.Landing();
					pImp->SetResurrectState(pVar->resurrect_state,pVar->resurrect_exp_reduce,DEFAULT_RESURRECT_HP_FACTOR,DEFAULT_RESURRECT_MP_FACTOR);
				}
				break;
			case VERSION6:
				{
					pImp->SetPVPState(pVar->pk_count,pVar->pvp_cooldown,pVar->pvp_flag);
					pImp->SetDeadFlag(pVar->dead_flag);
					if(pVar->is_drop) pImp->_layer_ctrl.Landing();
					pImp->SetResurrectState(pVar->resurrect_state,pVar->resurrect_exp_reduce,DEFAULT_RESURRECT_HP_FACTOR,DEFAULT_RESURRECT_MP_FACTOR);
					pPlayer->dir = pVar->dir;
				}
				break;
			case VERSION7:
				{
					pImp->SetPVPState(pVar->pk_count,pVar->pvp_cooldown,pVar->pvp_flag);
					pImp->SetDeadFlag(pVar->dead_flag);
					if(pVar->is_drop) pImp->_layer_ctrl.Landing();
					pImp->SetResurrectState(pVar->resurrect_state,pVar->resurrect_exp_reduce,pVar->resurrect_hp_factor,pVar->resurrect_mp_factor);
					pPlayer->dir = pVar->dir;
				}
				break;
			case CUR_VERSION:
				{
					pImp->SetPVPState(pVar->pk_count,pVar->pvp_cooldown,pVar->pvp_flag);
					pImp->SetDeadFlag(pVar->dead_flag);
					if(pVar->is_drop) pImp->_layer_ctrl.Landing();
					pImp->SetResurrectState(pVar->resurrect_state,pVar->resurrect_exp_reduce,pVar->resurrect_hp_factor,pVar->resurrect_mp_factor);
					pPlayer->dir = pVar->dir;
					pImp->InitInstanceReenter(pVar->last_instance_tag,pVar->last_world_type,pVar->ins_key,pVar->instance_reenter_timeout,pVar->last_logout_pos);
				}
				break;
		}
	}

	static int GetTrashBoxSize(const void * buf, unsigned int size)
	{
		if(size < sizeof(int))
		{
			return TRASHBOX_BASE_SIZE;
		}
		player_var_data * pVar = (player_var_data*)buf;
		switch(pVar->version)
		{
			case VERSION5:
			case VERSION6:
			case VERSION7:
			case CUR_VERSION:
				return pVar->trashbox_size;
			default:
			return TRASHBOX_BASE_SIZE;
		}
	}

	template <typename INSTANCE_KEY>
	static void GetInstanceKey(const void * buf, unsigned int size,INSTANCE_KEY & key)
	{
		memset(&key,0,sizeof(key));
		if(size < sizeof(int)) 
		{
			key.key1 = 0;
			key.key2 = time(NULL);
			return;
		}
		player_var_data * pVar = (player_var_data*)buf;
		switch(pVar->version)
		{
			case VERSION3:
			case VERSION4:
			case VERSION5:
			case VERSION6:
			case VERSION7:
			case CUR_VERSION:
				{
					key = pVar->ins_key;
				}
				return ;
		}
		key.key1 = 0;
		key.key2 = time(NULL);
		return;
	}

	template <int >
	static void GetLastInstance(const void * buf, unsigned int size,int &last_ins_tag, A3DVECTOR & pos,int & create_ts)
	{
		if(size < sizeof(int)) 
		{
			last_ins_tag = -1;
			create_ts = -1;
			return;
		}
		player_var_data * pVar = (player_var_data*)buf;
		switch(pVar->version)
		{
			case VERSION5:
			case VERSION6:
			case VERSION7:
			case CUR_VERSION:
				{
					last_ins_tag = pVar->last_instance_tag;
					pos = pVar->last_instance_pos;
					create_ts = pVar->last_instance_timestamp;
				}
				return ;
		}
		last_ins_tag = -1;
		create_ts = -1;
		return;
	}

	enum 
	{
		ALIVE 	= 0,
		KILLED_BY_PLAYER = 1,
		KILLED_BY_NPC = 2,
	};

};
enum
{
	PLAYER_MONSTER_TYPE = 256,
};

#endif

