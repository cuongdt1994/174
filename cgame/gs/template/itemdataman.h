#ifndef _ITEMDATAMAN_H_
#define _ITEMDATAMAN_H_

#include <vector.h>
#include <unordered_map>

#ifdef LINUX
#include <arandomgen.h>
#include <amemory.h>
#include "exptypes.h"
#include "../itemdata.h"
#include "../item/item_addon.h"
#else	// Windows
#include "exptypes.h"
#include <Windows.h>

#pragma pack(4)

namespace abase
{
	inline int Rand(int lower, int upper)	
	{ 
		if(upper==lower)
			return lower;
		else	
			return rand()%(upper-lower)+lower; 
	}
	
	inline float Rand(float lower, float upper) 
	{ return lower+(upper-lower)*rand()/(float)RAND_MAX; }
	
	inline int RandNormal(int lower, int upper) { return Rand(lower, upper); }
	inline float RandUniform() { return Rand(0.f, 1.f); }
	
	inline int RandSelect(const void * option, int stride, int num) 
	{ 
		const char * tmp = (const char *)option;
		float op = RandUniform();
		for(int i =0;i < num; i ++)
		{
			float prob = *(float*)tmp;
			if(op < prob ) 
				return i;
			op -= prob;
			tmp += stride;
		}
		assert(false);
		return 0;
	}
	inline void * fastalloc(unsigned int size) { return malloc(size); }
	inline void   fastfree(void * buf, unsigned int size) {free(buf); }
};

struct item_data
{
	unsigned int type;   		//��Ʒ��ģ��ID
	unsigned int count;  				//��Ʒ������
	unsigned int pile_limit;			//��Ʒ�Ķѵ�����
	int equip_mask;  			//��Ʒ�Ŀ�װ����־��0x8000��ʾ����Ƕ��
	int proc_type;				//��Ʒ�Ĵ�����ʽ
	int classid;  				//��Ʒ��Ӧ�����ID
	struct
	{ 
		int guid1;
		int guid2;
	} guid;   				//��Ʒ��GUID
	int price;   				//��Ʒ�ļ۸�
	int expire_date;			//����ʱ��
	unsigned int content_length;
	char * item_content;
};

struct addon_data
{
	int id;
	int arg[3];
};

struct qihun_addon_data
{
	int addons_count;
	struct
	{
		int id;
		unsigned int arg;
	} addons[5];
};

struct prerequisition
{       
	short level;
	short race;
	short strength;
	short vitality;
	short agility;
	short energy;
	int durability;
	int max_durability;
};      

#endif

int hsv2rgb( float h, float s, float v);
class elementdataman;
namespace element_data
{
	
	enum LOWER{LOWER_TREND};
	enum UPPER{UPPER_TREND};
	enum MIDDLE{MIDDLE_TREND};
	enum ANY{ANY_TREND};
	
	enum NORMAL{ NORMAL_RAND};
	enum SPECIFIC{ SPECIFIC_RAND};

	struct SpecRand
	{
		int * IndexList;
		int   IdxCap;
		int   IdxIndex;
		float * RandList;
		int RandCap;
		int  RandIndex;
		SpecRand():IndexList(0),IdxCap(0),IdxIndex(0),RandList(0),RandCap(0),RandIndex(0){}		
		int RandSelect(int num)
		{
			if(!IndexList) return 0;
			if(IdxIndex >= IdxCap) return 0;
			if(num <=0) return 0;
			int idx =IndexList[IdxIndex++];
			if(idx >= num) idx = num - 1;
			return idx;
		}

		int RandNormal(int lower, int upper)
		{
			if(!RandList) return lower;
			if(RandIndex >= RandCap) return lower;
			float r = RandList[RandIndex ++];
			if(r <0) r = 0.f;
			if(r >=1.0f) r = 0.9999999f;
			return (int)((upper - lower + 1)* r + lower);
		}
		
		float Rand(float lower, float upper)
		{
			if(!RandList) return lower;
			if(RandIndex >= RandCap) return lower;
			float r = RandList[RandIndex ++];
			if(r <0) r = 0.f;
			if(r >=1.0f) r = 0.9999999f;
			return (float)((upper - lower)* r + lower);
		}
	};

	struct SpecCls
	{
		SpecRand * imp;
		SpecCls(SpecRand * tmp):imp(tmp){}
		int RandSelect(const void * option, int stride, int num)
		{
			if(imp) return imp->RandSelect(num);
			return 0;
		}

		int RandNormal(int lower, int upper)
		{
			if(imp) return imp->RandNormal(lower,upper);
			return lower;
		}

		float Rand(float lower, float upper)
		{
			if(imp) return imp->Rand(lower,upper);
			return lower;
		}

	};
	
	template<typename cls,typename trend>  int RandNormal(int lower, int upper,cls c,trend)
	{
		return c.RandNormal(lower,upper);
	}

	template<typename cls,typename trend> int RandSelect(const void * option, int stride, int num,cls c,trend)
	{
		return c.RandSelect(option,stride,num);
	}

	template<typename cls,typename trend>  float Rand(float lower, float upper,cls c,trend)
	{
		return c.Rand(lower,upper);
	}
	
	template<> 
		inline	int RandSelect<NORMAL,LOWER>(const void * option, int stride, int num,NORMAL,LOWER)
	{
		return abase::RandSelect(option, stride, num);
	}
	
	template<> 
		inline	int RandSelect<NORMAL,MIDDLE>(const void * option, int stride, int num,NORMAL,MIDDLE)
	{
		return abase::RandSelect(option, stride, num);
	}
	
	template<> 
		inline	int RandSelect<SPECIFIC,LOWER>(const void * option, int stride,int num,SPECIFIC,LOWER)
	{
		return 0;
	}
	template<> 
		inline	int RandSelect<SPECIFIC,MIDDLE>(const void * option, int stride, int num,SPECIFIC,MIDDLE)
	{
		return num / 2;
	}

	
	template<>
		inline int RandNormal<NORMAL,LOWER>(int lower,int upper,NORMAL,LOWER)
	{
		return abase::RandNormal(lower,upper);
	}
	
	template<>
		inline int RandNormal<NORMAL,UPPER>(int lower,int upper,NORMAL,UPPER)
	{
		return abase::RandNormal(lower,upper);
	}
	
	template<>
		inline int RandNormal<NORMAL,MIDDLE>(int lower,int upper,NORMAL,MIDDLE)
	{
		return abase::RandNormal(lower,upper);
	}
	template<>
		inline int RandNormal<NORMAL,ANY>(int lower,int upper,NORMAL,ANY)
	{
		return abase::RandNormal(lower,upper);
	}	
	
	template<>
		inline int RandNormal<SPECIFIC,LOWER>(int lower,int upper,SPECIFIC,LOWER)
	{
		return lower;
	}
	
	template<>
		inline int RandNormal<SPECIFIC,UPPER>(int lower,int upper,SPECIFIC,UPPER)
	{
		return upper;
	}
	
	template<>
		inline int RandNormal<SPECIFIC,MIDDLE>(int lower,int upper,SPECIFIC,MIDDLE)
	{
		return (upper + lower) / 2;
	}
	template<>
		inline int RandNormal<SPECIFIC,ANY>(int lower,int upper,SPECIFIC,ANY)
	{
		return abase::RandNormal(lower,upper);
	}	

	template <>
		inline float Rand<NORMAL,LOWER>(float lower, float upper,NORMAL,LOWER)
	{
		return abase::Rand(lower,upper);
	}

	template <>
		inline float Rand<NORMAL,MIDDLE>(float lower, float upper,NORMAL,MIDDLE)
	{
		return abase::Rand(lower,upper);
	}

	template <>
		inline float Rand<NORMAL,UPPER>(float lower, float upper,NORMAL,UPPER)
	{
		return abase::Rand(lower,upper);
	}

	template <>
		inline float Rand<NORMAL,ANY>(float lower, float upper,NORMAL,ANY)
	{
		return abase::Rand(lower,upper);
	}
	
	template<>
		inline float Rand<SPECIFIC,LOWER>(float lower,float upper,SPECIFIC,LOWER)
	{
		return lower;
	}
	
	template<>
		inline float Rand<SPECIFIC,UPPER>(float lower,float upper,SPECIFIC,UPPER)
	{
		return upper;
	}
	
	template<>
		inline float Rand<SPECIFIC,MIDDLE>(float lower,float upper,SPECIFIC,MIDDLE)
	{
		return (upper + lower) / 2;
	}
	
	template<>
		inline float Rand<SPECIFIC,ANY>(float lower,float upper,SPECIFIC,ANY)
	{
		return abase::Rand(lower,upper);
	}

	enum GEN_ADDON_MODE
	{
		ADDON_LIST_SHOP,
		ADDON_LIST_DROP,
		ADDON_LIST_PRODUCE,
		ADDON_LIST_SPEC,
	};

	enum ITEM_MAKE_TAG
	{	
		IMT_NULL,
		IMT_CREATE,
		IMT_DROP,
		IMT_SHOP,
		IMT_PRODUCE,
		IMT_SIGN,	//װ����ǩ��
	};
#pragma pack(1)
	struct item_tag_t
	{
		char type;
		char size;
	};
#pragma pack()


};// name space element_data

//  move into class itemdataman?
void set_to_classid(DATA_TYPE type, item_data * data, int major_type);
void get_item_guid(int id,int & g1, int & g2);
int addon_generate_arg(DATA_TYPE type, addon_data & data, int arg_num/*��ʼ�Ĳ�������*/); //�������յĲ������������ᳬ����ʼ������
int addon_update_ess_data(const addon_data & data, void * essence,unsigned int ess_size, prerequisition * require);
void update_require_data(prerequisition *require);

class itemdataman
{
protected:
	
#define ELEMENTDATAMAN_EQUIP_MASK_WEAPON       0x0001
#define ELEMENTDATAMAN_EQUIP_MASK_HEAD         0x0002
#define ELEMENTDATAMAN_EQUIP_MASK_NECK         0x0004
#define ELEMENTDATAMAN_EQUIP_MASK_SHOULDER     0x0008
#define ELEMENTDATAMAN_EQUIP_MASK_BODY         0x0010
#define ELEMENTDATAMAN_EQUIP_MASK_WAIST        0x0020
#define ELEMENTDATAMAN_EQUIP_MASK_LEG          0x0040
#define ELEMENTDATAMAN_EQUIP_MASK_FOOT         0x0080
#define ELEMENTDATAMAN_EQUIP_MASK_WRIST        0x0100
#define ELEMENTDATAMAN_EQUIP_MASK_FINGER1      0x0200
#define ELEMENTDATAMAN_EQUIP_MASK_FINGER2	0x0400
#define ELEMENTDATAMAN_EQUIP_MASK_PROJECTILE	0x0800
#define ELEMENTDATAMAN_EQUIP_MASK_FLYSWORD	0x1000
#define ELEMENTDATAMAN_EQUIP_MASK_DAMAGERUNE	0x20000
#define ELEMENTDATAMAN_EQUIP_MASK_BIBLE		0x40000
#define ELEMENTDATAMAN_EQUIP_MASK_SPEAKER       0x80000
#define ELEMENTDATAMAN_EQUIP_MASK_AUTO_HP       0x100000
#define ELEMENTDATAMAN_EQUIP_MASK_AUTO_MP       0x200000
#define	ELEMENTDATAMAN_EQUIP_MASK_ELF			0x800000	//lgc
#define	ELEMENTDATAMAN_EQUIP_MASK_STALLCARD		0x1000000
#define	ELEMENTDATAMAN_EQUIP_MASK_FORCE_TICKET	0x4000000
#define	ELEMENTDATAMAN_EQUIP_MASK_DYNSKILL_ALL	0x18000000
#define ELEMENTDATAMAN_EQUIP_MASK_ASTROLABE	   0x0000004000000000LL
//
#define ELEMENTDATAMAN_EQUIP_MASK_HAS_ADDON    0x40000000
#define ELEMENTDATAMAN_EQUIP_MASK_EXTEND64     0x80000000
#define ELEMENTDATAMAN_EQUIP_MASK_HIGH         0xC0000000

#define ELEMENTDATAMAN_EQUIP_MASK_NEW_FASHION_ARM	0x10000
#define ELEMENTDATAMAN_EQUIP_MASK_NEW_FASHION_LEG	0x4000
#define ELEMENTDATAMAN_EQUIP_MASK_NEW_FASHION_SHOULDER	0x2000
#define ELEMENTDATAMAN_EQUIP_MASK_NEW_FASHION_HEAD	0x2000000
#define ELEMENTDATAMAN_EQUIP_MASK_NEW_FASHION_WEAPON	0x20000000
#define ELEMENTDATAMAN_EQUIP_MASK_NEW_FASHION_FOOT	0x8000


	static unsigned int equip_mask_64_to_32(UINT64 equip_mask)
	{
		unsigned int mask = 0;
		if(equip_mask >> 32)
		{
			mask = ((unsigned int)(equip_mask >> 32)) & (~ELEMENTDATAMAN_EQUIP_MASK_HIGH) | ELEMENTDATAMAN_EQUIP_MASK_EXTEND64;
		}
		else
		{
			mask = ((unsigned int)(equip_mask & 0xFFFFFFFF)) & (~ELEMENTDATAMAN_EQUIP_MASK_HIGH);
		}
		return mask;
	}

	static UINT64 equip_mask_32_to_64(unsigned int mask)
	{
		UINT64 equip_mask = 0;
		if(mask & ELEMENTDATAMAN_EQUIP_MASK_EXTEND64)
		{
			equip_mask = mask & (~ELEMENTDATAMAN_EQUIP_MASK_HIGH);
			equip_mask <<= 32;
		}
		else
		{
			equip_mask = mask & (~ELEMENTDATAMAN_EQUIP_MASK_HIGH);
		}
		return equip_mask;
	}
	
	enum
	{
			ELEMENTDATAMAN_EQUIP_INDEX_WEAPON      = 0,
			ELEMENTDATAMAN_EQUIP_INDEX_HEAD        = 1,
			ELEMENTDATAMAN_EQUIP_INDEX_NECK        = 2,
			ELEMENTDATAMAN_EQUIP_INDEX_SHOULDER    = 3,
			ELEMENTDATAMAN_EQUIP_INDEX_BODY        = 4,
			ELEMENTDATAMAN_EQUIP_INDEX_WAIST       = 5,
			ELEMENTDATAMAN_EQUIP_INDEX_LEG         = 6,
			ELEMENTDATAMAN_EQUIP_INDEX_FOOT        = 7,
			ELEMENTDATAMAN_EQUIP_INDEX_WRIST       = 8,
			ELEMENTDATAMAN_EQUIP_INDEX_FINGER1     = 9,
			ELEMENTDATAMAN_EQUIP_INDEX_FINGER2     = 10,
			ELEMENTDATAMAN_EQUIP_INDEX_PROJECTILE  = 11,
			ELEMENTDATAMAN_EQUIP_INDEX_FLYSWORD    = 12,
	};
	
	
#define ELEMENTDATAMAN_MAX_NUM_ADDON_PARAM 3
	struct _addon		//������Ŀ
	{
		int addon_type;
		int addon_arg[ELEMENTDATAMAN_MAX_NUM_ADDON_PARAM]; 		// 0 ~ 3 ��Ŀ�� ((type & 0x6000)>>13)
	};
	
#define ELEMENTDATAMAN_MAX_NUM_HOLES 5
#define ELEMENTDATAMAN_MAX_NUM_ADDONS 32
	
#pragma  pack(1)	
	struct _item_content
	{
		prerequisition preq;
		short sizeofessence;					//װ�������С���ֽڣ�;
		//	essence							//char ����[];							//ÿ�ֲ�ͬװ���ı���ṹ��ͬ
		int num_hole;						//�׶�����Ŀ��������;
		//	int hole_type[MAX_NUM_HOLES];	//�׶���Ƕ���������[�׶�����Ŀ];       //����׶���ĿΪ0,���������
		int num_addon;						//���Ա���Ŀ����Ŀ��������;
		//	_addon ad[MAX_NUM_ADDONS];		//[���Ա���Ŀ����Ŀ];
	};
#pragma  pack()	

	struct _weapon_essence
	{
		enum
		{
			WEAPON_TYPE_MELEE = 0,
			WEAPON_TYPE_RANGE = 1,
			WEAPON_TYPE_MELEE_ASN = 2,	//�̿�ʹ�õĽ���������������Ӱ���﹥�⣬�����������ͬ
		};
		
		short weapon_type;       	//������� ��Ӧģ����Ľ���Զ�̱�־
		short weapon_delay;		//�����Ĺ����ӳ�ʱ�䣬��50msΪ��λ
		int weapon_class;       	//�������� ��Ӧģ����Ĵ��� ���絶�� ������
		int weapon_level;		//�������� ĳЩ������Ҫ��������
		int require_projectile; 	//��Ҫ��ҩ������
		int damage_low;         	//����������С��ֵ
		int damage_high;        	//������������ֵ
		int magic_damage_low;   	//ħ������
		int magic_damage_high;  	//ħ������
		int attack_speed;
		float attack_range;
		float attack_short_range;
	};
	
	struct _projectile_essence
	{
		int projectile_type;		//��ҩ����
		int enhance_damage;		//���������Ĺ�����
		int scale_enhance_damage; 	//���ձ������ӹ�����
		int weapon_level_low;
		int weapon_level_high;
	};

	struct _stone_essence
	{
	};
	
	struct _fashion_essence
	{
		int require_level;
		unsigned short color;
		unsigned short gender;
	};

	struct _fashion_new_essence
	{
			short require_level;
			short cls;
			int unk1;
			int unk2;
			int durability;
			int max_durability;
			short unk3;
			short unk4;
			int unk5;
			unsigned short color;
			unsigned short gender;
			// Name Here
			short unk6;
			char unk7;
			char effect_indice;			
	};
	
#define ELEMENTDATAMAN_MAGIC_CLASS 5
	struct _armor_essence
	{
		int defense;
		int armor;
		int mp_enhance;
		int hp_enhance;
		int resistance[ELEMENTDATAMAN_MAGIC_CLASS];
	};

	struct _new_armor_essence
	{
		int defense;
		int armor;
		int mp_enhance;
		int hp_enhance;
		int resistance[ELEMENTDATAMAN_MAGIC_CLASS];
	};

	struct _qihun_essence
	{
		int item_id;
		char crystal_lvl;	 // 1
		char crystal_enable; // 0 ~ 1
		char lock_unlock;	 // 0 ~ 1

		int addons_count;
		struct
		{
			int id;
			unsigned int arg;
		} addons[5];
	};

	struct _qiling_essence
	{
		int crystal_id;
		char crystal_level;
		char crystal_color;

		int exp;
		char lock_unlock;

		char stars[5]; // Estrelas nv 0 até 5

		// Reserve
		char res[15];

		int addons_count;

		struct addon
		{
			int id_addon;
			int arg;
		} addons[2];
	};

	struct _decoration_essence
	{
		int damage;
		int magic_damage;
		int defense;
		int armor;
		int resistance[ELEMENTDATAMAN_MAGIC_CLASS];
	};
	
	struct _medicine_essence
	{
		
	};
	
	struct _material_essence
	{
		
	};
	
	struct _damagerune_essence
	{
		short rune_type;               
		short require_weapon_level_min;
		short require_weapon_level_max;
		short enhance_damage;
	};
	
	struct _armorrune_essence
	{
		int rune_type; 
		int require_player_level;
		float enhance_percent;
		int reduce_times;
	};
	
	struct _skilltome_essence
	{
		
	};
	
	struct _cls_flysword_essence
	{
		int cur_time;
		int max_time;
		short require_level;
		char level;
		char improve_level;
		int require_class;
		unsigned int time_per_element;
		float speed_increase;		//��ͨ�����ٶ�
		float speed_increase2;		//���ٷ����ٶ�
	};

	struct _wingmanwing_essence
	{
		int require_level;
		unsigned int mp_launch;
		unsigned int mp_per_second;
		float speed_increase;
	};

	struct _taskdice_essence
	{
		int task_id;
	};
	
	struct _townscroll_essence
	{
		int duration;
	};
	
	struct _unionscroll_essence
	{
		
	};
	
	struct _revivescroll_essence
	{
		
	};
	
	struct _element_essence
	{
		
	};
	
	struct _taskmatter_essence
	{
		
	};
	
	struct _tossmatter_essence
	{
		int require_level;
		int require_strength;
		int require_agility;
		int damage_low;
		int damage_high;
		float attack_range;
		unsigned int use_time;
	};
	
	struct _facepill_essence
	{
		int duration;
		int require_class;
	};

	struct _petegg_essence
	{
		int req_level;          //������Ҽ���
		int req_class;          //�������ְҵ
		int honor_point;        //�øж�
		int pet_tid;            //�����ģ��ID
		int pet_vis_tid;        //����Ŀɼ�ID�����Ϊ0�����ʾ������ɼ�ID��
		int pet_egg_tid;        //���ﵰ��ID
		int pet_class;          //�������� ս�裬��裬���ͳ�
		short level;            //���Ｖ��
		unsigned short color;   //������ɫ�����λΪ1��ʾ��Ч��Ŀǰ���������Ч
		int exp;                //���ﵱǰ����
		int skill_point;        //ʣ�༼�ܵ�
		unsigned short name_len;//���ֳ��� 
		unsigned short skill_count;//��������		
		char name[16];          //��������
		//������漼�ܱ�
		/*
		 *	
		 struct
		 {	
			int skill
			int level;
		 }skills[]
		 */
	};
	
	struct _evo_prop
	{
		int r_attack;
		int r_defense;
		int r_hp;
		int r_atk_lvl;
		int r_def_lvl;
		int nature;
	};//�������ר�����ԣ����ϵ�����Ը�
	
	struct _petfood_essense
	{
		int honor_effect;	//����Ч��
		int food_mask;		//ʳ������
	};
	
	struct _monster_essence
	{
		
	};
	
	struct _npc_essence
	{
		
	};

#pragma pack(1)			//lgc
	struct _elf_skill_data
	{
		unsigned short id;
		short level;	
	};

	struct _elf_essence
	{
		unsigned int exp;
		short level;
	
		short total_attribute;	//�������������Ե�������������װ�����ӵļ������Գ�ʼֵ
		short strength;			//�ɼ����Ե������������ֵ��������װ�����ӵļ������Գ�ʼֵ
		short agility;
		short vitality;
		short energy;

		short total_genius;		//�츳�㣬������װ�����ӵ�
		short genius[5];			//��ľˮ����0-4
	
		short refine_level;
		int stamina; 			//����
		int status_value;		//0:��ȫ -1:�ɽ��� ����:ת��״̬
	};
	

	struct _elf_item_content
	{
		struct _elf_essence ess;	//С���鱾��
		int equip_cnt;				//��װ����װ������
		//unsigned int equipid[equip_cnt];		//װ��id
		int skill_cnt;				//��ѧ��ļ�����
		//struct _elf_skill_data skill[skill_cnt];	//���� id�͵ȼ�
	};

	struct _elf_exppill_essence
	{
		unsigned int exp;	
		int level;
	};
	
	struct _astrolabe_essence
	{
		int   exp;
		unsigned char  level;
		unsigned short  slot;			// ����λ���Ƿ������� ˳ʱ��
		unsigned short aptit[ELEMENTDATA_MAX_ASTROLABE_ADDON_COUNT]; // 1/1000
	};

#pragma pack()
	
	struct _wedding_bookcard_essence
	{
		int year;
		int month;
		int day;
	};

	struct _wedding_invitecard_essence
	{
		int start_time;
		int end_time;
		int groom;
		int bride;
		int scene;
		int invitee;
	};

	struct _force_ticket_essence
	{
		int require_force;
		int repu_total;
		int repu_inc_ratio;
	};

	struct _generalcard_essence
	{
		int type;
		int rank;
		int require_level;
		int require_leadership;
		int max_level;
		int level;
		int exp;
		int merge_times;
		int king_soul; // Espírito Imperial
	};

	public:
		itemdataman();
		~itemdataman();
		
		int load_data(const char * pathname, bool disable_bind2 = false);
		int get_name_length(unsigned short *name);	
		unsigned int get_data_id(ID_SPACE idspace, unsigned int index, DATA_TYPE& datatype);
		unsigned int get_first_data_id(ID_SPACE idspace, DATA_TYPE& datatype);
		unsigned int get_next_data_id(ID_SPACE idspace, DATA_TYPE& datatype);
		
		unsigned int get_data_num(ID_SPACE idspace);
		DATA_TYPE get_data_type(unsigned int id, ID_SPACE idspace);
		
		const void * get_data_ptr(unsigned int id, ID_SPACE idspace, DATA_TYPE& datatype);
		bool get_list_by_data_type( ID_SPACE idspace, DATA_TYPE datatype, std::vector<int> & list );
		
		int generate_item_from_monster(unsigned int id, int * list,  unsigned int size,int *money);
		void get_monster_drop_money(unsigned int id,int & low, int &high);
		int get_monster_drop_times(unsigned int id);

		bool generate_mine_from_monster(unsigned int id, int & mine_id, int & remain_time);

		const void* get_item_for_sell(unsigned int id);
		
		bool generate_addon(unsigned int addon_id, addon_data & data);
		int generate_addon(unsigned int item_id, unsigned int addon_id, addon_data & data);
		int generate_addon_from_rands(unsigned int item_id, unsigned int addon_id, addon_data & data);
		int generate_astrolabe_addonlist(const char * candidate_header, unsigned int candidate_num, addon_data* addon_list,unsigned int addon_num, unsigned int default_addon_id);

		item_data * generate_item_for_shop(unsigned int id,const void * tag, unsigned int tag_size);
		item_data * generate_item_for_drop(unsigned int id,const void * tag, unsigned int tag_size);
		item_data * generate_item_from_player(unsigned int id,const void * tag, unsigned int tag_size); //������������Ʒ

		item_data * generate_item_from_player_g17(unsigned int id,const void * tag, unsigned int tag_size, int class_pos); 

		item_data * generate_equipment(unsigned int id, float rlist[32], int ridxlist[32], int addon[ELEMENTDATAMAN_MAX_NUM_ADDONS]);

		int get_refine_meterial_id();
		int get_item_refine_addon(unsigned int id,int &material_need);
		int get_item_sell_price(unsigned int id);
		int get_item_shop_price(unsigned int id);
		int get_item_pile_limit(unsigned int id);
		int get_item_repair_fee(unsigned int id);
		int get_item_repair_g17_item(unsigned int id);
		int get_item_repair_g17_num(unsigned int id);
		int get_item_proc_type(unsigned int id);
		int get_cool_time(unsigned int id);
		int get_item_damaged_drop(unsigned int id,unsigned int &damaged_drop);	//damaged_drop����ٺ������Ʒ��id������ֵ�ǵ�������
		int get_item_class_limit(unsigned int id);
		int get_item_reputation_limit(unsigned id);
		int get_item_level(unsigned int id);	//��ȡ��Ʒ��Ʒ��
		int reset_classid(item_data * data);
		unsigned char * get_item_name(unsigned int id,int &name_len);

		uint16_t floatTohalf(float value);

#include "generate_item_temp.h"

	protected:
		int generate_addon(DATA_TYPE datatype,unsigned int addon_id, addon_data &pData);
		unsigned int generate_addon_buffer(DATA_TYPE datatype,unsigned int addon_id, char * buf);

		int generate_item_for_sell(bool disable_bind2);
		int duplicate_static_item(unsigned int id, char ** list,  unsigned int& size);
		
	protected:
		int	generate_equipment_addon(DATA_TYPE essencetype,char * header, unsigned int num_candidate, addon_data & data);
		unsigned int generate_equipment_addon_buffer(DATA_TYPE essencetype, const char * candidate_header, unsigned int candidate_num, char * addon_buffer,unsigned int & addon_num);
		unsigned int generate_equipment_addon_buffer_2(DATA_TYPE essencetype, int * candidate_addon,unsigned int candidate_addon_size,unsigned int candidate_num, char * addon_buffer,unsigned int & addon_num);
		unsigned int generate_spec_addon_buffer(DATA_TYPE essencetype, char * addon_buffer, unsigned int max_addon_size, unsigned int &addon_num,int * sa_list);	
		
	protected:
		elementdataman * _edm;
	public:
		elementdataman * GetElementDataMan() { return _edm;}
		
	private:
		template<class T>
			class array
		{
		public:
			inline unsigned int size() { return _v.size(); }
			inline void push_back(const T & x) { _v.push_back(x); }
			inline const T & operator [](unsigned int pos) const { return _v[pos]; }
			inline T & operator [](unsigned int pos) {return _v[pos];}
		protected:
			abase::vector<T> _v;
		};
		
		array<void *>				sale_item_ptr_array;
		array<unsigned int>				sale_item_size_array;
		
		struct LOCATION
		{
			DATA_TYPE		type;
			unsigned int	pos;
		};
		
		typedef std::unordered_map<unsigned int, LOCATION> IDToLOCATIONMap;
		IDToLOCATIONMap				sale_item_id_index_map;
};

#pragma pack()
#endif
