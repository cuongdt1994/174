#ifndef __ONLINEGAME_GS_OBJECT_H__
#define __ONLINEGAME_GS_OBJECT_H__

#include <spinlock.h>
#include <common/message.h>
#include <common/types.h>
#include <common/packetwrapper.h>
#include <ASSERT.h>

#include "config.h"


class world;
class gobject_imp;
struct slice;
struct gobject
{
	enum 
	{
		MSG_MASK_PLAYER_MOVE  	= 0x0001,
//		MSG_MASK_NPC_MOVE	= 0x0002,
//		MSG_MASK_MATTER_DROP	= 0x0004,
//		MSG_MASK_PLAYER_DEATH	= 0x0008,
//		MSG_MASK_ACTIVE		= 0x0010,
		MSG_MASK_ATTACK		= 0x0020,
		MSG_MASK_ENCHANT	= 0x0040,
		MSG_MASK_CRY_FOR_HELP	= 0x0080,
	};

	int	spinlock;	//������
#ifdef _DEBUG
	int 	cur_msg;	//���һ��ִ�е���Ϣ��
#endif
	unsigned int crc;	//���ܵ�crcУ��ֵ
	bool 	b_chk_state;	//��ʾ�ö����Ƿ�ʹ���еļ����
	bool 	b_zombie;	//�Ƿ�ʬ״̬
	bool	b_disconnect;	//�Ƿ��Ѿ����ߣ�ֻ��playerʹ��
	unsigned char dir;	//���������ķ���
	int	msg_mask;	//������Ϣ�ı�־
	gobject * pPrev;	//ǰһ��ָ��
	gobject * pNext;	//��һ��ָ��
	slice 	* pPiece;	//�����Ŀ�ָ��
	A3DVECTOR pos;		//����������λ��
	XID 	ID;		//id,�����û���ʹ��uid,����������Ʒ��NPC��ʹ�� 2λ��־ | 14λ world_index | 16λ����
	gobject_imp * imp;	//ʵ�ʵľ���ʵ��
	world * plane;
	float	body_size;	//����Ĵ�С�߶� 
	int 	collision_id;//��ײ��־λ
public:
	inline void Lock() {mutex_spinlock(&spinlock);}
	inline void Unlock() {mutex_spinunlock(&spinlock);}
	inline int  TryLock() { return mutex_spinset(&spinlock);}
	inline bool IsEmpty() { return !b_chk_state;}
	inline bool IsActived() { return b_chk_state;}
	inline bool IsZombie() { return b_zombie;}
	inline void SetActive() { b_chk_state = true;}
	inline void ClrActive() { b_chk_state = false;}
	inline void Clear()
	{
		crc = 0;
		b_chk_state = false; 
		b_zombie = false;
		dir = 0;
		msg_mask = 0;
		ID.id = -1;
		ID.type = -1;
		imp = NULL;
		body_size = 0.f;
		b_disconnect = false;
		collision_id = 0;
	}
};

struct object_base_info
{
	int race;			//ְҵ���Ա� ����npc��Ч
	int faction;			//��ϵ
	int level;			//����
	int hp;				//hp
	int max_hp;			//mp
	int mp;				//���� 
};


struct gactive_object : public gobject
{
	object_base_info base_info;
	unsigned int object_state;	//�����ʾ�����״̬
	unsigned int object_state2;
	unsigned char shape_form;	//��������
	unsigned char emote_form;	//��������
	unsigned char effect_count;
	unsigned char npc_reborn_flag;	//��NPC���ԣ���������ʱ��Ҫ��������
	unsigned int extend_state;
	unsigned int extend_state2;
	unsigned int extend_state3;
	unsigned int extend_state4;
	unsigned int extend_state5;
	unsigned int extend_state6;
	unsigned int extend_state7;
	unsigned int extend_state8;
	unsigned int extend_state9;
	int	invisible_degree;		//��ǰ�����npc����������δʹ����������ʱ��ֵΪ0
	int anti_invisible_degree;	//��ǰ�����npc�ķ�������
	int multiobj_effect_count;
	struct
	{
		int target;
		char type;
	}multiobj_effect_list[MAX_MULTIOBJ_EFFECT_COUNT];

	template<typename WRAPPER>
	WRAPPER & Export(WRAPPER & wrapper)
	{
		wrapper << object_state << object_state2 << shape_form << effect_count << extend_state << extend_state2 << extend_state3 << extend_state4 << extend_state5 << extend_state6 << extend_state7 << extend_state8 << extend_state9 << invisible_degree << anti_invisible_degree << multiobj_effect_count;
		return wrapper.push_back(multiobj_effect_list, sizeof(multiobj_effect_list));
	}

	template<typename WRAPPER>
	WRAPPER & Import(WRAPPER & wrapper)
	{
		wrapper >> object_state >> object_state2 >> shape_form >> effect_count >> extend_state >> extend_state2 >> extend_state3 >> extend_state4 >> extend_state5 >> extend_state6 >> extend_state7 >> extend_state8 >> extend_state9 >> invisible_degree >> anti_invisible_degree >> multiobj_effect_count;
		return wrapper.pop_back(multiobj_effect_list, sizeof(multiobj_effect_list));
	}

	void Clear()
	{
		extend_state = 0;
		extend_state2 = 0;
		extend_state3 = 0;
		extend_state4 = 0;
		extend_state5 = 0;
		extend_state6 = 0;
		extend_state7 = 0;
		extend_state8 = 0;
		extend_state9 = 0;
		object_state = 0;
		object_state2 = 0;
		shape_form = 0;
		emote_form = 0;//�ƶ���ʱ���ô��̣���Ϊ��ʱ��������Ӧ�ı���
		effect_count = 0;
		memset(&base_info,0,sizeof(base_info));
		invisible_degree = 0;
		anti_invisible_degree = 0;
		multiobj_effect_count = 0;
		memset(multiobj_effect_list, 0, sizeof(multiobj_effect_list));
		gobject::Clear();
	}
	enum		
	{	
		//�����ö�����Ƿŵ�object_state�ϵ�
	//���ǿ��Խ�playerר�����ݷŵ���NPCר������ƽ�е�λ����
		STATE_SHAPE		= 0x00000001,   //�Ƿ����״̬
		STATE_EMOTE		= 0x00000002,   //�Ƿ�����������
		STATE_INVADER 		= 0x00000004,   //�Ƿ����
		STATE_PARIAH 		= 0x00000008,   //�Ƿ����
		STATE_FLY		= 0x00000010,   //�Ƿ����
		STATE_SITDOWN		= 0x00000020,   //�Ƿ�����
		STATE_EXTEND_PROPERTY	= 0x00000040,   //�Ƿ�����չ״̬
		STATE_ZOMBIE		= 0x00000080,	//�Ƿ�ʬ��

		STATE_TEAM		= 0x00000100,   //�Ƿ��Ա
		STATE_TEAMLEADER	= 0x00000200,   //�Ƿ�ӳ�
		STATE_ADV_MODE		= 0x00000400,   //�Ƿ��й������
		STATE_MAFIA		= 0x00000800,   //�Ƿ���ɳ�Ա
		STATE_MARKET		= 0x00001000,	//�Ƿ����ڰ�̯
		STATE_FASHION_MODE	= 0x00002000,	//�Ƿ�ʱװģʽ
		STATE_GAMEMASTER	= 0x00004000,	//���������GM����״̬
		STATE_PVPMODE		= 0x00008000,	//�Ƿ�����PVP����

		STATE_EFFECT		= 0x00010000,	//�Ƿ�������Ч��
		STATE_IN_PVP_COMBAT	= 0x00020000,	//�Ƿ�����PVP��
		STATE_IN_DUEL_MODE	= 0x00040000,	//�Ƿ����ھ�����
		STATE_MOUNT		= 0x00080000,	//���������
		STATE_IN_BIND		= 0x00100000,	//�ͱ��˰���һ��
		STATE_BATTLE_OFFENSE	= 0x00200000,	//ս��������
		STATE_BATTLE_DEFENCE	= 0x00400000,	//ս�����ط�
		STATE_SPOUSE            = 0x00800000,   //������ż

		STATE_ELF_REFINE_ACTIVATE = 0x01000000, //��ǰװ����С���龫��Ч������ lgc
		STATE_SHIELD_USER		  = 0x02000000,	//��������û�
		STATE_INVISIBLE			  = 0x04000000,	//��������״̬
		STATE_EQUIPDISABLED		  = 0x08000000,	//��װ���Ѿ�ʧЧ
		STATE_FORBIDBESELECTED	  = 0x10000000,	//��ֹ��������ѡ��
		STATE_PLAYERFORCE	  	  = 0x20000000,	//�Ѽ�������
		STATE_MULTIOBJ_EFFECT	  = 0x40000000,	//�����������������Ч��
		STATE_COUNTRY			  = 0x80000000,	//�Ѽ������

		STATE_STATE_CORPSE	= 0x00000008,	//NPC�Ƿ����ʬ������������ZOMBIE��һ������ʬ�������Ҳ����ZOMBIE
		STATE_NPC_ADDON1	= 0x00000100,	//NPC��������λ1 
		STATE_NPC_ADDON2	= 0x00000200,	//NPC��������λ2 
		STATE_NPC_ADDON3	= 0x00000400,	//NPC��������λ3 
		STATE_NPC_ADDON4	= 0x00000800,	//NPC��������λ4 
		STATE_NPC_ALLADDON	= 0x00000F00,	//NPC��������λ 
		STATE_NPC_PET		= 0x00001000,	//NPC��һ��PET���������PET������ID
		STATE_NPC_NAME		= 0x00002000,	//NPC�ж��ص����֣��������һ�ֽ�char����������
		STATE_NPC_FIXDIR	= 0x00004000,	//NPC���򲻱�
		STATE_NPC_MAFIA		= 0x00008000,	//NPC����ID
		STATE_NPC_FLY		= 0x00010000,	//NPC����
		STATE_NPC_SWIM		= 0x00020000,	//NPC��Ӿ

		//�����ö�����Ƿŵ�object_state2�ϵ�
		STATE_KING				= 0x00000001,	//�ǹ���
		STATE_TITLE				= 0x00000002,   //�ƺ�
		STATE_REINCARNATION		= 0x00000004,   //ת��
		STATE_REALMLEVEL		= 0x00000008,   //����ȼ�
		STATE_IN_COMBAT			= 0x00000010,	//ս��״̬
		STATE_MAFIA_PVP_MASK    = 0x00000020,   //����pvp ״̬
        STATE_PLAYER_GENDER     = 0x00000040,   //��ɫ�Ա� 0-�� 1-Ů
		STATE_MNFACTION_MASK	= 0x00000080,	//���Ψһ����id
		STATE_CASH_VIP_MASK     = 0x00000100,    //VIP
		STATE_SEALED            = 0x00000200, 
        STATE_STUN            = 0x00000400,
        STATE_CARRIER            = 0x00000800, // CARRIER INFO //send 2 INT (size = 8)
        STATE_SLEEP            = 0x00001000,
        STATE_ROOT            = 0x00002000,
        STATE_DEBUFF_SK1            = 0x00004000,
        STATE_DEBUFF_SK2            = 0x00008000,
        STATE_DEBUFF_SK3            = 0x00010000,
        STATE_DEBUFF_SK4            = 0x00020000,
        STATE_NEWSTUN            = 0x00040000,
        STATE_FREEMOVE            = 0x00080000,
        STATE_UNKNOW11            = 0x00100000,
        STATE_UNKNOW12            = 0x00400000,
        STATE_UNKNOW13            = 0x00800000, //174
        //STATE_UNKNOW14            = 0x01000000, //176+, provavelmente existe um object state3 na 176+
        //como não tenho certteza coloquei aqui

		// Custom
		/*STATE_SEALED			= 0x01000000,
		STATE_STUN 				= 0x02000000,
		STATE_SLEEP				= 0x04000000,
		STATE_ROOT				= 0x08000000,
		STATE_DEBUFF_SK1 		= 0x10000000,
		STATE_DEBUFF_SK2 		= 0x20000000,
		STATE_DEBUFF_SK3 		= 0x40000000,
		STATE_DEBUFF_SK4 		= 0x40000001,
		STATE_NEWSTUN			= 0x40000002,
		STATE_FREEMOVE 			= 0x40000003,*/	
    };

	inline bool IsFemale() const
	{
		return base_info.race < 0;
	}

	inline bool IsDuelMode() const
	{
		return object_state & STATE_IN_DUEL_MODE;
	}

	inline bool IsMountMode() const
	{
		return object_state & STATE_MOUNT;
	}

	inline bool IsCarrierMode() const
	{
		return object_state2 & STATE_CARRIER;
	}

	inline bool IsFlyMode() const
	{
		return object_state & STATE_FLY;	
	}

	inline void SetBattleOffense() 
	{
		object_state &= ~STATE_BATTLE_DEFENCE;
		object_state |= STATE_BATTLE_OFFENSE;
	}

	inline void SetBattleDefence()
	{
		object_state &= ~STATE_BATTLE_OFFENSE;
		object_state |= STATE_BATTLE_DEFENCE;
	}

	inline void ClrBattleMode()
	{
		object_state &= ~(STATE_BATTLE_OFFENSE | STATE_BATTLE_DEFENCE);
	}

	inline bool IsBattleOffense() 
	{
		return object_state & STATE_BATTLE_OFFENSE;
	}

	inline bool IsBattleDefence()
	{
		return object_state & STATE_BATTLE_DEFENCE;
	}

	inline bool IsInvisible()
	{
		return object_state & STATE_INVISIBLE;	
	}

	inline bool IsKing()
	{
		return object_state2 & STATE_KING;
	}
};

struct gnpc: public gactive_object
{
	unsigned int spawn_index;
	int native_state;
	int tid;		//ʵ�ʵ�tid
	int vis_tid;		//�ɼ���tid
	int monster_faction;	//�����С��
	int cruise_timer;	//�й�ʱ�ļ�����
	int idle_timer;
	int idle_timer_count;	//������idle״̬��ʱ
	int npc_idle_heartbeat;	//�����idleʱʹ�ú��ַ�ʽ���Լ��ټ���
	int master_id;		//����ID��ֻ�д���PET״̬��NPC�Ż��д�״̬
	int mafia_id;		//��������id;
	short name_size;	//NPC���Զ�һ���ֳ��ȣ�ֻ�����������ֱ�־��npc��״̬����Ч 
	char npc_name[18];	//NPC���Զ������֣�ֻ�����������ֱ�־��npc��״̬����Ч
	enum
	{
		TYPE_NORMAL ,
		TYPE_NATIVE ,
		TYPE_EXTERN_NATIVE,
		TYPE_FREE,
	};
	inline gnpc* get_next() { return (gnpc*)pNext;}
	inline gnpc* get_prev() { return (gnpc*)pPrev;}
	void Clear()
	{
		tid = 0;
		vis_tid = 0;
		master_id = 0;
		mafia_id = 0;
		name_size = 0;
		native_state = TYPE_NORMAL;
		npc_idle_heartbeat = 0;
		gactive_object::Clear();
	}
	bool IsNative()
	{
		return native_state == TYPE_NATIVE;
	}

	template<typename WRAPPER>
	WRAPPER & Import(WRAPPER & wrapper)
	{
		gactive_object::Import(wrapper); 
		wrapper >> ID >> pos >> msg_mask >> tid >> vis_tid >> 
		base_info.race >> base_info.faction >>
		base_info.level >> base_info.hp >>
		base_info.max_hp >> monster_faction >> body_size;
		wrapper >> master_id >> mafia_id >> name_size;
		return wrapper.pop_back(npc_name,sizeof(npc_name));
	}
	
	template<typename WRAPPER>
	WRAPPER & Export(WRAPPER & wrapper)
	{
		gactive_object::Export(wrapper); 
		wrapper << ID << pos << msg_mask << tid << vis_tid << 
		base_info.race << base_info.faction <<
		base_info.level << base_info.hp <<
		base_info.max_hp << monster_faction << body_size;
		wrapper << master_id << mafia_id << name_size;
		return wrapper.push_back(npc_name,sizeof(npc_name));
	}
};

struct gmatter : public gobject
{
	unsigned char dir1;
	unsigned char rad;
	unsigned char matter_state;
	unsigned char matter_value;
	enum
	{
		STATE_MASK_NORMAL_MINE = 0x00,			//��ͨ����
		STATE_MASK_DYN_OBJECT = 0x01,           //��ʾ����ƷΪ��̬��Ʒ����IDӦ�Ӷ�̬ID��Ѱ��
		STATE_MASK_SOUL_MINE = 0x02,			//Ԫ��
	};
	int matter_type;
	int spawn_index;		//ֻ�Կ������
	inline gmatter* get_next() { return (gmatter*)pNext;}
	inline gmatter* get_prev() { return (gmatter*)pPrev;}
	inline void SetDirUp(unsigned char d, unsigned char d1, unsigned char r)
	{
		dir = d;
		dir1 = d1;
		rad = r;
	}
	
	inline void SetMatterValue(unsigned char v)
	{
		matter_value = v;
	}

	inline void Clear()
	{
		gobject::Clear();
		dir1 = rad = matter_state = matter_value = 0;
	}

};
#endif

