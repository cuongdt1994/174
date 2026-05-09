#ifndef	__GNET_LOCALMACRO_H
#define	__GNET_LOCALMACRO_H

#include "groleinventory"
#include "gshoplog"
#include "gmailsyncdata"

namespace GNET
{

#define LOGICUID_START    1024
#define LOGICUID_MAX      0x7FFFFFFF 
#define LOGICUID(rid)     (int)((rid) & 0xFFFFFFF0)
#define MAX_NAME_SIZE     40
#define MAX_FACTION_NAME_SIZE     40
#define USER_CLASS_COUNT  15

enum 
{
	CMSG_BIDSTART        = 1,  //��ս��ʼ����
	CMSG_BIDEND          = 2,  //��ս���۽���
	CMSG_BATTLESTART     = 3,  //��ս��ʼ
	CMSG_BATTLEEND       = 4,  //��ս����
	CMSG_BATTLEBID       = 5,  //��ս����
	CMSG_BONUSSEND       = 6,  //���������Ѿ�����
	CMSG_SPECIAL		 = 7, //special bonus sent

	CMSG_CARDONSALE      = 10, //�㿨��ʼ����
	CMSG_CARDSOLD        = 11, //�㿨�Ѿ��۳�
	CMSG_SELLCARDDONE    = 12, //�㿨�۳�(����)
	CMSG_BUYCARDDONE     = 13, //����㿨�ɹ�(����)
	CMSG_BUYCARDFAILED   = 14, //����㿨ʧ��(����)
	CMSG_MONEYOVERFLOW   = 15, //������Ǯ��������(����)
	CMSG_MONEYRECEIVED   = 16, //�ۿ������ѵ���(����)
	CMSG_CARDTIMEOUT     = 17, //�㿨���۳�ʱ(����)

	CMSG_SYSAUCTION_FORENOTICE 	= 18,	//Ԥ�澺��ʱ��
	CMSG_SYSAUCTION_START 	= 19,	//���Ŀ���
	CMSG_SYSAUCTION_BID   	= 20,	//���ľ���
	CMSG_SYSAUCTION_BIDINFO = 21,	//���ľ�����Ϣ
	CMSG_SYSAUCTION_END		= 22,	//���Ľ���
											//24�Ѿ�������ռ��
	CMSG_FACTION_BECAME_ALLIANCE	= 25,	//��������ͬ��
	CMSG_FACTION_REMOVE_ALLIANCE	= 26,	//�������ͬ��
	CMSG_FACTION_BECAME_HOSTILE		= 27,	//�������ɵж�
	CMSG_FACTION_REMOVE_HOSTILE		= 28,	//������ɵж�

	CMSG_FF_CHALLENGESTART 		= 29,	//���ɻ�����ս��ʼ
	CMSG_FF_BATTLESTART 		= 30,	//���ɻ���ս����ʼ
	CMSG_FF_CREATE				= 31,	//���ɻ��ش���(�����ں���)
	CMSG_FF_HEALTHNOTIFY		= 32,	//���ɻ��ؽ�����(�����ں���)
	CMSG_FF_BECHALLENGED 		= 33,	//���ɻ��ر�ȡ��ս��Ȩ(�����ں���)
	CMSG_FF_CHALLENGESUCCESS 	= 34,	//���ɻ���ȡ��ս��Ȩ(�����ں���)
	CMSG_FF_BATTLESTARTNOTIFY	= 35,	//���ɻ���ս����ʼ(�����ں���)
	CMSG_FF_BATTLEENDNOTIFY		= 36,	//���ɻ���ս������(�����ں���)
	//���°��ɻ��غ�����gsʹ�ã�������gsp_if.h��һ�£������е�msg������ʽ���ϲ�ͬ
	CMSG_FF_LEVELUP 			= 37,	//���ɻ�������(�����ں���)
	CMSG_FF_TECHNOLOGYUP		= 38,	//���ɻ��ؿƼ�����(�����ں���)
	CMSG_FF_CONSTRUCT			= 39,	//���ɻ��ؽ���(�����ں���)
	CMSG_FF_CONSTRUCTCOMPLETE 	= 40,	//���ɻ��ؽ������(�����ں���)
	CMSG_FF_HANDINCONTRIB		= 41,	//���ɻ����Ͻ�����(�����ں���)
	CMSG_FF_HANDINMATERIAL		= 42,	//���ɻ����Ͻ�����(�����ں���)
	CMSG_FF_BATTLEPREPARECLEAR 	= 43,	//���ɻ���ս��ǰ�峡(�����ں���)
	CMSG_FF_KEYBUILDINGDESTROY 	= 44,	//���ɻ��شݻ������ؽ���
	CMSG_FF_DISMANTLE			= 45,	//���ɻ�����ʩ���

	CMSG_COUNTRYBATTLE_ATTACK	= 46,	//���������������
	CMSG_COUNTRYBATTLE_DEFEND	= 47,	//�����������ҵĽ���
	CMSG_COUNTRYBATTLE_WIN		= 48,	//Ӯ��һ����սս��ʤ��
	CMSG_COUNTRYBATTLE_LOSE		= 49,	//һ����սս��ʧ��
	CMSG_COUNTRYBATTLE_KING_ASSIGN_ASSAULT = 50,	//��ս����ʹ�ü�Ԯ��
	CMSG_COUNTRYBATTLE_KING_SET_BATTLE_LIMIT = 51,	//��ս��������ս������

	CMSG_KE_CANDIDATE_APPLY_ANNOUNCE= 52,
	CMSG_KE_CANDIDATE_APPLY_START	= 53,
	CMSG_KE_CANDIDATE_APPLY_END		= 54,
	CMSG_KE_VOTE_ANNOUNCE			= 55,
	CMSG_KE_VOTE_START				= 56,
	CMSG_KE_VOTE_END				= 57,
	CMSG_KE_CANDIDATE_VOTES_100		= 58,
	CMSG_KE_CANDIDATE_VOTES_TOP		= 59,

    CMSG_FRB_MINECAR_LIMIT	= 60,
    CMSG_FRB_MINEBASE_LIMIT	= 61,
    CMSG_FRB_BATTLE_ON_PVP	= 62,
    CMSG_FRB_BATTLE_OPEN	= 63,
    CMSG_FRB_BATTLE_CLOSE	= 64,
};

enum
{
	//��Ҫ��db_if.h �е�DBMASK_PUT_*����һ��
	CDB_PUT_STOREHOUSE   =  0x00000001,
	CDB_PUT_INVENTORY    =  0x00000002,
	CDB_PUT_TASK         =  0x00000004,
	CDB_PUT_EQUIPMENT    =  0x00000008,
	CDB_PUT_CASH         =  0x00000010,
//	CDB_PUT_ALL          =  0x0000001F,
	CDB_PUT_USERSTORE    =  0x00000020,
	CDB_PUT_ALL          =  0x0000003F,
};

enum{
	GMSTATE_ACTIVE   = 0x01, 
};

enum
{
	CASH_GETSERIAL_FAILED = -16,
	CASH_ADD_FAILED       = -17,
	CASH_NOT_ENOUGH       = -18,
	CASH_USER_NOT_EXIST   = -19,	//AU��ֵ�յ�����������-19,���ٽ��г�ֵ����.
};

#define MAX_USER_CASH           2000000000
#define CASH_BALANCE_THRESHOLD  10000000
#define STOCK_TAX_RATE    1.02
#define STOCK_BALANCE_ID  1
#define MAX_USER_CASH_VIP_SCORE           2000000000
#define CASH_VIP_SCORE_BALANCE_THRESHOLD  50000000

enum{
        SYNC_STOTEHOUSE  = 0x01,
        SYNC_CASHUSED    = 0x02,
        SYNC_CASHTOTAL   = 0x04,
		SYNC_SHOPLOG     = 0x08,
		SYNC_MODIFYCASHUSED     = 0x10,
};

enum ERR_STOCK
{
        ERR_STOCK_CLOSED        = 1,
        ERR_STOCK_ACCOUNTBUSY   = 2,
        ERR_STOCK_INVALIDINPUT  = 3,
        ERR_STOCK_OVERFLOW      = 4,
        ERR_STOCK_DATABASE      = 5,
        ERR_STOCK_NOTENOUGH     = 6,
        ERR_STOCK_MAXCOMMISSION = 7,
        ERR_STOCK_NOTFOUND      = 8,
	ERR_STOCK_CASHLOCKED    = 9,
	ERR_STOCK_CASHUNLOCKFAILED= 10,
};

enum ORDER_RESULT
{
        STOCK_ORDER_DONE        = 0,
        STOCK_ORDER_CANCEL      = 1,
        STOCK_ORDER_TIMEOUT     = 2,
};

class Utility
{
public:
        static void SwapSyncData(GMailSyncData& data1, GMailSyncData& data2)
        {
                std::swap(data1.data_mask, data2.data_mask);
                std::swap(data1.cash_total, data2.cash_total);
                std::swap(data1.cash_used, data2.cash_used);
                std::swap(data1.cash_serial, data2.cash_serial);
                std::swap(data1.inventory.capacity, data2.inventory.capacity);
                std::swap(data1.inventory.timestamp, data2.inventory.timestamp);
                std::swap(data1.inventory.money, data2.inventory.money);

                data1.inventory.items.swap(data2.inventory.items);
		data1.equipment.swap(data2.equipment);

                std::swap(data1.storehouse.capacity, data2.storehouse.capacity);
                std::swap(data1.storehouse.money, data2.storehouse.money);
                data1.storehouse.items.swap(data2.storehouse.items);
                std::swap(data1.storehouse.size1, data2.storehouse.size1);
                std::swap(data1.storehouse.size2, data2.storehouse.size2);
                data1.storehouse.dress.swap(data2.storehouse.dress);
                data1.storehouse.material.swap(data2.storehouse.material);
		data1.logs.swap(data2.logs);
        }
};

enum CHALLENGE_ALGO
{
	ALGO_NONE = -1,
	ALGO_MD5 = 0,
	ALGO_SHA = 1,
};

enum AC_LOGIN_STATUS
{
	AC_LOGIN_STATUS_CLIENT,
	AC_LOGIN_STATUS_WEB = 0x10,
	AC_LOGIN_STATUS_MOBILE = 0x20,
};

enum ERR_KOREALOGIN
{
	CMD_LOGIN_DENY_CANT = 231,
	CMD_FAULT_AGECHECK = 232,
	CMD_LOGIN_DENY_SERVERBUSY = 233,
	CMD_FAULT_BETA_TESTER = 234,
	CMD_LOGIN_DENY_IDERR = 235,
	CMD_LOGIN_DENY_PASSERRLIMIT = 236,
	CMD_LOGIN_DENY_PASSERR = 237,
	CMD_LOGIN_DENY_AGE = 238,
};

enum FORBID
{
	FORBID_USER_LOGIN=100,
	FORBID_CHEATER_LOGIN=105,
};

enum USER_STATUS
{
	STATUS_CASHINVISIBLE   =   0x01,
};

enum REF_ERRCODE
{
        REF_ERR_NOREFERRALFOUND =	281,
        REF_ERR_REFERRERNOTLOADED,
        REF_ERR_REFERRERINTRANSACTION,
        REF_ERR_NOBONUSAVAILABLE,
        REF_ERR_SUBMITTODB,
        REF_ERR_INVALIDSPREADCODE,
        REF_ERR_REFERRERNOTINSERVER,
        REF_ERR_REFREFERRERLEVEL,
        REF_ERR_REFERRERNOTINDISTRICT,
        REF_ERR_REFERRER_DUPLICATE,
	REF_ERR_REPUTATION,
};

enum ERR_REWARD
{
	ERR_REWARD_PLAYER_NOT_ONLINE 	=	301,
	ERR_REWARD_DB_BUSY,
	ERR_REWARD_DB_ERR,
	ERR_REWARD_POINTS_INADEQUATE,
	ERR_REWARD_NOT_ACTIVE,
	ERR_REWARD_INVALID_TYPE,
	ERR_REWARD_EXCHANGE_REACH_UPLIMIT,
};

enum REWARD_LIMIT
{
	REWARD_NUM_PER_PAGE	= 	15,
};

enum REF_LIMIT
{
	REF_LIMIT_REFERRERLEVEL   = 20,
        REF_LIMIT_REFERRALPERPAGE = 15,
	REF_LIMIT_REPUTATION      = 20,	
};

enum MAIL_LIMIT
{
	PLAYER_MAIL_LIMIT	= 32,
	SYS_MAIL_LIMIT		= 64, //������ҷ������ʼ���������Ϊ��ϵͳ�ʼ� ������Ϊ64
};

enum		//����Ƶ��,��gsp_if.h�ж����˳����ͬ
{
	GN_CHAT_CHANNEL_LOCAL = 0,		//��ͨƵ��
	GN_CHAT_CHANNEL_FARCRY ,		//����Ƶ��
	GN_CHAT_CHANNEL_TEAM ,			//����Ƶ��
	GN_CHAT_CHANNEL_FACTION ,		//����Ƶ��
	GN_CHAT_CHANNEL_WHISPER ,		//������Ϣ
	GN_CHAT_CHANNEL_DAMAGE,			//�˺���Ϣ
	GN_CHAT_CHANNEL_FIGHT,			//ս����Ϣ
	GN_CHAT_CHANNEL_TRADE ,			//����Ƶ��
	GN_CHAT_CHANNEL_SYSTEM,			//ϵͳ��Ϣ
	GN_CHAT_CHANNEL_BROADCAST,		//�㲥��Ϣ
	GN_CHAT_CHANNEL_MISC,			//������Ϣ
	GN_CHAT_CHANNEL_INSTANCE,		//����Ƶ��
	GN_CHAT_CHANNEL_SUPERFARCRY,	//��������Ƶ��
	GN_CHAT_CHANNEL_BATTLE,			//ս������
	GN_CHAT_CHANNEL_COUNTRY,		//����
	GN_CHAT_CHANNEL_GLOBAL,			//ȫ��
	GN_CHAT_CHANNEL_ALLIANCE,		//= 16	--���ݧڧѧߧ�
	GN_CHAT_CHANNEL_CENTER,			//= 17	--�� ��֧ߧ��� ��ܧ�ѧߧ�
};

enum ERR_WEBTRADE
{
	ERR_WT_UNOPEN			=	311,
	ERR_WT_SN_EXHAUSE,
	ERR_WT_CANNOT_POST,
	ERR_WT_CANNOT_CANCELPOST,
	ERR_WT_NOT_ENOUGH_DEPOSIT,
	ERR_WT_ILLEGAL_SELL_PERIOD,
	ERR_WT_ENTRY_NOT_FOUND,
	ERR_WT_ENTRY_IS_BUSY,
	ERR_WT_SN_ROLEID_MISMATCH,
	ERR_WT_TIMESTAMP_MISMATCH,	
	ERR_WT_ENTRY_HAS_BEEN_SOLD,
	ERR_WT_BUYER_NOT_EXIST,
	ERR_WT_SN_USERID_MISMATCH,
	ERR_WT_ILLEGAL_SELL_PRICE,
	ERR_WT_TOO_MANY_ATTEND_SELL,
	ERR_WT_MAILBOX_FULL,
	ERR_WT_BUYER_STATUS_INAPPROPRIATE,
	ERR_WT_BUYER_NOT_EXIST_2,
	ERR_WT_SELLER_NOT_EXIST,
	ERR_WT_BUYER_CONDITION_NOT_MATCH,
	ERR_WT_HAS_ATTEND_SELL,
	ERR_WT_USER_OTHER_ROLE_ON_SALE,
};

enum ERR_SYSAUCTION
{
	ERR_SA_ENTRY_NOT_FOUND = 341,
	ERR_SA_ENTRY_IS_BUSY,
	ERR_SA_CANNOT_BID,
	ERR_SA_LOW_BIDPRICE,
	ERR_SA_BID_FAILED,
	ERR_SA_USER_NOT_FOUND,
	ERR_SA_USER_IS_BUSY,
	ERR_SA_CASH_NOT_ENOUGH,
	ERR_SA_CASH_OVERFLOW,
	ERR_SA_CASH_NOT_ENOUGH_FOR_TRANSFER,
};

enum ERR_FACTIONFORTRESS
{
	ERR_FF_UNOPEN = 361,
	ERR_FF_FORTRESS_ALREADY_EXIST,
	ERR_FF_FORTRESS_NOT_EXIST, 
	ERR_FF_FORTRESS_DESTROYED,
	ERR_FF_CANNOT_CHALLENGE,
	ERR_FF_TARGET_ALREADY_CHALLENGED,
	ERR_FF_FORTRESS_IN_SYNC,
};

enum ERR_FACTIONRELATION
{
	ERR_FR_LEVEL_NOT_MATCH = 381,
	ERR_FR_OP_IN_COOLDOWN,
	ERR_FR_OP_DENY_IN_CUR_RELATION,
	ERR_FR_NO_MATCH_APPLY,
};

enum ERR_COUPON
{
	ERR_COUPON_NOT_ENOUGH = 401,
	ERR_COUPON_EXCHANGE_ONGOING,
	ERR_COUPON_EXCHANGE_DAYLIMIT,
	ERR_COUPON_EXCHANGE_SERVER_FORBID,
	ERR_COUPON_EXCHANGE_NOT_ACTIVE,
	ERR_COUPON_EXCHANGE_TOO_OFTEN,
};

enum ERR_CASH
{
	ERR_CASH_CARD_NUM_PASSWD_WRONG = 411,
	ERR_CASH_CARD_EXPIRED,
	ERR_CASH_CARD_USED_TOO_OFTEN,
	ERR_CASH_ACCOUNT_BUSY,
};

enum
{
	ERR_AUMAIL_TENCD = 421 , //10��CD
	ERR_AUMAIL_FIVECD,//5������
	ERR_AUMAIL_NOTFOUND ,//û���ҵ�����
	ERR_AUMAIL_NOTSOLONG,//�뿪ʱ�䲻����
	ERR_AUMAIL_LEVELLOW,//�ȼ�����
	ERR_AUMAIL_NOTENABLE,//�ٻع���δ��
};

enum __MAIL_SENDER_TYPE		//��rpc/macros.h�µ�MAIL_SENDER_TYPEһ��
{
	_MST_WEBTRADE = 5,
	_MST_SYSAUCTION = 6,
	_MST_COUNTRYBATTLE = 7,
	_MST_PLAYERPRESENT = 8,
	_MST_PLAYERSHOP = 9,
	_MST_TANKBATTLE = 10,
	_MST_ANTICHEAT = 11,
    _MST_FACTIONRESOURCEBATTLE = 12,
	_MST_FRIENDCALLBACK = 13,
	_MST_MNFBATTLE = 14,
};

enum MASS_MAIL_TYPE
{
	MMT_FACTION,

	MMT_TYPECOUNT,
};

enum WEBTRADE_INFORM
{
	WEBTRADE_CANCEL_POST,		
	WEBTRADE_POST_EXPIRE,
	WEBTRADE_SOLD,
	WEBTRADE_BUY,
	WEBTRADE_POST_FAILED,
};

enum SYSAUCTION_INFORM
{
	SYSAUCTION_BID_SUCCESS,
};

enum COUNTRYBATTLE_INFORM
{
	COUNTRYBATTLE_BONUS,
	COUNTRYBATTLE_NO_BONUS,
};

enum TANKBATTLE_INFORM
{
	TANKBATTLE_BONUS,
	TANKBATTLE_NO_BONUS,
};

enum FACTIONRESOURCEBATTLE_INFORM
{
	FACTIONRESOURCEBATTLE_BONUS,
	FACTIONRESOURCEBATTLE_NO_BONUS,
};

enum PLAYERPRESENT_INFORM
{
	PLAYERPRESENT_GIVE,
	PLAYERPRESENT_ASK,
};

enum PLAYERSHOP_INFORM
{
	PLAYERSHOP_TIMEOUT,			//���̹���
	PLAYERSHOP_DELETED,			//ɾ������
	PLAYERSHOP_RETURN_ITEM,		//���ڷ�����Ʒ
	PLAYERSHOP_RETURN_MONEY,	//���ڷ��ؽ�Ǯ
};

enum PLAYERREQUITE_INFORM
{
	PLAYERREQUITE_CALL,
	PLAYERREQUITE_ANSWER,
};

enum
{
	//��Ҫ��db_if.h �е�DBMASK_SERVERDATA_*����һ��
	CDBMASK_SERVERDATA_WEDDING	= 0x00000001,
	CDBMASK_SERVERDATA_DPSRANK	= 0x00000002,
	CDBMASK_SERVERDATA_ALL		= 0x00000003,

};

enum
{
	LINK_TYPE_IWEB,		//iweb��link
	LINK_TYPE_PC,		//pc�����ӵ�link
	LINK_TYPE_PHONE,	//�ֻ������ӵ�link
};

#define FACTION_FORTRESS_CHALLENGE_FEE  5000000

enum FACTIONRELATION_APPLY_TYPE
{
	ALLIANCE_FROM_OTHER = 0,	//�����˷�����ͬ������
	ALLIANCE_TO_OTHER,			//���������˵�ͬ������
	HOSTILE_FROM_OTHER,			//�����˷����ĵж�����
	HOSTILE_TO_OTHER,			//���������˵ĵж�����
	REMOVE_RELATION_FROM_OTHER,	//�����˷����Ľ����ϵ����
	REMOVE_RELATION_TO_OTHER	//���������˵Ľ����ϵ����
};

enum FACTIONRELATION_TYPE
{
	NONE_RELATION = 0,		//�޹�ϵ
	ALLIANCE,				//�Ѿ���ͬ��
	HOSTILE,				//�Ѿ��ǵж�
	ALLIANCE_HALF,			//һ��������ͬ�����룬��һ����δ�ظ�
	HOSTILE_HALF,			//һ�������˵ж����룬��һ����δ�ظ�
	REMOVE_RELATION_HALF,	//һ���ѽ����˽����ϵ����,��һ����δ�ظ�
};

enum ERR_MISC
{
	ERR_PRP_RESETPOS_OK 			= 431,  //λ��������� �����ٴε�¼
	ERR_MATRIXFAILURE_PHONETOKEN 	= 432,	//�ֻ�������֤ʧ��
	ERR_OBSOLETE_NAME 				= 433,	//�ͻ��˸������ֲ��ҽ�ɫid��ʱ���͵��Ƿ���������
	ERR_FC_DELAYEXPEL = 434, 		//�����ԱԤɾ��
	ERR_FC_ALREADY_DELAYEXPEL = 435, 		//�����ԱԤɾ����
	ERR_UNIQUE_CLASH = 436,         // Ψһֵ���ó�ͻ 
	ERR_UNIQUE_TYPE_INVALID = 437,  // Ψһֵ���ʹ���
	ERR_UNIQUE_VERSION_FAIL = 438,  // Ψһֵ�汾����
	ERR_NOT_RECALL_USER = 439,		// û���ڻ�����������ɫ���ʸ�
	ERR_RECALL_ROLE_EXCEED = 440,	// �ڻ�����ڼ䣬����������ɫ����������(Ŀǰ���ܴ���һ����ɫ)
	ERR_FC_PVP_OPERATE_RESTRICTED = 441,	// ����PVP��ڲ�������
};

enum ALGORITHM
{
	ALGORITHM_MASK_OPT    		= 0x0000FF00,
	ALGORITHM_OPT_NOCACHE		= 0x00000100,	//��Ҫ�����ʺ�����,���ڵ绰�ܱ��û�
	ALGORITHM_PASSWORD_OBSOLETE	= 0x00000200,	//���볤��δ����,����������Ҹ�����
	ALGORITHM_CARD_OBSOLETE		= 0x00000400,	//�ܱ�������δ����
	ALGORITHM_GM_ACCOUNT		= 0x00000800,	//����Ա�ʺ�
	ALGORITHM_SSO				= 0x00001000,	//single sign on�����¼
	ALGORITHM_MASK_HIGH 		= 0xFFFF0000,
	ALGORITHM_NONE        		= 0x0,
	ALGORITHM_CARD 				= 0x00010000,	//�ܱ���
	ALGORITHM_HANDSET 			= 0x00020000,	//�ֻ��ܱ�
	ALGORITHM_USBKEY			= 0x00030000,	//һ�����
	ALGORITHM_PHONE				= 0x00040000,	//�绰�ܱ�
	ALGORITHM_USBKEY2			= 0x00050000,	//�������
	ALGORITHM_PHONETOKEN		= 0x00060000,	//�ֻ�����
	
	ALGORITHM_MISSING_ACCOUNTINFO1 	= 0x00000001,		//��Ҫ��������֤����
	ALGORITHM_MISSING_ACCOUNTINFO2 	= 0x00000002,		//��Ҫ�����˺���������
	ALGORITHM_FILLIN_ACCOUNTINFO1_REWARD = 0x00000010,	//��������֤�����Ľ���
	ALGORITHM_FILLIN_ACCOUNTINFO2_REWARD = 0x00000020,	//�����˺���������Ľ���
};

#define _SECONDS_ONE_DAY 86400
enum SYNMUTADATAMASK
{
	SYNMUTADATAMASK_NONE 	= 0x0,
	SYNMUTADATAMASK_LEVEL	= 0x00000001,
};

//���ڹ�����Ʒɨ��ļ���key
const unsigned char arc4_key_buf[128] = {0xbf, 0x61, 0x09, 0x72, 0x1d, 0xbb, 0x6e, 0xe9, 0x9a, 0x01, 0x3d, 0x47, 0x0a, 0x11, 0x60, 0xbe, 
	0xf8, 0x48, 0x36, 0x3f, 0xdf, 0xf5, 0x9a, 0xc5, 0xca, 0x1c, 0x79, 0xcb, 0xbb, 0x90, 0x96, 0xde, 
	0x58, 0xff, 0x60, 0xbe, 0x8d, 0x4e, 0xd8, 0xe1, 0x83, 0x47, 0xd0, 0x57, 0x6d, 0x62, 0x49, 0x66, 
	0x71, 0x0d, 0x6a, 0xfe, 0x3b, 0xdc, 0xd2, 0x33, 0xd7, 0x8f, 0x5c, 0x5e, 0x4b, 0x86, 0x25, 0xd0, 
	0x6f, 0xd2, 0xd0, 0x8c, 0x53, 0x46, 0xd0, 0xc4, 0x3a, 0x10, 0xae, 0xcf, 0x75, 0xcd, 0xfd, 0x74, 
	0xe8, 0xfe, 0x80, 0x0b, 0x14, 0xf9, 0x94, 0x66, 0xcc, 0x19, 0x7d, 0xda, 0x89, 0x27, 0x91, 0xab, 
	0x5c, 0x73, 0xb0, 0x78, 0xc9, 0x1c, 0x48, 0xd9, 0xe2, 0xc3, 0xfc, 0xfa, 0x3e, 0x74, 0xb9, 0xec, 
	0x81, 0x66, 0x5f, 0x5b, 0x3e, 0x73, 0x1e, 0xd2, 0x31, 0x99, 0xc7, 0xb5, 0xe7, 0x3e, 0xbc, 0x02,}; 

//��ҵ�����غ�
#define	PSHOP_BLIST_CAP				20				//�չ�������
#define	PSHOP_SLIST_CAP				20				//����������
#define	PSHOP_STORE_CAP				40				//�ֿ�����
#define PSHOP_MONEY_CAP				2000000000u		//���̽�Ǯ����
#define PSHOP_YINPIAO_CAP			10000u			//������Ʊ����
#define PSHOP_YP_LIST_CAP			10				//��Ʊ������
#define PSHOP_DELETE_DELAY			7*24*3600u		//�ӳ�ɾ��ʱ�� 7��
#define PSHOP_ITEM_PRICE_LIMIT		4000000000u		//��Ʒ��������
#define PSHOP_PAGE_SIZE				80u				//ÿҳ��ʾ����
#define WANMEI_YINPIAO_PRICE		10000000u		//������Ʊ����
#define WANMEI_YINPIAO_PILELIMIT	1000u			//��Ʊ�ѵ�����
#define WANMEI_YINPIAO_ID			21652u			//��Ʊ��ƷID
#define PSHOP_CONSUME_ITEM_1		39176u			//��������������Ʒtype1
#define PSHOP_CONSUME_ITEM_2		39177u			//��������������Ʒtype2
#define PSHOP_CONSUME_ITEM_3		40065u			//��������������Ʒtype3

//��ҵ���״̬
enum PLAYER_SHOP_STATUS
{
	PSHOP_STATUS_NORMAL,
	PSHOP_STATUS_EXPIRED,
};

enum		//ԭʼ������../gdbclient/db_if.h �뱣��һ��
{
	GROLE_STATUS_EXTRAPROP_TOUCH_HALF_TRADE = 0, // Touch
	GROLE_STATUS_EXTRAPROP_DAILY_SIGN_IN,
	GROLE_STATUS_EXTRAPROP_GIFTCARD_HALF_REDEEM, // ��Ʒ��һ�
	GROLE_STATUS_EXTRAPROP_LEADERSHIP,
	GROLE_STATUS_EXTRAPROP_GENERALCARD_COLLECTION,
	GROLE_STATUS_EXTRAPROP_FATERING,
	GROLE_STATUS_EXTRAPROP_CLOCK_DATA,
	GROLE_STATUS_EXTRAPROP_RAND_MALL_DATA,
	GROLE_STATUS_EXTRAPROP_WORLD_CONTRIBUTION,
	GROLE_STATUS_EXTRAPROP_ASTROLABE_EXTERN,
	GROLE_STATUS_EXTRAPROP_SOLO_CHALLENGE_INFO,     // 10
	GROLE_STATUS_EXTRAPROP_MNFACTION_INFO,
	GROLE_STATUS_EXTRAPROP_VISA_INFO,
	GROLE_STATUS_EXTRAPROP_FIX_POSITION_TRANSMIT_INFO,
    GROLE_STATUS_EXTRAPROP_CASH_RESURRECT_INFO,

	GROLE_STATUS_EXTRAPROP_COUNT, // ������������
};

#define CARNIVAL_DOOR_UNKEY_BEG 8000
#define CARNIVAL_COUNT_UNKEY_BEG 8500

enum CARNIVAL_TYPE
{
	CT_TYPE_BEG,

	CT_COUNTRY_BATTLE = CT_TYPE_BEG,
	CT_UNCK_TANK_BATTLE,
	CT_MNFACTION_BATTLE,
	CT_ARENA_OF_AURORA_BLUE,
	CT_ARENA_OF_AURORA_RED,

	CT_TYPE_END
};

} // namespace GNET

#define PWRD_SILVER_NOTE_ID 21652

enum
{
	SSO_GET_TICKET_EXTRA_INFO_NONE 	= 0,	//����Ҫ������Ϣ
	SSO_GET_TICKET_EXTRA_INFO1 		= 1,	//��Ҫ������Ϣ1(�ȼ�����������)
};

#define  _ROLE_STATUS_CROSS_LOCKED 5 //���ڿ������ɫ����
#define  _ROLE_STATUS_CROSS_RECYCLE 6 //�뿪�������ɫ����(�������ֱ��ɾ����ɫ)

enum
{
	KICKOUT_LOCAL = 1,
	KICKOUT_REMOTE = 2,
};

enum
{	
	DS_TO_CENTRALDS = 1,
	CENTRALDS_TO_DS = 2,
	DIRECT_TO_CENTRALDS = 3,
	MNFACTION_BATTLE = 4,
	ARENA_OF_AURORA = 5,
	//DIRECT_TO_RAID = 4,//direct login raid line
};

enum
{
	FC_DELAYEXPEL_EXECUTE = 1,
	FC_DELAYEXPEL_CANCEL = 2,
	FC_DELAYEXPEL_TIMEOUT = 3,
};

enum ErrCode_Local
{	
	ERR_IDENTITY_TIMEOUT = 451, //��� ��֤�볬ʱ
	ERR_IDENTITY_NOTMATCH = 452, //��� ��֤�벻ƥ��	
	ERR_REMOTE_VERIFYFAILED = 453, //��� �����������ݴ���	
	ERR_CDS_COMMUNICATION = 454, //�������������ά��
	ERR_REMOTE_DATA_INVALID = 455, //��� ��������쳣
};

enum ErrCode_Rename
{
	ERR_PR_OUTOFSERVICE	= 461,	//���Ѹ������񲻿���
	ERR_PR_DUPLICATE	= 462,	//���Ѹ���ʱ�������Ѿ�����
	ERR_PR_VALIDNAME	= 463,	//���Ѹ���ʱ�����ְ���������

    ERR_PR_PROFILE      = 464,  //�������Եϵͳ����д��ƥ����Ϣ
};

enum ERR_KINGELECTION
{
	ERR_KE_CANNOT_APPLY_CANDIDATE 	= 471,
	ERR_KE_CANDIDATE_APPLY_EXCEED_UPPER_LIMIT,
	ERR_KE_ALREADY_APPLY_CANDIDATE,
	ERR_KE_CANNOT_VOTE,
	ERR_KE_CANDIDATE_NOT_EXIST,
};

enum ERR_PRESENT
{
	ERR_GP_OUTOFSERVICE	= 481,				//����������񲻿���
	ERR_GP_MAIL_ERR = 482,					//������ȡ�ʼ����������ظ�
	ERR_GP_GIVE_TARGET_MAILBOX_FULL	= 483,	//���Ͷ������������
	ERR_GP_ASK_TARGET_MAILBOX_FULL = 484,	//��ȡ�������������
	ERR_GP_ASK_TARGET_TOO_MUCH = 485,		//�Ѿ�����ȡ������ȡ����3����
};

enum ErrCode_PlayerShop
{
	ERR_PLAYERSHOP_VERIFY_FAILED	= 500,	//������֤ʧ��
	ERR_SHOPMARKET_NOT_INIT			= 501,	//����������δ��DB�������
	ERR_PLAYERSHOP_NOTFIND			= 502,	//���̲�����
	ERR_PLAYERSHOP_EXIST			= 503,	//�����Ѵ���
	ERR_PLAYERSHOP_BLIST_FULL		= 504,	//�����չ�����
	ERR_PLAYERSHOP_SLIST_FULL		= 505,	//���̳�������
	ERR_PLAYERSHOP_STORE_FULL		= 507,	//���ֿ̲���
	ERR_PLAYERSHOP_MONEY_FULL		= 508,	//���̽�Ǯ��
	ERR_PLAYERSHOP_NO_MONEY			= 509,	//���̽�Ǯ����
	ERR_PLAYERSHOP_EXPIRED			= 510,	//�����ѹ���
	ERR_PLAYERSHOP_BUSY				= 511,	//����æ(���ڱ�����)
	ERR_PLAYERSHOP_ITEM_NOTFOUND	= 512,	//��Ʒ������
	ERR_PLAYERSHOP_ITEM_NOTMATCH	= 513,	//��Ʒ��ƥ��
};

enum ERR_TANKBATTLE
{
	ERR_TANK_BATTLE_NOT_OPEN		= 490,	//ս��ս����û�п���
	ERR_TANK_BATTLE_ALREADY_APPLY	= 491,	//�Ѿ������ս����
	ERR_TANK_BATTLE_SWITCH_TIMEOUT	= 492,	//��תʧ�ܣ����±���
	ERR_TANK_BATTLE_NOT_APPLY_MAP	= 493,	//����֮����ת��ͼ�ˣ���Ҫ���±���
};

enum UNIQUE_DATA_TYPE
{
	UDT_INVALID,
	UDT_INT,
	UDT_DOUBLE,
	UDT_OCTETS
};

enum
{
	CNET_CONFIG_COUNTRYBATTLE_BONUS = 0,
	CNET_CONFIG_MAX,
};

enum MNFACTION_TIME_ADJUST
{
	//cds
	TIME_ADJUST_FILTRATE_END_TO_BEGIN = 10*60,	//����ȫ�����������ʱ��� ���ʱ��������ķ�ͣ����ȫ���ı�����û��ѡ�У����˿�
	TIME_ADJUST_BONUS_TO_BATTLEEND = 15*60, 	//ս������ʱ�䵽����ʱ�䣬��Ϊ��Ҫ֪������ս��������ܷ���
	TIME_ADJUST_CLOSE_TO_BONUS = 10*60,			//�����������,���ʱ����������

	//cdc
	TIME_ADJUST_FETCH_APPLYRES_TO_APPLYEND = TIME_ADJUST_FILTRATE_END_TO_BEGIN + 10*60, 			//ԭ����ȡ���ķ�ɸѡ������������ʱ��
	TIME_ADJUST_CLOSE_TO_BATTLEEND = TIME_ADJUST_BONUS_TO_BATTLEEND + TIME_ADJUST_CLOSE_TO_BONUS, 	//����� �� ս������
};

enum ERR_MNFACTION_SERVER //�������ս(�������ڲ�)  600-629
{
	ERR_MNF_CROSS_MAXMUM = 601,				//�����ɽ�����������̫��
	ERR_MNF_WRONG_ARGUMENT = 602,			//����Ĳ�������
	ERR_MNF_MULTI_DOMAIN = 603,				//һ����ɫֻ�ܽ���һ��ս��
	ERR_MNF_INVITE_COUNT_PERDOMAIN_MAXMUM = 604,	//����ս���Ľ�ɫ̫��
	ERR_MNF_BONUS_NOT_FOUND = 605,			//�������ɾ�û�л�ý���
	ERR_MNF_DATA_NOT_FOUND = 606,			//����û�ҵ�
};

enum ERR_MNFACTION_CLIENT //�����ս(֪ͨ�ͻ���) 630-650 
{
	ERR_MNF_PLAYER_NOTIN_CENTRAL = 630,		//��Ҳ������ķ�	
	ERR_MNF_PLAYER_IN_CENTRAL = 631,		//��������ķ�	
	ERR_MNF_PLAYER_NOT_FOUND = 632,			//û���ҵ������Ϣ
	ERR_MNF_FACTION_NOT_FOUND = 633,		//û���ҵ�������Ϣ
	ERR_MNF_DOMAIN_NOTENOUGH = 634,			//���������Ŀ����	
	ERR_MNF_CROSS_DOMAIN_NOTENOUGH = 635,	//��������Ŀ����
	ERR_MNF_WRONG_TIME = 636,				//�û�п���
	ERR_MNF_APPLY_TYPE = 637,				//����ı�������
	ERR_MNF_APPLY_DUNPLICATE = 638,			//�ظ�����
	ERR_MNF_FACTION_NOT_INVITED = 639,		//��ǰ���ɲ���������				
	ERR_MNF_APPLY_MONEY_NOT_ENOUGH = 640,	//�����Ѳ���
};

//֪ͨ�ͻ���
enum MNFACTION_MAIL_SENDER 
{       
	SENDER_APPLY_REJECTED_CLIENT, //������ɸѡ��
	SENDER_APPLY_REJECTED_SERVER, //�����ɸѡ��
	SENDER_BATTLE_BONUS_MASTER,		//��������
	SENDER_BATTLE_BONUS_A,			//����A
	SENDER_BATTLE_BONUS_B,			//����B
	SENDER_BATTLE_BONUS_C,			//����C
}; 

#endif
