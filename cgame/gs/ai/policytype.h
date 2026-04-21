#ifndef _AI_POLICY_TYPE_H_
#define _AI_POLICY_TYPE_H_


#define OP_TEXT_LENGTH 256

#ifdef _WINDOWS
typedef wchar_t policy_char;
#else
typedef unsigned short policy_char;
#endif

//	���ٻ�����ʧ��������
enum enumPolicySummoneeDisppearType
{
	enumPSDTNever = 0,						//	�����κ���������
	enumPSDTFollowSummoner,					//	���ٻ�������
	enumPSDTFollowSummonTarget,				//	���ٻ�Ŀ������
	enumPSDTFollowSummonerOrSummonTarget,	//	�ٻ��ߺ��ٻ�Ŀ������һ������������
	enumPSDTNum,
};

//	����Ѳ�߷�ʽ
enum enumPolicyMonsterPatrolType
{
	enumPMPTStopAtEnd = 0,	//	�յ�ֹͣ
	enumPMPTReturn,			//	ԭ·����
	enumPMPTLoop,			//	ʼ��ѭ��
	enumPMPTNum,
};

//	����Ѳ���ٶ�����
enum enumPolicyMonsterPatrolSpeedType
{
	enumPMPSTBegin = 1,
	enumPMPSTSlow = enumPMPSTBegin,	//	����
	enumPMPSTFast,					//	����
	enumPMPSTEnd = enumPMPSTFast,
};

//	��������ö��
enum enumPolicyVarType
{
	enumPVTGlobalVarID = 0,	//	ȫ�ֱ���ID
	enumPVTLocalVarID,		//	�ֲ�����ID
	enumPVTConst,			//	����
	enumPVTRandom,			//	��0-99��������ֵ
	enumPVTNum,
};

//	����ʽ��������
enum enumPolicyOperatorType
{
	enumPOTAdd = 0,			//	�ӷ� +
	enumPOTSub,				//	���� -
	enumPOTMul,				//	�˷� *
	enumPOTDiv,				//	���� /
	enumPOTMod,				//	ȡ�� %
	enumPOTNum,
};

//	���� PVP ���ֽ�������
enum enumPolicyFactionPVPPointType
{
	enumPFPPTMineCar = 0,	//	������Դ�����ͻ���
	enumPFPPTMineBase,		//	������Դ�������ͻ���
	enumPFPPTMineCarArrived,	//	������Դ���������ͻ���
};

//	˵����������ѡ��
enum enumPolicyTalkTextAppendDataMask
{
	enumPTTADMName		= 0x000001,
	enumPTTADMLocalVar0	= 0x000002,
	enumPTTADMLocalVar1	= 0x000004,
	enumPTTADMLocalVar2	= 0x000008,
};

struct POLICY_ZONE_VERT{
	union{
		struct{
			float x;
			float y;
			float z;
		};		
		float v[3];
	};
	void clear(){
		x = y = z = 0.0f;
	}
	bool operator == (const POLICY_ZONE_VERT& src) const{
		return (v[0] == src.v[0] && v[1] == src.v[1] && v[2] == src.v[2]);
	}	
	bool great_than(const float v[3]) const {
		return x >= v[0] && y >= v[1] && z >= v[2];
	}
	bool less_than (const float v[3]) const {
		return x <= v[0] && y <= v[1] && z <= v[2];
	}
};
//----------------------------------------------------------------------------------------
//Condition
//----------------------------------------------------------------------------------------

struct C_TIME_COME
{
	unsigned int uID;
};

struct C_HP_LESS
{
	float fPercent;
};


struct C_RANDOM
{
	float fProbability;
};

struct C_VAR
{
	int iID;
};

struct C_CONSTANT
{
	int iValue;
};

// struct C_TIME_POINT
// {
// 	unsigned int uHour;
// 	unsigned int uMinute;
// };

struct C_BE_HURT
{
	int iHurtLow;
	int iHurtHigh;
};

struct C_REACH_END
{
	int iPathID;
};

struct C_HISTORY_STAGE
{
	int iID;
};

struct C_HISTORY_VALUE
{
	int iValue;
};

struct C_LOCAL_VAR
{
	int iID;
};

struct C_REACH_END_2
{
	int iPathID;
	int iPathIDType;	//	iPathID ֵ�ĺ�����ͣ�ȡֵ enumPolicyVarType �� 0~2
};

struct C_HAS_FILTER
{
	int iID;
};

struct C_PLAYER_COUNT_IN_RADIUS
{
  float fRadius;
};

struct C_PLAYER_COUNT_IN_REGION
{
  POLICY_ZONE_VERT zvMin;
  POLICY_ZONE_VERT zvMax;
};

//----------------------------------------------------------------------------------------
//Operation
//----------------------------------------------------------------------------------------

struct O_ATTACK_TYPE
{
	unsigned int uType;
};

struct O_USE_SKILL
{
	unsigned int uSkill; //����
	unsigned int uLevel; //����
};

struct O_TALK_TEXT
{
	unsigned int uSize;
	policy_char* szData;
	unsigned int uAppendDataMask;	//	��������ѡ��
};

struct O_RUN_TRIGGER
{
	unsigned int uID;
};

struct O_STOP_TRIGGER
{
	unsigned int uID;
};

struct O_ACTIVE_TRIGGER
{
	unsigned int uID;
};

struct O_CREATE_TIMER
{
	unsigned int uID;
	unsigned int uPeriod;
	unsigned int uCounter;
};

struct O_KILL_TIMER
{
	unsigned int uID;
};

struct O_ACTIVE_CONTROLLER
{
	unsigned int uID;
	bool         bStop;//�Ƿ���ֹͣ����
};

struct O_SET_GLOBAL_VERSION3
{
	int  iID;
	int  iValue;
};

struct O_SET_GLOBAL
{
	int  iID;
	int  iValue;
	bool bIsValue;	//�Ƿ��ǰ�ֱ����ȫ�ֱ������ǰ�ȫ�ֱ���ֵ��ȫ�ֱ���
};

struct O_REVISE_GLOBAL
{
	int  iID;
	int  iValue;
};


struct O_SUMMON_MONSTER_VERSION6
{
	int	  iMonsterID;//���ٻ��Ĺ���ID
	int	  iRange;//��Ŀ��ĵľ���(m)
	int   iLife; //��ʧ�ӳ�ʱ��(��) , -1��ʾ����
	bool  bDispear;//�Ƿ���Ŀ����������ʧ
	bool  bUsePolicyTarget; //Ŀ������ 0,��ʾ������1��ʾ�ò��Ե�Ŀ��
	int   iPathID; //·��ID
	int	  iMonsterNum;//��������
};

struct O_SUMMON_MONSTER
{
	int	  iMonsterID;//���ٻ��Ĺ���ID
	int	  iRange;//��Ŀ��ĵľ���(m)
	int   iLife; //��ʧ�ӳ�ʱ��(��) , -1��ʾ����
	int   iDispearCondition;//�Ƿ���Ŀ����������ʧ������Ŀ�жϱ��ٻ��Ĺ����Ƿ���Ŀ������ٻ�����ʧ
					//0��1��2��3��4��ѡ��0�ǲ����κ�����������1�����ٻ���������2�����ٻ�Ŀ��������3���ٻ��ߺ��ٻ�Ŀ������һ������������

	int   iPathID; //·��ID
	int	  iMonsterNum;//��������
};

struct O_WALK_ALONG
{
	int iWorldID;
	int iPathID;
	int iPatrolType;
	int iSpeedType;
};

struct O_PLAY_ACTION_VERSION8
{
	char szActionName[128]; //¶¯×÷Ãû×Ö
	bool bLoop;				//ÊÇ·ñÑ­»·²¥·Å
	int	 iInterval;			//¼ä¸ôÊ±¼äºÀÃë
};

struct O_PLAY_ACTION_VERSION37
{
	char szActionName[128]; //¶¯×÷Ãû×Ö
	//bool bLoop;				//ÊÇ·ñÑ­»·²¥·Å
	int  iLoopCount;		//Ñ­»·²¥·Å´ÎÊý 
	int	 iInterval;			//¼ä¸ôÊ±¼äºÀÃë
	int	 iPlayTime;
};

struct O_PLAY_ACTION
{
	char szActionName[128]; //¶¯×÷Ãû×Ö
	//bool bLoop;				//ÊÇ·ñÑ­»·²¥·Å
	int  iLoopCount;		//Ñ­»·²¥·Å´ÎÊý 
	int	 iInterval;			//¼ä¸ôÊ±¼äºÀÃë
	int	 iPlayTime;
	int	 iUnknown;
};

struct O_REVISE_HISTORY
{
	int  iID;
	int  iValue;
};

struct O_SET_HISTORY
{
	int  iID;
	int  iValue;
	bool bIsHistoryValue;	//  iValue ֵΪ��ʷ����ֵ������������
};

struct O_DELIVER_FACTION_PVP_POINTS
{
	unsigned int uType;
};

struct O_CALC_VAR
{
	int  iDst;			//	�洢�������ı���ID
	int  iDstType;		//	iDst ������ ȡֵ enumPolicyVarType �� 0~1
	int	 iSrc1;			//	����1
	int  iSrc1Type;		//	����1���ͣ��μ� enumPolicyVarType
	int  iOp;			//	����������ͣ����� enumPolicyOperatorType
	int	 iSrc2;		//	����2
	int  iSrc2Type;	//	����2���ͣ��μ� enumPolicyVarType
};

struct O_SUMMON_MONSTER_2
{
	int   iDispearCondition;//�Ƿ���Ŀ����������ʧ������Ŀ�жϱ��ٻ��Ĺ����Ƿ���Ŀ������ٻ�����ʧ
	//0��1��2��3��4��ѡ��0�ǲ����κ�����������1�����ٻ���������2�����ٻ�Ŀ��������3���ٻ��ߺ��ٻ�Ŀ������һ������������
	int	  iMonsterID;//���ٻ��Ĺ���ID
	int   iMonsterIDType;	//	iMonsterID ���壺enumPolicyVarType ��ȡֵ 0~2
	int	  iRange;//��Ŀ��ĵľ���(m)
	int   iLife; //��ʧ�ӳ�ʱ��(��) , -1��ʾ����	
	int   iPathID; //·��ID
	int	  iPathIDType;		//	iPathID ���壺enumPolicyVarType ��ȡֵ 0~2
	int	  iMonsterNum;//��������
	int	  iMonsterNumType;	//	iMonsterNum ���壺enumPolicyVarType ��ȡֵ 0~2
};

struct O_WALK_ALONG_2
{
	int iWorldID;		//	��ͼID
	int iPathID;
	int iPathIDType;	//	iPathID ���壺enumPolicyVarType ��ȡֵ 0~2
	int iPatrolType;	//	Ѳ�߷�ʽ
	int iSpeedType;		//	�ٶ�����
};

struct O_USE_SKILL_2
{
	unsigned int uSkill;		//����
	unsigned int uSkillType;	//uSkill ���壺enumPolicyVarType ��ȡֵ 0~2
	unsigned int uLevel;		//����
	unsigned int uLevelType;	//uLevel ���壺enumPolicyVarType ��ȡֵ 0~2
};

struct O_ACTIVE_CONTROLLER_2
{
	unsigned int uID;
	unsigned int uIDType;		//uID ���壺enumPolicyVarType ��ȡֵ 0~2
	bool         bStop;//�Ƿ���ֹͣ����
};

struct O_DELIVER_TASK
{
	unsigned int uID;
	unsigned int uIDType;		//uID ���壺enumPolicyVarType ��ȡֵ 0~2
};

struct O_SUMMON_MINE_VERSION_23
{
	int   iLifeType;		//	iLife º¬Òå£ºenumPolicyVarType ÖÐÈ¡Öµ 0~2
	int	  iMineID;
	int   iMineIDType;		//	iMineID º¬Òå£ºenumPolicyVarType ÖÐÈ¡Öµ 0~2
	int	  iRange;			//	ÓëÄ¿±êµÄµÄ¾àÀë(m)
	int   iLife;			//	ÏûÊ§ÑÓ³ÙÊ±¼ä(Ãë) , -1±íÊ¾ÎÞÇî
	int	  iMineNum;
	int	  iMineNumType;		//	iMineNum º¬Òå£ºenumPolicyVarType ÖÐÈ¡Öµ 0~2
};

struct O_SUMMON_MINE
{
	int   iLifeType;		//	iLife ���壺enumPolicyVarType ��ȡֵ 0~2
	int	  iMineID;
	int   iMineIDType;		//	iMineID ���壺enumPolicyVarType ��ȡֵ 0~2
	int	  iRange;			//	��Ŀ��ĵľ���(m)
	int   iLife;			//	��ʧ�ӳ�ʱ��(��) , -1��ʾ����
	int	  iMineNum;
	int	  iMineNumType;		//	iMineNum ���壺enumPolicyVarType ��ȡֵ 0~2

	bool  bSetOwnerAsMonsterOwner;
};

struct O_SUMMON_NPC
{
	int   iLifeType;		//	iLife ���壺enumPolicyVarType ��ȡֵ 0~2
	int	  iNPCID;
	int   iNPCIDType;		//	iNPCID ���壺enumPolicyVarType ��ȡֵ 0~2
	int	  iRange;			//	��Ŀ��ĵľ���(m)
	int   iLife;			//	��ʧ�ӳ�ʱ��(��) , -1��ʾ����	
	int   iPathID;			//	·��ID
	int	  iPathIDType;		//	iPathID ���壺enumPolicyVarType ��ȡֵ 0~2
	int	  iNPCNum;
	int	  iNPCNumType;		//	iNPCNum ���壺enumPolicyVarType ��ȡֵ 0~2
};

struct O_DELIVER_RANDOM_TASK_IN_REGION
{
	unsigned int		uID;		//	TASK_LIST_CONFIG id
	POLICY_ZONE_VERT	zvMin;		//	����Χ��Сֵ
	POLICY_ZONE_VERT	zvMax;		//	����Χ���ֵ
};

struct O_DELIVER_TASK_IN_HATE_LIST
{
	unsigned int uID;
	unsigned int uIDType;		//uID ���壺enumPolicyVarType ��ȡֵ 0~2
	int			 iRange;		//��������ľ���(m)
	int			 iPlayerNum;	//��������������500��
};

struct O_CLEAR_TOWER_TASK_IN_REGION
{
	POLICY_ZONE_VERT	zvMin;		//	����Χ��Сֵ
	POLICY_ZONE_VERT	zvMax;		//	����Χ���ֵ
};


struct O_SAVE_PLAYER_COUNT_IN_RADIUS_TO_PARAM
{
  float fRadiusValue;
  unsigned int uRadiusType;
  int iTargetID;
  unsigned int uTargetType;
};

struct O_SAVE_PLAYER_COUNT_IN_REGION_TO_PARAM
{
  POLICY_ZONE_VERT zvMin;
  POLICY_ZONE_VERT zvMax;
  int iTargetID;
  unsigned int uTargetType;
};

struct O_SKILL_WITH_TALK
{
  O_USE_SKILL_2 skill;
  O_TALK_TEXT talk;
};

struct O_USE_SKILL_3
{
  unsigned int uSkill;
  unsigned int uSkillType;
  unsigned int uLevel;
  unsigned int uLevelType;
  int tmpID;
  int life;
};

struct O_SORT_NUM
{
  int uNum;
  unsigned int uuNumType;
  unsigned int varID;
  unsigned int varType;
};

struct O_GET_POS_NUM
{
  int uNum;
  unsigned int uuNumType;
  unsigned int pos;
  unsigned int posType;
  unsigned int varID;
  unsigned int varType;
};

struct O_AUTO_BIND_CARRIER
{
  float range;
};

struct O_ADD_RANGE_TO_HATE_LIST
{
  int range;
  unsigned int rangeType;
};

struct O_SAVE_ALIVE_PLAYER_COUNT_IN_RADIUS_TO_PARAM
{
  float fRadiusValue;
  unsigned int uRadiusType;
  int iTargetID;
  unsigned int uTargetType;
};

struct O_SAVE_ALIVE_PLAYER_COUNT_IN_REGION_TO_PARAM
{
  POLICY_ZONE_VERT zvMin;
  POLICY_ZONE_VERT zvMax;
  int iTargetID;
  unsigned int uTargetType;
};

struct O_WALK_ALONG_3
{
  int iWorldID;
  int iStartPathID;
  int iStartPathIDType;
  int iEndPathID;
  int iEndPathIDType;
  int iPatrolType;
  int iSpeedType;
};

struct O_WALK_ALONG_4
{
  int iWorldID;
  int iStartPathID;
  int iStartPathIDType;
  int iEndPathID;
  int iEndPathIDType;
  int iMinPathID;
  int iMinPathIDType;
  int iMaxPathID;
  int iMaxPathIDType;
  int iPatrolType;
  int iSpeedType;
  int iOrientationType;
};

struct O_SAVE_TIME
{
  int iTimeType;
  int iParamID;
  int iParamIDType;
};

struct O_RANDOM_ASSIGNMENT
{
  int iRandomMin;
  int iRandomMax;
  int iQuantity;
  int iTargetID;
};

struct O_CARRIER_VOTING
{
  int iVoteID;
  int iVoteIDType;
};

struct O_VOTING_RESULT
{
  int iVoteID;
  int iVoteIDType;
  int iSelect;
  int iSelectType;
  int iSaveResult;
  int iSaveResultType;
};

struct POLICY_VOTING_SHOW
{	
	int unk1;
	int unk2;
};

struct O_VOTING_SHOW
{
  int iStyle;
  POLICY_VOTING_SHOW cwData[8];
};

struct O_CARRIER_DELIVERY_TASK
{
  int iTaskID;
  int iTaskIDType;
};

struct O_CARRIER_NOENTRY
{
  int iOpen;
};

struct O_TALK_TEXT_2
{
  O_TALK_TEXT talk;
  int iChannelKinds;
  int iChannelKindsType;
  int iChannelID;
  int iChannelIDType;
};

struct O_CREATE_TIMER_2
{
  unsigned int uID;
  int iIDType;
  unsigned int uPeriod;
  int iPeriodType;
  unsigned int uCounter;
  int iCounterType;
};

struct O_KILL_TIMER_2
{
  unsigned int uID;
  int iIDType;
};

struct O_CHANGE_MONSTER_FIGHTING_STATE
{
  int iChangeState;
};

struct O_CHANGE_MONSTER_ACTIVE_PASSIVE
{
  int iChoose;
};

struct O_CHILD_CARRIER_PARENT_MONSTER
{
  float fRange;
  int iPos;
};

struct O_CLOSE_CHILD_MONSTER
{
  int iMonsterID;
};

struct O_DELIVER_HATE_TARGETS
{
  int iMonsterID;
};

struct O_CHANGE_MONSTER_ATTACK_POLICY
{
  int iAttackPolicyID;
};

struct O_SPECIFY_FAILED_TASK_ID
{
  int iTaskID;
  int iTaskIDType;
};

struct O_SPECIFY_FAILED_TASK_ID_REGIONAL
{
  int iTaskID;
  int iTaskIDType;
  POLICY_ZONE_VERT zvMin;
  POLICY_ZONE_VERT zvMax;
};

struct O_64
{
  int ID;
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
  int Val7;
  int Val8;
  int Val9;
  int Val10;
  int Val11;
  int Val12;
  int Val13;
  int Val14;
  int Val15;
  int Val16;
  int Val17;
  int Val18;
  int Val19;
  int Val20;
};

struct O_65
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
  int Val7;
  int Val8;
  int Val9;
  int Val10;
  int Val11;
};

struct O_66
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
  int Val7;  
  int Val8;  
};

struct O_67
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;  
  int Val7;  
};

struct O_68
{
  int Val1;
  int Val2;  
  int Val3;  
};

struct O_69
{
  int Val1;  
  int Val2;  
};

struct O_70
{
  int Val1;
};

struct O_71
{
  int Val1;
  int Val2;
};

struct O_72
{
  O_TALK_TEXT talk;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
  int Val7;  
};

struct O_73
{
   int Val1;
   int Val2;
};

struct O_74
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
  int Val7;
  int Val8;
  int Val9;
  int Val10;
  int Val11;
  int Val12;
  int Val13;
  int Val14;
  int Val15;
  int Val16;  
  int Val17;  
};

struct O_75
{
  int Val1; 
};

struct O_76
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;  
};

struct O_77
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;  
  int Val6; 
};

struct O_78
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
  int Val7;
  int Val8;
  int Val9;
  int Val10;
};

struct O_79
{
  int Val1;
  int Val2;
};

struct O_80
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
};

struct O_81
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
};

struct O_82
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
  int Val7;
};

struct O_83
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
  int Val7;
  int Val8;
  int Val9;
  int Val10;
  int Val11;
};

struct O_84
{
  //
};

struct O_85
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
  int Val7;
};

struct O_86
{
  int Val1;
  int Val2;
  int Val3;
};

struct O_87
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
  int Val7;
  int Val8;
  int Val9;
  int Val10;
  int Val11;
};

struct O_88
{
  int Val1;
  int Val2;
  int Val3;
};

struct O_89
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
  int Val7;
  int Val8;
  int Val9;
  int Val10;
};

struct O_90
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
};

struct O_91
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
};

struct O_92
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
  int Val7;
  int Val8;
  int Val9;
};

struct O_93
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
  int Val7;
  int Val8;
  int Val9;
  int Val10;
};

struct O_94
{
  bool Val1;
};

struct O_95
{
  bool Val1;
};

struct O_96
{
  int Val1;
  int Val2;  
};

struct O_97
{
  //
};

struct O_98
{
  int Val1; 
};

struct O_99
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
  int Val7;
  int Val8;
  int Val9;
  int Val10;
  int Val11;
  int Val12;
  int Val13;
  int Val14;
  int Val15;
  int Val16;
  int Val17;
  int Val18;
  int Val19;
  int Val20;  
};

struct O_100
{
  int Val1;
  int Val2;  
};

struct O_101
{
  int Val1;
  int Val2;    
};

struct O_102
{
  int Val1;
  int Val2;    
};

struct O_103
{
  int Val1;
  int Val2;  
  int Val3;  
};

struct O_104
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;
  int Val6;
  int Val7;
  int Val8;
  int Val9;
  int Val10;
  int Val11;
  int Val12;
  int Val13;
  int Val14;
  int Val15;
  int Val16;
  int Val17;
  int Val18;
  int Val19;
  int Val20;
  int Val21;
  int Val22;
  int Val23;
  int Val24;
  int Val25;
  int Val26;  
};

struct O_105
{
  int Val1;
  int Val2;
  int Val3;
  int Val4;
  int Val5;  
};

struct T_OCCUPATION
{
	unsigned int uBit;
};

struct T_RANGE
{
  unsigned int meter;
};

#endif
