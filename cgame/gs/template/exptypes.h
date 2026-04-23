#ifndef _EXP_TYPES_H_
#define _EXP_TYPES_H_

#include <stdio.h>
#include <stdlib.h>

#if defined(_WINDOWS) || defined(WIN32) || defined(_WIN32)
	typedef	unsigned __int64 UINT64;
	typedef __int64	INT64;
#else
	#include <stdint.h>
	typedef uint64_t UINT64;
	typedef int64_t	INT64;
#endif

#define ELEMENTDATA_VERSION		0x30000160 //174
enum
{
	ELEMENTDATA_NUM_PROFESSION	=	15,	//	ְҵ��Ŀ
	ELEMENTDATA_NUM_GENDER		=	2,	//	�Ա���Ŀ
	ELEMENTDATA_NUM_POKER_TYPE	=	6,	//	�����������
	ELEMENTDATA_NUM_POKER_RANK	=	5,	//	����Ʒ�׸���
	ELEMENTDATA_MAX_POKER_LEVEL	=	100,//	������ߵȼ�
	ELEMENTDATA_MAX_UNIVERSAL_TOKEN_USAGE	=	32,		//	ͨ�ÿͻ��˹�����Ʒ��ʹ�÷�ʽ������
	ELEMENTDATA_MAX_ENGRAVE_ADDON_COUNT		=	3,		//	�Կ̲��������������
	ELEMENTDATA_MAX_INHERIT_ADDON_COUNT		=	5,		//	�������Լ̳������������
	ELEMENTDATA_DOMAIN_COUNT	=	52,	//	��ս������
	ELEMENTDATA_NUM_ASTROLABE_LEVEL				=	100, 	// 176+ 105	���������ȼ�����
	ELEMENTDATA_NUM_ASTROLABE_ADDON_RANDOM_LEVEL=	10,		//	���������������ȼ�����
	ELEMENTDATA_MAX_ASTROLABE_ADDON_COUNT		=	10,		//	����������Ŀ���ֵ
	ELEMENTDATA_NUM_SOLO_TOWER_CHALLENGE_STAGE	=	108,	//	���������������ؿ���
	ELEMENTDATA_NUM_SOLO_TOWER_CHALLENGE_BUFF_COUNT = 16,	//	���������������û��ֻ�ȡ��Buff����
	ELEMENTDATA_NUM_HOME_CONFIG	=	32,
	ELEMENTDATA_NUM_HOME_CONFIG_MAP  = 16,
};

enum SERVICE_TYPE
{
	// ��̸����
	NPC_TALK = 0x80000000,
	// ������Ʒ�ķ���
	NPC_SELL,
	// �չ���Ʒ�ķ���
	NPC_BUY,
	// ������Ʒ�ķ���
	NPC_REPAIR,
	// ��Ƕ����
	NPC_INSTALL,
	// �������
	NPC_UNINSTALL,
	// ������ط���,�ַ��������������Լ�����������Ʒ
	NPC_GIVE_TASK,
	NPC_COMPLETE_TASK,
	NPC_GIVE_TASK_MATTER,
	// ������ط���
	NPC_SKILL,
	// ���Ʒ���
	NPC_HEAL,
	// ���ͷ���
	NPC_TRANSMIT,
	// �������
	NPC_TRANSPORT,
	// ���۷���
	NPC_PROXY,
	// �洢��Ʒ����Ǯ
	NPC_STORAGE,
	// ��������
	NPC_MAKE,
	// �ֽ����
	NPC_DECOMPOSE,
	// TALK����
	TALK_RETURN,
	// �����Ի�
	TALK_EXIT,
	// �ֿ�����
	NPC_STORAGE_PASSWORD,
	// ��������
	NPC_IDENTIFY,
	// ��������
	TALK_GIVEUP_TASK,
	// ��ս�����������
	NPC_WAR_TOWERBUILD,
	// ϴ�����
	NPC_RESETPROP,
	// �����������
	NPC_PETNAME,
	// ����ѧϰ���ܷ���
	NPC_PETLEARNSKILL,
	// �����������ܷ���
	NPC_PETFORGETSKILL,
	// װ���󶨷���
	NPC_EQUIPBIND,
	// װ�����ٷ���
	NPC_EQUIPDESTROY,
	// װ��������ٷ���
	NPC_EQUIPUNDESTROY,
	// �ʺŲֿ�
	NPC_ACCOUNT_STORAGE,
	// �Կ̷���
	NPC_ENGRAVE,
	// װ��������������ԣ�
	NPC_RANDPROP,
	// NEW
	NPC_ARENA_SELL,
};

#define		NUM_WEAPON_TYPE		18

#pragma pack(push, EXP_TYPES_INC, 4)

#ifdef _WINDOWS
typedef wchar_t namechar;
#else
typedef unsigned short namechar;
#endif

///////////////////////////////////////////////////////////////////////////////////////
//
// ����װ����ģ����Ҫʹ�õĸ������Ե����ݽṹ����
//
//		ע�⣺װ���������������ߡ���Ʒ�ȼ�����
//
//
///////////////////////////////////////////////////////////////////////////////////////


struct EQUIPMENT_ADDON
{
	unsigned int	id;							// �������Ե�ID����������ƷʱҪ���14��13λ��ʾ��
												// �������ԵĲ�����������12λ����ռ��
												// ���������ڲ���IDʱ������ʹ��12��13��14����λ

	namechar		name[32];					// �������Ե�����

	int				num_params;					// ���������ԵĲ�������
	int				param1;						// ��1�������������Ǹ�����
	int				param2;						// ��2�������������Ǹ�����
	int				param3;						// ��3�������������Ǹ�����

};


///////////////////////////////////////////////////////////////////////////////////////
//
// ����ģ�����ݽṹ����
//
//
///////////////////////////////////////////////////////////////////////////////////////

// ���������
struct WEAPON_MAJOR_TYPE
{
	unsigned int	id;							// ���������ID
	namechar		name[32];					// �������������
};

// ����С���
struct WEAPON_SUB_TYPE
{
	unsigned int	id;							// ����С���ID
	namechar		name[32];					// ����С�������
	
	char			file_hitgfx[128];			// ����Ч������
	char			file_hitsfx[128];			// ��������Ч������

	float			probability_fastest;		// ����Ƶ�ʷֲ��ĸ���: ����	-0.1
	float			probability_fast;			// ����Ƶ�ʷֲ��ĸ���: ��	-0.05
	float			probability_normal;			// ����Ƶ�ʷֲ��ĸ���: ��ͨ	0
	float			probability_slow;			// ����Ƶ�ʷֲ��ĸ���: ��	+0.05
	float			probability_slowest;		// ����Ƶ�ʷֲ��ĸ���: ����	+0.1

	float			attack_speed;				// �����ٶȣ������ۼ�ʱ�䣩����λΪ��
	float			attack_short_range;			// ��С����������С�������룬�ǹ���������Ϊ0.0

	// ����û����ʽ���߼�װ��Mask, ��Ϊ����ֻ��װ����������

	unsigned int	action_type;				// ���������Ŷ������
												// ���ֶ̡�˫�ֶ̡�˫�ֳ���˫�ֶ��ء�˫�ֳ��ء����ޡ�������ȭ�ס�ǹ
};

// ������������
struct WEAPON_ESSENCE
{
	// ������Ϣ
	unsigned int	id;							// ����(����)ID
	unsigned int	id_major_type;				// ���������ID
	unsigned int	id_sub_type;				// ����С���ID
	namechar		name[32];					// ��������, ���15������

	unsigned int	require_projectile;			// ��Ҫ�ĵ�ҩ����(�ӵ�ҩ������ѡȡ), ��������Ҫ�ĵ�ҩ���ͺ͵�ҩ�ϵ�����һ��ʱ����ʹ��

	// ��ʾ��Ϣ
	char			file_model_right[128];		// ����ģ��·����
	char			file_model_left[128];		// ����ģ��·����
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	// װ�����
	int				require_strength;			// ��������
	int				require_agility;			// ��������
	int				require_energy;				// ��������
	int				require_tili;				// ��������

	unsigned int	character_combo_id;			// ְҵ�������, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����

	int				require_level;				// �ȼ�����
	int				require_reputation;			// ��������

	// ��������
	int				level;						// �����ĵȼ�

	int				fixed_props;				// ���ɹ̶���������(0~3)

	//155
	float			probability_hide;

	int				damage_low;					// ������������Сֵ
	int				damage_high_min;			// �������������ֵ��������Сֵ
	int				damage_high_max;			// �������������ֵ���������ֵ
				
	int				magic_damage_low;			// ������������Сֵ
	int				magic_damage_high_min;		// �������������ֵ��������Сֵ
	int				magic_damage_high_max;		// �������������ֵ���������ֵ
				
	float			attack_range;				// ��������
	unsigned int	short_range_mode;			// ����������Զ�̱�־, 0-Զ�� ,1-����, 2-�̿ͽ���

	int				durability_min;				// �;ö���Сֵ
	int				durability_max;				// �;ö����ֵ

	int				levelup_addon;				// ����Ч��
	int				material_need;				// ��������Ļ���ʯ��Ŀ

	// ��Ǯ
	int				price;						// �����
	int				shop_price;					// ������

	int				repairfee;					// �����۸�

	// ������
	float			drop_probability_socket0;	// ����ʱ����0���Ļ���
	float			drop_probability_socket1;	// ����ʱ����1���Ļ���
	float			drop_probability_socket2;	// ����ʱ����2���Ļ���

	// ������
	float			make_probability_socket0;	// ����ʱ����0���Ļ���
	float			make_probability_socket1;	// ����ʱ����1���Ļ���
	float			make_probability_socket2;	// ����ʱ����2���Ļ���
	
	// ��������
	float			probability_addon_num0;		// �������Գ���0���Ļ���
	float			probability_addon_num1;		// �������Գ���1���Ļ���
	float			probability_addon_num2;		// �������Գ���2���Ļ���
	float			probability_addon_num3;		// �������Գ���3���Ļ���
	float			probability_addon_num4;		// �������Գ���4���Ļ���
	float			probability_addon_num5;		// �������Գ���5���Ļ���
	float			probability_addon_num6;		// �������Գ���5���Ļ���
	float			probability_unique;			// ����Ψһ���Եĸ���
	
	struct
	{
		unsigned int	id_addon;				// �������Ե�����ID
		float			probability_addon;		// �������Գ��ֵĸ���
	} addons[32];

	struct
	{
		unsigned int	id_rand;				// ������Ե�����ID
		float			probability_rand;		// ������Գ��ֵĸ���
	} rands[32];

	struct 
	{
		unsigned int	id_unique;				// Ψһ���Ե�����ID
		float			probability_unique;		// Ψһ���Գ��ֵĸ���
	} uniques[16];

	struct 
	{
		unsigned int	id_hide;				// Ψһ���Ե�����ID
		float			probability_hide;		// Ψһ���Գ��ֵĸ���
	} hides[16];

	int				durability_drop_min;		// ����ʱ���;ö���Сֵ
	int				durability_drop_max;		// ����ʱ���;ö���Сֵ

	// �����Ϣ
	int				decompose_price;			// ��ּ۸�
	int				decompose_time;				// ���ʱ��
	unsigned int	element_id;					// ��ֳɵ�Ԫʯ������ID
	int				element_num;				// ��ֲ�����Ԫʯ��
	
	unsigned int	id_drop_after_damaged;		// ��ٺ�������Ʒ
	int				num_drop_after_damaged;		// ��ٺ�������Ʒ����

	// �ѵ���Ϣ
	int				pile_num_max;				// �������Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};				


///////////////////////////////////////////////////////////////////////////////////////
//
// ����ģ�����ݽṹ����
//
//		���ǰѻ�����ΪArmor��ͬʱ�����еĶ����Ȱ���Diablo��ϰ���÷�Ҳ��armor�ƺ���
//
//
///////////////////////////////////////////////////////////////////////////////////////

// ���ߴ����
struct ARMOR_MAJOR_TYPE
{
	unsigned int	id;							// ���ߴ����ID
	namechar		name[32];					// ���ߴ��������
};

// ����С���
struct ARMOR_SUB_TYPE
{
	unsigned int	id;							// ����С���ID
	namechar		name[32];					// ����С�������

	unsigned int	equip_mask;					// ��װ���߼��Ͽ���װ���Ĳ�λ���ɲ�ͬ��λMask��ɣ�ʹ�õĲ�λ����Ϊ��
												// EQUIP_MASK_WEAPON       = 0x0001,
												// EQUIP_MASK_HEAD         = 0x0002,
												// EQUIP_MASK_NECK         = 0x0004,
												// EQUIP_MASK_SHOULDER     = 0x0008,
												// EQUIP_MASK_BODY         = 0x0010,
												// EQUIP_MASK_WAIST        = 0x0020,
												// EQUIP_MASK_LEG          = 0x0040,
												// EQUIP_MASK_FOOT         = 0x0080,
												// EQUIP_MASK_WRIST        = 0x0100,
												// EQUIP_MASK_FINGER1      = 0x0200,
												// EQUIP_MASK_FINGER2      = 0x0400,
												// EQUIP_MASK_PROJECTILE   = 0x0800,
};

// ���߱�������
struct ARMOR_ESSENCE
{
	// ������Ϣ
	unsigned int	id;							// ����(����)ID
	unsigned int	id_major_type;				// ���ߴ����ID
	unsigned int	id_sub_type;				// ����С���ID
	namechar		name[32];					// ��������, ���15������

	char			realname[32];				// ��ʵװ����

	// ��ʾ��Ϣ
	// ��ģ���в��ٶ��廤�ߵ�ģ������Ϊ��Щ���߻�Ϊÿ��������һ����ͬ��ģ��,
	// ��ģ�͵����������ǹ̶���:������+װ����.ski
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	unsigned int	equip_location;				// �û���ģ�͸��ǵĲ�λ���ɲ�ͬ��λMask��ɣ����ܵĲ�λ�У�����������������Ь�ĸ�����

	int				level;						// �����ȼ�

	// װ�����
	int				require_strength;			// ��������
	int				require_agility;			// ��������
	int				require_energy;				// ��������
	int				require_tili;				// ��������

	unsigned int	character_combo_id;			// ְҵ�������, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����

	int				require_level;				// �ȼ�����
	int				require_reputation;			// ��������

	int				fixed_props;				// ���ɹ̶���������(0~3)

	// ��������
	int				defence_low;				// ������������Сֵ
	int				defence_high;				// �������������ֵ
	
	struct {
		int			low;						// ������������Сֵ
		int			high;						// �������������ֵ
	} magic_defences[5];						// ��5������Ϊ�ֽ�ľˮ������ϵ

	int				mp_enhance_low;				// ��MP����Сֵ
	int				mp_enhance_high;			// ��MP�����ֵ

	int				hp_enhance_low;				// ��HP����Сֵ
	int				hp_enhance_high;			// ��HP�����ֵ

	int				armor_enhance_low;			// �Ӷ����ȵ���Сֵ
	int				armor_enhance_high;			// �Ӷ����ȵ����ֵ

	int				durability_min;				// �;ö���Сֵ
	int				durability_max;				// �;ö����ֵ

	int				levelup_addon;				// ����Ч��
	int				material_need;				// ��������Ļ���ʯ��Ŀ

	// ��Ǯ
	int				price;						// �����
	int				shop_price;					// ������

	int				repairfee;					// �����۸�

	// ������
	float			drop_probability_socket0;	// ����ʱ����0���Ļ���
	float			drop_probability_socket1;	// ����ʱ����1���Ļ���
	float			drop_probability_socket2;	// ����ʱ����2���Ļ���
	float			drop_probability_socket3;	// ����ʱ����3���Ļ���
	float			drop_probability_socket4;	// ����ʱ����4���Ļ���

	// ������
	float			make_probability_socket0;	// ����ʱ����0���Ļ���
	float			make_probability_socket1;	// ����ʱ����1���Ļ���
	float			make_probability_socket2;	// ����ʱ����2���Ļ���
	float			make_probability_socket3;	// ����ʱ����3���Ļ���
	float			make_probability_socket4;	// ����ʱ����4���Ļ���
	
	// ��������
	float			probability_addon_num0;		// �������Գ���0���Ļ���
	float			probability_addon_num1;		// �������Գ���1���Ļ���
	float			probability_addon_num2;		// �������Գ���2���Ļ���
	float			probability_addon_num3;		// �������Գ���3���Ļ���
	float			probability_addon_num4;		// �������Գ���4���Ļ���
	
	struct
	{
		unsigned int	id_addon;				// �������Ե�����ID
		float			probability_addon;		// �������Գ��ֵĸ���
	} addons[32];

	struct
	{
		unsigned int	id_rand;				// ������Ե�����ID
		float			probability_rand;		// ������Գ��ֵĸ���
	} rands[32];

	int				durability_drop_min;		// ����ʱ���;ö���Сֵ
	int				durability_drop_max;		// ����ʱ���;ö���Сֵ
	
	// �����Ϣ
	int				decompose_price;			// ��ּ۸�
	int				decompose_time;				// ���ʱ��
	unsigned int	element_id;					// ��ֳɵ�Ԫʯ������ID
	int				element_num;				// ��ֲ�����Ԫʯ��
			   
	unsigned int	id_drop_after_damaged;		// ��ٺ�������Ʒ
	int				num_drop_after_damaged;		// ��ٺ�������Ʒ����
	
	unsigned int	id_hair;					// ͷ��ģ��ID������ͷ��ʱ��Ч
	unsigned int	id_hair_texture;			// ͷ������ID������ͷ��ʱ��Ч
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �û��ߵĶѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
	
	unsigned int	force_all_magic_defences;	//	ǿ�������������з���
	
	char			high_poly_realname[32];
};


///////////////////////////////////////////////////////////////////////////////////////
//
// ��Ʒģ�����ݽṹ����
//
//
///////////////////////////////////////////////////////////////////////////////////////

// ��Ʒ�����
struct DECORATION_MAJOR_TYPE
{
	unsigned int	id;							// ��Ʒ�����ID
	namechar		name[32];					// ��Ʒ���������
};

// ��ƷС���
struct DECORATION_SUB_TYPE
{
	unsigned int	id;							// ��ƷС���ID
	namechar		name[32];					// ��ƷС�������

	unsigned int	equip_mask;					// ��װ���߼��Ͽ���װ���Ĳ�λ���ɲ�ͬ��λMask���
};

// ��Ʒ��������
struct DECORATION_ESSENCE
{
	// ������Ϣ
	unsigned int	id;							// ��Ʒ(����)ID
	unsigned int	id_major_type;				// ��Ʒ�����ID
	unsigned int	id_sub_type;				// ��ƷС���ID
	namechar		name[32];					// ��Ʒ����, ���15������

	// ��ʾ��Ϣ
	char			file_model[128];			// ģ��·����
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				level;						// �����ȼ�

	// װ�����
	int				require_strength;			// ��������
	int				require_agility;			// ��������
	int				require_energy;				// ��������
	int				require_tili;				// ��������
	
	unsigned int	character_combo_id;			// ְҵ�������, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����

	int				require_level;				// �ȼ�����
	int				require_reputation;			// ��������

	int				fixed_props;				// ���ɹ̶���������(0~3)

	// ��������
	int				damage_low;					// ������������Сֵ
	int				damage_high;				// �������������ֵ
				
	int				magic_damage_low;			// ������������Сֵ
	int				magic_damage_high;			// �������������ֵ

	int				defence_low;				// ������������Сֵ
	int				defence_high;				// �������������ֵ
				
	struct {
		int			low;						// ������������Сֵ
		int			high;						// �������������ֵ
	} magic_defences[5];						// ��5������Ϊ�ֽ�ľˮ������ϵ

	int				armor_enhance_low;			// �Ӷ����ȵ���Сֵ
	int				armor_enhance_high;			// �Ӷ����ȵ����ֵ

	int				durability_min;				// �;ö���Сֵ
	int				durability_max;				// �;ö����ֵ

	int				levelup_addon;				// ����Ч��
	int				material_need;				// ��������Ļ���ʯ��Ŀ

	// ��Ǯ
	int				price;						// �����
	int				shop_price;					// ������

	int				repairfee;					// �����۸�

	// ��������
	float			probability_addon_num0;		// �������Գ���0���Ļ���
	float			probability_addon_num1;		// �������Գ���1���Ļ���
	float			probability_addon_num2;		// �������Գ���2���Ļ���
	float			probability_addon_num3;		// �������Գ���3���Ļ���
	float			probability_addon_num4;		// �������Գ���4���Ļ���
	
	struct
	{
		unsigned int	id_addon;				// �������Ե�����ID
		float			probability_addon;		// �������Գ��ֵĸ���
	} addons[32];

	struct
	{
		unsigned int	id_rand;				// ������Ե�����ID
		float			probability_rand;		// ������Գ��ֵĸ���
	} rands[32];

	int				durability_drop_min;		// ����ʱ���;ö���Сֵ
	int				durability_drop_max;		// ����ʱ���;ö���Сֵ

	// �����Ϣ
	int				decompose_price;			// ��ּ۸�
	int				decompose_time;				// ���ʱ��
	unsigned int	element_id;					// ��ֳɵ�Ԫʯ������ID
	int				element_num;				// ��ֲ�����Ԫʯ��

	unsigned int	id_drop_after_damaged;		// ��ٺ�������Ʒ
	int				num_drop_after_damaged;		// ��ٺ�������Ʒ����
	
	// �ѵ���Ϣ
	int				pile_num_max;				// ����Ʒ�Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};


/////////////////////////////////////////////////////////////////////////////////////////////////
// ��װ�����ͻ�����ʾʱ��ѯ������
/////////////////////////////////////////////////////////////////////////////////////////////////

// ��װ����
struct SUITE_ESSENCE
{
	// ������Ϣ
	unsigned int	id;							// ��װ(����)ID
	namechar		name[32];					// ��װ����, ���15������

	int				max_equips;					// ��װ����
	int				used_in_treasury;

	struct 
	{
		unsigned int	id;						// ��װ���1~12
	} equipments[12];

	struct 
	{
		unsigned int	id;						// 2����12���ܼ���ĸ�������
	} addons[11];

	char			file_gfx[128];				// ���׺��Ч��
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// ������װ
/////////////////////////////////////////////////////////////////////////////////////////////////

// ��װ����
struct POKER_SUITE_ESSENCE
{
	// ������Ϣ
	unsigned int	id;							// ��װ(����)ID
	namechar		name[32];					// ��װ����, ���15������
	
	float			promote_ratio;				// ��������������������
	unsigned int    list[6];					// ��װ���
};


///////////////////////////////////////////////////////////////////////////////////////
//
// �������������
//
///////////////////////////////////////////////////////////////////////////////////////
struct POKER_DICE_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������
	
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				swallow_exp;				// ���ɺ��ṩ����ֵ
	
	struct
	{
		unsigned int	id;						// ����id
		float			probability;			// ��������		
	}list[256];
	
	int				price;						// �����
	int				shop_price;					// ������
	int				pile_num_max;				// �ѵ�����
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	unsigned int	proc_type;					// ������ʽ
};


///////////////////////////////////////////////////////////////////////////////////////
//
// ����С���
//
///////////////////////////////////////////////////////////////////////////////////////
struct POKER_SUB_TYPE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				type;						// ����(type=poker_type_type)

	union
	{
		struct  
		{
			unsigned int equip_mask1;			// �������õ��߼���λ1(type=equip_mask)
			unsigned int equip_mask2;			// �������õ��߼���λ2(type=equip_mask2)
		};
		UINT64			equip_mask;
	};
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ���Ʊ���
//
///////////////////////////////////////////////////////////////////////////////////////
struct POKER_ESSENCE
{
	unsigned int	id;							// (����)ID
	unsigned int	id_sub_type;				// С���ID
	namechar		name[32];					// ����, ���15������
	
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����	
	char			file_img[128];				// չʾ��Դ·��
	char			file_gfx[128];				// ��Ч·��

	int				require_level;				// ��ҵȼ�Ҫ�������ʷ�ȼ���
	int				require_control_point[2];	// ͳ����Ҫ��

	int				rank;						// Ʒ��(type=poker_rank)
	int				max_level;					// �ȼ�����
	
	int				hp;							// ��ʼHP
	int				damage;						// ��ʼ�﹥
	int				magic_damage;				// ��ʼ����
	int				defence;					// ��ʼ���
	int				magic_defence[5];			// ��ʼ����
	int				vigour;						// ��ʼ����

	int				inc_hp;						// ÿ������HP
	int				inc_damage;					// ÿ�������﹥
	int				inc_magic_damage;			// ÿ�����ӷ���
	int				inc_defence;				// ÿ���������
	int				inc_magic_defence[5];		// ÿ�����ӷ���
	int				inc_vigour;					// ÿ����������
	
	int				swallow_exp;				// ���ɺ��ṩ����ֵ	

	int				addon[4];					// ��������

	int				show_order;					// չʾ˳��
	
	int				price;						// �����
	int				shop_price;					// ������
	int				pile_num_max;				// �ѵ�����
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	unsigned int	proc_type;					// ������ʽ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ʱװģ�����ݽṹ����
//
//
///////////////////////////////////////////////////////////////////////////////////////

// ʱװ�����
struct FASHION_MAJOR_TYPE
{
	unsigned int	id;							// ʱװ�����ID
	namechar		name[32];					// ʱװ���������
};

// ʱװС���
struct FASHION_SUB_TYPE
{
	unsigned int	id;							// ʱװС���ID
	namechar		name[32];					// ʱװС�������

	unsigned int	equip_fashion_mask;			// ��װ���߼��Ͽ���װ���Ĳ�λ���ɲ�ͬ��λMask��ɣ�����߲�ͬ��ʹ�õĲ�λ����Ϊ��
												// EQUIP_MASK_FASHION_BODY = 0x00002000,
												// EQUIP_MASK_FASHION_LEG  = 0x00004000,
												// EQUIP_MASK_FASHION_FOOT = 0x00008000,
												// EQUIP_MASK_FASHION_WRIST= 0x00010000,
												// EQUIP_MASK_FASHION_HEAD = 0x02000000,
												// EQUIP_MASK_FASHION_WEAPON = 0x20000000,
};

// ʱװ��������
struct FASHION_ESSENCE
{
	// ������Ϣ
	unsigned int	id;							// ʱװ(����)ID
	unsigned int	id_major_type;				// ʱװ�����ID
	unsigned int	id_sub_type;				// ʱװС���ID
	namechar		name[32];					// ʱװ����, ���15������

	char			realname[32];				// ��ʵʱװ��

	// ��ʾ��Ϣ
	// ��ģ���в��ٶ��廤�ߵ�ģ������Ϊ��Щ���߻�Ϊÿ��������һ����ͬ��ģ��,
	// ��ģ�͵����������ǹ̶���:������+װ����.ski
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	unsigned int	equip_location;				// �û���ģ�͸��ǵĲ�λ���ɲ�ͬ��λMask��ɣ����ܵĲ�λ�У�����������������Ь�ĸ�����

	int				level;						// �����ȼ�

	// װ�����
	int				require_level;				// �ȼ�����

	// Ⱦɫ
	int				require_dye_count;			// Ⱦɫ�����Ⱦɫ����Ŀ
	
	// ��Ǯ
	int				price;						// �����
	int				shop_price;					// ������

	// �Ա�
	int				gender;						// ��ʹ�õ��˵��Ա�: 0-�У�1-Ů

	unsigned int	id_hair;					// ͷ��ģ��ID������ͷ��ʱ��Ч
	unsigned int	id_hair_texture;			// ͷ������ID������ͷ��ʱ��Ч

	// �ѵ���Ϣ
	int				pile_num_max;				// �û��ߵĶѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ

	char			wear_action[32];			// ʱװ��������
	
	char			file_model_right[128];		// ʱװ��������ģ��·����
	char			file_model_left[128];		// ʱװ��������ģ��·����
	unsigned int	character_combo_id;			// ʱװ����ְҵ����
	unsigned int	action_type;				// ʱװ�������Ŷ������

	unsigned int	combined_switch;			// ������Ͽ��أ��� FASHION_COMBINED_SWITCH ��

	float			h_min;						// ɫ����Сֵ
	float			h_max;						// ɫ�����ֵ
	float			s_min;						// ���Ͷ���Сֵ
	float			s_max;						// ���Ͷ����ֵ
	float			v_min;						// ������Сֵ
	float			v_max;						// �������ֵ
	
	
	int				related_item_id;
};

enum FASHION_COMBINED_SWITCH
{
	FCS_RANDOM_COLOR_IN_RANGE	= 0x00000001,	// ָ����Χ�������ɫ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ������Ʒ��ģ��
//
//		ע�⣺������,��Ʒ�Ǹ��������Ʒ����,������ͨ��Ʒ,������Ʒ�Լ�������Ʒ������
//
//
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
//
// ҩƷģ�����ݽṹ����
//
//
///////////////////////////////////////////////////////////////////////////////////////
// ҩƷ�����
struct MEDICINE_MAJOR_TYPE
{
	unsigned int	id;							// ҩƷ�����ID
	namechar		name[32];					// ҩƷ���������
};

// ҩƷС���
struct MEDICINE_SUB_TYPE
{
	unsigned int	id;							// ҩƷС���ID
	namechar		name[32];					// ҩƷС�������
};

// ҩƷ��������
struct MEDICINE_ESSENCE
{
	unsigned int	id;							// ҩƷ(����)ID
	unsigned int	id_major_type;				// ҩƷ�����ID
	unsigned int	id_sub_type;				// ҩƷС���ID
	namechar		name[32];					// ҩƷ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				require_level;				// ��ҵļ���Ҫ��
	int				cool_time;					// ��ȴʱ�䣨���룩

	int				hp_add_total;				// ��HP����������
	int				hp_add_time;				// ��HP�����������ʱ��
	int				mp_add_total;				// ��MP����������
	int				mp_add_time;				// ��MP�����������ʱ��

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// ��ҩƷ�Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ����װ����Ʒ
//
///////////////////////////////////////////////////////////////////////////////////////
struct BIBLE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	unsigned int	id_addons[10];				// 10 ����������

	int				price;						// �����
	int				shop_price;					// ������

	unsigned int	id_drop_after_damaged;		// ��ٺ�������Ʒ
	int				num_drop_after_damaged;		// ��ٺ�������Ʒ����
	
	// Decomposicao (Novo)
	int				decompose_price;			// ��ּ۸�
	int				decompose_time;				// ���ʱ��
	unsigned int	element_id;					// ��ֳɵ�Ԫʯ������ID
	int				element_num;				// ��ֲ�����Ԫʯ��
	
	
	// �ѵ���Ϣ
	int				pile_num_max;				// ��ҩƷ�Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
	

	// Nível do Refino (Novo)
	unsigned int	level;
	int	levelup_addon;
	
	// Atributos de Refino(Novo)
	struct
	{
		unsigned int id_addon;
		unsigned int refine_level;
	}levelup_addons[12];
	
};

struct SPEAKER_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				id_icon_set;				// ��������׺�

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// ��ҩƷ�Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

struct AUTOHP_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				total_hp;					// ��Ѫ����
	float			trigger_amount;				// ������Ѫ����
	int				cool_time;					// ��ȴʱ�䣨���룩

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// ��ҩƷ�Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

struct AUTOMP_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				total_mp;					// ��ħ����
	float			trigger_amount;				// ������ħ����
	int				cool_time;					// ��ȴʱ�䣨���룩

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// ��ҩƷ�Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

//	��������
struct FORCE_TOKEN_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];				// ����, ���15������

	char					file_matter[128];		//	�����ģ��
	char					file_icon[128];			//	ͼ������

	unsigned int	require_force;			//	��������
	int					reputation_add;		//	��������
	int					reputation_increase_ratio;	//	�������Ӹ��Ӱٷֱ�

	int					price;							//	�����
	int					shop_price;				//	������

	int					pile_num_max;			//	�ѵ�����
	unsigned int	has_guid;					//	�Ƿ�Ϊ�����ȫ��ΨһID
	unsigned int	proc_type;					//	������ʽ
};

// ����װ������ô�װ���������Ӧ���ܣ�
struct DYNSKILLEQUIP_ESSENCE
{
	unsigned int	id;								// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				id_skill[4];				// ��ü���

	int				price;							// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ����ԭ��ģ�����ݽṹ����
//
///////////////////////////////////////////////////////////////////////////////////////
// ����ԭ�ϴ����
struct MATERIAL_MAJOR_TYPE
{
	unsigned int	id;							// ����ԭ�ϴ����ID
	namechar		name[32];					// ����ԭ�ϴ��������
};

// ����ԭ��С���
struct MATERIAL_SUB_TYPE
{
	unsigned int	id;							// ����ԭ��С���ID
	namechar		name[32];					// ����ԭ��С�������
};

// ����ԭ�ϱ�������
struct MATERIAL_ESSENCE
{
	unsigned int	id;							// ����ԭ��(����)ID
	unsigned int	id_major_type;				// ����ԭ�ϴ����ID
	unsigned int	id_sub_type;				// ����ԭ��С���ID
	namechar		name[32];					// ����ԭ������, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				price;						// �����
	int				shop_price;					// ������

	// �����Ϣ
	int				decompose_price;			// ��ּ۸�
	int				decompose_time;				// ���ʱ��
	unsigned int	element_id;					// ��ֳɵ�Ԫʯ������ID
	int				element_num;				// ��ֲ�����Ԫʯ��

	// �ѵ���Ϣ
	int				pile_num_max;				// ��ԭ�ϵĶѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

///////////////////////////////////////////////////////////////////////////////////////
// �������ʵ�������
///////////////////////////////////////////////////////////////////////////////////////
struct REFINE_TICKET_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	namechar		desc[16];					// ������

	float			ext_reserved_prob;			// ���ʧ�ܺ�ֻ��һ���ĸ���
	float			ext_succeed_prob;			// ��߾����ɹ��ĸ���

	unsigned int	fail_reserve_level;			// �Ƿ�ʧ�ܲ��������ȼ�
	float			fail_ext_succeed_prob[12];	// �ھ���ʧ�ܲ����ȼ�ʱ�ĳɹ����ʵ���

	unsigned int binding_only;				//	ֻ�����˺�һװ��
	int				require_level_max;		//	����Ʒ������

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
	unsigned int	bind_refine;
	
	unsigned int 	account_bind_refine;
};

///////////////////////////////////////////////////////////////////////////////////////
// ˫���������
///////////////////////////////////////////////////////////////////////////////////////
struct DOUBLE_EXP_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				double_exp_time;			// ˫������ʱ��(��)

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

///////////////////////////////////////////////////////////////////////////////////////
// ʱװȾɫ����
///////////////////////////////////////////////////////////////////////////////////////
struct DYE_TICKET_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	float			h_min;						// ɫ����Сֵ
	float			h_max;						// ɫ�����ֵ
	float			s_min;						// ���Ͷ���Сֵ
	float			s_max;						// ���Ͷ����ֵ
	float			v_min;						// ������Сֵ
	float			v_max;						// �������ֵ
	
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ

	unsigned int	usage;						// Ⱦɫ����;��0:��ͨȾɫ��ʱװ������ȣ���1:ǩ��īˮ�ȣ�
};

///////////////////////////////////////////////////////////////////////////////////////
// ���������е���Ʒ
///////////////////////////////////////////////////////////////////////////////////////
struct DESTROYING_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// �����Ż���ģ�����ݽṹ����
//
//		������,���ǰ�"��"����Diabllo�е�ʹ�÷���,��Ϊ"rune",��Ϊ�Ŵ���ŷʹ�õ�����, ʫ��
//
//
///////////////////////////////////////////////////////////////////////////////////////
// �����Ż���С���
struct DAMAGERUNE_SUB_TYPE
{
	unsigned int	id;							// �����Ż���С���ID
	namechar		name[32];					// �����Ż���С�������
};

// �����Ż�����������
struct DAMAGERUNE_ESSENCE
{
	unsigned int	id;							// (����)ID
	unsigned int	id_sub_type;				// �����Ż���С���ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����
	
	unsigned int	damage_type;				// ����ѡ������������ѡ��(0 ��ʾ����, 1 ��ʾ����)

	int				price;						// �����
	int				shop_price;					// ������

	int				require_weapon_level_min;	// ʹ�������ļ������䣺��С
	int				require_weapon_level_max;	// ʹ�������ļ������䣺���
	
	int				damage_increased;			// ���������������̶�ֵ(int)

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// �����Ż���ģ�����ݽṹ����
//
//		������,���ǰ�"��"����Diabllo�е�ʹ�÷���,��Ϊ"rune",��Ϊ�Ŵ���ŷʹ�õ�����, ʫ��
//
//
///////////////////////////////////////////////////////////////////////////////////////
// �����Ż���С���
struct ARMORRUNE_SUB_TYPE
{
	unsigned int	id;							// �����Ż���С���ID
	namechar		name[32];					// �����Ż���С�������
};

// �����Ż�����������
struct ARMORRUNE_ESSENCE
{
	unsigned int	id;							// (����)ID
	unsigned int	id_sub_type;				// �����Ż���С���ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����
	char			file_gfx[128];				// ʹ��ʱ��Ч��
	char			file_sfx[128];				// ʹ��ʱ����Ч

	unsigned int	damage_type;				// ����ѡ������������ѡ��(0 ��ʾ����, 1 ��ʾ����)

	int				price;						// �����
	int				shop_price;					// ������

	int				require_player_level_min;	// ʹ����Ҫ�����Ｖ�����䣺��С
	int				require_player_level_max;	// ʹ����Ҫ�����Ｖ�����䣺���
	
	float			damage_reduce_percent;		// �Թ������������Ľ��Ͱٷֱ�
	int				damage_reduce_time;			// ���������Ĵ�������������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};


///////////////////////////////////////////////////////////////////////////////////////
//
// ������ģ�����ݽṹ����
//
//		������,���ǰ�"��"����Diabllo�е�ʹ�÷���,��Ϊ"tome",��Ϊ��, ��, ��, ����
//
//
///////////////////////////////////////////////////////////////////////////////////////
// ������С���
struct SKILLTOME_SUB_TYPE
{
	unsigned int	id;							// ������С���ID
	namechar		name[32];					// ������С�������
};
		   
// �����鱾������
struct SKILLTOME_ESSENCE
{
	unsigned int	id;							// (����)ID
	unsigned int	id_sub_type;				// ������С���ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

// �̳Ǵ���
struct SHOP_TOKEN_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������
	
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	unsigned int	id_token_shop;				// ��Ӧ�̳�
	
	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

// ͨ�ÿͻ��˹�����Ʒ
struct UNIVERSAL_TOKEN_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������
	
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����
	
	unsigned int	combined_switch;			// ������Ͽ��أ��� UNIVERSAL_TOKEN_COMBINED_SWITCH ��
	int				config_id[ELEMENTDATA_MAX_UNIVERSAL_TOKEN_USAGE];	//	����Ͽ��ض�Ӧ�����ò��� ��ÿ�ֶ�Ӧһ����Ͽ��أ�
	
	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

enum	UNIVERSAL_TOKEN_COMBINED_SWITCH
{
	UTCS_VISIT_HTTP_WITH_TOKEN	= 0x00000001,	//	���� token ��������ַ
};


/////////////////////////////////////////////////////////////////////////////////////////////////
// ����
/////////////////////////////////////////////////////////////////////////////////////////////////
struct ASTROLABE_ESSENCE
{
	unsigned int	id;							//	ID
	namechar		name[32];					//	����
	
	char			file_matter[128];			//	���ڵ��ϵ�ģ��·����
	char			file_icon[128];				//	ͼ��·����

	unsigned int	character_combo_id;			//	���ְҵ�������
	int				require_level;				//	�ȼ�����

	int				min_inner_point_value;		//	�ڵ���������
	int				max_inner_point_value;		//	�ڵ���������
	int				max_init_inner_point_value;	//	�ڵ��ʼ��������
	int				init_all_inner_point_value;	//	�ڵ��ʼ������
	int				max_all_inner_point_value;	//	�ڵ�����������

	struct
	{
		unsigned int	id_rand;				//	������Ե�����ID
		float			probability_rand;		//	������Գ��ֵĸ���
		int				max_appear_times_rand;	//	������������ִ���
	}rands[32];
	unsigned int	id_rand_when_fail;			//	������Բ�λ����ID

	int				base_swallow_exp;			//	���ɺ������ֵ

	int				price;						//	�����
	int				shop_price;					//	������
	
	int				pile_num_max;				//	�ѵ�����
	unsigned int	has_guid;					//	�Ƿ�Ϊ�����ȫ��ΨһID
	unsigned int	proc_type;					//	������ʽ
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// ���������������
/////////////////////////////////////////////////////////////////////////////////////////////////
struct ASTROLABE_RANDOM_ADDON_ESSENCE
{
	unsigned int	id;							//	ID
	namechar		name[32];					//	����
	
	char			file_matter[128];			//	���ڵ��ϵ�ģ��·����
	char			file_icon[128];				//	ͼ��·����
	
	int				addon_random_exp_gained;	//	���Ӿ���ֵ
	
	int				price;						//	�����
	int				shop_price;					//	������
	
	int				pile_num_max;				//	�ѵ�����
	unsigned int	has_guid;					//	�Ƿ�Ϊ�����ȫ��ΨһID
	unsigned int	proc_type;					//	������ʽ
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// ���������ʵ���
/////////////////////////////////////////////////////////////////////////////////////////////////
struct ASTROLABE_INC_INNER_POINT_VALUE_ESSENCE
{
	unsigned int	id;							//	ID
	namechar		name[32];					//	����
	
	char			file_matter[128];			//	���ڵ��ϵ�ģ��·����
	char			file_icon[128];				//	ͼ��·����
	
	int				increase_base;				//	������������ֵ
	float			increase_probability[10];	//	��������%������ֵ����

	int				require_min_all_inner_point_value;	//	�ڵ���������������
	int				require_max_all_inner_point_value;	//	�ڵ���������������
	
	int				price;						//	�����
	int				shop_price;					//	������
	
	int				pile_num_max;				//	�ѵ�����
	unsigned int	has_guid;					//	�Ƿ�Ϊ�����ȫ��ΨһID
	unsigned int	proc_type;					//	������ʽ
};


/////////////////////////////////////////////////////////////////////////////////////////////////
// �����Ǿ������
/////////////////////////////////////////////////////////////////////////////////////////////////
struct ASTROLABE_INC_EXP_ESSENCE
{
	unsigned int	id;							//	ID
	namechar		name[32];					//	����
	
	char			file_matter[128];			//	���ڵ��ϵ�ģ��·����
	char			file_icon[128];				//	ͼ��·����
	
	int				swallow_exp;				//	���ɺ��ṩ����ֵ
	
	int				require_level_min;
	int				require_level_max;
	
	int				price;						//	�����
	int				shop_price;					//	������
	
	int				pile_num_max;				//	�ѵ�����
	unsigned int	has_guid;					//	�Ƿ�Ϊ�����ȫ��ΨһID
	unsigned int	proc_type;					//	������ʽ
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// ͨ��ְҵ��Ʒ��
/////////////////////////////////////////////////////////////////////////////////////////////////
struct ITEM_PACKAGE_BY_PROFESSION_ESSENCE
{
	unsigned int	id;							//	ID
	namechar		name[32];					//	����
	
	char			file_matter[128];			//	���ڵ��ϵ�ģ��·����
	char			file_icon[128];				//	ͼ��·����

	struct
	{
		struct 
		{
			unsigned int item_id;				//	��ƷID
		}gender_list[ELEMENTDATA_NUM_GENDER];	//	�Ա�%
	}prof_list[ELEMENTDATA_NUM_PROFESSION];		//	ְҵ%
	int				item_expire_timelength;		//	��Ʒ����ʱ�䣨�룩

	int				price;						//	�����
	int				shop_price;					//	������
	
	int				pile_num_max;				//	�ѵ�����
	unsigned int	has_guid;					//	�Ƿ�Ϊ�����ȫ��ΨһID
	unsigned int	proc_type;					//	������ʽ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ������ʩģ�����ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
// ������ʩС���
struct FACTION_BUILDING_SUB_TYPE
{
	unsigned int	id;							// ������ʩС���ID
	namechar		name[32];					// ������ʩС�������
};
		   
// ������ʩ��������
struct FACTION_BUILDING_ESSENCE
{
	unsigned int	id;							// (����)ID
	unsigned int	id_sub_type;				// ������ʩС���ID
	namechar		name[32];					// ����, ���15������

	char			file_icon[128];				//	ͼ������

	int		level;								//	�ȼ�

	int		require_level;						//	���صȼ�����
	int		technology[5];						//	��Ҫ�Ƽ�%�츳����
	int		money;								//	��Ҫ��Ǯ
	int		material[8];						//	��Ҫ����%����

	int		base_time;							//	������ʱ��
	int		delta_time;							//	��������һ�ɼ��ٵĽ���ʱ��

	int		controller_id0;						//	�����еĿ�����ID
	int		controller_id1;						//	�����Ŀ�����ID
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ���ز���ģ�����ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct FACTION_MATERIAL_ESSENCE
{
	unsigned int	id;							//	(����)ID
	namechar		name[32];					//	����, ���15������

	char			file_matter[128];			//	�����ģ��
	char			file_icon[128];				//	ͼ������

	int				material_count[8];			//	�ȼ۲���%����

	int				price;						//	�����
	int				shop_price;					//	������

	int				pile_num_max;				//	�ѵ�����
	unsigned int	has_guid;					//	�Ƿ�Ϊ�����ȫ��ΨһID
	unsigned int	proc_type;					//	������ʽ
};


///////////////////////////////////////////////////////////////////////////////////////
//
// �ɽ���ģ�����ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct FLYSWORD_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_model[128];			// ģ��·����
	char			file_model2[128];			// ����ģ��·����
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����
	char			file_model3[128];			// ����ģ��·����

	int				price;						// �����
	int				shop_price;					// ������

	int				level;						// �ɽ��ȼ�
	int				require_player_level_min;	// ʹ����Ҫ��������Ｖ��

	float			speed_increase_min;			// ���������������
	float			speed_increase_max;			// ���������������
	float			speed_rush_increase_min;	// ��̷�����������
	float			speed_rush_increase_max;	// ��̷�����������

	float			time_max_min;				// ������ʱ������
	float			time_max_max;				// ������ʱ������

	float			time_increase_per_element;	// ÿװ��һ��Ԫʯ���ӵķ���ʱ��

	unsigned int	fly_mode;					// ����ģʽ��������: 0-�ɽ�, 1-���, 2-�����

	unsigned int	character_combo_id;			// ְҵ�������, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����

	unsigned int	id_drop_after_damaged;		// ��ٺ�������Ʒ
	int				num_drop_after_damaged;		// ��ٺ�������Ʒ����

	int				max_improve_level;			// ǿ���ȼ�����
	struct  
	{
		unsigned int require_item_num;			// ������Ʒ����
		float		speed_increase;				// ���ӳ�������ٶ�
		float		speed_rush_increase;		// ���ӳ�̷����ٶ�
	}improve_config[10];						// %��ǿ��
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ

	// �ɽ�û����ʽ���߼�װ��Mask, ��Ϊ�ɽ�ֻ��װ�ڷɽ�����
	unsigned int	unc1;

	//176+
	/*char 		tag_path[128];
	namechar	collection_desc[32];	
	int			rarity_level;
	int			use_mask;
	int			source_mask;
	int			source_npc_id;	
	namechar	source_desc[32];
	int 		fly_charm_value;	
	unsigned int	fly_type;*/
};

// ���˳����
struct WINGMANWING_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_model[128];			// ģ��·����
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				price;						// �����
	int				shop_price;					// ������

	int				require_player_level_min;	// ʹ����Ҫ��������Ｖ��
	float			speed_increase;				// ���ٶȵ���������

	int				mp_launch;					// ��ɺķѵ�MP
	int				mp_per_second;				// ÿ��ķѵ�MP

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ

	// ���˳��û����ʽ���߼�װ��Mask, ��Ϊ���˳��ֻ��װ�ڷɽ�����
};


///////////////////////////////////////////////////////////////////////////////////////
//
// �سǾ���ģ�����ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct TOWNSCROLL_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	float			use_time;					// ʹ��ʱ�䣨�룩

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ���ͷ�ģ�����ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct TRANSMITSCROLL_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				price;						// �����
	int				shop_price;					// ������

	unsigned int	cross_world_transmit;
	unsigned int	num_use_pertime;

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ���ɻسǾ���ģ�����ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct UNIONSCROLL_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	float			use_time;					// ʹ��ʱ�䣨�룩

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// �������ģ�����ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct REVIVESCROLL_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	float			use_time;					// ʹ��ʱ�䣨�룩
	int				cool_time;					// ��ȴʱ�䣨���룩

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// Ԫʯģ�����ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct ELEMENT_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				level;						// Ԫʯ�ȼ�

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
	unsigned int	bind_refine;
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ��Ҷ���ʯ
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct MONEY_CONVERTIBLE_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������
	
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����
	
	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ������Ʒģ�����ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct TASKMATTER_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_icon[128];				// ͼ��·����

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

///////////////////////////////////////////////////////////////////////////////////////
//	�����������ͨ��Ʒ
///////////////////////////////////////////////////////////////////////////////////////
struct TASKNORMALMATTER_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ����������������ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct TASKDICE_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	struct
	{
		unsigned int 	space;
		unsigned int	id;						// ����id
		float			probability;			// ����������ĸ���
		
	}task_lists[20];

	// ���˾��ñ�־
	unsigned int	use_on_pick;				// true or false, �ͻ��˶��ڼ���Լ��İ�����������������Ķ���������ʹ��
	
	//155
	unsigned int	price;
	unsigned int	shop_price;	
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
	int				no_use_in_combat;			//	ս��ʱ�Ƿ���ã�1���ǣ���0����
	int 			unk;
	int 			use_identification;

	//176+
	//unsigned int 	redenvelop_item;
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ����ģ�����ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct TOSSMATTER_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_model[128];			// ģ��·����
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����
	char			file_firegfx[128];			// ����Ч��
	char			file_hitgfx[128];			// ����Ч������
	char			file_hitsfx[128];			// ��������Ч������

	// ʹ�ù���
	int				require_strength;			// ��������
	int				require_agility;			// ��������
	int				require_level;				// �ȼ�����

	int				damage_low;					// ������������Сֵ
	int				damage_high_min;			// �������������ֵ��������Сֵ
	int				damage_high_max;			// �������������ֵ���������ֵ

	float			use_time;					// ʹ��ʱ�䣨�룩
	float			attack_range;				// ��������

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ��ҩģ�����ݽṹ����
//		
//		��ҩҪ����װ��, ��ʹ��, ��������װ��.
//
///////////////////////////////////////////////////////////////////////////////////////
struct PROJECTILE_TYPE
{
	unsigned int	id;							// ��ҩ����ID
	namechar		name[32];					// ����, ���15������

};

struct PROJECTILE_ESSENCE
{
	unsigned int	id;							// (����)ID
	unsigned int	type;						// ��ҩ���� (�ӵ�ҩ������ѡȡ)
	namechar		name[32];					// ����, ���15������

	char			file_model[128];			// ģ��·����
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	char			file_firegfx[128];			// ����Ч��
	char			file_hitgfx[128];			// ����Ч������
	char			file_hitsfx[128];			// ��������Ч������

	int				require_weapon_level_min;	// ʹ����������Χ: ��Сֵ (װ��ʱ���������,������ֻ�Ǹ��߻�������)
	int				require_weapon_level_max;	// ʹ����������Χ: ���ֵ

	int				damage_enhance;				// ������������������
	int				damage_scale_enhance;		// ����������������������
	
	int				price;						// �����
	int				shop_price;					// ������

	// ��������
	unsigned int	id_addon0;					// �������Ե�����ID
	unsigned int	id_addon1;					// �������Ե�����ID
	unsigned int	id_addon2;					// �������Ե�����ID
	unsigned int	id_addon3;					// �������Ե�����ID
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ

	// ��ҩû����ʽ�ƶ���װ��������Ϊ���еĵ�ҩ��װ��Mask����ΪEQUIP_MASK_PROJECTILE
};

// ����ļ���С���
struct QUIVER_SUB_TYPE
{
	unsigned int	id;							// ��������ID
	namechar		name[32];					// ��������, ���15������

};

// ���ұ���
struct QUIVER_ESSENCE
{
	unsigned int	id;							// (����)ID
	unsigned int	id_sub_type;				// ����С���ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	unsigned int	id_projectile;				// ʢװ�ĵ�ҩ����
	int				num_min;					// ��Ŀ��
	int				num_max;					// ��Ŀ��

};

// ��Ƕ�ñ�ʯС���
struct STONE_SUB_TYPE
{
	unsigned int	id;							// ��ʯ����ID
	namechar		name[32];					// ��ʯ����, ���15������

};

// ��Ƕ�ñ�ʯ����
struct STONE_ESSENCE
{
	unsigned int	id;							// (����)ID
	unsigned int	id_sub_type;				// ��ʯС���ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				level;						// ����
	int				color;						// ��ɫ (0-7)

	int				price;						// �����
	int				shop_price;					// ������

	int				install_price;				// ��Ƕ�۸�
	int				uninstall_price;			// ����۸�

	unsigned int	id_addon_damage;			// ������������id
	unsigned int	id_addon_defence;			// ������������id

	namechar		weapon_desc[16];			// ���������ϵ�����
	namechar		armor_desc[16];				// ���ڷ����ϵ�����

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ

	// ��ʯû����ʽ�ƶ���װ��������Ϊ���еı�ʯֻ��������Ƕ��Mask����Ϊ0x8000
		
	unsigned int	combined_switch;			// ������Ͽ��أ���STONE_COMBINED_SWITCH��

	unsigned int	id_addon_decoration;		// ��Ʒ��������id
	namechar		decoration_desc[16];		// ������Ʒ�ϵ�����
};

enum STONE_COMBINED_SWITCH
{
	SCS_WEAPON_CANNOT_USE	= 0x00000001,		//	����������
	SCS_ARMOR_CANNOT_USE	= 0x00000002,		//	���߲�����
	SCS_NECK_CAN_USE		= 0x00000004,		//	��������
	SCS_WAIST_CAN_USE		= 0x00000008,		//	�������
};

/////////////////////////////////////////////////////////////////////////////////////////
//
// ���Ի������Ʒ
//
/////////////////////////////////////////////////////////////////////////////////////////

// ���ݾ������
struct FACETICKET_MAJOR_TYPE
{
	unsigned int	id;							// ���ݾ������ID
	namechar		name[32];					// ���ݾ����������
};

// ���ݾ�С���
struct FACETICKET_SUB_TYPE
{
	unsigned int	id;							// ���ݾ�С���ID
	namechar		name[32];					// ���ݾ�С�������
};

// ���ݾ�����
struct FACETICKET_ESSENCE
{
	unsigned int	id;							// (����)ID
	unsigned int	id_major_type;				// ���ݾ������ID
	unsigned int	id_sub_type;				// ���ݾ�С���ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				require_level;				// ��������

	char			bound_file[128];			// ���Ի������ļ�
	unsigned int	unsymmetrical;				// �Ƿ������ǶԳƵ���

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �����ݾ��Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

// ����������
struct FACEPILL_MAJOR_TYPE
{
	unsigned int	id;							// ����������ID
	namechar		name[32];					// ��������������
};

// ������С���
struct FACEPILL_SUB_TYPE
{
	unsigned int	id;							// ������С���ID
	namechar		name[32];					// ������С�������
};

// �����豾��
struct FACEPILL_ESSENCE
{
	unsigned int	id;							// (����)ID
	unsigned int	id_major_type;				// ����������ID
	unsigned int	id_sub_type;				// ������С���ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				duration;					// ��Чʱ��(��)

	float			camera_scale;				// ���������������
	
	unsigned int	character_combo_id;			// ְҵ�������, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����

	struct
	{
		char		file[128];					// ������·��

	} pllfiles[ELEMENTDATA_NUM_PROFESSION * 2];				// ��������ı������ļ�

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ñ�����Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};


////////////////////////////////////////////////////////////////////////////////////
// GM ��Ʒ������
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// GM ��Ʒ���������Ͷ���
///////////////////////////////////////////////////////////////////////////////////////
struct GM_GENERATOR_TYPE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������
};

///////////////////////////////////////////////////////////////////////////////////////
// GM ��Ʒ���������ݽṹ����
///////////////////////////////////////////////////////////////////////////////////////
struct GM_GENERATOR_ESSENCE
{
	unsigned int	id;							// ID
	unsigned int	id_type;					// ���ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	unsigned int	id_object;					// �����ɵ���Ʒ�����ID

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

/////////////////////////////////////////////////////////////////////////////////////////
// ���������Ʒ
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// ���ﵰ
/////////////////////////////////////////////////////////////////////////////////////////
struct PET_EGG_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				id_pet;						// �����ģ��ID

	int				money_hatched;				// �����۸�
	int				money_restored;				// ��ԭ�۸�

	int				honor_point;				// ��ʼ�ҳ϶�
	int				level;						// ��ʼ���Ｖ��
	int				exp;						// ��ʼ���ﾭ��ֵ
	int				skill_point;				// ��ʼʣ�༼�ܵ�

    struct 
	{
		int			id_skill;					// ��ʼ����ID
		int			level;						// ��ʼ���ܵȼ�
	} skills[32];

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ó��ﵰ�Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
	int illustrated_only;


	//176+
	/*char		tag_path[128];
	namechar	collection_desc[32];

	int 		rarity_level;
	int 		use_mask;
	int			source_mask;
	int			source_npc_id;
	namechar	source_desc[32];
	int			pet_charm_value;*/
};

////////////////////////////////////////////////////////////////////////////////////////
// ����ʳƷ
////////////////////////////////////////////////////////////////////////////////////////
struct PET_FOOD_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				level;						// �ȼ�

	int				hornor;						// ���ӵ��ҳ϶�
	int				exp;						// ���ӵľ���ֵ

	int				food_type;					// ʳ���������Mask���ӵ͵�������Ϊ���ϡ����ࡢҰ�ˡ�ˮ������ˮ

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ó���ʳƷ�Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

////////////////////////////////////////////////////////////////////////////////////////
// �������ݾ�
////////////////////////////////////////////////////////////////////////////////////////
struct PET_FACETICKET_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ó������ݾ��Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

////////////////////////////////////////////////////////////////////////////////////////
// �̻�����
////////////////////////////////////////////////////////////////////////////////////////
struct FIREWORKS_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	char			file_fw[128];				// ���Ч���ļ�
	int				level;						// �ȼ�

	int				time_to_fire;				// ȼ��ʱ��(��)

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// ���̻��Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

// �̻�2����
struct FIREWORKS2_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����
	
	int				level;						// �ȼ�
	int				time_to_fire;				// ȼ��ʱ��(��)
	
	int				price;						// �����
	int				shop_price;					// ������

	int				male_mine_id;					// ����ʹ���̻������ɵĿ�id
	int				female_mine_id;					// Ů��ʹ���̻������ɵĿ�id

	char			cast_action[32];			// ʩ���߶�������

	namechar		caster_word[256];				// ʩ���ߺ���
	namechar		receiver_word[256];				// �����ߺ���
	
	// �ѵ���Ϣ
	int				pile_num_max;				// ���̻��Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	
	int unk1[2];
	int unk2[2];
	int unk4;
	
	
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

////////////////////////////////////////////////////////////////////////////////////////
// ��ս�����Ʒ
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// Ͷʯ���л�ȯ
////////////////////////////////////////////////////////////////////////////////////////
struct WAR_TANKCALLIN_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// ��Ͷʯ���л�ȯ�Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
	
	int tank_id[8];
	
	
};

// �����鱾������
struct SKILLMATTER_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				level_required;				// �ȼ�����

	int				id_skill;					// ����ID
	int				level_skill;				// ���ܵȼ�

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

struct INC_SKILL_ABILITY_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������
	
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����
	
	int				id_skill;					// ����ID
	int				level_required;				// ���ܵȼ�ǰ��
	float           inc_ratio;					// ����������
	
	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

////////////////////////////////////////////////////////////////////////////////////////
// ����ԤԼƾ֤
////////////////////////////////////////////////////////////////////////////////////////
struct WEDDING_BOOKCARD_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				year;						// ԤԼʱ�䣨�꣩
	int				month;						// ԤԼʱ�䣨�£�
	int				day;						// ԤԼʱ�䣨�գ�

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

////////////////////////////////////////////////////////////////////////////////////////
// �������
////////////////////////////////////////////////////////////////////////////////////////
struct WEDDING_INVITECARD_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����
	
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

////////////////////////////////////////////////////////////////////////////////////////
// ĥ��ʯ
////////////////////////////////////////////////////////////////////////////////////////
struct SHARPENER_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����
	
	int				level;						// ����

	unsigned int	equip_mask;					// �������õ��߼���λ

	int				addon[3];					// �������Ե�����ID
	int				addon_time;					// �������Ե���Чʱ�䣨�룩
	
	int				gfx_index;					// ��Ч��0~15)
	
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

////////////////////////////////////////////////////////////////////////////////////////
// ������
////////////////////////////////////////////////////////////////////////////////////////
struct CONGREGATE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����	

	unsigned int congregate_type;				//	�������ͣ����鼯�ᡢ���ɼ���ȣ�

	struct  
	{
		int		id;								//	��ʹ�õ�ͼID
		int		require_level;					//	ʹ���ߵȼ�Ҫ��
		int		require_reply_level;			//	��Ӧ�ߵȼ�Ҫ��
		int		require_reply_level2;			//	��Ӧ������Ҫ��
		int		require_reply_reincarnation_times;	//	��Ӧ��ת������Ҫ��
		int		require_reply_realm_level;
	}area[8];
	
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

////////////////////////////////////////////////////////////////////////////////////////
// ����Ԫ��
////////////////////////////////////////////////////////////////////////////////////////
struct MONSTER_SPIRIT_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				type;						//	����
	int				level;						//	�ȼ�
	int				power;						//	��������
	
	int				price;						// �����
	int				shop_price;					// ������
	int				pile_num_max;
	unsigned int	has_guid;
	unsigned int	proc_type;
};

///////////////////////////////////////////////////////////////////////////////////////
//
// �䷽ģ�����ݽṹ����
//
//
///////////////////////////////////////////////////////////////////////////////////////

// �䷽�����
struct RECIPE_MAJOR_TYPE
{
	unsigned int	id;							// �䷽�����ID
	namechar		name[32];					// �䷽���������
};

// �䷽С���
struct RECIPE_SUB_TYPE
{
	unsigned int	id;							// �䷽С���ID
	namechar		name[32];					// �䷽С�������
};

struct RECIPE_ESSENCE
{
	unsigned int	id;							// �䷽����ID
	unsigned int	id_major_type;				// �䷽�����ID
	unsigned int	id_sub_type;				// �䷽С���ID
	namechar		name[32];					// �䷽��������

	int				recipe_level;				// �䷽�ȼ�

	unsigned int	id_skill;					// ��Ҫ�ļ��ܺ�
	int				skill_level;				// ��Ҫ�ļ��ܵȼ�
	int				bind_type;					// ���˺�һ����

	struct 
	{
		unsigned int	id_to_make;				// ���ɵĶ���ID
		float			probability;			// ���ɵĸ���

	} targets[4];

	float			fail_probability;			// ����ʧ�ܸ���
	int				num_to_make;				// һ�������ĸ�����Ĭ��1
	int				price;						// һ�������ļ۸���������
	float			duration;					// һ��������ʱ�䣺�������루float��

	int				exp;						// ÿ��������õľ���
	int				skillpoint;					// ÿ��������õļ��ܵ�

	struct
	{
		unsigned int	id;						// ԭ��id
		int				num;					// ԭ��������Ŀ

	}  materials[32];
	
	unsigned int	id_upgrade_equip;			// Ҫ������װ��ID
	float			upgrade_rate;				// �̳з���ϵ��,Ĭ��Ϊ1����Χ0.01-2

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ

	unsigned int	character_combo_id;			//	ְҵ������ϣ�0Ϊ������
	float			engrave_upgrade_rate;		//	�Կ̼̳з���ϵ��,Ĭ��Ϊ1����Χ0.01-10
	float			addon_inherit_fee_rate;		//	�������Լ̳з���ϵ����0.01~2
	
	unsigned int	arena_personal_point;
	unsigned int	poker_recipe;
	
	unsigned int	camp_level_limit;

	//176+
	//int 			role_limit;
	//int				role_limit_personal_value;
	//int 			role_limit_type;
};

///////////////////////////////////////////////////////////////////////////////////////
//
// �Կ�ģ�����ݽṹ����
//
//
///////////////////////////////////////////////////////////////////////////////////////

// �Կ̴����
struct ENGRAVE_MAJOR_TYPE
{
	unsigned int	id;						// �Կ̴����ID
	namechar		name[32];				// �Կ̴��������
};

// �Կ�С���
struct ENGRAVE_SUB_TYPE
{
	unsigned int	id;						// �Կ�С���ID
	namechar		name[32];				// �Կ�С�������
};

struct ENGRAVE_ESSENCE
{
	unsigned int	id;						// �Կ̱���ID
	unsigned int	id_major_type;			// �Կ̴����ID
	unsigned int	id_sub_type;			// �Կ�С���ID
	namechar		name[32];				// �Կ̱�������
	
	char				file_icon[128];		// ͼ���ļ�
	int					level;				// �ȼ�
	
	unsigned int		equip_mask;			// װ������

	int					require_level_min;	//	����Ʒ������
	int					require_level_max;	//	����Ʒ������

	int					duration;			//	�Կ�ʱ��

	struct
	{
		unsigned int	id;					// ԭ��id
		int				num;				// ԭ��������Ŀ
	}  materials[8];
	
	float				probability_addon_num[ELEMENTDATA_MAX_ENGRAVE_ADDON_COUNT+1];	// �Կ̲���%�����ԵĻ���

	struct
	{
		unsigned int	id;					// �������Ե�����ID
		float			probability;		// �������Գ��ֵĸ���
	} addons[32];
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ����ֵ���ģ�����ݽṹ����
//
//
///////////////////////////////////////////////////////////////////////////////////////

// ����ֵ������
struct RANDPROP_TYPE
{
	unsigned int	id;								// ���ID
	namechar		name[32];					// �������
};

struct RANDPROP_ESSENCE
{
	unsigned int	id;								// ����ֵ�������ID
	unsigned int	id_type;					// ����ֵ������ID
	namechar		name[32];					// ����ֵ�����������
	
	unsigned int	id_skill;					// ��Ҫ�ļ��ܺ�
	int						skill_level;				// ��Ҫ�ļ��ܵȼ�
	
	int					money;						//	�����Ǯ
	int					duration;						//	����ʱ��

	unsigned int equip_id[32];				//	װ��ID�б�
	
	struct
	{
		unsigned int	id;						// ԭ��id
		int				num;						// ԭ��������Ŀ
	} materials[8];
};


///////////////////////////////////////////////////////////////////////////////////////
//
// ��ʯת���䷽ģ�����ݽṹ����
//
//
///////////////////////////////////////////////////////////////////////////////////////

// ��ʯת���䷽���
struct STONE_CHANGE_RECIPE_TYPE
{
	unsigned int	id;							// ���ID
	namechar		name[32];					// �������
};

// ��ʯת���䷽����
struct STONE_CHANGE_RECIPE
{
	unsigned int	id;							// ����ID
	unsigned int	id_type;					// ���ID
	namechar		name[32];					// ��������
	
	unsigned int	id_old_stone;				// ת��ǰ��ʯ
	unsigned int	id_new_stone;				// ת����ʯ
	
	int				money;						// �����Ǯ

	struct
	{
		unsigned int	id;						// ת����Ҫԭ��id
		int				num;					// ת����Ҫԭ������
	}materials[8];
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ����ģ�����ݽṹ����
//
//		ע: ����ֻ�������������С����, ����Ĺ�����������Ϊһ�������ڸ����н���ѡ��
//
//
///////////////////////////////////////////////////////////////////////////////////////

// ���︽������
struct MONSTER_ADDON
{
	unsigned int	id;							// �˸������Ե�ID����Ʒ�ĸ������Բ���һ���ռ�
												// �������Ե�ID����������ƷʱҪ���14��13λ��ʾ��
												// �������ԵĲ�����������12λ����ռ��
												// ���������ڲ���IDʱ������ʹ��12��13��14����λ

	namechar		name[32];					// �������Ե�����

	int				num_params;					// ���������ԵĲ�������
	int				param1;						// ��1�������������Ǹ�����
	int				param2;						// ��2�������������Ǹ�����
	int				param3;						// ��3�������������Ǹ�����
};

// �������
struct MONSTER_TYPE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	struct {
		unsigned int	id_addon;				// �����������, ��������ë�ȡ�˫�����׵�
		float			probability_addon;		// ����������Գ��ֵĸ���
	} addons[16];
};

// ���ﱾ������
struct MONSTER_ESSENCE
{
	unsigned int	id;							// ����(����)ID
	unsigned int	id_type;					// ���ID
	namechar		name[32];					// ����, ���15������

	namechar		prop[16];					// ��������, ���16������
	namechar		desc[16];					// ��������, ���16������

	unsigned int	faction;					// ��Ӫ��ϴ���
	unsigned int	monster_faction;			// �������ϸ��ϵ����

	char			file_model[128];			// ģ��·����

	char			file_gfx_short[128];		// ��������Ч������
	char			file_gfx_short_hit[128];	// ������������Ч������
	
	float			size;						// ��С

	float			damage_delay;				// �����չ��˺��ͺ�ʱ��

	unsigned int	id_strategy;				// ���﹥������ID
												// 0 ��ʾ�����ⲫ�����
												// 1 ��ʾ�������������
												// 2 ��ʾħ�������
												// 3 ��ʾ�ⲫ��Զ�������
												// 4 ������
												// 5 ����������
												// 6 ��׮��
												// 7 ����ħ����
												// 8 Carrier
											
	unsigned int	role_in_war;				// ��ս�������ݵĽ�ɫ
												// 0 �ޣ�1 ���Ľ�����2 ������3 ������4 Ͷʯ����5 ���͵㣻6 ����㣻7 ����NPC��8 �ɱ�������NPC

	int				level;						// ����ȼ�
	int				show_level;					// ��ʾ�ȼ�
	unsigned		id_pet_egg_captured;		// ������Ϊ�ĳ��ﵰ

	int				life;						// ����ֵ
	int				defence;					// ���	

	int				magic_defences[5];			// ��5������Ϊ�ֽ�ľˮ������ϵ

	unsigned int	immune_type;				// �������ͣ�ΪMask��Ϸ�ʽ����λ�ĺ���ӵ͵�������Ϊ�������������������߽�ϵ������ľϵ������ˮϵ
												// ���߻�ϵ��������ϵ�����߻��ԡ�����˯�ߡ����߼��١����߶��������߷�ӡ�������˺��ӱ������߻��ˡ�����HP�����½�����������HP

	int				exp;						// ����
	int				skillpoint;					// ���ܵ�
					
	int 			money_average;				// Ǯ����׼ֵ
	int				money_var;					// Ǯ������ֵ

	unsigned int	short_range_mode;			// ����������Զ�̱�־, 1-����, 0-Զ��

	int				sight_range;				// ��Ұ

	int				attack;						// ������
	int				armor;						// �ر���

	int				damage_min;					// ��������: ��С������
	int				damage_max;					// ��������: ��󹥻���
	struct {
		int				damage_min;				// ����������������ϵħ���˺�ֵ: ��С
		int				damage_max;				// ����������������ϵħ���˺�ֵ: ���
	} magic_damages_ext[5];

	float			attack_range;				// ��������: ��������
	float			attack_speed;				// ��������: �������

	int				magic_damage_min;			// ��������: ��С������
	int				magic_damage_max;			// ��������: ��󹥻���

	unsigned int	id_skill;					// ���ܺ�
	int				skill_level;				// ���ܼ���

	int				hp_regenerate;				// ��Ѫ�ٶȣ�/�룩������

	unsigned int	aggressive_mode;			// ������ѡ��0 - ������1 - ����

	unsigned int	monster_faction_ask_help;	// ����Щ��Ӫ�������ӹ�����ϸ��ϵ��ѡ��
	unsigned int	monster_faction_can_help;	// ������Щ��Ӫ�������ӹ�����ϸ��ϵ��ѡ��

	float			aggro_range;				// ��޾���
	float			aggro_time;					// ���ʱ��

	unsigned int	inhabit_type;				// ��Ϣ����𣬿��ܵ��У����桢ˮ�¡����С������ˮ�¡�����ӿ��С�ˮ�¼ӿ��С���½��
	unsigned int	patroll_mode;				// Ѳ�߷�ʽ�����ܵ��У�����߶���...
	unsigned int	stand_mode;					// վ����ʽ�����ܵ��У�����վ����˫��վ�������շ��е�

	float			walk_speed;					// �����ٶ�
	float			run_speed;					// �����ٶ�
	float			fly_speed;					// �����ٶ�
	float			swim_speed;					// ��Ӿ�ٶ�

	int				attack_degree;				// �����ȼ�
	int				defend_degree;				// �����ȼ�

	unsigned int	common_strategy;			// �������

	struct
	{
		unsigned int	id;						// ��޶Ȳ��ԣ������ǣ���ͨ��ɱ�ͼ���ɱѪ��֮һ
		float			probability;			// ��޶Ȳ��Գ��ָ���
	} aggro_strategy[4];

	// ������������
	struct 
	{
		unsigned int	id_skill;				// �����������������ID	(HP<75%ʱʹ�õļ���)
		int				level;					// ���ܵļ���
		float			probability;			// ���������ܳ��ֵĸ���
	} skill_hp75[5];

	struct
	{
		unsigned int	id_skill;				// �����������������ID	(HP<50%ʱʹ�õļ���)
		int				level;					// ���ܵļ���
		float			probability;			// ���������ܳ��ֵĸ���
	} skill_hp50[5];

	struct
	{
		unsigned int	id_skill;				// �����������������ID	(HP<25%ʱʹ�õļ���)
		int				level;					// ���ܵļ���
		float			probability;			// ���������ܳ��ֵĸ���
	} skill_hp25[5];
	
	unsigned int		after_death;			// ������������-0���Ա�-0x1������-0x2

	int				after_death_use_skill_id;
	int				after_death_use_skill_level;
	int				after_death_summon_monster_id;
	int				after_death_summon_monster_radius;
	int				after_death_summon_monster_num;
	int				unk12;
	int				unk13;
	int				unk14;
	int				unk15;
	int				unk16;
	
	// ��ͨ���������б�
	struct
	{
		unsigned int	id_skill;				// �������ͨ��������ID
		int				level;					// ���ܵļ���
	} skills[32];							
	
	float			probability_drop_num0;		// �������Ʒ����0���Ļ���
	float			probability_drop_num1;		// �������Ʒ����1���Ļ���
	float			probability_drop_num2;		// �������Ʒ����2���Ļ���
	float			probability_drop_num3;		// �������Ʒ����3���Ļ���
	int				drop_times;					// �������(1-10)
	int				drop_protected;				// ������Ʒ������

	struct {
		unsigned int	id;						// �������Ʒ
		float			probability;			// ����ĸ���
	} drop_matters[32];

	unsigned int		highest_frequency;		// ʼ�������Ƶ��ִ�в���
	int					no_accept_player_buff;
	
	int					invisible_lvl;			// �����ȼ�
	int					uninvisible_lvl;		// �����ȼ�
	
	int					no_auto_fight;			// ���Զ�����ս����0���񣬼��Զ�����ս����1���ǣ������Զ�����ս��
	int					fixed_direction;		// ����̶���0����1�����﷽���ڲ���ʱ�������Ժ��ٸĶ�
	unsigned	int		id_building;			// ��Ӧ���ɻ�����ʩ
	
	unsigned	int		combined_switch;	//	������Ͽ��أ���MONSTER_COMBINED_SWITCH��
	
	int					hp_adjust_common_value;				//	Ѫ������ȫ�ֱ���ID
	int					defence_adjust_common_value;	//	��������ȫ�ֱ���ID
	int					attack_adjust_common_value;		//	��������ȫ�ֱ���ID

	float				max_move_range;			//	���׷������
	unsigned	int		drop_for_no_profit_time;//	���䲻��������ʱ���ɫ����Ӱ�죺0���� 1����

	int					drop_mine_condition_flag;	//	�������������ǣ�ƥ����ٵ��䣩
	float				drop_mine_probability;		//	����������
	struct  
	{
		unsigned int	id;						//	����ID
		float			probability;			//	�������
		int				life;					//	����ʱ�����룩
	}drop_mines[4];

	namechar			label[16];				//	�Զ����ǩ
	unsigned int		domain_related;			//	�Ƿ�������� 0-��1-��
	int					local_var[3];			//	�ֲ�������ʼֵ
	
	
	unsigned int		carrier_id;
	unsigned int		max_received_damage_per_hit;
	unsigned int		monster_father_id;
	unsigned int		unk7;
	unsigned int		monster_father_part;
	unsigned int		unk9;
	unsigned int		faction_instance_drop_config_id;
	unsigned int		unk10;
	unsigned int		unk11;
	unsigned int		realm_level;
	unsigned int		vigour;
	
	//176+
	//int			stat_id;

};

enum	MONSTER_COMBINED_SWITCH
{
	MCS_SUMMONER_ATTACK_ONLY	= 0x00000001,			//	ֻ�����ٻ��߹���
	MCS_FORBID_SELECTION		= 0x00000002,			//	��Ҳ��ɽ���ѡ��
	MCS_HIDE_IMMUNE				= 0x00000004,			//	ͷ������ʾ����
	MCS_RECORD_DPS_RANK			= 0x00000008,			//	������¼DPS���а�
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ����ģ�����ݽṹ����
//
///////////////////////////////////////////////////////////////////////////////////////
// �������
struct PET_TYPE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������
};

// ���ﱾ������
struct PET_ESSENCE
{
	unsigned int	id;							// 锟斤拷锟斤拷(锟斤拷锟斤拷)ID
	unsigned int	id_type;					// 锟斤拷锟絀D
	namechar		name[32];					// 锟斤拷锟斤拷, 锟斤拷锟?15锟斤拷锟斤拷锟斤拷

	char			file_model[128];			// 模锟斤拷路锟斤拷
	char			file_model2[128];
	char			file_icon[128];				// 图锟斤拷锟侥硷拷
	
	unsigned int	character_combo_id;			// 职业锟斤拷锟斤拷锟斤拷锟?, 锟接碉拷位锟斤拷锟斤拷位锟侥猴拷锟斤拷为锟斤拷0-锟斤拷锟斤拷, 1-锟斤拷师, 2-锟斤拷师, 3-锟斤拷锟斤拷, 4-锟斤拷锟斤拷, 5-锟教匡拷, 6-锟斤拷芒, 7-锟斤拷锟斤拷, 8-锟斤拷锟斤拷, 9-锟斤拷锟斤拷
	
	int				level_max;					// 锟斤拷锟斤拷锟斤拷锟斤拷
	int				level_require;				// 锟饺硷拷要锟斤拷

	unsigned int	pet_snd_type;				// 锟斤拷锟斤拷挪锟斤拷锟斤拷锟斤拷锟斤拷锟?

	// 锟斤拷锟铰诧拷锟斤拷为锟诫级锟斤拷锟斤拷氐锟较碉拷锟?
	float			hp_a;						// hp系锟斤拷A
	float			hp_b;						// hp系锟斤拷B
	float			hp_c;						// hp系锟斤拷C

	float			hp_gen_a;					// hp锟街革拷系锟斤拷A
	float			hp_gen_b;					// hp锟街革拷系锟斤拷B
	float			hp_gen_c;					// hp锟街革拷系锟斤拷C

	float			damage_a;					// 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷系锟斤拷A
	float			damage_b;					// 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷系锟斤拷B
	float			damage_c;					// 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷系锟斤拷C
	float			damage_d;					// 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷系锟斤拷D

	float			speed_a;					// 锟劫讹拷系锟斤拷A
	float			speed_b;					// 锟劫讹拷系锟斤拷B

	float			attack_a;					// 锟斤拷锟叫讹拷系锟斤拷A
	float			attack_b;					// 锟斤拷锟叫讹拷系锟斤拷B
	float			attack_c;					// 锟斤拷锟叫讹拷系锟斤拷C

	float			armor_a;					// 锟斤拷锟杰讹拷系锟斤拷A
	float			armor_b;					// 锟斤拷锟杰讹拷系锟斤拷B
	float			armor_c;					// 锟斤拷锟杰讹拷系锟斤拷C

	float			physic_defence_a;			// 锟斤拷锟斤拷锟斤拷锟斤拷系锟斤拷A
	float			physic_defence_b;			// 锟斤拷锟斤拷锟斤拷锟斤拷系锟斤拷B
	float			physic_defence_c;			// 锟斤拷锟斤拷锟斤拷锟斤拷系锟斤拷C
	float			physic_defence_d;			// 锟斤拷锟斤拷锟斤拷锟斤拷系锟斤拷D

	float			magic_defence_a;			// 锟斤拷锟斤拷锟斤拷锟斤拷系锟斤拷A
	float			magic_defence_b;			// 锟斤拷锟斤拷锟斤拷锟斤拷系锟斤拷B
	float			magic_defence_c;			// 锟斤拷锟斤拷锟斤拷锟斤拷系锟斤拷C
	float			magic_defence_d;			// 锟斤拷锟斤拷锟斤拷锟斤拷系锟斤拷D
	
	float			mp_a;						// mp系锟斤拷A
	float			mp_gen_a;					// mp锟街革拷系锟斤拷A
	float			attack_degree_a;			// 锟斤拷锟斤拷锟饺硷拷系锟斤拷A
	float			defence_degree_a;			// 锟斤拷锟斤拷锟饺硷拷系锟斤拷A
	
	// 锟斤拷锟斤拷锟斤拷锟斤拷为锟教讹拷值
	float			size;						// 锟斤拷小
	float			damage_delay;				// 锟斤拷锟斤拷锟秸癸拷锟剿猴拷锟酵猴拷时锟斤拷
	float			attack_range;				// 锟斤拷锟斤拷锟斤拷锟斤拷锟侥癸拷锟斤拷锟斤拷锟斤拷
	float			attack_speed;				// 锟斤拷锟斤拷锟斤拷锟斤拷锟侥癸拷锟斤拷锟斤拷锟?
	int				sight_range;				// 锟斤拷野

	unsigned int	food_mask;					// 锟缴吃碉拷食锟斤拷Mask锟斤拷锟接低碉拷锟斤拷锟斤拷锟斤拷为锟斤拷锟较★拷锟斤拷锟洁、野锟剿★拷水锟斤拷锟斤拷锟斤拷水
	
	unsigned int	inhabit_type;				// 锟斤拷息锟斤拷锟斤拷穑?匡拷锟杰碉拷锟叫ｏ拷锟斤拷锟芥、水锟铰★拷锟斤拷锟叫★拷锟斤拷锟斤拷锟剿?锟铰★拷锟斤拷锟斤拷涌锟斤拷小锟剿?锟铰加匡拷锟叫★拷锟斤拷陆锟斤拷
	unsigned int	stand_mode;					// 站锟斤拷锟斤拷式锟斤拷锟斤拷锟杰碉拷锟叫ｏ拷锟斤拷锟斤拷站锟斤拷锟斤拷双锟斤拷站锟斤拷锟斤拷锟斤拷锟秸凤拷锟叫碉拷

	unsigned int	plant_group;				// 植锟斤拷锟斤拷锟饺猴拷锟金，匡拷锟杰碉拷锟叫ｏ拷锟斤拷通锟斤拷锟秸硷拷1锟斤拷锟秸硷拷2
	int				group_limit;				// 植锟斤拷锟斤拷锟饺猴拷锟侥?
	
	int				unk1;
	
	unsigned int	immune_type;				// 锟斤拷锟斤拷锟斤拷锟酵ｏ拷为Mask锟斤拷戏锟绞斤拷锟斤拷锟轿伙拷暮锟斤拷锟接低碉拷锟斤拷锟斤拷锟斤拷为锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟竭斤拷系锟斤拷锟斤拷锟斤拷木系锟斤拷锟斤拷锟斤拷水系
												// 锟斤拷锟竭伙拷系锟斤拷锟斤拷锟斤拷锟斤拷系锟斤拷锟斤拷锟竭伙拷锟皆★拷锟斤拷锟斤拷睡锟竭★拷锟斤拷锟竭硷拷锟劫★拷锟斤拷锟竭讹拷锟斤拷锟斤拷锟斤拷锟竭凤拷印锟斤拷锟斤拷锟斤拷锟剿猴拷锟接憋拷锟斤拷锟斤拷锟竭伙拷锟剿★拷锟斤拷锟斤拷HP锟斤拷锟斤拷锟铰斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷HP
	
	int				player_gain_skill;			// 锟斤拷锟斤拷锟斤拷锟斤拷一锟矫硷拷锟斤拷
	int				require_dye_count;			// 染色锟斤拷锟斤拷锟饺旧?锟斤拷锟斤拷目

	unsigned int    id_pet_egg_evolved;			// 锟斤拷锟斤拷锟斤拷锟斤拷锏?
	int				cost_index;					// 锟节筹拷锟斤拷锟斤拷锟斤拷锟斤拷帽锟斤拷械锟斤拷锟斤拷锟斤拷锟斤拷锟?

	int				hp_inherit_max_rate;		// 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟街碉拷坛锟较碉拷锟?(0~100)
	int				attack_inherit_max_rate;	// 锟斤拷锟斤拷锟斤拷锟斤拷蠊セ锟斤拷锟斤拷坛锟较碉拷锟?(0~100)
	int				defence_inherit_max_rate;	// 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟教筹拷系锟斤拷(0~100)
	int				attack_level_inherit_max_rate;	// 锟斤拷锟斤拷锟斤拷锟斤拷蠊セ锟斤拷燃锟斤拷坛锟较碉拷锟?(0~100)
	int				defence_level_inherit_max_rate;	// 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟饺硷拷锟教筹拷系锟斤拷(0~100)
	int				hp_inherit_min_rate;
	int				attack_inherit_min_rate;
	int				defence_inherit_min_rate;
	int				specific_skill;				//	锟斤拷锟斤拷锟斤拷专锟斤拷锟斤拷锟斤拷

	char			file_gfx_short[128];		// 锟斤拷锟斤拷效锟斤拷锟斤拷锟狡ｏ拷战锟借、锟斤拷锟斤拷锟斤拷使锟矫ｏ拷
	unsigned int	id_evolved_skill_rand;		// 锟斤拷锟斤拷锟借技锟斤拷锟斤拷锟斤拷锟斤拷帽锟?
	
	unsigned int	combined_switch;			//  锟斤拷锟斤拷锟斤拷峡锟斤拷兀锟斤拷锟? PET_COMBINED_SWITCH 锟斤拷
	

	char action[3][32];
	char cc_name[32];
	
	int id_multiride_config;
	float mult_r_hp;
	float mult_r_attack;
	float mult_r_defense;
	float unk8;
	float unk9;
	int common_strategy;
	int unk11;
	int id_pet_skin;
	char skin_name[32];

	//176+
	//char special_action4[32];
	//char special_action5[32];

};

enum	PET_COMBINED_SWITCH
{
	PCS_USE_MASTER_MODEL	= 0x00000001,		//	ʹ���ٻ���ģ��
	PCS_FORBID_SELECTION	= 0x00000002,		//	��Ҳ��ɽ���ѡ��
	PCS_HIDE_IMMUNE			= 0x00000004,		//	ͷ������ʾ����
	PCS_HIDE_NAME			= 0x00000008,		//	ͷ������ʾ����
};


///////////////////////////////////////////////////////////////////////////////////////
//
//	С�������ģ������ݽṹ����
//
///////////////////////////////////////////////////////////////////////////////////////

//	С���鱾��
struct GOBLIN_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_model1[128];			// ״̬1ģ���ļ���
	char			file_model2[128];			// ״̬2ģ���ļ���
	char			file_model3[128];			// ״̬3ģ���ļ���
	char			file_model4[128];			// ״̬4ģ���ļ���
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon1[128];			// ״̬1ͼ��·����
	char			file_icon2[128];			// ״̬2ͼ��·����
	char			file_icon3[128];			// ״̬3ͼ��·����
	char			file_icon4[128];			// ״̬4ͼ��·����

	float			exp_factor;					// ��������ֵϵ���������ﾭ��ֵ�ı�����
	int				init_strength;				// ��ʼ����
	int				init_agility;				// ��ʼ����
	int				init_energy;				// ��ʼ����
	int				init_tili;					// ��ʼ����
	int				max_refine_lvl;				// С����ľ�������

	int				price;						// �����
	int				shop_price;					// ������
	
	int				default_skill1;				// ��ʼ����1
	int				default_skill2;				// ��ʼ����2
	int				default_skill3;				// ��ʼ����3
	
	struct 
	{
		int			rand_num;					// ���������ֵ
		float		rand_rate;					// ������Ը���
	}rand_prop[10];

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

//	С����װ�����
struct GOBLIN_EQUIP_TYPE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������
};

//	С����װ��
struct GOBLIN_EQUIP_ESSENCE
{
	unsigned int	id;							// (����)ID
	unsigned int	id_type;					// ���ID
	namechar		name[32];					// ����, ���15������

	char			file_model[128];			// ģ��·��
	char			file_icon[128];				// ͼ���ļ�

	int				equip_type;					// װ�����ͣ�0-��1-����2-����3-��
	int				req_goblin_level;			// С����ȼ�Ҫ��
	int				strength;					// ����
	int				agility;					// ����
	int				energy;						// ����
	int				tili;						// ����
	int				magic[5];					// ��������
	
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

//	С���龭����
struct GOBLIN_EXPPILL_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	unsigned int	exp;						// ����ֵ
	int				level;						// �ȼ�

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

//	����ƾ֤
struct SELL_CERTIFICATE_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char            show_model[128];            // ̯λģ��·��
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	unsigned int	num_sell_item;				// ������Ʒ������
	unsigned int	num_buy_item;				// �չ���Ʒ������

	unsigned int	max_name_length;			// ��̯���Ƴ�������

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
	char            name_image[128];            // ̯λ���Ʊ���ͼƬ·��
};

//	��Ŀ��ʹ����Ʒ
struct TARGET_ITEM_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				num_use_pertime;			// ÿ��ʹ��������Ʒ����,0��ʾ�����ģ�1��ʾ����1��

	int				num_area;					// ��ʹ����������0-10
	int				area_id[10];					// ��ʹ������ID

	unsigned int	id_skill;					// ����ID
	int				skill_level;				// ���ܵȼ�

	int				use_in_combat;				// �ɷ���ս����ʹ��,1����0������
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ

	unsigned int	use_in_sanctuary_only;		// ���ڰ�ȫ��ʹ�ã�0-��1-��

	unsigned int	combined_switch;			// ������Ͽ��أ��� TARGET_ITEM_COMBINED_SWITCH ��
	int				target_id_for_pop[32];		// ѡ�д�Ŀ��ʱ������Ļ����ʾʹ�ã�Ϊ0ʱ�����ƣ�
	unsigned int	target_faction;				// ��ʹ��Ŀ����Ӫɸѡ
	int				require_level;				// �ȼ�����
};

enum	TARGET_ITEM_COMBINED_SWITCH
{
	TICS_CAN_POP			= 0x00000001,		//	����Ļ����ʾʹ��
	TICS_TARGET_MY_FACTION_OBJECT	= 0x00000002,	//	ֻ�Ա���Ŀ��ʹ��
};

//	�鿴Ŀ�����Ե���
struct LOOK_INFO_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// NPCģ�����ݽṹ����
//
//		ע: NPC���������ͬ�����ݽṹ����NPC�ܹ��ṩ�ķ�������Ϊһ�����Լ��뵽NPC�����У�
//			���������ص����ݣ��磺������ʱ����Ʒ�б��ȣ�����Ϊ��������Դ��ڷ���ģ����
//			
//		����Ŀǰ���˽⣬����һ�������¼��ࣺ
//			0��������̸���ܣ������ɻ�����̸�����������ķ�����Ҫ�����ݣ��Ի�����
//			1��������Ʒ����Ҫ�����ݣ��Ի����ݡ����ṩ����Ʒ�б�
//			2���չ���Ʒ����Ҫ�����ݣ��Ի�����
//			3��������Ʒ��װ������Ҫ�����ݣ��Ի�����
//			4�������۵�װ����Ƕ��Ʒ����Ҫ�����ݣ��Ի����ݡ������ṩ����Ʒ�б���
//			5���������Ƕ��װ���ϵ���Ʒ����Ҫ�����ݣ��Ի�����
//			6����������֤���񡢽��������������ͬ������صķ�����Ҫ�����ݣ��Ի����ݡ������ṩ�ʹ����������б�
//			7������һЩ���ܣ���Ҫ�����ݣ��Ի����ݡ����Խ��ڵļ����б�
//			8������ҽ������ƣ���Ҫ�����ݣ��Ի�����
//			9������ҽ��д��ͣ���Ҫ�����ݣ��Ի����ݣ�˲��Ŀ��㣬�����б�
//			10��Ϊ����ṩ��ͨ�������������Ҫ�����ݣ��Ի����ݣ���ͨ���߼�·�ߣ��۸�
//			11����Ʒ�Ĵ��ۣ���Ҫ�����ݣ��Ի�����
//			12��Ϊ��Ҵ洢��Ʒ����Ǯ����Ҫ�����ݣ��Ի����ݣ�������
//			13���������ֽ��ض���Ʒ����Ҫ�����ݣ��Ի����ݡ����������ֽ����Ʒ�б����������ֽ���Ʒ��ʱ�䡢��ǮӰ�����
//			14���򵼹��ܣ���Ҫ�����ݣ��ض��ȼ���Ӧ�ĶԻ������������Ӧ�ĶԻ�
//			15����ɱ����/ɱ�����
//			16��ɱ��ĳ�����������
//			17��������ɻ��أ����ɹ���NPC
//			18����·ͨ����֤�ˣ���ƱԱNPC
//
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
//
// NPC����ģ�����ݽṹ����
//
///////////////////////////////////////////////////////////////////////////////////////

// ��̸����
struct NPC_TALK_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������	

	unsigned int	id_dialog;					// �Ի�ID
};

// ������Ʒ�ķ���
struct NPC_SELL_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	struct
	{
		namechar		page_title[8];			// ҳ�����֣����7������

		int				require_contrib;		// �ۻ����׶�����

		unsigned int	require_force;		//	�������ƣ�0Ϊ���������ƣ�
		int				require_force_reputation;	//	�ۻ�������������
		int				require_camp_level;

		struct
		{
			unsigned int id;					// ��Ʒid
			int		   	contrib_cost;				// ���Ĺ��׶�
			int			force_contribution_cost;	//	��������ս��
			int			solo_tower_challenge_score_cost;	//	���ĵ�����������
			int		   	cross_server_money_cost;
		}goods[32];								// �ɳ��۵���Ʒ�б�

	} pages[8];									// ��ҳ��ʽ�洢����Ʒ�б�

	unsigned int	id_dialog;					// �Ի�ID
};

// �չ���Ʒ�ķ���
struct NPC_BUY_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������	

	unsigned int	id_dialog;					// �Ի�ID
};

// ������Ʒ�ķ���
struct NPC_REPAIR_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������	

	unsigned int	id_dialog;					// �Ի�ID
};

// ��Ƕ����
struct NPC_INSTALL_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������	

	unsigned int	id_goods[32];				// ����Ƕ����Ʒ�б�

	unsigned int	id_dialog;					// �Ի�ID
};

// �������
struct NPC_UNINSTALL_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������	

	unsigned int	id_goods[32];				// ����Ƕ����Ʒ�б�

	unsigned int	id_dialog;					// �Ի�ID
};

// �����������
struct NPC_TASK_OUT_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������	
	
	unsigned int	storage_id;					//	�������
	unsigned int	storage_open_item;			//  ������������
	unsigned int	storage_refresh_item;		//  ������ˢ�µ���
	int				storage_refresh_per_day;	//	������ÿ�տ�ˢ�´���
	int				storage_refresh_count_min;	//	������ÿ��ˢ����������Сֵ
	int				storage_refresh_count_max;	//	������ÿ��ˢ�����������ֵ
	int				storage_refresh_money;
	int				storage_deliver_per_day;	//  ������ÿ����ȡ����������,0-255

	struct
	{
		unsigned int space;
		unsigned int id;
	}tasks[256];
	//unsigned int	id_tasks[256];				// ���Դ����������б�
	unsigned int	storage_refresh_on_crossserver;	//	��������ʱ�Ƿ�ˢ��
};

// ��֤���������� 
struct NPC_TASK_IN_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������	

	struct
	{
		unsigned int space;
		int id;
	}tasks[256];
	//unsigned int	id_tasks[256];				// ���Դ����������б�
};

// ����������Ʒ����
struct NPC_TASK_MATTER_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������	

	struct
	{
		unsigned int 	space;
		unsigned int	id_task;				// ���Դ����������б�
		struct
		{
		unsigned int	id_matter;				// ���������Ʒid
		int				num_matter;				// ���������Ʒ��Ŀ
		} taks_matters[4];

	} tasks[16];
};

// ������ط���
struct NPC_SKILL_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������	

	unsigned int	id_skills[256];				// ���Դ����ļ����б�

	unsigned int	id_dialog;					// �Ի�ID
};

// ���Ʒ���
struct NPC_HEAL_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	unsigned int	id_dialog;					// �Ի�ID
};

// ���ͷ���
struct NPC_TRANSMIT_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	int				num_targets;				// Ŀ�����Ŀ
	struct {
		int			idTarget;					// Ŀ����վID
		int			fee;						// ����
		int			required_level;				// �ȼ�Ҫ��
	} targets[32];

	unsigned int	id_dialog;					// �Ի�ID
};

// �������
struct NPC_TRANSPORT_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	struct 
	{
		unsigned int	id;						// ·��ID
		unsigned int	fee;					// ����

	} routes[32];

	unsigned int	id_dialog;					// �Ի�ID
};

// ���۷���
struct NPC_PROXY_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	unsigned int	id_dialog;					// �Ի�ID
};

// �洢��Ʒ����Ǯ
struct NPC_STORAGE_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������
};

// ��������
struct NPC_MAKE_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	unsigned int	id_make_skill;				// �����������ļ���id
	int				produce_type;				// ��������, 0 - ��ͨ������1 - �ϳɣ�2 - �Ĳ�; 3 - ��������; 4 - ��Ԥ������; 5 - �߼���������

	struct
	{
		namechar		page_title[8];			// ҳ�����֣����7������
		unsigned int	id_goods[32];			// ����������Ʒ�б�

	} pages[8];									// ��ҳ��ʽ�洢����Ʒ�б�

};

// �ֽ����
struct NPC_DECOMPOSE_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	unsigned int	id_decompose_skill;			// �ɷֽ�ļ���id
};

// ��������
struct NPC_IDENTIFY_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	int				fee;						// ����
};

// ��ս�����������
struct NPC_WAR_TOWERBUILD_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	struct 
	{
		int				id_in_build;				// �����еĶ���id
		int				id_buildup;					// ������ɺ�Ķ���id
		int				id_object_need;				// ����Ҫ����Ʒ
		int				time_use;					// ���������ʱ��
		int				fee;						// ��������ķ���

	} build_info[4];
};

// ϴ�����
struct NPC_RESETPROP_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	struct 
	{
		int				id_object_need;			// ����Ҫ����Ʒ
		int				strength_delta;			// �����������仯
		int				agility_delta;			// �����������仯
		int				vital_delta;			// �����������仯
		int				energy_delta;			// ���顱�����仯

	} prop_entry[15];
};

// �����������
struct NPC_PETNAME_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	int				id_object_need;				// ����Ҫ����Ʒ
	int				price;						// ����Ҫ��Ǯ
};

// ����ѧϰ���ܷ���
struct NPC_PETLEARNSKILL_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	unsigned int	id_skills[128];				// ���Դ����ļ����б�

	unsigned int	id_dialog;					// �Ի�ID
};

// �����������ܷ���
struct NPC_PETFORGETSKILL_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	int				id_object_need;				// ����Ҫ����Ʒ
	int				price;						// ����Ҫ��Ǯ
};

// װ���󶨷���
struct NPC_EQUIPBIND_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	int				id_object_need[4];				// ����Ҫ����Ʒ
	int				price;						// ����Ҫ��Ǯ
	
	unsigned int bind_type;			//	�󶨷�ʽ(ITEM_BIND_TYPE)
};

enum	ITEM_BIND_TYPE
{
	ITEM_BIND_DEFAULT = 0x0,		//	���˺�һ��
	ITEM_BIND_WEBTRADE = 0x1,	//	Ѱ�����󶨣����˺�һ+��Ѱ�������ף�
};

// װ�����ٷ���
struct NPC_EQUIPDESTROY_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	int				id_object_need;				// ����Ҫ����Ʒ
	int				price;						// ����Ҫ��Ǯ
};

// װ��������ٷ���
struct NPC_EQUIPUNDESTROY_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	int				id_object_need;				// ����Ҫ����Ʒ
	int				price;						// ����Ҫ��Ǯ
};

// �Կ̷���
struct NPC_ENGRAVE_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	unsigned int	id_engrave[16];			//	�Կ�ģ��id
};

// ����ֵ�������
struct NPC_RANDPROP_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];			// ����, ���15������

	struct
	{
		namechar		page_title[8];		// ����
		unsigned int	id_recipe;			// �䷽ID
	} pages[8];										// ��%ҳ

};

// ��������
struct NPC_FORCE_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	unsigned int	force_id;					//	����ID
};

// �������
struct NPC_CROSS_SERVER_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	int				activity_type;				//	�����(type=cross_server_activity)
	int				player_count_limit;			//	��������
	int				time_out;					//	�ʱ��_��
	int				need_item_tid;				//	������Ʒid(type=all_type)
	int				need_item_count;			//	������Ʒ����
	unsigned int	cost_item;					//	�Ƿ���������Ʒ(type=bool)
	int				history_max_level_require;	//	��ʷ��ߵȼ�����
	int				taoist_rank_require;		//	����ȼ�Ҫ��(type=taoist_rank_require)
	int				realm_level_require;		//	����ȼ�Ҫ��
	int 			unk1;
};

// NPC���Ͷ���
struct NPC_TYPE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������	

};

///////////////////////////////////////////////////////////////////////////////////////
//
// NPC����ģ�����ݽṹ����
//
///////////////////////////////////////////////////////////////////////////////////////
struct NPC_ESSENCE
{
	unsigned int	id;							// NPC(����)ID
	namechar		name[32];					// ����, ���15������

	unsigned int	id_type;					// NPC ����, ��NPC_TYPE��ѡȡ

	float			refresh_time;				// ˢ��ʱ��

	unsigned int	attack_rule;				// �Ƿ񱻹��� 0-���ɱ�������1-�ɱ����������������2-�ɱ��������������
	
	char			file_model[128];			// ģ��·����

	float			tax_rate;					// ˰��, Ĭ��ֵ0.05
	
	unsigned int	id_src_monster;				// �����������ԵĹ���ID������ʱ��ʾʹ�øù��������滻��NPC������

	namechar		hello_msg[256];				// �����ף���һ�������ı�

	unsigned int	id_to_discover;				// ���ֺʹ��ͷ������վID

	unsigned int	domain_related;				// �Ƿ�������� 0-��1-��

	// �����б�
	unsigned int	id_talk_service;			// ��̸�ķ���ID
	unsigned int	id_sell_service;			// ������Ʒ�ķ���ID
	unsigned int	id_buy_service;				// �չ���Ʒ�ķ���ID
	unsigned int	id_repair_service;			// ������Ʒ�ķ���ID
	unsigned int	id_install_service;			// ��װ��ǶƷ�ķ���ID
	unsigned int	id_uninstall_service;		// �����ǶƷ�ķ���ID
	unsigned int	id_task_out_service;		// ������صķ���ID: �����������
	unsigned int	id_task_in_service;			// ������صķ���ID: ��֤����������
	unsigned int	id_task_matter_service;		// ������صķ���ID: ����������Ʒ����
	unsigned int	id_skill_service;			// ���ڼ��ܵķ���ID
	unsigned int	id_heal_service;			// ���Ƶķ���ID
	unsigned int	id_transmit_service;		// ���͵ķ���ID
	unsigned int	id_transport_service;		// ����ķ���ID
	unsigned int	id_proxy_service;			// ���۵ķ���ID
	unsigned int	id_storage_service;			// �ֿ�ķ���ID
	unsigned int	id_make_service;			// �����ķ���ID
	unsigned int	id_decompose_service;		// �ֽ�ķ���ID
	unsigned int	id_identify_service;		// �����ķ���ID
	unsigned int	id_war_towerbuild_service;	// ��ս��������ķ���ID
	unsigned int	id_resetprop_service;		// ϴ�����ID
	unsigned int	id_petname_service;			// �����������
	unsigned int	id_petlearnskill_service;	// ����ѧϰ���ܷ���
	unsigned int	id_petforgetskill_service;	// �����������ܷ���
	unsigned int	id_equipbind_service;		// װ���󶨷���
	unsigned int	id_equipdestroy_service;	// װ�����ٷ���
	unsigned int	id_equipundestroy_service;	// װ��������ٷ���
	unsigned int	id_goblin_skill_service;	// С���鼼��ѧϰ����
	unsigned int	id_cross_server_service;	// �������
	unsigned int	combined_services;			// �򵥷�����ϣ����е�ÿһλ����һ������Ҫ�����ķ��񣬿��ܵ��У�
												//		0�������������ܣ�1����׷���2���գ�3����վ���ַ���4�����ɷ���5�����ݷ���6���ʼķ���7����������8��˫������򿨷���
												//		9���������ﵰ����10����ԭ���ﵰ����11����ս��������12���뿪ս������13���㿨���ۣ�14��װ����������15��Ⱦɫ����16��ŤתǬ������
												//		17: С����ϴ���Ե�; 18: С����ϴ���ܵ�; 19: С������������; 20: С���龫������; 21: С�����ַ���; 22: С����װ����ж; 23: �޸����װ��
												//		24: ����ƽ̨����25: ��ħת������26: �����ط���27: ���ؿ�������28: �����������Ͻɣ�29: ���ز��϶һ�; 30:������鿴�ֿ⣻31:�鿴ɳ�����а�
	unsigned int	id_mine;					// �����Ŀ�
	unsigned int	no_collision;				//	���Դ� NPC ����ײ���
	unsigned int	id_engrave_service;			//	�Կ̷���
	unsigned int	id_randprop_service;		//	����ֵ�������
	unsigned int	combined_services2;			//	�򵥷������2: 0: �����ս 1���뿪��սս�� 2��װ��ǩ�� 3��������(����) 4������ԭ�� 5���������� 6����ʯת�����滻 7������ѡ�� 8���������� 9��ʱװ��� 10��ת������ 11����Ʒ��
												//					12��ս��ս������ 13������ת�� 14���ɽ�ǿ�� 15�����������Ӷ� 16������ר��Ǭ�����̳� 17������ר�ú����̳� 18�����Է��� 19����Ʒ��׷��� 20����������ѡ�ط���
												//					21����������(����)���а��ѯ����22���������ս��������23���������ս��ѯ���������24���������ս�鿴���а����25�������սս�����ͷ���
												//					26�������սս�������뿪����27����������(���)���а��ѯ����
	unsigned int	id_force_service;			//	��������
	unsigned int	combined_switch;			//  ������Ͽ��أ��� NPC_COMBINED_SWITCH ��
	
	unsigned int	id_arena_sell_service;
	unsigned int	id_statue_create_service;
	unsigned int	id_lib_produce_service;
	unsigned int	id_mysterious_shop_service;
	unsigned int	id_pridictable_engrave_service;
	unsigned int	combined_services3;
	unsigned int	id_faction_sell_service;
	unsigned int	faction_combined_services;
	unsigned int	id_park_enter_service;
	unsigned int	skill_id;
	
	struct
	{
		unsigned int space;
		unsigned int id;
	}tasks[20];

};

enum	NPC_COMBINED_SWITCH
{
	NCS_IGNORE_DISTANCE_CHECK	= 0x00000001,	//	�����޾�������
};

///////////////////////////////////////////////////////////////////////////////////////
//
// �����ģ�����ݽṹ����
//
///////////////////////////////////////////////////////////////////////////////////////
struct MINE_TYPE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������
};


///////////////////////////////////////////////////////////////////////////////////////
//
// ����ģ�����ݽṹ����
//
///////////////////////////////////////////////////////////////////////////////////////
struct MINE_ESSENCE
{
	unsigned int	id;							// ��(����)ID
	unsigned int	id_type;					// ���ID
	namechar		name[32];					// ����, ���15������
						  
	unsigned int	level;						// �ȼ�
	unsigned int	level_required;				// �ȼ�����
	unsigned int	id_equipment_required;		// ����Ҫ�Ĺ�������
	unsigned int	eliminate_tool;				// ���Ĳɾ��þ�(true or false)
	unsigned int	time_min;					// �ɿ�ʱ�����ޣ��룩
	unsigned int	time_max;					// �ɿ�ʱ�����ޣ��룩

	int				exp;						// ÿ�βɼ�����ľ���
	int				skillpoint;					// ÿ�βɼ�����ļ��ܵ�
	
	char			file_model[128];			// ģ��·����

	struct 
	{
		unsigned int	id;						// ԭ��ID 
		float			probability;			// �ڳ����� (�ܺ�Ϊ1)
		int				life;					// ����ʱ�����룩
	} materials[16];							// �ھ����ԭ�ϱ�

	int				num1;						// ԭ����Ŀ1
	float			probability1;				// ����1
	int				num2;						// ԭ����Ŀ2
	float			probability2;				// ����2

	unsigned int	task_in_space;
	unsigned int	task_in;					// ��Ҫ������id
	unsigned int	task_out_space;
	unsigned int	task_out;					// �ɼ����������id

	unsigned int	task_space;
	unsigned int	task;					// �ɼ����������id

	unsigned int	uninterruptable;			// �ɼ����̲����ж�, false-�����ж�, true-�����ж�
	
	unsigned int npcgen_times;
	
	struct 
	{
		unsigned int	id_monster;				// ��������id
		int				num;					// ����������Ŀ
		float			radius;					// �����뾶���ף�
		int				life_time;				//	����ʱ�䣨�룩
		float			probability;
	} npcgen[4];

	struct 
	{
		unsigned int	monster_faction;		// ��޵Ĺ�����ϸ��ϵ
		float			radius;					// ��ް뾶���ף�
		int				num;					// �����ֵ
	} aggros[1];

	unsigned int	permenent;					// �ɼ�����ﲻ��ʧ��false-��ʧ��true-����ʧ

	unsigned int	combined_switch;			// ������Ͽ��أ��� MINE_COMBINED_SWITCH ��

	int				max_gatherer;				//	ͬʱ�ɼ��������
	int				mine_type;					//	��������(0:Ĭ�ϣ�1:�������)
	float			gather_dist;				//	�ɼ�����
	float			material_gain_ratio;		//	�ɼ���ԭ�ϵĸ���

	unsigned int	gather_start_action_config;	//	�ɿ�ʼ PLAYER_ACTION_INFO_CONFIG ID 
	unsigned int	gather_loop_action_config;	//	�ɿ�ѭ�� PLAYER_ACTION_INFO_CONFIG ID
	
	namechar		caster_word[32];
	char			file_icon[128];	
	unsigned int	cursor;
	
	
	namechar		passwd[32];
	int skill;
	int skill_level;
};

enum	MINE_COMBINED_SWITCH
{
	MCS_MINE_BELONG_TO_SOMEONE	= 0x00000001,	//	�����Ƿ����ĳ���
	MCS_MINE_BROADCAST_ON_GAIN	= 0x00000002,	//	�ɼ��ɹ��Ƿ�㲥
};

// �Ի����̶���
struct talk_proc
{
	
	namechar			text[64];					// �Ի��ĵ�һ�����ڵ���ʾ���֣����63������

	struct option
	{
		unsigned int	id;							// ѡ�����ӵ��Ӵ��ڻ���ID, ������λΪ1��ʾ��һ��Ԥ����Ĺ���, Ϊ0��ʾһ���Ӵ���
		namechar		text[64];					// ѡ�����ӵ���ʾ���֣����63������
		unsigned int	param;						// ѡ����صĲ���
	};

	struct window
	{
		unsigned int	id;							// ����ID, ���λ����Ϊ1
		unsigned int	id_parent;					// ������ID, Ϊ-1��ʾ������

		int				talk_text_len;				// �Ի����ֵĳ���
		namechar * 		talk_text;					// �Ի�����

		int				num_option;					// ѡ����Ŀ
		option *		options;					// ѡ���б�

		window() { options = 0; }
		~window() { delete [] options; options = 0; delete [] talk_text; talk_text = 0; }

		int save(FILE * fp)
		{
			fwrite(&id, sizeof(id), 1, fp);
			fwrite(&id_parent, sizeof(id_parent), 1, fp);

			fwrite(&talk_text_len, sizeof(talk_text_len), 1, fp);
			fwrite(talk_text, sizeof(namechar), talk_text_len, fp);

			fwrite(&num_option, sizeof(num_option), 1, fp);
			fwrite(options, sizeof(option), num_option, fp);
			return 0;
		}

		int load(FILE * fp)
		{
			fread(&id, sizeof(id), 1, fp);
			fread(&id_parent, sizeof(id_parent), 1, fp);

			fread(&talk_text_len, sizeof(talk_text_len), 1, fp);
			//printf("FUCK_TALK: id = %d , id_parent = %d , talk_text_len = %d \n", id,id_parent,talk_text_len);
			talk_text = new namechar[talk_text_len];
			fread(talk_text, sizeof(namechar), talk_text_len, fp);

			fread(&num_option, sizeof(num_option), 1, fp);
			options = new option[num_option];
			fread(options, sizeof(option), num_option, fp);
			return 0;
		}
	};

	unsigned int		id_talk;				// �Ի������ID

	int					num_window;				// ���Ի����ֵĴ��ڸ���
	window *			windows;				// ���Ի����ֵĴ���
	
	talk_proc()
	{
		id_talk		= 0;
		num_window	= 0;
		windows		= 0;
	}

	~talk_proc()
	{
		delete [] windows;
		windows = 0;
		num_window = 0;
	}

	// ���̶���
	int save(FILE * fp)
	{
		fwrite(&id_talk, sizeof(id_talk), 1, fp);
		fwrite(text, sizeof(text), 1, fp);

		fwrite(&num_window, sizeof(num_window), 1, fp);
		for(int i=0; i<num_window; i++)
			windows[i].save(fp);
		
		return 0;
	}

	int load(FILE * fp)
	{
		fread(&id_talk, sizeof(id_talk), 1, fp);
		fread(text, sizeof(text), 1, fp);

		fread(&num_window, sizeof(num_window), 1, fp);
		windows = new window[num_window]; 
		for(int i=0; i<num_window; i++)
			windows[i].load(fp);

		return 0;
	}
};

///////////////////////////////////////////////////////////////////////////////////////
//
// �������Ի��������ݶ���ģ��
//
///////////////////////////////////////////////////////////////////////////////////////

// ĳ��λ����ͼ���÷������ݶ���
struct FACE_TEXTURE_ESSENCE
{
	unsigned int	id;						// ��ͼ����(����)ID
	namechar		name[32];				// ����, ���15������

	char			file_base_tex[128];		// �������ͼ�ļ�,���еĲ�λ��ͼ�����������
	char			file_high_tex[128];		// �ϲ��alpha����ͼ�ļ�,����Face��Nose�ⶼ���������
	char			file_icon[128];			// ͼ���ļ�

	unsigned int	tex_part_id;			// ��λID,��ʾ����ͼ���÷����������ĸ���λ��, ����:0-����1-��Ƥ��2-��Ӱ��3-ü��4-����5-�ǡ�6-���顢7-С���ӡ�8-ͷ����9-����ӡ�10-����
	unsigned int	character_combo_id;		// ְҵ�������, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����
	unsigned int	gender_id;				// ��������ͼ���õ��Ա���Ϣ,���ܵ���:0-��, 1-Ů
	unsigned int	visualize_id;			// �������Թ���, ��mask��Ϸ�ʽ�����ܵ��У��ԸС��ǸС��ɰ������ޡ����㡢���ʡ����ġ�Ұ�ԡ�Ӳ�ʡ����ա����
	unsigned int	user_data;				// �û��Զ�������
	unsigned int	facepill_only;			// ������ר��
	unsigned int    fashion_head_only;      // ʱװͷ��ר��
};

// ĳ��λ����״���ݶ���
struct FACE_SHAPE_ESSENCE
{
	unsigned int	id;						// ��״(����)ID
	namechar		name[32];				// ����, ���15������

	char			file_shape[128];		// ��״�����ļ�
	char			file_icon[128];			// ͼ���ļ�

	unsigned int	shape_part_id;			// ��λID,��ʾ����ͼ���÷����������ĸ���λ��, ����:0-���͡�1-���͡�2-ü�͡�3-��ͷ��4-������5-�ϴ��ߡ�6-������7-�´��ߡ�8-����
	unsigned int	character_combo_id;		// ְҵ�������, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����
	unsigned int	gender_id;				// ��������״���õ��Ա���Ϣ,���ܵ���:0-��, 1-Ů
	unsigned int	visualize_id;			// �������Թ���, ��mask��Ϸ�ʽ�����ܵ��У��ԸС��ǸС��ɰ������ޡ����㡢���ʡ����ġ�Ұ�ԡ�Ӳ�ʡ����ա����
	unsigned int	user_data;				// �û��Զ�������, Ŀǰ���ڶ�ͬ��λ�ĸ�����״���ݽ��н�һ���ķ���
	unsigned int	facepill_only;			// ������ר��
};

// �����ı�����������������
struct FACE_EMOTION_TYPE
{
	unsigned int	id;						// ����(����)ID
	namechar		name[32];				// ����, ���15������

	char			file_icon[128];			// ͼ���ļ�
};

// �����ı������ݶ���
struct FACE_EXPRESSION_ESSENCE
{
	unsigned int	id;						// ����(����)ID
	namechar		name[32];				// ����, ���15������

	char			file_expression[128];	// ���������ļ�
	char			file_icon[128];			// ͼ���ļ�

	unsigned int	character_combo_id;		// ְҵ�������, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����
	unsigned int	gender_id;				// �����˱������õ��Ա���Ϣ,���ܵ���:0-��, 1-Ů
	unsigned int	emotion_id;				// �˱��������������, ��Ҫ��FACE_EMOTION_TYPE��ѡȡ
};

// ��ѡͷ��ģ�����ݶ���
struct FACE_HAIR_ESSENCE
{
	unsigned int	id;						// ͷ��(����)ID
	namechar		name[32];				// ����, ���15������

	char			file_hair_skin[128];	// ͷ������ͬͷ����ӵ�Skin�ļ�
	char			file_hair_model[128];	// ͷ���Ĵ����Ӳ��ֵ�ģ���ļ�
	char			file_hair_high_poly_model[128];
	char			file_icon[128];			// ͼ��·��

	unsigned int	character_combo_id;		// ְҵ�������, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����
	unsigned int	gender_id;				// ������ͷ�����õ��Ա���Ϣ,���ܵ���:0-��, 1-Ů

	unsigned int	visualize_id;			// �������Թ���, ��mask��Ϸ�ʽ�����ܵ��У��ԸС��ǸС��ɰ������ޡ����㡢���ʡ����ġ�Ұ�ԡ�Ӳ�ʡ����ա����
	unsigned int	facepill_only;			// ������ר��
	
	unsigned int    fashion_head_only;      // ʱװͷ��ר��
};

// ��ѡ����ģ�����ݶ���
struct FACE_MOUSTACHE_ESSENCE
{
	unsigned int	id;						// ����(����)ID
	namechar		name[32];				// ����, ���15������

	char			file_moustache_skin[128];// ���Ӻ�����ӵ�Skin�ļ�
	
	char			file_icon[128];			// ͼ��·��

	unsigned int	character_combo_id;		// ְҵ�������, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����
	unsigned int	gender_id;				// �����˺������õ��Ա���Ϣ,���ܵ���:0-��, 1-Ů

	unsigned int	visualize_id;			// �������Թ���, ��mask��Ϸ�ʽ�����ܵ��У��ԸС��ǸС��ɰ������ޡ����㡢���ʡ����ġ�Ұ�ԡ�Ӳ�ʡ����ա����
	unsigned int	facepill_only;			// ������ר��
};

// ��ѡ����ģ�����ݶ���
struct FACE_FALING_ESSENCE
{
	unsigned int	id;						// ����(����)ID
	namechar		name[32];				// ����, ���15������

	char			file_faling_skin[128];	// ���������ӵ�Skin�ļ�
	
	char			file_icon[128];			// ͼ��·��

	unsigned int	character_combo_id;		// ְҵ�������, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����
	unsigned int	gender_id;				// �����˷������õ��Ա���Ϣ,���ܵ���:0-��, 1-Ů

	unsigned int	visualize_id;			// �������Թ���, ��mask��Ϸ�ʽ�����ܵ��У��ԸС��ǸС��ɰ������ޡ����㡢���ʡ����ġ�Ұ�ԡ�Ӳ�ʡ����ա����
	unsigned int	facepill_only;			// ������ר��
};

// ����
struct FACE_THIRDEYE_ESSENCE
{
	unsigned int	id;						// ����(����)ID
	namechar		name[32];				// ����, ���15������

	char			file_gfx[128];			// ��Ч�ļ�
	
	char			file_icon[128];			// ͼ��·��

	unsigned int	character_combo_id;		// ְҵ�������, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����
	unsigned int	gender_id;				// �����˷������õ��Ա���Ϣ,���ܵ���:0-��, 1-Ů

	unsigned int	facepill_only;			// ������ר��
};

// ��ɫѡ��ͼ���ļ����ݶ���
struct COLORPICKER_ESSENCE
{
	unsigned int	id;						// ��ɫѡ��ͼ��ID
	namechar		name[32];				// ���ƣ����15������

	char			file_colorpicker[128];	// ��ɫͼ�ļ���

	unsigned int	color_part_id;			// ��λID,��ʾ����ɫѡ��ͼ�������ĸ���λ��, ����:0-����1-��Ӱ��2-üë��3-�촽��4-ͷ����5-���顢6-����
	unsigned int	character_combo_id;		// ְҵ�������, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����
	unsigned int	gender_id;				// �����˺������õ��Ա���Ϣ,���ܵ���:0-��, 1-Ů
};

// ���Ի����ݶ���
struct CUSTOMIZEDATA_ESSENCE
{
	unsigned int	id;						// ���Ի�����ID
	namechar		name[32];				// ���ƣ����15������

	char			file_data[128];			// ���Ի������ļ���
	char			file_icon[128];			// ͼ���ļ�

	unsigned int	character_combo_id;		// ְҵ�������, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����
	unsigned int	gender_id;				// �����������ļ����õ��Ա���Ϣ,���ܵ���:0-��, 1-Ů
};

// ��¼���͵����Ʒ
struct FIX_POSITION_TRANSMIT_ESSENCE
{
	unsigned int	id;						// ���Ի�����ID
	namechar		name[32];				// ���ƣ����15������
	
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	int				energy;						// ����ֵ

	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// ���̻��Ķѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
};


///////////////////////////////////////////////////////////////////////////////////////
// ϵͳ�����ļ���ģ��
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// ��Ӫ�ж��б�
///////////////////////////////////////////////////////////////////////////////////////
struct ENEMY_FACTION_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	unsigned int	enemy_factions[32];		// 0 - "��ȱ"�ĵж���Ӫ�б�
											// 1 - "������ͨ"�ĵж���Ӫ�б�
											// 2 - "������ͨ"�ĵж���Ӫ�б�
											// 3 - "������ͨ"�ĵж���Ӫ�б�
											// 4 - "ϫ����ͨ"�ĵж���Ӫ�б�
											// 5 - "����BOSS"�ĵж���Ӫ�б�
											// 6 - "�����Ѻ���Ӫ"�ĵж���Ӫ�б�
											// 7 - "�����ж���Ӫ"�ĵж���Ӫ�б�

											// 8 - "������ͨ"�ĵж���Ӫ�б�
											// 9 - "����״̬"�ĵж���Ӫ�б�
											// 10 - "��ȱ"�ĵж���Ӫ�б�
											// 11 - "��ս����"�ĵж���Ӫ�б�
											// 12 - "��ս�ط�"�ĵж���Ӫ�б�
											// 13 - "��ս����"�ĵж���Ӫ�б�
											// 14 - "��ս�ط�"�ĵж���Ӫ�б�
											// 15 - "�д�״̬"�ĵж���Ӫ�б�

											// 16 - "��ȱ"�ĵж���Ӫ�б�
											// 17 - "��"�ĵж���Ӫ�б�
											// 18 - "ħ"�ĵж���Ӫ�б�
											// 19 - "��ȱ"�ĵж���Ӫ�б�
											// 20 - "��ȱ"�ĵж���Ӫ�б�
											// 21 - "��ͨNPC(���ܡ�����)"�ĵж���Ӫ�б�
											// 22 - "��ս����"�ĵж���Ӫ�б�
											// 23 - "����NPC"�ĵж���Ӫ�б�

											// 24 - "��ȱ"�ĵж���Ӫ�б�
											// 25 - "����"�ĵж���Ӫ�б�
											// 26 - "�ٻ���"�ĵж���Ӫ�б�
											// 27 - "��ȱ"�ĵж���Ӫ�б�
											// 28 - "�������"�ĵж���Ӫ�б�
											// 29 - "ħ�����"�ĵж���Ӫ�б�
											// 30 - "��ͨ����"�ĵж���Ӫ�б�
											// 31 - "��ȱ"�ĵж���Ӫ�б�
};

///////////////////////////////////////////////////////////////////////////////////////
// ְҵ�����б���ͨ�����ԣ� ����ͳһ�Ĳ���������HP��MP��������ͬ�����ɫ��ص����ݷ���
//		�����ݿ��У���Щ�����ĵ���ͨ��ר�ŵ�ͬ���ݿ������ҳ��������
///////////////////////////////////////////////////////////////////////////////////////
struct CHARRACTER_CLASS_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	unsigned int	character_class_id;		// ְҵid, ���ܵ���:0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����
	
	unsigned int	faction;				// ��ְҵ��������Ӫ
	unsigned int	enemy_faction;			// ��ְҵ�ĵж���Ӫ

	float			attack_speed;			// ����ʱ�������룩
	float			attack_range;			// ��������
	int				hp_gen;					// hp �ָ��ٶ�
	int				mp_gen;					// mp �ָ��ٶ�

	float			walk_speed;				// �����ٶ�
	float			run_speed;				// �����ٶ�
	float			swim_speed;				// ��Ӿ�ٶ�
	float			fly_speed;				// �����ٶ�
	int				crit_rate;				// �ػ��ʣ�%��
	
	int				vit_hp;					// 1��������Ӧ��hp
	int				eng_mp;					// 1��������Ӧ��mp
	int				agi_attack;				// 1�����ݶ�Ӧ������
	int				agi_armor;				// 1�����ݶ�Ӧ������

	int				lvlup_hp;				// ÿ��һ����������hp
	int				lvlup_mp;				// ÿ��һ����������mp
	float			lvlup_dmg;				// ÿ��һ��������������������
	float			lvlup_magic;			// ÿ��һ����������ħ��������
	float			lvlup_defense;			// ÿ��һ���������ķ�����
	float			lvlup_magicdefence;		// ÿ��һ����������ħ��������

	int				angro_increase;			// һ���չ����ӵ�ŭ��ֵ
	float			spirit_adjust[ELEMENTDATA_NUM_POKER_TYPE];		// ������������ϵ��
	float			poker_adjust[ELEMENTDATA_NUM_POKER_TYPE];		//	���ƻ������Ե���ϵ��
};

///////////////////////////////////////////////////////////////////////////////////////
// ����������������Ϸ�м��㹫ʽ��ĳЩ����������
///////////////////////////////////////////////////////////////////////////////////////
struct PARAM_ADJUST_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	struct
	{
		int			level_diff;				// Player�͹���ļ����(Player-����)(�������Ϊ0ʱ����ʾ����ʧЧ)
		float		adjust_exp;				// ��ɱ�ֻ�þ��������ϵ��(0.0~1.0)
		float		adjust_sp;				// ��ɱ�ֻ��SP������ϵ��(0.0~1.0)
		float		adjust_money;			// ��ɱ�ֻ�ý�Ǯ������Ŀ������ϵ��(0.0~1.0)
		float		adjust_matter;			// ��ɱ�ֵ�����Ʒ�ĸ��ʵ�����(0.0~1.0)
		float		adjust_attack;			// �Թ������ĵȼ��ͷ�ϵ��(0.0~1.0)
	} level_diff_adjust[16];				// ������ɱ�ּ�������			

	struct
	{
		float		adjust_exp;				// ��ɱ�ֻ�þ��������ϵ��(0.0~1.0)
		float		adjust_sp;				// ��ɱ�ֻ��SP������ϵ��(0.0~1.0)
	} team_adjust[11];						// ���ɱ��ʱ�ļ�������
	
	struct
	{
		float		adjust_exp;				// ��ɱ�ֻ�þ��������ϵ��(0.0~1.0)
		float		adjust_sp;				// ��ɱ�ֻ��SP������ϵ��(0.0~1.0)
	} team_profession_adjust[11];			// ������ְҵ������������

	float		dmg_adj_to_spec_atk_speed;			//	�����˺�����ϵ��
	float		atk_rate_adj_to_spec_atk_speed;	//	������������ϵ��

	float		use_monster_spirit_adjust[10];	//	����Ԫ������ϵ������������ֵȼ��
};

///////////////////////////////////////////////////////////////////////////////////////
// ����������߱�
///////////////////////////////////////////////////////////////////////////////////////
struct PLAYER_LEVELEXP_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	int				exp[150];				// 150��ÿ����һ������Ҫ�ľ���ֵ
};

///////////////////////////////////////////////////////////////////////////////////////
// �����������߱�
///////////////////////////////////////////////////////////////////////////////////////
struct ASTROLABE_LEVELEXP_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������
	
	int				exp[ELEMENTDATA_NUM_ASTROLABE_LEVEL];	// %���������辭��
};

///////////////////////////////////////////////////////////////////////////////////////
// ��������������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct ASTROLABE_ADDON_RANDOM_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ����
	int 			max_level[10];
	int				levelup_exp[ELEMENTDATA_NUM_ASTROLABE_ADDON_RANDOM_LEVEL];	// %���������辭��
	float			rand_probability[ELEMENTDATA_NUM_ASTROLABE_ADDON_RANDOM_LEVEL][ELEMENTDATA_MAX_ASTROLABE_ADDON_COUNT];	//	��������ȼ�%��������һ�����Եĸ���
};

///////////////////////////////////////////////////////////////////////////////////////
// ����������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct ASTROLABE_APPEARANCE_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������
	
	struct  
	{
		unsigned int	name_color;			//	������ɫ(type=color)
		char			file_gfx[128];		//	��Ч·��(type=path)
	}list[ELEMENTDATA_MAX_ASTROLABE_ADDON_COUNT];	//	%����������
};

///////////////////////////////////////////////////////////////////////////////////////
// װ���������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct EQUIP_MAKE_HOLE_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������
	
	struct
	{
		struct
		{
			unsigned int	require_item_id;		//	������ƷID
			int				require_item_count;		//	������Ʒ����
			int				fee;					//	����
		}hole_list[4];	//	����%
	}level_list[20];	//	Ʒ��%
};

///////////////////////////////////////////////////////////////////////////////////////
// ������������ѡ�����ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct SOLO_TOWER_CHALLENGE_LEVEL_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������
	
	struct{
		float		trans_pos[3];						//	��Ҵ���λ��%
		int			controller_id_to_activate[8];		//	�����ؿ�������ID%
	}room[6];	//	����%

	int				controller_id_to_deactivate[256];	//	����ؿ�ʱ�رյĿ�����ID%

	struct{
		unsigned int	playing_method_controller[32];	//	�淨������ID%
		int		playing_method_boss_controller;	//	BOSS������ID
	}steps[12];	//	�׶�%
};

///////////////////////////////////////////////////////////////////////////////////////
// ���������������ؽ������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct SOLO_TOWER_CHALLENGE_AWARD_PAGE_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������
	
	struct{
		unsigned int	id;					//	id
		unsigned int	count;				//	����
		float			probability;		//	��������
	}list[20];	//	������Ʒ%
};

///////////////////////////////////////////////////////////////////////////////////////
// ���������������������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct SOLO_TOWER_CHALLENGE_AWARD_LIST_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������
	
	struct{
		unsigned int	award_page_config_id;	//	���ؽ������ñ�id
		unsigned int	draw_award_times;		//	�ɿ�������
		int				award_score;			//	��������
	}level[ELEMENTDATA_NUM_SOLO_TOWER_CHALLENGE_STAGE];	//	��%��
};

///////////////////////////////////////////////////////////////////////////////////////
// �����������������������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct SOLO_TOWER_CHALLENGE_SCORE_COST_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������
	
	struct{
		unsigned int score_cost;	// ���Ļ���
		unsigned int cooldown_id;	// ��ȴID
		unsigned int cooldown_time;	// ��ȴʱ��
		float param[5];				// ״̬������
		namechar param_comment[256];		// ����ע��
	}score_buff_list[ELEMENTDATA_NUM_SOLO_TOWER_CHALLENGE_BUFF_COUNT];
};

///////////////////////////////////////////////////////////////////////////////////////
// �����ս���ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct MNFACTION_WAR_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	unsigned int sign_up_money_cost;		// �������Ľ�Ǯ 

	int resource_point;			//ս������Դ��
	int resource_tower_destroy_reduce_point;	//�ݻ���Դ���з����ٵ���Դ��
	int attacker_boss_tid;			//��������ģ��ID(type=monster_type)
	int defender_boss_tid;			//�ط�����ģ��ID(type=monster_type)

	int attacker_small_boss_tid;//����Сbossģ��ID(type==monster_type)
	int defender_small_boss_tid;//�ط�Сbossģ��ID(type==monster_type)
	int small_boss_death_reduce_point;//Сboss�������ٵ���Դ��
	int attacker_small_boss_controller_id;//��������Сboss�Ŀ�����ID
	int defender_small_boss_controller_id;//�����ط�Сboss�Ŀ�����ID

	int small_boss_appear_time;//��ս������n��Сboss����(s)
	int debuff_appear_time;//��ս������n������DEBUFF(s)
	int debuff_init_ratio;//DEBUFF��ʼ����Ч������
	int debuff_enhance_ratio_per_minute;//DEBUFFÿ�������ӵı���
	
	struct{
		int controller_id[3];		//������%ID
		int guard_controller_id;		//����������ID
		unsigned int matter_id;	//�ɼ���ģ��ID(type=mine_type)
	}attacker_resource_tower[4];		//������Դ��%
	
	struct{
		int controller_id[3];		//������%ID
		int guard_controller_id;		//����������ID
		unsigned int matter_id;	//�ɼ���ģ��ID(type=mine_type)
	}defender_resource_tower[4];	//�ط���Դ��%
	
	struct{
		int controller_id[5];		//������%ID
		unsigned int matter_id;	//�ɼ���ģ��ID(type=mine_type)
	}neutral_tower[4];			//����������%
	
	struct{
		float attacker_incoming_pos[3];				// ������������%
		float defender_incoming_pos[3];				// �ط���������%
		float attacker_transmit_pos[3];				// �������ش��͵�����%
		float defender_transmit_pos[3];				// �ط����ش��͵�����%
		struct{
			float resurrect_pos[3];					// ����%
		}resurrect_pos[2];							// �����%
		float resurrect_pos_range;                  // �����뾶
		float resource_point_range;					// ��������Դ��뾶
		struct{
			float resourse_point_pos[3];		// ���ĵ�����%
		}resourse_point_pos[2];			// ��������Դ��%	
		struct{
			int controller_id[5];		// ������%ID
			int matter_id;				// �ɼ���ģ��(type=mine_type)
			float transmit_pos[3];			// ����%
		}transmit_pos[5];				// �����ᴫ�͵�%
		int degree_total;				// ��������Դ���̶ܿ�
		int degree_per_person_second;	// ÿ��ÿ��Կ�������Դ���Ӱ��
		int gain_resource_point_per_second;		// ռ���������Դ��ÿ�����ӵ���Դ����
		int gain_resource_point_interval;		// ������Դ���ʱ����
		int reduce_resource_point_on_death;		// ÿ�������������ٵ���Դ����
	}domain;						// ս��
};

struct PLAYER_SECONDLEVEL_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	float			exp_lost[256];			// ������ʧ��
};

//	���ת�����ñ�
struct  PLAYER_REINCARNATION_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	struct  
	{
		int			require_item;			//	������Ʒ
		float		exp_premote;			//	������������
	}level[10];
};

//	��Ҿ������ñ�
struct  PLAYER_REALM_CONFIG
{
	unsigned int	id;
	namechar		name[32];
	
	struct  
	{
		struct LEVEL
		{
			int			require_exp;		//	�����˼������辳�羭��
			int			vigour;				//	�˴������Ӧ����
		}level[10];	//	��%��
	}list[10];		//	��%�ؾ���
};

///////////////////////////////////////////////////////////////////////////////////////
// �������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct FACTION_FORTRESS_CONFIG
{
	unsigned int	id;						//	id
	namechar		name[32];				//	���ƣ����15������

	int	require_level;						//	���ɵȼ�����

	struct 
	{
		int			id;						//	id (type = all_type)
		int			count;					//	����
	}require_item[8];						//	����������Ʒ%

	struct
	{
		int	exp;							//	�������辭��
		int	tech_point;						//	������ÿƼ�����
	}level[50];								//	��%��

	int	tech_point_cost[5][7];				//	�Ƽ��������ѿƼ�����

	int	init_building[20];					//	��ʼ��ʩ

	int controller_id[100];				//	����Ŀ�����%��ID

	struct
	{
		int id;										//	��ID
		int value;								//	��ֵ
	}common_value[100];				//	ȫ�ֱ���%
};


///////////////////////////////////////////////////////////////////////////////////////
// �������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct FORCE_CONFIG
{
	unsigned int	id;						//	id
	namechar		name[32];			//	���ƣ����15������

	char					file_icon[128];		//	ͼ��·����
	unsigned int	color;					//	��ʾ��ɫ
	namechar		desc[256];			//	�������֣����255������

	int					reputation_max;	//	��������
	int					contribution_max;	//	ս������

	int					join_money_cost;				//	�������Ľ�Ǯ
	int					join_item_cost;					//	����������Ʒid 

	int					quit_repution_cost;		//	�˳��ۼ������ٷֱ�
	int					quit_contribution_cost;	//	�˳��ۼ�ս���ٷֱ�
};

///////////////////////////////////////////////////////////////////////////////////////
// ��ս���ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct COUNTRY_CONFIG
{
	unsigned int		id;					//	id
	namechar			name[32];			//	���ƣ����15������
	
	struct  
	{
		namechar		name[32];			//	��������
		char			file_icon[128];		//	ͼ��·��
		unsigned int	color;				//	��ʾ��ɫ
	}country[4];
	
	int					flag_controller_id[3];	//	���Ķ�Ӧ������ID
	unsigned int		flag_mine_id;			//	���Ķ�Ӧ��ID
	
	float				attack_flag_goal[3];			//	��������Ŀ��λ��
	float				attack_flag_goal_radius;		//	��������Ŀ�귶Χ
	float				defence_flag_goal[3];			//	�ط�����Ŀ��λ��
	float				defence_flag_goal_radius;		//	�ط�����Ŀ�귶Χ

	struct  
	{
		int				controller_id;			//	������ID
		unsigned int	id;						//	����ID
		int				group;					//	��ID
	}attack_tower[9];	//	������
	
	struct  
	{
		int				controller_id;			//	������ID
		unsigned int	id;						//	����ID
		int				group;					//	��ID
	}defence_tower[9];	//	�ط���

	struct StrongHold
	{
		struct State
		{
			int				controller_id;		//	������ID
			unsigned int	mine_id;			//	��ID
		}state[5];								//	״̬
		float				pos[3];				//	λ��
		float				radius;				//	��Χ
		float				mine_pos[3];		//	��λ��
	}stronghold[8];		//	�ݵ�
};

///////////////////////////////////////////////////////////////////////////////////////
// �������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct MERIDIAN_CONFIG
{
	unsigned int	id;					//	id
	namechar		name[32];			//	���ƣ����15������
	
	struct
	{
		int			hp;					//	����
		int			phy_damage;			//	�﹥
		int			magic_damage;		//	����
		int			phy_defence;		//	���
		int			magic_defence;		//	����
	}prof_para[ELEMENTDATA_NUM_PROFESSION];			//	ְҵϵ��
};

///////////////////////////////////////////////////////////////////////////////////////
// GM��������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct GM_ACTIVITY_CONFIG
{
	unsigned int		id;					//	id
	namechar			name[32];			//	���ƣ����15������

	int					openlist[8];		//	�����������
	int					closelist[8];		//	��رտ�����
	unsigned int		disabled;			//	��GM����
};

///////////////////////////////////////////////////////////////////////////////////////
// Touch�̳�
///////////////////////////////////////////////////////////////////////////////////////
struct TOUCH_SHOP_CONFIG
{
	unsigned int	id;							//	id
	namechar		name[32];					//	����
	
	struct Page
	{
		namechar		title[8];				// ҳ�����֣����7������
		struct Item
		{
			unsigned int id;
			unsigned int num;
			unsigned int price;
			int expire_timelength;		//	����ʱ�䣨�룩
		}item[16];
	}page[8];
};

///////////////////////////////////////////////////////////////////////////////////////
// ���Ҷһ��̳�
///////////////////////////////////////////////////////////////////////////////////////
struct TOKEN_SHOP_CONFIG
{
	unsigned int	id;							//	id
	namechar		name[32];					//	����

	unsigned int	item[64];					//	�̳���Ʒ
};

///////////////////////////////////////////////////////////////////////////////////////
// ����̳�
///////////////////////////////////////////////////////////////////////////////////////
struct RAND_SHOP_CONFIG
{
	unsigned int	id;							//	id
	namechar		name[32];					//	����

	int				first_buy_price;			//	�״ι���۸�
	int				first_buy_range;			//	�״ι����������������б���1λ��ʼ��
	int				price;						//	�����۸�
	int				point;
	
	struct
	{
		unsigned int	id;						//	id
		float			probability;			//	�������		
		unsigned int	point;
	}list[256];									//	�������Ʒ�б�
};

///////////////////////////////////////////////////////////////////////////////////////
// ����ʱ������
///////////////////////////////////////////////////////////////////////////////////////
struct PROFIT_TIME_CONFIG
{
	unsigned int	id;							//	id
	namechar		name[32];					//	����
	
	int				upper_limit;				//	����ʱ�����ޣ�Сʱ��
};

///////////////////////////////////////////////////////////////////////////////////////
// �򵥳ƺ�
///////////////////////////////////////////////////////////////////////////////////////
struct TITLE_CONFIG
{
	unsigned int	id;							//	id
	namechar		name[32];					//	����
	
	namechar		desc[256];					//	������(type=single_text)
	namechar		condition[256];				//	��ȡ��������(type=single_text)
	unsigned int	color;						//	��ʾ��ɫ(type=color)
	unsigned int	after_name;					//	�Ƿ������ʾ(type=bool)

	int				phy_damage;					//	�����﹥
	int				magic_damage;				//	���ӷ���
	int				phy_defence;				//	�������
	int				magic_defence;				//	���ӷ���
	int				attack;						//	��������
	int				armor;						//	��������
	int				hp;							//	��������

	unsigned int	broadcast;					//	�Ƿ�㲥(type=bool)
	
	unsigned int	type;
	namechar		event[256];
	unsigned int	show_order;
	unsigned int	enabled;
	unsigned int	unk1;
	unsigned int	dynamic_color;
	unsigned int	switch_color_interval;
	unsigned int	dynamic_colors[9];
	
	int 			file_img_width;
	int 			file_img_height;
	int 			file_img_sprite_count;
	float 			file_img_scale;
	char 			file_img[128];

};

///////////////////////////////////////////////////////////////////////////////////////
// ���ϳƺ�
///////////////////////////////////////////////////////////////////////////////////////
struct COMPLEX_TITLE_CONFIG : public TITLE_CONFIG
{
	unsigned int	sub_titles[20];				//	�ӳƺ��б�
};

///////////////////////////////////////////////////////////////////////////////////////
// �������Ա�����Ϸ�в��Ŷ���ʱ����Ҫ����Ϣ
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// ��Ҷ������Ա�����Ϸ�в��Ŷ���ʱ����Ҫ����Ϣ
///////////////////////////////////////////////////////////////////////////////////////
struct PLAYER_ACTION_INFO_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// �������ƣ����15�����֣���ʾ�ã�

	char			action_name[32];		// �߼��������ƣ����15�����֣����߼�����һһ��Ӧ
	
	char			action_prefix[32];		// �붯�����ƶ�Ӧ�Ķ���ǰ׺
	struct 
	{
		char		suffix[32];				// ��ĳ����������״̬�µĶ�����׺

	} action_weapon_suffix[NUM_WEAPON_TYPE];		// ��˳��ֱ��ǵ��ֶ̡�˫�ֶ̡�˫�ֳ���
													// ˫�ֶ��ء�˫�ֳ��ء����ޡ�������ȭ�ס�ǹ�����֣�ذ�ף����� 
													// ֱ��ʹ������С���е�action_type�����������ɣ�
	
	unsigned int	hide_weapon;			// �Ƿ���������
};

///////////////////////////////////////////////////////////////////////////////////////
// ����������
///////////////////////////////////////////////////////////////////////////////////////
struct UPGRADE_PRODUCTION_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// �������ƣ����15�����֣���ʾ�ã�

	int				num_refine[12];			// �����ȼ���Ӧ�̳з��û���
	int				num_weapon[2][20];		// ����Ʒ�׺Ϳ�����Ӧ�̳з��û���
	int				num_armor[4][20];		// ����Ʒ�׺Ϳ�����Ӧ�̳з��û���
	int				num_stone[20];			// ��ʯ�ȼ���Ӧ�̳з��û���
	int				num_engrave[ELEMENTDATA_MAX_ENGRAVE_ADDON_COUNT];	// �Կ�������Ӧ�̳з��û���
	int				num_addon[ELEMENTDATA_MAX_INHERIT_ADDON_COUNT];		// ��������������Ӧ�̳з��û���
	int				num_decoration[4][20];	// ��ƷƷ�׺Ϳ�����Ӧ�̳з��û���
	int				bible[4][20];
};

///////////////////////////////////////////////////////////////////////////////////////
// ���ɷ����ʺŲֿ���Ʒ�б�
///////////////////////////////////////////////////////////////////////////////////////
struct ACC_STORAGE_BLACKLIST_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	unsigned int	blacklist[512];			// ���ɷ����ʺŲֿ���ƷID
};

///////////////////////////////////////////////////////////////////////////////////////
// �����ײͱ�
///////////////////////////////////////////////////////////////////////////////////////
struct MULTI_EXP_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������
	
	struct
	{
		int	multi_time;					    // ����ʱ�䣨���ӣ�
		float rate;							// ����ʱ���鱶��
		int	item_count;					    // ���Ľᵤʯ��Ŀ
		int	demulti_wait_time;				// ���黺��ʱ�䣨���ӣ�
		int	demulti_time;					// ɢ��ʱ�䣨���ӣ�
	}choice[20];
};

///////////////////////////////////////////////////////////////////////////////////////
// ͷ��ģ�ͺ�������Ӧ��
///////////////////////////////////////////////////////////////////////////////////////
struct FACE_HAIR_TEXTURE_MAP
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������
	
	struct
	{
		int	model_id;					    // ģ��ID
		int	texture_id;		     			// ����ID
	}hair_texture_map[128]; //176+ 512
};

///////////////////////////////////////////////////////////////////////////////////////
// ������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct WEDDING_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������
	
	struct
	{
		int start_hour;						// ���εĿ�ʼʱ�䣨Сʱ��
		int start_min;						// ���εĿ�ʼʱ�䣨���ӣ�
		int end_hour;						// ���εĽ���ʱ�䣨Сʱ��
		int end_min;						// ���εĽ���ʱ�䣨���ӣ�
	}wedding_session[10];					// �������ζ�Ӧ�ľ���ʱ��

	struct
	{
		int year;							// ���
		int month;							// �·�
		int day;							// ����
	}reserved_day[20];						// �����ĸ���������

	struct 
	{
		namechar	name[16];				// ���񳡾�����
		float		pos[3];					// ����ص�
	}wedding_scene[10];
};


///////////////////////////////////////////////////////////////////////////////////////
// ��ħ����ת����
///////////////////////////////////////////////////////////////////////////////////////
struct GOD_EVIL_CONVERT_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������
	
	int skill_map[128][2];					// ��ħ����ת���б�
};


///////////////////////////////////////////////////////////////////////////////////////
// �ٿƲ��ҽ��ɱ�
///////////////////////////////////////////////////////////////////////////////////////
struct WIKI_TABOO_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];		// ���ƣ����15������

	unsigned int	essence[512];				// ��ͨ��Ʒ(ID_SPACE_ESSENCE)
	unsigned int	recipe[128];				// �䷽(ID_SPACE_RECIPE)
	//unsigned int	task[512];					// ����
	struct
	{
		unsigned int space;
		unsigned int id;
	}tasks[512];
	unsigned int	skill[128];					// ����
};


///////////////////////////////////////////////////////////////////////////////////////
// ������������
///////////////////////////////////////////////////////////////////////////////////////
struct PLAYER_DEATH_DROP_CONFIG
{
	unsigned int	id;							// id
	namechar		name[32];				// ����

	unsigned int	itemlist[256];			// �������������ƷID
};

///////////////////////////////////////////////////////////////////////////////////////
// ���ѻ������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct CONSUME_POINTS_CONFIG
{
	unsigned int	id;							// id
	namechar		name[32];			// ����

	struct  
	{
		unsigned int	id;						//	��ƷID
		int					point;					//	��û���ֵ
	}list[1024];
};

///////////////////////////////////////////////////////////////////////////////////////
// ���߽������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct ONLINE_AWARDS_CONFIG
{
	unsigned int	id;							// id
	namechar		name[32];			// ����

	int					max_time;						//	ÿ����һ�ʱ�䣨�룩
	struct
	{
		int					time;						//	��ֵ���ӽ���ʱ�䣨�룩
		int					interval;					//	��ȡ����ʱ�������룩
		unsigned int	ids[8];						//	��ǰ�ײͳ�ֵ������ƷID�б�
		int					 exp[150];				//	��ǰ�ײ͹һ�ʱÿ����ջ���ֵ
	}choice[4];
};

///////////////////////////////////////////////////////////////////////////////////////
// ʱװ�������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct FASHION_WEAPON_CONFIG
{
	unsigned int	id;							//	id
	namechar		name[32];					//	����

	unsigned int	action_mask[NUM_WEAPON_TYPE];//	ʱװ������ƥ�䶯��
};

///////////////////////////////////////////////////////////////////////////////////////
// ����������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct PET_EVOLVE_CONFIG
{
	unsigned int	id;						//	id
	namechar		name[32];				//	����
	
	struct  
	{
		int			num_evolve[3][2];		//	����������Ʒ
		int			num_inherit[3][2];		//	�̳�ϵ�������������Ʒ
		int			num_rand_skill[3][2];	//	���������������Ʒ
	}cost[5];
};

///////////////////////////////////////////////////////////////////////////////////////
// �����輼�����ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct PET_EVOLVED_SKILL_CONFIG
{
	unsigned int	id;				//	id
	namechar		name[32];		//	����

	struct
	{
		int		id;					//	����ID
		int		level;				//	���ܵȼ�
	}skills[2];
};

///////////////////////////////////////////////////////////////////////////////////////
// �����輼��������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct PET_EVOLVED_SKILL_RAND_CONFIG
{
	unsigned int	id;						//	id
	namechar		name[32];				//	����
		
	struct  
	{
		float		probability;			//	����ѡ�и���
		unsigned int list[30];				//	�ȸ����Ը����б�
	}rand_skill_group[3];					//	�����輼�ܷ���
};

///////////////////////////////////////////////////////////////////////////////////////
// �Զ�������ʾ���ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct AUTOTASK_DISPLAY_CONFIG
{
	unsigned int	id;						//	id
	namechar		name[32];				//	����
	
	struct  
	{
		int			space;
		int			task_id;				//	�Զ�����ID(type=task_type)
		char		file_icon[128];			//	ͼ��·����(type=path)
		char		file_gfx[128];			//	�����Ч·����(type=path)
		unsigned int	color;				//	�ı���ɫ(type=color)
	}list[16];
};

///////////////////////////////////////////////////////////////////////////////////////
// ��������������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct PLAYER_SPIRIT_CONFIG
{
	unsigned int	id;						//	id
	namechar		name[32];				//	����

	int				type;					//	����
	
	int				hp;						// ����HP
	int				damage;					// �����﹥
	int				magic_damage;			// ��������
	int				defence;				// �������
	int				magic_defence[5];		// ��������
	int				vigour;					// ��������

	struct  
	{
		int			require_level;			//	���������ʷ��ߵȼ�
		int			require_power;			//	��������
		float		gain_ratio;				//	������Ա���
	}list[10];
};

///////////////////////////////////////////////////////////////////////////////////////
// ��ʷ�׶�
///////////////////////////////////////////////////////////////////////////////////////
struct HISTORY_STAGE_CONFIG
{
	unsigned int	id;						//	id
	namechar		name[32];				//	����
	
	namechar		desc[256];				//	���׶�����
	int				progress_value_id;		//	���ȴ洢λ��
	int				progress_value_goal;	//	�׶�Ŀ��ֵ
};

///////////////////////////////////////////////////////////////////////////////////////
// ��ʷ�ƽ����ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct HISTORY_ADVANCE_CONFIG
{
	unsigned int	id;						//	id
	namechar		name[32];				//	����
	
	int				history_stage_id[64];	//	��ʷ�׶�ID
};

///////////////////////////////////////////////////////////////////////////////////////
// �Զ�������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct AUTOTEAM_CONFIG
{
	unsigned int	id;						//	id
	namechar		name[32];				//	����

	char			file_icon[128];			//	ͼ��·����(type=path)
	namechar		desc[256];				//	����(type=text_type)
	int 			unc1;

	unsigned int	type;					//	�ճ�������������

	int				require_level[2];		//	������/��ߵȼ���0 Ϊ�����ƣ�
	int				require_maxlevel[2];	//	�����ʷ��ߵȼ���Χ��0 Ϊ�����ƣ�
	int				require_gender;			//	�Ա�0 �С�1 Ů��2 �����ƣ�
	int				require_num[2];			//	��������/�������
	int				num_prof[ELEMENTDATA_NUM_PROFESSION];	//	��ְҵ�������ã�-1 Ϊ�����ƣ�
	int				require_level2;				//	����������Ҫ��
	int				require_reincarnation_times[2];		//	����/���ת��������0Ϊ�����ƣ�
	int				require_realm_level[2];		//	���/��߾��磨0Ϊ�����ƣ�
	int				require_soul_power;			//	��С����ֵ��0Ϊ�����ƣ�

	int				worldtag;				//	����͵�ͼID
	float			trans_pos[3];			//	�����λ��
	int				worldtag_from[32];		//	�ɴ��͵�ͼ
	unsigned int	combined_switch;		//	������Ͽ��أ��� AUTOTEAM_COMBINED_SWITCH ��

	int				time_type;				//	ʱ�����ͣ�0�����ܣ�1�����գ�
	unsigned int	week;					//	���ܣ��ӵ�λ����λ����Ϊ��һ������
	int				day[2][3];				//	���գ���[2009][10][01]-[2009][10][07]����Ϊ0ʱ��ʾÿ�꣬�¡���Ϊ0ʱ����
	int				daytime[4];				//	ÿ�գ���[9][30]-[19][30]��ȫΪ0ʱ��ʾȫ��
	int 			use_daytime2;
	int				daytime2[4];
	int				award[3];				//	������Ʒ(type=all_type)
	
	struct
	{
		unsigned int space;
		unsigned int id;
	}tasks[32];	
	
	
	//unsigned int	task[32];				//	��������(type=task_type)
};

enum AUTOTEAM_COMBINED_SWITCH
{
	ACS_DISABLED	=	0x00000001,			//	�����õ�ǰ����
	ACS_RECOMMENDED	=	0x00000002,			//	�Ƽ�����
};

///////////////////////////////////////////////////////////////////////////////////////
// ս�����ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct CHARIOT_CONFIG
{
	unsigned int	id;						//	id
	namechar		name[32];				//	����

	char			file_icon[128];			//	ͼ��·����(type=path)
	namechar		desc[256];				//	����(type=text_type)

	int				level;					//	�ȼ�

	int				shape;					//	����id

	int				hp;						//	��������ֵ
	float			hp_inc_ratio;			//	�������Ӱٷֱ�
	int				defence;				//	������������
	int				magic_defences[5];		//	���ӷ�������
	int				damage;					//	������������
	int				magic_damage;			//	���ӷ�������
	float			speed;					//	�����ƶ��ٶ�

	int				skill[4];				//	���Ӽ���

	int				pre_chariot;			//	ǰ��ս��
	int				upgrade_cost;			//	������������
};


///////////////////////////////////////////////////////////////////////////////////////
// ս��ս������
///////////////////////////////////////////////////////////////////////////////////////
struct CHARIOT_WAR_CONFIG
{
	unsigned int	id;						//	id
	namechar		name[32];				//	����

	struct  
	{
		unsigned int	id;						//	ID
		int				power;					//	��������ֵ
	}mines[3];		//	������
};


///////////////////////////////////////////////////////////////////////////////////////
// �����������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct POKER_LEVELEXP_CONFIG
{
	unsigned int	id;						//	id
	namechar		name[32];				//	����
	
	int		exp[ELEMENTDATA_MAX_POKER_LEVEL];		//	��%���������辭��
	float	exp_adjust[ELEMENTDATA_NUM_POKER_RANK];	//	��%Ʒ����������ֵ����ϵ��
};


///////////////////////////////////////////////////////////////////////////////////////
// GT���ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct GT_CONFIG
{
	unsigned int	id;						//	id
	namechar		name[32];				//	����
	
	int				inc_attack_degree;		// ���ӹ����ȼ�
	int				inc_defend_degree;		// ���ӷ����ȼ�
};


///////////////////////////////////////////////////////////////////////////////////////
// �����Ӷ�ս���ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct FACTION_PVP_CONFIG
{
	unsigned int	id;						//	id
	namechar		name[32];				//	����

	struct 
	{
		int		domain_count;				//	�������������ο�ֵ
		int		minebase_gen_count;			//	�ɲ�����Դ��������
		int		points_per_gen_minecar;		//	��Դ���ز���ÿ����Դ������
		int		base_points;				//	ÿ�ܱ��׽���
		int		minecar_count_can_rob;		//	���Ӷ���Դ����
		int		minebase_count_can_rob;		//	���Ӷ���Դ������
	}list[8];

	int		controller_id[ELEMENTDATA_DOMAIN_COUNT];	//	����%������ID
};

///////////////////////////////////////////////////////////////////////////////////////
// �����б����ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct TASK_LIST_CONFIG
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������
	
	struct
	{
		unsigned int space;
		int id;
	}tasks[256];
	//unsigned int	id_tasks[256];				// �����б�
};

///////////////////////////////////////////////////////////////////////////////////////
// Ȩ��ʽ����������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct TASK_DICE_BY_WEIGHT_CONFIG
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	unsigned int storage_id;
    int max_weight;
  
  
  struct {
		float probability;
		int weight;
		unsigned int task_list_config_id;
		namechar desc[256];
		int complete_num;
		char file_icon[128];
		int gold;
		int realm_exp;
		int exp;
		int sp;
		int dividend;
	
	struct {
		int id;
		int num;
	} award[2];
  
	int detail_id;
	//176+
	//int task_id;
	
  } uniform_weight_list[20];
  
  
	unsigned int other_task_list_config_id;
	unsigned int storage_refresh_on_crossserver;
    
  struct {
		int period;
		int award;
		int num;
		int ex_award;
		int ex_num;
		
		//176+
		//int task_id;
  } period_award[10];
	
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// ʱװ��װ
/////////////////////////////////////////////////////////////////////////////////////////////////
enum	FashionSuiteItemIndex
{
	FASHION_SUITE_HEAD,
	FASHION_SUITE_UPPER_BODY,
	FASHION_SUITE_WRIST,
	FASHION_SUITE_LOWER,
	FASHION_SUITE_FOOT,
	FASHION_SUITE_WEAPON,
	FASHION_SUITE_NUM,
};

struct FASHION_SUITE_ESSENCE
{
	// ������Ϣ
	unsigned int	id;							//	��װ(����)ID
	namechar		name[32];					//	��װ����, ���15������

	char			file_icon[128];				//	ͼ������
	unsigned	int	list[FASHION_SUITE_NUM];	//	id�б�
	int				show_order;					//	չʾ˳��

	//176+
	/*
	char		tag_path[128];
	namechar	colletion_desc[32];
	int			rarity_level;
	int			use_mask;
	int			source_mask;
	int			source_npc_id;
	namechar	source_desc[32];
	int			charm_value;
	char		suit_relax_action_name[32];
	char		hang_point_name[32];
	char		pet_model_patch[128];
	char		god_baoyuan_gfx_path[128];
	char		evil_baoyuan_gfx_path[128];
	int			award_portraits_id;
	int			award_title_id;
	char		suit_single_action_name[32];
	char		unreal_folder_name[32];
	char		unreal_head_model[32];
	char		unreal_body_model[32];

	struct
	{
		char action_name[32];
	}actions[8];
	
	struct { } actions2[2][2][2];
	*/

	
};

///////////////////////////////////////////////////////////////////////////////////////
// ʱװ���ɫ���ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct FASHION_BEST_COLOR_CONFIG
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	struct  
	{
		unsigned int	id;						//	ʱװID
		unsigned int	color;					//	�����ɫ
	}list[64];
};

///////////////////////////////////////////////////////////////////////////////////////
// ǩ���������ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct SIGN_AWARD_CONFIG
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������	
	struct  
	{
		unsigned int	id;						//	������ƷID
		int				num;					//	������Ʒ����
	}list[31];									//	ǩ���ۼ�%��
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_CONFIG
{
	
	unsigned int	id;						// ID
	namechar		name[32];				// Name
	
	// List 1

	struct{
		int			need_exp;						//	Exp to Upgrade
		int			need_history_max_level;		//	Max Level
		int			reincarnation_times;		//	Reborn Level
		int			need_realmlevel;		//	Sky Level
	}upgrade[ELEMENTDATA_NUM_HOME_CONFIG]; // Count for looping

	// List 2 

	struct{
		int			energy;						//	Energy
		int			prosperous;		//	Prosperity
		int			clean;		//	Cleaning
		int			max_grain_capacity;		//	Food Max
		int			max_resource_capacity;		//	Resource Capacity Max
	}level[ELEMENTDATA_NUM_HOME_CONFIG]; // Count for looping

	// List 3

	struct{
	unsigned int	home_level;						//	Level for Up
	unsigned int	need_item_id;			//	Item for Up
		int			need_item_count;		//	Count Item for Up
	}map[ELEMENTDATA_NUM_HOME_CONFIG_MAP]; // Count for looping

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_PRODUCTS_CONFIG
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	
	

	unsigned int	produce_type; // Type
	char		file_icon[128]; // Icon

	int		is_have_surface_display; 
	int		time;

	int		building_level_min;
	int		building_level_max;

	char		file_matter_primary[128]; // Icon 2
	char		file_matter_grow_up[128]; // Icon 3
	char		file_matter_ripe[128]; // Icon 4

	int		output;
	int		exp;
	int		produce_item_times;

	// List 1

	struct{
	unsigned int	id;						//	Id do Item
		float		probability; // Probalility
		int			num;		//	Num
	}produce_items[8]; // Count for looping

	int		cost_grain_count;
	unsigned int	cost_item_id;
	int		cost_item_count;
	unsigned int	sub_type;

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_RESOURCE_PRODUCE_CONFIG
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	
	
	unsigned int	produce_type; // Type
	float		prosperous_effect_ratio; // Bonus Prosp

	struct{
		int			max_capacity;
		int			max_add_energy_time;
		float		produce_multiple;		//	Multiple
		int			upgrade_need_home_level;
		int			upgrade_consume_stone_num;
		int			upgrade_consume_wood_num;
		int			upgrade_consume_iron_num;
		int			upgrade_consume_cloth_num;
		int			upgrade_consume_grain_num;
		int			upgrade_consume_money;

	}level[16]; // Count for looping


};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_FORMULAS_PRODUCE_RECIPE
{
	
	unsigned int	id;							// ID
	unsigned int	id_major_type;							// Type
	unsigned int	id_sub_type;							// Sub Type

	namechar		name[32];					// Name	

	unsigned int	service_type;	
	unsigned int	proc_type;
	int		type;
	int		level_need;
	unsigned int	home_item_id;
	float		success_probility;
	int		count;
	int		time_use;
	int		energy_consume;
	int		home_exp_gain;
	int		produce_consume_stone_num;
	int		produce_consume_wood_num;
	int		produce_consume_iron_num;
	int		produce_consume_cloth_num;
	int		produce_consume_money;
	
	struct{
	unsigned int	home_item_id;
	unsigned int	normal_item_id;
	unsigned int	item_count;		
	}material[8]; // Count for looping

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_FORMULAS_ITEM_ESSENCE
{
	
	unsigned int	id;							// ID
	unsigned int	id_major_type;							// Type
	unsigned int	id_sub_type;							// Sub Type

	namechar		name[32];					// Name	

	char		file_matter[128]; // Icon 2
	char		file_icon[128]; // Icon 3

	int		formulas_id; // ID Recipe
	int		produce_times;
	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;
	unsigned int		proc_type;

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_PRODUCE_SERVICE_CONFIG
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	struct{
		int			formulas_id;
		int			level;	
	}produce_list[128]; // Count for looping

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_FACTORY_CONFIG
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	unsigned int	factory_type;

	struct{
		int		upgrade_consume_stone_num;
		int		upgrade_consume_wood_num;
		int		upgrade_consume_iron_num;	
		int		upgrade_consume_cloth_num;	
		int		upgrade_consume_grain_num;
		int		upgrade_consume_money;
		int		upgrade_need_home_level;
	}level[16]; // Count for looping

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_ITEM_MAJOR_TYPE
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_ITEM_SUB_TYPE
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_ITEM_ENTITY
{
	
	unsigned int	id;							// ID
	unsigned int	id_major_type;							// Type
	unsigned int	id_sub_type;							// Sub Type

	namechar		name[32];					// Name	

	int		rank;
	namechar	desc[256];
	char	file_model[128];
	char	file_icon[128];

	int		min_general_prosperous;
	int		max_general_prosperous;
	int		min_fine_prosperous;
	int		max_fine_prosperous;
	float	fine_rand_probability;
	int		clean;
	int		max_placed_num;
	int		is_permit_outside;
	int		hang_type;
	int		is_display;
	int		data_start_1;
	int		data_start_2;
	int		data_start_3;
	int		factory_config;
	int		resource_config;
	int		show_priority;
	int		forbid_equip;
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_FORMULAS_PRODUCE_MAJOR_TYPE
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_FORMULAS_PRODUCE_SUB_TYPE
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_FORMULAS_ITEM_MAJOR_TYPE
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_FORMULAS_ITEM_SUB_TYPE
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_STORAGE_TASK_CONFIG
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	int		refresh_task_count_min;
	int		refresh_task_count_max;
	int		refresh_times;
	int		refresh_cost_grain;
	int		deliver_task_max;

	struct{
	unsigned int		out_service_id;
		int		flourish_min;
		int		flourish_max;	
		int		random_weight;	
	}task_storage[16]; // Count for looping
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct WISH_TRIBUTE_ESSENCE
{
	
	unsigned int	id;							// ID

	namechar		name[32];					// Name	

	char	file_matter[128];
	char	file_icon[128];

	unsigned int	unique_id;
	int		value_add;
	unsigned int	skill_id;
	int		skill_level;
	unsigned int	item_id;
	int		item_count;
	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;
	unsigned int	proc_type;

	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct RED_PACKET_PAPER_ESSENCE
{
	
	unsigned int	id;							// ID

	namechar		name[32];					// Name	

	char	file_matter[128];
	char	file_icon[128];

	int		time_limit;
	int		money_min;
	int		money_max;
	int		cash_min;
	int		cash_max;
	int		deliver_count_min;
	int		deliver_count_max;

	unsigned int	special_item_id;

	struct
	{
		float		item_count_rand;
	}special[ELEMENTDATA_NUM_HOME_CONFIG];
	
	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;
	unsigned int	proc_type;

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162 > NEW TESOURES SYSTEM
///////////////////////////////////////////////////////////////////////////////////////

struct LOTTORY_TICKET_STORAGE_CONFIG
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	struct
	{
	unsigned int	id;
	float	probability;
	int		level;
	int		count;

	}list[100];
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162 > NEW TESOURES SYSTEM
///////////////////////////////////////////////////////////////////////////////////////

struct LOTTORY_TICKET_COST_CONFIG
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	unsigned int	require_item_id;
	int		require_item_count;
	int		cost[10];

		
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162 > NEW TESOURES SYSTEM
///////////////////////////////////////////////////////////////////////////////////////

struct LOTTORY_TICKET_OTHER_AWARD_CONFIG
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	int		award_point;
	int		require_point;
	unsigned int	controller_id;

	int		data_start[6];
	int		data_end_[6];

	struct // Loop
	{
	unsigned int	id;
	float	probability;
	int		number;
	}award[32];
		
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_TEXTURE_ENTITY
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	namechar		desc[256];

	unsigned int	type;

	char		file_tex[128];
	char		file_icon[128];
	
	float		unk1;
	float		unk2;
	
	int		is_display;
	int		data_start[3];
	int		show_priority;
	unsigned int texture_id;
		
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_GRASS_ENTITY
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	namechar		desc[256];
	char		file_model[128];
	char		file_model2[128];
	char		file_icon[128];
	
	float		unk1;
	float		unk2;
	
	int		is_display;
	int		data_start[3];
	int		show_priority;
	unsigned int grass_id;
		
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_UNLOCK_PAPER_ESSENCE
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	char		file_matter[128];
	char		file_icon[128];
		
	unsigned int	id_home_item;
	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;
	unsigned int proc_type;
		
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct GUARDIAN_BEAST_EGG_ESSENCE
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	char		file_matter[128];
	char		file_icon[128];
		
	unsigned int	id_guardian_beast;

	float		unk1;
	float		unk2;

	int		swallow_energy;
	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;
	unsigned int	proc_type;
		
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct GUARDIAN_BEAST_ENTITY
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	char		file_icon[128];
	char		file_model[128];
		
	unsigned int	guardian_beast_id;

	int		race;
	int		rank;
	int		attack;
	int		energy;
	int		unk1;
	int		max_level;
	int		unk2;

	float		unk3;
	float		unk4;
	float		probability_element_1;
	float		probability_element_2;
	float		probability_element_3;
	float		probability_element_4;
	float		probability_element_5;

	int		max_enemy;

	struct // Loop
	{

	int			race;
	float		probability;

	}enemy[32];

	unsigned int	id_cost_config;

	float		inc_resource_produce_1;
	float		inc_resource_produce_2;
	float		inc_resource_produce_3;
	float		inc_resource_produce_4;
	float		inc_resource_produce_5;
	float		inc_resource_produce_6;

	unsigned int	id_guardian_beast_egg;
	int		is_display;
	int		data_start_1;
	int		data_start_2;
	int		data_start_3;

	namechar	desc[256];
				
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct GUARDIAN_BEAST_RACE_CONFIG
{
	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	struct
	{
	char	_[32];
	}race[32];

		struct
	{
	char	_[32];
	}enemy_race[32];
			
				
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct GUARDIAN_BEAST_UPGRADE_CONFIG
{	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	struct
	{

	int		inc;
	float	cost;
	
	}level[8];

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct GUARDIAN_BEAST_REWARD_CONFIG
{	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	int		base_reward; // Main Reward 

	struct
	{

	int		cost;
	unsigned int	id;
	int		count;
	
	}reward[32];

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct HOME_SKIN_ENTITY
{	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	namechar		desc[256];
	char		file_img[128];
		
	unsigned int	skin_id;

	unsigned int	area_id;
	int		is_display;
	int		data_start[3];
	int		show_priority;

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct WORLD_SPEAK_COST_CONFIG
{	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	unsigned int	max_cost;
	int		cost[12];

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct EASY_PRODUCE_ITEM_ESSENCE
{	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	char		file_icon[128];
	char		file_matter[128];
		
	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;
	unsigned int	proc_type;
	int		duration; // Duração
	int		enable_produce_ui; // Janela de Forja
	int		produce_times; // Quantidade de usos
	int		unk1;

	struct // Make
	{
		unsigned int	id_to_make;
		int		num_to_make;
		float	probability;	
	}targets[6];

	struct  // Materials
	{
		unsigned int	id;
		int		num;	
	}materials[10];	


};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct ARENA_CONFIG
{	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

		
	int		require_level;
	int		team_create_cost;
	int		battle_match_cost;	

	int 	require_item_id;

	//176+
	//long long start_stamp;
	//long long end_stamp;
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct ARENA_SCENE_CONFIG
{	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	unsigned int	area_id;

	float		trans_pos_1_1;
	float		trans_pos_1_2;
	float		trans_pos_1_3;

	float		trans_pos_2_1;
	float		trans_pos_2_2;
	float		trans_pos_2_3;

	int		teammate_max_count;
	int		require_level;	

	int unk1;

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct ARENA_POINT_CONFIG
{	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	struct
	{
		int		unk1;
		int		points_min;
		int		points_max;
		
		struct
		{
			char	normal[128];
			char	hover[128];
		}img[2];
		

	}rank[16];
	
	
	int		unk2;	
	namechar		unk3[256];

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct ARENA_ENTITY_CONFIG
{	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	int		itemlist[64];

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct NPC_ARENA_SELL_SERVICE
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name

	struct
	{
		namechar		page_title[8];			// Count Pages
		int 			require_page_point;

		struct
		{			
			int		   	require_point;
			unsigned int	id;

		}goods[32]; // Lines Number

	} pages[8];	

	unsigned int	id_dialog;		

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct UNLOCK_RUNE_SLOT_ITEM_ESSENCE
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	char		file_icon[128];
	char		file_matter[128];
		
	int		require_level2;
	int		require_level;
	int		slot;
	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;	
	unsigned int	proc_type;

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct RUNE_ITEM_ESSENCE
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	char		file_icon[128];
	char		file_matter[128];
		
	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;
	unsigned int	proc_type;
	unsigned int	next_level_item_id;
	unsigned int	previous_level_item_id;	
	int		type;
	int		level;

	int		addon[20];
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct RUNE_SKILL_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	struct
	{
		unsigned int	skills_id_skill_base;
		unsigned int	skills_id_skill_rune[5];

	}list[50];
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct RUNE_UPGRADE_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	unsigned int	unk;

	struct
	{
		float	probability[10];

	}level[10];
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct ARENA_MAP_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	struct
	{
	unsigned int	area_id;
	int		config_id;
	}list[16];
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct NPC_STATUE_CREATE_SERVICE
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	unsigned int	id_src_npc;
	int		timelength;
	unsigned int	require_item_id;
	int		require_item_count;
	int		price;
	float		model_scale;
	int		unk2;
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct ARENA_FORBID_ITEM_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	int		items[64];
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct ARENA_FORBID_SKILL_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	int		skills[64];
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct CARRIER_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	
	namechar		desc[256];

	int slot_count;
	struct
	{
		int		unk2;
		unsigned int	monster_id;
		int		unk3;

		int		skill_id[6];
		int		skill_level[6];

	}slots[16];

	int		faction_type;
	int		owner_type;
	int		binding_limit;
	int		waiting_time;
	int		riding_time;
	int		destroy_time;
	unsigned int	range_id;
	int		attack_type;
	int		bind_action;
	int		can_bind_in_battle;
	unsigned int	required_item;
	int		is_cost_item;
	int		auto_access_channel;
	float	pos_x;
	float	pos_y;
	float	pos_z;
	
	int		shield_compulsive_attack;
	int		is_pvp_carrier;

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct CHANGE_PROPERTY_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	int		config_id[19];
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct PROFESSION_PROPERTY_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	int		param[10];
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct FIX_MONSTER_ITEM_ESSENCE
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	char		file_matter[128];
	char		file_icon[128];
		
	int		unk1;
	unsigned int	skill_id;
	int		skill_level;
	int		unk2;
	int		num_area;
	int		area_id[10];

	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;
	unsigned int	proc_type;
	int		unk4;

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct NPC_LIB_PRODUCE_SERVICE
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	int		id_goods[16];

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct LIB_PRODUCE_RECIPE
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	struct
	{
		unsigned int	id;
		int		point;

	}materials[256];

	int		produce_consume_point;
	int		produce_consume_money;
	int		duration;
	
	struct
	{
		unsigned int	id;
		int		num;
		float		probability;
		
	}targets[64];

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct VOTE_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	
	unsigned int	type;
	namechar		desc[256];

	int		choice_count;
	namechar	choice[8][32];

	int		time_limit;

	struct
	{		
		unsigned int	item_id;
		char		word[512];
		float		probability;
		int		item_count;
		int		unk1;		

	}award[2];

	int		unk2;
	int		unk3;
	int		unk4;
	int		unk5;
	int		unk6;
	int		unk7;
	namechar	unk8[256];
	int		unk9;
	int		unk10;

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct SIMULATOR_ITEM_LIST_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	int		list[500];

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct EQUIPMENT_PRODUCE_METHOD_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	unsigned int	equipment_id;
	namechar		desc[16];

	struct
	{		
		int		line;
		namechar	desc[16];

	}produce_method[32];	
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct PRODUCTION_LINE_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	int	recipe_id[10];
		
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct SOURCE_OF_MATERIAL_CONFIG
{	
	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	unsigned int	material_id;
	namechar		material_desc[256];

	struct 
	{
		int		id_space;
		unsigned int	id;
		namechar		desc[256];		
	}source_task[8];

	struct
	{
		unsigned int	id;
		namechar		desc[256];		
	}source_instance[4];

	struct
	{
		unsigned int	id;
		namechar		desc[256];		
	}source_event[5];
	
		int		source_recipe_id[8];
		namechar		source_recipe_desc[256];
		int		source_qshop;
		int		source_offline_shop;
		int		unk1;
		int		source_box;
		int		unk2;

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct FACTION_TALENT_SINGLE_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	unsigned int	type;
	namechar		desc[256];
	char		file_icon[128];

	int		param[32];

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct FACTION_TALENT_TREE_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	struct 
	{
		int		talent_id[5];
		int		faction_money_cost;
		
	}level[8];
	

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct FACTION_TARGET_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	namechar		desc[256];
	int		require_complete_times;
	int		faction_score;
	int		faction_money;
	int		auto_active;
	int		time_type;	

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct INSTANCE_STAGE_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	unsigned int	area_id;

	struct 
	{
		namechar		desc[256];
		unsigned int	target_monster;
		int		unk1;
		int		target_common_value_id1;
		int		target_common_value1;
		int		target_common_value_id2;
		int		target_common_value2;
		int		target_common_value_id3;
		int		target_common_value3;
		
	}targets[10];	
	
	int		target_id[8];
	int		common_value_id;
	int		common_value;

};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct FACTION_PET_FEED_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	

	char		ACTName[32];
  
	struct {
		unsigned int id;
		int type;
		int var;
	} makegood[15];
		

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct FACTION_TILLAGE_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	
	
	struct 
	{
		namechar		desc[32];
		int		time;
		int		controller_id;	
		
	}list[8];	

	struct 
	{				
		unsigned int	item_id;
		int		item_count;		
		int		time;

	}harvest[2];

	int		quick_ratio;
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct FACTION_PET_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	
	
	struct 
	{
		int		unk2;
		int		unk3;
		int		unk4;
		
	}unk1[3];	
	
	int		feed_config_id[2];
	char	action[32];
	int		unk7;
	int		unk8;
	int		unk9;
	int		unk10;
	int		unk11;
	int		unk12;
	int		unk13;
	int		unk14;

	struct 
	{
		int		confidence;
		int		happy;
		int		feed;	
	unsigned int	bless_config_id;	
		
	}bless[3];

	int controller_id[3];
	int	faction_target_config_id;

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct FACTION_PET_BLESS_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	
	
	char		action[32];

	struct 
	{		
		int		unk1;
		unsigned int		item_id;	
		int		item_count;	
		float		probability;
		
	}list[15];	


};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct ITEM_USED_FOR_AREA_ESSENCE
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	
	
	char		file_icon[128];
	char		file_matter[128];

	int		unk1;
	int		unk2;
	int		unk3;
	int		unk4;
	int		unk5;
	int		unk6;
	int		unk7;
	int		unk8;

	int		price;
	int		shop_price;
	int		pile_num_max;

	unsigned int	has_guid;
	unsigned int	proc_type;

	int		unk9;
	int		unk10;
	int		unk11;
	int		unk12;
	int		unk13;

	struct
	{
		unsigned int	id;
		float	pos_min[3];
		float	pos_max[3];
		
	}area[20];

	unsigned int	task_id_space;
	unsigned int	task_id;
	float		task_probability;
	


};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct CAPTURE_ITEM_ESSENCE
{	


	unsigned int	id;							// ID
	namechar		name[32];					// Name	
	
	char		file_icon[128];
	char		file_matter[128];
	char		cast_start_action[32];
	char		cast_loop_action[32];
	char		cast_end_action[32];
	char		file_pos_img[128];

	int		progress_speed;
	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;
	unsigned int	proc_type;
	int		unk1;
	int		unk2;
	unsigned int		unk3;
	unsigned int		unk4;
	
	struct
	{
		unsigned int	id;
		float	pos_min[3];
		float	pos_max[3];
		
	}area[20];

	struct {
		int length;
		int cfg_id;
	} roll_bar_infos[5]; //176+ 10
  
	struct {
		char area_icon[128];
	} area_icons[5]; //176+ 10

	struct {
		int task_id_space;
		int task_id;
	} tasks_data[1];
  
	float task_rate;
	char behavior_name[32];
	


};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct DRIFT_BOTTLE_DROP_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name	
	
	
	struct
	{
		unsigned int	item_id;
		float		probability;
		int		item_count;		
	}drop[50];


};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct NPC_FACTION_SELL_SERVICE
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name

	struct
	{
		namechar		page_title[8];			// Count Pages

		struct
		{			
			unsigned int	id;
			int		faction_money_cost;
			unsigned int	expire;

		}goods[32]; // Lines Number

	} pages[8];	

	int		id_dialog;	


};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct FACTION_INSTANCE_DROP_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name

	float		probability_drop_num0;
	float		probability_drop_num1;
	float		probability_drop_num2;
	float		probability_drop_num3;
	unsigned int	drop_times;
	unsigned int	drop_protected;

		struct
		{			
			unsigned int	id;
			float	probability;

		}drop_matters[32]; // Lines Number

		struct
		{			
			unsigned int	id;
			int	count;

		}faction_drop_matters[32]; // Lines Number

		int		faction_money;


};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct NPC_PARK_ENTER_SERVICE
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name

	int		unk1;
	int		unk2;
	int		require_player_count;
	int		unk3;
	int		unk4;

	float	pos_max_x;
	float	pos_max_z;

	float	pos_min_x;
	float	pos_min_z;

	unsigned int	require_item_id;

	int	unk5;
	int	unk6;
	int	unk7;
	int	unk8;

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct FACTION_STORAGE_WHITELIST_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name

	int		whitelist[256];	

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct NPC_BOSSRUSH_AWARD_SERVICE
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name

	// Don't have line

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct PROFESSION_STATE_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name

	unsigned int	character_class_id;
	int		id_states[50];

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct MENTAL_PARAM_ADJUST_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name

	float	unk1;

	float	unk2[10];
	float	unk3[10];	

	float	unk4;

	float	unk5[10];
	int		vigour_limit[10];

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct BIBLE_REFINE_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct
	{
		int cost;
		int trans_cost;
		float levelup_addon_rate;
		float success_probability;
	}level[12];
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct BIBLE_REFINE_TICKET_ESSENCE
{	

	unsigned int	id;							// ID
	namechar		name[32];					// Name

	char		file_matter[128];	
	char		file_icon[128];
	namechar	desc[16];
	float		ext_reserved_prob;
	float		ext_succeed_prob;
	int		fail_reserve_level;

	float		fail_ext_succeed_prob[12];
	int		binding_only;
	int		require_level_max;
	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;
	unsigned int	proc_type;
	


};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct TITLE_PROGRESS_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name
	
	struct 
	{
		unsigned int	id;
		int		id_space;
		
	}tasks[100];
	

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct NPC_MYSTERIOUS_SHOP_SERVICE
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name
	
	struct 
	{
		unsigned int	id;
		char	file_icon[128];
		float	probability;
		int		count;
		int		unk3;
		int		unk4;
		int		expire_timelength;
		int		unk6;
		int		cash;
		int		cash_rand;
		int		money;
		int		money_rand;

	}item[50];
	

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct OFFLINEBATTLE_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name
	
	int	unk1;
	int	unk2;
	int	unk3;
	int	unk4;
	int	unk5;
	int	unk6;
	int	unk7;
	
	float	unk8[3];
	float	unk9[3];
	float	unk10[3];
	float	unk11[3];

	float	unk12;
	int	unk13;
	

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct NPC_PRIDICTABLE_ENGRAVE_SERVICE
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name
	
	struct
	{
		namechar		page_title[16];
		unsigned int	id_engraves[32];

	} pages[20];	

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct NEW_ARMOR_MAJOR_TYPE
{
	unsigned int	id;							// ���ߴ����ID
	namechar		name[32];					// ���ߴ��������
};

// ����С���
struct NEW_ARMOR_SUB_TYPE
{
	unsigned int	id;							// ����С���ID
	namechar		name[32];					// ����С�������

	unsigned int	equip_mask;					// ��װ���߼��Ͽ���װ���Ĳ�λ���ɲ�ͬ��λMask��ɣ�ʹ�õĲ�λ����Ϊ��
												// EQUIP_MASK_WEAPON       = 0x0001,
												// EQUIP_MASK_HEAD         = 0x0002,
												// EQUIP_MASK_NECK         = 0x0004,
												// EQUIP_MASK_SHOULDER     = 0x0008,
												// EQUIP_MASK_BODY         = 0x0010,
												// EQUIP_MASK_WAIST        = 0x0020,
												// EQUIP_MASK_LEG          = 0x0040,
												// EQUIP_MASK_FOOT         = 0x0080,
												// EQUIP_MASK_WRIST        = 0x0100,
												// EQUIP_MASK_FINGER1      = 0x0200,
												// EQUIP_MASK_FINGER2      = 0x0400,
												// EQUIP_MASK_PROJECTILE   = 0x0800,
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct NEW_ARMOR_ESSENCE
{
	// ������Ϣ
	unsigned int	id;							// ����(����)ID
	unsigned int	id_major_type;				// ���ߴ����ID
	unsigned int	id_sub_type;				// ����С���ID
	namechar		name[32];					// ��������, ���15������

	char			realname[32];				// ��ʵװ����

	// ��ʾ��Ϣ
	// ��ģ���в��ٶ��廤�ߵ�ģ������Ϊ��Щ���߻�Ϊÿ��������һ����ͬ��ģ��,
	// ��ģ�͵����������ǹ̶���:������+װ����.ski
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	unsigned int	equip_location;				// �û���ģ�͸��ǵĲ�λ���ɲ�ͬ��λMask��ɣ����ܵĲ�λ�У�����������������Ь�ĸ�����

	int			level;						// �����ȼ�

	// װ�����
	int				require_strength;			// ��������
	int				require_agility;			// ��������
	int				require_energy;				// ��������
	int				require_tili;				// ��������

	unsigned int	character_combo_id;			// ְҵ�������, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʦ, 2-��ʦ, 3-����, 4-����, 5-�̿�, 6-��â, 7-����, 8-����, 9-����

	int				require_level;				// �ȼ�����
	int				require_reputation;			// ��������

	int				fixed_props;				// ���ɹ̶���������(0~3)

	// ��������
	int				defence_low;				// ������������Сֵ
	int				defence_high;				// �������������ֵ
	
	struct {
		int			low;						// ������������Сֵ
		int			high;						// �������������ֵ
	} magic_defences[5];						// ��5������Ϊ�ֽ�ľˮ������ϵ

	int				mp_enhance_low;				// ��MP����Сֵ
	int				mp_enhance_high;			// ��MP�����ֵ

	int				hp_enhance_low;				// ��HP����Сֵ
	int				hp_enhance_high;			// ��HP�����ֵ

	int				armor_enhance_low;			// �Ӷ����ȵ���Сֵ
	int				armor_enhance_high;			// �Ӷ����ȵ����ֵ

	int				durability_min;				// �;ö���Сֵ
	int				durability_max;				// �;ö����ֵ

	int				levelup_addon;				// ����Ч��
	int				material_need;				// ��������Ļ���ʯ��Ŀ

	// ��Ǯ
	int				price;						// �����
	int				shop_price;					// ������

	int				repairfee;					// �����۸�

	// ������
	float			drop_probability_socket0;	// ����ʱ����0���Ļ���
	float			drop_probability_socket1;	// ����ʱ����1���Ļ���
	float			drop_probability_socket2;	// ����ʱ����2���Ļ���
	float			drop_probability_socket3;	// ����ʱ����3���Ļ���
	float			drop_probability_socket4;	// ����ʱ����4���Ļ���

	// ������
	float			make_probability_socket0;	// ����ʱ����0���Ļ���
	float			make_probability_socket1;	// ����ʱ����1���Ļ���
	float			make_probability_socket2;	// ����ʱ����2���Ļ���
	float			make_probability_socket3;	// ����ʱ����3���Ļ���
	float			make_probability_socket4;	// ����ʱ����4���Ļ���
	
	// ��������
	float			probability_addon_num0;		// �������Գ���0���Ļ���
	float			probability_addon_num1;		// �������Գ���1���Ļ���
	float			probability_addon_num2;		// �������Գ���2���Ļ���
	float			probability_addon_num3;		// �������Գ���3���Ļ���
	float			probability_addon_num4;		// �������Գ���4���Ļ���
	
	struct
	{
		unsigned int	id_addon;				// �������Ե�����ID
		float			probability_addon;		// �������Գ��ֵĸ���
	} addons[32];

	struct
	{
		unsigned int	id_rand;				// ������Ե�����ID
		float			probability_rand;		// ������Գ��ֵĸ���
	} rands[32];

	int				durability_drop_min;		// ����ʱ���;ö���Сֵ
	int				durability_drop_max;		// ����ʱ���;ö���Сֵ
	
	// �����Ϣ
	int				decompose_price;			// ��ּ۸�
	int				decompose_time;				// ���ʱ��
	unsigned int	element_id;					// ��ֳɵ�Ԫʯ������ID
	int				element_num;				// ��ֲ�����Ԫʯ��
			   
	unsigned int	id_drop_after_damaged;		// ��ٺ�������Ʒ
	int				num_drop_after_damaged;		// ��ٺ�������Ʒ����
	
	unsigned int	id_hair;					// ͷ��ģ��ID������ͷ��ʱ��Ч
	unsigned int	id_hair_texture;			// ͷ������ID������ͷ��ʱ��Ч
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �û��ߵĶѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ
	
	unsigned int	force_all_magic_defences;	//	ǿ�������������з���


	// New

	int 	installed_qihun_max_level;
	int		repair_cost_item_id;
	int		repair_cost_item_num;

	char 	high_poly_realname[32];
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct QIHUN_ESSENCE
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char		file_matter[128];
	char		file_icon[128];

	unsigned int	equip_mask;	
	int		level;

	float		probability_addon_num0;
	float		probability_addon_num1;
	float		probability_addon_num2;
	float		probability_addon_num3;
	float		probability_addon_num4;
	float		probability_addon_num5;

	float		probability_moon_phase_level0;
	float		probability_moon_phase_level1;
	float		probability_moon_phase_level2;
	float		probability_moon_phase_level3;
	float		probability_moon_phase_level4;
	float		probability_moon_phase_level5;

	
	struct
	{
		unsigned int		character_combo_id;

		struct
		{			
			unsigned int		id_addon;
			float		probability_addon;

		}addons[32]; // Lines Number

	}addon_list[7];	
	
	int		id_recipe;
	
	struct
	{		
		unsigned int	id;
		int		num;
		
	}decompose_item[3];

	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;
	unsigned int	proc_type;
	

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct QILING_ESSENCE
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char		file_matter[128];
	char		file_icon[128];

	int	max_level;	

	float		require_exp[10];
	int		max_color_level[5];


	
	struct
	{
		int addon[5];

	}equipment[23];	
		
	int		addons[5];
	float	base_swallow_exp;

	char	file_icons_1[128];
	char	file_icons_2[128];
	char	file_icons_3[128];
	char	file_icons_4[128];
	char	file_icons_5[128];

	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;
	unsigned int	proc_type;
	

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct STORY_BOOK_MAJOR_TYPE
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char		file_icon[128];
		

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct STORY_BOOK_CONFIG
{	
	
	unsigned int	id;							// ID
	unsigned int	id_major_type;
	namechar		name[32];				// Name

	char		dialog[32];
	char		file_icon[128];
	char		file_gfx[128];
		
	namechar		desc[256];

	int		require_level_min;
	int		require_level_max;
	int		require_reincarnation_times;
	int		require_realm_level_min;
	int		require_realm_level_max;
	unsigned int	story_book_id;
	int		suite;
	int		show_order;
	int		condition;


	struct 
	{
		
		int		id_space;
		int		id;

	}tasks[256];
	

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct MENTOR_LEVEL_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct
	{
		int		require_point;
		int		task_id_space;
		unsigned int	task_id;
		unsigned int	item_id;
		namechar		name[16];


			
	}level[30];
		

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct STUDENT_AWARD_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct
	{
		int		item_count;
		int		unk3;
		int		unk4;
		int		unk5;
	unsigned int	item_id;

	}award[30];
		

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////

struct QIHUN_COVER_ESSENCE
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name


	char	file_matter[128];
	char	file_icon[128];

	unsigned int	item_id;
	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;
	unsigned int	proc_type;

		

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct GROWTH_TARGET_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char	file_icon[128];
	namechar	desc[256];

	int		task_id_space;
	unsigned int	task_id;
	int		check_story_book;
	int		type;
	int		category;
	int		show_order;
	int		npc_service;
	int		system_teach;
	char	dialog[128];
		

};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct QUESTION_TIP_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct 
		{			
		unsigned int	question_tip;
		int			answer_tip[4];
		}questions[300];
		
		
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct QUESTION_AWARD_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct 
		{			
		int	unk2;
		int	unk3;
		int	unk4;
		}award[10];
		
		
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct SLIDE_SKILL_ESSENCE
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char		file_matter[128];
	char		file_icon[128];

	int		unk1;
	int		unk2;

	struct
	{
		int		goal_value;
		int		skill_id;
		int		skill_level;
		char	file_fill_img[128];
		
	}sliders[5];
	
	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;
	unsigned int	proc_type;		
	
	int		num_area;
	int		area_id[10];

	//176+
	//int		target_pop;
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
////////////////////////////////////////////////////////////////////////////////////////


struct CONSUME_MONEY_CONFIG
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct
	{
		namechar	desc[256];
		int		consume;				
		
	}list[10];
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 155 > 162
///////////////////////////////////////////////////////////////////////////////////////


struct USE_FOR_SELF_ITEM_ESSENCE
{	

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char		file_matter[128];
	char		file_icon[128];

	int		unk1;
	int		num_area;

	unsigned int	area_id[10];
	
	unsigned int	skill_id;
	int		skill_level;
	int		unk2;
	int		price;
	int		shop_price;
	int		pile_num_max;
	unsigned int	has_guid;
	unsigned int	proc_type;
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct HIGH_POLY_FACE_TEXTURE_ESSENCE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name
	
	char		file_tex1[128];
	char		file_tex2[128];
	char		file_tex3[128];
	char		file_icon[128];

	int		tex_part_id;
	unsigned int		character_combo_id;
	int		gender_id;

};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct HIGH_POLY_FACE_TEMPL_CONFIG
{

	unsigned int		id;							// ID
	namechar		name[32];				// Name
	
	int		template_id[256];

};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct NEW_LOTTERY_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name
	
	float		probability_level[4];

	struct
	{
		unsigned int		item_id;	
		int		point;
		int		item_count;			
		
	}award[10];

	struct  
	{
		unsigned int		id;
		float		probability;
		int		count;
		unsigned int		rank;

	}items[5][30];
	
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct CURRENCY_ITEM_WHITELIST_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name
	
	int		items[256];	
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct HIGH_POLY_CUSTOMIZEDATA_ESSENCE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name
	
	char	file_data[128];
	char	file_icon[128];
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct HIGH_POLY_RACE_FEATURE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name
	
	char	file_icon[128];

	struct
	{
		char	file_model[128];
		char	hh_name[128];
		char	cc_name[128];
		int		unk;
		
	}list[6];
	
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct BLACK_WHITE_LIST_MAJOR_TYPE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name
	
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct BLACK_WHITE_LIST_CONFIG
{

	unsigned int	id;							// ID
	unsigned int	major_type;				// Major Type
	namechar		name[32];				// Name

	int		unk;

	int		list[512];	
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct LOANS_PRODUCE_MAJOR_TYPE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name
	
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct LOANS_PRODUCE_SUB_TYPE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	int		equip_mask;
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct LOANS_PRODUCE_RECIPE
{

	unsigned int	id;							// ID
	unsigned int	id_major_type;				// Major Type
	unsigned int	id_sub_type;				// Sub Type
	namechar		name[32];				// Name

	unsigned int	id_to_make;
	unsigned int	id_upgrade_equip;
	float		duration;

	struct
	{
		int		max_produce_times;
		unsigned int		require_cash;
		unsigned int		expire_timelength;
		float		upgrade_rate;
		float		engrave_upgrade_rate;	
		float 		addon_inherit_fee_rate;

		struct
		{
			unsigned int		id;
			int		num;

		}materials[8];


	}list[2];

	unsigned int	proc_type;
	unsigned int	character_combo_id;
	unsigned int	enabled;
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct RED_BOOK_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	int 		open_time_year;
	int 		open_time_month;
	int 		open_time_day;
	int 		open_time_hour;
	int 		open_time_minute;
	int 		open_time_second;

	int 		close_time_year;
	int 		close_time_month;
	int 		close_time_day;
	int 		close_time_hour;
	int 		close_time_minute;
	int 		close_time_second;

	int		    task_dlg_close_time_year;
	int		    task_dlg_close_time_month;
	int		    task_dlg_close_time_day;
	int		    task_dlg_close_time_hour;
	int		    task_dlg_close_time_minute;
	int		    task_dlg_close_time_second;

	int			eat_close_time_year;
	int			eat_close_time_month;
	int			eat_close_time_day;
	int			eat_close_time_hour;
	int			eat_close_time_minute;
	int			eat_close_time_second;


	struct
	{
		unsigned int		item_id;
		unsigned int		item_count;
		unsigned int		award_type;

	}level_award[500];
	
	struct
	{
		unsigned int		item_id;
		unsigned int		item_count;
		unsigned int		award_type;

	}vip_level_award[500];

	int		interval_after_500;

	int		item_id_after_500;
	int		item_count_after_500;
	int		award_type_after_500;

	int		vip_item_id_after_500;
	int		vip_item_count_after_500;
	int		vip_award_type_after_500;

};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct RED_BOOK_UPGRADE_ITEM
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char	file_matter[128];
	char	file_icon[128];

	int		require_level;
	unsigned int		swallow_exp;
	int		open_red_book;
	int		price;
	int		shop_price;	
	unsigned int		pile_num_max;

	unsigned int		has_guid;
	unsigned int		proc_type;

};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct RED_BOOK_LOTTERY_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	unsigned int		lottery_draw_lv;
	int		cost_item_id;
	int		cost_item_count;

	unsigned int		open_time;
	unsigned int		close_time;

	struct
	{
		int		award_item_id;
		int		award_item_count;
		float	award_item_probability;

	}award_pool_high[1];

	struct
	{
		unsigned int		award_item_id;
		int		award_item_count;
		float	award_prob_init;
		float	award_prob_add;
		int		award_times_max;

	}award_pool_middle[4];

	struct
	{
		unsigned int		award_item_id;
		int		award_item_count;
		float	award_item_probability;

	}award_pool_low[8];

};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct ARENA_TICKET_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	int		unk;
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct CAMP_LEVEL_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	unsigned int		camp_type;

	struct
	{
		namechar		name[256];
		int		require_scor;
		unsigned int		award_item_id;

	}level[16];
	
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct CAMP_TOKEN_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct
	{
		unsigned int		camp_token_tree_id;
		unsigned int		require_exp;

	}level[256];
	
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct CAMP_TOKEN_TREE_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct
	{
		int		parent;
		int 	first_child;
		int 	next_sibling;
		float 	probability;
		unsigned int 	addon_id;
		int	 addon_ratio;

	}node[128];
	
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct CAMP_TOKEN_ESSENCE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char	file_matter[128];
	char	file_icon[128];

	unsigned int		character_combo_id;
	int		require_level;
	unsigned int		camp_token_id;
	int		price;
	int		shop_price;
	unsigned int		pile_num_max;

	unsigned int		has_guid;
	unsigned int		proc_type;
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct CAMP_TOKEN_UPGRADE_ESSENCE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char	file_matter[128];
	char	file_icon[128];

	unsigned int		swallow_exp;
	int		price;
	int		shop_price;
	unsigned int		pile_num_max;

	unsigned int		has_guid;
	unsigned int		proc_type;
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct CAMP_TOKEN_ROLLBACK_ESSENCE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char	file_matter[128];
	char	file_icon[128];

	unsigned int		rollback_exp;
	int		price;
	int		shop_price;
	unsigned int		pile_num_max;

	unsigned int		has_guid;
	unsigned int		proc_type;
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct CAMP_TOKEN_PROB_ADJUST_ESSENCE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char	file_matter[128];
	char	file_icon[128];

	float		adjust_probability;
	int		price;
	int		shop_price;
	unsigned int		pile_num_max;

	unsigned int		has_guid;
	unsigned int		proc_type;
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct CAMP_BATTLE_TECH_TREE_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct
	{
		namechar		name[32];
		unsigned int		parent_node;
		unsigned int		common_value1;
		unsigned int		common_value2;

	}node[16];
	
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct SPECIAL_ACTIVITY_RESPAWN_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	int		unk1;	
	unsigned int		area_id;
	int		camp_type;
	unsigned int		common_value;

	float		pos[10][3];

	unsigned int		skill_id;
	int		skill_level;
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct MAP_EVENT_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	unsigned int		area_id;
	unsigned int		unk2;
	unsigned int		unk3;
	unsigned int		unk4;
	unsigned int		unk5;
	unsigned int		unk6;
	unsigned int		unk7;

	struct
	{

		unsigned int		common_value_id;
		int		common_value;
		char		unk11[32];
		int		unk12;
		
	}list[32];

	int		common_value[32];
	unsigned int		value_type;
	
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct SPECIAL_CAMP_REGION_MAJOR_TYPE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct SPECIAL_CAMP_REGION_CONFIG
{

	unsigned int	id;							// ID
	unsigned int		major_type_id;
	namechar		name[32];				// Name

	int		area_id;
	int		unk2;	
	unsigned int		common_value;
	int		camp_type;
	unsigned int		monster_id;
	int		unk14;
	int		unk15;
	int		unk6;
	int		unk7;
	int		unk8;
	int		unk9;
	char		dialog[128];
	int		unk11;
	char		sfx[128];
	unsigned int		event_hint_id;
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct PET_SKIN_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct
	{
		unsigned int		pet_id;
		int		cost;

	}list[32];
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct PET_MULTIRIDE_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	unsigned int		max_rider_count;

	struct
	{
		struct
		{
			char		hh_name[32];
			char		action[32];
		}	gender[2];
	}	list[10];
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct FASHION_NEW_MAJOR_TYPE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct FASHION_NEW_SUB_TYPE
{
	unsigned int	id;							// ʱװС���ID
	namechar		name[32];					// ʱװС�������

	unsigned int	equip_fashion_mask;			// ��װ���߼��Ͽ���װ���Ĳ�λ���ɲ�ͬ��λMask��ɣ�����߲�ͬ��ʹ�õĲ�λ����Ϊ��
												// EQUIP_MASK_FASHION_BODY = 0x00002000,
												// EQUIP_MASK_FASHION_LEG  = 0x00004000,
												// EQUIP_MASK_FASHION_FOOT = 0x00008000,
												// EQUIP_MASK_FASHION_WRIST= 0x00010000,
												// EQUIP_MASK_FASHION_HEAD = 0x02000000,
												// EQUIP_MASK_FASHION_WEAPON = 0x20000000,
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct FASHION_NEW_ESSENCE
{
	// ������Ϣ
	unsigned int	id;							// ʱװ(����)ID
	unsigned int	id_major_type;				// ʱװ�����ID
	unsigned int	id_sub_type;				// ʱװС���ID
	namechar		name[32];					// ʱװ����, ���15������

	char			realname[32];				// ��ʵʱװ��

	// ��ʾ��Ϣ
	// ��ģ���в��ٶ��廤�ߵ�ģ������Ϊ��Щ���߻�Ϊÿ��������һ����ͬ��ģ��,
	// ��ģ�͵����������ǹ̶���:������+װ����.ski
	char			file_matter[128];			// ���ڵ��ϵ�ģ��·����
	char			file_icon[128];				// ͼ��·����

	unsigned int	equip_location;				// �û���ģ�͸��ǵĲ�λ���ɲ�ͬ��λMask��ɣ����ܵĲ�λ�У�����������������Ь�ĸ�����

	int				level;						// �����ȼ�

	// װ�����
	int				require_level;				// �ȼ�����

	// Ⱦɫ
	int				require_dye_count;			// Ⱦɫ�����Ⱦɫ����Ŀ
	
	// ��Ǯ
	int				price;						// �����
	int				shop_price;					// ������

	// �Ա�
	int				gender;						// ��ʹ�õ��˵��Ա�: 0-�У�1-Ů

	unsigned int	id_hair;					// ͷ��ģ��ID������ͷ��ʱ��Ч
	unsigned int	id_hair_texture;			// ͷ������ID������ͷ��ʱ��Ч

	// �ѵ���Ϣ
	int				pile_num_max;				// �û��ߵĶѵ�����
	// GUID��Ϣ
	unsigned int	has_guid;					// �Ƿ�Ϊ�����ȫ��ΨһID��������Trace��0-��1-��
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ��װ����󶨣��Ƿ�ɽ���뿪������ʧ

	char			wear_action[32];			// ʱװ��������
	
	char			file_model_right[128];		// ʱװ��������ģ��·����
	char			file_model_left[128];		// ʱװ��������ģ��·����
	unsigned int	character_combo_id;			// ʱװ����ְҵ����
	unsigned int	action_type;				// ʱװ�������Ŷ������

	unsigned int	combined_switch;			// ������Ͽ��أ��� FASHION_COMBINED_SWITCH ��

	float			h_min;						// ɫ����Сֵ
	float			h_max;						// ɫ�����ֵ
	float			s_min;						// ���Ͷ���Сֵ
	float			s_max;						// ���Ͷ����ֵ
	float			v_min;						// ������Сֵ
	float			v_max;						// �������ֵ

	int		related_item_id;
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct WAR_AREA_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct
	{
		unsigned int	region_id;
		unsigned int	common_value;

	}list[64];
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct ILLUSTRATED_FASHION_SERIES_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	unsigned int	character_combo_id;
	int		gender;

	char	file_icon[128];
	int		award_item_count;
	int		award_require_count;
	unsigned int	award_item_id;

	unsigned int	list[256];
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct ILLUSTRATED_FASHION_ESSENCE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char	file_matter[128];
	char	file_icon[128];

	unsigned int		id_fashion_suite;

	int		price;
	int		shop_price;
	unsigned int		pile_num_max;

	unsigned int		has_guid;
	unsigned int		proc_type;

	float			h_min;						// ɫ����Сֵ
	float			h_max;						// ɫ�����ֵ
	float			s_min;						// ���Ͷ���Сֵ
	float			s_max;						// ���Ͷ����ֵ
	float			v_min;						// ������Сֵ
	float			v_max;						// �������ֵ

	int		set_color;
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct ILLUSTRATED_WING_EGG_ESSENCE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char	file_matter[128];
	char	file_icon[128];

	struct
	{
		unsigned int		id;
		int		unk1;
	}list[6];
	

	int		price;
	int		shop_price;
	unsigned int		pile_num_max;

	unsigned int		has_guid;
	unsigned int		proc_type;
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct ILLUSTRATED_WING_SERIES_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	unsigned int	race;
	char	file_icon[128];
	
	int		award_item_count;
	int		award_require_count;
	unsigned int	award_item_id;

	struct
	{
		unsigned int	id;
		int		suite;

	}list[256];
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct ILLUSTRATED_PET_EGG_ESSENCE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char	file_matter[128];
	char	file_icon[128];

	unsigned int		id_pet_egg;

	int		price;
	int		shop_price;
	unsigned int		pile_num_max;

	unsigned int		has_guid;
	unsigned int		proc_type;
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct ILLUSTRATED_PET_SERIES_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char	file_icon[128];
	
	int		award_item_count;
	int		award_require_count;
	unsigned int	award_item_id;
	int		type;

	struct
	{
		unsigned int	id;
		int		suite;

	}list[256];
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct ACROSS_SERVER_MATCH_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char 	dialog[32];

	unsigned int	common_value;
	int		area_id;
	int		require_level_min;
	int		require_level_max;
	int		unk5;
	int		unk6;
	unsigned int	require_realm_level;
	int		unk8;
	int		match_count;
	unsigned int		max_player_count;
	int		unk11;
	int		unk12;
	int		unk13;
	int		unk14;
	int		unk15;
	int		unk16;
	int		unk17;
	namechar 	desc[256];
	int		unk18;
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct NPC_ASAL_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	
	unsigned int	npc_id;
	int		across_server_matchs[50];	
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct WILDERNESS_SURVIVAL_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	int		unk1;
	int		unk2;

	float 	pos[3]; 

	unsigned int	unk6;
	unsigned int	unk7; 

	struct
	{
		unsigned int		unk9;
		unsigned int		unk10;
		float			unk11;
		float			unk12;

	}unk8[10];

	unsigned int	monster_id;

	int		unk14[10];	
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct ILLUSTRATED_REWARD_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	unsigned int		type;
	int		step;

	struct
	{
		unsigned int		id;
		char				file_icon[128];
		namechar			name[256];

	}reward[128];

		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct FAST_PRODUCE_ITEM_TYPE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct FAST_PRODUCE_ITEM_ESSENCE
{

	unsigned int	id;							// ID
	unsigned int		id_type;
	namechar		name[32];				// Name
	
	char	file_icon[128];
	char	file_matter[128];

	int		price;
	int		shop_price;
	unsigned int		pile_num_max;

	unsigned int		has_guid;
	unsigned int		proc_type;

	int		duration;
	int		enable_produce_ui;
	int		produce_times;
	int		unk1;

	struct 
	{
		unsigned int		id_to_make;
		int		num_to_make;
		float				probability;
	}targets[6];

	struct 
	{
		unsigned int		id;
		int		num;

	}materials[10];

	int 	addons[10];

	int		type;
	int		level;
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct KID_SYSTEM_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	int perception_use_update_course;
	struct 
	{
		int price[3];
	}
	course_price[5];

	int perception_use_upgrade_school;
	int exp_get_upgrade_school;
	int exp_get_daily_school;

	struct 
	{
		int school_upgrade_exp;
		int school_course_quality_prob[5];
	}
	school_data[10];
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct COURSE_ESSENCE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name
	
	int		level;
	char	file_icon[128];
	int		unk3;
	int		course_mask;
	int		score[3];
	unsigned int		cost;
	int		unk9;
	int		unk10;
	int		pile_num_max;
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct COURSE_SUITE_ESSENCE 
{
	unsigned int id;
	namechar name[32];

	int suite_mask; // Mask de Courses somados dÃ£o esse valor e ativa
	int suite_condition;

	struct 
	{
		int bonus_require_count; // Quantos Courses precisam ser aplicados para liberar o bonus
		int bonus_area_type; //??
		int bonus_trigger_probability; // Probabilidade de divisÃ£o do bonus
		int bonus_percent_add; // Quantos porcento de bonus aumenta no exp final ( o valor vem em int, mas precisa ser transformador em float, exemplo 15000 (150%))

	}bonus_info[3];
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct KID_PROPERTY_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char	file_model[128];
	int		unk1;
	char	file_img_bar[128];
	char	file_icon[128];
	char	file_icon_bar[128];
	char	file_img_panel_vertical[128];
	char	file_img_panel_horizontal[128];
	char	file_icon_panel_vertical[128];
	char	file_icon_panel_horizontal[128];

	unsigned int		desc;
	int		gender;
	int		attack_type;
	unsigned int	rahk;
	int		hp;
	unsigned int	damage;
	unsigned int	magic_damage;
	
	unsigned int	defence;
	unsigned int	magic_defence;

	float	crit_hit_probability;
	float	attack_interval;
	float	attack_dist;
	float	walk_speed;
	float	fly_speed;
	float	swim_speed;
	
	float attack_lvl_rank_param;
	float defence_lvl_rank_param;
	float anti_defence_param;
	float anti_magic_param;

	float	enchant_time_reduce;
	
	unsigned int	id_kid_skill;
	int		kid_debri_type;
	unsigned int	kid_debri_exp; //decomp_exp
	unsigned int	kid_debri_id; //fragment_id;
	unsigned int	upgrade_exp; 
	unsigned int	id_kid_upgrade;
	unsigned int	kid_upgrade_star_config;

	int		order;
	int		broadcast;
	int		star;
	int		level;
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct KID_QUALITY_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct
	{
		int		level;
		unsigned int		require_score_min;
		unsigned int		require_score_max;

		struct
		{
			struct {
				unsigned int		id;
				float		probability;
			}kid[8];
			
		}gender_list[2];
		
		
	}list[4];
	
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct KID_EXP_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	int		exp[150];					// Exp
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct KID_UPGRADE_STAR_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	float zero_star_param;

	struct
	{
		int start_exp;
		float star_param;
		
	}upgrade_star_info[6];
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct KID_LEVEL_REWARD_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	unsigned int		type;

	struct
	{
		int		require_level;
		unsigned int		addon_id;
		unsigned int		item_id;
		int		item_count;

	}reward[64];
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct KID_LEVEL_MAX_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	int		level_max[10];
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct KID_DEBRIS_ESSENCE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char	file_icon[128];
	char	file_matter[128];

	int		price;
	int		shop_price;

	unsigned int		type;
	unsigned int		swallow_exp;

	unsigned int		pile_num_max;

	unsigned int proc_type;
	int has_guid;
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct KID_SKILL_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct
	{
		int		id;
		int		level[10];
	
	}skill[16];
	
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct KID_DEBRIS_GENERATOR_ESSENCE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char	file_icon[128];
	char	file_matter[128];

	struct
	{
		unsigned int		id;
		float		probability;

	}list[256];


	int		price;
	int		shop_price;

	unsigned int		pile_num_max;

	unsigned int		proc_type;
	unsigned int		has_guid;
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct KID_PROFESSION_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	int		type;
	char	file_gfx1[128];
	char	file_gfx2[128];

	struct
	{
		char	file_sfx1[128];
		char	file_sfx2[128];
		char	file_sfx3[128];
		char	file_sfx4[128];
	
	}list1[6];
	
	struct
	{
		char	file_sfx1[128];
		char	file_sfx2[128];
		char	file_sfx3[128];
		char	file_sfx4[128];
		char	file_sfx5[128];
		char	file_sfx6[128];
	
	}list2[6];
	
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct BIS_SOI_USE_LIMIT_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	unsigned int		area_id;
	int		unk2;
	int		unk3;
	int		unk4;
	int		unk5;


	struct
	{
		int		type;	
		unsigned int		id;

	}list[64];
	
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct REWARD_INTERFACE_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	int		time_mode;
	UINT64		start_time;
	UINT64		online_end_time;
	UINT64		end_time;

	int		reward_cycle;
	int		reward_times;
	int		value1_index;
	int		value2_index;
	int		value_open_key;
	int		value_open_min_value;


	struct
	{
		unsigned int		item_id;
		int		item_count;
		int		item_expire_date;
		unsigned int		item_cooldown;
				
	}reward[30];
	
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct HISTORY_VARIABLE_PROGRESS_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	unsigned int		history_value_id;
	UINT64		data_start;
	UINT64		data_end;

	struct
	{
		unsigned int		history_value_goal;
		namechar		name[32];
		namechar		desc[256];		
				
	}stage[50];
	
	int		unk10;
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct ANNIVERSARY_EVENT_INTERFACE_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	unsigned int		start_task_id_space;
	unsigned int		start_task_id;
	UINT64		data_start;
	UINT64		data_start_max;
	UINT64		data_end;

    int		unk9;
	unsigned int		task_id_space;
	unsigned int		task_id;

	int		unk12;
	int		unk13;
	int		unk14;
	int		unk15;
	int		unk16;
	int		unk17;
	int		unk18;
	int		unk19;
	int		unk20;
	int		unk21;
	int		unk22;
	int		unk23;

	struct
	{
		unsigned int		task_id;
		int		reward1;
		int		reward2;
		namechar		desc[256];	
		unsigned int		task_id_space;	
				
	}list[9];
	
	int		unk30;
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct ACTIVITY_EXPUP_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	unsigned int		exp_bonus_max_reincarnation;
	int		exp_bonus_max_level;
	int		exp_bonus_rate;		
	unsigned int		realm_exp_max_relm_level;
	int		realm_exp_rate;

	//176+
	//int		type;
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct ACTIVITY_CHALLENGE_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	namechar		condition[256];
	int		unk2;

	char		file_icon[128];
	int		reward_count;
	unsigned int		reward_id;
	int		task_id_space;
	unsigned int		task_id;
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct ACTIVITY_CONSUMPTION_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	int		unk1;

	struct
	{
		int		require_rmb;
		unsigned int		item_id;
		int		item_count;

	}list[7];

	//176+
	//int		type;
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct ACTIVITY_SHOP_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct
	{
		unsigned int		id;
		char		file_icon[128];
		int		buy_times_limit;
		int		buy_times_limit_mode;
		int		unk6;
		unsigned int		price;

	}item[16];

	//176+
	//int		type;
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct REWARD_SHOW_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct
	{
		unsigned int		id;
		int		count;
	}item[6];

	unsigned int		relm_exp;
	unsigned int		silver;
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct EXTR_PROP_SKILL_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct
	{
		unsigned int		id;
		int		level;

	}skill[32];
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct MATERIAL_REFINE_ESSENCE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char	file_matter[128];
	char	file_icon[128];

	unsigned int		id_to_make;
	int		num_to_make;
	int		max_count;

	struct
	{
		int		count;
		float		probability;
		unsigned int		price;

	}list[64];


	int		price;
	int		shop_price;

	unsigned int		pile_num_max;

	unsigned int		has_guid;
	unsigned int		proc_type;
	
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

struct MATERIAL_REFINE_TICKET_ESSENCE
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	char	file_matter[128];
	char	file_icon[128];

	int		list_id;

	float		ext_succeed_prob[64];

	int		price;
	int		shop_price;

	unsigned int		pile_num_max;

	unsigned int		has_guid;
	unsigned int		proc_type;
	
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 162 > 171
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// NEW 172
///////////////////////////////////////////////////////////////////////////////////////

struct REMOVE_TASK_RESULT_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	int		unk1;
	int		unk2;
	int		unk3;
	int		unk4;

	struct
	{
		unsigned int		id_space;
		int		id;

	}tasks[256];
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 172
///////////////////////////////////////////////////////////////////////////////////////

struct LDBATTLE_TRANSFER_POINT_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	struct
	{
		unsigned int		unk2;
		float		pos[3];

	}tasks[51];
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 172
///////////////////////////////////////////////////////////////////////////////////////

struct HUAZHAO_ACTIVITY_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name


	unsigned int	start_task_id_space;
	unsigned int	start_task_id;
	INT64		data_start;
	INT64		data_end;
	unsigned int		receiveAwardConfig;
	int showTaskMark;

	struct
	{
		
		unsigned int		task_id;
		int		task_id_space;
		int		history_value;
		namechar		desc[1024];
		int		config_id;

	}event[3];
	
	struct {
  		unsigned int awardItemsID;
  		unsigned int awardItemsNum;
  	} task1Awards[5];
  
  	struct {
  		unsigned int awardItemsID;
  		unsigned int awardItemsNum;
  	} task2Awards[5];
  
  	struct {
  		unsigned int awardItemsID;
  		unsigned int awardItemsNum;
 	 } task3Awards[5];
	
	 int flowerNum;
 	 unsigned int waterTaskID;
 	 unsigned int warerTaskSpace;
 	 unsigned int awardTaskID;
  	 unsigned int awardTaskSpace;
 	 int active;

	 /*176+
 	 int flower_personal_value;
 	 unsigned int flower_taskconfig_id;
 	 INT64 flower_end_timestamp;
	 */
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 172
///////////////////////////////////////////////////////////////////////////////////////

struct HERO_BOOK_CONFIG
{

	unsigned int	id;							// ID
	namechar		name[32];				// Name

	int		unk1;
	int		unk2;
	int		unk3;
	int		unk4;
	
	unsigned int	start_task_id_space;
	int	start_task_id;
	int		unk5;
	
	char	file_icon[128];
	char	file_img[128];
	
	int		character_desc;
	unsigned int		reward_task_id;
	int		reward_task_id_space;
	namechar reward_desc[256];

	struct
	{
		namechar		desc[256];
		int		text;
		unsigned int	task_id;
		int		task_space;
		int		unk6;

	}chapter[4];
	
	int		desc;
	
	struct
	{
		int		chapter;
		int		space;
		unsigned int		id;

	}tasks[64];
		
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 174
///////////////////////////////////////////////////////////////////////////////////////

struct CROSS_SERVICE_ROOM_CONFIG
{

	unsigned int	id;					
	namechar		name[32];			

	namechar		desc[256];

	int		unk1;
	int		unk2;
	int		unk3;
	
	struct
	{
		namechar		unk5[32];
		unsigned int	unk6;
		unsigned int	unk7;

	}unk4[10];
			
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 174
///////////////////////////////////////////////////////////////////////////////////////

struct FLYING_CHESS_CONFIG
{

	unsigned int	id;						
	namechar		name[32];			

	unsigned int task_space;
	unsigned int task_id;

	char 	dialog[32];
	char	file_icon[128];
	

	int		unk1;
	int		unk2;
	int		unk3;
	int		unk4;
	int		unk5;
	int		unk6;
	
	struct
	{
		namechar		unk8[8];
		namechar		unk9[16];
		float	unk10[2];
		int	unk11;
		int	unk12;
		int	unk13;

	}unk7[64];
			
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 176
///////////////////////////////////////////////////////////////////////////////////////

struct HIGHPOLY_MAINCUSTOMIZE_CONFIG
{

	unsigned int	id;						
	namechar		name[32];			

	unsigned int character_combo_id;
	
  unsigned int head[2];
  unsigned int upper_length[2];
  unsigned int upper_width[2];
  unsigned int arm[2];
  unsigned int hand[2];
  unsigned int waist[2];
  unsigned int lower_length[2];
  unsigned int thigh[2];
  unsigned int calf[2];
  unsigned int foot[2];
  unsigned int chest[2];
				
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 176
///////////////////////////////////////////////////////////////////////////////////////

struct HUARONG_ROAD_GAME_CONFIG
{

	unsigned int	id;						
	namechar		name[32];			
	
	struct
	{
		int	type;
		int	x;
		int y;

	}chess_info[17];
				
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 176
///////////////////////////////////////////////////////////////////////////////////////

struct TASK_BRANCH_RESULT_CONFIG
{

	unsigned int	id;						
	namechar		name[32];			
	
	namechar	dialog[32];
	unsigned int task_id;
	unsigned int task_space;
	unsigned int award_task_id;
	unsigned int award_task_space;

	struct
	{
		struct 
		{
			int		pre_task_id;
			int		pre_task_space;
			int		pre_item_id;
			namechar 	pre_dialog[256];
		}choice[4];

	}brand_info[16];
				
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 176
///////////////////////////////////////////////////////////////////////////////////////

struct TIME_LIMIT_ITEM_CONFIG
{

	unsigned int	id;						
	namechar		name[32];			
	

	struct
	{	
		unsigned int	item_id;
		long long time_stamp;	

	}item[512];
				
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 176
///////////////////////////////////////////////////////////////////////////////////////

struct CHRISTMAS_ACTIVITY_CONFIG
{

	unsigned int	id;						
	namechar		name[32];	

	long long main_start_time_stamp;
	long long main_end_time_stamp;
	
	struct
	{	
		long long start_time_stamp;
		long long end_time_stamp;
		unsigned int	trace_npc_id;
		unsigned int	trace_map_id;

		struct 
		{
			float	x;
			float	y;
			float	z;
		}trace_pos;

	}activities_infos[5];
				
};

/*176
///////////////////////////////////////////////////////////////////////////////////////
// NEW 176
///////////////////////////////////////////////////////////////////////////////////////

struct PORTRAIT_PICTURE_ESSENCE
{

	unsigned int	id;						
	namechar		name[32];	

	int	active;
	int	source_mask;
	int	source_npc_id;

	namechar	others1[256];
	char	unk1[128];
	char	unk2[128];
	namechar	others2[256];
	char	unk7[128];
	char	unk8[128];
	char	unk9[128];
		
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 176
///////////////////////////////////////////////////////////////////////////////////////

struct BLINDBOX_CONFIG
{

	unsigned int	id;						
	namechar		name[32];	

	char			file_icon[128];
	char			show_anim[128];
	char			poster[128];

	float			once_probability;
	float			all_probability;
	int				once_consume;
	int				all_consume;
	int				count;
	int				need_time;
	INT64	start_time_stamp;
	INT64	end_time_stamp;

	struct
	{
		unsigned int shop_index;
		unsigned int item_id;
		
	}normal_itens[32];

	struct
	{
		unsigned int shop_index;
		unsigned int item_id;
		
	}limited_itens[32];

				
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 176
///////////////////////////////////////////////////////////////////////////////////////

struct ARENA_PEAK_SEASON_CONFIG
{

	unsigned int	id;						
	namechar		name[32];	

	struct
	{
		namechar	 season_name[16];
		long long start_time_stamp;
		
	}seasons_infos[40];

	int				type_count;
	int				battle_turns;
	int				type_interval_period;
	int				battle_period;
	int				wait_period;
	int				first_begin_time_offset;
				
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 176
///////////////////////////////////////////////////////////////////////////////////////

struct ARENA_PEAK_BATTLE_CONFIG
{

	unsigned int	id;						
	namechar		name[32];	

	int				sign_cost;
	int				prepare_time;
	int				battle_time;
	int				pre_point;
	int				win_upbound;
	int				win_lose_off;
	int				win_none_person;

	float			attack_first_pos_x;
	float			attack_first_pos_y;
	float			attack_first_pos_z;

	float			attack_reborn_pos_x;
	float			attack_reborn_pos_y;
	float			attack_reborn_pos_z;

	float			defend_first_pos_x;
	float			defend_first_pos_y;
	float			defend_first_pos_z;

	float			defend_reborn_pos_x;
	float			defend_reborn_pos_y;
	float			defend_reborn_pos_z;

	float		forbid_dis;
	int			die_punish_time;
	int			pre_join_time;
	int			break_time;
	int			force_trans_time;

	float		attack_waitting_rom_pos_x;
	float		attack_waitting_rom_pos_y;
	float		attack_waitting_rom_pos_z;

	float		defend_waitting_rom_pos_x;
	float		defend_waitting_rom_pos_y;
	float		defend_waitting_rom_pos_z;

	float		ob_pos_x;
	float		ob_pos_y;
	float		ob_pos_z;

				
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 176
///////////////////////////////////////////////////////////////////////////////////////

struct ARENA_PEAK_AWARD_CONFIG
{

	unsigned int	id;						
	namechar		name[32];	

	struct
	{
		unsigned int	item_id;
		int		item_count;
		
	}award_item[8];
				
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 176
///////////////////////////////////////////////////////////////////////////////////////

struct DEFAULT_HAIR_CONFIG
{

	unsigned int	id;						
	namechar		name[32];	

	unsigned int character_combo_id;
	unsigned int gender_id;
	unsigned int hair_id;
	unsigned int hair_id_highpoly;
				
};

///////////////////////////////////////////////////////////////////////////////////////
// NEW 176
///////////////////////////////////////////////////////////////////////////////////////

struct EXPLORE_PROGRESS_DIALOG_CONFIG
{

	unsigned int	id;						
	namechar		name[32];	

	int		map_id;
	int		explore_area_id;
	namechar	dialog_text[256];
	char	dialog_name[32];

	int explored_value_key;
	int max_explore_value;

	struct
	{

		int		point_value;
		int		award_item_id;

	}explore_point[16];

	struct
	{
		
		namechar	type_name[32];
		int 	explored_value_key;
		int		max_explore_value;

	}explore_type[6];

				
};


///////////////////////////////////////////////////////////////////////////////////////
// NEW 176
///////////////////////////////////////////////////////////////////////////////////////

struct EXPLORE_OBJECT_CONFIG
{

	unsigned int	id;						
	namechar		name[32];	

	int		pre_task_id;
	int		dialog_templ_id;

	struct
	{

		int 	task_id;
		int		icon_id;
		float 	pos_x;
		float 	pos_y;

	}object[256];

				
};
*/




enum ID_SPACE
{
	ID_SPACE_ESSENCE	= 0,
	ID_SPACE_ADDON		= 1,
	ID_SPACE_TALK		= 2,
	ID_SPACE_FACE		= 3,
	ID_SPACE_RECIPE		= 4,
	ID_SPACE_CONFIG		= 5,
	ID_SPACE_HOME		= 6,
};

enum DATA_TYPE
{
	DT_INVALID					= 0,
	DT_EQUIPMENT_ADDON			,
	DT_WEAPON_MAJOR_TYPE		,
	DT_WEAPON_SUB_TYPE			,
	DT_WEAPON_ESSENCE			,

	DT_ARMOR_MAJOR_TYPE			,	
	DT_ARMOR_SUB_TYPE			,
	DT_ARMOR_ESSENCE			,
	DT_DECORATION_MAJOR_TYPE	,
	DT_DECORATION_SUB_TYPE		,

	DT_DECORATION_ESSENCE		= 10,
	DT_MEDICINE_MAJOR_TYPE		,
	DT_MEDICINE_SUB_TYPE		,
	DT_MEDICINE_ESSENCE			,
	DT_MATERIAL_MAJOR_TYPE		,

	DT_MATERIAL_SUB_TYPE		,
	DT_MATERIAL_ESSENCE			,
	DT_DAMAGERUNE_SUB_TYPE		,
	DT_DAMAGERUNE_ESSENCE		,
	DT_ARMORRUNE_SUB_TYPE		,
	
	DT_ARMORRUNE_ESSENCE		= 20,
	DT_SKILLTOME_SUB_TYPE		,
	DT_SKILLTOME_ESSENCE		,
	DT_FLYSWORD_ESSENCE			,
	DT_WINGMANWING_ESSENCE		,

	DT_TOWNSCROLL_ESSENCE		,
	DT_UNIONSCROLL_ESSENCE		,
	DT_REVIVESCROLL_ESSENCE		,
	DT_ELEMENT_ESSENCE			,		
	DT_TASKMATTER_ESSENCE		,

	DT_TOSSMATTER_ESSENCE		= 30,
	DT_PROJECTILE_TYPE			,
	DT_PROJECTILE_ESSENCE		,
	DT_QUIVER_SUB_TYPE			,
	DT_QUIVER_ESSENCE			,

	DT_STONE_SUB_TYPE			,
	DT_STONE_ESSENCE			,
	DT_MONSTER_ADDON			,		
	DT_MONSTER_TYPE				,
	DT_MONSTER_ESSENCE			,	

	DT_NPC_TALK_SERVICE			= 40,
	DT_NPC_SELL_SERVICE			,
	DT_NPC_BUY_SERVICE			,
	DT_NPC_REPAIR_SERVICE		,
	DT_NPC_INSTALL_SERVICE		,

	DT_NPC_UNINSTALL_SERVICE	,
	DT_NPC_TASK_IN_SERVICE		,
	DT_NPC_TASK_OUT_SERVICE		,
	DT_NPC_TASK_MATTER_SERVICE	,
	DT_NPC_SKILL_SERVICE		,
	
	DT_NPC_HEAL_SERVICE			= 50,
	DT_NPC_TRANSMIT_SERVICE		,
	DT_NPC_TRANSPORT_SERVICE	,
	DT_NPC_PROXY_SERVICE		,
	DT_NPC_STORAGE_SERVICE		,

	DT_NPC_MAKE_SERVICE			,
	DT_NPC_DECOMPOSE_SERVICE	,
	DT_NPC_TYPE					,
	DT_NPC_ESSENCE				,
	DT_TALK_PROC				,

	DT_FACE_TEXTURE_ESSENCE		= 60,
	DT_FACE_SHAPE_ESSENCE		,
	DT_FACE_EMOTION_TYPE		,
	DT_FACE_EXPRESSION_ESSENCE	,
	DT_FACE_HAIR_ESSENCE		,

	DT_FACE_MOUSTACHE_ESSENCE	,
	DT_COLORPICKER_ESSENCE		,
	DT_CUSTOMIZEDATA_ESSENCE	,
	DT_RECIPE_MAJOR_TYPE		,
	DT_RECIPE_SUB_TYPE			,

	DT_RECIPE_ESSENCE			= 70,	
	DT_ENEMY_FACTION_CONFIG		,
	DT_CHARRACTER_CLASS_CONFIG	,
	DT_PARAM_ADJUST_CONFIG		,
	DT_PLAYER_ACTION_INFO_CONFIG,

	DT_TASKDICE_ESSENCE			= 75,
	DT_TASKNORMALMATTER_ESSENCE	,
	DT_FACE_FALING_ESSENCE		,
	DT_PLAYER_LEVELEXP_CONFIG	,
	DT_MINE_TYPE,

	DT_MINE_ESSENCE				= 80,
	DT_NPC_IDENTIFY_SERVICE		,
	DT_FASHION_MAJOR_TYPE		,	
	DT_FASHION_SUB_TYPE			,
	DT_FASHION_ESSENCE			,

	DT_FACETICKET_MAJOR_TYPE	= 85,
	DT_FACETICKET_SUB_TYPE,
	DT_FACETICKET_ESSENCE,
	DT_FACEPILL_MAJOR_TYPE,
	DT_FACEPILL_SUB_TYPE,

	DT_FACEPILL_ESSENCE			= 90,
	DT_SUITE_ESSENCE,
	DT_GM_GENERATOR_TYPE,
	DT_GM_GENERATOR_ESSENCE,
	DT_PET_TYPE,

	DT_PET_ESSENCE				= 95,
	DT_PET_EGG_ESSENCE,
	DT_PET_FOOD_ESSENCE,
	DT_PET_FACETICKET_ESSENCE,
	DT_FIREWORKS_ESSENCE,

	DT_WAR_TANKCALLIN_ESSENCE	= 100,
	DT_NPC_WAR_TOWERBUILD_SERVICE,
	DT_PLAYER_SECONDLEVEL_CONFIG,
	DT_NPC_RESETPROP_SERVICE,
	DT_NPC_PETNAME_SERVICE,

	DT_NPC_PETLEARNSKILL_SERVICE  = 105,
	DT_NPC_PETFORGETSKILL_SERVICE,
	DT_SKILLMATTER_ESSENCE,
	DT_REFINE_TICKET_ESSENCE,
	DT_DESTROYING_ESSENCE,

	DT_NPC_EQUIPBIND_SERVICE	  = 110,
	DT_NPC_EQUIPDESTROY_SERVICE,
	DT_NPC_EQUIPUNDESTROY_SERVICE,
	DT_BIBLE_ESSENCE,
	DT_SPEAKER_ESSENCE,			

	DT_AUTOHP_ESSENCE			  = 115,		
	DT_AUTOMP_ESSENCE,
	DT_DOUBLE_EXP_ESSENCE,
	DT_TRANSMITSCROLL_ESSENCE,
	DT_DYE_TICKET_ESSENCE,

	DT_GOBLIN_ESSENCE			= 120,
	DT_GOBLIN_EQUIP_TYPE,
	DT_GOBLIN_EQUIP_ESSENCE,
	DT_GOBLIN_EXPPILL_ESSENCE,
	DT_SELL_CERTIFICATE_ESSENCE,

	DT_TARGET_ITEM_ESSENCE		= 125,
	DT_LOOK_INFO_ESSENCE,
	DT_UPGRADE_PRODUCTION_CONFIG,
	DT_ACC_STORAGE_BLACKLIST_CONFIG,
	DT_FACE_HAIR_TEXTURE_MAP,

	DT_MULTI_EXP_CONFIG         = 130,
	DT_INC_SKILL_ABILITY_ESSENCE,
	DT_GOD_EVIL_CONVERT_CONFIG,
	DT_WEDDING_CONFIG,
	DT_WEDDING_BOOKCARD_ESSENCE,

	DT_WEDDING_INVITECARD_ESSENCE	=	135,
	DT_SHARPENER_ESSENCE,
	DT_FACE_THIRDEYE_ESSENCE,
	DT_FACTION_FORTRESS_CONFIG,
	DT_FACTION_BUILDING_SUB_TYPE,

	DT_FACTION_BUILDING_ESSENCE	=	140,
	DT_FACTION_MATERIAL_ESSENCE,
	DT_CONGREGATE_ESSENCE,
	DT_ENGRAVE_MAJOR_TYPE,
	DT_ENGRAVE_SUB_TYPE,

	DT_ENGRAVE_ESSENCE	= 145,
	DT_NPC_ENGRAVE_SERVICE,
	DT_NPC_RANDPROP_SERVICE,
	DT_RANDPROP_TYPE,
	DT_RANDPROP_ESSENCE,

	DT_WIKI_TABOO_CONFIG = 150,
	DT_FORCE_CONFIG,
	DT_FORCE_TOKEN_ESSENCE,
	DT_NPC_FORCE_SERVICE,
	DT_PLAYER_DEATH_DROP_CONFIG,

	DT_DYNSKILLEQUIP_ESSENCE = 155,
	DT_CONSUME_POINTS_CONFIG,
	DT_ONLINE_AWARDS_CONFIG,
	DT_COUNTRY_CONFIG,
	DT_GM_ACTIVITY_CONFIG,

	DT_FASHION_WEAPON_CONFIG = 160,
	DT_PET_EVOLVE_CONFIG,
	DT_PET_EVOLVED_SKILL_CONFIG,
	DT_MONEY_CONVERTIBLE_ESSENCE,
	DT_STONE_CHANGE_RECIPE_TYPE,

	DT_STONE_CHANGE_RECIPE = 165,
	DT_MERIDIAN_CONFIG,
	DT_PET_EVOLVED_SKILL_RAND_CONFIG,
	DT_AUTOTASK_DISPLAY_CONFIG,
	DT_TOUCH_SHOP_CONFIG,

	DT_TITLE_CONFIG,
	DT_COMPLEX_TITLE_CONFIG,
	DT_MONSTER_SPIRIT_ESSENCE,
	DT_PLAYER_SPIRIT_CONFIG,
	DT_PLAYER_REINCARNATION_CONFIG,

	DT_HISTORY_STAGE_CONFIG,
	DT_HISTORY_ADVANCE_CONFIG,
	DT_AUTOTEAM_CONFIG,
	DT_PLAYER_REALM_CONFIG,
	DT_CHARIOT_CONFIG,

	DT_CHARIOT_WAR_CONFIG,
	DT_POKER_LEVELEXP_CONFIG,
	DT_POKER_SUITE_ESSENCE,
	DT_POKER_DICE_ESSENCE,
	DT_POKER_SUB_TYPE,

	DT_POKER_ESSENCE,
	DT_TOKEN_SHOP_CONFIG,
	DT_SHOP_TOKEN_ESSENCE,
	DT_GT_CONFIG,
	DT_RAND_SHOP_CONFIG,

	DT_PROFIT_TIME_CONFIG,
	DT_FACTION_PVP_CONFIG,
	DT_UNIVERSAL_TOKEN_ESSENCE,
	DT_TASK_LIST_CONFIG,
	DT_TASK_DICE_BY_WEIGHT_CONFIG,

	DT_FASHION_SUITE_ESSENCE,
	DT_FASHION_BEST_COLOR_CONFIG,
	DT_SIGN_AWARD_CONFIG,
	DT_ASTROLABE_ESSENCE,
	DT_ASTROLABE_RANDOM_ADDON_ESSENCE,

	DT_ASTROLABE_INC_INNER_POINT_VALUE_ESSENCE,
	DT_ASTROLABE_INC_EXP_ESSENCE,
	DT_ITEM_PACKAGE_BY_PROFESSION_ESSENCE,
	DT_ASTROLABE_LEVELEXP_CONFIG,
	DT_ASTROLABE_ADDON_RANDOM_CONFIG,

	DT_ASTROLABE_APPEARANCE_CONFIG,
	DT_EQUIP_MAKE_HOLE_CONFIG,
	DT_SOLO_TOWER_CHALLENGE_LEVEL_CONFIG,
	DT_SOLO_TOWER_CHALLENGE_AWARD_PAGE_CONFIG,
	DT_SOLO_TOWER_CHALLENGE_AWARD_LIST_CONFIG,

	DT_SOLO_TOWER_CHALLENGE_SCORE_COST_CONFIG,
	DT_MNFACTION_WAR_CONFIG,
	DT_NPC_CROSS_SERVER_SERVICE,
	DT_FIREWORKS2_ESSENCE,
	DT_FIX_POSITION_TRANSMIT_ESSENCE,

	// Mew

	DT_HOME_CONFIG, // 155
	DT_HOME_PRODUCTS_CONFIG,
	DT_HOME_RESOURCE_PRODUCE_CONFIG,
	DT_HOME_FORMULAS_PRODUCE_RECIPE,
	DT_HOME_FORMULAS_ITEM_ESSENCE,
	DT_HOME_PRODUCE_SERVICE_CONFIG,
	DT_HOME_FACTORY_CONFIG,
	DT_HOME_ITEM_MAJOR_TYPE,
	DT_HOME_ITEM_SUB_TYPE,
	DT_HOME_ITEM_ENTITY,
	DT_HOME_FORMULAS_PRODUCE_MAJOR_TYPE,
	DT_HOME_FORMULAS_PRODUCE_SUB_TYPE,
	DT_HOME_FORMULAS_ITEM_MAJOR_TYPE,
	DT_HOME_FORMULAS_ITEM_SUB_TYPE,
	DT_HOME_STORAGE_TASK_CONFIG,
	

	DT_WISH_TRIBUTE_ESSENCE, // 155
	DT_RED_PACKET_PAPER_ESSENCE,
	DT_LOTTORY_TICKET_STORAGE_CONFIG,
	DT_LOTTORY_TICKET_COST_CONFIG,
	DT_LOTTORY_TICKET_OTHER_AWARD_CONFIG,
	DT_HOME_TEXTURE_ENTITY,
	DT_HOME_GRASS_ENTITY,
	DT_HOME_UNLOCK_PAPER_ESSENCE,
	
	DT_GUARDIAN_BEAST_EGG_ESSENCE, // 155 v2
	DT_GUARDIAN_BEAST_ENTITY,
	DT_GUARDIAN_BEAST_RACE_CONFIG,
	DT_GUARDIAN_BEAST_UPGRADE_CONFIG,
	DT_GUARDIAN_BEAST_REWARD_CONFIG,

	DT_HOME_SKIN_ENTITY, // 156
	DT_WORLD_SPEAK_COST_CONFIG,
	DT_EASY_PRODUCE_ITEM_ESSENCE,
	
	DT_ARENA_CONFIG, // 156
	DT_ARENA_SCENE_CONFIG,
	DT_ARENA_POINT_CONFIG,
	DT_ARENA_ENTITY_CONFIG,
	DT_NPC_ARENA_SELL_SERVICE,

	DT_UNLOCK_RUNE_SLOT_ITEM_ESSENCE, // 156
	DT_RUNE_ITEM_ESSENCE,
	DT_RUNE_SKILL_CONFIG,
	DT_RUNE_UPGRADE_CONFIG,
	DT_ARENA_MAP_CONFIG,
	DT_NPC_STATUE_CREATE_SERVICE,
	DT_ARENA_FORBID_ITEM_CONFIG,
	DT_ARENA_FORBID_SKILL_CONFIG,
		
	DT_CARRIER_CONFIG, // 156
	DT_CHANGE_PROPERTY_CONFIG,
	DT_PROFESSION_PROPERTY_CONFIG,
	DT_FIX_MONSTER_ITEM_ESSENCE,
	DT_NPC_LIB_PRODUCE_SERVICE,
	DT_LIB_PRODUCE_RECIPE,
	DT_VOTE_CONFIG,

	DT_SIMULATOR_ITEM_LIST_CONFIG, // 156
	DT_EQUIPMENT_PRODUCE_METHOD_CONFIG,
	DT_PRODUCTION_LINE_CONFIG,
	DT_SOURCE_OF_MATERIAL_CONFIG,
	DT_FACTION_TALENT_SINGLE_CONFIG,
	DT_FACTION_TALENT_TREE_CONFIG,
	DT_FACTION_TARGET_CONFIG,

	DT_INSTANCE_STAGE_CONFIG, // 157
	DT_FACTION_PET_FEED_CONFIG,
	DT_FACTION_TILLAGE_CONFIG,
	DT_FACTION_PET_CONFIG,	
	DT_FACTION_PET_BLESS_CONFIG,
	DT_ITEM_USED_FOR_AREA_ESSENCE,
	DT_CAPTURE_ITEM_ESSENCE,

	DT_DRIFT_BOTTLE_DROP_CONFIG, // 157-158
	DT_NPC_FACTION_SELL_SERVICE,
	DT_FACTION_INSTANCE_DROP_CONFIG,
	DT_NPC_PARK_ENTER_SERVICE,
	DT_FACTION_STORAGE_WHITELIST_CONFIG,
	DT_NPC_BOSSRUSH_AWARD_SERVICE,
	DT_PROFESSION_STATE_CONFIG,
	DT_MENTAL_PARAM_ADJUST_CONFIG,

	DT_BIBLE_REFINE_CONFIG, // 159
	DT_BIBLE_REFINE_TICKET_ESSENCE, 
	DT_TITLE_PROGRESS_CONFIG,
	DT_NPC_MYSTERIOUS_SHOP_SERVICE,
	DT_OFFLINEBATTLE_CONFIG,
	DT_NPC_PRIDICTABLE_ENGRAVE_SERVICE,

	DT_NEW_ARMOR_MAJOR_TYPE, // 160
	DT_NEW_ARMOR_SUB_TYPE,
	DT_NEW_ARMOR_ESSENCE,
	
	DT_QIHUN_ESSENCE, // 160
	DT_QILING_ESSENCE,
	DT_STORY_BOOK_MAJOR_TYPE,
	DT_STORY_BOOK_CONFIG,

	DT_MENTOR_LEVEL_CONFIG, // 160
	DT_STUDENT_AWARD_CONFIG,
	DT_QIHUN_COVER_ESSENCE,
	DT_GROWTH_TARGET_CONFIG,
	DT_QUESTION_TIP_CONFIG,
	DT_QUESTION_AWARD_CONFIG,

	DT_SLIDE_SKILL_ESSENCE, // 162
	DT_CONSUME_MONEY_CONFIG,
	DT_USE_FOR_SELF_ITEM_ESSENCE,

	// Mew
	
	DT_HIGH_POLY_FACE_TEXTURE_ESSENCE, // 162 > 171 start
	DT_HIGH_POLY_FACE_TEMPL_CONFIG,

	DT_NEW_LOTTERY_CONFIG,
	DT_CURRENCY_ITEM_WHITELIST_CONFIG,

	DT_HIGH_POLY_CUSTOMIZEDATA_ESSENCE,
	DT_HIGH_POLY_RACE_FEATURE,

	DT_BLACK_WHITE_LIST_MAJOR_TYPE,
	DT_BLACK_WHITE_LIST_CONFIG,

	DT_LOANS_PRODUCE_MAJOR_TYPE,
	DT_LOANS_PRODUCE_SUB_TYPE,
	DT_LOANS_PRODUCE_RECIPE,

	DT_RED_BOOK_CONFIG,
	DT_RED_BOOK_UPGRADE_ITEM,
	DT_RED_BOOK_LOTTERY_CONFIG,

	DT_ARENA_TICKET_CONFIG,
	DT_CAMP_LEVEL_CONFIG,
	DT_CAMP_TOKEN_CONFIG,
	DT_CAMP_TOKEN_TREE_CONFIG,
	DT_CAMP_TOKEN_ESSENCE,
	DT_CAMP_TOKEN_UPGRADE_ESSENCE,
	DT_CAMP_TOKEN_ROLLBACK_ESSENCE,
	DT_CAMP_TOKEN_PROB_ADJUST_ESSENCE,
	DT_CAMP_BATTLE_TECH_TREE_CONFIG,
	
	DT_SPECIAL_ACTIVITY_RESPAWN_CONFIG,
	DT_MAP_EVENT_CONFIG,
	DT_SPECIAL_CAMP_REGION_MAJOR_TYPE,

	DT_SPECIAL_CAMP_REGION_CONFIG,
	DT_PET_SKIN_CONFIG,
	DT_PET_MULTIRIDE_CONFIG,

	DT_FASHION_NEW_MAJOR_TYPE,
	DT_FASHION_NEW_SUB_TYPE,
	DT_FASHION_NEW_ESSENCE,

	DT_WAR_AREA_CONFIG,

	DT_ILLUSTRATED_FASHION_SERIES_CONFIG,
	DT_ILLUSTRATED_FASHION_ESSENCE,
	DT_ILLUSTRATED_WING_EGG_ESSENCE,
	DT_ILLUSTRATED_WING_SERIES_CONFIG,
	DT_ILLUSTRATED_PET_EGG_ESSENCE,
	DT_ILLUSTRATED_PET_SERIES_CONFIG,
	DT_ACROSS_SERVER_MATCH_CONFIG,
	DT_NPC_ASAL_CONFIG,
	DT_WILDERNESS_SURVIVAL_CONFIG,
	DT_ILLUSTRATED_REWARD_CONFIG,

	DT_FAST_PRODUCE_ITEM_TYPE,
	DT_FAST_PRODUCE_ITEM_ESSENCE,

	DT_KID_SYSTEM_CONFIG,
	DT_COURSE_ESSENCE,
	DT_COURSE_SUITE_ESSENCE,	
	DT_KID_PROPERTY_CONFIG,
	DT_KID_QUALITY_CONFIG,
	DT_KID_EXP_CONFIG,
	DT_KID_UPGRADE_STAR_CONFIG,
	DT_KID_LEVEL_REWARD_CONFIG,
	DT_KID_LEVEL_MAX_CONFIG,
	DT_KID_DEBRIS_ESSENCE,
	DT_KID_SKILL_CONFIG,
	DT_KID_DEBRIS_GENERATOR_ESSENCE,
	DT_KID_PROFESSION_CONFIG,
	DT_BIS_SOI_USE_LIMIT_CONFIG,

	DT_REWARD_INTERFACE_CONFIG,
	DT_HISTORY_VARIABLE_PROGRESS_CONFIG,
	DT_ANNIVERSARY_EVENT_INTERFACE_CONFIG,
	
	DT_ACTIVITY_EXPUP_CONFIG,
	DT_ACTIVITY_CHALLENGE_CONFIG,
	DT_ACTIVITY_CONSUMPTION_CONFIG,
	DT_ACTIVITY_SHOP_CONFIG,
	
	DT_REWARD_SHOW_CONFIG,
	DT_EXTR_PROP_SKILL_CONFIG,
	DT_MATERIAL_REFINE_ESSENCE,
	DT_MATERIAL_REFINE_TICKET_ESSENCE, // 162 > 171 end
	
	// 172 > 
	
	DT_REMOVE_TASK_RESULT_CONFIG,
	DT_LDBATTLE_TRANSFER_POINT_CONFIG,
	DT_HUAZHAO_ACTIVITY_CONFIG,
	DT_HERO_BOOK_CONFIG,	
	DT_CROSS_SERVICE_ROOM_CONFIG,

	DT_FLYING_CHESS_CONFIG,
	DT_HIGHPOLY_MAINCUSTOMIZE_CONFIG,
	DT_HUARONG_ROAD_GAME_CONFIG,
	DT_TASK_BRANCH_RESULT_CONFIG,

	DT_TIME_LIMIT_ITEM_CONFIG,
	DT_CHRISTMAS_ACTIVITY_CONFIG,	
	/*176
	DT_PORTRAIT_PICTURE_ESSENCE,
	DT_BLINDBOX_CONFIG,
	DT_ARENA_PEAK_SEASON_CONFIG,

	DT_ARENA_PEAK_BATTLE_CONFIG,
	DT_ARENA_PEAK_AWARD_CONFIG,	
	DT_DEFAULT_HAIR_CONFIG,
	DT_EXPLORE_PROGRESS_DIALOG_CONFIG,
	DT_EXPLORE_OBJECT_CONFIG,
	*/

	/* 177
	DT_GACHA_CONFIG,
	DT_WARTOKEN_TIME_CONFIG,	
	DT_WARTOKEN_AWARD_CONFIG,
	DT_PERIODIC_CHANGED_CONFIG,
	DT_TITLE_ITEM_ESSENCE,

	DT_ITEM_TITLE_CONFIG,
	 */


	DT_MAX,
	
};

#pragma pack(pop, EXP_TYPES_INC)
#endif//_EXP_TYPES_H_


