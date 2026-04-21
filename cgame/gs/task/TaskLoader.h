//----------------------------------------------------------------------------//
// -- TASK LOADER 2 x64
//----------------------------------------------------------------------------//
#ifndef _ATASK_LOADER_H_
#define _ATASK_LOADER_H_

#include "TaskTempl.h"

#pragma pack(push, 1)

//struct FILE;
class TaskPreloader
{
private:
	struct
	{
		short 					x_IDSpace; //0 Normal - 1 Extra
		unsigned int			m_ID;
		task_char				m_szName[MAX_TASK_NAME_LEN];
		//task_char*			m_pszSignature;
		unsigned int			m_ulType;
		unsigned int			m_ulTimeLimit;
		bool					m_bAbsFail;
		task_tm					m_tmAbsFailTime;
		bool					x_NextFail;
		task_tm 				x_NextFailTime; //172
		unsigned int			m_ulTimetable;
		char					m_tmType[MAX_TIMETABLE_SIZE];
		//task_tm*				m_tmStart;
		//task_tm*				m_tmEnd;
		int						m_lAvailFrequency;
		int						m_lPeriodLimit;
		bool 					m_AvailFrequency;
		task_tm 				x_AvailFrequencyTime;  //172
		unsigned int			m_ulMaxReceiver;
		unsigned int			m_ulDelvWorld;
		unsigned int			m_ulDelvRegionCnt;
		//Task_Region*			m_pDelvRegion;	
		unsigned int			m_ulEnterRegionWorld;
		unsigned int			m_ulEnterRegionCnt;
		//Task_Region*			m_pEnterRegion;	
		unsigned int			m_ulLeaveRegionWorld;
		unsigned int			m_ulLeaveRegionCnt;
		//Task_Region*			m_pLeaveRegion;	
		unsigned int			m_ulTransWldId;
		ZONE_VERT				m_TransPt;
		int						m_lMonsCtrl;
		unsigned int			m_ulSuitableLevel;
		unsigned int			m_ulDelvNPC;
		unsigned int			m_ulAwardNPC;
		unsigned int   			m_ulChangeKeyCnt;
		//int					*m_plChangeKey;		 
		//int            		*m_plChangeKeyValue;
		//bool            		*m_pbChangeType;
		int						x_PersonChangeKeyCnt;      //172
		//int					*x_PersonChangeKey;         //172
		//int					*x_PersonChangeKeyValue;    //172
		int						x_HistoryChangeKeyCnt;     //172
		//int					*x_HistoryChangeKey;        //172
		//int					*x_HistoryChangeKeyValue;   //172
		//bool            		*x_HistoryChangeType;			//172   
		int						m_iDeliveredSkillID;
		int						m_iDeliveredSkillLevel;
		int 					m_ulInventorySlotNum;
		unsigned int			m_ulPQExpCnt;
		//char					(*m_pszPQExp)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
		//TASK_EXPRESSION		(*m_pPQExpArr)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
		unsigned int			m_ulMonsterContribCnt;
		//MONSTERS_CONTRIB*		m_MonstersContrib;
		int						m_iPremNeedRecordTasksNum;
		int						m_iPremiseFactionContrib;
		unsigned int			m_ulAccountTaskLimitCnt;
		int 					x_FactionMem; 
		int 					x_CrossServer;                 //172
		int 					x_IconStateIDForAwardVar;      //172
		int 					x_GlobalKeyForAwardVar;        //172
		int 					x_PersonKeyForAwardVar;        //172
		int 					x_HistoryKeyForAwardVar;       //172
		bool					x_FailNeedComp;				   //172
		int 					x_FailExp1AndOrExp2;           //172
		COMPARE_KEY_VALUE 		x_Fail1KeyValue;               //172
		COMPARE_KEY_VALUE 		x_Fail2KeyValue;               //172
		bool					x_FailPersonNeedComp;				 //174
		int 					x_FailPersonExp1AndOrExp2;           //174
		COMPARE_KEY_VALUE 		x_FailPerson1KeyValue;               //174
		COMPARE_KEY_VALUE 		x_FailPerson2KeyValue;               //174
		int 					x_HideOtherPlayerTime;         //172
		int 					x_PropMonsterSummonedCnt;      //172
		//AWARD_MONSTERS_SUMMONED x_PropMonsterSummoned;         //172
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
		//ITEM_WANTED*			m_PremItems;
		unsigned int			m_ulGivenItems;
		unsigned int			m_ulGivenCmnCount;
		unsigned int			m_ulGivenTskCount;
		//ITEM_WANTED*			m_GivenItems;
		unsigned int			m_ulPremise_Deposit;
		int						m_lPremise_Reputation;
		int						m_lPremise_RepuMax;
		unsigned int			m_ulPremise_Task_Count;
		unsigned int			m_ulPremise_Tasks[MAX_PREM_TASK_COUNT];
		short 					x_PremiseTaskSpace[MAX_PREM_TASK_COUNT];
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
		short 					x_MutexTaskSpace[MAX_MUTEX_TASK_COUNT];
		unsigned int			m_ulMutexTasks[MAX_MUTEX_TASK_COUNT];
		int						m_lSkillLev[MAX_LIVING_SKILLS];
		char					m_DynTaskType;
		unsigned int			m_ulSpecialAward;		
		float					m_fTeammateDist;		
		float					m_fSuccDist;			
		float					m_fRcvMemDist;			
		float					m_fCntMemDist;			
		unsigned int			m_ulTeamMemsWanted;
		//TEAM_MEM_WANTED*		m_TeamMemsWanted;
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
		//int*			  		m_PremTitles;
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
		short					x_NeededTaskSpace[MAX_NEEDED_TASK_COUNT];      //172
		int						x_NeededTask[MAX_NEEDED_TASK_COUNT];           //172
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
		//PLAYER_WANTED*  		m_PlayerWanted;
		unsigned int			m_ulMonsterWanted;
		//MONSTER_WANTED*		m_MonsterWanted;	
		unsigned int			m_ulItemsWanted;
		//ITEM_WANTED*			m_ItemsWanted;
		unsigned int			m_ulGoldWanted;	
		int						m_iFactionContribWanted;
		int						m_iFactionExpContribWanted;	
		unsigned int			m_ulNPCToProtect;
		unsigned int			m_ulProtectTimeLen;	
		unsigned int			m_ulNPCMoving;
		unsigned int			m_ulNPCDestSite;	
		//Task_Region*			m_pReachSite;
		unsigned int			m_ulReachSiteCnt;
		unsigned int			m_ulReachSiteId;
		unsigned int			m_ulWaitTime;	
		ZONE_VERT				m_TreasureStartZone;
		unsigned char			m_ucZonesNumX;
		unsigned char			m_ucZonesNumZ;
		unsigned char   		m_ucZoneSide;	
		short 					x_treasureZoneMapID;
		//Task_Region*			m_pLeaveSite;
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
		//char					(*m_pszExp)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
		//TASK_EXPRESSION 		(*m_pExpArr)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
		int 					x_PersonExpCnt;                                       //172
		//char	 				(*x_PersonExp)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];      //172
		//TASK_EXPRESSION			(*x_PersonExpArr)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];   //172
		int 					x_HistoryExpCnt;                                      //172
		//char	 				(*x_HistoryExp)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];     //172
		//TASK_EXPRESSION			(*x_HistoryExpArr)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];  //172
		unsigned int       		m_ulTaskCharCnt;
		//task_char           	(*m_pTaskChar)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
		int 					x_PersonTaskCharCnt;                                     //172
		//task_char 				(*x_PersonTaskChar)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];    //172
		int 					x_HistoryTaskCharCnt;                                    //172
		//task_char 				(*x_HistoryTaskChar)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];   //172
		unsigned char			m_ucTransformedForm;
		//int 					x_TransformedDescID;
		unsigned int			m_ulReachLevel;
		unsigned int			m_ulReachReincarnationCount;
		unsigned int			m_ulReachRealmLevel;	
		unsigned int			m_uiEmotion;	
		int 					x_GrowthTarget;
		unsigned int			m_ulTMIconStateID;
		//int 					x_TMIconStateDescID;
		unsigned int 			m_ulTMHomeLevelType;
		unsigned int 			m_ulTMReachHomeLevel;
		unsigned int 			m_ulTMReachHomeFlourish;
		unsigned int 			m_ulHomeItemsWanted;
		//HOME_ITEM_WANTED*   	m_HomeItemsWanted;
		int 					x_TMBindCarrierId;
		short 					x_TMReachArenaTime;
		short 					x_ReachArenaWinCount;
		short 					x_Dialog[20];
		int 					x_SuccTriggerEvent;    //172
		int 					x_VerifyStateLayer;    //172
		int 					x_VerifyStateLayerID;  //172
		int 					x_KeyWordIndex;        //172
		short 					x_TaskEventChar[30];
		int   					x_TaskEventType;
		short 					x_TaskEventTime;
		short 					x_TaskEventMonsterCount;
		//TaskEventData* 			x_TaskEventMonsterData;
		short 					x_TaskEventMineCount;
		//TaskEventData* 			x_TaskEventMineData;
		short 					x_TaskEventTaskCount;
		//TaskEventData* 			x_TaskEventTaskData;
		short 					x_FactionPersonScore;
		char 					x_TMTinyGameType;
		int 					x_m_ulGold2Wanted;   //172
		int 					x_BarrageChannel;    //172
		int 					x_GuessInputID;      //174
		unsigned int			m_ulAwardType_S;
		unsigned int			m_ulAwardType_F;
		//AWARD_DATA* 			m_Award_S;
		//AWARD_DATA* 			m_Award_F;
		//AWARD_RATIO_SCALE* 		m_AwByRatio_S;
		//AWARD_RATIO_SCALE* 		m_AwByRatio_F;	
		//AWARD_ITEMS_SCALE* 		m_AwByItems_S;
		//AWARD_ITEMS_SCALE* 		m_AwByItems_F;	
		//AWARD_RATIO_SCALE* 		x_AwByTaskEventRatio_S;
		//AWARD_RATIO_SCALE* 		x_AwByTaskEventRatio_F;
		//AWARD_CARRIER_HP_SCALE* x_AwByCarrierHp_S;
		//AWARD_CARRIER_HP_SCALE* x_AwByCarrierHp_F;
		//AWARD_VAR_SCALE* 		x_AwByVar_S; 
		//AWARD_VAR_SCALE* 		x_AwByVar_F; 
		unsigned int			m_ulParent;
		unsigned int			m_ulPrevSibling;
		unsigned int			m_ulNextSibling;
		unsigned int			m_ulFirstChild;	
		float					m_fLibraryTasksProbability;
		int						m_iWorldContribution;
		char					x_CombinedSwitches[MAX_COMBINED_SWITCHES];            	//172
		int 					x_SpecialParametersCount;      	//172
		//SPECIAL_PARAMETERS* 	x_SpecialParameters;			//172
		//int 					reserve[20];
	}	task;

	struct
	{
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
		//AWARD_ITEMS_CAND*			m_CandItems;
		unsigned int				m_ulSummonedMonsters;
		//AWARD_MONSTERS_SUMMONED* 	m_SummonedMonsters;
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
		//AWARD_PQ_RANKING*			m_PQRankingAward;
		unsigned int       			m_ulChangeKeyCnt;
		//int                		*m_plChangeKey;
		//int                		*m_plChangeKeyValue;
		//bool                		*m_pbChangeType;
		unsigned int				m_ulPersonChangeCnt;           //172
		//int						*m_plPersonChangeKey;          //172
		//int						*m_plPersonChangeKeyValue;     //172
		//bool                		*m_pbPersonChangeType;         //172
		unsigned int				m_ulNewChangeCnt;              //172
		//int						*m_plNewChangeKey;             //172
		//int						*m_plNewChangeKeyValue;        //172
		//bool                		*m_pbNewChangeType;	           //172
		unsigned int				m_ulHistoryChangeCnt;
		//int						*m_plHistoryChangeKey;
		//int						*m_plHistoryChangeKeyValue;
		//bool                		*m_pbHistoryChangeType;
		bool                		m_bMulti;
		int                 		m_nNumType;
		int                			m_lNum;
		unsigned int       			m_ulDisplayKeyCnt;
		//int                		*m_plDisplayKey;
		unsigned int				m_ulExpCnt;
		//char						(*m_pszExp)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
		//TASK_EXPRESSION 			(*m_pExpArr)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
		unsigned int       			m_ulTaskCharCnt;
		//task_char           		(*m_pTaskChar)[TASK_AWARD_MAX_DISPLAY_CHAR_LEN];
		int							m_iForceContribution;
		int							m_iForceReputation;
		int							m_iForceActivity;
		int							m_iForceSetRepu;
		int							m_iTaskLimit;
		unsigned int				m_ulTitleNum;
		//TITLE_AWARD*				m_pTitleAward;	
		int							m_iLeaderShip;
		int							m_iWorldContribution; ///++++++++++
		int 						m_iHomeResource[5];
		bool 						m_bCreateHome;	
		bool 						AwardLeaveCarrier;
		int 						AwardFactionMoney;
		int 						AwardFactionPersonScore;
		int 						AwardFactionTarget;
		//FACTION_TARGET_DATA* 		AwardFactionTargetData;
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
	}	award;

	struct
	{
		//int*	ParameterTypes;
		//int*	unk176;
		int 	FailWhenGetOffCarrier;
		int 	TimeWhenRideCarrier;
		int 	GetOffCarrierID;
		int 	RideCarrierID;
		int 	UseItemTimes;
		int 	UseItemCount;
		//int*	UseItemIDs;
		int 	CompleteTaskTimes;
		int 	CompleteTaskCount;
		int 	ByCompleteTask;
		//int*	CompleteTaskIDs;
		//int*	CompleteTaskIDsSpace;
		int		Parameter9;
		// Anedota /*160+*/
		int		need_story_book_collection;
		int		story_book_collection_count;
	}	spec;
public:
	TaskPreloader();
	~TaskPreloader();
public:

	void TaskPreload(ATaskTemplFixedData* data, FILE* fp);
	void AwardPreload(AWARD_DATA* data, FILE* fp);
	void SpecPreload(SPECIAL_PARAMETERS* data, FILE* fp);

public:
	static int dbg_fread(void* ptrvoid, int size, int count, FILE* filestream);
private:
};

#pragma pack(pop)
#endif