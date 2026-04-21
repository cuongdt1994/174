#ifndef _ATASK_TEMPL_H_
#define _ATASK_TEMPL_H_

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <time.h>
#if defined(WIN32) || defined(_LUA_TASKTEST)
#pragma warning(disable:4786)
#endif
#include <set>
using namespace std;

#ifdef WIN32
	#include "ExpTypes.h"
#elif defined LINUX
	#include "../template/exptypes.h"
#else
	#include "exptypes.h"
#endif

#include "TaskInterface.h"
#include "TaskProcess.h"
#include "TaskExpAnalyser.h"

#define	MAX_TASK_NAME_LEN		30      
#define MAX_AWARD_NPC_NUM		8
#define MAX_PREM_TASK_COUNT		20
#define MAX_MUTEX_TASK_COUNT	5
#define MAX_TEAM_MEM_WANTED		MAX_OCCUPATIONS
#define MAX_TIMETABLE_SIZE		24

#define MAX_AWARD_SCALES		6
#define MAX_AWARD_CARRIER_HP_SCALES	5
#define MAX_AWARD_CANDIDATES	12
#define MAX_AWARD_VAR_SCALES	16
#define MAX_LIVING_SKILLS		4
#define MAX_CONTRIB_MONSTERS	100		// PQ�������׶�����������	
#define MAX_AWARD_PQ_RANKING	32		// PQ�����������������
#define MAX_TITLE_NUM			10

#define MAX_TASKREGION			8 // ����������

#define TASK_GENDER_NONE		0
#define TASK_GENDER_MALE		1
#define TASK_GENDER_FEMALE		2

#define TASK_MAX_PATH 260

#define INVALID_VAL	((unsigned int) -1)

#define TASK_MAX_LINE_LEN TASK_MAX_PATH

#define TASK_MAX_VALID_COUNT	6

#define TASK_TREASURE_MAP_SIDE_MULTIPLE	30    //�ر�ͼ�߳���С����߳��ı�

#define MAX_COMBINED_SWITCHES 20
#define MAX_NEEDED_TASK_COUNT 5

// Ŀǰ�����ֽ����������˺Ų��
static const int NUM_SPECIAL_AWARD = 2;
// ��������id
static const int TASK_SPECIAL_AWARD[NUM_SPECIAL_AWARD] =  {26969,26970};

enum DynTaskType
{
	enumDTTNone = 0,
	enumDTTSpecialAward,
	enumDTTNormal,
	enumDTTGiftCard,
};

struct NPC_INFO
{
	unsigned int id;
	short x;
	short y;
	short z;
};

/*
 *	Server Notifications
 */

#define TASK_SVR_NOTIFY_NEW				1	// �����񷢷�
#define TASK_SVR_NOTIFY_COMPLETE		2	// �������
#define TASK_SVR_NOTIFY_GIVE_UP			3	// �������
#define TASK_SVR_NOTIFY_MONSTER_KILLED	4	// ɱ������
#define TASK_SVR_NOTIFY_FINISHED		5	// ���ڵõ�����״̬
#define TASK_SVR_NOTIFY_ERROR_CODE		6	// ������
#define TASK_SVR_NOTIFY_FORGET_SKILL	7	// ���������
#define TASK_SVR_NOTIFY_DYN_TIME_MARK	8	// ��̬����ʱ����
#define TASK_SVR_NOTIFY_DYN_DATA		9	// ��̬��������
#define TASK_SVR_NOTIFY_SPECIAL_AWARD	10	// ���⽱����Ϣ
#define	TASK_SVR_NOTIFY_STORAGE			11	// �ֿ�����
#define TASK_SVR_NOTIFY_DIS_GLOBAL_VAL  12  // ��ʾȫ�ֱ���
#define TASK_SVR_NOTIFY_TREASURE_MAP	13  // �ر�λ��
#define TASK_SVR_NOTIFY_SET_TASK_LIMIT  14	// ���������б�����
#define TASK_SVR_NOTIFY_PLAYER_KILLED	15  // ɱ������


#define EVENT_DATA_SVR_NOTIFY_TASK	16 // Novas missões de anedota


/*
 *	Client Notifications
 */

#define TASK_CLT_NOTIFY_CHECK_FINISH	1
#define TASK_CLT_NOTIFY_CHECK_GIVEUP	2
#define TASK_CLT_NOTIFY_REACH_SITE		3
#define TASK_CLT_NOTIFY_AUTO_DELV		4
#define TASK_CLT_NOTIFY_MANUAL_TRIG		5
#define TASK_CLT_NOTIFY_FORCE_GIVEUP	6
#define TASK_CLT_NOTIFY_DYN_TIMEMARK	7
#define TASK_CLT_NOTIFY_DYN_DATA		8
#define TASK_CLT_NOTIFY_SPECIAL_AWARD	9
#define TASK_CLT_NOTIFY_LEAVE_SITE		10
#define TASK_CLT_NOTIFY_PQ_CHECK_INIT	11
#define	TASK_CLT_NOTIFY_STORAGE			12
#define TASK_CLT_NOTIFY_REQUEST_TREASURE_INDEX 14
#define TASK_CLT_NOTIFY_15DAYS_NOLOGIN	15
#define TASK_CLT_NOTIFY_SPECIAL_AWARD_MASK 16
#define TASK_CLT_NOTIFY_TITLE_TASK 17
#define TASK_CLT_NOTIFY_CHOOSE_AWARD 18
#define TASK_CLT_NOTIFY_BUY_TOKENSHOP_ITEM 20
#define TASK_CLT_NOTIFY_FINISH_TASK_BY_WORLD_CONTRIBUTION 21
#define	TASK_CLT_NOTIFY_RM_FINISH_TASK	150

/*
 *	Player Notifications
 */

#define TASK_PLY_NOTIFY_NEW_MEM_TASK	1	// ֪ͨ���ն�Ա����
#define TASK_PLY_NOTIFY_FORCE_FAIL		2	// ȫ��ʧ��
#define TASK_PLY_NOTIFY_FORCE_SUCC		3	// ȫ�ӳɹ�

/*
 *	Global Data Reason
 */

#define TASK_GLOBAL_CHECK_RCV_NUM		1
#define TASK_GLOBAL_CHECK_COTASK		2
#define TASK_GLOBAL_CHECK_ADD_MEM		3
#define TASK_GLOBAL_NPC_KILLED_TIME		4
#define TASK_GLOBAL_ADD_TIME_MARK		5
#define TASK_GLOBAL_DEL_TIME_MARK		6
#define TASK_GLOBAL_PROTECT_NPC			7

/*
 *	Cotask Condition
 */

#define COTASK_CORRESPOND				0
#define COTASK_ONCE						1

/*
 *	��������
 */
enum ENUM_TASK_TYPE
{
	enumTTDaily = 100,			// ÿ��
	enumTTLevel2,				// ����
	enumTTMajor,				// ����
	enumTTBranch,				// ֧��
	enumTTEvent,				// �
	enumTTQiShaList,			// ��ɱ��
	enumTTFaction,				// ����
	enumTTFunction,				// ��Ӫ
	enumTTLegend,				// ����
	enumTTQuestion,				// ����
	enumTTHome,
	enumTTEnd,
};

/* ��ɷ�ʽ */
enum
{
	enumTMNone = 0,					// ��
	enumTMKillNumMonster,			// ɱ������
	enumTMCollectNumArticle,		// �����������
	enumTMTalkToNPC,				// ���ض�NPC�Ի�
	enumTMReachSite,				// �����ض��ص�
	enumTMWaitTime,					// �ȴ��ض�ʱ��
	enumTMAnswerQuestion,			// ѡ���ʴ�
	enumTMTinyGame,					// С��Ϸ
	enumTMProtectNPC,				// �����ض�NPC
	enumTMNPCReachSite,				// NPC�����ض��ص�
	enumTMGlobalValOK,              // ȫ�ֱ�����������
	enumTMLeaveSite,				// �뿪�ض��ص�
	enumTMReachTreasureZone,		// �ﵽ�ر�����
	enumTMKillPlayer,				// ɱ�����
	enumTMTransform,				// ����״̬
	enumTMReachLevel,				// ���ȼ�����ͨ�ȼ���ת������������ȼ�
	enumTMSimpleClientTask,			// ������ֻ���ͻ�����֤��Ŀǰֻ��֤���鶯��
	enumTMSimpleClientTaskForceNavi,// ǿ���ƶ�
	enumTMHasIconStateID,			// ��֤״̬ͼ��ID
	enumTMReachHomeLevel,
	enumTMReachHomeFlourish,
	enumTMHomeItem,
	enumTMBindCarrierId,
	enumTMArenaTime,
	enumTMArenaWinCount,
	enumTMCompleteTaskEvent,
	enumTMReachFactionPersonScore,
	enumTMUseItems,
	enumTMBeingInTeam,
	enumTMReachGrowthTarget,
	enumTMOpenDialog,
	enumTMSuccTriggerEvent,
	enumTMVerifyStateLayer,
	enumTMKeyWorld,
	enumTMFinPersonKeyValue,
	enumTMHistoryKeyValue,
	enumTMGuess,
};

/* ������� */
enum
{
	enumTFTDirect = 0,		// ֱ�����
	enumTFTNPC,
	enumTFTConfirm,			// ��Ҫȷ�����
};

/* ������ʽ */
enum
{
	enumTATNormal = 0,		// ��ͨ
	enumTATEach,			// ��ɱ����Ŀ�������Ʒ��
	enumTATRatio,			// ��ʱ�����
	enumTATItemCount		// �������Ʒ�����ֲ�ͬ����
};

/* �����ظ���� */
enum
{
	enumTAFNormal = 0,		// ��ͨ
	enumTAFEachDay,			// ÿ��
	enumTAFEachWeek,		// ÿ��
	enumTAFEachMonth,		// ÿ��
	enumTAFEachYear			// ÿ��
};

enum ENUM_HOME_LEVEL_TYPE
{
  enumHomeLevel = 0x0,
  enumHomeFactory = 0x1,
  enumHomeBuy = 0x2,
  enumHomeStone = 0x3,
  enumHomeWood = 0x4,
  enumHomeIron = 0x5,
  enumHomeCloth = 0x6,
  enumHomeFarm = 0x7,
  enumHomeAnimal = 0x8
};
#pragma pack(1)

#define TASK_PACK_MAGIC		0x93858361

struct TASK_PACK_HEADER
{
	unsigned int	magic;
	unsigned int	version;
	unsigned int	item_count;
};

//-------------------162----------------------
struct FACTION_TARGET_DATA
{
	int FactionTargetId;
	int FactionNum;
};
struct SUBMIT_RANK_INFO
{
	int Param1;
	int Param2;
	int Param3;
};
//--------------------------------------------
struct ZONE_VERT
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};

		float v[3];
	};

	bool operator == (const ZONE_VERT& src) const
	{
		return (v[0] == src.v[0] && v[1] == src.v[1] && v[2] == src.v[2]);
	}

	bool great_than(const float v[3]) const { return x >= v[0] && y >= v[1] && z >= v[2]; }
	bool less_than (const float v[3]) const { return x <= v[0] && y <= v[1] && z <= v[2]; }
};

inline bool is_in_zone(
	const ZONE_VERT& _min,
	const ZONE_VERT& _max,
	const float pos[3])
{
	return _min.less_than(pos) && _max.great_than(pos);
}

struct PLAYER_WANTED
{
	unsigned int m_ulPlayerNum;
	unsigned int m_ulDropItemId;
	unsigned int m_ulDropItemCount;
	bool		  m_bDropCmnItem;
	float		  m_fDropProb;
	Kill_Player_Requirements m_Requirements;
};
struct MONSTER_WANTED
{
	unsigned int	m_ulMonsterTemplId;
	unsigned int	m_ulMonsterNum;
	unsigned int	m_ulDropItemId;
	unsigned int	m_ulDropItemCount;
	bool			m_bDropCmnItem;
	float			m_fDropProb;
	bool			m_bKillerLev;
	int				m_iDPH;
	int				m_iDPS;

	bool operator == (const MONSTER_WANTED& src) const
	{
		return (m_ulMonsterTemplId == src.m_ulMonsterTemplId && 
				m_ulMonsterNum == src.m_ulMonsterNum && 
				m_ulDropItemId == src.m_ulDropItemId && 
				m_ulDropItemCount == src.m_ulDropItemCount && 
				m_bDropCmnItem == src.m_bDropCmnItem && 
				m_fDropProb == src.m_fDropProb &&
				m_bKillerLev == src.m_bKillerLev);
	}
};

struct ITEM_WANTED
{
	unsigned int	m_ulItemTemplId;
	bool			m_bCommonItem;
	unsigned int	m_ulItemNum;
	float			m_fProb;
	int            m_lPeriod;

	bool operator == (const ITEM_WANTED& src) const
	{
		return (m_ulItemTemplId == src.m_ulItemTemplId && m_bCommonItem == src.m_bCommonItem && m_ulItemNum == src.m_ulItemNum && m_lPeriod == src.m_lPeriod);
	}
};

struct TITLE_AWARD
{
	unsigned int m_ulTitleID;
	int		  m_lPeriod;
	TITLE_AWARD():m_ulTitleID(0), m_lPeriod(0){}
};

struct TEAM_MEM_ITEM_WANTED
{
	unsigned int	m_ulItemTemplId;
	bool			m_bCommonItem;
	unsigned int	m_ulItemNum;
};

struct MONSTERS_SUMMONED
{
	unsigned int	m_ulMonsterTemplId;
	unsigned int	m_ulMonsterNum;
	float			m_fSummonProb;
	int            m_lPeriod;

	bool operator == (const MONSTERS_SUMMONED& src) const
	{
		return (m_ulMonsterTemplId == src.m_ulMonsterTemplId && m_ulMonsterNum == src.m_ulMonsterNum && m_fSummonProb == src.m_fSummonProb && m_lPeriod == src.m_lPeriod);
	}
};

struct MONSTERS_CONTRIB
{
	unsigned int	m_ulMonsterTemplId;		//  ����ID
	int				m_iWholeContrib;		//  ��Ӽ�������׶�
	int				m_iShareContrib;		//	��Ӽ乲�����׶�
	int				m_iPersonalWholeContrib; //	���˶������׶�

	bool operator == (const MONSTERS_CONTRIB& src) const
	{
		return (m_ulMonsterTemplId == src.m_ulMonsterTemplId && m_iWholeContrib == src.m_iWholeContrib && m_iShareContrib == src.m_iShareContrib && m_iPersonalWholeContrib == src.m_iPersonalWholeContrib);
	}

};

struct RANKING_AWARD
{
	unsigned int	m_iRankingStart;
	unsigned int	m_iRankingEnd;
	bool			m_bCommonItem;
	unsigned int	m_ulAwardItemId;
	unsigned int	m_ulAwardItemNum;
	int			m_lPeriod;

	bool operator == (const RANKING_AWARD& src) const
	{
		return (m_iRankingStart == src.m_iRankingStart && m_iRankingEnd == src.m_iRankingEnd && m_bCommonItem == src.m_bCommonItem && m_ulAwardItemId == src.m_ulAwardItemId && m_ulAwardItemNum == src.m_ulAwardItemNum && m_lPeriod == src.m_lPeriod);
	}
};


extern const unsigned int _race_occ_map[MAX_OCCUPATIONS];

struct TEAM_MEM_WANTED
{
	unsigned int	m_ulLevelMin;
	unsigned int	m_ulLevelMax;
	unsigned int	m_ulRace;
	unsigned int	m_ulOccupation;
	unsigned int	m_ulGender;
	unsigned int	m_ulMinCount;
	unsigned int	m_ulMaxCount;
	unsigned int	m_ulTask;
	int				m_iForce;
	//162
	short TeamMemTaskSpace;
// 	unsigned int	m_ulTeamMemsItemWanted;
// 	TEAM_MEM_ITEM_WANTED* m_TeamMemItemWanted; //[MAX_ITEM_WANTED];
// 	bool			m_bJustCheck;

	TEAM_MEM_WANTED(){Init();}

	bool IsMeetBaseInfo(const task_team_member_info* pInfo) const
	{
		if (m_ulLevelMin && pInfo->m_ulLevel < m_ulLevelMin) return false;

		if (m_ulLevelMax && pInfo->m_ulLevel > m_ulLevelMax) return false;

		if (m_ulRace)
		{
			if (pInfo->m_ulOccupation >= SIZE_OF_ARRAY(_race_occ_map)) return false;
			if (_race_occ_map[pInfo->m_ulOccupation] != m_ulRace) return false;
		}
		else if (m_ulOccupation != INVALID_VAL && pInfo->m_ulOccupation != m_ulOccupation)
			return false;

		if (m_iForce)
		{
			if (m_iForce == -1)
			{
				if (pInfo->m_iForce == 0)
					return false;
			}
			else if (pInfo->m_iForce != m_iForce) return false;
		}

#ifndef _TASK_CLIENT
		if (m_ulGender == TASK_GENDER_MALE   && !pInfo->m_bMale
		 || m_ulGender == TASK_GENDER_FEMALE && pInfo->m_bMale)
			return false;
#endif

		return true;
	}

	bool IsMeetCount(unsigned int ulCount) const
	{
		if (m_ulMinCount && ulCount < m_ulMinCount) return false;

		if (m_ulMaxCount && ulCount > m_ulMaxCount) return false;

		return true;
	}

	void Init()
	{
		m_ulLevelMin	= 0;
		m_ulLevelMax	= 0;
		m_ulRace		= 0;
		m_ulOccupation	= INVALID_VAL;
		m_ulGender		= 0;
		m_ulMinCount	= 0;
		m_ulMaxCount	= 0;
		m_ulTask		= 0;
		m_iForce		= 0;
		//162
		TeamMemTaskSpace= 0;
// 		m_ulTeamMemsItemWanted = 0;
// 		m_TeamMemItemWanted = NULL;
//		m_bJustCheck	= false;
	}

	bool IsValid() const
	{
		return !(
			m_ulLevelMin	== 0 &&
			m_ulLevelMax	== 0 &&
			m_ulRace		== 0 &&
			m_ulOccupation	== INVALID_VAL &&
			m_ulGender		== 0 &&
			m_ulMinCount	== 0 &&
			m_ulMaxCount	== 0 &&
			m_ulTask		== 0 // &&
// 			m_ulTeamMemsItemWanted == 0 &&
// 			m_TeamMemItemWanted == NULL &&
//			m_bJustCheck    == false);
			);
	}

	bool operator == (const TEAM_MEM_WANTED& src) const
	{
		return (
			m_ulLevelMin	== src.m_ulLevelMin &&
			m_ulLevelMax	== src.m_ulLevelMax &&
			m_ulRace		== src.m_ulRace &&
			m_ulOccupation	== src.m_ulOccupation &&
			m_ulGender		== src.m_ulGender &&
			m_ulMinCount	== src.m_ulMinCount &&
			m_ulMaxCount	== src.m_ulMaxCount &&
			m_ulTask		== src.m_ulTask// &&
//			m_ulTeamMemsItemWanted == src.m_ulTeamMemsItemWanted &&
//			*m_TeamMemItemWanted == *(src.m_TeamMemItemWanted) &&
//			m_bJustCheck	== src.m_bJustCheck
			);
	}
};

struct AWARD_ITEMS_CAND
{
	AWARD_ITEMS_CAND()
	{
		memset(this, 0, sizeof(*this));

#ifdef TASK_TEMPL_EDITOR
		m_AwardItems = new ITEM_WANTED[MAX_ITEM_AWARD];
		g_ulNewCount++;
#endif
	}
	~AWARD_ITEMS_CAND()
	{
		LOG_DELETE_ARR(m_AwardItems);
	}

	AWARD_ITEMS_CAND& operator = (const AWARD_ITEMS_CAND& src)
	{
		m_ulAwardItems		= src.m_ulAwardItems;
		m_ulAwardCmnItems	= src.m_ulAwardCmnItems;
		m_ulAwardTskItems	= src.m_ulAwardTskItems;
		m_bRandChoose		= src.m_bRandChoose;

#ifndef TASK_TEMPL_EDITOR

		if (m_ulAwardItems)
		{
			m_AwardItems = new ITEM_WANTED[m_ulAwardItems];
			g_ulNewCount++;
		}

#endif

		for (unsigned int i = 0; i < m_ulAwardItems; i++)
			m_AwardItems[i] = src.m_AwardItems[i];

		return *this;
	}

	bool operator == (const AWARD_ITEMS_CAND& src) const
	{
		if (m_ulAwardItems != src.m_ulAwardItems)
		{
			return false;
		}

		for (unsigned int i  = 0;i < m_ulAwardItems;++i)
		{
			if (!(m_AwardItems[i] == src.m_AwardItems[i]))
			{
				return false;
			}
		}
		return (m_ulAwardCmnItems == src.m_ulAwardCmnItems && m_ulAwardTskItems == src.m_ulAwardTskItems && m_bRandChoose == src.m_bRandChoose);
	}

	unsigned int	m_ulAwardItems;
	unsigned int	m_ulAwardCmnItems;
	unsigned int	m_ulAwardTskItems;
	ITEM_WANTED*	m_AwardItems; //[MAX_ITEM_AWARD];
	bool			m_bRandChoose;

	int MarshalBasicData(char* pData)
	{
		char* p = pData;

		*p = m_bRandChoose;
		p++;

		char cnt = (char)m_ulAwardItems;
		*p = cnt;
		p++;

		unsigned int sz = sizeof(ITEM_WANTED) * cnt;
		if (sz)
		{
			memcpy(p, m_AwardItems, sz);
			p += sz;
		}

		return p - pData;
	}

	int UnmarshalBasicData(const char* pData)
	{
		const char* p = pData;

		m_bRandChoose = (*p != 0);
		p++;

		m_ulAwardItems = *p;
		p++;

		if (m_ulAwardItems)
		{
#ifndef TASK_TEMPL_EDITOR
			m_AwardItems = new ITEM_WANTED[m_ulAwardItems];
			g_ulNewCount++;
#endif

			unsigned int sz = sizeof(ITEM_WANTED) * m_ulAwardItems;
			memcpy(m_AwardItems, p, sz);
			p += sz;

			for (unsigned int i = 0; i < m_ulAwardItems; i++)
			{
				if (m_AwardItems[i].m_bCommonItem)
					m_ulAwardCmnItems++;
				else
					m_ulAwardTskItems++;
			}
		}

		return p - pData;
	}
};

struct AWARD_MONSTERS_SUMMONED
{
	AWARD_MONSTERS_SUMMONED()
	{
		memset(this, 0, sizeof(*this));

#ifdef TASK_TEMPL_EDITOR
		m_Monsters = new MONSTERS_SUMMONED[MAX_MONSTER_SUMMONED];
		g_ulNewCount++;
#endif
	}
	~AWARD_MONSTERS_SUMMONED()
	{
		LOG_DELETE_ARR(m_Monsters);
	}

	AWARD_MONSTERS_SUMMONED& operator = (const AWARD_MONSTERS_SUMMONED& src)
	{
		m_ulMonsterNum		= src.m_ulMonsterNum;
		m_bRandChoose		= src.m_bRandChoose;
		m_ulSummonRadius	= src.m_ulSummonRadius;
		m_bDeathDisappear	= src.m_bDeathDisappear;

#ifndef TASK_TEMPL_EDITOR

		if (m_ulMonsterNum)
		{
			m_Monsters = new MONSTERS_SUMMONED[m_ulMonsterNum];
			g_ulNewCount++;
		}

#endif

		for (unsigned int i = 0; i < m_ulMonsterNum; i++)
			m_Monsters[i] = src.m_Monsters[i];

		return *this;
	}

	bool operator == (const AWARD_MONSTERS_SUMMONED& src) const
	{
		if (m_ulMonsterNum != src.m_ulMonsterNum)
		{
			return false;
		}

		for (unsigned int i = 0;i < m_ulMonsterNum;++i)
		{
			if (!(m_Monsters[i] == src.m_Monsters[i]))
			{
				return false;
			}
		}
		
		return ( m_bRandChoose == src.m_bRandChoose && m_ulSummonRadius == src.m_ulSummonRadius && m_bDeathDisappear == src.m_bDeathDisappear);
	}

	unsigned int		m_ulMonsterNum;
	bool				m_bRandChoose;
	unsigned int		m_ulSummonRadius;
	bool				m_bDeathDisappear;
	MONSTERS_SUMMONED*	m_Monsters; //[MAX_MONSTER_SUMMONED];

	int MarshalBasicData(char* pData)
	{
		char* p = pData;

		*p = m_bRandChoose;
		p++;

		*p = (char)m_ulSummonRadius;
		p++;

		char cnt = (char)m_ulMonsterNum;
		*p = cnt;
		p++;
	
		*p = m_bDeathDisappear;
		p++;

		unsigned int sz = sizeof(MONSTERS_SUMMONED) * cnt;
		if (sz)
		{
			memcpy(p, m_Monsters, sz);
			p += sz;
		}

		return p - pData;
	}

	int UnmarshalBasicData(const char* pData)
	{
		const char* p = pData;

		m_bRandChoose = (*p != 0);
		p++;

		m_ulSummonRadius = *p;
		p++;

		m_ulMonsterNum = *p;
		p++;

		m_bDeathDisappear = (*p != 0);
		p++;

		if (m_ulMonsterNum)
		{
#ifndef TASK_TEMPL_EDITOR
			m_Monsters = new MONSTERS_SUMMONED[m_ulMonsterNum];
			g_ulNewCount++;
#endif

			unsigned int sz = sizeof(MONSTERS_SUMMONED) * m_ulMonsterNum;
			memcpy(m_Monsters, p, sz);
			p += sz;
		}

		return p - pData;
	}
};

struct AWARD_PQ_RANKING
{
	AWARD_PQ_RANKING()
	{
		memset(this, 0, sizeof(*this));

#ifdef TASK_TEMPL_EDITOR
		m_RankingAward = new RANKING_AWARD[MAX_AWARD_PQ_RANKING];
		g_ulNewCount++;
#endif
	}
	~AWARD_PQ_RANKING()
	{
		LOG_DELETE_ARR(m_RankingAward);
	}

	AWARD_PQ_RANKING& operator = (const AWARD_PQ_RANKING& src)
	{
		m_bAwardByProf = src.m_bAwardByProf;
		m_ulRankingAwardNum = src.m_ulRankingAwardNum;

#ifndef TASK_TEMPL_EDITOR

		if (m_ulRankingAwardNum)
		{
			m_RankingAward = new RANKING_AWARD[m_ulRankingAwardNum];
			g_ulNewCount++;
		}

#endif

		for (unsigned int i = 0; i < m_ulRankingAwardNum; i++)
			m_RankingAward[i] = src.m_RankingAward[i];

		return *this;
	}

	bool operator == (const AWARD_PQ_RANKING& src) const
	{
		if (m_bAwardByProf != src.m_bAwardByProf)
		{
			return false;
		}
		if (m_ulRankingAwardNum != src.m_ulRankingAwardNum)
		{
			return false;
		}
		
		for (unsigned int i = 0;i < m_ulRankingAwardNum;++i)
		{
			if (!(m_RankingAward[i] == src.m_RankingAward[i]))
			{
				return false;
			}
		}
		return true;
	}	

	bool			m_bAwardByProf;
	unsigned int	m_ulRankingAwardNum;
	RANKING_AWARD*	m_RankingAward; //[MAX_AWARD_PQ_RANKING]

	int MarshalBasicData(char* pData)
	{
		char* p = pData;

		*p = m_bAwardByProf;
		p++;

		char cnt = (char)m_ulRankingAwardNum;
		*p = cnt;
		p++;
	
		unsigned int sz = sizeof(RANKING_AWARD) * cnt;
		if (sz)
		{
			memcpy(p, m_RankingAward, sz);
			p += sz;
		}

		return p - pData;
	}

	int UnmarshalBasicData(const char* pData)
	{
		const char* p = pData;

		m_bAwardByProf = (*p != 0);
		p++;

		m_ulRankingAwardNum = *p;
		p++;

		if (m_ulRankingAwardNum)
		{
#ifndef TASK_TEMPL_EDITOR
			m_RankingAward = new RANKING_AWARD[m_ulRankingAwardNum];
			g_ulNewCount++;
#endif

			unsigned int sz = sizeof(RANKING_AWARD) * m_ulRankingAwardNum;
			memcpy(m_RankingAward, p, sz);
			p += sz;
		}

		return p - pData;
	}
};

//���붨��operator ==
template<typename T_NAME>
inline bool CompareTwoPointer(T_NAME* p1,T_NAME* p2)
{
	if (p1 == 0 && p2 == 0)
	{
		return true;
	}
	else if (p1 != 0 && p2 != 0)
	{
		return (*p1 == *p2);
	}
	else
		return false;
}

struct AWARD_DATA
{
	AWARD_DATA()
	{
		memset(this, 0, sizeof(*this));

#ifdef TASK_TEMPL_EDITOR
		m_CandItems = new AWARD_ITEMS_CAND[MAX_AWARD_CANDIDATES];
		g_ulNewCount++;

		m_SummonedMonsters = new AWARD_MONSTERS_SUMMONED;
		g_ulNewCount++;

		m_PQRankingAward = new AWARD_PQ_RANKING;
		g_ulNewCount++;

		m_pTitleAward = new TITLE_AWARD[MAX_TITLE_NUM];
		g_ulNewCount++;
		
		AwardFactionTargetData = new FACTION_TARGET_DATA[4];
		g_ulNewCount++;		
		
#endif
	}
	AWARD_DATA(const AWARD_DATA& src)
	{
#ifdef TASK_TEMPL_EDITOR
		m_CandItems = new AWARD_ITEMS_CAND[MAX_AWARD_CANDIDATES];
		g_ulNewCount++;

		m_SummonedMonsters = new AWARD_MONSTERS_SUMMONED;
		g_ulNewCount++;

		m_PQRankingAward = new AWARD_PQ_RANKING;
		g_ulNewCount++;

		m_pTitleAward = new TITLE_AWARD[MAX_TITLE_NUM];
		g_ulNewCount++;
		
		AwardFactionTargetData = new FACTION_TARGET_DATA[4];
		g_ulNewCount++;

#endif

		*this = src;
	}
	~AWARD_DATA()
	{
		LOG_DELETE_ARR(m_CandItems);
		LOG_DELETE(m_SummonedMonsters);
		LOG_DELETE(m_PQRankingAward);
		LOG_DELETE_ARR(m_plChangeKey);
		LOG_DELETE_ARR(m_plChangeKeyValue);
		LOG_DELETE_ARR(m_pbChangeType);
		LOG_DELETE_ARR(m_plDisplayKey);
		LOG_DELETE_ARR(m_pszExp);
		LOG_DELETE_ARR(m_pExpArr);
		LOG_DELETE_ARR(m_pTaskChar);
		LOG_DELETE_ARR(m_pTitleAward);

		LOG_DELETE_ARR(m_plHistoryChangeKey);
		LOG_DELETE_ARR(m_plHistoryChangeKeyValue);
		LOG_DELETE_ARR(m_pbHistoryChangeType);
		
		LOG_DELETE_ARR(AwardFactionTargetData);
	}

	AWARD_DATA& operator = (const AWARD_DATA& src)
	{
#ifdef TASK_TEMPL_EDITOR
		AWARD_ITEMS_CAND* pCans = m_CandItems;
		AWARD_MONSTERS_SUMMONED* pSummoned = m_SummonedMonsters;
		AWARD_PQ_RANKING* pRanking	= m_PQRankingAward;
		TITLE_AWARD* pTitle = m_pTitleAward;
#endif

		memcpy(this, &src, sizeof(*this));

#ifdef TASK_TEMPL_EDITOR
		m_CandItems = pCans;
		m_SummonedMonsters = pSummoned;
		m_PQRankingAward = pRanking;
		m_pTitleAward = pTitle;
#else
		if (m_ulCandItems)
		{
			m_CandItems = new AWARD_ITEMS_CAND[m_ulCandItems];
			g_ulNewCount++;
		}
		else
			m_CandItems = NULL;

		if (m_ulSummonedMonsters)
		{
			m_SummonedMonsters = new AWARD_MONSTERS_SUMMONED;
			g_ulNewCount++;
		}
		else
			m_SummonedMonsters = NULL;

		if (m_ulPQRankingAwardCnt)
		{
			m_PQRankingAward = new AWARD_PQ_RANKING;
			g_ulNewCount++;
		}
		else
			m_PQRankingAward = NULL;
		if (m_ulTitleNum)
		{
			m_pTitleAward = new TITLE_AWARD[m_ulTitleNum];
			g_ulNewCount++;
		}
		else m_pTitleAward = NULL;

#endif

		unsigned int i;
		for (i = 0; i < m_ulCandItems; i++)
			m_CandItems[i] = src.m_CandItems[i];

		for (i = 0; i < m_ulTitleNum; i++)
			m_pTitleAward[i] = src.m_pTitleAward[i];

		if(m_ulSummonedMonsters)
			*m_SummonedMonsters = *src.m_SummonedMonsters;

		if(m_ulPQRankingAwardCnt)
			*m_PQRankingAward = *src.m_PQRankingAward;

		if (m_ulChangeKeyCnt)
		{
			m_plChangeKey = new int[m_ulChangeKeyCnt];
			m_plChangeKeyValue = new int[m_ulChangeKeyCnt];
			m_pbChangeType = new bool[m_ulChangeKeyCnt];

			for (unsigned int i = 0; i < m_ulChangeKeyCnt; i++)
			{
				m_plChangeKey[i] = src.m_plChangeKey[i];
				m_plChangeKeyValue[i] = src.m_plChangeKeyValue[i];
				m_pbChangeType[i] = src.m_pbChangeType[i];
			}
		}
		else
		{
			m_plChangeKey = 0;
			m_plChangeKeyValue = 0;
			m_pbChangeType = 0;
		}

		if (m_ulHistoryChangeCnt)
		{
			m_plHistoryChangeKey = new int[m_ulHistoryChangeCnt];
			m_plHistoryChangeKeyValue = new int[m_ulHistoryChangeCnt];
			m_pbHistoryChangeType = new bool[m_ulHistoryChangeCnt];
			
			for (unsigned int i = 0; i < m_ulHistoryChangeCnt; i++)
			{
				m_plHistoryChangeKey[i] = src.m_plHistoryChangeKey[i];
				m_plHistoryChangeKeyValue[i] = src.m_plHistoryChangeKeyValue[i];
				m_pbHistoryChangeType[i] = src.m_pbHistoryChangeType[i];
			}
		}
		else
		{
			m_plHistoryChangeKey = 0;
			m_plHistoryChangeKeyValue = 0;
			m_pbHistoryChangeType = 0;
		}

		if (m_ulDisplayKeyCnt)
		{
			m_plDisplayKey = new int[m_ulDisplayKeyCnt];
			memcpy(m_plDisplayKey, src.m_plDisplayKey, sizeof(int) * m_ulDisplayKeyCnt);
		}
		else
			m_plDisplayKey = 0;

		if (m_ulExpCnt)
		{
			m_pszExp = new char[m_ulExpCnt][TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
			m_pExpArr = new TASK_EXPRESSION[m_ulExpCnt][TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
			for (unsigned int i=0; i<m_ulExpCnt; i++)
			{
				memcpy(m_pszExp[i], src.m_pszExp[i], TASK_AWARD_MAX_DISPLAY_CHAR_LEN);
				memcpy(m_pExpArr[i], src.m_pExpArr[i], TASK_AWARD_MAX_DISPLAY_CHAR_LEN);
			}
		}
		else
		{
			m_pszExp = NULL;
			m_pExpArr = NULL;
		}

		if (m_ulTaskCharCnt)
		{
			m_pTaskChar = new task_char[m_ulTaskCharCnt][TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
			for (unsigned int i=0; i<m_ulTaskCharCnt; i++)
			{
				memcpy(m_pTaskChar[i], src.m_pTaskChar[i], TASK_AWARD_MAX_DISPLAY_CHAR_LEN);	
			}
		}
		else
			m_pTaskChar = NULL;

		return *this;
	}

	
	bool operator == (const AWARD_DATA& src) const
	{
		if (m_ulCandItems != src.m_ulCandItems || m_ulSummonedMonsters != src.m_ulSummonedMonsters || 
			m_ulChangeKeyCnt != src.m_ulChangeKeyCnt || m_ulDisplayKeyCnt != src.m_ulDisplayKeyCnt || 
			m_ulExpCnt != src.m_ulExpCnt || m_ulTaskCharCnt != src.m_ulTaskCharCnt ||
			m_ulHistoryChangeCnt != src.m_ulHistoryChangeCnt)
		{
			return false;
		}

		unsigned int i = 0;
		for (i = 0;i < m_ulCandItems;++i)
		{
			if (!(m_CandItems[i] == src.m_CandItems[i]))
			{
				return false;
			}
		}
		if (!CompareTwoPointer(m_SummonedMonsters,src.m_SummonedMonsters))
		{
			return false;
		}

		for (i = 0;i < m_ulChangeKeyCnt;++i)
		{
			if (m_plChangeKey[i] != src.m_plChangeKey[i] || m_plChangeKeyValue[i] != src.m_plChangeKeyValue[i] || m_pbChangeType[i] != src.m_pbChangeType[i])
			{
				return false;
			}
		}
		for (i = 0;i < m_ulHistoryChangeCnt;++i)
		{
			if (m_plHistoryChangeKey[i] != src.m_plHistoryChangeKey[i] || m_plHistoryChangeKeyValue[i] != src.m_plHistoryChangeKeyValue[i] || m_pbHistoryChangeType[i] != src.m_pbHistoryChangeType[i])
			{
				return false;
			}
		}

		for (i = 0;i < m_ulDisplayKeyCnt;++i)
		{
			if (m_plDisplayKey[i] != src.m_plDisplayKey[i])
			{
				return false;
			}
		}

		if (!CompareTwoPointer(m_PQRankingAward,src.m_PQRankingAward))
		{
			return false;
		}
		
		return (m_ulGoldNum					==	src.m_ulGoldNum					&&
				m_ulExp						==	src.m_ulExp						&&
				m_ulNewTask					==	src.m_ulNewTask					&&
				m_ulSP						==	src.m_ulSP						&&
				m_lReputation				==	src.m_lReputation				&&
				m_ulNewPeriod				==	src.m_ulNewPeriod				&&
				m_ulNewRelayStation			==	src.m_ulNewRelayStation			&&
				m_ulStorehouseSize			==	src.m_ulStorehouseSize			&&
				m_ulStorehouseSize2			==	src.m_ulStorehouseSize2			&&
				m_ulStorehouseSize3			==	src.m_ulStorehouseSize3			&&
				m_ulStorehouseSize4			==	src.m_ulStorehouseSize4			&&
				m_lInventorySize			==	src.m_lInventorySize			&&
				m_ulPetInventorySize		==	src.m_ulPetInventorySize		&&
				m_ulFuryULimit				==	src.m_ulFuryULimit				&&
				m_ulTransWldId				==	src.m_ulTransWldId				&&
				m_TransPt					==  src.m_TransPt					&&
				m_lMonsCtrl					==  src.m_lMonsCtrl					&&
				m_bTrigCtrl					==	src.m_bTrigCtrl					&&
				m_bUseLevCo					==	src.m_bUseLevCo					&&
				m_bDivorce					==  src.m_bDivorce					&&
				m_bSendMsg					==	src.m_bSendMsg					&&
				m_nMsgChannel				==	src.m_nMsgChannel				&&
				m_bAwardDeath				==  src.m_bAwardDeath				&&
				m_bAwardDeathWithLoss		==	src.m_bAwardDeathWithLoss		&&
				m_ulDividend				==	src.m_ulDividend				&&
				m_bAwardSkill				==	src.m_bAwardSkill				&&
				m_iAwardSkillID				==	src.m_iAwardSkillID				&&
				m_iAwardSkillLevel			==	src.m_iAwardSkillLevel			&&
				m_ulSpecifyContribTaskID	==	src.m_ulSpecifyContribTaskID	&&
				m_ulSpecifyContribSubTaskID ==	src.m_ulSpecifyContribSubTaskID &&
				m_ulSpecifyContrib			==	src.m_ulSpecifyContrib			&&
				m_ulContrib					==	src.m_ulContrib					&&
				m_ulRandContrib				==	src.m_ulRandContrib				&&
				m_ulLowestcontrib			==	src.m_ulLowestcontrib			&&
				m_iFactionContrib			==	src.m_iFactionContrib			&&
				m_iFactionExpContrib		==	src.m_iFactionExpContrib		&&
				m_ulPQRankingAwardCnt		==	src.m_ulPQRankingAwardCnt		&&
				m_bMulti					==	src.m_bMulti					&&
				m_nNumType					==	src.m_nNumType					&&
				m_lNum						==	src.m_lNum						&&
				m_iSoloTowerChallengeScore	==	src.m_iSoloTowerChallengeScore
				);
	}
//----------------------------------------------------------------------------------------------------
//--PW TASK AWARD DATA STRUCT & 162
//----------------------------------------------------------------------------------------------------
	unsigned int				m_ulGoldNum;
	unsigned int				m_ulExp;
	unsigned int				m_ulRealmExp;
	bool						m_bExpandRealmLevelMax;
	unsigned int				NewTaskSpace;
	unsigned int				m_ulNewTask;
	unsigned int				m_ulSP;
	int							m_lReputation;
	unsigned int				m_ulNewPeriod;
	unsigned int				m_ulNewRelayStation;
	unsigned int				m_ulStorehouseSize;
	unsigned int				m_ulStorehouseSize2;
	unsigned int				m_ulStorehouseSize3;
	unsigned int				m_ulStorehouseSize4;
	int							m_lInventorySize;
	unsigned int				m_ulPetInventorySize;
	unsigned int				m_ulFuryULimit;
	unsigned int				m_ulTransWldId;
	int 						x_CrossServerID;	//172
	ZONE_VERT					m_TransPt;
	int							m_lMonsCtrl;
	bool						m_bTrigCtrl;
	bool						m_bUseLevCo;
	bool						m_bDivorce;
	bool						m_bSendMsg;
	int							m_nMsgChannel;
	unsigned int				m_ulCandItems; //
	AWARD_ITEMS_CAND*			m_CandItems;
	unsigned int				m_ulSummonedMonsters;
	AWARD_MONSTERS_SUMMONED* 	m_SummonedMonsters;
	bool						m_bAwardDeath;
	bool						m_bAwardDeathWithLoss;
	unsigned int				m_ulDividend;
	bool						m_bAwardSkill;
	int							m_iAwardSkillID;
	int							m_iAwardSkillLevel;
	int							m_iSoloTowerChallengeScore;
	int 						AwardOpenSystem;
	unsigned int				m_ulSpecifyContribTaskID;
	unsigned int				m_ulSpecifyContribSubTaskID;
	unsigned int				m_ulSpecifyContrib;
	unsigned int				m_ulContrib;
	unsigned int				m_ulRandContrib;
	unsigned int				m_ulLowestcontrib;
	int							m_iFactionContrib;
	int							m_iFactionExpContrib;
	unsigned int				m_ulPQRankingAwardCnt;
	AWARD_PQ_RANKING*			m_PQRankingAward;
	unsigned int       			m_ulChangeKeyCnt;
	int                			*m_plChangeKey;
	int                			*m_plChangeKeyValue;
	bool                		*m_pbChangeType;
	unsigned int				m_ulPersonChangeCnt;           //172
	int							*m_plPersonChangeKey;          //172
	int							*m_plPersonChangeKeyValue;     //172
	bool                		*m_pbPersonChangeType;         //172
	unsigned int				m_ulNewChangeCnt;              //172
	int							*m_plNewChangeKey;             //172
	int							*m_plNewChangeKeyValue;        //172
	bool                		*m_pbNewChangeType;	           //172
	unsigned int				m_ulHistoryChangeCnt;
	int							*m_plHistoryChangeKey;
	int							*m_plHistoryChangeKeyValue;
	bool                		*m_pbHistoryChangeType;
	bool                		m_bMulti;
	int                 		m_nNumType;
	int                			m_lNum;
	unsigned int       			m_ulDisplayKeyCnt;
	int                			*m_plDisplayKey;
	unsigned int				m_ulExpCnt;
	char						(*m_pszExp)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
	TASK_EXPRESSION 			(*m_pExpArr)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
	unsigned int       			m_ulTaskCharCnt;
	task_char           		(*m_pTaskChar)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
	int							m_iForceContribution;
	int							m_iForceReputation;
	int							m_iForceActivity;
	int							m_iForceSetRepu;
	int							m_iTaskLimit;
	unsigned int				m_ulTitleNum;
	TITLE_AWARD*				m_pTitleAward;	
	int							m_iLeaderShip;
	int							m_iWorldContribution; ///++++++++++
	int 						m_iHomeResource[5];
	bool 						m_bCreateHome;	
	bool 						AwardLeaveCarrier;
	int 						AwardFactionMoney;
	int 						AwardFactionPersonScore;
	int 						AwardFactionTarget;
	FACTION_TARGET_DATA* 		AwardFactionTargetData;
	bool 						SubmitRankScore;
	SUBMIT_RANK_INFO 			submit_rank;
	int 						AwardMentorStudent; ///
	int 						AwardCoupleIntimacy;
	int 						AwardRedBookExp;          //172
	int 						AwardCampScore;           //172
	int 						AwardCrossServerMoney;    //172
	int 						InviterAward;             //172
	int 						AwardFriendPoint;         //172
	int 						InviterAwardCount;        //172
	int 						AwardPerceptionValue;     //172
//----------------------------------------------------------------------------------------------------
	bool HasAward() const
	{
		return m_ulGoldNum != 0
			|| m_ulExp != 0
			|| m_ulNewTask != 0
			|| m_ulSP != 0
			|| m_lReputation != 0
			|| m_ulNewPeriod != 0
			|| m_ulNewRelayStation != 0
			|| m_ulStorehouseSize != 0
			|| m_ulStorehouseSize2 != 0
			|| m_ulStorehouseSize3 != 0
			|| m_ulStorehouseSize4 != 0
			|| m_lInventorySize != 0
			|| m_ulPetInventorySize != 0
			|| m_ulFuryULimit
			|| m_bDivorce
			|| m_bSendMsg
			|| m_bAwardDeath
			|| m_ulCandItems != 0
			|| m_ulSummonedMonsters != 0
			|| m_ulSpecifyContrib != 0
			|| m_ulSpecifyContribTaskID != 0
			|| m_ulContrib != 0
			|| m_ulRandContrib != 0
			|| m_ulDividend != 0
			|| m_ulPQRankingAwardCnt != 0
			|| m_ulSpecifyContribSubTaskID !=0
			|| m_bAwardSkill
			|| m_iFactionContrib != 0
			|| m_iFactionExpContrib != 0
			|| m_iForceActivity != 0
			|| m_iForceContribution != 0
			|| m_iForceReputation != 0
			|| m_iForceSetRepu
			|| m_iTaskLimit
			|| m_ulRealmExp != 0
			|| m_bExpandRealmLevelMax
			|| m_iSoloTowerChallengeScore != 0
			|| AwardLeaveCarrier != 0;
	}

	int MarshalBasicData(char* pData)
	{
		char* p = pData;

		int* _mask = (int*)p;
		*_mask = 0;
		p += 4;

		if (m_ulGoldNum)
		{
			*_mask |= 1;
			*(int*)p = m_ulGoldNum;
			p += sizeof(int);
		}

		if (m_ulExp)
		{
			*_mask |= 1 << 1;
			*(int*)p = m_ulExp;
			p += sizeof(int);
		}

		if (m_ulSP)
		{
			*_mask |= 1 << 2;
			*(int*)p = m_ulSP;
			p += sizeof(int);
		}

		if (m_lReputation)
		{
			*_mask |= 1 << 3;
			*(int*)p = m_lReputation;
			p += sizeof(int);
		}

		if (m_ulCandItems)
		{
			*_mask |= 1 << 4;

			char cnt = (char)m_ulCandItems;
			*p = cnt;
			p++;

			for (char i = 0; i < cnt; i++)
				p += m_CandItems[i].MarshalBasicData(p);
		}

		if (m_ulSummonedMonsters)
		{
			*_mask |= 1 << 5;

			char cnt = (char)m_ulSummonedMonsters;
			*p = cnt;
			p++;

			p += m_SummonedMonsters->MarshalBasicData(p);
		}

		if (m_ulPQRankingAwardCnt)
		{
			*_mask |= 1 << 6;

			char cnt = (char)m_ulPQRankingAwardCnt;
			*p = cnt;
			p++;

			p += m_PQRankingAward->MarshalBasicData(p);
		}

		return p - pData;
	}

	int UnmarshalBasicData(const char* pData)
	{
		const char* p = pData;

		int _mask = *(int*)p;
		p += sizeof(int);

		if (_mask & 1)
		{
			m_ulGoldNum = *(int*)p;
			p += 4;
		}

		if (_mask & (1 << 1))
		{
			m_ulExp = *(int*)p;
			p += 4;
		}

		if (_mask & (1 << 2))
		{
			m_ulSP = *(int*)p;
			p += 4;
		}

		if (_mask & (1 << 3))
		{
			m_lReputation = *(int*)p;
			p += 4;
		}

		if (_mask & (1 << 4))
		{
			m_ulCandItems = *p;
			p++;

			if (m_ulCandItems)
			{
#ifndef TASK_TEMPL_EDITOR
				m_CandItems = new AWARD_ITEMS_CAND[m_ulCandItems];
				g_ulNewCount++;
#endif

				for (unsigned int i = 0; i < m_ulCandItems; i++)
					p += m_CandItems[i].UnmarshalBasicData(p);
			}
		}

		if (_mask & (1 << 5))
		{
			m_ulSummonedMonsters = *p;
			p++;

			if (m_ulSummonedMonsters)
			{
#ifndef TASK_TEMPL_EDITOR
				m_SummonedMonsters = new AWARD_MONSTERS_SUMMONED;
				g_ulNewCount++;
#endif

				p += m_SummonedMonsters->UnmarshalBasicData(p);
			}
		}

		if (_mask & (1 << 6))
		{
			m_ulPQRankingAwardCnt = *p;
			p++;

			if (m_ulPQRankingAwardCnt)
			{
#ifndef TASK_TEMPL_EDITOR
				m_PQRankingAward = new AWARD_PQ_RANKING;
				g_ulNewCount++;
#endif

				p += m_PQRankingAward->UnmarshalBasicData(p);
			}
		}

		return p - pData;
	}
};

struct AWARD_RATIO_SCALE
{
	AWARD_RATIO_SCALE()
	{
		memset(this, 0, sizeof(*this));

#ifdef TASK_TEMPL_EDITOR
		m_Awards = new AWARD_DATA[MAX_AWARD_SCALES];
		g_ulNewCount++;
#endif
	}
	~AWARD_RATIO_SCALE()
	{
		LOG_DELETE_ARR(m_Awards);
	}

	bool operator == (const AWARD_RATIO_SCALE& src) const
	{
		if (m_ulScales != src.m_ulScales)
		{
			return false;
		}

		for (unsigned int i = 0;i < m_ulScales;++i)
		{
			if (m_Ratios[i] != src.m_Ratios[i] || !(m_Awards[i] == src.m_Awards[i]))
			{
				return false;
			}
		}
		return true;
	}
	unsigned int	m_ulScales;
	float			m_Ratios[MAX_AWARD_SCALES];
	AWARD_DATA*		m_Awards; //[MAX_AWARD_SCALES];

	AWARD_RATIO_SCALE& operator = (const AWARD_RATIO_SCALE& src)
	{
		m_ulScales = src.m_ulScales;

#ifndef TASK_TEMPL_EDITOR

		if (m_ulScales)
		{
			m_Awards = new AWARD_DATA[m_ulScales];
			g_ulNewCount++;
		}

#endif

		for (unsigned int i = 0; i < m_ulScales; i++)
		{
			m_Ratios[i] = src.m_Ratios[i];
			m_Awards[i] = src.m_Awards[i];
		}

		return *this;
	}

	bool HasAward() const
	{
		for (unsigned int i = 0; i < m_ulScales; i++)
			if (m_Awards[i].HasAward())
				return true;

		return false;
	}
};


struct AWARD_CARRIER_HP_SCALE
{
	AWARD_CARRIER_HP_SCALE()
	{
		memset(this, 0, sizeof(*this));

#ifdef TASK_TEMPL_EDITOR
		m_Awards = new AWARD_DATA[MAX_AWARD_CARRIER_HP_SCALES];
		g_ulNewCount++;
#endif
	}
	~AWARD_CARRIER_HP_SCALE()
	{
		LOG_DELETE_ARR(m_Awards);
	}

	bool operator == (const AWARD_CARRIER_HP_SCALE& src) const
	{
		if (m_ulScales != src.m_ulScales)
		{
			return false;
		}

		for (unsigned int i = 0;i < m_ulScales;++i)
		{
			if (CarrierHp[i] != src.CarrierHp[i] || !(m_Awards[i] == src.m_Awards[i]))
			{
				return false;
			}
		}
		return true;
	}
	unsigned int	m_ulScales;
	float			CarrierHp[MAX_AWARD_CARRIER_HP_SCALES];
	AWARD_DATA*		m_Awards;

	AWARD_CARRIER_HP_SCALE& operator = (const AWARD_CARRIER_HP_SCALE& src)
	{
		m_ulScales = src.m_ulScales;

#ifndef TASK_TEMPL_EDITOR

		if (m_ulScales)
		{
			m_Awards = new AWARD_DATA[m_ulScales];
			g_ulNewCount++;
		}

#endif

		for (unsigned int i = 0; i < m_ulScales; i++)
		{
			CarrierHp[i] = src.CarrierHp[i];
			m_Awards[i] = src.m_Awards[i];
		}

		return *this;
	}

	bool HasAward() const
	{
		for (unsigned int i = 0; i < m_ulScales; i++)
			if (m_Awards[i].HasAward())
				return true;

		return false;
	}
};

struct AWARD_ITEMS_SCALE
{
	AWARD_ITEMS_SCALE()
	{
		memset(this, 0, sizeof(*this));

#ifdef TASK_TEMPL_EDITOR
		m_Awards = new AWARD_DATA[MAX_AWARD_SCALES];
		g_ulNewCount++;
#endif
	}
	~AWARD_ITEMS_SCALE()
	{
		LOG_DELETE_ARR(m_Awards);
	}

	unsigned int	m_ulScales;
	unsigned int	m_ulItemId;
	unsigned int	m_Counts[MAX_AWARD_SCALES];
	AWARD_DATA*		m_Awards; //[MAX_AWARD_SCALES];

	bool operator == (const AWARD_ITEMS_SCALE& src) const
	{
		if (m_ulScales != src.m_ulScales)
		{
			return false;
		}
		for (unsigned int i = 0;i < m_ulScales;++i)
		{
			if (m_Counts[i] != src.m_Counts[i] || !(m_Awards[i] == src.m_Awards[i]))
			{
				return false;
			}
		}

		return m_ulItemId == src.m_ulItemId;
	}

	AWARD_ITEMS_SCALE& operator = (const AWARD_ITEMS_SCALE& src)
	{
		m_ulScales = src.m_ulScales;
		m_ulItemId = src.m_ulItemId;

#ifndef TASK_TEMPL_EDITOR

		if (m_ulScales)
		{
			m_Awards = new AWARD_DATA[m_ulScales];
			g_ulNewCount++;
		}

#endif

		for (unsigned int i = 0; i < m_ulScales; i++)
		{
			m_Counts[i] = src.m_Counts[i];
			m_Awards[i] = src.m_Awards[i];
		}

		return *this;
	}

	bool HasAward() const
	{
		for (unsigned int i = 0; i < m_ulScales; i++)
			if (m_Awards[i].HasAward())
				return true;

		return false;
	}
};

//-------------------------------------------------------------------------------------//
struct AWARD_VAR_SCALE
{
	AWARD_VAR_SCALE()
	{
		memset(this, 0, sizeof(*this));

#ifdef TASK_TEMPL_EDITOR
		m_Awards = new AWARD_DATA[MAX_AWARD_VAR_SCALES];
		g_ulNewCount++;
#endif
	}
	~AWARD_VAR_SCALE()
	{
		LOG_DELETE_ARR(m_Awards);
	}

	bool operator == (const AWARD_VAR_SCALE& src) const
	{
		if (m_ulScales != src.m_ulScales)
		{
			return false;
		}

		for (unsigned int i = 0;i < m_ulScales;++i)
		{
			if ( !(AwardVar[i] == src.AwardVar[i]) || !(m_Awards[i] == src.m_Awards[i]))
			{
				return false;
			}
		}
		return true;
	}
	unsigned int	m_ulScales;
	TASK_EXPRESSION	AwardVar[MAX_AWARD_VAR_SCALES];
	AWARD_DATA*		m_Awards; //[MAX_AWARD_VAR_SCALES];

	AWARD_VAR_SCALE& operator = (const AWARD_VAR_SCALE& src)
	{
		m_ulScales = src.m_ulScales;

#ifndef TASK_TEMPL_EDITOR

		if (m_ulScales)
		{
			m_Awards = new AWARD_DATA[m_ulScales];
			g_ulNewCount++;
		}

#endif

		for (unsigned int i = 0; i < m_ulScales; i++)
		{
			AwardVar[i] = src.AwardVar[i];
			m_Awards[i] = src.m_Awards[i];
		}

		return *this;
	}

	bool HasAward() const
	{
		for (unsigned int i = 0; i < m_ulScales; i++)
			if (m_Awards[i].HasAward())
				return true;

		return false;
	}
};
//-------------------------------------------------------------------------------



enum task_tm_type
{
	enumTaskTimeDate = 0,
	enumTaskTimeMonth,
	enumTaskTimeWeek,
	enumTaskTimeDay
};

const int task_week_map[] =
{
	7,
	1,
	2,
	3,
	4,
	5,
	6
};

struct task_tm
{
	int year;
	int month;
	int day;
	int hour;
	int min;
	int wday;

	bool operator == (const task_tm& time) const
	{
		return ( year == time.year && month == time.month && day == time.day && hour == time.hour && min == time.min);
	}

	bool after(const tm* _tm) const
	{
		if (year < _tm->tm_year + 1900) return false;
		if (year > _tm->tm_year + 1900) return true;

		if (month < _tm->tm_mon + 1) return false;
		if (month > _tm->tm_mon + 1) return true;

		if (day < _tm->tm_mday) return false;
		if (day > _tm->tm_mday) return true;

		if (hour < _tm->tm_hour) return false;
		return hour > _tm->tm_hour || min > _tm->tm_min;
	}

	bool before(const tm* _tm) const
	{
		if (year > _tm->tm_year + 1900) return false;
		if (year < _tm->tm_year + 1900) return true;

		if (month > _tm->tm_mon + 1) return false;
		if (month < _tm->tm_mon + 1) return true;

		if (day > _tm->tm_mday) return false;
		if (day < _tm->tm_mday) return true;

		if (hour > _tm->tm_hour) return false;
		return hour < _tm->tm_hour || min <= _tm->tm_min;
	}

	bool after_per_month(const tm* _tm, bool bLastDay) const
	{
		// �����ǰ������趨����Ϊfalse
		if (day < _tm->tm_mday) return false;

		// �����ǰ���Ƿ����һ�죬��С���趨�죬�򷵻�true
		if (!bLastDay && day > _tm->tm_mday) return true;

		if (hour < _tm->tm_hour) return false;
		return hour > _tm->tm_hour || min > _tm->tm_min;
	}

	bool before_per_month(const tm* _tm, bool bLastDay) const
	{
		// �����ǰ������趨����Ϊtrue
		if (day < _tm->tm_mday) return true;

		// �����ǰ���Ƿ����һ�죬��С���趨�죬�򷵻�false
		if (!bLastDay && day > _tm->tm_mday) return false;

		if (hour > _tm->tm_hour) return false;
		return hour < _tm->tm_hour || min <= _tm->tm_min;
	}

	bool after_per_week(const tm* _tm) const
	{
		int w = task_week_map[_tm->tm_wday];

		if (wday < w) return false;
		if (wday > w) return true;

		if (hour < _tm->tm_hour) return false;
		return hour > _tm->tm_hour || min > _tm->tm_min;
	}

	bool before_per_week(const tm* _tm) const
	{
		int w = task_week_map[_tm->tm_wday];

		if (wday > w) return false;
		if (wday < w) return true;

		if (hour > _tm->tm_hour) return false;
		return hour < _tm->tm_hour || min <= _tm->tm_min;
	}

	bool after_per_day(const tm* _tm) const
	{
		if (hour < _tm->tm_hour) return false;
		return hour > _tm->tm_hour || min > _tm->tm_min;
	}

	bool before_per_day(const tm* _tm) const
	{
		if (hour > _tm->tm_hour) return false;
		return hour < _tm->tm_hour || min <= _tm->tm_min;
	}
};

inline bool judge_time_date(const task_tm* tmStart, const task_tm* tmEnd, unsigned int _ulCurTime, task_tm_type tm_type)
{
	tm _time, _time_tomorrow;
	time_t ulTimeTomorrow;
	time_t ulCurTime = _ulCurTime;
	bool last_day;

#ifdef _TASK_CLIENT
	ulCurTime -= time_t(TaskInterface::GetTimeZoneBias() * 60);

	if ((signed)ulCurTime < 0)
		ulCurTime = 0;

	_time = *gmtime(&ulCurTime);
	ulTimeTomorrow = ulCurTime + 24 * 3600;
	_time_tomorrow = *gmtime(&ulTimeTomorrow);
#else
	_time = *localtime(&ulCurTime);
	ulTimeTomorrow = ulCurTime + 24 * 3600;
	_time_tomorrow = *localtime(&ulTimeTomorrow);
#endif

	last_day = (_time.tm_mon != _time_tomorrow.tm_mon);

	switch (tm_type)
	{
	case enumTaskTimeDate:
		return tmStart->before(&_time) && tmEnd->after(&_time);
	case enumTaskTimeMonth:
		return tmStart->before_per_month(&_time, last_day) && tmEnd->after_per_month(&_time, last_day);
	case enumTaskTimeWeek:
		return tmStart->before_per_week(&_time) && tmEnd->after_per_week(&_time);
	case enumTaskTimeDay:
		return tmStart->before_per_day(&_time) && tmEnd->after_per_day(&_time);
	}

	return false;
}

#define MAX_SUB_TAGS 32

struct task_sub_tags
{
	union
	{
		unsigned short sub_task;
		unsigned char state;
	};

	unsigned char sz;
	unsigned char tags[MAX_SUB_TAGS];
	unsigned char cur_index; // for temporary use, dont take into account

	unsigned int get_size() const { return sz + 3; }
	bool valid_size(unsigned int _sz) const
	{
		if (_sz < 3) return false;
		return get_size() == _sz;
	}
};

struct task_notify_base
{
	unsigned char	reason;
	unsigned char	space;
	unsigned short	task;
};

struct task_notify_choose_award : public task_notify_base
{
	unsigned char choice;
};

struct task_player_notify : public task_notify_base
{
	unsigned int	param;
};

struct svr_monster_killed : public task_notify_base
{
	unsigned int	monster_id;
	unsigned short	monster_num;
	int				dps;
	int				dph;
};

struct svr_player_killed : public task_notify_base
{
	unsigned short index;
	unsigned short player_num;
};

struct svr_treasure_map : public task_notify_base
{
	int treasure_index;
};

// Novos modelos de missões (Evento)

struct event_notify_base
{
	unsigned short	reason; //8
	unsigned short	space; //0
	unsigned int	task; //id
};

struct svr_task_event_complete : public event_notify_base
{
	unsigned short points;
};

struct svr_new_task : public task_notify_base
{
	unsigned int	cur_time;
	unsigned int	cap_task;
	task_sub_tags	sub_tags;

	inline void set_data(
		unsigned int _cur_time,
		unsigned int _cap_task,
		const task_sub_tags& _sub_tags)
	{
		cur_time	= _cur_time;
		cap_task	= _cap_task;
		memcpy(&sub_tags, &_sub_tags, _sub_tags.get_size());
	}

	inline void get_data(
		unsigned int& _cur_time,
		unsigned int& _cap_task,
		task_sub_tags& _sub_tags) const
	{
		_cur_time	= cur_time;
		_cap_task	= cap_task;
		memcpy(&_sub_tags, &sub_tags, sub_tags.get_size());
	}

	inline unsigned int get_size() const { return sizeof(task_notify_base) + 8 + sub_tags.get_size(); }

	inline bool valid_size(unsigned int sz) const
	{
		const unsigned int base_sz = sizeof(task_notify_base) + 8;
		if (sz <= base_sz) return false;
		return sub_tags.valid_size(sz - base_sz);
	}
};

struct svr_task_complete : public task_notify_base
{
	unsigned int	cur_time;
	task_sub_tags	sub_tags;

	inline void set_data(
		unsigned int _cur_time,
		const task_sub_tags& _sub_tags
		)
	{
		cur_time	= _cur_time;
		memcpy(&sub_tags, &_sub_tags, _sub_tags.get_size());
	}

	inline void get_data(
		unsigned int& _cur_time,
		task_sub_tags& _sub_tags) const
	{
		_cur_time	= cur_time;
		memcpy(&_sub_tags, &sub_tags, sub_tags.get_size());
	}

	inline unsigned int get_size() const { return sizeof(task_notify_base) + 4 + sub_tags.get_size(); }

	inline bool valid_size(unsigned int sz) const
	{
		const unsigned int base_sz = sizeof(task_notify_base) + 4;
		if (sz <= base_sz) return false;
		return sub_tags.valid_size(sz - base_sz);
	}
};

struct svr_task_err_code : public task_notify_base
{
	unsigned int err_code;
};

struct svr_task_dyn_time_mark : public task_notify_base
{
	unsigned int time_mark;
	unsigned short version;
};

struct svr_task_special_award : public task_notify_base
{
	special_award sa;
};

struct COMPARE_KEY_VALUE
{
	int nLeftType;
	int lLeftNum;
	int nCompOper;
	int nRightType;
	int lRightNum;

	bool operator == (const COMPARE_KEY_VALUE& src) const
	{
		return (nLeftType == src.nLeftType && lLeftNum == src.lLeftNum && nCompOper == src.nCompOper && nRightType == src.nRightType && lRightNum == src.lRightNum);
	}
};

struct Task_Region
{
	int MapID;
	ZONE_VERT zvMin;
	ZONE_VERT zvMax;

	bool operator == (const Task_Region& src) const
	{
		return (zvMin == src.zvMin && zvMax == src.zvMax && MapID == MapID);
	}
};

//-------------------------------PW_STRUCT-----------------------------------//
//155
struct HOME_ITEM_WANTED
{
	unsigned int m_ulHomeItemTid;
	unsigned int m_ulHomeItemCount;
};
//162
struct EVENT_TASK_DATA
{
	short uSpace;
	int uID;
	float uValue;
};
//162

//162
struct SPECIAL_PARAMETERS
{
	int*	ParameterTypes;
	int*	unk176;
	int 	FailWhenGetOffCarrier;
	int 	TimeWhenRideCarrier;
	int 	GetOffCarrierID;
	int 	RideCarrierID;
	int 	UseItemTimes;
	int 	UseItemCount;
	int*	UseItemIDs;
	int 	CompleteTaskTimes;
	int 	CompleteTaskCount;
	int 	ByCompleteTask;
	int*	CompleteTaskIDs;
	int*	CompleteTaskIDsSpace;
	int		Parameter9;
	// Anedota /*160+*/
	int		need_story_book_collection;
	int		story_book_collection_count;
	
	int ReadInt32( FILE* fp )
	{
		int res = 0;
		fread(&res, sizeof(int) , 1, fp);
		return res;
	}

	void ReadParamter(FILE* fp, int ParameterType)
	{
		int UseItemIDsPointer, CompleteTaskIDsPointer, CompleteTaskIDsSpacePointer, unc1;
		
		switch (ParameterType)
		{
		case 0:
			FailWhenGetOffCarrier = ReadInt32(fp);
			break;
		case 1:
			TimeWhenRideCarrier = ReadInt32(fp);
			break;
		case 2:
			UseItemTimes = ReadInt32(fp);
			break;
		case 3:
			UseItemCount = ReadInt32(fp);
			break;
		case 4:
			UseItemIDsPointer = ReadInt32(fp);
			break;
		case 5:
			CompleteTaskTimes = ReadInt32(fp);
			break;
		case 6:
			CompleteTaskCount = ReadInt32(fp);
			break;
		case 7:
			CompleteTaskIDsPointer = ReadInt32(fp);
			break;
		case 8:
			ByCompleteTask = ReadInt32(fp);
			break;
		case 9:
			Parameter9 = ReadInt32(fp);
			break;
		case 10:
			GetOffCarrierID = ReadInt32(fp);
			break;
		case 11:
			RideCarrierID = ReadInt32(fp);
			break;
		// Anedota /*160+*/
		case 12:
			need_story_book_collection = ReadInt32(fp);
			break;
		case 13:
			story_book_collection_count = ReadInt32(fp);
			break;
		case 14:
			CompleteTaskIDsSpacePointer = ReadInt32(fp);
			break;
		default:
		return;
			break;
		}
	}
	
	void Load(FILE* fp, int count)
	{
		memset(this,0x00, sizeof(*this));
		ParameterTypes = new int[count];
		unk176 = new int[count];
		
		for (int i = 0; i < count; i++)
		{
			ParameterTypes[i] = ReadInt32(fp);
			ReadParamter(fp, ParameterTypes[i]);
			unk176[i] = ReadInt32(fp);
		}
		
		if(UseItemCount > 0)
		{
			UseItemIDs = new int[UseItemCount];
			for (int i = 0; i < UseItemCount; i++)
			{
				UseItemIDs[i] = ReadInt32(fp);
			}
		}

		if(CompleteTaskCount > 0)
		{
			CompleteTaskIDs = new int[CompleteTaskCount];
			for (int i = 0; i < CompleteTaskCount; i++)
			{
				CompleteTaskIDs[i] = ReadInt32(fp);
			}
			
			CompleteTaskIDsSpace = new int[CompleteTaskCount];
			for (int i = 0; i < CompleteTaskCount; i++)
			{
				CompleteTaskIDsSpace[i] = ReadInt32(fp);
			}
		}
	}
};
//----------------------------------------------------------------------------//				
struct ATaskTemplFixedData // No Virtual Functions, No Embedded Class
{
	ATaskTemplFixedData()
	{
		memset(this, 0, sizeof(*this));

		m_Award_S = new AWARD_DATA;
		g_ulNewCount++;
		m_Award_F = new AWARD_DATA;
		g_ulNewCount++;

		m_AwByRatio_S = new AWARD_RATIO_SCALE;
		g_ulNewCount++;
		m_AwByRatio_F = new AWARD_RATIO_SCALE;
		g_ulNewCount++;

		m_AwByItems_S = new AWARD_ITEMS_SCALE;
		g_ulNewCount++;
		m_AwByItems_F = new AWARD_ITEMS_SCALE;
		g_ulNewCount++;

		x_AwByTaskEventRatio_S = new AWARD_RATIO_SCALE;
		g_ulNewCount++;
		x_AwByTaskEventRatio_F = new AWARD_RATIO_SCALE;
		g_ulNewCount++;

		x_AwByCarrierHp_S = new AWARD_CARRIER_HP_SCALE;
		g_ulNewCount++;
		x_AwByCarrierHp_F = new AWARD_CARRIER_HP_SCALE;
		g_ulNewCount++;
		
#ifdef TASK_TEMPL_EDITOR
		m_pszSignature	= new task_char[MAX_TASK_NAME_LEN];
		g_ulNewCount++;
		m_tmStart		= new task_tm[MAX_TIMETABLE_SIZE];
		g_ulNewCount++;
		m_tmEnd			= new task_tm[MAX_TIMETABLE_SIZE];
		g_ulNewCount++;
		m_PremItems		= new ITEM_WANTED[MAX_ITEM_WANTED];
		g_ulNewCount++;
		m_GivenItems	= new ITEM_WANTED[MAX_ITEM_WANTED];
		g_ulNewCount++;
		m_TeamMemsWanted= new TEAM_MEM_WANTED[MAX_TEAM_MEM_WANTED];
		g_ulNewCount++;
		m_ItemsWanted	= new ITEM_WANTED[MAX_ITEM_WANTED];
		g_ulNewCount++;
		m_PlayerWanted  = new PLAYER_WANTED[MAX_PLAYER_WANTED];
		g_ulNewCount++;
		m_MonsterWanted	= new MONSTER_WANTED[MAX_MONSTER_WANTED];
		g_ulNewCount++;
		m_MonstersContrib = new MONSTERS_CONTRIB[MAX_CONTRIB_MONSTERS];
		g_ulNewCount++;

		m_pDelvRegion = new Task_Region[MAX_TASKREGION];
		g_ulNewCount++;

		m_pEnterRegion = new Task_Region[MAX_TASKREGION];
		g_ulNewCount++;

		m_pLeaveRegion = new Task_Region[MAX_TASKREGION];
		g_ulNewCount++;

		m_pReachSite = new Task_Region[MAX_TASKREGION];
		g_ulNewCount++;

		m_pLeaveSite = new Task_Region[MAX_TASKREGION];
		g_ulNewCount++;
		
		m_PremTitles = new int[MAX_TITLE_NUM];
		g_ulNewCount++;

		m_HomeItemsWanted = new HOME_ITEM_WANTED[20];
		g_ulNewCount++;
		
		x_TaskEventMonsterData = new TaskEventData[4];
		g_ulNewCount++;
		
		x_TaskEventMineData = new TaskEventData[4];
		g_ulNewCount++;
		
		x_TaskEventTaskData = new TaskEventData[4];
		g_ulNewCount++;
		
		x_SpecialParameters = new x_SpecialParameters;
		g_ulNewCount++;
#endif
	}

	~ATaskTemplFixedData()
	{
		LOG_DELETE(m_Award_S);
		LOG_DELETE(m_Award_F);
		LOG_DELETE(m_AwByRatio_S);
		LOG_DELETE(m_AwByRatio_F);
		LOG_DELETE(m_AwByItems_S);
		LOG_DELETE(m_AwByItems_F);
		
		LOG_DELETE(x_AwByTaskEventRatio_S);
		LOG_DELETE(x_AwByTaskEventRatio_F);
		LOG_DELETE(x_AwByCarrierHp_S);
		LOG_DELETE(x_AwByCarrierHp_F);		
		LOG_DELETE_ARR(m_pszSignature);
		LOG_DELETE_ARR(m_tmStart);
		LOG_DELETE_ARR(m_tmEnd);
		LOG_DELETE_ARR(m_PremItems);
		LOG_DELETE_ARR(m_GivenItems);
		LOG_DELETE_ARR(m_TeamMemsWanted);
		LOG_DELETE_ARR(m_ItemsWanted);
		LOG_DELETE_ARR(m_PlayerWanted);
		LOG_DELETE_ARR(m_MonsterWanted);
		LOG_DELETE_ARR(m_plChangeKey);
		LOG_DELETE_ARR(m_plChangeKeyValue);
		LOG_DELETE_ARR(m_pbChangeType);
		LOG_DELETE_ARR(m_pszExp);
		LOG_DELETE_ARR(m_pExpArr);
		LOG_DELETE_ARR(m_pTaskChar);
		LOG_DELETE_ARR(m_pszPQExp);
		LOG_DELETE_ARR(m_pPQExpArr);
		LOG_DELETE_ARR(m_MonstersContrib);

		LOG_DELETE_ARR(m_pDelvRegion);
		LOG_DELETE_ARR(m_pEnterRegion);
		LOG_DELETE_ARR(m_pLeaveRegion);
		LOG_DELETE_ARR(m_pReachSite);
		LOG_DELETE_ARR(m_pLeaveSite);
		LOG_DELETE_ARR(m_PremTitles);
		LOG_DELETE_ARR(m_HomeItemsWanted);
		
		LOG_DELETE_ARR(x_TaskEventMonsterData);
		LOG_DELETE_ARR(x_TaskEventMineData);
		LOG_DELETE_ARR(x_TaskEventTaskData);
		
		LOG_DELETE_ARR(x_SpecialParameters);
	}

	bool operator == (const ATaskTemplFixedData& src) const;

	enum TREASURE_DISTANCE_LEVEL {
		DISTANCE_FAR_FAR_AWAY,
		DISTANCE_FAR,
		DISTANCE_MEDIUM,
		DISTANCE_NEAR,
		DISTANCE_VERY_NEAR,
		DISTANCE_NUM,
	};
//---------------------------------------------------------------------------------------------------//
//                                   PW_STRUCT_TASK 172
//---------------------------------------------------------------------------------------------------//
	short 					x_IDSpace; // 0 Normal - 1 Extra
	unsigned int			m_ID;
	task_char				m_szName[MAX_TASK_NAME_LEN];
	task_char*				m_pszSignature;
	unsigned int			m_ulType;
	unsigned int			m_ulTimeLimit;
	bool					m_bAbsFail;
	task_tm					m_tmAbsFailTime;
	task_tm 				x_NextFailTime; //172
	unsigned int			m_ulTimetable;
	char					m_tmType[MAX_TIMETABLE_SIZE];
	task_tm*				m_tmStart;
	task_tm*				m_tmEnd;
	int						m_lAvailFrequency;
	int						m_lPeriodLimit;
	task_tm 				x_AvailFrequencyTime;  //172
	unsigned int			m_ulMaxReceiver;
	unsigned int			m_ulDelvWorld;
	unsigned int			m_ulDelvRegionCnt;
	Task_Region*			m_pDelvRegion;	
	unsigned int			m_ulEnterRegionWorld;
	unsigned int			m_ulEnterRegionCnt;
	Task_Region*			m_pEnterRegion;	
	unsigned int			m_ulLeaveRegionWorld;
	unsigned int			m_ulLeaveRegionCnt;
	Task_Region*			m_pLeaveRegion;	
	unsigned int			m_ulTransWldId;
	ZONE_VERT				m_TransPt;
	int						m_lMonsCtrl;
	unsigned int			m_ulSuitableLevel;
	unsigned int			m_ulDelvNPC;
	unsigned int			m_ulAwardNPC;
	unsigned int   			m_ulChangeKeyCnt;
	int						*m_plChangeKey;		 
	int            			*m_plChangeKeyValue;
	bool            		*m_pbChangeType;
	int						x_PersonChangeKeyCnt;      //172
	int						*x_PersonChangeKey;         //172
	int						*x_PersonChangeKeyValue;    //172
	bool					*x_PersonChangeType;		//172
	int						x_HistoryChangeKeyCnt;     //172
	int						*x_HistoryChangeKey;        //172
	int						*x_HistoryChangeKeyValue;   //172
	bool            		*x_HistoryChangeType;	   	//172		   
	int						m_iDeliveredSkillID;
	int						m_iDeliveredSkillLevel;
	int 					m_ulInventorySlotNum;
	unsigned int			m_ulPQExpCnt;
	char					(*m_pszPQExp)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
	TASK_EXPRESSION			(*m_pPQExpArr)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
	unsigned int			m_ulMonsterContribCnt;
	MONSTERS_CONTRIB*		m_MonstersContrib;
	int						m_iPremNeedRecordTasksNum;
	int						m_iPremiseFactionContrib;
	unsigned int			m_ulAccountTaskLimitCnt;
	int 					x_FactionMem; 
	int 					x_CrossServer;                 //172
	int 					x_IconStateIDForAwardVar;      //172
	int 					x_GlobalKeyForAwardVar;        //172
	int 					x_PersonKeyForAwardVar;        //172
	int 					x_HistoryKeyForAwardVar;       //172
	int 					x_FailExp1AndOrExp2;           //172
	COMPARE_KEY_VALUE 		x_Fail1KeyValue;               //172
	COMPARE_KEY_VALUE 		x_Fail2KeyValue;               //172
	int 					x_FailPersonExp1AndOrExp2;     //174
	COMPARE_KEY_VALUE 		x_FailPerson1KeyValue;         //174
	COMPARE_KEY_VALUE 		x_FailPerson2KeyValue;         //174
	int 					x_HideOtherPlayerTime;         //172
	int 					x_PropMonsterSummonedCnt;      //172
	AWARD_MONSTERS_SUMMONED*x_PropMonsterSummoned;         //172
	int 					x_GfxType;                     //172
	unsigned char			m_ucPremiseTransformedForm;
	unsigned int			m_ulPremise_Lev_Min;
	unsigned int			m_ulPremise_Lev_Max;
	unsigned int			m_bPremCheckMaxHistoryLevel;
	unsigned int			m_ulPremReincarnationMin;
	unsigned int			m_ulPremReincarnationMax;
	unsigned int			m_ulPremRealmLevelMin;
	unsigned int			m_ulPremRealmLevelMax;
	unsigned int			m_ulPremItems;
	ITEM_WANTED*			m_PremItems;
	unsigned int			m_ulGivenItems;
	unsigned int			m_ulGivenCmnCount;
	unsigned int			m_ulGivenTskCount;
	ITEM_WANTED*			m_GivenItems;
	unsigned int			m_ulPremise_Deposit;
	int						m_lPremise_Reputation;
	int						m_lPremise_RepuMax;
	unsigned int			m_ulPremise_Task_Count;
	unsigned int			m_ulPremise_Tasks[MAX_PREM_TASK_COUNT];
	short 					x_PremiseTaskSpace[20];
	unsigned int   			m_ulPremise_Task_Least_Num;
	unsigned int			m_ulPremise_Period;
	unsigned int			m_ulPremise_Faction;
	int						m_iPremise_FactionRole;	
	unsigned int			m_ulGender;
	unsigned int			m_ulOccupations;
	unsigned int			m_Occupations[MAX_OCCUPATIONS];
	unsigned int			m_ulPremRMBMin; 
	unsigned int			m_ulPremRMBMax;
	int						m_iCharStartTime;
	int						m_iCharEndTime;
	task_tm					m_tmCharEndTime;
	unsigned int			m_ulCharTimeGreaterThan;	
	short 					x_PremiseCotaskSpace;
	unsigned int			m_ulPremise_Cotask;
	unsigned int			m_ulCoTaskCond;
	unsigned int			m_ulMutexTaskCount;
	short 					x_MutexTaskSpace[5];
	unsigned int			m_ulMutexTasks[MAX_MUTEX_TASK_COUNT];
	int						m_lSkillLev[MAX_LIVING_SKILLS];
	char					m_DynTaskType;
	unsigned int			m_ulSpecialAward;		
	float					m_fTeammateDist;		
	float					m_fSuccDist;			
	float					m_fRcvMemDist;			
	float					m_fCntMemDist;			
	unsigned int			m_ulTeamMemsWanted;
	TEAM_MEM_WANTED*		m_TeamMemsWanted;
	int				  		m_nPremExp1AndOrExp2;
	COMPARE_KEY_VALUE 		m_Prem1KeyValue;
	COMPARE_KEY_VALUE 		m_Prem2KeyValue;
	int 					x_PremPersonExp1AndOrExp2;      //172
	COMPARE_KEY_VALUE 		x_PremPerson1KeyValue;          //172
	COMPARE_KEY_VALUE 		x_PremPerson2KeyValue;          //172
	int 					x_PremHistoryExp1AndOrExp2;     //172
	COMPARE_KEY_VALUE 		x_PremHistory1KeyValue;         //172
	COMPARE_KEY_VALUE 		x_PremHistory2KeyValue;         //172
	int				  		m_iPremForce;
	int				  		m_iPremForceReputation; 
	int				  		m_iPremForceContribution;
	int				  		m_iPremForceExp;
	int				  		m_iPremForceSP;
	int				  		m_iPremForceActivityLevel;
	unsigned int	  		m_iPremTitleNumTotal;
	unsigned int	  		m_iPremTitleNumRequired;
	int*			  		m_PremTitles;
	int				  		m_iPremHistoryStageIndex[2];
	unsigned int	  		m_ulPremGeneralCardCount;
	int				  		m_iPremGeneralCardRank;
	unsigned int	  		m_ulPremGeneralCardRankCount;
	unsigned int	  		m_ulPremIconStateID;
	int						m_iVIPLevelMin;
	int				  		m_iVIPLevelMax;
	int 					m_iPremHomeLevelMin;
	int 					m_iPremHomeLevelMax;
	int 					m_iPremHomeResourceLevelType;
	int 					m_iPremHomeResourceLevelMin;
	int 					m_iPremHomeResourceLevelMax;
	int 					m_iPremHomeFactoryLevelType;
	int 					m_iPremHomeFactoryLevelMin;
	int 					m_iPremHomeFactoryLevelMax;
	int 					m_iPremHomeFlourishMin;
	int 					m_iPremHomeFlourishMax;
	int  					x_ArenaWinCount;
	int  					x_PreFactionScore;
	int  					x_PreFactionMoney;
	int  					x_PreFactionPersonScore;
	int  					x_PremMentorMorality;
	int  					x_PremMentorStudent;
	int  					x_PremCoupleIntimacy;
	int 					x_m_ulPremise_Deposit2;     //172
	int 					x_NeededTaskCount;          //172
	short					*x_NeededTaskSpace;         //172
	int						*x_NeededTask;              //172
	char 					x_NeededTaskLeast;          //172
	int 					x_PremCamp;                 //172
	int 					x_PremCampPower;            //172
	int 					x_NeedFriendPoint;          //172
	int 					x_RankType;                 //172
	short 					x_RankGradeMin;             //172
	short 					x_RankGradeMax;             //172
	short 					x_RankGradeMinKey;          //172
	short 					x_RankGradeMaxKey;          //172
	unsigned int			m_enumMethod;
	unsigned int			m_enumFinishType;
	unsigned int			m_ulPlayerWanted;
	PLAYER_WANTED*  		m_PlayerWanted;
	unsigned int			m_ulMonsterWanted;
	MONSTER_WANTED*			m_MonsterWanted;	
	unsigned int			m_ulItemsWanted;
	ITEM_WANTED*			m_ItemsWanted;
	unsigned int			m_ulGoldWanted;	
	int						m_iFactionContribWanted;
	int						m_iFactionExpContribWanted;	
	unsigned int			m_ulNPCToProtect;
	unsigned int			m_ulProtectTimeLen;	
	unsigned int			m_ulNPCMoving;
	unsigned int			m_ulNPCDestSite;	
	Task_Region*			m_pReachSite;
	unsigned int			m_ulReachSiteCnt;
	unsigned int			m_ulReachSiteId;
	unsigned int			m_ulWaitTime;	
	ZONE_VERT				m_TreasureStartZone;
	unsigned char			m_ucZonesNumX;
	unsigned char			m_ucZonesNumZ;
	unsigned char   		m_ucZoneSide;	
	short 					x_treasureZoneMapID;
	Task_Region*			m_pLeaveSite;
	unsigned int			m_ulLeaveSiteCnt;
	unsigned int			m_ulLeaveSiteId;	
	int				  		m_nFinExp1AndOrExp2;
	COMPARE_KEY_VALUE 		m_Fin1KeyValue;
	COMPARE_KEY_VALUE 		m_Fin2KeyValue;		
	int 					x_FinPersonExp1AndOrExp2;        //172
	COMPARE_KEY_VALUE 		x_FinPerson1KeyValue;            //172
	COMPARE_KEY_VALUE 		x_FinPerson2KeyValue;            //172
	int 					x_FinHistoryExp1AndOrExp2;       //172
	COMPARE_KEY_VALUE 		x_FinHistory1KeyValue;           //172
	COMPARE_KEY_VALUE 		x_FinHistory2KeyValue;           //172
	unsigned int			m_ulExpCnt;
	char					(*m_pszExp)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
	TASK_EXPRESSION 		(*m_pExpArr)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
	int 					x_PersonExpCnt;                                       //172
	char	 				(*x_PersonExp)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];      //172
	TASK_EXPRESSION			(*x_PersonExpArr)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];   //172
	int 					x_HistoryExpCnt;                                      //172
	char	 				(*x_HistoryExp)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];     //172
	TASK_EXPRESSION			(*x_HistoryExpArr)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];  //172
	unsigned int       		m_ulTaskCharCnt;
	task_char           	(*m_pTaskChar)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
	int 					x_PersonTaskCharCnt;                                     //172
	task_char 				(*x_PersonTaskChar)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];    //172
	int 					x_HistoryTaskCharCnt;                                    //172
	task_char 				(*x_HistoryTaskChar)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];   //172
	unsigned char			m_ucTransformedForm;
	int 					x_TransformedDescID;
	unsigned int			m_ulReachLevel;
	unsigned int			m_ulReachReincarnationCount;
	unsigned int			m_ulReachRealmLevel;	
	unsigned int			m_uiEmotion;	
	int 					x_GrowthTarget;
	unsigned int			m_ulTMIconStateID;
	int 					x_TMIconStateDescID;
	unsigned int 			m_ulTMHomeLevelType;
	unsigned int 			m_ulTMReachHomeLevel;
	unsigned int 			m_ulTMReachHomeFlourish;
	unsigned int 			m_ulHomeItemsWanted;
	HOME_ITEM_WANTED*   	m_HomeItemsWanted;
	int 					x_TMBindCarrierId;
	short 					x_TMReachArenaTime;
	short 					x_ReachArenaWinCount;
	short 					x_Dialog[20];
	int 					x_SuccTriggerEvent;    //172
	int 					x_VerifyStateLayer;    //172
	int 					x_VerifyStateLayerID;  //172
	int 					x_KeyWordIndex;        //172

	// Novo Evento
	short 					x_TaskEventChar[30];
	int   					x_TaskEventType; // Tipo do Evento
	short 					x_TaskEventTime; // Quantidade Necessária

	short 					x_TaskEventMonsterCount;
	EVENT_TASK_DATA* 		x_TaskEventMonsterData;
	short 					x_TaskEventMineCount;
	EVENT_TASK_DATA* 		x_TaskEventMineData;
	short 					x_TaskEventTaskCount;
	EVENT_TASK_DATA* 		x_TaskEventTaskData;

	short 					x_FactionPersonScore;
	char 					x_TMTinyGameType;
	int 					x_m_ulGold2Wanted;   //172
	int 					x_BarrageChannel;    //172
	int 					x_GuessInputID;      //174
	unsigned int			m_ulAwardType_S;
	unsigned int			m_ulAwardType_F;
	AWARD_DATA* 			m_Award_S;
	AWARD_DATA* 			m_Award_F;
	AWARD_RATIO_SCALE* 		m_AwByRatio_S;
	AWARD_RATIO_SCALE* 		m_AwByRatio_F;	
	AWARD_ITEMS_SCALE* 		m_AwByItems_S;
	AWARD_ITEMS_SCALE* 		m_AwByItems_F;	
	AWARD_RATIO_SCALE* 		x_AwByTaskEventRatio_S;
	AWARD_RATIO_SCALE* 		x_AwByTaskEventRatio_F;
	AWARD_CARRIER_HP_SCALE* x_AwByCarrierHp_S;
	AWARD_CARRIER_HP_SCALE* x_AwByCarrierHp_F;
	AWARD_VAR_SCALE* 		x_AwByVar_S; 
	AWARD_VAR_SCALE* 		x_AwByVar_F; 
	unsigned int			m_ulParent;
	unsigned int			m_ulPrevSibling;
	unsigned int			m_ulNextSibling;
	unsigned int			m_ulFirstChild;	
	float					m_fLibraryTasksProbability;
	int						m_iWorldContribution;
	char					x_CombinedSwitches[MAX_COMBINED_SWITCHES];            	//172
	int 					x_SpecialParametersCount;      	//172
	SPECIAL_PARAMETERS* 	x_SpecialParameters;			//172
//---------------------------------------------------------------------------------------------------//
//--172 special params GET
//---------------------------------------------------------------------------------------------------//
	inline bool m_bShowByTransformed            () const  { return ((x_CombinedSwitches[0 ] >> 0 ) & 1u); }
	inline bool m_bShowByLev                    () const  { return ((x_CombinedSwitches[0 ] >> 1 ) & 1u); }
	inline bool m_bPremCheckReincarnation       () const  { return ((x_CombinedSwitches[0 ] >> 2 ) & 1u); }
	inline bool m_bShowByReincarnation          () const  { return ((x_CombinedSwitches[0 ] >> 3 ) & 1u); }
	inline bool m_bPremCheckRealmLevel          () const  { return ((x_CombinedSwitches[0 ] >> 4 ) & 1u); }
	inline bool m_bPremCheckRealmExpFull        () const  { return ((x_CombinedSwitches[0 ] >> 5 ) & 1u); }
	inline bool m_bShowByRealmLevel             () const  { return ((x_CombinedSwitches[0 ] >> 6 ) & 1u); }
	inline bool m_bShowByItems                  () const  { return ((x_CombinedSwitches[0 ] >> 7 ) & 1u); }
	inline bool m_bPremItemsAnyOne              () const  { return ((x_CombinedSwitches[1 ] >> 0 ) & 1u); }
	inline bool NotOpenWithItem                 () const  { return ((x_CombinedSwitches[1 ] >> 1 ) & 1u); }
	inline bool m_bShowByDeposit                () const  { return ((x_CombinedSwitches[1 ] >> 2 ) & 1u); }
	inline bool m_bShowByRepu                   () const  { return ((x_CombinedSwitches[1 ] >> 3 ) & 1u); }
	inline bool m_bShowByPreTask                () const  { return ((x_CombinedSwitches[1 ] >> 4 ) & 1u); }
	inline bool m_bShowByPeriod                 () const  { return ((x_CombinedSwitches[1 ] >> 5 ) & 1u); }
	inline bool m_bShowByFaction                () const  { return ((x_CombinedSwitches[1 ] >> 6 ) & 1u); }
	inline bool m_bShowByGender                 () const  { return ((x_CombinedSwitches[1 ] >> 7 ) & 1u); }
	inline bool m_bShowByOccup                  () const  { return ((x_CombinedSwitches[2 ] >> 0 ) & 1u); }
	inline bool m_bPremise_Spouse               () const  { return ((x_CombinedSwitches[2 ] >> 1 ) & 1u); }
	inline bool m_bShowBySpouse                 () const  { return ((x_CombinedSwitches[2 ] >> 2 ) & 1u); }
	inline bool m_bPremiseWeddingOwner          () const  { return ((x_CombinedSwitches[2 ] >> 3 ) & 1u); }
	inline bool m_bShowByWeddingOwner           () const  { return ((x_CombinedSwitches[2 ] >> 4 ) & 1u); }
	inline bool m_bGM                           () const  { return ((x_CombinedSwitches[2 ] >> 5 ) & 1u); }
	inline bool m_bShieldUser                   () const  { return ((x_CombinedSwitches[2 ] >> 6 ) & 1u); }
	inline bool m_bShowByRMB                    () const  { return ((x_CombinedSwitches[2 ] >> 7 ) & 1u); }
	inline bool m_bCharTime                     () const  { return ((x_CombinedSwitches[3 ] >> 0 ) & 1u); }
	inline bool m_bShowByCharTime               () const  { return ((x_CombinedSwitches[3 ] >> 1 ) & 1u); }
	inline bool m_bTeamwork                     () const  { return ((x_CombinedSwitches[3 ] >> 2 ) & 1u); }
	inline bool m_bRcvByTeam                    () const  { return ((x_CombinedSwitches[3 ] >> 3 ) & 1u); }
	inline bool m_bSharedTask                   () const  { return ((x_CombinedSwitches[3 ] >> 4 ) & 1u); }
	inline bool m_bSharedAchieved               () const  { return ((x_CombinedSwitches[3 ] >> 5 ) & 1u); }
	inline bool m_bCheckTeammate                () const  { return ((x_CombinedSwitches[3 ] >> 6 ) & 1u); }
	inline bool m_bAllFail                      () const  { return ((x_CombinedSwitches[3 ] >> 7 ) & 1u); }
	//---------------------------------------------------------------------------------------------------//
	inline bool m_bCapFail                      () const  { return ((x_CombinedSwitches[4 ] >> 0 ) & 1u); }
	inline bool m_bCapSucc                      () const  { return ((x_CombinedSwitches[4 ] >> 1 ) & 1u); }
	inline bool m_bDismAsSelfFail               () const  { return ((x_CombinedSwitches[4 ] >> 2 ) & 1u); }
	inline bool m_bRcvChckMem                   () const  { return ((x_CombinedSwitches[4 ] >> 3 ) & 1u); }
	inline bool m_bCntByMemPos                  () const  { return ((x_CombinedSwitches[4 ] >> 4 ) & 1u); }
	inline bool m_bAllSucc                      () const  { return ((x_CombinedSwitches[4 ] >> 5 ) & 1u); }
	inline bool m_bCoupleOnly                   () const  { return ((x_CombinedSwitches[4 ] >> 6 ) & 1u); }
	inline bool m_bDistinguishedOcc             () const  { return ((x_CombinedSwitches[4 ] >> 7 ) & 1u); }
	inline bool m_bShowByTeam                   () const  { return ((x_CombinedSwitches[5 ] >> 0 ) & 1u); }
	inline bool m_bPremNeedComp                 () const  { return ((x_CombinedSwitches[5 ] >> 1 ) & 1u); }
	inline bool PremNeedCompPerson              () const  { return ((x_CombinedSwitches[5 ] >> 2 ) & 1u); }
	inline bool PremNeedCompHistory             () const  { return ((x_CombinedSwitches[5 ] >> 3 ) & 1u); }
	inline bool m_bPremCheckForce               () const  { return ((x_CombinedSwitches[5 ] >> 4 ) & 1u); }
	inline bool m_bShowByForce                  () const  { return ((x_CombinedSwitches[5 ] >> 5 ) & 1u); }
	inline bool m_bShowByForceReputation        () const  { return ((x_CombinedSwitches[5 ] >> 6 ) & 1u); }
	inline bool m_bShowByForceContribution      () const  { return ((x_CombinedSwitches[5 ] >> 7 ) & 1u); }
	inline bool m_bShowByForceExp               () const  { return ((x_CombinedSwitches[6 ] >> 0 ) & 1u); }
	inline bool m_bShowByForceSP                () const  { return ((x_CombinedSwitches[6 ] >> 1 ) & 1u); }
	inline bool m_bShowByForceActivityLevel     () const  { return ((x_CombinedSwitches[6 ] >> 2 ) & 1u); }
	inline bool m_bPremIsKing                   () const  { return ((x_CombinedSwitches[6 ] >> 3 ) & 1u); }
	inline bool m_bShowByKing                   () const  { return ((x_CombinedSwitches[6 ] >> 4 ) & 1u); }
	inline bool m_bPremNotInTeam                () const  { return ((x_CombinedSwitches[6 ] >> 5 ) & 1u); }
	inline bool m_bShowByNotInTeam              () const  { return ((x_CombinedSwitches[6 ] >> 6 ) & 1u); }
	inline bool m_bShowByTitle                  () const  { return ((x_CombinedSwitches[6 ] >> 7 ) & 1u); }
	inline bool m_bShowByHistoryStage           () const  { return ((x_CombinedSwitches[7 ] >> 0 ) & 1u); }
	inline bool m_bShowByGeneralCard            () const  { return ((x_CombinedSwitches[7 ] >> 1 ) & 1u); }
	inline bool m_bShowByGeneralCardRank        () const  { return ((x_CombinedSwitches[7 ] >> 2 ) & 1u); }
	inline bool m_bShowByIconStateID            () const  { return ((x_CombinedSwitches[7 ] >> 3 ) & 1u); }
	inline bool m_bShowByVIPLevel               () const  { return ((x_CombinedSwitches[7 ] >> 4 ) & 1u); }
	inline bool PremNoHome                      () const  { return ((x_CombinedSwitches[7 ] >> 5 ) & 1u); }
	inline bool m_bShowByHomeLevel              () const  { return ((x_CombinedSwitches[7 ] >> 6 ) & 1u); }
	inline bool m_bShowByHomeResourceLevel      () const  { return ((x_CombinedSwitches[7 ] >> 7 ) & 1u); }
//---------------------------------------------------------------------------------------------------//
	inline bool m_bShowByHomeFactoryLevel       () const  { return ((x_CombinedSwitches[8 ] >> 0 ) & 1u); }
	inline bool m_bShowByHomeFlourish           () const  { return ((x_CombinedSwitches[8 ] >> 1 ) & 1u); }
	inline bool m_bPremHomeStorageTask          () const  { return ((x_CombinedSwitches[8 ] >> 2 ) & 1u); }
	inline bool m_bShowByHomeStorageTask        () const  { return ((x_CombinedSwitches[8 ] >> 3 ) & 1u); }
	inline bool MonsterTotalCount               () const  { return ((x_CombinedSwitches[8 ] >> 4 ) & 1u); }
	inline bool m_bFinNeedComp                  () const  { return ((x_CombinedSwitches[8 ] >> 5 ) & 1u); }
	inline bool FinNeedCompPerson               () const  { return ((x_CombinedSwitches[8 ] >> 6 ) & 1u); }
	inline bool FinNeedCompHistory              () const  { return ((x_CombinedSwitches[8 ] >> 7 ) & 1u); }
	inline bool m_bIsLibraryTask                () const  { return ((x_CombinedSwitches[9 ] >> 0 ) & 1u); }
	inline bool m_bIsUniqueStorageTask          () const  { return ((x_CombinedSwitches[9 ] >> 1 ) & 1u); }
	inline bool m_bHasSign                      () const  { return ((x_CombinedSwitches[9 ] >> 2 ) & 1u); }
	inline bool m_bOfflineFail                  () const  { return ((x_CombinedSwitches[9 ] >> 3 ) & 1u); }
	inline bool m_bItemNotTakeOff               () const  { return ((x_CombinedSwitches[9 ] >> 4 ) & 1u); }
	inline bool m_bAbsTime                      () const  { return ((x_CombinedSwitches[9 ] >> 5 ) & 1u); }
	inline bool MultiNpcTalk                    () const  { return ((x_CombinedSwitches[9 ] >> 6 ) & 1u); }
	inline bool m_bChooseOne                    () const  { return ((x_CombinedSwitches[9 ] >> 7 ) & 1u); }
	inline bool m_bRandOne                      () const  { return ((x_CombinedSwitches[10] >> 0 ) & 1u); }
	inline bool m_bExeChildInOrder              () const  { return ((x_CombinedSwitches[10] >> 1 ) & 1u); }
	inline bool m_bParentAlsoFail               () const  { return ((x_CombinedSwitches[10] >> 2 ) & 1u); }
	inline bool m_bParentAlsoSucc               () const  { return ((x_CombinedSwitches[10] >> 3 ) & 1u); }
	inline bool m_bCanGiveUp                    () const  { return ((x_CombinedSwitches[10] >> 4 ) & 1u); }
	inline bool m_bCanRedo                      () const  { return ((x_CombinedSwitches[10] >> 5 ) & 1u); }
	inline bool m_bCanRedoAfterFailure          () const  { return ((x_CombinedSwitches[10] >> 6 ) & 1u); }
	inline bool m_bClearAsGiveUp                () const  { return ((x_CombinedSwitches[10] >> 7 ) & 1u); }
	inline bool m_bNeedRecord                   () const  { return ((x_CombinedSwitches[11] >> 0 ) & 1u); }
	inline bool m_bFailAsPlayerDie              () const  { return ((x_CombinedSwitches[11] >> 1 ) & 1u); }
	inline bool m_bDelvInZone                   () const  { return ((x_CombinedSwitches[11] >> 2 ) & 1u); }
	inline bool m_bEnterRegionFail              () const  { return ((x_CombinedSwitches[11] >> 3 ) & 1u); }
	inline bool m_bLeaveRegionFail              () const  { return ((x_CombinedSwitches[11] >> 4 ) & 1u); }
	inline bool m_bLeaveForceFail               () const  { return ((x_CombinedSwitches[11] >> 5 ) & 1u); }
	inline bool m_bTransTo                      () const  { return ((x_CombinedSwitches[11] >> 6 ) & 1u); }
	inline bool m_bTrigCtrl                     () const  { return ((x_CombinedSwitches[11] >> 7 ) & 1u); }
//---------------------------------------------------------------------------------------------------//
	inline bool m_bAutoDeliver                  () const  { return ((x_CombinedSwitches[12] >> 0 ) & 1u); }
	inline bool m_bDisplayInExclusiveUI         () const  { return ((x_CombinedSwitches[12] >> 1 ) & 1u); }
	inline bool m_bReadyToNotifyServer          () const  { return ((x_CombinedSwitches[12] >> 2 ) & 1u); }
	inline bool m_bUsedInTokenShop              () const  { return ((x_CombinedSwitches[12] >> 3 ) & 1u); }
	inline bool m_bDeathTrig                    () const  { return ((x_CombinedSwitches[12] >> 4 ) & 1u); }
	inline bool m_bClearAcquired                () const  { return ((x_CombinedSwitches[12] >> 5 ) & 1u); }
	inline bool m_bShowPrompt                   () const  { return ((x_CombinedSwitches[12] >> 6 ) & 1u); }
	inline bool m_bKeyTask                      () const  { return ((x_CombinedSwitches[12] >> 7 ) & 1u); }
	inline bool m_bSkillTask                    () const  { return ((x_CombinedSwitches[13] >> 0 ) & 1u); }
	inline bool m_bCanSeekOut                   () const  { return ((x_CombinedSwitches[13] >> 1 ) & 1u); }
	inline bool m_bShowDirection                () const  { return ((x_CombinedSwitches[13] >> 2 ) & 1u); }
	inline bool m_bMarriage                     () const  { return ((x_CombinedSwitches[13] >> 3 ) & 1u); }
	inline bool m_bSwitchSceneFail              () const  { return ((x_CombinedSwitches[13] >> 4 ) & 1u); }
	inline bool m_bHidden                       () const  { return ((x_CombinedSwitches[13] >> 5 ) & 1u); }
	inline bool m_bDeliverySkill                () const  { return ((x_CombinedSwitches[13] >> 6 ) & 1u); }
	inline bool m_bShowGfxFinished              () const  { return ((x_CombinedSwitches[13] >> 7 ) & 1u); }
	inline bool m_bChangePQRanking              () const  { return ((x_CombinedSwitches[14] >> 0 ) & 1u); }
	inline bool m_bCompareItemAndInventory      () const  { return ((x_CombinedSwitches[14] >> 1 ) & 1u); }
	inline bool m_bTowerTask                    () const  { return ((x_CombinedSwitches[14] >> 2 ) & 1u); }
	inline bool m_bHomeTask                     () const  { return ((x_CombinedSwitches[14] >> 3 ) & 1u); }
	inline bool m_bDeliverInHostHome            () const  { return ((x_CombinedSwitches[14] >> 4 ) & 1u); }
	inline bool m_bFinishInHostHome             () const  { return ((x_CombinedSwitches[14] >> 5 ) & 1u); }
	inline bool m_bPQTask                       () const  { return ((x_CombinedSwitches[14] >> 6 ) & 1u); }
	inline bool m_bPQSubTask                    () const  { return ((x_CombinedSwitches[14] >> 7 ) & 1u); }
	inline bool m_bClearContrib                 () const  { return ((x_CombinedSwitches[15] >> 0 ) & 1u); }
	inline bool m_bShowByNeedRecordTasksNum     () const  { return ((x_CombinedSwitches[15] >> 1 ) & 1u); }
	inline bool m_bShowByFactionContrib         () const  { return ((x_CombinedSwitches[15] >> 2 ) & 1u); }
	inline bool m_bAccountTaskLimit             () const  { return ((x_CombinedSwitches[15] >> 3 ) & 1u); }
	inline bool m_bRoleTaskLimit                () const  { return ((x_CombinedSwitches[15] >> 4 ) & 1u); }
	inline bool m_bLeaveFactionFail             () const  { return ((x_CombinedSwitches[15] >> 5 ) & 1u); }
	inline bool m_bNotIncCntWhenFailed          () const  { return ((x_CombinedSwitches[15] >> 6 ) & 1u); }
	inline bool m_bNotClearItemWhenFailed       () const  { return ((x_CombinedSwitches[15] >> 7 ) & 1u); }
//---------------------------------------------------------------------------------------------------//
	inline bool m_bDisplayInTitleTaskUI         () const  { return ((x_CombinedSwitches[16] >> 0 ) & 1u); }
	inline bool CanAwardOnCarrier               () const  { return ((x_CombinedSwitches[16] >> 1 ) & 1u); }
	inline bool PreReachArenaWinCount           () const  { return ((x_CombinedSwitches[16] >> 2 ) & 1u); }
	inline bool ShowByReachArenaWinCount        () const  { return ((x_CombinedSwitches[16] >> 3 ) & 1u); }
	inline bool WriteSpecialLog                 () const  { return ((x_CombinedSwitches[16] >> 4 ) & 1u); }
	inline bool ShowByPreFactionScore           () const  { return ((x_CombinedSwitches[16] >> 5 ) & 1u); }
	inline bool ShowByPreFactionMoney           () const  { return ((x_CombinedSwitches[16] >> 6 ) & 1u); }
	inline bool ShowByPreFactionPersonScore     () const  { return ((x_CombinedSwitches[16] >> 7 ) & 1u); }
	inline bool CheckFactionMem                 () const  { return ((x_CombinedSwitches[17] >> 0 ) & 1u); }
	inline bool ByFactionScore                  () const  { return ((x_CombinedSwitches[17] >> 1 ) & 1u); }
	inline bool SpecailLogicTask                () const  { return ((x_CombinedSwitches[17] >> 2 ) & 1u); }
	inline bool BecomeMentor                    () const  { return ((x_CombinedSwitches[17] >> 3 ) & 1u); }
	inline bool ShowMentorMorality              () const  { return ((x_CombinedSwitches[17] >> 4 ) & 1u); }
	inline bool ShowMentorStudent               () const  { return ((x_CombinedSwitches[17] >> 5 ) & 1u); }
	inline bool DiscardedTask                   () const  { return ((x_CombinedSwitches[17] >> 6 ) & 1u); }
	inline bool NoCheckTitleGot                 () const  { return ((x_CombinedSwitches[17] >> 7 ) & 1u); }
	inline bool ShowCoupleIntimacy              () const  { return ((x_CombinedSwitches[18] >> 0 ) & 1u); }
	inline bool m_bShowByDeposit2               () const  { return ((x_CombinedSwitches[18] >> 1 ) & 1u); }
	inline bool RecordFinishCount               () const  { return ((x_CombinedSwitches[18] >> 2 ) & 1u); }
	inline bool ShowCamp                        () const  { return ((x_CombinedSwitches[18] >> 3 ) & 1u); }
	inline bool ShowCampPower                   () const  { return ((x_CombinedSwitches[18] >> 4 ) & 1u); }
	inline bool ReturnCampPower                 () const  { return ((x_CombinedSwitches[18] >> 5 ) & 1u); }
	inline bool TeamHaveRecaller                () const  { return ((x_CombinedSwitches[18] >> 6 ) & 1u); }
	inline bool BindedRecaller                  () const  { return ((x_CombinedSwitches[18] >> 7 ) & 1u); }
	inline bool ShowNeedFriendPoint             () const  { return ((x_CombinedSwitches[19] >> 0 ) & 1u); }
	inline bool HistoryVarTask                  () const  { return ((x_CombinedSwitches[19] >> 1 ) & 1u); }
	inline bool PremRank                        () const  { return ((x_CombinedSwitches[19] >> 2 ) & 1u); }
	inline bool DeliverByUI                     () const  { return ((x_CombinedSwitches[19] >> 3 ) & 1u); }
	inline bool WarTaskAward                    () const  { return ((x_CombinedSwitches[19] >> 4 ) & 1u); }
	inline bool HideOtherPlayer                 () const  { return ((x_CombinedSwitches[19] >> 5 ) & 1u); }
	inline bool RecordDeliverTime               () const  { return ((x_CombinedSwitches[19] >> 6 ) & 1u); }
//---------------------------------------------------------------------------------------------------//
//--172 special params SET
//---------------------------------------------------------------------------------------------------//
	inline void m_bShowByTransformed            (bool ptr)   { (ptr) ? x_CombinedSwitches[0 ]|=1<<0  : x_CombinedSwitches[0 ]&=~(1<<0  ); }
	inline void m_bShowByLev                    (bool ptr)   { (ptr) ? x_CombinedSwitches[0 ]|=1<<1  : x_CombinedSwitches[0 ]&=~(1<<1  ); }
	inline void m_bPremCheckReincarnation       (bool ptr)   { (ptr) ? x_CombinedSwitches[0 ]|=1<<2  : x_CombinedSwitches[0 ]&=~(1<<2  ); }
	inline void m_bShowByReincarnation          (bool ptr)   { (ptr) ? x_CombinedSwitches[0 ]|=1<<3  : x_CombinedSwitches[0 ]&=~(1<<3  ); }
	inline void m_bPremCheckRealmLevel          (bool ptr)   { (ptr) ? x_CombinedSwitches[0 ]|=1<<4  : x_CombinedSwitches[0 ]&=~(1<<4  ); }
	inline void m_bPremCheckRealmExpFull        (bool ptr)   { (ptr) ? x_CombinedSwitches[0 ]|=1<<5  : x_CombinedSwitches[0 ]&=~(1<<5  ); }
	inline void m_bShowByRealmLevel             (bool ptr)   { (ptr) ? x_CombinedSwitches[0 ]|=1<<6  : x_CombinedSwitches[0 ]&=~(1<<6  ); }
	inline void m_bShowByItems                  (bool ptr)   { (ptr) ? x_CombinedSwitches[0 ]|=1<<7  : x_CombinedSwitches[0 ]&=~(1<<7  ); }
	inline void m_bPremItemsAnyOne              (bool ptr)   { (ptr) ? x_CombinedSwitches[1 ]|=1<<0  : x_CombinedSwitches[1 ]&=~(1<<0  ); }
	inline void NotOpenWithItem                 (bool ptr)   { (ptr) ? x_CombinedSwitches[1 ]|=1<<1  : x_CombinedSwitches[1 ]&=~(1<<1  ); }
	inline void m_bShowByDeposit                (bool ptr)   { (ptr) ? x_CombinedSwitches[1 ]|=1<<2  : x_CombinedSwitches[1 ]&=~(1<<2  ); }
	inline void m_bShowByRepu                   (bool ptr)   { (ptr) ? x_CombinedSwitches[1 ]|=1<<3  : x_CombinedSwitches[1 ]&=~(1<<3  ); }
	inline void m_bShowByPreTask                (bool ptr)   { (ptr) ? x_CombinedSwitches[1 ]|=1<<4  : x_CombinedSwitches[1 ]&=~(1<<4  ); }
	inline void m_bShowByPeriod                 (bool ptr)   { (ptr) ? x_CombinedSwitches[1 ]|=1<<5  : x_CombinedSwitches[1 ]&=~(1<<5  ); }
	inline void m_bShowByFaction                (bool ptr)   { (ptr) ? x_CombinedSwitches[1 ]|=1<<6  : x_CombinedSwitches[1 ]&=~(1<<6  ); }
	inline void m_bShowByGender                 (bool ptr)   { (ptr) ? x_CombinedSwitches[1 ]|=1<<7  : x_CombinedSwitches[1 ]&=~(1<<7  ); }
	inline void m_bShowByOccup                  (bool ptr)   { (ptr) ? x_CombinedSwitches[2 ]|=1<<0  : x_CombinedSwitches[2 ]&=~(1<<0  ); }
	inline void m_bPremise_Spouse               (bool ptr)   { (ptr) ? x_CombinedSwitches[2 ]|=1<<1  : x_CombinedSwitches[2 ]&=~(1<<1  ); }
	inline void m_bShowBySpouse                 (bool ptr)   { (ptr) ? x_CombinedSwitches[2 ]|=1<<2  : x_CombinedSwitches[2 ]&=~(1<<2  ); }
	inline void m_bPremiseWeddingOwner          (bool ptr)   { (ptr) ? x_CombinedSwitches[2 ]|=1<<3  : x_CombinedSwitches[2 ]&=~(1<<3  ); }
	inline void m_bShowByWeddingOwner           (bool ptr)   { (ptr) ? x_CombinedSwitches[2 ]|=1<<4  : x_CombinedSwitches[2 ]&=~(1<<4  ); }
	inline void m_bGM                           (bool ptr)   { (ptr) ? x_CombinedSwitches[2 ]|=1<<5  : x_CombinedSwitches[2 ]&=~(1<<5  ); }
	inline void m_bShieldUser                   (bool ptr)   { (ptr) ? x_CombinedSwitches[2 ]|=1<<6  : x_CombinedSwitches[2 ]&=~(1<<6  ); }
	inline void m_bShowByRMB                    (bool ptr)   { (ptr) ? x_CombinedSwitches[2 ]|=1<<7  : x_CombinedSwitches[2 ]&=~(1<<7  ); }
	inline void m_bCharTime                     (bool ptr)   { (ptr) ? x_CombinedSwitches[3 ]|=1<<0  : x_CombinedSwitches[3 ]&=~(1<<0  ); }
	inline void m_bShowByCharTime               (bool ptr)   { (ptr) ? x_CombinedSwitches[3 ]|=1<<1  : x_CombinedSwitches[3 ]&=~(1<<1  ); }
	inline void m_bTeamwork                     (bool ptr)   { (ptr) ? x_CombinedSwitches[3 ]|=1<<2  : x_CombinedSwitches[3 ]&=~(1<<2  ); }
	inline void m_bRcvByTeam                    (bool ptr)   { (ptr) ? x_CombinedSwitches[3 ]|=1<<3  : x_CombinedSwitches[3 ]&=~(1<<3  ); }
	inline void m_bSharedTask                   (bool ptr)   { (ptr) ? x_CombinedSwitches[3 ]|=1<<4  : x_CombinedSwitches[3 ]&=~(1<<4  ); }
	inline void m_bSharedAchieved               (bool ptr)   { (ptr) ? x_CombinedSwitches[3 ]|=1<<5  : x_CombinedSwitches[3 ]&=~(1<<5  ); }
	inline void m_bCheckTeammate                (bool ptr)   { (ptr) ? x_CombinedSwitches[3 ]|=1<<6  : x_CombinedSwitches[3 ]&=~(1<<6  ); }
	inline void m_bAllFail                      (bool ptr)   { (ptr) ? x_CombinedSwitches[3 ]|=1<<7  : x_CombinedSwitches[3 ]&=~(1<<7  ); }
	//--------------------------------------------------------------------------------------------------------------------------------------//
	inline void m_bCapFail                      (bool ptr)   { (ptr) ? x_CombinedSwitches[4 ]|=1<<0  : x_CombinedSwitches[4 ]&=~(1<<0  ); }
	inline void m_bCapSucc                      (bool ptr)   { (ptr) ? x_CombinedSwitches[4 ]|=1<<1  : x_CombinedSwitches[4 ]&=~(1<<1  ); }
	inline void m_bDismAsSelfFail               (bool ptr)   { (ptr) ? x_CombinedSwitches[4 ]|=1<<2  : x_CombinedSwitches[4 ]&=~(1<<2  ); }
	inline void m_bRcvChckMem                   (bool ptr)   { (ptr) ? x_CombinedSwitches[4 ]|=1<<3  : x_CombinedSwitches[4 ]&=~(1<<3  ); }
	inline void m_bCntByMemPos                  (bool ptr)   { (ptr) ? x_CombinedSwitches[4 ]|=1<<4  : x_CombinedSwitches[4 ]&=~(1<<4  ); }
	inline void m_bAllSucc                      (bool ptr)   { (ptr) ? x_CombinedSwitches[4 ]|=1<<5  : x_CombinedSwitches[4 ]&=~(1<<5  ); }
	inline void m_bCoupleOnly                   (bool ptr)   { (ptr) ? x_CombinedSwitches[4 ]|=1<<6  : x_CombinedSwitches[4 ]&=~(1<<6  ); }
	inline void m_bDistinguishedOcc             (bool ptr)   { (ptr) ? x_CombinedSwitches[4 ]|=1<<7  : x_CombinedSwitches[4 ]&=~(1<<7  ); }
	inline void m_bShowByTeam                   (bool ptr)   { (ptr) ? x_CombinedSwitches[5 ]|=1<<0  : x_CombinedSwitches[5 ]&=~(1<<0  ); }
	inline void m_bPremNeedComp                 (bool ptr)   { (ptr) ? x_CombinedSwitches[5 ]|=1<<1  : x_CombinedSwitches[5 ]&=~(1<<1  ); }
	inline void PremNeedCompPerson              (bool ptr)   { (ptr) ? x_CombinedSwitches[5 ]|=1<<2  : x_CombinedSwitches[5 ]&=~(1<<2  ); }
	inline void PremNeedCompHistory             (bool ptr)   { (ptr) ? x_CombinedSwitches[5 ]|=1<<3  : x_CombinedSwitches[5 ]&=~(1<<3  ); }
	inline void m_bPremCheckForce               (bool ptr)   { (ptr) ? x_CombinedSwitches[5 ]|=1<<4  : x_CombinedSwitches[5 ]&=~(1<<4  ); }
	inline void m_bShowByForce                  (bool ptr)   { (ptr) ? x_CombinedSwitches[5 ]|=1<<5  : x_CombinedSwitches[5 ]&=~(1<<5  ); }
	inline void m_bShowByForceReputation        (bool ptr)   { (ptr) ? x_CombinedSwitches[5 ]|=1<<6  : x_CombinedSwitches[5 ]&=~(1<<6  ); }
	inline void m_bShowByForceContribution      (bool ptr)   { (ptr) ? x_CombinedSwitches[5 ]|=1<<7  : x_CombinedSwitches[5 ]&=~(1<<7  ); }
	inline void m_bShowByForceExp               (bool ptr)   { (ptr) ? x_CombinedSwitches[6 ]|=1<<0  : x_CombinedSwitches[6 ]&=~(1<<0  ); }
	inline void m_bShowByForceSP                (bool ptr)   { (ptr) ? x_CombinedSwitches[6 ]|=1<<1  : x_CombinedSwitches[6 ]&=~(1<<1  ); }
	inline void m_bShowByForceActivityLevel     (bool ptr)   { (ptr) ? x_CombinedSwitches[6 ]|=1<<2  : x_CombinedSwitches[6 ]&=~(1<<2  ); }
	inline void m_bPremIsKing                   (bool ptr)   { (ptr) ? x_CombinedSwitches[6 ]|=1<<3  : x_CombinedSwitches[6 ]&=~(1<<3  ); }
	inline void m_bShowByKing                   (bool ptr)   { (ptr) ? x_CombinedSwitches[6 ]|=1<<4  : x_CombinedSwitches[6 ]&=~(1<<4  ); }
	inline void m_bPremNotInTeam                (bool ptr)   { (ptr) ? x_CombinedSwitches[6 ]|=1<<5  : x_CombinedSwitches[6 ]&=~(1<<5  ); }
	inline void m_bShowByNotInTeam              (bool ptr)   { (ptr) ? x_CombinedSwitches[6 ]|=1<<6  : x_CombinedSwitches[6 ]&=~(1<<6  ); }
	inline void m_bShowByTitle                  (bool ptr)   { (ptr) ? x_CombinedSwitches[6 ]|=1<<7  : x_CombinedSwitches[6 ]&=~(1<<7  ); }
	inline void m_bShowByHistoryStage           (bool ptr)   { (ptr) ? x_CombinedSwitches[7 ]|=1<<0  : x_CombinedSwitches[7 ]&=~(1<<0  ); }
	inline void m_bShowByGeneralCard            (bool ptr)   { (ptr) ? x_CombinedSwitches[7 ]|=1<<1  : x_CombinedSwitches[7 ]&=~(1<<1  ); }
	inline void m_bShowByGeneralCardRank        (bool ptr)   { (ptr) ? x_CombinedSwitches[7 ]|=1<<2  : x_CombinedSwitches[7 ]&=~(1<<2  ); }
	inline void m_bShowByIconStateID            (bool ptr)   { (ptr) ? x_CombinedSwitches[7 ]|=1<<3  : x_CombinedSwitches[7 ]&=~(1<<3  ); }
	inline void m_bShowByVIPLevel               (bool ptr)   { (ptr) ? x_CombinedSwitches[7 ]|=1<<4  : x_CombinedSwitches[7 ]&=~(1<<4  ); }
	inline void PremNoHome                      (bool ptr)   { (ptr) ? x_CombinedSwitches[7 ]|=1<<5  : x_CombinedSwitches[7 ]&=~(1<<5  ); }
	inline void m_bShowByHomeLevel              (bool ptr)   { (ptr) ? x_CombinedSwitches[7 ]|=1<<6  : x_CombinedSwitches[7 ]&=~(1<<6  ); }
	inline void m_bShowByHomeResourceLevel      (bool ptr)   { (ptr) ? x_CombinedSwitches[7 ]|=1<<7  : x_CombinedSwitches[7 ]&=~(1<<7  ); }	
//------------------------------------------------------------------------------------------------------------------------------------------//
	inline void m_bShowByHomeFactoryLevel       (bool ptr)   { (ptr) ? x_CombinedSwitches[8 ]|=1<<0  : x_CombinedSwitches[8 ]&=~(1<<0  ); }
	inline void m_bShowByHomeFlourish           (bool ptr)   { (ptr) ? x_CombinedSwitches[8 ]|=1<<1  : x_CombinedSwitches[8 ]&=~(1<<1  ); }
	inline void m_bPremHomeStorageTask          (bool ptr)   { (ptr) ? x_CombinedSwitches[8 ]|=1<<2  : x_CombinedSwitches[8 ]&=~(1<<2  ); }
	inline void m_bShowByHomeStorageTask        (bool ptr)   { (ptr) ? x_CombinedSwitches[8 ]|=1<<3  : x_CombinedSwitches[8 ]&=~(1<<3  ); }
	inline void MonsterTotalCount               (bool ptr)   { (ptr) ? x_CombinedSwitches[8 ]|=1<<4  : x_CombinedSwitches[8 ]&=~(1<<4  ); }
	inline void m_bFinNeedComp                  (bool ptr)   { (ptr) ? x_CombinedSwitches[8 ]|=1<<5  : x_CombinedSwitches[8 ]&=~(1<<5  ); }
	inline void FinNeedCompPerson               (bool ptr)   { (ptr) ? x_CombinedSwitches[8 ]|=1<<6  : x_CombinedSwitches[8 ]&=~(1<<6  ); }
	inline void FinNeedCompHistory              (bool ptr)   { (ptr) ? x_CombinedSwitches[8 ]|=1<<7  : x_CombinedSwitches[8 ]&=~(1<<7  ); }
	inline void m_bIsLibraryTask                (bool ptr)   { (ptr) ? x_CombinedSwitches[9 ]|=1<<0  : x_CombinedSwitches[9 ]&=~(1<<0  ); }
	inline void m_bIsUniqueStorageTask          (bool ptr)   { (ptr) ? x_CombinedSwitches[9 ]|=1<<1  : x_CombinedSwitches[9 ]&=~(1<<1  ); }
	inline void m_bHasSign                      (bool ptr)   { (ptr) ? x_CombinedSwitches[9 ]|=1<<2  : x_CombinedSwitches[9 ]&=~(1<<2  ); }
	inline void m_bOfflineFail                  (bool ptr)   { (ptr) ? x_CombinedSwitches[9 ]|=1<<3  : x_CombinedSwitches[9 ]&=~(1<<3  ); }
	inline void m_bItemNotTakeOff               (bool ptr)   { (ptr) ? x_CombinedSwitches[9 ]|=1<<4  : x_CombinedSwitches[9 ]&=~(1<<4  ); }
	inline void m_bAbsTime                      (bool ptr)   { (ptr) ? x_CombinedSwitches[9 ]|=1<<5  : x_CombinedSwitches[9 ]&=~(1<<5  ); }
	inline void MultiNpcTalk                    (bool ptr)   { (ptr) ? x_CombinedSwitches[9 ]|=1<<6  : x_CombinedSwitches[9 ]&=~(1<<6  ); }
	inline void m_bChooseOne                    (bool ptr)   { (ptr) ? x_CombinedSwitches[9 ]|=1<<7  : x_CombinedSwitches[9 ]&=~(1<<7  ); }
	inline void m_bRandOne                      (bool ptr)   { (ptr) ? x_CombinedSwitches[10]|=1<<0  : x_CombinedSwitches[10]&=~(1<<0  ); }
	inline void m_bExeChildInOrder              (bool ptr)   { (ptr) ? x_CombinedSwitches[10]|=1<<1  : x_CombinedSwitches[10]&=~(1<<1  ); }
	inline void m_bParentAlsoFail               (bool ptr)   { (ptr) ? x_CombinedSwitches[10]|=1<<2  : x_CombinedSwitches[10]&=~(1<<2  ); }
	inline void m_bParentAlsoSucc               (bool ptr)   { (ptr) ? x_CombinedSwitches[10]|=1<<3  : x_CombinedSwitches[10]&=~(1<<3  ); }
	inline void m_bCanGiveUp                    (bool ptr)   { (ptr) ? x_CombinedSwitches[10]|=1<<4  : x_CombinedSwitches[10]&=~(1<<4  ); }
	inline void m_bCanRedo                      (bool ptr)   { (ptr) ? x_CombinedSwitches[10]|=1<<5  : x_CombinedSwitches[10]&=~(1<<5  ); }
	inline void m_bCanRedoAfterFailure          (bool ptr)   { (ptr) ? x_CombinedSwitches[10]|=1<<6  : x_CombinedSwitches[10]&=~(1<<6  ); }
	inline void m_bClearAsGiveUp                (bool ptr)   { (ptr) ? x_CombinedSwitches[10]|=1<<7  : x_CombinedSwitches[10]&=~(1<<7  ); }
	inline void m_bNeedRecord                   (bool ptr)   { (ptr) ? x_CombinedSwitches[11]|=1<<0  : x_CombinedSwitches[11]&=~(1<<0  ); }
	inline void m_bFailAsPlayerDie              (bool ptr)   { (ptr) ? x_CombinedSwitches[11]|=1<<1  : x_CombinedSwitches[11]&=~(1<<1  ); }
	inline void m_bDelvInZone                   (bool ptr)   { (ptr) ? x_CombinedSwitches[11]|=1<<2  : x_CombinedSwitches[11]&=~(1<<2  ); }
	inline void m_bEnterRegionFail              (bool ptr)   { (ptr) ? x_CombinedSwitches[11]|=1<<3  : x_CombinedSwitches[11]&=~(1<<3  ); }
	inline void m_bLeaveRegionFail              (bool ptr)   { (ptr) ? x_CombinedSwitches[11]|=1<<4  : x_CombinedSwitches[11]&=~(1<<4  ); }
	inline void m_bLeaveForceFail               (bool ptr)   { (ptr) ? x_CombinedSwitches[11]|=1<<5  : x_CombinedSwitches[11]&=~(1<<5  ); }
	inline void m_bTransTo                      (bool ptr)   { (ptr) ? x_CombinedSwitches[11]|=1<<6  : x_CombinedSwitches[11]&=~(1<<6  ); }
	inline void m_bTrigCtrl                     (bool ptr)   { (ptr) ? x_CombinedSwitches[11]|=1<<7  : x_CombinedSwitches[11]&=~(1<<7  ); }
//------------------------------------------------------------------------------------------------------------------------------------------//
	inline void m_bAutoDeliver                  (bool ptr)   { (ptr) ? x_CombinedSwitches[12]|=1<<0  : x_CombinedSwitches[12]&=~(1<<0  ); }
	inline void m_bDisplayInExclusiveUI         (bool ptr)   { (ptr) ? x_CombinedSwitches[12]|=1<<1  : x_CombinedSwitches[12]&=~(1<<1  ); }
	inline void m_bReadyToNotifyServer          (bool ptr)   { (ptr) ? x_CombinedSwitches[12]|=1<<2  : x_CombinedSwitches[12]&=~(1<<2  ); }
	inline void m_bUsedInTokenShop              (bool ptr)   { (ptr) ? x_CombinedSwitches[12]|=1<<3  : x_CombinedSwitches[12]&=~(1<<3  ); }
	inline void m_bDeathTrig                    (bool ptr)   { (ptr) ? x_CombinedSwitches[12]|=1<<4  : x_CombinedSwitches[12]&=~(1<<4  ); }
	inline void m_bClearAcquired                (bool ptr)   { (ptr) ? x_CombinedSwitches[12]|=1<<5  : x_CombinedSwitches[12]&=~(1<<5  ); }
	inline void m_bShowPrompt                   (bool ptr)   { (ptr) ? x_CombinedSwitches[12]|=1<<6  : x_CombinedSwitches[12]&=~(1<<6  ); }
	inline void m_bKeyTask                      (bool ptr)   { (ptr) ? x_CombinedSwitches[12]|=1<<7  : x_CombinedSwitches[12]&=~(1<<7  ); }
	inline void m_bSkillTask                    (bool ptr)   { (ptr) ? x_CombinedSwitches[13]|=1<<0  : x_CombinedSwitches[13]&=~(1<<0  ); }
	inline void m_bCanSeekOut                   (bool ptr)   { (ptr) ? x_CombinedSwitches[13]|=1<<1  : x_CombinedSwitches[13]&=~(1<<1  ); }
	inline void m_bShowDirection                (bool ptr)   { (ptr) ? x_CombinedSwitches[13]|=1<<2  : x_CombinedSwitches[13]&=~(1<<2  ); }
	inline void m_bMarriage                     (bool ptr)   { (ptr) ? x_CombinedSwitches[13]|=1<<3  : x_CombinedSwitches[13]&=~(1<<3  ); }
	inline void m_bSwitchSceneFail              (bool ptr)   { (ptr) ? x_CombinedSwitches[13]|=1<<4  : x_CombinedSwitches[13]&=~(1<<4  ); }
	inline void m_bHidden                       (bool ptr)   { (ptr) ? x_CombinedSwitches[13]|=1<<5  : x_CombinedSwitches[13]&=~(1<<5  ); }
	inline void m_bDeliverySkill                (bool ptr)   { (ptr) ? x_CombinedSwitches[13]|=1<<6  : x_CombinedSwitches[13]&=~(1<<6  ); }
	inline void m_bShowGfxFinished              (bool ptr)   { (ptr) ? x_CombinedSwitches[13]|=1<<7  : x_CombinedSwitches[13]&=~(1<<7  ); }
	inline void m_bChangePQRanking              (bool ptr)   { (ptr) ? x_CombinedSwitches[14]|=1<<0  : x_CombinedSwitches[14]&=~(1<<0  ); }
	inline void m_bCompareItemAndInventory      (bool ptr)   { (ptr) ? x_CombinedSwitches[14]|=1<<1  : x_CombinedSwitches[14]&=~(1<<1  ); }
	inline void m_bTowerTask                    (bool ptr)   { (ptr) ? x_CombinedSwitches[14]|=1<<2  : x_CombinedSwitches[14]&=~(1<<2  ); }
	inline void m_bHomeTask                     (bool ptr)   { (ptr) ? x_CombinedSwitches[14]|=1<<3  : x_CombinedSwitches[14]&=~(1<<3  ); }
	inline void m_bDeliverInHostHome            (bool ptr)   { (ptr) ? x_CombinedSwitches[14]|=1<<4  : x_CombinedSwitches[14]&=~(1<<4  ); }
	inline void m_bFinishInHostHome             (bool ptr)   { (ptr) ? x_CombinedSwitches[14]|=1<<5  : x_CombinedSwitches[14]&=~(1<<5  ); }
	inline void m_bPQTask                       (bool ptr)   { (ptr) ? x_CombinedSwitches[14]|=1<<6  : x_CombinedSwitches[14]&=~(1<<6  ); }
	inline void m_bPQSubTask                    (bool ptr)   { (ptr) ? x_CombinedSwitches[14]|=1<<7  : x_CombinedSwitches[14]&=~(1<<7  ); }
	inline void m_bClearContrib                 (bool ptr)   { (ptr) ? x_CombinedSwitches[15]|=1<<0  : x_CombinedSwitches[15]&=~(1<<0  ); }
	inline void m_bShowByNeedRecordTasksNum     (bool ptr)   { (ptr) ? x_CombinedSwitches[15]|=1<<1  : x_CombinedSwitches[15]&=~(1<<1  ); }
	inline void m_bShowByFactionContrib         (bool ptr)   { (ptr) ? x_CombinedSwitches[15]|=1<<2  : x_CombinedSwitches[15]&=~(1<<2  ); }
	inline void m_bAccountTaskLimit             (bool ptr)   { (ptr) ? x_CombinedSwitches[15]|=1<<3  : x_CombinedSwitches[15]&=~(1<<3  ); }
	inline void m_bRoleTaskLimit                (bool ptr)   { (ptr) ? x_CombinedSwitches[15]|=1<<4  : x_CombinedSwitches[15]&=~(1<<4  ); }
	inline void m_bLeaveFactionFail             (bool ptr)   { (ptr) ? x_CombinedSwitches[15]|=1<<5  : x_CombinedSwitches[15]&=~(1<<5  ); }
	inline void m_bNotIncCntWhenFailed          (bool ptr)   { (ptr) ? x_CombinedSwitches[15]|=1<<6  : x_CombinedSwitches[15]&=~(1<<6  ); }
	inline void m_bNotClearItemWhenFailed       (bool ptr)   { (ptr) ? x_CombinedSwitches[15]|=1<<7  : x_CombinedSwitches[15]&=~(1<<7  ); }	
//------------------------------------------------------------------------------------------------------------------------------------------//
	inline void m_bDisplayInTitleTaskUI         (bool ptr)   { (ptr) ? x_CombinedSwitches[16]|=1<<0  : x_CombinedSwitches[16]&=~(1<<0  ); }
	inline void CanAwardOnCarrier               (bool ptr)   { (ptr) ? x_CombinedSwitches[16]|=1<<1  : x_CombinedSwitches[16]&=~(1<<1  ); }
	inline void PreReachArenaWinCount           (bool ptr)   { (ptr) ? x_CombinedSwitches[16]|=1<<2  : x_CombinedSwitches[16]&=~(1<<2  ); }
	inline void ShowByReachArenaWinCount        (bool ptr)   { (ptr) ? x_CombinedSwitches[16]|=1<<3  : x_CombinedSwitches[16]&=~(1<<3  ); }
	inline void WriteSpecialLog                 (bool ptr)   { (ptr) ? x_CombinedSwitches[16]|=1<<4  : x_CombinedSwitches[16]&=~(1<<4  ); }
	inline void ShowByPreFactionScore           (bool ptr)   { (ptr) ? x_CombinedSwitches[16]|=1<<5  : x_CombinedSwitches[16]&=~(1<<5  ); }
	inline void ShowByPreFactionMoney           (bool ptr)   { (ptr) ? x_CombinedSwitches[16]|=1<<6  : x_CombinedSwitches[16]&=~(1<<6  ); }
	inline void ShowByPreFactionPersonScore     (bool ptr)   { (ptr) ? x_CombinedSwitches[16]|=1<<7  : x_CombinedSwitches[16]&=~(1<<7  ); }
	inline void CheckFactionMem                 (bool ptr)   { (ptr) ? x_CombinedSwitches[17]|=1<<0  : x_CombinedSwitches[17]&=~(1<<0  ); }
	inline void ByFactionScore                  (bool ptr)   { (ptr) ? x_CombinedSwitches[17]|=1<<1  : x_CombinedSwitches[17]&=~(1<<1  ); }
	inline void SpecailLogicTask                (bool ptr)   { (ptr) ? x_CombinedSwitches[17]|=1<<2  : x_CombinedSwitches[17]&=~(1<<2  ); }
	inline void BecomeMentor                    (bool ptr)   { (ptr) ? x_CombinedSwitches[17]|=1<<3  : x_CombinedSwitches[17]&=~(1<<3  ); }
	inline void ShowMentorMorality              (bool ptr)   { (ptr) ? x_CombinedSwitches[17]|=1<<4  : x_CombinedSwitches[17]&=~(1<<4  ); }
	inline void ShowMentorStudent               (bool ptr)   { (ptr) ? x_CombinedSwitches[17]|=1<<5  : x_CombinedSwitches[17]&=~(1<<5  ); }
	inline void DiscardedTask                   (bool ptr)   { (ptr) ? x_CombinedSwitches[17]|=1<<6  : x_CombinedSwitches[17]&=~(1<<6  ); }
	inline void NoCheckTitleGot                 (bool ptr)   { (ptr) ? x_CombinedSwitches[17]|=1<<7  : x_CombinedSwitches[17]&=~(1<<7  ); }
	inline void ShowCoupleIntimacy              (bool ptr)   { (ptr) ? x_CombinedSwitches[18]|=1<<0  : x_CombinedSwitches[18]&=~(1<<0  ); }
	inline void m_bShowByDeposit2               (bool ptr)   { (ptr) ? x_CombinedSwitches[18]|=1<<1  : x_CombinedSwitches[18]&=~(1<<1  ); }
	inline void RecordFinishCount               (bool ptr)   { (ptr) ? x_CombinedSwitches[18]|=1<<2  : x_CombinedSwitches[18]&=~(1<<2  ); }
	inline void ShowCamp                        (bool ptr)   { (ptr) ? x_CombinedSwitches[18]|=1<<3  : x_CombinedSwitches[18]&=~(1<<3  ); }
	inline void ShowCampPower                   (bool ptr)   { (ptr) ? x_CombinedSwitches[18]|=1<<4  : x_CombinedSwitches[18]&=~(1<<4  ); }
	inline void ReturnCampPower                 (bool ptr)   { (ptr) ? x_CombinedSwitches[18]|=1<<5  : x_CombinedSwitches[18]&=~(1<<5  ); }
	inline void TeamHaveRecaller                (bool ptr)   { (ptr) ? x_CombinedSwitches[18]|=1<<6  : x_CombinedSwitches[18]&=~(1<<6  ); }
	inline void BindedRecaller                  (bool ptr)   { (ptr) ? x_CombinedSwitches[18]|=1<<7  : x_CombinedSwitches[18]&=~(1<<7  ); }
	inline void ShowNeedFriendPoint             (bool ptr)   { (ptr) ? x_CombinedSwitches[19]|=1<<0  : x_CombinedSwitches[19]&=~(1<<0  ); }
	inline void HistoryVarTask                  (bool ptr)   { (ptr) ? x_CombinedSwitches[19]|=1<<1  : x_CombinedSwitches[19]&=~(1<<1  ); }
	inline void PremRank                        (bool ptr)   { (ptr) ? x_CombinedSwitches[19]|=1<<2  : x_CombinedSwitches[19]&=~(1<<2  ); }
	inline void DeliverByUI                     (bool ptr)   { (ptr) ? x_CombinedSwitches[19]|=1<<3  : x_CombinedSwitches[19]&=~(1<<3  ); }
	inline void WarTaskAward                    (bool ptr)   { (ptr) ? x_CombinedSwitches[19]|=1<<4  : x_CombinedSwitches[19]&=~(1<<4  ); }
	inline void HideOtherPlayer                 (bool ptr)   { (ptr) ? x_CombinedSwitches[19]|=1<<5  : x_CombinedSwitches[19]&=~(1<<5  ); }
	inline void RecordDeliverTime               (bool ptr)   { (ptr) ? x_CombinedSwitches[19]|=1<<6  : x_CombinedSwitches[19]&=~(1<<6  ); }
//------------------------------------------------------------------------------------------------------------------------------------------//
	
	void* GetFixedDataPtr() { return (void*)this; }
	void Init();
	bool LoadFixedDataFromTextFile(FILE* fp, unsigned int ulVersion);
	bool LoadFixedDataFromBinFile(FILE* fp);
	bool HasFailAward() const { return m_Award_F->HasAward() || m_AwByRatio_F->HasAward() || m_AwByItems_F->HasAward(); }
	bool HasSuccAward() const {	return m_Award_S->HasAward() || m_AwByRatio_S->HasAward() || m_AwByItems_S->HasAward();	}
	bool CanRedo() const { return (!m_bNeedRecord() || m_bCanRedo() || m_bCanRedoAfterFailure() ); }

#ifdef _TASK_CLIENT
	void SaveFixedDataToTextFile(FILE* fp);
	void SaveFixedDataToBinFile(FILE* fp);
	void SetName(const wchar_t* szName)
	{
		memset(m_szName, 0, sizeof(m_szName));
		wcsncpy((wchar_t*)m_szName, szName, MAX_TASK_NAME_LEN-1);
	}
	const wchar_t* GetName() const { return (const wchar_t*)m_szName; }
#endif
};

#pragma pack()

inline void ATaskTemplFixedData::Init()
{
	int 				combinedswitch1 = 0;
	int 				combinedswitch2 = 0;
	int 				combinedswitch3 = 0;
	int 				combinedswitch4 = 0;
	int 				combinedswitch5 = 0;

	m_bCanRedo				(true);
	m_bParentAlsoFail		(true);
	m_bCanGiveUp			(true);
	m_bCanRedoAfterFailure	(true);
	m_bClearAcquired		(true);
	
	m_enumMethod			= enumTMNone;
	m_enumFinishType		= enumTFTDirect;
	
	m_bShowByDeposit			(true);
	m_bShowByDeposit2			(true);
	m_bShowByGender				(true);
	m_bShowByItems				(true);
	m_bShowByFactionContrib 	(true);
	m_bShowByNeedRecordTasksNum (true);
	m_bShowByLev				(true);
	m_bShowByOccup				(true);
	m_bShowByPreTask			(true);
	m_bShowByRepu				(true);
	m_bShowByTeam				(true);
	m_bShowByFaction			(true);
	m_bShowByPeriod				(true);
	m_bShowPrompt				(true);
	m_bShowByCharTime			(true);
	m_bShowByRMB				(true);
	m_bShowBySpouse				(true);
	m_bShowByWeddingOwner		(true);
	m_bShowByKing				(true);
	m_bShowByNotInTeam			(true);
	m_bShowByGeneralCard		(true);
	
	m_ulDelvWorld			= 1;
	m_ulTransWldId			= 1;
	m_ulReachSiteId			= 1;
	m_lPeriodLimit			= 1;
	
	m_bCanSeekOut			(false);
	m_bShowDirection		(true );
	m_bDeliverySkill		(false); // verison 77
	
	m_iDeliveredSkillID		= 0;
	m_iDeliveredSkillLevel  = 0;

	m_bShowGfxFinished 		(false); // version 78
	m_bChangePQRanking 		(false); // version 79. Ĭ�� PQ �ı����������
	m_bDisplayInExclusiveUI (false);
	m_bReadyToNotifyServer 	(false);
	m_bDistinguishedOcc 	(false);
	m_bUsedInTokenShop 		(false);
	m_iPremise_FactionRole = 6;

	m_TreasureStartZone.x = 0;
	m_TreasureStartZone.y = 0;
	m_TreasureStartZone.z = 0;
	m_ucZonesNumX = 1;
	m_ucZonesNumZ = 1;
	m_ucZoneSide = 10;

	m_ucPremiseTransformedForm = 0xFF;

	m_bShowByForce				(true);
	m_bShowByForceContribution 	(true);
	m_bShowByForceReputation 	(true);
	m_bShowByForceExp 			(true);
	m_bShowByForceSP 			(true);
	m_bShowByForceActivityLevel (true);
	m_bShowByReincarnation 		(true);
	m_bShowByRealmLevel 		(true);
	m_bShowByGeneralCardRank 	(true);
	m_bShowByHistoryStage 		(true);

	m_iPremForceActivityLevel = -1;
	m_iPremGeneralCardRank = -1;

	m_ulPremIconStateID	 = 0;
	m_bShowByIconStateID (false);
	m_ulTMIconStateID = 0;
	
}

class ATaskTempl : public ATaskTemplFixedData
{
public:
	ATaskTempl() :
	m_pwstrDescript(0),
	m_pwstrOkText(0),
	m_pwstrNoText(0),
	m_pwstrTribute(0),
	m_pParent(0),
	m_pPrevSibling(0),
	m_pNextSibling(0),
	m_pFirstChild(0),
	m_uDepth(1),
	m_ulMask(0),
	m_nSubCount(0),
	m_uValidCount(0),
	m_ulPremStorageItem(0)
	{
		m_szFilePath[0] = '\0';
		memset(&m_DelvTaskTalk, 0, sizeof(talk_proc));
		memset(&m_UnqualifiedTalk, 0, sizeof(talk_proc));
		memset(&m_DelvItemTalk, 0, sizeof(talk_proc));
		memset(&m_ExeTalk, 0, sizeof(talk_proc));
		memset(&m_AwardTalk, 0, sizeof(talk_proc));
	}

	virtual ~ATaskTempl()
	{
		if (m_pParent && m_pParent->m_pFirstChild == this)
			m_pParent->m_pFirstChild = m_pNextSibling;

		if (m_pPrevSibling) m_pPrevSibling->m_pNextSibling = m_pNextSibling;
		if (m_pNextSibling) m_pNextSibling->m_pPrevSibling = m_pPrevSibling;

		ATaskTempl* pChild = m_pFirstChild;
		while (pChild)
		{
			ATaskTempl* pNext = pChild->m_pNextSibling;
			LOG_DELETE(pChild);
			pChild = pNext;
		}

		LOG_DELETE_ARR(m_pwstrDescript);
		LOG_DELETE_ARR(m_pwstrOkText);
		LOG_DELETE_ARR(m_pwstrNoText);
		LOG_DELETE_ARR(m_pwstrTribute);
	}

public:
	// ��������
	task_char*		m_pwstrDescript;
	task_char*		m_pwstrOkText;
	task_char*		m_pwstrNoText;
	task_char*		m_pwstrTribute;

	char			m_szFilePath[TASK_MAX_PATH];

	/* ��νṹ */
	ATaskTempl*		m_pParent;
	ATaskTempl*		m_pPrevSibling;
	ATaskTempl*		m_pNextSibling;
	ATaskTempl*		m_pFirstChild;
	unsigned char	m_uDepth;
	unsigned int	m_ulMask;
	int				m_nSubCount;
	mutable unsigned char m_uValidCount;
	unsigned int	m_ulPremStorageItem;

	struct TASK_TIME_MARK
	{
		unsigned int	m_ulPlayerId;
		unsigned int	m_ulStartTime;

		void SetData(unsigned int ulPlayerId, unsigned int ulStartTime)
		{
			m_ulPlayerId	= ulPlayerId;
			m_ulStartTime	= ulStartTime;
		}
	};

	// Talk data
	talk_proc m_DelvTaskTalk;
	talk_proc m_UnqualifiedTalk;
	talk_proc m_DelvItemTalk;
	talk_proc m_ExeTalk;
	talk_proc m_AwardTalk;

protected:
	bool Load(FILE* fp, unsigned int ulVersion, bool bTextFile);

#ifdef _TASK_CLIENT
	void Save(FILE* fp, bool bTextFile);
	void SaveBinary(FILE* fp);

#endif

public:
	static ATaskTempl* CreateEmptyTaskTempl();

	void AddSubTaskTempl(ATaskTempl* pSub);
	void InsertTaskTemplBefore(ATaskTempl* pInsert);
	void SynchID()
	{
		m_ulParent = (m_pParent ? m_pParent->m_ID : 0);
		m_ulNextSibling = (m_pNextSibling ? m_pNextSibling->m_ID : 0);
		m_ulPrevSibling = (m_pPrevSibling ? m_pPrevSibling->m_ID : 0);
		m_ulFirstChild = (m_pFirstChild ? m_pFirstChild->m_ID : 0);
		if (m_pFirstChild) m_enumMethod = enumTMNone;
	}
	ATaskTempl* GetSubById(unsigned int ulId)
	{
		ATaskTempl* pChild = m_pFirstChild;

		while (pChild)
		{
			if (pChild->m_ID == ulId) return pChild;
			pChild = pChild->m_pNextSibling;
		}

		return NULL;
	}
	const ATaskTempl* GetConstSubById(unsigned int ulId) const
	{
		const ATaskTempl* pChild = m_pFirstChild;

		while (pChild)
		{
			if (pChild->m_ID == ulId) return pChild;
			pChild = pChild->m_pNextSibling;
		}

		return NULL;
	}
	const ATaskTempl* GetSubByIndex(int nIndex) const
	{
		const ATaskTempl* pChild = m_pFirstChild;

		while (pChild)
		{
			if (nIndex-- == 0)
				return pChild;

			pChild = pChild->m_pNextSibling;
		}

		return NULL;
	}
	const ATaskTempl* GetTopTask() const
	{
		const ATaskTempl* pTop = this;
		const ATaskTempl* pParent = m_pParent;

		while (pParent)
		{
			pTop = pParent;
			pParent = pParent->m_pParent;
		}

		return pTop;
	}
	int GetSubCount() const
	{
		int nCount = 0;
		ATaskTempl* pChild = m_pFirstChild;

		while (pChild)
		{
			nCount++;
			pChild = pChild->m_pNextSibling;
		}

		return nCount;
	}
	void CheckDepth()
	{
		ATaskTempl* pChild = m_pFirstChild;
		unsigned char uDepth = 0;
		bool bMaxChildDepth = m_bExeChildInOrder() || m_bChooseOne() || m_bRandOne();

		while (pChild)
		{
			pChild->CheckDepth();

			if (bMaxChildDepth)
			{
				if (uDepth < pChild->m_uDepth)
					uDepth = pChild->m_uDepth;
			}
			else
				uDepth += pChild->m_uDepth;

			pChild = pChild->m_pNextSibling;
		}

		m_uDepth += uDepth;
	}

	void CheckMask();
	bool LoadFromTextFile(FILE* fp, bool bLoadDescript);
	bool LoadFromTextFile(const char* szPath, bool bLoadDescript = true);
	bool LoadBinary(FILE* fp);
	bool LoadFromBinFile(FILE* fp);
	bool LoadDescription(FILE* fp);
	bool LoadDescriptionBin(FILE* fp);
	bool LoadTribute(FILE* fp);
	bool LoadTributeBin(FILE* fp);
	int UnmarshalKillMonster(const char* pData);
	int UnmarshalCollectItems(const char* pData);
	int UnmarshalDynTask(const char* pData);
	int UnmarshalSpecialAwardData(const char* pData);
	
	// inline funcs
	unsigned int GetSpaceID() const { return x_IDSpace; }
	unsigned int GetID() const { return m_ID; }
	unsigned int GetType() const { return m_ulType; }
	unsigned int HasSignature() const { return m_bHasSign(); }
	const task_char* GetSignature() const { return m_pszSignature; }
	bool CanShowDirection() const { return m_bShowDirection(); }
	void IncValidCount() const { m_uValidCount++; }
	void ClearValidCount() const { m_uValidCount = 0; }
	bool IsValidState() const { return m_uValidCount < TASK_MAX_VALID_COUNT; }

	bool IsNotChangePQRanking() { return m_bChangePQRanking();}

	void ConfirmFinishAutoDelTask(){m_bReadyToNotifyServer (true);}
	void ResetAutoDelTask() const {const_cast<ATaskTempl*>(this)->m_bReadyToNotifyServer(false);}

#ifdef _TASK_CLIENT
	void SyncTaskType(); // ʹ����������������游����
	bool GetShowGfxFlag() { return m_bShowGfxFinished();} 
	const wchar_t* GetDescription() const { assert(m_pwstrDescript); return (wchar_t*)m_pwstrDescript; }
	const wchar_t* GetOkText() const { assert(m_pwstrOkText); return (wchar_t*)m_pwstrOkText; }
	const wchar_t* GetNoText() const { assert(m_pwstrNoText); return (wchar_t*)m_pwstrNoText; }
	const wchar_t* GetTribute() const { assert(m_pwstrTribute); return (wchar_t*)m_pwstrTribute; }

	const talk_proc* GetDeliverTaskTalk() const { return &m_DelvTaskTalk; }
	const talk_proc* GetUnqualifiedTalk() const { return &m_UnqualifiedTalk; }
	const talk_proc* GetDeliverItemTalk() const { return &m_DelvItemTalk; }
	const talk_proc* GetUnfinishedTalk() const { return &m_ExeTalk; }
	const talk_proc* GetAwardTalk() const { return &m_AwardTalk; }

	unsigned int GetDeliverNPC() const { return m_ulDelvNPC; }
	unsigned int GetAwardNPC() const { return m_ulAwardNPC; }

	void SaveToTextFile(FILE* fp);
	bool SaveToTextFile(const char* szPath);
	void SaveToBinFile(FILE* fp) { SaveBinary(fp); }
	void SaveDescription(FILE* fp);
	void SaveDescriptionBin(FILE* fp);
	void SaveTribute(FILE* fp);
	void SaveTributeBin(FILE* fp);
	void SaveAllText(FILE* fp);
	int MarshalKillMonster(char* pData);
	int MarshalCollectItems(char* pData);
	int MarshalDynTask(char* pData);
	int MarshalSpecialAwardData(char* pData);

	ATaskTempl& operator= (const ATaskTempl& src);
	bool operator == (const ATaskTempl& src) const
	{
		return *(ATaskTemplFixedData*)this == *(const ATaskTemplFixedData*)&src;
	}
#endif
	const char* GetFilePath() const { return m_szFilePath; }
	void SetFilePath(const char* szPath) { strcpy(m_szFilePath, szPath); }

	// Process Part, all const functions

public:
	ActiveTaskEntry* DeliverTask(
		TaskInterface* pTask,
		ActiveTaskList* pList,
		ActiveTaskEntry* pEntry,
		unsigned int ulCaptainTask,
		unsigned int& ulMask,
		unsigned int ulCurTime,
		const ATaskTempl* pSubTempl,
		task_sub_tags* pSubTag,
		TaskGlobalData* pGlobal,
		unsigned char uParentIndex = (unsigned char)0xff) const;

	void RecursiveAward				(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, unsigned int ulCurTime, int nChoice, task_sub_tags* pSubTag) const;
	void RecursiveCheckTimeLimit	(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, unsigned int ulCurTime) const;
	bool RecursiveCheckParent		(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry) const;
	bool HasAllItemsWanted			(TaskInterface* pTask) const;
	bool HasAllMonsterWanted		(TaskInterface* pTask, const ActiveTaskEntry* pEntry) const;
	bool HasAllPlayerKilled			(TaskInterface* pTask, const ActiveTaskEntry* pEntry) const;
	void CalcAwardDataByRatio		(AWARD_DATA* pAward, ActiveTaskEntry* pEntry, unsigned int ulTaskTime, unsigned int ulCurTime) const;
	void CalcAwardDataByItems		(TaskInterface* pTask, AWARD_DATA* pAward, ActiveTaskEntry* pEntry) const;
	bool CanAwardItems				(TaskInterface* pTask, const AWARD_ITEMS_CAND* pAward) const;
	void CalcAwardItemsCount		(TaskInterface* pTask, const AWARD_ITEMS_CAND* pAward, unsigned int& ulCmnCount, unsigned int& ulTskCount) const;
	unsigned int CheckAwardWhenAtCrossServer(TaskInterface* pTask, const AWARD_DATA* pAward) const;
	unsigned int RecursiveCalcAward(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, unsigned int ulCurTime, int nChoice, unsigned int& ulCmnCount, unsigned int& ulTskCount, unsigned int& ulTopCount, unsigned char& uBudget, int& lReputation) const;

	unsigned int CheckFnshLst		(TaskInterface* pTask, unsigned int ulCurTime) const;
	unsigned int CheckRecordTasksNum(TaskInterface* pTask) const;
	unsigned int CheckFactionContrib(TaskInterface* pTask) const;
	unsigned int CheckLevel		(TaskInterface* pTask) const;
	unsigned int CheckReincarnation(TaskInterface* pTask) const;
	unsigned int CheckRealmLevel(TaskInterface* pTask) const;
	unsigned int CheckRealmExpFull(TaskInterface* pTask) const;
	unsigned int CheckRepu			(TaskInterface* pTask) const;
	unsigned int CheckDeposit		(TaskInterface* pTask) const;
	unsigned int CheckDeposit2		(TaskInterface* pTask) const;
	unsigned int CheckItems		(TaskInterface* pTask) const;
	unsigned int CheckFaction		(TaskInterface* pTask) const;
	unsigned int CheckGender		(TaskInterface* pTask) const;
	unsigned int CheckOccupation	(TaskInterface* pTask) const;
	unsigned int CheckPeriod		(TaskInterface* pTask) const;
	unsigned int CheckGM			(TaskInterface* pTask) const;
	unsigned int CheckShieldUser	(TaskInterface* pTask) const;
	unsigned int CheckTeamTask		(TaskInterface* pTask) const;
	unsigned int CheckMutexTask	(TaskInterface* pTask, unsigned int ulCurTime) const;
	unsigned int CheckInZone		(TaskInterface* pTask) const;
	unsigned int CheckGivenItems	(TaskInterface* pTask) const;
	unsigned int CheckLivingSkill	(TaskInterface* pTask) const;
	unsigned int CheckPreTask		(TaskInterface* pTask) const;
	unsigned int CheckTimetable	(unsigned int ulCurTime) const;
	unsigned int CheckDeliverTime	(TaskInterface* pTask, unsigned int ulCurTime) const;	
	unsigned int CheckSpecialAward	(TaskInterface* pTask) const;
	unsigned int CheckSpouse		(TaskInterface* pTask) const;
	unsigned int CheckWeddingOwner (TaskInterface* pTask) const;
	unsigned int CheckGlobalKeyValue(TaskInterface* pTask, bool bFinCheck = false) const;
	unsigned int CheckGlobalPQKeyValue(bool bFinCheck = false) const;
	unsigned int CheckDeliverCount (TaskInterface* pTask) const;
	unsigned int CheckAccountRMB	(TaskInterface* pTask) const;
	unsigned int CheckCharTime		(TaskInterface* pTask) const;

	unsigned int CheckIvtrEmptySlot(TaskInterface* pTask) const;
	
	unsigned int CheckTransform(TaskInterface* pTask) const;
	unsigned int CheckForce(TaskInterface* pTask) const;
	unsigned int CheckForceReputation(TaskInterface* pTask) const;
	unsigned int CheckForceContribution(TaskInterface* pTask) const;
	unsigned int CheckExp(TaskInterface* pTask) const;
	unsigned int CheckSP(TaskInterface* pTask) const;
	unsigned int CheckForceActivityLevel(TaskInterface* pTask) const;
	unsigned int CheckKing(TaskInterface* pTask) const;
	unsigned int CheckNotInTeam(TaskInterface* pTask) const;
	unsigned int CheckTitle(TaskInterface* pTask) const;
	unsigned int CheckHistoryStage(TaskInterface* pTask) const;
	bool		  CheckReachLevel(TaskInterface* pTask) const;
	unsigned int CheckCardCollection(TaskInterface* pTask) const;
	unsigned int CheckCardRankCount(TaskInterface* pTask) const;
	unsigned int CheckInTransformShape(TaskInterface* pTask) const;
	unsigned int CheckPremIconStateID(TaskInterface *pTask)const;
	bool		  CheckTMIconStateID(TaskInterface *pTask)const;
	unsigned int CheckVIPLevel(TaskInterface* pTask)const;

	bool		  CheckAllPointsEventTask(TaskInterface* pTask)const;

#ifdef _TASK_CLIENT
#else
	void RemovePrerequisiteItem		(TaskInterface* pTask) const;
	unsigned int DeliverByAwardData(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, const AWARD_DATA* pAward, unsigned int ulCurTime, int nChoice) const;
	void DeliverTeamMemTask			(TaskInterface* pTask, TaskGlobalData* pGlobal, unsigned int ulCurTime) const;
	bool CheckTeamMemPos			(TaskInterface* pTask, ActiveTaskEntry* pEntry, float fSqrDist) const;
	void DeliverGivenItems			(TaskInterface* pTask) const;
	const ATaskTempl* RandOneChild	(TaskInterface* pTask, int& nSub) const;
	void AwardNotifyTeamMem			(TaskInterface* pTask, ActiveTaskEntry* pEntry) const;
	unsigned int CalMultiByGlobalKeyValue(TaskInterface* pTask, const AWARD_DATA* pAward) const;

#endif

public:
	unsigned int CheckBudget		(ActiveTaskList* pList) const;
	unsigned int CheckPrerequisite	(TaskInterface* pTask, ActiveTaskList* pList, unsigned int ulCurTime, bool bCheckPrevTask = true, bool bCheckTeam = true, bool bCheckBudge = true, bool bCheckLevel = true) const;
	unsigned int CheckMarriage		(TaskInterface* pTask) const;
	bool CanFinishTask				(TaskInterface* pTask, const ActiveTaskEntry* pEntry, unsigned int ulCurTime) const;
	void OnServerNotify				(TaskInterface* pTask, ActiveTaskEntry* pEntry, const task_notify_base* pNotify, unsigned int sz) const;
	void CalcAwardData				(TaskInterface* pTask, AWARD_DATA* pAward, ActiveTaskEntry* pEntry, unsigned int ulTaskTime, unsigned int ulCurTime) const;
	unsigned int CalcAwardMulti	(TaskInterface* pTask, ActiveTaskEntry* pEntry, unsigned int ulTaskTime, unsigned int ulCurTime) const;
	unsigned int GetMemTaskByInfo	(const task_team_member_info* pInfo) const;
	bool IsAutoDeliver				() const;
	bool NeedFinishedTaskListSupport()const;
	bool NeedFinishTimeListSupport()const;
	bool NeedFinishCountListSupport()const;
	unsigned int GetSuitableLevel	() { return GetTopTask()->m_ulSuitableLevel; }
	bool CanShowPrompt				() const { return GetTopTask()->m_bShowPrompt(); }
	bool IsKeyTask					() const { return GetTopTask()->m_bKeyTask(); }
	unsigned int HasAllTeamMemsWanted(TaskInterface* pTask, bool bStrict) const;
	unsigned int RecursiveCheckAward(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, unsigned int ulCurTime, int nChoice) const;
	bool CanDeliverWorldContribution(TaskInterface* pTask) const;

	class TaskRecursiveChecker{
	public:
		virtual ~TaskRecursiveChecker(){}
		virtual bool Downward()const=0;
		virtual bool IsMatch(const ATaskTempl *pTask)const=0;
	};
	bool RecursiveCheck(TaskRecursiveChecker *pChecker)const;

	unsigned int CheckRecordListSpace(TaskInterface *pTask)const;
	

#ifdef _TASK_CLIENT
	bool CanShowTask				(TaskInterface* pTask) const;
	bool HasShowCond()				const;	
	void GetGlobalTaskChar			(TaskInterface* pTask, abase::vector<wchar_t*>& TaskCharArr) const;
	float CalcOneGlobalExp			(TaskInterface* pTask, int nIndex) const;
	bool CanShowInExclusiveUI		(TaskInterface* pTask, unsigned int ulCurTime) const;
#else
	void NotifyClient				(TaskInterface* pTask, const ActiveTaskEntry* pEntry, unsigned char uReason, unsigned int ulCurTime, size_t ulParam = 0, int dps = 0, int dph = 0) const;
	bool CheckGlobalRequired		(TaskInterface* pTask, unsigned int ulSubTaskId, const TaskPreservedData* pPreserve, const TaskGlobalData* pGlobal, unsigned short reason) const;
	bool CheckKillMonster			(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, unsigned int ulTemplId, unsigned int ulLev, bool bTeam, float fRand, int dps, int dph) const;
	bool CheckKillPlayer			(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, int iOccupation, int iLevel, bool bGender, int iForce, float fRand) const;
	void CheckCollectItem			(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, bool bAtNPC, int nChoice) const;
	void CheckMonsterKilled			(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, bool bAtNPC, int nChoice) const;
	void CheckMining				(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry) const;
	void CheckWaitTime				(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, unsigned int ulCurTime, bool bAtNPC, int nChoice) const;
	void GiveUpOneTask				(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, bool bForce) const;
	void OnSetFinished				(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, bool bNotifyMem = true) const;
	bool DeliverAward				(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, int nChoice, bool bNotifyTeamMem = true, TaskGlobalData* pGlobal = NULL) const;
	void RemoveAcquiredItem			(TaskInterface* pTask, bool bClearTask, bool bSuccess) const;
	void TakeAwayGivenItems			(TaskInterface* pTask) const;
	bool OnDeliverTeamMemTask		(TaskInterface* pTask, TaskGlobalData* pGlobal) const;
	unsigned int CheckDeliverTask	(TaskInterface* pTask, unsigned int ulSubTaskId, TaskGlobalData* pGlobal, bool bNotifyErr = true, bool bMemTask = false, unsigned int ulCapId = 0) const;
	bool HasGlobalData() const;
	void NotifyClientEvent				(TaskInterface* pTask, const ActiveTaskEntry* pEntry, unsigned short uReason, unsigned int ulCurTime, unsigned short ulParam, int dps = 0, int dph = 0) const;

#endif

};

#ifdef _TASK_CLIENT
enum ENUM_TASK_TYPE_OLD
{
		enumTTCommonOld = 0,		// ��ͨ
		enumTTRepeatedOld,			// �ظ�
		enumTTKeyOld,				// ����
		enumTTBossOld,				// Boss
		enumTTInstanceOld,			// ����
		enumTTScrollOld,			// ����
		enumTTBusinessOld,			// ����
		enumTTFeelingOld,			// ����
		enumTTTacticsOld,			// ����
		enumTTRegionOld,			// ����
		enumTTSevenKillListOld,	// ��ɱ��
		enumTTFactionOld,			// ��������
		enumTTHintOld,				// ��ʾ����
};

int GetTaskTypeFromComboBox(int nComboBoxData);
int GetComboBoxFromTaskType(int nTaskType);
#endif

#include "TaskTempl.inl"

#endif
