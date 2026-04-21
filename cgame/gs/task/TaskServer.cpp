#include "TaskServer.h"
#include "TaskTempl.h"
#include "TaskConst.h"

#ifdef LINUX
	#include "../template/elementdataman.h"
#else
	#include "elementdataman.h"
#endif

#include "TaskTemplMan.h"

#ifndef _TASK_CLIENT

static const int FIFTEEN_DAYS_SECONDS = 1296000;

// ���������ļ�
bool LoadTasksFromPack(elementdataman* pDataMan, const char* szPackPath, const char* szDynPackPath)
{
	GetTaskTemplMan()->Init(pDataMan);
	bool b = GetTaskTemplMan()->LoadTasksFromPack(szPackPath, true);
	if (!b) return false;
	b = GetTaskTemplMan()->InitStorageTask();
	if (!b) return false;
	return GetTaskTemplMan()->LoadDynTasksFromPack(szDynPackPath);
}

// �ͷ������ļ�
void ReleaseTaskTemplMan()
{
	GetTaskTemplMan()->Release();
}

/*
 *	������Ա����ʧ�ܵ���ȫ��ʧ��
 */
inline void OnTaskForceFail(TaskInterface* pTask, unsigned int ulTaskId, bool bNotifyMem)
{
	TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 1, "TeamForceFail");

	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	unsigned char i;

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pEntries[i];

		if (CurEntry.m_ID == ulTaskId || CurEntry.m_uCapTaskId == ulTaskId)
		{
			CurEntry.ClearSuccess();
			CurEntry.GetTempl()->OnSetFinished(pTask, pLst, &CurEntry, bNotifyMem);
			return;
		}
	}
}

inline void OnTaskForceSucc(TaskInterface* pTask, unsigned int ulTaskId)
{
	TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 1, "TeamForceSucc");

	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	unsigned char i;

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pEntries[i];

		if (CurEntry.m_ID == ulTaskId || CurEntry.m_uCapTaskId == ulTaskId)
		{
			CurEntry.GetTempl()->OnSetFinished(pTask, pLst, &CurEntry, false);
			return;
		}
	}
}

// ���齨��
void OnTeamSetup(TaskInterface* pTask)
{
	const int nMemNum = pTask->GetTeamMemberNum();
	task_team_member_info mi;
	int i;
	float pos[3];
	unsigned int ulWorldTag = pTask->GetPos(pos);

	for (i = 1; i < nMemNum; i++)
	{
		pTask->GetTeamMemberInfo(i, &mi);
		if (mi.m_ulWorldTag != ulWorldTag) continue;
		OnTeamAddMember(pTask, &mi);
	}
}

/*
 *	�����ӳ�����Ա�˳�
 */

bool _teammember_leave(TaskInterface* pTask)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	unsigned char i;

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pEntries[i];

		if (!CurEntry.IsSuccess())
			continue;

		const ATaskTempl* pCapTempl = CurEntry.GetCapOrSelf();
		const ATaskTempl* pTempl = CurEntry.GetTempl();

		if (!pCapTempl || !pCapTempl->m_bTeamwork())
			continue;

		if (pCapTempl->m_bDismAsSelfFail())
		{
			CurEntry.ClearSuccess();
			CurEntry.GetTempl()->OnSetFinished(pTask, pLst, &CurEntry);
			return true;
		}

		if (pTempl && pTempl->m_bMarriage())
		{
			CurEntry.ClearSuccess();
			pTempl->OnSetFinished(pTask, pLst, &CurEntry);
			return true;
		}	
	}

	return false;
}

void OnTeamMemberLeave(TaskInterface* pTask)
{
	while (_teammember_leave(pTask)) {}
}

void OnTeamDisband(TaskInterface* pTask)
{
	OnTeamMemberLeave(pTask);
}

void OnTeamTransferCaptain(TaskInterface* pTask)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	const ATaskTempl* pCap;
	int i, j;
	const int nMemNum = pTask->GetTeamMemberNum();

	for (i = 0; i < (int)pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pEntries[i];
		pCap = CurEntry.GetCapOrSelf();

		if (!pCap || !pCap->m_bTeamwork() || !pCap->m_bRcvByTeam())
			continue;

		OnTaskForceFail(pTask, pCap->m_ID, false); // ����ʧ��

		for (j = 1; j < nMemNum; j++) // �����ӳ�
		{
			TaskNotifyPlayer(
					pTask,
					pTask->GetTeamMemberId(j),
					pCap->m_ID,
					pCap->x_IDSpace,
					TASK_PLY_NOTIFY_FORCE_FAIL);
		}
		// �������ѽ������б���;����ɲ������ĳ����������Ҫ���±����б�
		bool bCleared = true;
		for (j = 0; j < (int)pLst->m_uTaskCount; j++)
		{
			ActiveTaskEntry& CurEntry = pEntries[j];
			
			if (CurEntry.m_ID == pCap->m_ID || CurEntry.m_uCapTaskId == pCap->m_ID)
			{
				bCleared = false;
				break;
			}
		}
		if (bCleared) i = -1;
	}
}

void OnTeamAddMember(TaskInterface* pTask, const task_team_member_info* pInfo)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	const ATaskTempl* pTempl;
	unsigned int ulTask;
	unsigned char i;

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pEntries[i];
		pTempl = CurEntry.GetTempl();
		if (!pTempl) continue;

		if (pTempl->m_bTeamwork() && pTempl->m_bSharedTask())
		{
			ulTask = pTempl->GetMemTaskByInfo(pInfo);
			if (!ulTask) continue;

			TaskNotifyPlayer(
				pTask,
				pInfo->m_ulId,
				pTempl->m_ID,
				pTempl->x_IDSpace,
				TASK_PLY_NOTIFY_NEW_MEM_TASK);
		}
	}
}

void OnTeamDismissMember(TaskInterface* pTask, unsigned int ulPlayerId)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	const ATaskTempl* pCap;
	int i,j;
	const int nMemNum = pTask->GetTeamMemberNum();

	for (i = 0; i < (int)pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pEntries[i];
		pCap = CurEntry.GetCapOrSelf();

		if (!pCap || !pCap->m_bTeamwork()) continue;

		if (pCap->HasAllTeamMemsWanted(pTask, false) != 0)
		{
			OnTaskForceFail(pTask, pCap->m_ID, false); // ����ʧ��

			for (j = 1; j < nMemNum; j++) // �����ӳ�
			{
				unsigned int ulId = pTask->GetTeamMemberId(j);
				if (ulId == ulPlayerId) continue;

				TaskNotifyPlayer(
					pTask,
					ulId,
					pCap->m_ID,
					pCap->x_IDSpace,
					TASK_PLY_NOTIFY_FORCE_FAIL);
			}
			// �������ѽ������б���;����ɲ������ĳ����������Ҫ���±����б�
			bool bCleared = true;
			for (j = 0; j < (int)pLst->m_uTaskCount; j++)
			{
				ActiveTaskEntry& CurEntry = pEntries[j];
				
				if (CurEntry.m_ID == pCap->m_ID || CurEntry.m_uCapTaskId == pCap->m_ID)
				{
					bCleared = false;
					break;
				}
			}
			if (bCleared) i = -1;
		}
	}
}

void OnTaskCheckAwardDirect(TaskInterface* pTask, unsigned int ulTaskId);
void OnTaskGiveUpOneTask(TaskInterface* pTask, unsigned int ulTaskId, bool bForce);
void OnTaskAutoDelv(TaskInterface* pTask, unsigned int ulTaskId);
void OnTaskReachSite(TaskInterface* pTask,unsigned int ulTaskId);
void OnTaskLeaveSite(TaskInterface* pTask,unsigned int ulTaskId);
void OnTaskProtectNPC(TaskInterface* pTask, unsigned int ulTaskId, TaskGlobalData* pGlobal);
bool OnTaskCheckTreasure(TaskInterface* pTask,unsigned int ulTaskId);
void OnTaskRequestTreasureIndex(TaskInterface* pTask,unsigned int ulTaskId, unsigned int ulSpace);
bool OnTaskCheckTransform(TaskInterface* pTask,unsigned int ulTaskId);
void OnTaskChooseAward(TaskInterface* pTask, unsigned int ulTaskId, int nChoice);
void OnTaskChooseSubTask(TaskInterface* pTask, unsigned int ulTaskId);
bool OnTaskCheckLevel(TaskInterface* pTask, unsigned int ulTaskId);
bool OnTaskCheckTMIconStateID(TaskInterface* pTask, unsigned int ulTaskId);
void OnTaskBuyTokenShopItem(TaskInterface* pTask,unsigned int ulTaskId);
void OnTaskFinishSpendingWorldContribution(TaskInterface* pTask, unsigned int ulTaskId, unsigned char choice);
void OnTaskGetLoginReward(TaskInterface* pTask);
bool OnTaskCheckPointsFinish(TaskInterface* pTask, unsigned int ulTaskId);

void OnPlayerNotify(TaskInterface* pTask, unsigned int ulPlayerId, const void* pBuf, unsigned int sz)
{
	if (sz != sizeof(task_player_notify)) return;
	const task_player_notify* pNotify = static_cast<const task_player_notify*>(pBuf);
	const ATaskTempl* pTempl;

	char log[1024];
	sprintf(log, "PlayerNotify: Sender = %d, Reason = %d", ulPlayerId, pNotify->reason);
	TaskInterface::WriteLog(pTask->GetPlayerId(), 0, 1, log);

	switch (pNotify->reason)
	{
	case TASK_PLY_NOTIFY_NEW_MEM_TASK:
		if (!pTask->IsInTeam()) break;
		pTempl = GetTaskTemplMan()->GetTopTaskByID(pNotify->task);
		if (!pTempl) break;
		if (pNotify->param
		 || pTempl->CheckGlobalRequired(
			pTask,
			0,
			NULL,
			NULL,
			TASK_GLOBAL_CHECK_ADD_MEM))
		{
			if (!pTempl->OnDeliverTeamMemTask(pTask, NULL)
			 && pNotify->param)
			{
				// ֪ͨ�ӳ�ʧ��
				TaskNotifyPlayer(
					pTask,
					pTask->GetTeamMemberId(0),
					pTempl->m_ID,
					pTempl->x_IDSpace,
					TASK_PLY_NOTIFY_FORCE_FAIL,
					pNotify->param
					);
			}
		}
		break;
	case TASK_PLY_NOTIFY_FORCE_FAIL:
		OnTaskForceFail(pTask, pNotify->task, pNotify->param != 0);
		break;
	case TASK_PLY_NOTIFY_FORCE_SUCC:
		OnTaskForceSucc(pTask, pNotify->task);
		break;
	default:
		sprintf(log, "Player Notify, Sender = %d, Reason = %d", ulPlayerId, pNotify->reason);
		TaskInterface::WriteLog(pTask->GetPlayerId(), 0, 0, log);
	}
}

void OnClientNotify(TaskInterface* pTask, const void* pBuf, unsigned int sz)
{
	if (sz != sizeof(task_notify_base) && sz != sizeof(task_notify_choose_award)) return;
	const task_notify_base* pNotify = static_cast<const task_notify_base*>(pBuf);

	char log[1024];
	sprintf(log, "Clt: Reason = %d", pNotify->reason);
	TaskInterface::WriteLog(pTask->GetPlayerId(), pNotify->task, 1, log);

	switch (pNotify->reason)
	{
	case TASK_CLT_NOTIFY_CHECK_FINISH:
		OnTaskCheckAwardDirect(pTask, pNotify->task);
		break;
	case TASK_CLT_NOTIFY_CHECK_GIVEUP:
		OnTaskGiveUpOneTask(pTask, pNotify->task, false);
		break;
	case TASK_CLT_NOTIFY_AUTO_DELV:
		OnTaskAutoDelv(pTask, pNotify->task);
		break;
	case TASK_CLT_NOTIFY_REACH_SITE:
		OnTaskReachSite(pTask, pNotify->task);
		break;
	case TASK_CLT_NOTIFY_LEAVE_SITE:
		OnTaskLeaveSite(pTask, pNotify->task);
		break;
	case TASK_CLT_NOTIFY_DYN_TIMEMARK:
		GetTaskTemplMan()->OnTaskGetDynTasksTimeMark(pTask);
		break;
	case TASK_CLT_NOTIFY_SPECIAL_AWARD:
		GetTaskTemplMan()->OnTaskGetSpecialAward(pTask);
		break;
	case TASK_CLT_NOTIFY_DYN_DATA:
		GetTaskTemplMan()->OnTaskGetDynTasksData(pTask);
		break;
	case TASK_CLT_NOTIFY_STORAGE:
		GetTaskTemplMan()->OnTaskUpdateStorage(pTask, pTask->GetCurTime());
		break;
	case TASK_CLT_NOTIFY_PQ_CHECK_INIT:
		OnTaskCheckPQEnterWorldInit(pTask, pNotify->task);
		break;
	case TASK_CLT_NOTIFY_REQUEST_TREASURE_INDEX:
		OnTaskRequestTreasureIndex(pTask,pNotify->task, pNotify->space);
		break;
	case TASK_CLT_NOTIFY_15DAYS_NOLOGIN:
		OnTaskGetLoginReward(pTask);
		break;
	case TASK_CLT_NOTIFY_SPECIAL_AWARD_MASK:
		OnTaskSpecialAwardMask(pTask,pNotify->task);
		break;
	case TASK_CLT_NOTIFY_TITLE_TASK:
		OnTaskTitle(pTask, pNotify->task);
		break;
	case TASK_CLT_NOTIFY_CHOOSE_AWARD:
		{
			task_notify_choose_award* pCmd = (task_notify_choose_award*)pNotify;
			OnTaskChooseAward(pTask, pCmd->task, pCmd->choice);
		}
		break;
	case TASK_CLT_NOTIFY_BUY_TOKENSHOP_ITEM:
		OnTaskBuyTokenShopItem(pTask, pNotify->task);
		break;
	case TASK_CLT_NOTIFY_FINISH_TASK_BY_WORLD_CONTRIBUTION: 
		{
			task_notify_choose_award* pCmd = (task_notify_choose_award*)pNotify;
			OnTaskFinishSpendingWorldContribution(pTask, pNotify->task, pCmd->choice);
		}
		break;
	default:
		break;
	}
}

void OnTaskProtectNPC(TaskInterface* pTask, unsigned int ulTaskId, TaskGlobalData* pGlobal)
{
	/*
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTaskId);
	if (!pEntry || !pEntry->m_ulTemplAddr || pEntry->IsFinished()) return;

	if (!pGlobal)
	{
		TaskPreservedData tpd;
		tpd.m_ulSrcTask = ulTaskId;
		tpd.reason = TASK_GLOBAL_PROTECT_NPC;

		TaskQueryGlobalData(
			ulTaskId,
			pTask->GetPlayerId(),
			&tpd,
			sizeof(TaskPreservedData)
			);

		return;
	}

	const ATaskTempl* pTempl = pEntry->GetTempl();
	if (pTempl->m_enumMethod != enumTMProtectNPC) return;

	unsigned int ulCurTime = TaskInterface::GetCurTime();
	unsigned int ulLimit = pEntry->m_ulTaskTime + pTempl->m_ulProtectTimeLen;

	// NPC�Ƿ��������ڼ䱻ɱ
	if (pGlobal->m_ulKillTime >= pEntry->m_ulTaskTime
	 && pGlobal->m_ulKillTime <= ulLimit)
	{
		pEntry->ClearSuccess();
		pTempl->OnSetFinished(
			pTask,
			pLst,
			pEntry);
	}
	else if (ulCurTime > ulLimit) // �ɹ�
	{
		pTempl->OnSetFinished(
			pTask,
			pLst,
			pEntry);
	}
	*/
}

void OnTaskAutoDelv(TaskInterface* pTask, unsigned int ulTaskId)
{
	const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(ulTaskId);
	if (!pTempl || !pTempl->m_bAutoDeliver()) return;

	if (pTempl->CheckGlobalRequired(
		pTask,
		0,
		NULL,
		NULL,
		TASK_GLOBAL_CHECK_RCV_NUM))
		pTempl->CheckDeliverTask(pTask, 0, NULL);
}

void OnTaskReachSite(TaskInterface* pTask,unsigned int ulTaskId)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTaskId);
	if (!pEntry || pEntry->IsFinished()) return;

	const ATaskTempl* pTaskTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskId);
	if (!pTaskTempl || pTaskTempl->m_enumMethod != enumTMReachSite || pTaskTempl->m_enumFinishType != enumTFTDirect) return;

	float pos[3];
	unsigned int ulWorldId = pTask->GetPos(pos);

	if (ulWorldId != pTaskTempl->m_ulReachSiteId)
		return;
	
	unsigned int i;
	for (i=0;i<pTaskTempl->m_ulReachSiteCnt;i++)
	{
		const Task_Region& t = pTaskTempl->m_pReachSite[i];
		if (is_in_zone(t.zvMin,t.zvMax,pos))
			break;
	}
	if(i>=pTaskTempl->m_ulReachSiteCnt)
		return;

	pTaskTempl->OnSetFinished(
		pTask,
		pLst,
		pEntry);
}

void OnTaskLeaveSite(TaskInterface* pTask,unsigned int ulTaskId)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTaskId);
	if (!pEntry || pEntry->IsFinished()) return;

	const ATaskTempl* pTaskTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskId);
	if (!pTaskTempl || pTaskTempl->m_enumMethod != enumTMLeaveSite || pTaskTempl->m_enumFinishType != enumTFTDirect) return;

	float pos[3];
	unsigned int ulWorldId = pTask->GetPos(pos);

	unsigned int i;
	if (ulWorldId == pTaskTempl->m_ulLeaveSiteId)
	{
		for (i=0;i<pTaskTempl->m_ulLeaveSiteCnt;i++)
		{
			const Task_Region& t = pTaskTempl->m_pLeaveSite[i];
			if (is_in_zone(t.zvMin,t.zvMax,pos))
				return;
		}
	}

	pTaskTempl->OnSetFinished(
		pTask,
		pLst,
		pEntry);
}

bool OnTaskCheckDeliver(TaskInterface* pTask, unsigned int ulTaskId,unsigned int ulStorageId, bool bNotifyErr)
{
	char log[1024];
	sprintf(log, "TaskCheckDeliverAtNPC, Storage id = %d", ulStorageId);
	TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 1, log);
	
	if (ulStorageId == 0 && GetTaskTemplMan()->IsStorageTask(ulTaskId))
	{
		TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 0, "TaskCheckDeliver: wrong storage id");
		return false;
	}

	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());

	const ATaskTempl* pTaskTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskId);
	if (!pTaskTempl) return false;

	if (pTaskTempl->m_pParent) pTaskTempl = pTaskTempl->m_pParent;
	else ulTaskId = 0;

	if (ulStorageId > TASK_STORAGE_COUNT)
		return false;

	StorageTaskList* pStorageLst = static_cast<StorageTaskList*>(pTask->GetStorageTaskList());
	
	if (ulStorageId)
	{
		unsigned int uId = pTaskTempl->GetID();
		unsigned short* arr = pStorageLst->m_Storages[ulStorageId-1];
		int i;
		
		for (i = 0; i < TASK_STORAGE_LEN; i++)
			if (arr[i] == uId)
				break;
			
		if (i == TASK_STORAGE_LEN)
		{
			TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 0, "TaskCheckDeliver: Not In Cur Storage List");
			return false;
		}

		if (!pTask->GetCommonItemCount(GetTaskTemplMan()->GetStoragePremItemID(ulStorageId)))
		{
			TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 0, "TaskCheckDeliver: No Prem Storage Item");
			return false;
		}
	}

	if (pTaskTempl->CheckGlobalRequired(
		pTask,
		ulTaskId,
		NULL,
		NULL,
		TASK_GLOBAL_CHECK_RCV_NUM))
		return pTaskTempl->CheckDeliverTask(pTask, ulTaskId, NULL, bNotifyErr) == 0;

	return true;
}

void OnTaskManualTrig(TaskInterface* pTask, unsigned int ulTask, bool bNotifyErr)
{
	// ֻ�в�ͨ��npc����������ֶ�����
	const ATaskTempl* pTaskTempl = GetTaskTemplMan()->GetTopTaskByID(ulTask);
	if (!pTaskTempl || pTaskTempl->m_ulDelvNPC) return;

	OnTaskCheckDeliver(pTask, ulTask, 0, bNotifyErr);
}
void OnTaskBuyTokenShopItem(TaskInterface* pTask, unsigned int ulTask)
{
	const ATaskTempl* pTaskTempl = GetTaskTemplMan()->GetTopTaskByID(ulTask);
	if (!pTaskTempl || !pTaskTempl->m_bUsedInTokenShop()) return;
	
	OnTaskCheckDeliver(pTask, ulTask,0);
}
void OnTaskFinishSpendingWorldContribution(TaskInterface* pTask, unsigned int ulTaskId, unsigned char choice)
{
	const ATaskTempl* pTaskTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskId);
	if (!pTaskTempl) return;
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTaskId);
	const ATaskTempl* pTopTask = pTaskTempl->GetTopTask();
	bool bTopTask = pTopTask == pTaskTempl;
	ActiveTaskEntry* pTopEntry = pLst->GetEntry(pTopTask->m_ID);

	if (   !pEntry 
		|| !pTopEntry
		|| pTaskTempl->m_pFirstChild != NULL   // ���׶�����ֻ�����ڵײ�����
		|| pTopTask->CanDeliverWorldContribution(pTask) // �������׶ȵ������������Ĺ��׶������
		|| pEntry->IsFinished() // ����ɵ���������ʹ�ù��׶����
		|| pTopTask->m_iWorldContribution == 0 // ��Ϊ���������Ĺ��׶ȿ���Ϊ0���ö��������m_iWorldContribution���ֹ��׶��������ͨ����
		|| pTaskTempl->m_iWorldContribution > pTask->GetWorldContribution() 
		|| pTaskTempl->m_bPQTask() || pTaskTempl->m_bPQSubTask()
		|| (!pTask->PlayerCanSpendContributionAsWill() && pTask->GetWorldContributionSpend() +  pTaskTempl->m_iWorldContribution > TASK_WORLD_CONTRIBUTION_SPEND_PER_DAY)) 
		return;
	// �۳����׶�
	pTask->TakeAwayWorldContribution(pTaskTempl->m_iWorldContribution);
	
	// ���ñ�Ƿ�ֹʹ�ù��׶���ɵ������ٽ������׶�
	pTopEntry->SetContributionFinish();
	pEntry->SetFinished();
	// ֪ͨ�ͻ��������
	pTaskTempl->NotifyClient(
		pTask,
		pEntry,
		TASK_SVR_NOTIFY_FINISHED,
		0);

	TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 1, "ContributionFinished");
	pTaskTempl->DeliverAward(pTask, pLst, pEntry, choice);
}
void OnTaskRequestTreasureIndex(TaskInterface* pTask,unsigned int ulTaskId, unsigned int ulSpace)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTaskId);
	if (!pEntry || pEntry->IsFinished()) return;
	
	unsigned char buf[20];
	svr_treasure_map* pNotify = reinterpret_cast<svr_treasure_map*>(buf);
	unsigned int sz = sizeof(svr_treasure_map);

	pNotify->reason = TASK_SVR_NOTIFY_TREASURE_MAP;
	pNotify->space = static_cast<unsigned short>(ulSpace);
	pNotify->task = static_cast<unsigned short>(ulTaskId);
	pNotify->treasure_index = pEntry->m_iUsefulData1;
	
	pTask->NotifyClient(buf,sz);
}
bool OnTaskCheckTreasure(TaskInterface* pTask,unsigned int ulTaskId)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTaskId);
	if (!pEntry) return false;

	const ATaskTempl* pTaskTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskId);
	if (!pTaskTempl || pTaskTempl->m_enumMethod != enumTMReachTreasureZone 
		|| (pTaskTempl->m_enumFinishType != enumTFTDirect && pTaskTempl->m_enumFinishType != enumTFTConfirm)) return false;

	float pos[3];
	unsigned int ulWorldId = pTask->GetPos(pos);

	if (ulWorldId != 1)
		return false;

	unsigned short uTreasureLocIndex = pEntry->m_iUsefulData1 & 0xFFFF;
	float fTreasureLocX = pTaskTempl->m_TreasureStartZone.x + (uTreasureLocIndex % pTaskTempl->m_ucZonesNumX - 1) * pTaskTempl->m_ucZoneSide;
	float fTreasureLocZ = pTaskTempl->m_TreasureStartZone.z + (uTreasureLocIndex / pTaskTempl->m_ucZonesNumX) * pTaskTempl->m_ucZoneSide;

	bool bPass = pos[0] >= fTreasureLocX && pos[2] >= fTreasureLocZ && pos[0] <= fTreasureLocX + pTaskTempl->m_ucZoneSide && pos[2] <= fTreasureLocZ + pTaskTempl->m_ucZoneSide;
	
	return bPass;
	
}
bool OnTaskCheckTransform(TaskInterface* pTask,unsigned int ulTaskId)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTaskId);
	if (!pEntry) return false;

	const ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskId);
	if (!pTempl || pTempl->m_enumMethod != enumTMTransform) return false;

	int playerShapeType = pTask->GetShapeMask();
	bool bPass = false;

	// �����ְҵ����
	if (pTempl->m_ucTransformedForm == 0x80)
	{
		if ((playerShapeType >> 6) == 2)
		{
			bPass = true;
		}
	}
	// �ض�����
	else if (pTempl->m_ucTransformedForm == playerShapeType)
	{
		bPass = true;
	}

	return bPass;
}
void OnNPCDeliverTaskItem(TaskInterface* pTask, unsigned int ulNPCId, unsigned int ulTaskId)
{
	DATA_TYPE dt;
	elementdataman* pMan = GetTaskTemplMan()->GetEleDataMan();

	NPC_ESSENCE* pNPC = (NPC_ESSENCE*)pMan->get_data_ptr(
		ulNPCId,
		ID_SPACE_ESSENCE,
		dt
		);

	if (!pNPC || dt != DT_NPC_ESSENCE)
		return;

	if (!pNPC->id_task_matter_service) return;

	NPC_TASK_MATTER_SERVICE* pSvr = (NPC_TASK_MATTER_SERVICE*)pMan->get_data_ptr(
		pNPC->id_task_matter_service,
		ID_SPACE_ESSENCE,
		dt
		);

	if (!pSvr || dt != DT_NPC_TASK_MATTER_SERVICE)
		return;

	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	const int sz = sizeof(pSvr->tasks) / sizeof(pSvr->tasks[0]);

	int i, j;
	for (i = 0; i < sz; i++)
	{
		unsigned int id = pSvr->tasks[i].id_task;

		if (!id) break;
		if (ulTaskId != id || !pLst->GetEntry(id)) continue;

		const int matter_num = sizeof(pSvr->tasks[0].taks_matters) / sizeof(pSvr->tasks[0].taks_matters[0]);
		int actual_num = 0;

		for (j = 0; j < matter_num; j++)
		{
			unsigned int matter = pSvr->tasks[i].taks_matters[j].id_matter;
			if (!matter) break;
			actual_num++;
		}

		if (actual_num == 0 || !pTask->CanDeliverTaskItem(actual_num)) break;

		for (j = 0; j < actual_num; j++)
		{
			unsigned int matter = pSvr->tasks[i].taks_matters[j].id_matter;
			int matter_num = pSvr->tasks[i].taks_matters[j].num_matter;

			if (pTask->GetTaskItemCount(matter)) continue;
			pTask->DeliverTaskItem(matter, matter_num);
		}

		break;
	}
}

void OnTaskCheckAward(TaskInterface* pTask, unsigned int ulTaskId, int nChoice)
{
	ActiveTaskList* pList = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;
	unsigned int ulCurTime = pTask->GetCurTime();

	for (unsigned char i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];

		if (CurEntry.m_ID != ulTaskId
		|| !CurEntry.m_ulTemplAddr)
			continue;

		const ATaskTempl* pTempl = CurEntry.GetTempl();

		if (pTempl->m_enumFinishType != enumTFTNPC)
		{
			TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 0, "SvrCheckAward Not By NPC");
			return;
		}

		if (pTempl->m_bMarriage())
		{
			if (pTask->IsCaptain() && pTempl->CheckMarriage(pTask) == 0)
				pTask->CheckTeamRelationship(TASK_TEAM_RELATION_MARRIAGE);

			return;
		}

		const ATaskTempl* pTopTask = pTempl->GetTopTask();
		ActiveTaskEntry* pParentEntry = pList->GetEntry(pTopTask->m_ID);
		if (!pParentEntry)
			return;
		if (pParentEntry->IsContributionFinish()) 
		{
			pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
			return;
		}

		if (pTempl->m_enumMethod == enumTMCollectNumArticle)
			pTempl->CheckCollectItem(pTask, pList, &CurEntry, true, nChoice);
		else if (pTempl->m_enumMethod == enumTMKillNumMonster)
			pTempl->CheckMonsterKilled(pTask, pList, &CurEntry, true, nChoice);
		else if (pTempl->m_enumMethod == enumTMWaitTime || pTempl->m_enumMethod > 28)
			pTempl->CheckWaitTime(pTask, pList, &CurEntry, ulCurTime, true, nChoice);
		else if (pTempl->m_enumMethod == enumTMTalkToNPC)
		{
			CurEntry.SetFinished();
			pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
		}
		else if (pTempl->m_enumMethod == enumTMGlobalValOK)
		{
			if (pTempl->CheckGlobalKeyValue(pTask, true) == 0)  // PQ sub task can also use this method instead of CheckGlobalPQKeyValue()
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
			}			
		}
		else if (pTempl->m_enumMethod == enumTMTransform)
		{
			if (OnTaskCheckTransform(pTask, ulTaskId))
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
			}
		}
		else if ((pTempl->m_enumMethod == enumTMSimpleClientTask) || (pTempl->m_enumMethod == enumTMSimpleClientTaskForceNavi))
		{
			CurEntry.SetFinished();
			pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
		}
		else if (pTempl->m_enumMethod == enumTMReachLevel)
		{
			if (OnTaskCheckLevel(pTask, ulTaskId))
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
			}			
		}
		else if (pTempl->m_enumMethod == enumTMHasIconStateID)
		{
			if (OnTaskCheckTMIconStateID(pTask, ulTaskId))
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
			}
		}
		else if (pTempl->m_enumMethod == enumTMCompleteTaskEvent) // Novo Evento
		{
			if (OnTaskCheckPointsFinish(pTask, ulTaskId))
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
			}
		}
		else if (CurEntry.IsFinished())
			pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
		else
			TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 0, "CheckAwardAtNPC: Task Not Finished");

		return;
	}
}

void OnTaskCheckAwardDirect(TaskInterface* pTask, unsigned int ulTaskId)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;
	unsigned int ulCurTime = pTask->GetCurTime();

	TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 1, "SvrCheckAwardDirect");

	for (unsigned char i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];

		if (CurEntry.m_ID != ulTaskId
		|| !CurEntry.m_ulTemplAddr)
			continue;

		const ATaskTempl* pTempl = CurEntry.GetTempl();

		// PQ����ʱ����һ��
		if (pTempl->m_bPQSubTask())
		{
			if(!pTempl->m_pParent || CurEntry.m_ParentIndex == 0xff)
				return;
			
			ActiveTaskEntry& ParentEntry = aEntries[CurEntry.m_ParentIndex];
			
			if(pTempl->m_pNextSibling)	// ��������Ż���
			{
				if(ParentEntry.m_ulTaskTime != PublicQuestInterface::GetCurTaskStamp(ParentEntry.GetTempl()->m_ID))
				{
					CurEntry.ClearSuccess();
					CurEntry.GetTempl()->OnSetFinished(pTask, pList, &CurEntry);
					return;
				}
				else	// ʱ��һ�£���ǰ�ѽ�PQ���������һ�׶�Ϊ��������PQ������ʱ���Ż����
				{
					if(pTempl->m_pNextSibling->m_ID != PublicQuestInterface::GetCurSubTask(ParentEntry.GetTempl()->m_ID) &&
					   pTempl->m_ID != PublicQuestInterface::GetCurSubTask(ParentEntry.GetTempl()->m_ID))
					{
						CurEntry.ClearSuccess();
						CurEntry.GetTempl()->OnSetFinished(pTask, pList, &CurEntry);
						return;
					}
				}
			}
		}

		const ATaskTempl* pTopTask = pTempl->GetTopTask();
		ActiveTaskEntry* pParentEntry = pList->GetEntry(pTopTask->m_ID);
		if (!pParentEntry)
			return;
		if (pParentEntry->IsContributionFinish()) 
		{
			pTempl->DeliverAward(pTask, pList, &CurEntry, -1);
			return;
		}

		if (CurEntry.IsFinished() && !CurEntry.IsSuccess()) // ʧ��
		{
			pTempl->DeliverAward(pTask, pList, &CurEntry, -1);
			return;
		}

		if (pTempl->m_ulTimeLimit
		 && CurEntry.m_ulTaskTime + pTempl->m_ulTimeLimit < ulCurTime) // ��ʱ
		{
			CurEntry.ClearSuccess();
			CurEntry.GetTempl()->OnSetFinished(pTask, pList, &CurEntry);
			return;
		}

		// ��������ʧЧ����
		if (pTempl->m_bAbsFail)
		{
			time_t t64 = ulCurTime;
			tm cur = *localtime(&t64);
			ulCurTime = t64;
			
			if(pTempl->m_tmAbsFailTime.before(&cur))
			{
				CurEntry.ClearSuccess();
				CurEntry.GetTempl()->OnSetFinished(pTask, pList, &CurEntry);
				return;	
			}
		}

		// ����or�뿪��������ʧ��	
		float pos[3];
		unsigned int ulWorldId = pTask->GetPos(pos);
		unsigned int iRegion;

		if (pTempl->m_bEnterRegionFail() && ulWorldId == pTempl->m_ulEnterRegionWorld)
		{
			for (iRegion = 0;iRegion<pTempl->m_ulEnterRegionCnt;iRegion++)
			{
				const Task_Region& t = pTempl->m_pEnterRegion[iRegion];
				if (is_in_zone(t.zvMin, t.zvMax, pos))
				{
					CurEntry.ClearSuccess();
					CurEntry.GetTempl()->OnSetFinished(pTask, pList, &CurEntry);
					return;
				}
			}		
		}
		
		if (pTempl->m_bLeaveRegionFail())
		{
			bool bLeaveRegion = false;
			if (ulWorldId != pTempl->m_ulLeaveRegionWorld)	bLeaveRegion = true;			
			else
			{
				for (iRegion = 0;iRegion<pTempl->m_ulLeaveRegionCnt;iRegion++)
				{
					const Task_Region& t = pTempl->m_pLeaveRegion[iRegion];
					if (is_in_zone(t.zvMin, t.zvMax, pos))
						break;
				}
				if (iRegion >= pTempl->m_ulLeaveRegionCnt)	bLeaveRegion = true;
			}
			if (bLeaveRegion)
			{
				CurEntry.ClearSuccess();
				CurEntry.GetTempl()->OnSetFinished(pTask, pList, &CurEntry);
				return;
			}
		}

		if (!pTask->IsAtCrossServer() && pTempl->m_bLeaveFactionFail() && !pTask->IsInFaction(1))
		{
			CurEntry.ClearSuccess();
			CurEntry.GetTempl()->OnSetFinished(pTask, pList, &CurEntry);
			return;
		}

		if (pTempl->m_enumFinishType != enumTFTDirect && pTempl->m_enumFinishType != enumTFTConfirm)
		{
			if (!pTempl->m_ulTimeLimit)
				TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 0, "SvrCheckAwardDirect Not Direct or Confirm");

			return;
		}

		// ��������ֻ����npc�����
		if (pTempl->m_bMarriage())
			return;

		if (pTempl->m_enumMethod == enumTMCollectNumArticle)
			pTempl->CheckCollectItem(pTask, pList, &CurEntry, false, -1);
		else if (pTempl->m_enumMethod == enumTMKillNumMonster)
			pTempl->CheckMonsterKilled(pTask, pList, &CurEntry, false, -1);
		else if (pTempl->m_enumMethod == enumTMWaitTime || pTempl->m_enumMethod > 28)
			pTempl->CheckWaitTime(pTask, pList, &CurEntry, ulCurTime, false, -1);
		else if (pTempl->m_enumMethod == enumTMGlobalValOK)
		{
			if (pTempl->CheckGlobalKeyValue(pTask, true) == 0)
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, -1);
			}
		}
		else if (pTempl->m_enumMethod == enumTMTransform)
		{
			if (OnTaskCheckTransform(pTask, ulTaskId))
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, -1);
			}
		}
		else if (pTempl->m_enumMethod == enumTMReachLevel)
		{
			if (OnTaskCheckLevel(pTask, ulTaskId))
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, -1);
			}			
		}
		else if (pTempl->m_enumMethod == enumTMReachTreasureZone)
		{
			if (OnTaskCheckTreasure(pTask, ulTaskId))
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, -1);
			}
		}
		else if ((pTempl->m_enumMethod == enumTMSimpleClientTask) || (pTempl->m_enumMethod == enumTMSimpleClientTaskForceNavi)) // �����񣬷���������֤��ֻ�ͻ��˼��
		{
			CurEntry.SetFinished();
			pTempl->DeliverAward(pTask, pList, &CurEntry, -1);
		}		
		else if (pTempl->m_enumMethod == enumTMHasIconStateID)
		{
			if (OnTaskCheckTMIconStateID(pTask, ulTaskId))
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, -1);
			}
		}
		else if (pTempl->m_enumMethod == enumTMCompleteTaskEvent) // Novo Evento
		{
			if (OnTaskCheckPointsFinish(pTask, ulTaskId))
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, -1);
			}
		}
		else if (CurEntry.IsFinished())
			pTempl->DeliverAward(pTask, pList, &CurEntry, -1);
		else if (!pTempl->m_ulTimeLimit)
			TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 0, "SvrCheckAward Wrong Method");

		return;
	}
}

inline void _on_kill_monster(
	TaskInterface* pTask,
	unsigned int ulTemplId,
	unsigned int ulLev,
	bool bTeam,
	float fRand,
	int dps, 
	int dph)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;

	for (int i = 0; i < (int)pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];

		const ATaskTempl* pTempl = CurEntry.GetTempl();

		if (!pTempl)
			continue;

		pTempl->CheckKillMonster(
			pTask,
			pList,
			&CurEntry,
			ulTemplId,
			ulLev,
			bTeam,
			fRand,
			dps,
			dph);

		// �������ѽ������б���;����ɲ������ĳ����������Ҫ���±����б�
		bool bCleared = pTempl->m_ID != (unsigned int)CurEntry.m_ID;
		if (bCleared) i = -1;
	}
}

void OnTaskKillMonster(TaskInterface* pTask, unsigned int ulTemplId, unsigned int ulLev, float fRand,int dps, int dph)
{
	_on_kill_monster(pTask, ulTemplId, ulLev, false,fRand,dps,dph);
}

void OnTaskTeamKillMonster(TaskInterface* pTask, unsigned int ulTemplId, unsigned int ulLev, float fRand)
{
	_on_kill_monster(pTask, ulTemplId, ulLev, true, fRand,0,0);
}

void OnTaskMining(TaskInterface* pTask, unsigned int ulTaskId)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* pEntry = pList->GetEntry(ulTaskId);

	if (!pEntry || !pEntry->GetTempl()) return;
	pEntry->GetTempl()->CheckMining(pTask, pList, pEntry);
}

bool _on_player_killed(TaskInterface* pTask)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;

	for (unsigned char i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];

		if (!CurEntry.IsSuccess())
			continue;

		const ATaskTempl* pTempl = CurEntry.GetTempl();

		if (pTempl && pTempl->m_bFailAsPlayerDie())
		{
			CurEntry.ClearSuccess();
			pTempl->OnSetFinished(pTask, pList, &CurEntry);			
			return true;
		}
	}

	return false;
}

void OnTaskPlayerKilled(TaskInterface* pTask)
{
	while (_on_player_killed(pTask)) {}
	GetTaskTemplMan()->CheckDeathTrig(pTask);
}

void OnTaskGiveUpOneTask(TaskInterface* pTask, unsigned int ulTaskId, bool bForce)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());

	for (unsigned char i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pList->m_TaskEntries[i];
		if (!CurEntry.m_ulTemplAddr) continue;
		const ATaskTempl* pTempl = CurEntry.GetTempl();
		if (!pTempl || pTempl->m_ID != ulTaskId) continue;

		// ��ʱ�����������Ƿ���ɣ�����ɵ����������ʱ��ǰ�ص�NPC
		pTempl->GiveUpOneTask(pTask, pList, &CurEntry, bForce);
		return;
	}
}

void OnTaskCheckAllTimeLimits(unsigned int ulCurTime)
{
	GetTaskTemplMan()->OnTaskCheckAllTimeLimits(ulCurTime);
}

void OnTaskReceivedGlobalData(
	TaskInterface* pTask,
	unsigned int ulTaskId,
	unsigned char pData[TASK_GLOBAL_DATA_SIZE],
	const void* pPreservedData,
	unsigned int size)
{
	const TaskPreservedData* pPreserve = (const TaskPreservedData*)pPreservedData;
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	TaskGlobalData* pGlobal = (TaskGlobalData*)pData;
	unsigned short reason = pPreserve->reason;

	switch (reason)
	{
	case TASK_GLOBAL_NPC_KILLED_TIME:
		return;
	case TASK_GLOBAL_PROTECT_NPC:
		OnTaskProtectNPC(pTask, ulTaskId, pGlobal);
		return;
	}

	const ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID(pPreserve->m_ulSrcTask);

	if (!pTempl)
		return;

	pGlobal->CheckRcvUpdateTime(pTask->GetCurTime(), pTempl->m_lAvailFrequency);

	switch (reason)
	{
	case TASK_GLOBAL_CHECK_RCV_NUM:
	case TASK_GLOBAL_CHECK_COTASK:
		if (pTempl->CheckGlobalRequired(pTask, pPreserve->m_ulSubTaskId, pPreserve, pGlobal, reason))
			pTempl->CheckDeliverTask(pTask,	pPreserve->m_ulSubTaskId, pGlobal);
		else
		{
			pTempl->NotifyClient(
				pTask,
				NULL,
				TASK_SVR_NOTIFY_ERROR_CODE,
				0,
				TASK_PREREQU_FAIL_MAX_RCV);
		}
		break;
	case TASK_GLOBAL_CHECK_ADD_MEM:
		if (pTempl->CheckGlobalRequired(pTask, 0, pPreserve, pGlobal, TASK_GLOBAL_CHECK_ADD_MEM))
			pTempl->OnDeliverTeamMemTask(pTask, pGlobal);
		break;
	case TASK_GLOBAL_ADD_TIME_MARK:
		break;
	case TASK_GLOBAL_DEL_TIME_MARK:
		break;
	}
}

void OnForgetLivingSkill(TaskInterface* pTask)
{
	GetTaskTemplMan()->OnForgetLivingSkill(pTask);
}

void OnTaskPlayerLevelUp(TaskInterface* pTask)
{
}

// PQ�����ʼ��
bool InitPublicQuestSystem()
{
	const ATaskTempl* pTempl;
	TaskTemplMap& PQTemplMap = GetTaskTemplMan()->GetPQTemplMap();
	TaskTemplMap::iterator it = PQTemplMap.begin();
	bool bInitSucc = true;

	for(; it != PQTemplMap.end(); ++it)
	{
		pTempl = it->second;
		
		if(!pTempl || !(pTempl->m_pFirstChild))
		{	
			printf("InitPublicQuestSystem(): cannot find task or task has no child. task id = %d", (pTempl) ? pTempl->m_ID : -1);
			bInitSucc = false;
			continue;
		}

		int global_value[TASK_AWARD_MAX_DISPLAY_EXP_CNT], i;
		for(i=0;i<pTempl->m_ulPQExpCnt; i++)
		{
			global_value[i] = atoi(pTempl->m_pszPQExp[i]);
		}

		if(!PublicQuestInterface::InitAddQuest(pTempl->GetID(), pTempl->m_pFirstChild->GetID(), global_value, pTempl->m_ulPQExpCnt))
		{
			printf("InitPublicQuestSystem(): InitAddQuest() error, task id = %d.", pTempl->GetID());
			bInitSucc = false;
			continue;
		}

		for(i=0;i<pTempl->m_pFirstChild->m_ulPQExpCnt; i++)
		{
			global_value[i] = atoi(pTempl->m_pFirstChild->m_pszPQExp[i]);
		}		

		// �����ճ�ʼ����PQ����
		PublicQuestInterface::QuestSetStart(pTempl->GetID(), global_value, pTempl->m_pFirstChild->m_ulPQExpCnt,pTempl->m_pFirstChild->IsNotChangePQRanking());

		// �������������
		if(pTempl->m_pFirstChild->m_lMonsCtrl)
			PublicQuestInterface::SetMonsterController(pTempl->m_pFirstChild->m_lMonsCtrl, pTempl->m_pFirstChild->m_bTrigCtrl());
	}

	return bInitSucc;
}

// PQ�����heartbeat
void OnTaskPublicQuestHeartbeat()
{
	const ATaskTempl* pTempl;
	TaskTemplMap& PQTemplMap = GetTaskTemplMan()->GetPQTemplMap();
	TaskTemplMap::iterator it = PQTemplMap.begin();

	for(; it != PQTemplMap.end(); ++it)
	{
		pTempl = it->second;
		
		if(!pTempl)
			return;

		int iSubTaskId = PublicQuestInterface::GetCurSubTask(pTempl->m_ID);
		const ATaskTempl* pSubTempl = GetTaskTemplMan()->GetTaskTemplByID(iSubTaskId);

		if(!pSubTempl)
			return;

		// �ж�PQ����ĵ�ǰ�������Ƿ����
		if(pSubTempl->CheckGlobalPQKeyValue(true) == 0)
		{
			// ���ӵ�ǰ�������׶�(�̶�&���)
			PublicQuestInterface::QuestSetRandContrib(pTempl->m_ID, pSubTempl->m_Award_S->m_ulContrib, pSubTempl->m_Award_S->m_ulRandContrib, pSubTempl->m_Award_S->m_ulLowestcontrib);

			// �����������������
			if(pSubTempl->m_Award_S->m_lMonsCtrl)
				PublicQuestInterface::SetMonsterController(pSubTempl->m_Award_S->m_lMonsCtrl, pSubTempl->m_Award_S->m_bTrigCtrl);


			int global_value[TASK_AWARD_MAX_DISPLAY_EXP_CNT], i;
			if(pSubTempl->m_pNextSibling)
			{
				for(i=0;i<pSubTempl->m_pNextSibling->m_ulPQExpCnt; i++)
				{
					global_value[i] = atoi(pSubTempl->m_pNextSibling->m_pszPQExp[i]);
				}

				PublicQuestInterface::QuestSetNextChildTask(pTempl->m_ID, pSubTempl->m_pNextSibling->m_ID, global_value, pSubTempl->m_pNextSibling->m_ulPQExpCnt,pSubTempl->m_pNextSibling->IsNotChangePQRanking());

				// �������������
				if(pSubTempl->m_pNextSibling->m_lMonsCtrl)
					PublicQuestInterface::SetMonsterController(pSubTempl->m_pNextSibling->m_lMonsCtrl, pSubTempl->m_pNextSibling->m_bTrigCtrl());

			}
			else	// ��PQ�����Ѿ���ɣ���������һ���µ�
			{				
				PublicQuestInterface::QuestSetFinish(pTempl->m_ID);

				for(i=0;i<pTempl->m_pFirstChild->m_ulPQExpCnt; i++)
				{
					global_value[i] = atoi(pTempl->m_pFirstChild->m_pszPQExp[i]);
				}

				PublicQuestInterface::QuestSetStart(pTempl->m_ID, global_value, pTempl->m_pFirstChild->m_ulPQExpCnt,pTempl->m_pFirstChild->IsNotChangePQRanking());

				// �������������
				if(pTempl->m_pFirstChild->m_lMonsCtrl)
					PublicQuestInterface::SetMonsterController(pTempl->m_pFirstChild->m_lMonsCtrl, pTempl->m_pFirstChild->m_bTrigCtrl());

			}
		}
	}
}

// PQ����ɱ�ֹ��׶ȷ���
/*
	ulTemplId:		��ɱ�ֵ�ID
	bOwner:			�Ƿ�Ϊ��������������ģʽʱ���ò������ã�
	fTeamContrib:	���鹱�ס������Ҳ��ڶ����У���Ϊ���˹��ף�����ģʽʱ���ò������ã�
	iTeamMemberCnt:	�����ж�Ա�����������Ҳ��ڶ����У���Ϊ1��
*/
void OnTaskKillPQMonster(TaskInterface* pTask, unsigned int ulTemplId, bool bOwner,float fTeamContrib,int iTeamMemberCnt,float fPersonalContib)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;

	for(unsigned char i=0;i<pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];

		if (!CurEntry.m_ulTemplAddr)
			continue;

		const ATaskTempl* pTempl = CurEntry.GetTempl();

		// ��ΪPQ��������������ʱ����뵱ǰPQ����һ�£�������й��׶ȷ���
		if(pTempl->m_bPQSubTask())
		{
			// ���ʱ����Ƿ�һ��
			if (CurEntry.m_ParentIndex != 0xff)
			{
				ActiveTaskEntry& ParentEntry = aEntries[CurEntry.m_ParentIndex];

				if(ParentEntry.m_ulTaskTime != PublicQuestInterface::GetCurTaskStamp(pTempl->m_pParent->m_ID))
					return;
			}

			for (unsigned char j=0; j<pTempl->m_ulMonsterContribCnt; j++)
			{
				const MONSTERS_CONTRIB& MonstersContrib = pTempl->m_MonstersContrib[j];
				
				if(ulTemplId == MonstersContrib.m_ulMonsterTemplId)
				{
					int iTotalContrib = 0;

					// ��Ӽ�������׶�
					if(bOwner)	
						iTotalContrib += MonstersContrib.m_iWholeContrib/iTeamMemberCnt;
					
					// ��Ӽ�������׶�
					iTotalContrib += (int)((MonstersContrib.m_iShareContrib * fTeamContrib)/iTeamMemberCnt + 0.5f);

					// ���˶������׶�
					iTotalContrib += (int)(MonstersContrib.m_iPersonalWholeContrib * fPersonalContib+0.5f);

					PublicQuestInterface::QuestUpdatePlayerContrib(pTempl->m_pParent->m_ID, pTask->GetPlayerId(), iTotalContrib);
					return;
				}
			}

			return;
		}
	}
}

// PQ�����ʼ��link��Ϣ
void OnTaskCheckPQEnterWorldInit(TaskInterface* pTask, unsigned int ulTaskId)
{
	pTask->PQEnterWorldInit(ulTaskId);
}

void OnTaskLeaveForce(TaskInterface* pTask)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());	
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;

	for(int i=0;i<(int)pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];

		if (!CurEntry.m_ulTemplAddr)
			continue;

		const ATaskTempl* pTempl = CurEntry.GetTempl();

		if (pTempl->m_bLeaveForceFail())
		{
			CurEntry.ClearSuccess();
			pTempl->OnSetFinished(pTask,pList,&CurEntry);

			// �������ѽ������б���;����ɲ������ĳ����������Ҫ���±����б�
			bool bCleared = pTempl->m_ID != (unsigned int)CurEntry.m_ID;
			if (bCleared) i = -1;
		}
	}
}

void OnTaskExternEvent(TaskInterface* pTask, int Event)
{
	switch (Event)
	{
	case EX_TK_SENDAUMAIL_LEVEL1:
		OnTaskManualTrig(pTask,INVITE_BACKGAME_LEVEL_1_REWARDING_TASK_ID);
		break;
	case EX_TK_SENDAUMAIL_LEVEL2:
		OnTaskManualTrig(pTask,INVITE_BACKGAME_LEVEL_2_REWARDING_TASK_ID);
		break;
	case EX_TK_SENDAUMAIL_LEVEL3:
		OnTaskManualTrig(pTask,INVITE_BACKGAME_LEVEL_3_REWARDING_TASK_ID);
		break;
	case EX_TK_SENDAUMAIL_EXAWARD:
		OnTaskManualTrig(pTask,26190);
		break;
	case EX_TK_WELCOMBACK_REWARD:
		OnTaskManualTrig(pTask,BACKGAME_INVITEE_MAIL_REWARDING_TASK_ID);
		break;
	}
}

// ǧ�ﴫ�����߽���
void OnTaskGetLoginReward(TaskInterface* pTask)
{
	if (pTask->GetCurTime() - pTask->GetRoleLastLoginTime() >= FIFTEEN_DAYS_SECONDS)
	{
		OnTaskManualTrig(pTask,29099);
	}
}

void OnTaskSpecialAwardMask(TaskInterface* pTask,unsigned int ulTaskID)
{
	special_award sa;
	pTask->GetSpecailAwardInfo(&sa);
	for (int i = 0;i < NUM_SPECIAL_AWARD;i++)
	{
		if ((sa.special_mask & (1 << i)) && TASK_SPECIAL_AWARD[i] == ulTaskID)
		{
			OnTaskManualTrig(pTask,TASK_SPECIAL_AWARD[i]);
		}
	}
}

void OnTaskKillPlayer(TaskInterface* pTask, int iOccupation, bool bGender, int iLevel, int iForce, float fRand)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;
	
	for (int i = 0; i < (int)pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];	
		const ATaskTempl* pTempl = CurEntry.GetTempl();
		if (!pTempl)
			continue;
		
		pTempl->CheckKillPlayer(pTask,pList,&CurEntry,iOccupation,iLevel,bGender,iForce,fRand);
		// �������ѽ������б���;����ɲ������ĳ����������Ҫ���±����б�
		bool bCleared = pTempl->m_ID != (unsigned int)CurEntry.m_ID;
		if (bCleared) i = -1;
	}
}

void OnTaskTitle(TaskInterface* pTask, unsigned int ulTaskID)
{
	const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(ulTaskID);
	if (!pTempl || !pTempl->m_bDisplayInTitleTaskUI()) return;
	
	if (pTempl->CheckGlobalRequired(
		pTask,
		0,
		NULL,
		NULL,
		TASK_GLOBAL_CHECK_RCV_NUM))
		pTempl->CheckDeliverTask(pTask, 0, NULL);
}

void OnTaskChooseAward(TaskInterface* pTask, unsigned int ulTaskId, int nChoice)
{
	ActiveTaskList* pList = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;
	unsigned int ulCurTime = pTask->GetCurTime();
	
	for (unsigned char i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];
		
		if (CurEntry.m_ID != ulTaskId
			|| !CurEntry.m_ulTemplAddr)
			continue;
		
		const ATaskTempl* pTempl = CurEntry.GetTempl();
		
		if (pTempl->m_enumFinishType != enumTFTConfirm || pTempl->m_Award_S->m_ulCandItems <= 1)
		{
			TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 0, "SvrCheckChooseAward Invalid");
			return;
		}

		const ATaskTempl* pTopTask = pTempl->GetTopTask();
		ActiveTaskEntry* pParentEntry = pList->GetEntry(pTopTask->m_ID);
		if (!pParentEntry)
			return;
		if (pParentEntry->IsContributionFinish()) 
		{
			pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
			return;
		}
		
		if (pTempl->m_enumMethod == enumTMCollectNumArticle)
			pTempl->CheckCollectItem(pTask, pList, &CurEntry, true, nChoice);
		else if (pTempl->m_enumMethod == enumTMKillNumMonster)
			pTempl->CheckMonsterKilled(pTask, pList, &CurEntry, true, nChoice);
		else if (pTempl->m_enumMethod == enumTMWaitTime || pTempl->m_enumMethod > 28)
			pTempl->CheckWaitTime(pTask, pList, &CurEntry, ulCurTime, true, nChoice);
		else if (pTempl->m_enumMethod == enumTMTalkToNPC)
		{
			CurEntry.SetFinished();
			pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
		}
		else if (pTempl->m_enumMethod == enumTMGlobalValOK)
		{
			if (pTempl->CheckGlobalKeyValue(pTask, true) == 0)
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
			}			
		}
		else if (pTempl->m_enumMethod == enumTMTransform)
		{
			if (OnTaskCheckTransform(pTask, ulTaskId))
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
			}
		}
		else if (pTempl->m_enumMethod == enumTMReachLevel)
		{
			if (OnTaskCheckLevel(pTask, ulTaskId))
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
			}			
		}		
		else if (pTempl->m_enumMethod == enumTMHasIconStateID)
		{
			if (OnTaskCheckTMIconStateID(pTask, ulTaskId))
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
			}
		}
		else if (pTempl->m_enumMethod == enumTMCompleteTaskEvent) // Novo Evento
		{
			if (OnTaskCheckPointsFinish(pTask, ulTaskId))
			{
				CurEntry.SetFinished();
				pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
			}
		}
		else if (CurEntry.IsFinished())
			pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
		else
			TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 0, "CheckChooseAward: Task Not Finished");
		
		return;
	}
}

bool OnTaskCheckLevel(TaskInterface* pTask, unsigned int ulTaskId)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTaskId);
	if (!pEntry) return false;
	
	const ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskId);
	if (!pTempl || pTempl->m_enumMethod != enumTMReachLevel) return false;
	
	return pTempl->CheckReachLevel(pTask);
}

bool OnTaskCheckTMIconStateID(TaskInterface* pTask, unsigned int ulTaskId)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTaskId);
	if (!pEntry) return false;
	
	const ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskId);
	if (!pTempl || pTempl->m_enumMethod != enumTMHasIconStateID) return false;
	
	return pTempl->CheckTMIconStateID(pTask);
}

void ClearAllTowerTask(TaskInterface* pTask)
{
	ActiveTaskList* pList = (ActiveTaskList*)pTask->GetActiveTaskList();
	for(unsigned char i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pList->m_TaskEntries[i];
		if (!CurEntry.m_ulTemplAddr)
		{
			continue;
		}
		const ATaskTempl* pTempl = CurEntry.GetTempl();
		if (!pTempl->m_pParent && pTempl->m_bTowerTask())
		{
			// �����Ƿ��ܷ�������ǿ�Ʒ���
			CurEntry.ClearSuccess();
			CurEntry.SetGiveUp();
			pTempl->OnSetFinished(pTask, pList, &CurEntry);
			TaskInterface::WriteLog(pTask->GetPlayerId(), pTempl->m_ID, 1, "GiveUpTask");
			TaskInterface::WriteKeyLog(pTask->GetPlayerId(), pTempl->m_ID, 1, "GiveUpTask");
		}
	}
}

// Nova missão de evento

bool OnTaskCheckPointsFinish(TaskInterface* pTask, unsigned int ulTaskId)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTaskId);
	if (!pEntry) return false;
	
	const ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskId);
	if (!pTempl || pTempl->m_enumMethod != enumTMCompleteTaskEvent) return false;
	
	return pTempl->CheckAllPointsEventTask(pTask);
}

#endif
