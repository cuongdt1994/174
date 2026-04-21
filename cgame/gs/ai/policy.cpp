#include "policy.h"
#include <memory.h>

#define CONDITION_TREE_NULL 0
#define CONDITION_LEFT_SUB  1
#define CONDITION_RIGHT_SUB 2
#define CONDITION_LEAF      3
#define CONDITION_NODE_END  4

//----------------------------------------------------------------------------------------
//CTriggerData
//----------------------------------------------------------------------------------------

void CTriggerData::AddOperaion(unsigned int iType, void *pParam, _s_target *pTarget)
{
	_s_operation* pNew = new _s_operation;
	pNew->iType = iType;
	pNew->pParam = pParam;
	pNew->mTarget.iType = pTarget->iType;
	pNew->mTarget.pParam = pTarget->pParam;
	listOperation.push_back(pNew);
}

void CTriggerData::AddOperaion(_s_operation*pOperation)
{
	if(pOperation==0) return;
	listOperation.push_back(pOperation);
}

int	CTriggerData::GetOperaion(unsigned int idx, unsigned int &iType, void **ppData, _s_target& target)
{
	if(idx> listOperation.size()) return -1;
	iType = listOperation[idx]->iType;
	*ppData = listOperation[idx]->pParam;
	target = listOperation[idx]->mTarget;
	return iType;
}

void CTriggerData::SetOperation(unsigned int idx, unsigned int iType, void *pData, _s_target *pTarget)
{
	if(idx > listOperation.size()) return;
	listOperation[idx]->iType = iType;
	listOperation[idx]->pParam = pData;
	listOperation[idx]->mTarget = *pTarget;
}

void CTriggerData::DelOperation(unsigned int idx)
{
	if(idx > listOperation.size()) return;
	FreeOperation(listOperation[idx]);
	listOperation.erase(&listOperation[idx]);
}

CTriggerData::_s_tree_item *CTriggerData::GetConditonRoot()
{
	return rootConditon;
}

unsigned int CTriggerData::GetConditionParamSize(int condition)
{
	unsigned int result(0);
	switch(condition) 
	{
	case c_time_come:
		result = sizeof(C_TIME_COME);
		break;
	case c_hp_less:
		result = sizeof(C_HP_LESS);
		break;
	case c_random:
		result = sizeof(C_RANDOM);
		break;
	case c_var:
		result = sizeof(C_VAR);
		break;
	case c_constant:
		result = sizeof(C_CONSTANT);
		break;
	case c_be_hurt:
		result = sizeof(C_BE_HURT);
		break;
	case c_reach_end:
		result = sizeof(C_REACH_END);
		break;			
	case c_at_history_stage:
		result = sizeof(C_HISTORY_STAGE);
		break;
	case c_history_value:
		result = sizeof(C_HISTORY_VALUE);
		break;
	case c_local_var:
		result = sizeof(C_LOCAL_VAR);
		break;
	case c_reach_end_2:
		result = sizeof(C_REACH_END_2);
		break;
	case c_has_filter:
		result = sizeof(C_HAS_FILTER);
		break;
	case c_player_count_in_radius:
		result = sizeof(C_PLAYER_COUNT_IN_RADIUS);
		break;
	case c_player_count_in_region:
		result = sizeof(C_PLAYER_COUNT_IN_REGION);
		break;

	}
	return result;
}

bool CTriggerData::SaveConditonTree(FILE *pFile, _s_tree_item *pNode)
{
	if(pNode==0) return true;
	int dat_size = 0;
	int flag = 0;
	fwrite(&pNode->mConditon.iType,sizeof(int),1,pFile);
	if(pNode->mConditon.pParam==0)
		fwrite(&dat_size,sizeof(int),1,pFile);
	else
	{
		dat_size = GetConditionParamSize(pNode->mConditon.iType);
		fwrite(&dat_size,sizeof(int),1,pFile);
		fwrite(pNode->mConditon.pParam,dat_size,1,pFile);
	}

	if(pNode->pLeft == 0 && pNode->pRight == 0)
	{
		flag = CONDITION_LEAF;
		fwrite(&flag,sizeof(int),1,pFile);
		return true;
	}

	if(pNode->pLeft) 
	{
		flag = CONDITION_LEFT_SUB;
		fwrite(&flag,sizeof(int),1,pFile);
		SaveConditonTree(pFile,pNode->pLeft);
	}

	if(pNode->pRight)
	{
		flag = CONDITION_RIGHT_SUB;
		fwrite(&flag,sizeof(int),1,pFile);
		SaveConditonTree(pFile,pNode->pRight);
	}
	
	flag = CONDITION_NODE_END;
	fwrite(&flag,sizeof(int),1,pFile);

	return true;
}

bool CTriggerData::ReadConditonTree(FILE *pFile, _s_tree_item *pNode)
{
	if(pNode==0) return true;
	int dat_size = 0;
	int flag = 0;
	fread(&pNode->mConditon.iType,sizeof(int),1,pFile);
	fread(&dat_size,sizeof(int),1,pFile);
	if(dat_size!=0) 
	{
		pNode->mConditon.pParam = malloc(dat_size); 
		fread(pNode->mConditon.pParam,dat_size,1,pFile);
	}else pNode->mConditon.pParam= 0;
	
	while(1)
	{
		fread(&flag,sizeof(int),1,pFile);
		if(flag==CONDITION_LEAF)
		{
			pNode->pLeft = 0;
			pNode->pRight = 0;
			break;
		}else if(flag==CONDITION_LEFT_SUB)
		{
			pNode->pLeft = new _s_tree_item;
			ReadConditonTree(pFile, pNode->pLeft);
		}else if(flag==CONDITION_RIGHT_SUB)
		{
			pNode->pRight = new _s_tree_item;
			ReadConditonTree(pFile,pNode->pRight);
		}else if(flag==CONDITION_NODE_END) break;
	}
	return true;	
}


bool CTriggerData::Save(FILE *pFile)
{
	unsigned int dwVersion = F_TRIGGER_VERSION;
	fwrite(&dwVersion,sizeof(unsigned int),1,pFile);
	fwrite(&uID,sizeof(unsigned int),1,pFile);
	fwrite(&bActive,sizeof(bool),1,pFile);
	fwrite(&bRun,sizeof(bool),1,pFile);
	fwrite(&bAttackValid,sizeof(bool),1,pFile);
	fwrite(szName,sizeof(char)*128,1,pFile);
	
	int n;
	//Writting conditon data(tree)
	
	SaveConditonTree(pFile,rootConditon);

	//Writting operation data
	n = listOperation.size();
	fwrite(&n,sizeof(int),1,pFile);
	for(int i = 0; i < n; ++i)
	{
		_s_operation *pTemp = (_s_operation* )listOperation[i];
		fwrite(&pTemp->iType,sizeof(int),1,pFile);
		WriteOperationParam(pTemp, pFile);
		
		fwrite(&pTemp->mTarget.iType,sizeof(int),1,pFile);
		WriteOperationTarget(pTemp, pFile);
	}

	return true;
}

bool CTriggerData::Load(FILE *pFile)
{
	unsigned int dwVersion;
	fread(&dwVersion,sizeof(unsigned int),1,pFile);
	if(dwVersion==0)
	{
		
		fread(&uID,sizeof(unsigned int),1,pFile);
		fread(&bActive,sizeof(bool),1,pFile);
		fread(&bRun,sizeof(bool),1,pFile);
		fread(&bAttackValid,sizeof(bool),1,pFile);
		fread(szName,sizeof(char)*128,1,pFile);
		int n = 0;
		
		//Reading conditon data
		rootConditon = new _s_tree_item;
		ReadConditonTree(pFile,rootConditon);
		
		//Reading operation data
		fread(&n,sizeof(int),1,pFile);
		for(int i = 0; i < n; ++i)
		{
			_s_operation *pTemp = new _s_operation;
			fread(&pTemp->iType,sizeof(int),1,pFile);
			switch(pTemp->iType) 
			{
			case o_attact:
			case o_use_skill:
			case o_talk:
			case o_run_trigger:
			case o_stop_trigger:
			case o_active_trigger:
			case o_create_timer:
			case o_kill_timer:
			case o_active_controller:
		
				ReadOperationParam(pTemp, pFile, dwVersion);
				break;			
			default:
				pTemp->pParam = 0;
				break;
			}
			
			fread(&pTemp->mTarget.iType,sizeof(int),1,pFile);
			ReadOperationTarget(pTemp, pFile);
			
			listOperation.push_back(pTemp);
		}
	}else if(dwVersion==1)
	{
		fread(&uID,sizeof(unsigned int),1,pFile);
		fread(&bActive,sizeof(bool),1,pFile);
		fread(&bRun,sizeof(bool),1,pFile);
		fread(&bAttackValid,sizeof(bool),1,pFile);
		fread(szName,sizeof(char)*128,1,pFile);
		int n = 0;
		
		//Reading conditon data
		rootConditon = new _s_tree_item;
		ReadConditonTree(pFile,rootConditon);
		
		//Reading operation data
		fread(&n,sizeof(int),1,pFile);
		for(int i = 0; i < n; ++i)
		{
			_s_operation *pTemp = new _s_operation;
			fread(&pTemp->iType,sizeof(int),1,pFile);
			switch(pTemp->iType) 
			{
			case o_attact:
			case o_use_skill:
			case o_talk:
			case o_run_trigger:
			case o_stop_trigger:
			case o_active_trigger:
			case o_create_timer:
			case o_kill_timer:
			case o_active_controller:

				ReadOperationParam(pTemp, pFile, dwVersion);
				break;
			default:
				pTemp->pParam = 0;
				break;
			}
			
			fread(&pTemp->mTarget.iType,sizeof(int),1,pFile);
			ReadOperationTarget(pTemp, pFile);
			
			listOperation.push_back(pTemp);
		}
	}else if(dwVersion==2)
	{
		fread(&uID,sizeof(unsigned int),1,pFile);
		fread(&bActive,sizeof(bool),1,pFile);
		fread(&bRun,sizeof(bool),1,pFile);
		fread(&bAttackValid,sizeof(bool),1,pFile);
		fread(szName,sizeof(char)*128,1,pFile);
		int n = 0;
		
		//Reading conditon data
		rootConditon = new _s_tree_item;
		ReadConditonTree(pFile,rootConditon);
		
		//Reading operation data
		fread(&n,sizeof(int),1,pFile);
		for(int i = 0; i < n; ++i)
		{
			_s_operation *pTemp = new _s_operation;
			fread(&pTemp->iType,sizeof(int),1,pFile);
			switch(pTemp->iType) 
			{
			case o_attact:
			case o_use_skill:
			case o_talk:
			case o_run_trigger:
			case o_stop_trigger:
			case o_active_trigger:
			case o_create_timer:
			case o_kill_timer:
			case o_active_controller:
			case o_set_global:
			case o_revise_global:
		
				ReadOperationParam(pTemp, pFile, dwVersion);
				break;
			default:
				pTemp->pParam = 0;
				break;
			}
			
			fread(&pTemp->mTarget.iType,sizeof(int),1,pFile);
			ReadOperationTarget(pTemp, pFile);
			
			listOperation.push_back(pTemp);
		}
	}else if(dwVersion==3)
	{
		fread(&uID,sizeof(unsigned int),1,pFile);
		fread(&bActive,sizeof(bool),1,pFile);
		fread(&bRun,sizeof(bool),1,pFile);
		fread(&bAttackValid,sizeof(bool),1,pFile);
		fread(szName,sizeof(char)*128,1,pFile);
		int n = 0;
		
		//Reading conditon data
		rootConditon = new _s_tree_item;
		ReadConditonTree(pFile,rootConditon);
		
		//Reading operation data
		fread(&n,sizeof(int),1,pFile);
		for(int i = 0; i < n; ++i)
		{
			_s_operation *pTemp = new _s_operation;
			fread(&pTemp->iType,sizeof(int),1,pFile);
			switch(pTemp->iType) 
			{
			case o_attact:
			case o_use_skill:
			case o_talk:
			case o_run_trigger:
			case o_stop_trigger:
			case o_active_trigger:
			case o_create_timer:
			case o_kill_timer:
			case o_active_controller:
			case o_set_global:
			case o_revise_global:

				ReadOperationParam(pTemp, pFile, dwVersion);
				break;
			default:
				pTemp->pParam = 0;
				break;
			}
			
			fread(&pTemp->mTarget.iType,sizeof(int),1,pFile);
			ReadOperationTarget(pTemp, pFile);
			
			listOperation.push_back(pTemp);
		}
	}else if(dwVersion >= 4)
	{
		fread(&uID,sizeof(unsigned int),1,pFile);
		fread(&bActive,sizeof(bool),1,pFile);
		fread(&bRun,sizeof(bool),1,pFile);
		fread(&bAttackValid,sizeof(bool),1,pFile);
		fread(szName,sizeof(char)*128,1,pFile);
		int n = 0;
		
		//Reading conditon data
		rootConditon = new _s_tree_item;
		ReadConditonTree(pFile,rootConditon);
		
		//Reading operation data
		fread(&n,sizeof(int),1,pFile);
		for(int i = 0; i < n; ++i)
		{
			_s_operation *pTemp = new _s_operation;
			fread(&pTemp->iType,sizeof(int),1,pFile);
			ReadOperationParam(pTemp, pFile, dwVersion);
			
			fread(&pTemp->mTarget.iType,sizeof(int),1,pFile);
			ReadOperationTarget(pTemp, pFile);
			
			listOperation.push_back(pTemp);
		}
	}else return false;
	return true;
}

void CTriggerData::ReleaseConditionTree()
{
	if(rootConditon)
		delete rootConditon;
	rootConditon = 0;
}

void CTriggerData::Release()
{
	ReleaseConditionTree();

	int n = listOperation.size();
	for(int i = 0; i < n; ++i )
	{
		FreeOperation(listOperation[i]);
	}
	listOperation.clear();
}

void CTriggerData::SetName(const char *name)
{
	const int NAME_LEN = sizeof(szName)/sizeof(szName[0])-1;
	strncpy(szName, name, NAME_LEN);
	szName[NAME_LEN] = '\0';
}

CTriggerData *CTriggerData::CopyObject()
{
	CTriggerData *pNewObject = new CTriggerData;
	if(pNewObject==0) return 0;

	pNewObject->SetName(szName);
	pNewObject->bActive = bActive;
	pNewObject->bRun = bRun;
	pNewObject->bAttackValid = bAttackValid;
	pNewObject->uID = uID;
	pNewObject->rootConditon = CopyConditonTree(rootConditon);

	for( unsigned int i = 0; i < listOperation.size(); ++i)
	{
		_s_operation* pNewOperation = CopyOperation(listOperation[i]);
		pNewObject->AddOperaion(pNewOperation);
	}

	return pNewObject;
}

CTriggerData::_s_tree_item* CTriggerData::CopyConditonTree( CTriggerData::_s_tree_item* pRoot)
{
	if(pRoot==0) return 0;
	CTriggerData::_s_tree_item *pNew = new CTriggerData::_s_tree_item;
	if(pNew==0) return 0;
	
	pNew->mConditon.iType = pRoot->mConditon.iType;
	pNew->mConditon.pParam = 0;
	pNew->pLeft = 0;
	pNew->pRight = 0;
	CopyConditionParam(pNew, pRoot);

	if(pRoot->pLeft) pNew->pLeft = CTriggerData::CopyConditonTree(pRoot->pLeft);
	if(pRoot->pRight) pNew->pRight = CTriggerData::CopyConditonTree(pRoot->pRight);
	return pNew;
}

void CTriggerData::CopyConditionParam(_s_tree_item* pDst, const _s_tree_item* pSrc){
	if (pSrc->mConditon.pParam){
		unsigned int size = GetConditionParamSize(pSrc->mConditon.iType);
		pDst->mConditon.pParam = malloc(size);
		memcpy(pDst->mConditon.pParam, pSrc->mConditon.pParam, size);
	}else{
		pDst->mConditon.pParam = 0;
	}
}

void CTriggerData::ReadOperationParam(_s_operation *p, FILE *pFile, unsigned int dwVersion)
{
	if (unsigned int size = GetOperationParamSize(p->iType)){
		p->pParam = malloc(size);
		switch (p->iType)
		{
		case o_active_controller:
			if(dwVersion < 1){
				O_ACTIVE_CONTROLLER *pDst = (O_ACTIVE_CONTROLLER *)p->pParam;
				fread(&pDst->uID,sizeof(unsigned int),1,pFile);
				pDst->bStop = false;
				return;
			}
			break;			
		case o_set_global:
			if(dwVersion < 4){
				O_SET_GLOBAL *pDst = (O_SET_GLOBAL *)p->pParam;
				O_SET_GLOBAL_VERSION3 v;
				fread(&v,sizeof(O_SET_GLOBAL_VERSION3),1,pFile);
				pDst->iID = v.iID;
				pDst->iValue = v.iValue;
				pDst->bIsValue = true;
				return;
			}
			break;
		case o_talk:
			{
				O_TALK_TEXT *pDst = (O_TALK_TEXT *)p->pParam;
				fread(&pDst->uSize,sizeof(unsigned int),1,pFile);
				pDst->szData = new policy_char[pDst->uSize/2];
				fread(pDst->szData,pDst->uSize,1,pFile);
				if (dwVersion < 17){
					pDst->uAppendDataMask = 0;
				}else{
					fread(&pDst->uAppendDataMask,sizeof(pDst->uAppendDataMask),1,pFile);
				}
				return;
			}
			break;
		case o_summon_monster:
			if(dwVersion < 7){
				O_SUMMON_MONSTER_VERSION6 v;
				fread(&v,sizeof(O_SUMMON_MONSTER_VERSION6),1,pFile);
				O_SUMMON_MONSTER *pDst = (O_SUMMON_MONSTER *)p->pParam;
				pDst->iDispearCondition = 0;
				pDst->iLife = v.iLife;
				pDst->iMonsterID = v.iMonsterID;
				pDst->iMonsterNum = v.iMonsterNum;
				pDst->iPathID = v.iPathID;
				pDst->iRange = v.iRange;
				return;
			}
			break;
		case o_play_action:
			if(dwVersion < 9){
				O_PLAY_ACTION_VERSION8 v;
				fread(&v,sizeof(O_PLAY_ACTION_VERSION8),1,pFile);
				O_PLAY_ACTION *pDst = (O_PLAY_ACTION *)p->pParam;
				strcpy(pDst->szActionName,v.szActionName);
				pDst->iInterval = v.iInterval;
				pDst->iLoopCount = 1;
				pDst->iPlayTime = 5000;
				return;
			}
			if (dwVersion < 38) {
				O_PLAY_ACTION_VERSION37 v;
				fread(&v, sizeof(O_PLAY_ACTION_VERSION37), 1, pFile);
				O_PLAY_ACTION* pDst = (O_PLAY_ACTION*)p->pParam;
				memcpy(pDst, &v, sizeof(O_PLAY_ACTION_VERSION37));
				pDst->iUnknown = 0;
				return;
			}
			break;
		case o_summon_mine:
			if (dwVersion < 24)
			{
				O_SUMMON_MINE_VERSION_23 v;
				fread(&v, sizeof(O_SUMMON_MINE_VERSION_23), 1, pFile);
				O_SUMMON_MINE* pDst = (O_SUMMON_MINE*)p->pParam;
				memcpy(pDst, &v, sizeof(O_SUMMON_MINE_VERSION_23));
				pDst->bSetOwnerAsMonsterOwner = false;
				return;
			}
			break;
		case o_skill_with_talk:
			{
				O_SKILL_WITH_TALK* pDst = (O_SKILL_WITH_TALK*)p->pParam;

				fread(&pDst->skill.uSkill, sizeof(unsigned int), 1, pFile);
				fread(&pDst->skill.uSkillType, sizeof(unsigned int), 1, pFile);
				fread(&pDst->skill.uLevel, sizeof(unsigned int), 1, pFile);
				fread(&pDst->skill.uLevelType, sizeof(unsigned int), 1, pFile);

				fread(&pDst->talk.uSize, sizeof(unsigned int), 1, pFile);
				pDst->talk.szData = new policy_char[pDst->talk.uSize / 2];
				fread(pDst->talk.szData, pDst->talk.uSize, 1, pFile);
				if (dwVersion < 17) {
					pDst->talk.uAppendDataMask = 0;
				}
				else {
					fread(&pDst->talk.uAppendDataMask, sizeof(pDst->talk.uAppendDataMask), 1, pFile);
				}
				return;
			}
			break;
		case o_talk_2:
			{
				O_TALK_TEXT_2 *pDst = (O_TALK_TEXT_2 *)p->pParam;
				fread(&pDst->talk.uSize,sizeof(unsigned int),1,pFile);
				pDst->talk.szData = new policy_char[pDst->talk.uSize/2];
				fread(pDst->talk.szData,pDst->talk.uSize,1,pFile);
				if (dwVersion < 17) {
					pDst->talk.uAppendDataMask = 0;
				}else{
					fread(&pDst->talk.uAppendDataMask,sizeof(pDst->talk.uAppendDataMask),1,pFile);
				}
				fread(&pDst->iChannelKinds, sizeof(unsigned int), 1, pFile);
				fread(&pDst->iChannelKindsType, sizeof(unsigned int), 1, pFile);
				fread(&pDst->iChannelID, sizeof(unsigned int), 1, pFile);
				fread(&pDst->iChannelIDType, sizeof(unsigned int), 1, pFile);
				return;
			}
		case o_72:
			{
				int uSize = 0;
				fread(&uSize, 4u, 1u, pFile);
				auto szData = new policy_char[uSize / 2];
				fread(szData, uSize, 1u, pFile);
				if (dwVersion > 16) {
					int uAppendDataMask = 0;
					fread(&uAppendDataMask, 4u, 1u, pFile);
				}
				char zvMin[12];
				char zvMax[12];
				fread(zvMin, 0xCu, 1u, pFile);
				fread(zvMax, 0xCu, 1u, pFile);
				return;
			}
			break;
				
		}
		fread(p->pParam,size,1,pFile);
	}else{
		p->pParam = 0;
	}
}

void CTriggerData::WriteOperationParam(const _s_operation *p, FILE *pFile){
	if (unsigned int size = GetOperationParamSize(p->iType)){
		switch (p->iType)
		{
		case o_talk:
			{
				const O_TALK_TEXT *pText = (const O_TALK_TEXT *)p->pParam;
				fwrite(&pText->uSize,sizeof(unsigned int),1,pFile);
				fwrite(pText->szData,pText->uSize,1,pFile);
				fwrite(&pText->uAppendDataMask,sizeof(pText->uAppendDataMask),1,pFile);
				break;
			}
		default:
			fwrite(p->pParam,size,1,pFile);
			break;
		}
	}
}

void CTriggerData::FreeOperationParam(_s_operation *pOperation){
	if (pOperation->pParam)
	{
		switch (pOperation->iType)
		{
		case CTriggerData::o_talk:
			{
				O_TALK_TEXT * p = (O_TALK_TEXT *)pOperation->pParam;
				if (p->szData)
				{
					delete [] p->szData;
					p->szData = 0;
				}
			}
			break;
		}
		free(pOperation->pParam);
		pOperation->pParam = 0;
	}
}

void CTriggerData::CopyOperationParam(int operation, void *&pDst, const void *pSrc){
	if (pSrc){
		unsigned int size = GetOperationParamSize(operation);
		pDst = malloc(size);
		switch (operation){
		case o_talk:
			{
				O_TALK_TEXT * p = (O_TALK_TEXT*)pDst;
				const O_TALK_TEXT *q = (const O_TALK_TEXT *)pSrc;
				p->uSize = q->uSize;
				p->szData = new policy_char[q->uSize/2];
				memcpy(p->szData,q->szData,q->uSize);
				p->uAppendDataMask = q->uAppendDataMask;
			}
			break;
		default:
			memcpy(pDst,pSrc,size);
			break;
		}
	}else{
		pDst = 0;
	}
}

void CTriggerData::CopyOperationParam(_s_operation* pDst, const _s_operation* pSrc){
	CopyOperationParam(pSrc->iType, pDst->pParam, pSrc->pParam);
}

unsigned int CTriggerData::GetOperationParamSize(int operation)
{
	unsigned int result(0);
	switch (operation)
	{
	case o_attact:
		result = sizeof(O_ATTACK_TYPE);
		break;
	case o_use_skill:
		result = sizeof(O_USE_SKILL);
		break;
	case o_talk:
		result = sizeof(O_TALK_TEXT);
		break;
	case o_run_trigger:
		result = sizeof(O_RUN_TRIGGER);
		break;
	case o_stop_trigger:
		result = sizeof(O_STOP_TRIGGER);
		break;
	case o_active_trigger:
		result = sizeof(O_ACTIVE_TRIGGER);
		break;
	case o_create_timer:
		result = sizeof(O_CREATE_TIMER);
		break;
	case o_kill_timer:
		result = sizeof(O_KILL_TIMER);
		break;
	case o_active_controller:
		result = sizeof(O_ACTIVE_CONTROLLER);
		break;
	case o_set_global:
		result = sizeof(O_SET_GLOBAL);
		break;
	case o_revise_global:
		result = sizeof(O_REVISE_GLOBAL);
		break;
	case o_summon_monster:
		result = sizeof(O_SUMMON_MONSTER);
		break;
	case o_walk_along:
		result = sizeof(O_WALK_ALONG);
		break;
	case o_play_action:
		result = sizeof(O_PLAY_ACTION);
		break;
	case o_revise_history:
		result = sizeof(O_REVISE_HISTORY);
		break;
	case o_set_history:
		result = sizeof(O_SET_HISTORY);
		break;
	case o_deliver_faction_pvp_points:
		result = sizeof(O_DELIVER_FACTION_PVP_POINTS);
		break;
	case o_calc_var:
		result = sizeof(O_CALC_VAR);
		break;
	case o_summon_monster_2:
		result = sizeof(O_SUMMON_MONSTER_2);
		break;
	case o_walk_along_2:
		result = sizeof(O_WALK_ALONG_2);
		break;
	case o_use_skill_2:
		result = sizeof(O_USE_SKILL_2);
		break;
	case o_active_controller_2:
		result = sizeof(O_ACTIVE_CONTROLLER_2);
		break;
	case o_deliver_task:
		result = sizeof(O_DELIVER_TASK);
		break;
	case o_summon_mine:
		result = sizeof(O_SUMMON_MINE);
		break;
	case o_summon_npc:
		result = sizeof(O_SUMMON_NPC);
		break;
	case o_deliver_random_task_in_region:
		result = sizeof(O_DELIVER_RANDOM_TASK_IN_REGION);
		break;
	case o_deliver_task_in_hate_list:
		result = sizeof(O_DELIVER_TASK_IN_HATE_LIST);
		break;
	case o_clear_tower_task_in_region:
		result = sizeof(O_CLEAR_TOWER_TASK_IN_REGION);
		break;
	
	case o_save_player_count_in_radius_to_param:
		result = sizeof(O_SAVE_PLAYER_COUNT_IN_RADIUS_TO_PARAM);
		break;
	case o_save_player_count_in_region_to_param:
		result = sizeof(O_SAVE_PLAYER_COUNT_IN_REGION_TO_PARAM);
		break;
	case o_skill_with_talk:
		result = sizeof(O_SKILL_WITH_TALK);
		break;
	case o_use_skill_3:
		result = sizeof(O_USE_SKILL_3);
		break;
	case o_sort_num:
		result = sizeof(O_SORT_NUM);
		break;
	case o_get_pos_num:
		result = sizeof(O_GET_POS_NUM);
		break;
	case o_auto_bind_carrier:
		result = sizeof(O_AUTO_BIND_CARRIER);
		break;
	case o_add_range_to_hate_list:
		result = sizeof(O_ADD_RANGE_TO_HATE_LIST);
		break;
	case o_save_alive_player_count_in_radius_to_param:
		result = sizeof(O_SAVE_ALIVE_PLAYER_COUNT_IN_RADIUS_TO_PARAM);
		break;
	case o_save_alive_player_count_in_region_to_param:
		result = sizeof(O_SAVE_ALIVE_PLAYER_COUNT_IN_REGION_TO_PARAM);
		break;
	case o_walk_along_3:
		result = sizeof(O_WALK_ALONG_3);
		break;
	case o_walk_along_4:
		result = sizeof(O_WALK_ALONG_4);
		break;
	case o_save_time:
		result = sizeof(O_SAVE_TIME);
		break;
	case o_random_assignment:
		result = sizeof(O_RANDOM_ASSIGNMENT);
		break;
	case o_carrier_voting:
		result = sizeof(O_CARRIER_VOTING);
		break;
	case o_voting_result:
		result = sizeof(O_VOTING_RESULT);
		break;
	case o_voting_show:
		result = sizeof(O_VOTING_SHOW);
		break;
	case o_carrier_delivery_task:
		result = sizeof(O_CARRIER_DELIVERY_TASK);
		break;
	case o_carrier_noentry:
		result = sizeof(O_CARRIER_NOENTRY);
		break;
	case o_talk_2:
		result = sizeof(O_TALK_TEXT_2);
		break;
	case o_create_timer_2:
		result = sizeof(O_CREATE_TIMER_2);
		break;
	case o_kill_timer_2:
		result = sizeof(O_KILL_TIMER_2);
		break;
	case o_change_monster_fighting_state:
		result = sizeof(O_CHANGE_MONSTER_FIGHTING_STATE);
		break;
	case o_change_monster_active_passive:
		result = sizeof(O_CHANGE_MONSTER_ACTIVE_PASSIVE);
		break;
	case o_child_carrier_parent_monster:
		result = sizeof(O_CHILD_CARRIER_PARENT_MONSTER);
		break;
	case o_close_child_monster:
		result = sizeof(O_CLOSE_CHILD_MONSTER);
		break;
	case o_deliver_hate_targets:
		result = sizeof(O_DELIVER_HATE_TARGETS);
		break;
	case o_change_monster_attack_policy:
		result = sizeof(O_CHANGE_MONSTER_ATTACK_POLICY);
		break;
	case o_specify_failed_task_id:
		result = sizeof(O_SPECIFY_FAILED_TASK_ID);
		break;
	case o_specify_failed_task_id_regional:
		result = sizeof(O_SPECIFY_FAILED_TASK_ID_REGIONAL);
		break;
	case o_64:
		result = sizeof(O_64);
		break;
	case o_65:
		result = sizeof(O_65);
		break;
	case o_66:
		result = sizeof(O_66);
		break;
	case o_67:
		result = sizeof(O_67);
		break;
	case o_68:
		result = sizeof(O_68);
		break;
	case o_69:
		result = sizeof(O_69);
		break;
	case o_70:
		result = sizeof(O_70);
		break;
	case o_71:
		result = sizeof(O_71);
		break;
	case o_72:
		result = 36; // sizeof(O_72);	 	
		break;
	case o_73:
		result = sizeof(O_73);
		break;
	case o_74:
		result = sizeof(O_74);
		break;
	case o_75:
		result = sizeof(O_75);
		break;
	case o_76:
		result = sizeof(O_76);
		break;
	case o_77:
		result = sizeof(O_77);
		break;
	case o_78:
		result = sizeof(O_78);
		break;
	case o_79:
		result = sizeof(O_79);
		break;
	case o_80:
		result = sizeof(O_80);
		break;
	case o_81:
		result = sizeof(O_81);
		break;
	case o_82:
		result = sizeof(O_82);
		break;
	case o_83:
		result = sizeof(O_83);
		break;
	case o_85:
		result = sizeof(O_85);
		break;
	case o_86:
		result = sizeof(O_86);
		break;
	case o_87:
		result = sizeof(O_87);
		break;
	case o_88:
		result = sizeof(O_88);
		break;
	case o_89:
		result = sizeof(O_89);
		break;
	case o_90:
		result = sizeof(O_90);
		break;
	case o_91:
		result = sizeof(O_91);
		break;
	case o_92:
		result = sizeof(O_92);
		break;
	case o_93:
		result = sizeof(O_93);
		break;
	case o_94:
		result = sizeof(O_94);
		break;
	case o_95:
		result = sizeof(O_95);
		break;
	case o_96:
		result = sizeof(O_96);
		break;
	case o_98:
		result = sizeof(O_98);
		break;
	case o_99:
		result = sizeof(O_99);
		break;
	case o_100:
		result = sizeof(O_100);
		break;
	case o_101:
		result = sizeof(O_101);
		break;
	case o_102:
		result = sizeof(O_102);
		break;
	case o_103:
		result = sizeof(O_103);
		break;
	case o_104:
		result = sizeof(O_104);
		break;
	case o_105:
		result = sizeof(O_105);
		break;

	}

	return result;
}

CTriggerData::_s_operation* CTriggerData::CopyOperation(CTriggerData::_s_operation *pOperation)
{
	_s_operation* pNewObject = new _s_operation;
	if(pNewObject==0) return 0;

	pNewObject->iType = pOperation->iType;
	CopyOperationParam(pNewObject, pOperation);
	
	pNewObject->mTarget.iType = pOperation->mTarget.iType;
	CopyOperationTarget(pNewObject, pOperation);

	return pNewObject;
}

void CTriggerData::FreeOperation(_s_operation *&pOperation){
	FreeOperationParam(pOperation);
	FreeOperationTarget(pOperation);
	delete pOperation;
	pOperation = 0;
}

void CTriggerData::FreeOperationTarget(_s_target & mTarget){
	if (mTarget.pParam)
	{
		free(mTarget.pParam);
		mTarget.pParam = 0;
	}
}

void CTriggerData::FreeOperationTarget(_s_operation *pOperation){	
	FreeOperationTarget(pOperation->mTarget);
}

void CTriggerData::WriteOperationTarget(const _s_operation *pOperation, FILE *pFile){	
	switch(pOperation->mTarget.iType) 
	{
	case t_occupation_list:
		fwrite(pOperation->mTarget.pParam,sizeof(T_OCCUPATION),1,pFile);
		break;
	}
}

void CTriggerData::ReadOperationTarget(_s_operation *pOperation, FILE *pFile){

	if (pOperation->mTarget.iType == t_occupation_list)
	{
		pOperation->mTarget.pParam = malloc(sizeof(T_OCCUPATION));
		fread(pOperation->mTarget.pParam, sizeof(T_OCCUPATION), 1, pFile);
	}
	else if (pOperation->mTarget.iType == 18)
	{
		pOperation->mTarget.pParam = malloc(12);
		fread(pOperation->mTarget.pParam, 12, 1, pFile);
	}
	else if (pOperation->mTarget.iType == 20)
	{
		pOperation->mTarget.pParam = malloc(16);
		fread(pOperation->mTarget.pParam, 16, 1, pFile);
	}
	else if (pOperation->mTarget.iType < 6 || (unsigned int)(pOperation->mTarget.iType - 14) > 2)
	{
		pOperation->mTarget.pParam = NULL;
	}
	else
	{
		pOperation->mTarget.pParam = malloc(4);
		fread(pOperation->mTarget.pParam, 4, 1, pFile);
	}
}

void CTriggerData::CopyOperationTarget(_s_operation* pDst, const _s_operation* pSrc){
	switch (pDst->mTarget.iType)
	{
	case t_occupation_list:
		pDst->mTarget.pParam = malloc(sizeof(T_OCCUPATION));
		memcpy(pDst->mTarget.pParam,pSrc->mTarget.pParam,sizeof(T_OCCUPATION));
		break;
	default:
		pDst->mTarget.pParam = 0;
		break;
	}
}

//----------------------------------------------------------------------------------------
//CPolicyData
//----------------------------------------------------------------------------------------


bool CPolicyData::Save(const char *szPath)
{
	FILE *pFile = fopen(szPath,"wb");
	if(pFile==NULL) return false;
	unsigned int nVersion = F_POLICY_VERSION;
	fwrite(&nVersion,sizeof(unsigned int),1,pFile);
	fwrite(&uID,sizeof(unsigned int),1,pFile);
	int n = listTriggerPtr.size();
	fwrite(&n,sizeof(int),1,pFile);
	for( int i = 0; i < n; i++)
		listTriggerPtr[i]->Save(pFile);
	fclose(pFile);
	return true;
}

bool CPolicyData::Save(FILE *pFile)
{
	unsigned int nVersion = F_POLICY_VERSION;
	fwrite(&nVersion,sizeof(unsigned int),1,pFile);
	fwrite(&uID,sizeof(unsigned int),1,pFile);
	int n = listTriggerPtr.size();
	fwrite(&n,sizeof(int),1,pFile);
	for( int i = 0; i < n; i++)
		listTriggerPtr[i]->Save(pFile);
	return true;
}

bool CPolicyData::Load(const char *szPath)
{
	
	FILE *pFile = fopen(szPath,"rb");
	if(pFile==NULL) return false;
	unsigned int nVersion;
	fread(&nVersion,sizeof(unsigned int),1,pFile);
	if(nVersion  <= F_POLICY_VERSION)
	{
		int n = 0;
		fread(&uID,sizeof(unsigned int),1,pFile);
		fread(&n,sizeof(int),1,pFile);
		for( int i = 0; i < n; i++)
		{
			CTriggerData *pNew = new CTriggerData;
			pNew->Load(pFile);
			listTriggerPtr.push_back(pNew);
		}
	}
	fclose(pFile);
	return true;
}

bool CPolicyData::Load(FILE *pFile)
{
	unsigned int nVersion;
	fread(&nVersion,sizeof(unsigned int),1,pFile);
	if(nVersion <= F_POLICY_VERSION)
	{
		int n = 0;
		fread(&uID,sizeof(unsigned int),1,pFile);
		fread(&n,sizeof(int),1,pFile);
		for( int i = 0; i < n; i++)
		{
			CTriggerData *pNew = new CTriggerData;
			pNew->Load(pFile);
			listTriggerPtr.push_back(pNew);
		}
	}else return false;
	return true;
}

void CPolicyData::Release()
{
	int n = listTriggerPtr.size();
	for( int i = 0; i < n; ++i)
	{
		listTriggerPtr[i]->Release();
		delete listTriggerPtr[i];
	}
	listTriggerPtr.clear();
}

int CPolicyData::GetIndex( unsigned int id)
{
	int n = listTriggerPtr.size();
	for( int i = 0; i < n; ++i)
	{
		if(listTriggerPtr[i]->GetID() == id)
			return i;
	}
	return -1;
}

CTriggerData * CPolicyData::GetTriggerPtrByID(unsigned int uTriggerID)
{
	int index = GetIndex(uTriggerID);
	if (index >= 0)
	{
		return GetTriggerPtr(index);
	}
	return NULL;
}

unsigned int CPolicyData::GetNextTriggerID()
{	
	unsigned int result = 0;
	for (int i = 0; i < GetTriggerPtrNum(); ++i)
	{
		CTriggerData *pData = GetTriggerPtr(i);
		if (result <= pData->GetID())
		{
			result = pData->GetID() + 1;
		}
	}
	return result;
}

void CPolicyData::DelTriggerPtr(int idx)
{
	if((unsigned int)idx > listTriggerPtr.size()) return;
	if(listTriggerPtr[idx]) 
	{
		listTriggerPtr[idx]->Release();
		CTriggerData **temp = &listTriggerPtr[idx];
		delete listTriggerPtr[idx];
		listTriggerPtr.erase(temp);
	}
}


//----------------------------------------------------------------------------------------
//CPolicyDataMannager
//----------------------------------------------------------------------------------------

bool CPolicyDataManager::Load(const char* szPath)
{
	FILE *pFile = fopen(szPath,"rb");
	if(pFile==0) return false;
	
	unsigned int uVersion;	
	int num;
	fread(&uVersion,sizeof(unsigned int),1,pFile);
	if(uVersion!=F_POLICY_EXP_VERSION) 
	{
		fclose(pFile);
		return false;
	}
	fread(&num,sizeof(int),1,pFile);
	for (int i = 0; i < num; ++i)
	{
		CPolicyData *pNew = new CPolicyData;
		if(pNew==0) goto fail;
		if(!pNew->Load(pFile)) goto fail;
		listPolicy.push_back(pNew);
	}
	fclose(pFile);
	return true;

fail:
	fclose(pFile);
	num = listPolicy.size();
	for( int k = 0; k < num; ++k)
	{
		listPolicy[k]->Release();
		delete listPolicy[k];
	}
	listPolicy.clear();
	return false;
}

bool CPolicyDataManager::Save(const char* szPath)
{
	FILE *pFile = fopen(szPath,"wb");
	if(pFile==0) return false;
	
	unsigned int uVersion = F_POLICY_EXP_VERSION;
	int num = listPolicy.size();
	fwrite(&uVersion,sizeof(unsigned int),1,pFile);
	fwrite(&num,sizeof(int),1,pFile);
	for (int i = 0; i < num; ++i)
	{
		CPolicyData *pPolicy = listPolicy[i];
		if(!pPolicy->Save(pFile)) 
		{
			fclose(pFile);
			return false;
		}
	}
	fclose(pFile);
	return true;
}

void CPolicyDataManager::Release()
{
	int n = listPolicy.size();
	for( int i = 0; i < n; ++i)
	{
		listPolicy[i]->Release();
		delete listPolicy[i];
	}
	listPolicy.clear();
}
