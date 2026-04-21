//===========================================================================
// -- TASK LOADER 2 x64
//===========================================================================
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdint>
#include <cstring>

#include "TaskTempl.h"
#include "TaskTemplMan.h"
#include "TaskLoader.h"

#ifdef	_WINDOWS
int TASK_DEFAULT_MAX_SIMULTANEOUS_COUT() { return 20; }
int TASK_MAX_SIMULTANEOUS_COUT() { return 30; }
int TASK_HIDDEN_COUNT() { return 30; }
int TASK_TITLE_TASK_COUNT() { return 10; }
#endif

TaskPreloader::TaskPreloader()
{
	memset(this, 0x00, sizeof(*this));
}

TaskPreloader::~TaskPreloader()
{
	
}

int TaskPreloader::dbg_fread( void * ptrvoid, int size, int count, FILE * filestream )
{
	return fread(ptrvoid,size,count,filestream);
}

void TaskPreloader::TaskPreload(ATaskTemplFixedData * data, FILE* fp)
{
	dbg_fread(&this->task, sizeof(this->task), 1, fp);
	
	data->x_IDSpace =                                this->task.x_IDSpace;                                                          
	data->m_ID =                                     this->task.m_ID;                                                               
	memcpy(data->m_szName,							 this->task.m_szName, sizeof(data->m_szName));                                     
	data->m_ulType =                                 this->task.m_ulType;                                                           
	data->m_ulTimeLimit =                            this->task.m_ulTimeLimit;                                                      
	data->m_bAbsFail =                               this->task.m_bAbsFail;                                                         
	data->m_tmAbsFailTime =                          this->task.m_tmAbsFailTime;                                                    
	data->x_NextFailTime =                           this->task.x_NextFailTime;                                                     
	data->m_ulTimetable =                            this->task.m_ulTimetable;    
	memcpy(data->m_tmType,							 this->task.m_tmType, sizeof(data->m_tmType));                                                                                         
	data->m_lAvailFrequency =                        this->task.m_lAvailFrequency;                                                  
	data->m_lPeriodLimit =                           this->task.m_lPeriodLimit;  		
	data->x_AvailFrequencyTime =                     this->task.x_AvailFrequencyTime;                                               
	data->m_ulMaxReceiver =                          this->task.m_ulMaxReceiver;                                                    
	data->m_ulDelvWorld =                            this->task.m_ulDelvWorld;                                                      
	data->m_ulDelvRegionCnt =                        this->task.m_ulDelvRegionCnt;                                                  
	data->m_ulEnterRegionWorld =                     this->task.m_ulEnterRegionWorld;                                               
	data->m_ulEnterRegionCnt =                       this->task.m_ulEnterRegionCnt;                                                 
	data->m_ulLeaveRegionWorld =                     this->task.m_ulLeaveRegionWorld;                                               
	data->m_ulLeaveRegionCnt =                       this->task.m_ulLeaveRegionCnt;                                                 
	data->m_ulTransWldId =                           this->task.m_ulTransWldId;                                                     
	data->m_TransPt =                                this->task.m_TransPt;                                                          
	data->m_lMonsCtrl =                              this->task.m_lMonsCtrl;                                                        
	data->m_ulSuitableLevel =                        this->task.m_ulSuitableLevel;                                                  
	data->m_ulDelvNPC =                              this->task.m_ulDelvNPC;                                                        
	data->m_ulAwardNPC =                             this->task.m_ulAwardNPC;                                                       
	data->m_ulChangeKeyCnt =                         this->task.m_ulChangeKeyCnt;                                                   
	data->x_PersonChangeKeyCnt =                     this->task.x_PersonChangeKeyCnt;                                               
	data->x_HistoryChangeKeyCnt =     	             this->task.x_HistoryChangeKeyCnt;     	                                     
	data->m_iDeliveredSkillID =                      this->task.m_iDeliveredSkillID;                                                
	data->m_iDeliveredSkillLevel =                   this->task.m_iDeliveredSkillLevel;                                             
	data->m_ulInventorySlotNum =                     this->task.m_ulInventorySlotNum;                                               
	data->m_ulPQExpCnt =                             this->task.m_ulPQExpCnt;                                                                
	data->m_ulMonsterContribCnt =                    this->task.m_ulMonsterContribCnt;                                              
	data->m_iPremNeedRecordTasksNum =                this->task.m_iPremNeedRecordTasksNum;                                          
	data->m_iPremiseFactionContrib =                 this->task.m_iPremiseFactionContrib;                                           
	data->m_ulAccountTaskLimitCnt =                  this->task.m_ulAccountTaskLimitCnt;                                            
	data->x_FactionMem =                             this->task.x_FactionMem;                                                       
	data->x_CrossServer =                            this->task.x_CrossServer;                                                      
	data->x_IconStateIDForAwardVar =                 this->task.x_IconStateIDForAwardVar;                                           
	data->x_GlobalKeyForAwardVar =                   this->task.x_GlobalKeyForAwardVar;                                             
	data->x_PersonKeyForAwardVar =                   this->task.x_PersonKeyForAwardVar;                                             
	data->x_HistoryKeyForAwardVar =                  this->task.x_HistoryKeyForAwardVar;                                            
	data->x_FailExp1AndOrExp2 =                      this->task.x_FailExp1AndOrExp2;                                                
	data->x_Fail1KeyValue =                          this->task.x_Fail1KeyValue;                                                    
	data->x_Fail2KeyValue =                          this->task.x_Fail2KeyValue;
	//174
	data->x_FailPersonExp1AndOrExp2 =                this->task.x_FailPersonExp1AndOrExp2;                                                
	data->x_FailPerson1KeyValue =                    this->task.x_FailPerson1KeyValue;                                                    
	data->x_FailPerson2KeyValue =                    this->task.x_FailPerson2KeyValue; 
	//174
	data->x_HideOtherPlayerTime =                    this->task.x_HideOtherPlayerTime;                                              
	data->x_PropMonsterSummonedCnt =                 this->task.x_PropMonsterSummonedCnt;                                                                                         
	data->x_GfxType =                                this->task.x_GfxType;                                                          
	data->m_ucPremiseTransformedForm =               this->task.m_ucPremiseTransformedForm;                                         
	data->m_ulPremise_Lev_Min =                      this->task.m_ulPremise_Lev_Min;                                                
	data->m_ulPremise_Lev_Max =                      this->task.m_ulPremise_Lev_Max;                                                
	data->m_bPremCheckMaxHistoryLevel =              this->task.m_bPremCheckMaxHistoryLevel;                                        
	data->m_ulPremReincarnationMin =                 this->task.m_ulPremReincarnationMin;                                           
	data->m_ulPremReincarnationMax =                 this->task.m_ulPremReincarnationMax;                                           
	data->m_ulPremRealmLevelMin =                    this->task.m_ulPremRealmLevelMin;                                              
	data->m_ulPremRealmLevelMax =                    this->task.m_ulPremRealmLevelMax;                                              
	data->m_ulPremItems =                            this->task.m_ulPremItems;                                                      
	data->m_ulGivenItems =                           this->task.m_ulGivenItems;                                                     
	data->m_ulGivenCmnCount =                        this->task.m_ulGivenCmnCount;                                                  
	data->m_ulGivenTskCount =                        this->task.m_ulGivenTskCount;                                                  
	data->m_ulPremise_Deposit =                      this->task.m_ulPremise_Deposit;                                                
	data->m_lPremise_Reputation =                    this->task.m_lPremise_Reputation;                                              
	data->m_lPremise_RepuMax =                       this->task.m_lPremise_RepuMax;                                                 
	data->m_ulPremise_Task_Count =                   this->task.m_ulPremise_Task_Count;      
	memcpy(data->m_ulPremise_Tasks,					 this->task.m_ulPremise_Tasks, sizeof(data->m_ulPremise_Tasks)); 
	memcpy(data->x_PremiseTaskSpace,				 this->task.x_PremiseTaskSpace, sizeof(data->x_PremiseTaskSpace));
	data->m_ulPremise_Task_Least_Num =               this->task.m_ulPremise_Task_Least_Num;                                         
	data->m_ulPremise_Period =                       this->task.m_ulPremise_Period;                                                 
	data->m_ulPremise_Faction =                      this->task.m_ulPremise_Faction;                                                
	data->m_iPremise_FactionRole = 	                 this->task.m_iPremise_FactionRole; 	                                         
	data->m_ulGender =                               this->task.m_ulGender;                                                         
	data->m_ulOccupations =                          this->task.m_ulOccupations;    
	memcpy(data->m_Occupations,						 this->task.m_Occupations, sizeof(data->m_Occupations));                                                                                     
	data->m_ulPremRMBMin =                           this->task.m_ulPremRMBMin;                                                     
	data->m_ulPremRMBMax =                           this->task.m_ulPremRMBMax;                                                     
	data->m_iCharStartTime =                         this->task.m_iCharStartTime;                                                   
	data->m_iCharEndTime =                           this->task.m_iCharEndTime;                                                     
	data->m_tmCharEndTime =                          this->task.m_tmCharEndTime;                                                    
	data->m_ulCharTimeGreaterThan = 	             this->task.m_ulCharTimeGreaterThan; 	                                         
	data->x_PremiseCotaskSpace =                     this->task.x_PremiseCotaskSpace;                                               
	data->m_ulPremise_Cotask =                       this->task.m_ulPremise_Cotask;                                                 
	data->m_ulCoTaskCond =                           this->task.m_ulCoTaskCond;                                                     
	data->m_ulMutexTaskCount =                       this->task.m_ulMutexTaskCount; 
	memcpy(data->x_MutexTaskSpace,					 this->task.x_MutexTaskSpace, sizeof(data->x_MutexTaskSpace)); 
	memcpy(data->m_ulMutexTasks,					 this->task.m_ulMutexTasks, sizeof(data->m_ulMutexTasks)); 
	memcpy(data->m_lSkillLev,						 this->task.m_lSkillLev, sizeof(data->m_lSkillLev));                                                                                     
	data->m_DynTaskType =                            this->task.m_DynTaskType;                                                      
	data->m_ulSpecialAward = 		                 this->task.m_ulSpecialAward; 		                                             
	data->m_fTeammateDist = 		                 this->task.m_fTeammateDist; 		                                             
	data->m_fSuccDist = 			                 this->task.m_fSuccDist; 			                                             
	data->m_fRcvMemDist = 			                 this->task.m_fRcvMemDist; 			                                         
	data->m_fCntMemDist = 			                 this->task.m_fCntMemDist; 			                                         
	data->m_ulTeamMemsWanted =                       this->task.m_ulTeamMemsWanted;                                                 
	data->m_nPremExp1AndOrExp2 =                     this->task.m_nPremExp1AndOrExp2;                                               
	data->m_Prem1KeyValue =                          this->task.m_Prem1KeyValue;                                                    
	data->m_Prem2KeyValue =                          this->task.m_Prem2KeyValue;                                                    
	data->x_PremPersonExp1AndOrExp2 =                this->task.x_PremPersonExp1AndOrExp2;                                          
	data->x_PremPerson1KeyValue =                    this->task.x_PremPerson1KeyValue;                                              
	data->x_PremPerson2KeyValue =                    this->task.x_PremPerson2KeyValue;                                              
	data->x_PremHistoryExp1AndOrExp2 =               this->task.x_PremHistoryExp1AndOrExp2;                                         
	data->x_PremHistory1KeyValue =                   this->task.x_PremHistory1KeyValue;                                             
	data->x_PremHistory2KeyValue =                   this->task.x_PremHistory2KeyValue;                                             
	data->m_iPremForce =                             this->task.m_iPremForce;                                                       
	data->m_iPremForceReputation =                   this->task.m_iPremForceReputation;                                             
	data->m_iPremForceContribution =                 this->task.m_iPremForceContribution;                                           
	data->m_iPremForceExp =                          this->task.m_iPremForceExp;                                                    
	data->m_iPremForceSP =                           this->task.m_iPremForceSP;                                                     
	data->m_iPremForceActivityLevel =                this->task.m_iPremForceActivityLevel;                                          
	data->m_iPremTitleNumTotal =                     this->task.m_iPremTitleNumTotal;                                               
	data->m_iPremTitleNumRequired =                  this->task.m_iPremTitleNumRequired; 
	memcpy(data->m_iPremHistoryStageIndex,			 this->task.m_iPremHistoryStageIndex, sizeof(data->m_iPremHistoryStageIndex));                                                                                   
	data->m_ulPremGeneralCardCount =                 this->task.m_ulPremGeneralCardCount;                                           
	data->m_iPremGeneralCardRank =                   this->task.m_iPremGeneralCardRank;                                             
	data->m_ulPremGeneralCardRankCount =             this->task.m_ulPremGeneralCardRankCount;                                       
	data->m_ulPremIconStateID =                      this->task.m_ulPremIconStateID;                                                
	data->m_iVIPLevelMin =                           this->task.m_iVIPLevelMin;                                                     
	data->m_iVIPLevelMax =                           this->task.m_iVIPLevelMax;                                                     
	data->m_iPremHomeLevelMin =                      this->task.m_iPremHomeLevelMin;                                                
	data->m_iPremHomeLevelMax =                      this->task.m_iPremHomeLevelMax;                                                
	data->m_iPremHomeResourceLevelType =             this->task.m_iPremHomeResourceLevelType;                                       
	data->m_iPremHomeResourceLevelMin =              this->task.m_iPremHomeResourceLevelMin;                                        
	data->m_iPremHomeResourceLevelMax =              this->task.m_iPremHomeResourceLevelMax;                                        
	data->m_iPremHomeFactoryLevelType =              this->task.m_iPremHomeFactoryLevelType;                                        
	data->m_iPremHomeFactoryLevelMin =               this->task.m_iPremHomeFactoryLevelMin;                                         
	data->m_iPremHomeFactoryLevelMax =               this->task.m_iPremHomeFactoryLevelMax;                                         
	data->m_iPremHomeFlourishMin =                   this->task.m_iPremHomeFlourishMin;                                             
	data->m_iPremHomeFlourishMax =                   this->task.m_iPremHomeFlourishMax;                                             
	data->x_ArenaWinCount =                          this->task.x_ArenaWinCount;                                                    
	data->x_PreFactionScore =                        this->task.x_PreFactionScore;                                                  
	data->x_PreFactionMoney =                        this->task.x_PreFactionMoney;                                                  
	data->x_PreFactionPersonScore =                  this->task.x_PreFactionPersonScore;                                            
	data->x_PremMentorMorality =                     this->task.x_PremMentorMorality;                                               
	data->x_PremMentorStudent =                      this->task.x_PremMentorStudent;                                                
	data->x_PremCoupleIntimacy =                     this->task.x_PremCoupleIntimacy;                                               
	data->x_m_ulPremise_Deposit2 =                   this->task.x_m_ulPremise_Deposit2; // 172                                              
	data->x_NeededTaskCount =                        this->task.x_NeededTaskCount;                                                  
	data->x_NeededTaskLeast =                        this->task.x_NeededTaskLeast;                                                  
	data->x_PremCamp =                               this->task.x_PremCamp;                                                         
	data->x_PremCampPower =                          this->task.x_PremCampPower;                                                    
	data->x_NeedFriendPoint =                        this->task.x_NeedFriendPoint;                                                  
	data->x_RankType =                               this->task.x_RankType;                                                         
	data->x_RankGradeMin =                           this->task.x_RankGradeMin;                                                     
	data->x_RankGradeMax =                           this->task.x_RankGradeMax;                                                     
	data->x_RankGradeMinKey =                        this->task.x_RankGradeMinKey;                                                  
	data->x_RankGradeMaxKey =                        this->task.x_RankGradeMaxKey;                                                  
	data->m_enumMethod =                             this->task.m_enumMethod;                                                       
	data->m_enumFinishType =                         this->task.m_enumFinishType;                                                   
	data->m_ulPlayerWanted =                         this->task.m_ulPlayerWanted;                                                   
	data->m_ulMonsterWanted =                        this->task.m_ulMonsterWanted;                                                  
	data->m_ulItemsWanted =                          this->task.m_ulItemsWanted;                                                    
	data->m_ulGoldWanted = 	                         this->task.m_ulGoldWanted; 	                                                 
	data->m_iFactionContribWanted =                  this->task.m_iFactionContribWanted;                                            
	data->m_iFactionExpContribWanted = 	             this->task.m_iFactionExpContribWanted; 	                                     
	data->m_ulNPCToProtect =                         this->task.m_ulNPCToProtect;                                                   
	data->m_ulProtectTimeLen = 	                     this->task.m_ulProtectTimeLen; 	                                             
	data->m_ulNPCMoving =                            this->task.m_ulNPCMoving;                                                      
	data->m_ulNPCDestSite = 	                     this->task.m_ulNPCDestSite; 	                                                 
	data->m_ulReachSiteCnt =                         this->task.m_ulReachSiteCnt;                                                   
	data->m_ulReachSiteId =                          this->task.m_ulReachSiteId;                                                    
	data->m_ulWaitTime = 	                         this->task.m_ulWaitTime; 	                                                     
	data->m_TreasureStartZone =                      this->task.m_TreasureStartZone;                                                
	data->m_ucZonesNumX =                            this->task.m_ucZonesNumX;                                                      
	data->m_ucZonesNumZ =                            this->task.m_ucZonesNumZ;                                                      
	data->m_ucZoneSide = 	                         this->task.m_ucZoneSide; 	                                                     
	data->x_treasureZoneMapID =                      this->task.x_treasureZoneMapID;                                                
	data->m_ulLeaveSiteCnt =                         this->task.m_ulLeaveSiteCnt;                                                   
	data->m_ulLeaveSiteId = 	                     this->task.m_ulLeaveSiteId; 	                                                 
	data->m_nFinExp1AndOrExp2 =                      this->task.m_nFinExp1AndOrExp2;                                                
	data->m_Fin1KeyValue =                           this->task.m_Fin1KeyValue;                                                     
	data->m_Fin2KeyValue = 		                     this->task.m_Fin2KeyValue; 		                                             
	data->x_FinPersonExp1AndOrExp2 =                 this->task.x_FinPersonExp1AndOrExp2;                                           
	data->x_FinPerson1KeyValue =                     this->task.x_FinPerson1KeyValue;                                               
	data->x_FinPerson2KeyValue =                     this->task.x_FinPerson2KeyValue;                                               
	data->x_FinHistoryExp1AndOrExp2 =                this->task.x_FinHistoryExp1AndOrExp2;                                          
	data->x_FinHistory1KeyValue =                    this->task.x_FinHistory1KeyValue;                                              
	data->x_FinHistory2KeyValue =                    this->task.x_FinHistory2KeyValue;                                              
	data->m_ulExpCnt =                               this->task.m_ulExpCnt;                                                                              
	data->x_PersonExpCnt =                           this->task.x_PersonExpCnt;                                                          
	data->x_HistoryExpCnt =                          this->task.x_HistoryExpCnt;                                                         
	data->m_ulTaskCharCnt =                          this->task.m_ulTaskCharCnt;                                                           
	data->x_PersonTaskCharCnt =                      this->task.x_PersonTaskCharCnt;                                                
	data->x_HistoryTaskCharCnt =                     this->task.x_HistoryTaskCharCnt;                                               
	data->m_ucTransformedForm =                      this->task.m_ucTransformedForm;                                                                                               
	data->m_ulReachLevel =                           this->task.m_ulReachLevel;                                                     
	data->m_ulReachReincarnationCount =              this->task.m_ulReachReincarnationCount;                                        
	data->m_ulReachRealmLevel = 	                 this->task.m_ulReachRealmLevel; 	                                             
	data->m_uiEmotion = 	                         this->task.m_uiEmotion; 	                                                     
	data->x_GrowthTarget =                           this->task.x_GrowthTarget;                                                     
	data->m_ulTMIconStateID =                        this->task.m_ulTMIconStateID;                                                                                               
	data->m_ulTMHomeLevelType =                      this->task.m_ulTMHomeLevelType;                                                
	data->m_ulTMReachHomeLevel =                     this->task.m_ulTMReachHomeLevel;                                               
	data->m_ulTMReachHomeFlourish =                  this->task.m_ulTMReachHomeFlourish;                                            
	data->m_ulHomeItemsWanted =                      this->task.m_ulHomeItemsWanted;                                                
	data->x_TMBindCarrierId =                        this->task.x_TMBindCarrierId;                                                  
	data->x_TMReachArenaTime =                       this->task.x_TMReachArenaTime;                                                 
	data->x_ReachArenaWinCount =                     this->task.x_ReachArenaWinCount;  
	memcpy(data->x_Dialog,							 this->task.x_Dialog, sizeof(data->x_Dialog));                                                                                                   
	data->x_SuccTriggerEvent =                       this->task.x_SuccTriggerEvent;                                                 
	data->x_VerifyStateLayer =                       this->task.x_VerifyStateLayer;                                                 
	data->x_VerifyStateLayerID =                     this->task.x_VerifyStateLayerID;                                               
	data->x_KeyWordIndex =                           this->task.x_KeyWordIndex;   
	memcpy(data->x_TaskEventChar,					 this->task.x_TaskEventChar, sizeof(data->x_TaskEventChar));                                                                                                  
	data->x_TaskEventType =                          this->task.x_TaskEventType;                                                    
	data->x_TaskEventTime =                          this->task.x_TaskEventTime;                                                    
	data->x_TaskEventMonsterCount =                  this->task.x_TaskEventMonsterCount;                                            
	data->x_TaskEventMineCount =                     this->task.x_TaskEventMineCount;                                               
	data->x_TaskEventTaskCount =                     this->task.x_TaskEventTaskCount;                                               
	data->x_FactionPersonScore =                     this->task.x_FactionPersonScore;                                               
	data->x_TMTinyGameType =                         this->task.x_TMTinyGameType;                                                   
	data->x_m_ulGold2Wanted =                        this->task.x_m_ulGold2Wanted;                                                  
	data->x_BarrageChannel =                         this->task.x_BarrageChannel;                                                                                                        
	data->m_ulAwardType_S =                          this->task.m_ulAwardType_S;                                                    
	data->m_ulAwardType_F =                          this->task.m_ulAwardType_F;                                                    
	data->m_ulParent =                               this->task.m_ulParent;                                                         
	data->m_ulPrevSibling =                          this->task.m_ulPrevSibling;                                                    
	data->m_ulNextSibling =                          this->task.m_ulNextSibling;                                                    
	data->m_ulFirstChild = 	                         this->task.m_ulFirstChild; 	                                                 
	data->m_fLibraryTasksProbability =               this->task.m_fLibraryTasksProbability;                                         
	data->m_iWorldContribution =                     this->task.m_iWorldContribution;  
	memcpy(data->x_CombinedSwitches,				 this->task.x_CombinedSwitches, sizeof(data->x_CombinedSwitches));                                                                     
	data->x_SpecialParametersCount =                 this->task.x_SpecialParametersCount;
}

void TaskPreloader::AwardPreload(AWARD_DATA * data, FILE* fp)
{
	dbg_fread(&this->award, sizeof(this->award), 1, fp);
	
		data->m_ulGoldNum =                       this->award.m_ulGoldNum;
		data->m_ulExp =                           this->award.m_ulExp;
		data->m_ulRealmExp =                      this->award.m_ulRealmExp;
		data->m_bExpandRealmLevelMax =            this->award.m_bExpandRealmLevelMax;
		data->NewTaskSpace =                      this->award.NewTaskSpace;
		data->m_ulNewTask =                       this->award.m_ulNewTask;
		data->m_ulSP =                            this->award.m_ulSP;
		data->m_lReputation =                     this->award.m_lReputation;
		data->m_ulNewPeriod =                     this->award.m_ulNewPeriod;
		data->m_ulNewRelayStation =               this->award.m_ulNewRelayStation;
		data->m_ulStorehouseSize =                this->award.m_ulStorehouseSize;
		data->m_ulStorehouseSize2 =               this->award.m_ulStorehouseSize2;
		data->m_ulStorehouseSize3 =               this->award.m_ulStorehouseSize3;
		data->m_ulStorehouseSize4 =               this->award.m_ulStorehouseSize4;
		data->m_lInventorySize =                  this->award.m_lInventorySize;
		data->m_ulPetInventorySize =              this->award.m_ulPetInventorySize;
		data->m_ulFuryULimit =                    this->award.m_ulFuryULimit;
		data->m_ulTransWldId =                    this->award.m_ulTransWldId;
		data->x_CrossServerID =                   this->award.x_CrossServerID;
		data->m_TransPt =                         this->award.m_TransPt;
		data->m_lMonsCtrl =                       this->award.m_lMonsCtrl;
		data->m_bTrigCtrl =                       this->award.m_bTrigCtrl;
		data->m_bUseLevCo =                       this->award.m_bUseLevCo;
		data->m_bDivorce =                        this->award.m_bDivorce;
		data->m_bSendMsg =                        this->award.m_bSendMsg;
		data->m_nMsgChannel =                     this->award.m_nMsgChannel;
		data->m_ulCandItems =                     this->award.m_ulCandItems;
		data->m_ulSummonedMonsters =              this->award.m_ulSummonedMonsters;
		data->m_bAwardDeath =                     this->award.m_bAwardDeath;
		data->m_bAwardDeathWithLoss =             this->award.m_bAwardDeathWithLoss;
		data->m_ulDividend =                      this->award.m_ulDividend;
		data->m_bAwardSkill =                     this->award.m_bAwardSkill;
		data->m_iAwardSkillID =                   this->award.m_iAwardSkillID;
		data->m_iAwardSkillLevel =                this->award.m_iAwardSkillLevel;
		data->m_iSoloTowerChallengeScore =        this->award.m_iSoloTowerChallengeScore;
		data->AwardOpenSystem =                   this->award.AwardOpenSystem;
		data->m_ulSpecifyContribTaskID =          this->award.m_ulSpecifyContribTaskID;
		data->m_ulSpecifyContribSubTaskID =       this->award.m_ulSpecifyContribSubTaskID;
		data->m_ulSpecifyContrib =                this->award.m_ulSpecifyContrib;
		data->m_ulContrib =                       this->award.m_ulContrib;
		data->m_ulRandContrib =                   this->award.m_ulRandContrib;
		data->m_ulLowestcontrib =                 this->award.m_ulLowestcontrib;
		data->m_iFactionContrib =                 this->award.m_iFactionContrib;
		data->m_iFactionExpContrib =              this->award.m_iFactionExpContrib;
		data->m_ulPQRankingAwardCnt =             this->award.m_ulPQRankingAwardCnt;
		data->m_ulChangeKeyCnt =                  this->award.m_ulChangeKeyCnt;
		data->m_ulPersonChangeCnt =               this->award.m_ulPersonChangeCnt;
		data->m_ulNewChangeCnt =                  this->award.m_ulNewChangeCnt;
		data->m_ulHistoryChangeCnt =              this->award.m_ulHistoryChangeCnt;
		data->m_bMulti =                          this->award.m_bMulti;
		data->m_nNumType =                        this->award.m_nNumType;
		data->m_lNum =                            this->award.m_lNum;
		data->m_ulDisplayKeyCnt =                 this->award.m_ulDisplayKeyCnt;
		data->m_ulExpCnt =                        this->award.m_ulExpCnt;
		data->m_ulTaskCharCnt =                   this->award.m_ulTaskCharCnt;
		data->m_iForceContribution =              this->award.m_iForceContribution;
		data->m_iForceReputation =                this->award.m_iForceReputation;
		data->m_iForceActivity =                  this->award.m_iForceActivity;
		data->m_iForceSetRepu =                   this->award.m_iForceSetRepu;
		data->m_iTaskLimit =                      this->award.m_iTaskLimit;
		data->m_ulTitleNum =                      this->award.m_ulTitleNum;
		data->m_iLeaderShip =                     this->award.m_iLeaderShip;
		data->m_iWorldContribution =              this->award.m_iWorldContribution;
		data->m_iHomeResource[5] =                this->award.m_iHomeResource[5];
		data->m_bCreateHome = 	                  this->award.m_bCreateHome;	
		data->AwardLeaveCarrier =                 this->award.AwardLeaveCarrier;
		data->AwardFactionMoney =                 this->award.AwardFactionMoney;
		data->AwardFactionPersonScore =           this->award.AwardFactionPersonScore;
		data->AwardFactionTarget =                this->award.AwardFactionTarget;
		data->SubmitRankScore =                   this->award.SubmitRankScore;
		data->submit_rank =                       this->award.submit_rank;
		data->AwardMentorStudent =                this->award.AwardMentorStudent;
		data->AwardCoupleIntimacy =               this->award.AwardCoupleIntimacy;
		data->AwardRedBookExp =                   this->award.AwardRedBookExp;          
		data->AwardCampScore =                    this->award.AwardCampScore;           
		data->AwardCrossServerMoney =             this->award.AwardCrossServerMoney;    
		data->InviterAward =                      this->award.InviterAward;             
		data->AwardFriendPoint =                  this->award.AwardFriendPoint;         
		data->InviterAwardCount =                 this->award.InviterAwardCount;        
		data->AwardPerceptionValue =              this->award.AwardPerceptionValue; 
	
}

void TaskPreloader::SpecPreload(SPECIAL_PARAMETERS * data, FILE* fp)
{
	dbg_fread(&this->spec, sizeof(this->spec), 1, fp);

	data->FailWhenGetOffCarrier =          this->spec.FailWhenGetOffCarrier;
	data->TimeWhenRideCarrier =            this->spec.TimeWhenRideCarrier;
	data->GetOffCarrierID =                this->spec.GetOffCarrierID; 
	data->RideCarrierID =                  this->spec.RideCarrierID;
	data->UseItemTimes =                   this->spec.UseItemTimes;
	data->UseItemCount =                   this->spec.UseItemCount;
	data->CompleteTaskTimes =              this->spec.CompleteTaskTimes;
	data->CompleteTaskCount =              this->spec.CompleteTaskCount;
	data->ByCompleteTask =                 this->spec.ByCompleteTask;
	data->Parameter9 =                     this->spec.Parameter9;
	// Anedota /*160+*/
	data->need_story_book_collection =     this->spec.need_story_book_collection;
	data->story_book_collection_count =    this->spec.story_book_collection_count;
	
}


