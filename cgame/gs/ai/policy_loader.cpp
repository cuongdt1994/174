#include "policy.h"
#include "policy_loader.h"
#include "../aitrigger.h"
#include "../world.h"
#include "../worldmanager.h"
#include <ASSERT.h>

using namespace ai_trigger;

namespace
{
	expr * ConvertExpr (void *pTree);

	condition * ConvertCondition(void * pTree)
	{
		CTriggerData::_s_tree_item * temp = (CTriggerData::_s_tree_item*)pTree;
		switch(temp->mConditon.iType) 
		{
			case CTriggerData::c_time_come:
				return new cond_timer(((C_TIME_COME*)temp->mConditon.pParam)->uID);
			case CTriggerData::c_hp_less:
				return new cond_hp_less(((C_HP_LESS*)temp->mConditon.pParam)->fPercent);
			case CTriggerData::c_random:
				return new cond_random(((C_RANDOM*)temp->mConditon.pParam)->fProbability);

			case CTriggerData::c_and:
				{
					condition * left = ConvertCondition(temp->pLeft);
					condition * right = ConvertCondition(temp->pRight);
					return new cond_and(left,right);
				}
			case CTriggerData::c_not:
				{
					condition * right = ConvertCondition(temp->pRight);
					return new cond_not(right);
				}
				
			case CTriggerData::c_or:
				{
					condition * left = ConvertCondition(temp->pLeft);
					condition * right = ConvertCondition(temp->pRight);
					return new cond_or(left,right);
				}

			case CTriggerData::c_kill_player:
				return new cond_kill_target();
			case CTriggerData::c_start_attack:
				return new cond_start_combat();

			case CTriggerData::c_died:
				return new cond_on_death();

			case CTriggerData::c_less:
				{
					expr * left = ConvertExpr(temp->pLeft);
					expr * right = ConvertExpr(temp->pRight);
					return new cond_compare_less (left, right);
				}
			case CTriggerData::c_great:
				{
					expr * left = ConvertExpr(temp->pLeft);
					expr * right = ConvertExpr(temp->pRight);
					return new cond_compare_greater (left, right);
				}
			case CTriggerData::c_equ:
				{
					expr * left = ConvertExpr(temp->pLeft);
					expr * right = ConvertExpr(temp->pRight);
					return new cond_compare_equal (left, right);
				}
			//case CTriggerData::c_time_point://lgc
				//return new cond_time_point(((C_TIME_POINT*)temp->mConditon.pParam)->uHour, ((C_TIME_POINT*)temp->mConditon.pParam)->uMinute);	

			case CTriggerData::c_be_hurt:
				return new cond_on_damage(((C_BE_HURT*)temp->mConditon.pParam)->iHurtLow,((C_BE_HURT*)temp->mConditon.pParam)->iHurtHigh);

			case CTriggerData::c_reach_end:
				return new cond_path_end(((C_REACH_END*)temp->mConditon.pParam)->iPathID);
			
			case CTriggerData::c_at_history_stage:
				return new cond_at_history_stage(((C_HISTORY_STAGE*)temp->mConditon.pParam)->iID);
			
			case CTriggerData::c_stop_fight:
				return new cond_end_combat();

			case CTriggerData::c_reach_end_2:
				return new cond_path_end_2(((C_REACH_END_2*)temp->mConditon.pParam)->iPathID,((C_REACH_END_2*)temp->mConditon.pParam)->iPathIDType); 

			case CTriggerData::c_has_filter:
				return new cond_spec_filter(((C_HAS_FILTER*)temp->mConditon.pParam)->iID) ;

			case CTriggerData::c_get_servertime_day: // Solicita a data ( timestamp )
				return new cond_get_servertime_day();

			case CTriggerData::c_get_servertime_week:
				return new cond_get_servertime_week(); // Solicita a data ( DIA hora:minuto )

		case CTriggerData::c_not_bind_carrier: // Verifica (bool)
				return new cond_not_bind_carrier();

			case CTriggerData::c_reach_start: 
				return new cond_reach_start(((C_REACH_END_2*)temp->mConditon.pParam)->iPathID,((C_REACH_END_2*)temp->mConditon.pParam)->iPathIDType); 

			case CTriggerData::c_carrier_full: // Verifica (bool)
				return new cond_carrier_full();

			case CTriggerData::c_time_come_2:
				return new cond_timer_2(((C_TIME_COME*)temp->mConditon.pParam)->uID);
			
			case CTriggerData::c_35:
				return new cond_35();

			case CTriggerData::c_get_radius_player_count:
				return new cond_36();
			
			case CTriggerData::c_37:
				return new cond_37();

			case CTriggerData::c_38:
				return new cond_38();

			case CTriggerData::c_39:
				return new cond_39();
			
			case CTriggerData::c_40:
				return new cond_40();

			case CTriggerData::c_41:
				return new cond_41();

			case CTriggerData::c_42:
				return new cond_42();

			case CTriggerData::c_43:
				return new cond_43();

			case CTriggerData::c_44:
				return new cond_44();

			case CTriggerData::c_45:
				return new cond_45();

			case CTriggerData::c_46:
				return new cond_46();

			case CTriggerData::c_47:
				return new cond_47();
			default:
		 	printf("[policy_loader][ConvertCondition] unknown condition type %d\n", temp->mConditon.iType);
			ASSERT(false);
			break;
		}
		return NULL;
	}

	expr * ConvertExpr (void *pTree)
	{
		//��ʵ���ⲿ���Ǳ���ʽ��������������
		CTriggerData::_s_tree_item * temp = (CTriggerData::_s_tree_item*)pTree;
		switch(temp->mConditon.iType)
		{
			case CTriggerData::c_plus:
				{
					expr * left = ConvertExpr(temp->pLeft);
					expr * right = ConvertExpr(temp->pRight);
					return new expr_plus (left, right);
				}
			case CTriggerData::c_minus:
				{
					expr * left = ConvertExpr(temp->pLeft);
					expr * right = ConvertExpr(temp->pRight);
					return new expr_minus (left, right);
				}
			case CTriggerData::c_multiply:
				{
					expr * left = ConvertExpr(temp->pLeft);
					expr * right = ConvertExpr(temp->pRight);
					return new expr_multiply (left, right);
				}
			case CTriggerData::c_divide:
				{
					expr * left = ConvertExpr(temp->pLeft);
					expr * right = ConvertExpr(temp->pRight);
					return new expr_divide (left, right);
				}
			case CTriggerData::c_constant:
				{
					return new expr_constant(((C_CONSTANT*)temp->mConditon.pParam)->iValue);
				}
			case CTriggerData::c_var:
				{
					return new expr_common_data(((C_VAR*)temp->mConditon.pParam)->iID);
				}
			case CTriggerData::c_history_value:
				{
					return new expr_history_value(((C_HISTORY_VALUE*)temp->mConditon.pParam)->iValue); 
				}
			case CTriggerData::c_local_var:
				{
					return new expr_local_value(((C_LOCAL_VAR*)temp->mConditon.pParam)->iID);
				}
			case CTriggerData::c_room_index:
				{
					return new expr_room_index();
				}
			case CTriggerData::c_player_count_in_radius:
				{
					return new expr_sphere_player_count(((C_PLAYER_COUNT_IN_RADIUS*)temp->mConditon.pParam)->fRadius);
				}
			case CTriggerData::c_player_count_in_region:
				{
					C_PLAYER_COUNT_IN_REGION* joslian = ((C_PLAYER_COUNT_IN_REGION*)temp->mConditon.pParam);
					return new expr_box_player_count(joslian->zvMin.x, joslian->zvMin.z, joslian->zvMax.x, joslian->zvMax.z);
				}

			case CTriggerData::c_35:
				{
					return new expr_c_35();
				}
			case CTriggerData::c_get_radius_player_count:
				{
					C_PLAYER_COUNT_IN_REGION* joslian = ((C_PLAYER_COUNT_IN_REGION*)temp->mConditon.pParam);
					return new expr_c_get_radius_player_count(joslian->zvMin.x, joslian->zvMin.z, joslian->zvMax.x, joslian->zvMax.z);
				}
			case CTriggerData::c_37:
				{
					return new expr_c_37();
				}
			case CTriggerData::c_38:
				{
					return new expr_c_38();
				}
			case CTriggerData::c_39:
				{
					return new expr_c_39();
				}

			case CTriggerData::c_45:
				{
					return new expr_c_45();
				}
			case CTriggerData::c_47:
				{
					return new expr_c_47();
				}
					
			default:
			 	printf("[policy_loader][ConvertExpr] unknown expr condition type %d\n", temp->mConditon.iType);
				ASSERT(false);
				break;
		}
		return NULL;
	}

	target * ConvertTarget(const CTriggerData::_s_target & mTarget)
	{
		//Ŀ�����ͣ�Ŀǰֻ��t_occupation_list�в���
		switch(mTarget.iType)
		{
			case CTriggerData::t_hate_first:
				return new target_aggro_first();
			case CTriggerData::t_hate_second:
				return new target_aggro_second();
			case CTriggerData::t_hate_others:
				return new target_aggro_second_rand();
			case CTriggerData::t_most_hp:
				return new target_most_hp();
			case CTriggerData::t_most_mp:
				return new target_most_mp();
			case CTriggerData::t_least_hp:
				return new target_least_hp();
			case CTriggerData::t_self:
				return new target_self();
			case CTriggerData::t_occupation_list:
				{
					int bit =  ((T_OCCUPATION*)mTarget.pParam)->uBit;
					return new target_class_combo(bit);
				}
			case CTriggerData::t_monster_killer:
				return new target_monster_killer();
			case CTriggerData::t_monster_birthplace_faction:
				return new target_monster_birthplace_faction();
			case CTriggerData::t_hate_random_one:
				return new target_aggro_special(target_aggro_special::ATAS_RAND);
			case CTriggerData::t_hate_nearest:
				return new target_aggro_special(target_aggro_special::ATAS_NEAR);
			case CTriggerData::t_hate_farthest:
				return new target_aggro_special(target_aggro_special::ATAS_FAR);
			case CTriggerData::t_hate_first_redirected:
				return new target_aggro_first_redirected();

			case CTriggerData::t_random_in_range:
				return NULL;
			case CTriggerData::t_nearest_in_range:
				return NULL;
			case CTriggerData::t_farthest_in_range:
				return NULL;
			case CTriggerData::t_17:
				return NULL;
			case CTriggerData::t_18:
				return NULL;
			case CTriggerData::t_19:
				return NULL;
			case CTriggerData::t_20:
				return NULL;
		}

	 	printf("[policy_loader][ConvertTarget] unknown target type %d\n", mTarget.iType);

		return NULL;
	}

	trigger * ConvertTrigger(CPolicyData *pPolicyData, CTriggerData *pTriggerData);
	operation * ConvertOperation(CPolicyData *pPolicyData, CTriggerData::_s_operation *pOperation)
	{
		target * tar = ConvertTarget(pOperation->mTarget);
		operation * pOP = NULL;

		#define OPARAM(x) ((x*)pOperation->pParam)

		//��������
		switch(pOperation->iType) 
		{
			case CTriggerData::o_attact:
				pOP = new op_attack(OPARAM(O_ATTACK_TYPE)->uType);
				break;
			case CTriggerData::o_use_skill:
				pOP = new op_skill(OPARAM(O_USE_SKILL)->uSkill,OPARAM(O_USE_SKILL)->uLevel);
				break;
			case CTriggerData::o_use_skill_2:
				pOP = new op_skill_2(OPARAM(O_USE_SKILL_2)->uSkill,OPARAM(O_USE_SKILL_2)->uSkillType,
						OPARAM(O_USE_SKILL_2)->uLevel,OPARAM(O_USE_SKILL_2)->uLevelType);
				break;
			case CTriggerData::o_talk:
				if(OPARAM(O_TALK_TEXT)->uAppendDataMask)
					pOP = new op_say_2(OPARAM(O_TALK_TEXT)->szData,OPARAM(O_TALK_TEXT)->uSize,OPARAM(O_TALK_TEXT)->uAppendDataMask);
				else	
					pOP = new op_say(OPARAM(O_TALK_TEXT)->szData,OPARAM(O_TALK_TEXT)->uSize);
				break;
			case CTriggerData::o_reset_hate_list:
				pOP = new op_reset_aggro();
				break;
			case CTriggerData::o_run_trigger:
				{
					int idx = pPolicyData->GetIndex(OPARAM(O_RUN_TRIGGER)->uID);
					CTriggerData *pTriggerData = pPolicyData->GetTriggerPtr(idx);
					trigger * pT = ConvertTrigger(pPolicyData,pTriggerData);
					pOP = new op_exec_trigger(pT);
				}
				break;
			case CTriggerData::o_stop_trigger:
				pOP = new op_enable_trigger(OPARAM(O_STOP_TRIGGER)->uID,false);
				break;
			case CTriggerData::o_active_trigger:
				pOP = new op_enable_trigger(OPARAM(O_ACTIVE_TRIGGER)->uID,true);
				break;
			case CTriggerData::o_create_timer:
				pOP = new op_create_timer(OPARAM(O_CREATE_TIMER)->uID,OPARAM(O_CREATE_TIMER)->uPeriod,OPARAM(O_CREATE_TIMER)->uCounter);
				break;
			case CTriggerData::o_kill_timer:
				pOP = new op_remove_timer(OPARAM(O_KILL_TIMER)->uID);
				break;

			case CTriggerData::o_flee:
				pOP = new op_flee();
				break;
			case CTriggerData::o_set_hate_to_first:
				pOP = new op_be_taunted();
				break;
			case CTriggerData::o_set_hate_to_last:
				pOP = new op_fade_target();
				break;
			case CTriggerData::o_set_hate_fifty_percent:
				pOP = new op_aggro_fade();
				break;
			case CTriggerData::o_skip_operation:
				pOP = new op_break();
				break;
			case CTriggerData::o_active_controller:
				pOP = new op_active_spawner(OPARAM(O_ACTIVE_CONTROLLER)->uID,OPARAM(O_ACTIVE_CONTROLLER)->bStop);
				break;
			case CTriggerData::o_active_controller_2:
				pOP = new op_active_spawner_2(OPARAM(O_ACTIVE_CONTROLLER_2)->uID,OPARAM(O_ACTIVE_CONTROLLER_2)->uIDType,OPARAM(O_ACTIVE_CONTROLLER_2)->bStop);
				break;
			case CTriggerData::o_set_global:
				{
					O_SET_GLOBAL * pSG = (O_SET_GLOBAL*)(pOperation->pParam);
					pOP = new op_set_common_data (pSG->iID, pSG->iValue, pSG->bIsValue);
				}
				break;
			case CTriggerData::o_revise_global:
				{
					O_REVISE_GLOBAL * pRG = (O_REVISE_GLOBAL*)(pOperation->pParam);
					pOP = new op_add_common_data (pRG->iID, pRG->iValue);
				}
				break;
			case CTriggerData::o_summon_monster:
				{
					O_SUMMON_MONSTER* pSM = (O_SUMMON_MONSTER *)(pOperation->pParam);
					pOP = new op_summon_monster(pSM->iMonsterID, pSM->iMonsterNum, pSM->iRange, pSM->iLife, (char)pSM->iDispearCondition, pSM->iPathID);
				}
				break;
			case CTriggerData::o_summon_monster_2:
				{
					O_SUMMON_MONSTER_2* pSM = (O_SUMMON_MONSTER_2 *)(pOperation->pParam);
					pOP = new op_summon_monster_2(pSM->iMonsterID,pSM->iMonsterIDType, pSM->iMonsterNum,pSM->iMonsterNumType, pSM->iRange, pSM->iLife, (char)pSM->iDispearCondition, pSM->iPathID,pSM->iPathIDType);
				}
				break;
			case CTriggerData::o_walk_along:
				{
					O_WALK_ALONG* pWA = (O_WALK_ALONG*)(pOperation->pParam);
					pOP = new op_change_path(pWA->iWorldID,pWA->iPathID,pWA->iPatrolType,pWA->iSpeedType);
				}
				break;
			case CTriggerData::o_walk_along_2:
				{
					O_WALK_ALONG_2* pWA = (O_WALK_ALONG_2*)(pOperation->pParam);
					pOP = new op_change_path_2(pWA->iWorldID,pWA->iPathID,pWA->iPathIDType,pWA->iPatrolType,pWA->iSpeedType);
				}
				break;
			case CTriggerData::o_play_action:
				{
					O_PLAY_ACTION* pPA= (O_PLAY_ACTION*)(pOperation->pParam);
					pOP = new op_play_action(pPA->szActionName,pPA->iLoopCount,pPA->iPlayTime,pPA->iInterval);
				}
				break;
			case CTriggerData::o_revise_history:
				{
					O_REVISE_HISTORY* pRH = (O_REVISE_HISTORY*)(pOperation->pParam);
					pOP = new op_revise_history(pRH->iID,pRH->iValue);
				}
				break;
			case CTriggerData::o_set_history:
				{
					O_SET_HISTORY* pSH = (O_SET_HISTORY*)(pOperation->pParam);
					pOP = new op_set_history(pSH->iID,pSH->iValue,pSH->bIsHistoryValue);
				}
				break;
			case CTriggerData::o_deliver_faction_pvp_points:
				{
					O_DELIVER_FACTION_PVP_POINTS* pDF = (O_DELIVER_FACTION_PVP_POINTS*)(pOperation->pParam);
					pOP = new op_deliver_faction_pvp_points(pDF->uType);
				}
				break;
			case CTriggerData::o_calc_var:
				{
					O_CALC_VAR* pCV = (O_CALC_VAR*)(pOperation->pParam);
					pOP = new op_calc_var(pCV->iDst,pCV->iDstType,pCV->iSrc1,pCV->iSrc1Type,pCV->iSrc2,pCV->iSrc2Type,pCV->iOp); 
				}
				break;
			case CTriggerData::o_deliver_task:
				{
					O_DELIVER_TASK* pDT = (O_DELIVER_TASK*)(pOperation->pParam); 
					pOP = new op_deliver_task(pDT->uID,pDT->uIDType); 
				}
				break;
			case CTriggerData::o_summon_mine:
				{
					O_SUMMON_MINE* pSM = (O_SUMMON_MINE*)(pOperation->pParam); 
					pOP = new op_summon_mine(pSM->iMineID,pSM->iMineIDType,pSM->iMineNum,pSM->iMineNumType,
							pSM->iLife,pSM->iLifeType,pSM->iRange);
				}
				break;
			case CTriggerData::o_summon_npc:
				{
					O_SUMMON_NPC* pSN = (O_SUMMON_NPC*)(pOperation->pParam);
					pOP = new op_summon_npc(pSN->iNPCID,pSN->iNPCIDType,pSN->iNPCNum,pSN->iNPCNumType,
							pSN->iRange,pSN->iLife,pSN->iLifeType,pSN->iPathID,pSN->iPathIDType); 
				}
				break;
			case CTriggerData::o_deliver_random_task_in_region:
				{
					O_DELIVER_RANDOM_TASK_IN_REGION* pDR = (O_DELIVER_RANDOM_TASK_IN_REGION*)(pOperation->pParam);	
					pOP = new op_deliver_random_task_in_region(pDR->uID,pDR->zvMin.x,pDR->zvMin.z,
							pDR->zvMax.x,pDR->zvMax.z);
				}
				break;
			case CTriggerData::o_deliver_task_in_hate_list:
				{
					O_DELIVER_TASK_IN_HATE_LIST* pDT = (O_DELIVER_TASK_IN_HATE_LIST*)(pOperation->pParam);
					pOP = new op_deliver_task_in_dmglist(pDT->uID,pDT->uIDType,pDT->iRange,pDT->iPlayerNum); 
				}
				break;
			case CTriggerData::o_clear_tower_task_in_region:
				{
					O_CLEAR_TOWER_TASK_IN_REGION* pCT =(O_CLEAR_TOWER_TASK_IN_REGION*)(pOperation->pParam);
					pOP = new op_clear_tower_task_in_region(pCT->zvMin.x,pCT->zvMin.z,pCT->zvMax.x,pCT->zvMax.z);
				}
				break;
			case CTriggerData::o_save_player_count_in_radius_to_param:
				{
					O_SAVE_PLAYER_COUNT_IN_RADIUS_TO_PARAM* pWT = (O_SAVE_PLAYER_COUNT_IN_RADIUS_TO_PARAM*)(pOperation->pParam);
					pOP = new op_get_sphere_player_count(pWT->fRadiusValue, pWT->uRadiusType, pWT->iTargetID, pWT->uTargetType);
				}
				break;
			case CTriggerData::o_save_player_count_in_region_to_param:
				{
					O_SAVE_PLAYER_COUNT_IN_REGION_TO_PARAM* pZT = (O_SAVE_PLAYER_COUNT_IN_REGION_TO_PARAM*)(pOperation->pParam);
					pOP = new op_get_box_player_count(pZT->zvMin.x, pZT->zvMin.z, pZT->zvMax.x, pZT->zvMax.z, pZT->iTargetID, pZT->uTargetType);
				}
				break;
			
			case CTriggerData::o_skill_with_talk:
				{
					O_SKILL_WITH_TALK* parm = (O_SKILL_WITH_TALK*)(pOperation->pParam);
					pOP = new op_skill_with_talk(parm->skill.uSkill, parm->skill.uSkillType, parm->skill.uLevel, parm->skill.uLevelType, parm->talk.uSize, parm->talk.szData, parm->talk.uAppendDataMask);
				}
				break;

			case CTriggerData::o_use_skill_3:
				{
					O_USE_SKILL_3* pUS = (O_USE_SKILL_3*)(pOperation->pParam);
					pOP = new op_use_skill_3(pUS->uSkill,pUS->uSkillType,pUS->uLevel,pUS->uLevelType,pUS->tmpID,pUS->life);
				}
				break;

			case CTriggerData::o_sort_num:
				{
					O_SORT_NUM* pSN = (O_SORT_NUM*)(pOperation->pParam);
					pOP = new op_sort_num(pSN->uNum,pSN->uuNumType,pSN->varID,pSN->varType);
				}
				break;			
			case CTriggerData::o_get_pos_num:
				{
					O_GET_POS_NUM* pPN = (O_GET_POS_NUM*)(pOperation->pParam);
					pOP = new op_get_pos_num(pPN->uNum, pPN->uuNumType, pPN->pos, pPN->posType, pPN->varID, pPN->varType);
				}
				break;

			case CTriggerData::o_auto_bind_carrier:
				{
					O_AUTO_BIND_CARRIER* pABC = (O_AUTO_BIND_CARRIER*)(pOperation->pParam);
					pOP = new op_auto_bind_carrier(pABC->range);
				}
				break;

			case CTriggerData::o_add_range_to_hate_list:
				{
					O_ADD_RANGE_TO_HATE_LIST* pAR = (O_ADD_RANGE_TO_HATE_LIST*)(pOperation->pParam);
					pOP = new op_add_range_to_hate_list(pAR->range, pAR->rangeType);
				}
				break;
			
			case CTriggerData::o_save_alive_player_count_in_radius_to_param:
				{
					O_SAVE_ALIVE_PLAYER_COUNT_IN_RADIUS_TO_PARAM* pWT = (O_SAVE_ALIVE_PLAYER_COUNT_IN_RADIUS_TO_PARAM*)(pOperation->pParam);
					pOP = new op_get_sphere_alive_player_count(pWT->fRadiusValue, pWT->uRadiusType, pWT->iTargetID, pWT->uTargetType);
				}
				break;

			case CTriggerData::o_save_alive_player_count_in_region_to_param:
				{
					O_SAVE_ALIVE_PLAYER_COUNT_IN_REGION_TO_PARAM* pZT = (O_SAVE_ALIVE_PLAYER_COUNT_IN_REGION_TO_PARAM*)(pOperation->pParam);
					pOP = new op_get_box_alive_player_count(pZT->zvMin.x, pZT->zvMin.z, pZT->zvMax.x, pZT->zvMax.z, pZT->iTargetID, pZT->uTargetType);
				}
				break;

			case CTriggerData::o_walk_along_3:
				{				
					O_WALK_ALONG_3* pWA = (O_WALK_ALONG_3*)(pOperation->pParam);
					pOP = new op_walk_along_3(pWA->iWorldID, pWA->iStartPathID, pWA->iStartPathIDType, pWA->iEndPathID, pWA->iEndPathIDType, pWA->iPatrolType, pWA->iSpeedType);
				}
				break;

			case CTriggerData::o_walk_along_4:
				{				
					O_WALK_ALONG_4* pWA = (O_WALK_ALONG_4*)(pOperation->pParam);
					pOP = new op_walk_along_4(pWA->iWorldID, pWA->iStartPathID, pWA->iStartPathIDType, pWA->iEndPathID, pWA->iEndPathIDType, 
					pWA->iMinPathID, pWA->iMinPathIDType, pWA->iMaxPathID, pWA->iMaxPathIDType, pWA->iPatrolType, pWA->iSpeedType, pWA->iOrientationType);
				}
				break;

			case CTriggerData::o_save_time:
				{
					O_SAVE_TIME* pST = (O_SAVE_TIME*)(pOperation->pParam);
					pOP = new op_save_time(pST->iTimeType, pST->iParamID, pST->iParamIDType);
				}
				break;

				case CTriggerData::o_random_assignment:
				{
					O_RANDOM_ASSIGNMENT *pRA = (O_RANDOM_ASSIGNMENT *)(pOperation->pParam);
					pOP = new op_random_assignment(pRA->iRandomMin, pRA->iRandomMax, pRA->iQuantity, pRA->iTargetID);
				}
				break;

				case CTriggerData::o_carrier_voting:
				{
					O_CARRIER_VOTING *pCV = (O_CARRIER_VOTING *)(pOperation->pParam);
					pOP = new op_carrier_voting(pCV->iVoteID, pCV->iVoteIDType);
				}
				break;

				case CTriggerData::o_voting_result:
				{
					O_VOTING_RESULT *pVR = (O_VOTING_RESULT *)(pOperation->pParam);
					pOP = new op_voting_result(pVR->iVoteID, pVR->iVoteIDType, pVR->iSelect, pVR->iSelectType, pVR->iSaveResult, pVR->iSaveResultType);
				}
				break;

				case CTriggerData::o_voting_show:
				{
					O_VOTING_SHOW *pVS = (O_VOTING_SHOW *)(pOperation->pParam);

					packet_wrapper h1(8);
					pOP = new op_voting_show(pVS->iStyle, h1.data());
				}
				break;

				case CTriggerData::o_carrier_delivery_task:
				{
					O_CARRIER_DELIVERY_TASK *pCDT = (O_CARRIER_DELIVERY_TASK *)(pOperation->pParam);
					pOP = new op_carrier_delivery_task(pCDT->iTaskID, pCDT->iTaskIDType);
				}
				break;

				case CTriggerData::o_carrier_noentry:
				{
					O_CARRIER_NOENTRY *pCNE = (O_CARRIER_NOENTRY *)(pOperation->pParam);
					pOP = new op_carrier_noentry(pCNE->iOpen);
				}
				break;

				case CTriggerData::o_talk_2:
				if(OPARAM(O_TALK_TEXT_2)->talk.uAppendDataMask)
					pOP = new op_say_2_new(OPARAM(O_TALK_TEXT_2)->talk.szData,OPARAM(O_TALK_TEXT_2)->talk.uSize,OPARAM(O_TALK_TEXT_2)->talk.uAppendDataMask);
				else	
					pOP = new op_say_new(OPARAM(O_TALK_TEXT_2)->talk.szData,OPARAM(O_TALK_TEXT_2)->talk.uSize);
				break;

				case CTriggerData::o_create_timer_2:
				{
					O_CREATE_TIMER_2 *pCT2 = (O_CREATE_TIMER_2 *)(pOperation->pParam);
					pOP = new op_create_timer_2(pCT2->uID, pCT2->iIDType, pCT2->uPeriod, pCT2->iPeriodType, pCT2->uCounter, pCT2->iCounterType);
				}
				break;

				case CTriggerData::o_kill_timer_2:
				{
					O_KILL_TIMER_2 *pKT2 = (O_KILL_TIMER_2 *)(pOperation->pParam);
					pOP = new op_kill_timer_2(pKT2->uID, pKT2->iIDType);
				}
				break;

				case CTriggerData::o_change_monster_fighting_state:
				{
					O_CHANGE_MONSTER_FIGHTING_STATE *pCMFS = (O_CHANGE_MONSTER_FIGHTING_STATE *)(pOperation->pParam);
					pOP = new op_change_monster_fighting_state(pCMFS->iChangeState);
				}
				break;

				case CTriggerData::o_change_monster_active_passive:
				{
					O_CHANGE_MONSTER_ACTIVE_PASSIVE *pCMAP = (O_CHANGE_MONSTER_ACTIVE_PASSIVE *)(pOperation->pParam);
					pOP = new op_change_monster_active_passive(pCMAP->iChoose);
				}
				break;

				case CTriggerData::o_child_carrier_parent_monster:
				{
					O_CHILD_CARRIER_PARENT_MONSTER *pCCPM = (O_CHILD_CARRIER_PARENT_MONSTER *)(pOperation->pParam);
					pOP = new op_child_carrier_parent_monster(pCCPM->fRange, pCCPM->iPos);
				}
				break;

				case CTriggerData::o_close_child_monster:
				{
					O_CLOSE_CHILD_MONSTER *pCCM = (O_CLOSE_CHILD_MONSTER *)(pOperation->pParam);
					pOP = new op_close_child_monster(pCCM->iMonsterID);
				}
				break;

				case CTriggerData::o_deliver_hate_targets:
				{
					O_DELIVER_HATE_TARGETS *pDHT = (O_DELIVER_HATE_TARGETS *)(pOperation->pParam);
					pOP = new op_deliver_hate_targets(pDHT->iMonsterID);
				}
				break;

				case CTriggerData::o_change_monster_attack_policy:
				{
					O_CHANGE_MONSTER_ATTACK_POLICY *pCMAP = (O_CHANGE_MONSTER_ATTACK_POLICY *)(pOperation->pParam);
					pOP = new op_change_monster_attack_policy(pCMAP->iAttackPolicyID);
				}
				break;
				
				case CTriggerData::o_specify_failed_task_id:
				{
					O_SPECIFY_FAILED_TASK_ID *pSFTI = (O_SPECIFY_FAILED_TASK_ID *)(pOperation->pParam);
					pOP = new op_specify_failed_task_id(pSFTI->iTaskID, pSFTI->iTaskIDType);
				}
				break;

				case CTriggerData::o_specify_failed_task_id_regional:
				{
					O_SPECIFY_FAILED_TASK_ID_REGIONAL* pZT = (O_SPECIFY_FAILED_TASK_ID_REGIONAL*)(pOperation->pParam);
					pOP = new op_specify_failed_task_id_regional(pZT->zvMin.x, pZT->zvMin.z, pZT->zvMax.x, pZT->zvMax.z, pZT->iTaskID, pZT->iTaskIDType);
				}
				break;
				
				case CTriggerData::o_64:
				{
					O_64 *pO64 = (O_64 *)(pOperation->pParam);
					pOP = new op_64(pO64->ID, pO64->Val1, pO64->Val2, pO64->Val3, pO64->Val4, pO64->Val5, pO64->Val6, pO64->Val7, pO64->Val8, pO64->Val9, pO64->Val10, pO64->Val11, pO64->Val12, pO64->Val13, pO64->Val14, pO64->Val15, pO64->Val16, pO64->Val17, pO64->Val18, pO64->Val19, pO64->Val20);
				}
				break;

				case CTriggerData::o_65:
				{
					O_65 *pO65 = (O_65 *)(pOperation->pParam);
					pOP = new op_65(pO65->Val1, pO65->Val2, pO65->Val3, pO65->Val4, pO65->Val5, pO65->Val6, pO65->Val7, pO65->Val8, pO65->Val9, pO65->Val10, pO65->Val11);
				}
				break;

				case CTriggerData::o_66:
				{
					O_66 *pO66 = (O_66 *)(pOperation->pParam);
					pOP = new op_66(pO66->Val1, pO66->Val2, pO66->Val3, pO66->Val4, pO66->Val5, pO66->Val6, pO66->Val7, pO66->Val8);
				}
				break;

				case CTriggerData::o_67:
				{
					O_67 *pO67 = (O_67 *)(pOperation->pParam);
					pOP = new op_67(pO67->Val1, pO67->Val2, pO67->Val3, pO67->Val4, pO67->Val5, pO67->Val6, pO67->Val7);
				}
				break;

				case CTriggerData::o_68:
				{
					O_68 *pO68 = (O_68 *)(pOperation->pParam);
					pOP = new op_68(pO68->Val1, pO68->Val2, pO68->Val3);
				}
				break;

				case CTriggerData::o_69:
				{
					O_69 *pO69 = (O_69 *)(pOperation->pParam);
					pOP = new op_69(pO69->Val1, pO69->Val2);
				}
				break;

				case CTriggerData::o_70:
				{
					O_70 *pO70 = (O_70 *)(pOperation->pParam);
					pOP = new op_70(pO70->Val1);
				}
				break;

				case CTriggerData::o_71:
				{
					O_71 *pO71 = (O_71 *)(pOperation->pParam);
					pOP = new op_71(pO71->Val1, pO71->Val2);
				}
				break;
				case CTriggerData::o_72:
				{
					O_72 *pO72 = (O_72 *)(pOperation->pParam);
					pOP = new op_72();
				}
				break;	
				
				case CTriggerData::o_73:
				{
					O_73 *pO73 = (O_73 *)(pOperation->pParam);
					pOP = new op_73(pO73->Val1, pO73->Val2);
				}
				break;

				case CTriggerData::o_74:
				{
					O_74 *pO74 = (O_74 *)(pOperation->pParam);
					pOP = new op_74(pO74->Val1, pO74->Val2, pO74->Val3, pO74->Val4, pO74->Val5, pO74->Val6, pO74->Val7, pO74->Val8, pO74->Val9, pO74->Val10, pO74->Val11, pO74->Val12, pO74->Val13, pO74->Val14, pO74->Val15, pO74->Val16, pO74->Val17);
				}
				break;

				case CTriggerData::o_75:
				{
					O_75 *pO75 = (O_75 *)(pOperation->pParam);
					pOP = new op_75(pO75->Val1);
				}
				break;

				case CTriggerData::o_76:
				{
					O_76 *pO76 = (O_76 *)(pOperation->pParam);
					pOP = new op_76(pO76->Val1, pO76->Val2, pO76->Val3, pO76->Val4);
				}
				break;

				case CTriggerData::o_77:
				{
					O_77 *pO77 = (O_77 *)(pOperation->pParam);
					pOP = new op_77(pO77->Val1, pO77->Val2, pO77->Val3, pO77->Val4, pO77->Val5, pO77->Val6);
				}
				break;

				case CTriggerData::o_78:
				{
					O_78 *pO78 = (O_78 *)(pOperation->pParam);
					pOP = new op_78(pO78->Val1, pO78->Val2, pO78->Val3, pO78->Val4, pO78->Val5, pO78->Val6, pO78->Val7, pO78->Val8, pO78->Val9, pO78->Val10);
				}
				break;

				case CTriggerData::o_79:
				{
					O_79 *pO79 = (O_79 *)(pOperation->pParam);
					pOP = new op_79(pO79->Val1, pO79->Val2);
				}
				break;

				case CTriggerData::o_80:
				{
					O_80 *pO80 = (O_80 *)(pOperation->pParam);
					pOP = new op_80(pO80->Val1, pO80->Val2, pO80->Val3, pO80->Val4, pO80->Val5, pO80->Val6);
				}
				break;

				case CTriggerData::o_81:
				{
					O_81 *pO81 = (O_81 *)(pOperation->pParam);
					pOP = new op_81(pO81->Val1, pO81->Val2, pO81->Val3, pO81->Val4, pO81->Val5, pO81->Val6);
				}
				break;

				case CTriggerData::o_82:
				{
					O_82 *pO82 = (O_82 *)(pOperation->pParam);
					pOP = new op_82(pO82->Val1, pO82->Val2, pO82->Val3, pO82->Val4, pO82->Val5, pO82->Val6, pO82->Val7);
				}
				break;

				case CTriggerData::o_83:
				{
					O_83 *pO83 = (O_83 *)(pOperation->pParam);
					pOP = new op_83(pO83->Val1, pO83->Val2, pO83->Val3, pO83->Val4, pO83->Val5, pO83->Val6, pO83->Val7, pO83->Val8, pO83->Val9, pO83->Val10, pO83->Val11);
				}
				break;

				case CTriggerData::o_84:
				{
					O_84 *pO97 = (O_84 *)(pOperation->pParam);
					pOP = new op_84();
				}
				break;
				case CTriggerData::o_85:
				{
					O_85 *pO85 = (O_85 *)(pOperation->pParam);
					pOP = new op_85(pO85->Val1, pO85->Val2, pO85->Val3, pO85->Val4, pO85->Val5, pO85->Val6, pO85->Val7);
				}
				break;

				case CTriggerData::o_86:
				{
					O_86 *pO86 = (O_86 *)(pOperation->pParam);
					pOP = new op_86(pO86->Val1, pO86->Val2, pO86->Val3);
				}
				break;

				case CTriggerData::o_87:
				{
					O_87 *pO87 = (O_87 *)(pOperation->pParam);
					pOP = new op_87(pO87->Val1, pO87->Val2, pO87->Val3, pO87->Val4, pO87->Val5, pO87->Val6, pO87->Val7, pO87->Val8, pO87->Val9, pO87->Val10, pO87->Val11);
				}
				break;

				case CTriggerData::o_88:
				{
					O_88 *pO88 = (O_88 *)(pOperation->pParam);
					pOP = new op_88(pO88->Val1, pO88->Val2, pO88->Val3);
				}
				break;

				case CTriggerData::o_89:
				{
					O_89 *pO89 = (O_89 *)(pOperation->pParam);
					pOP = new op_89(pO89->Val1, pO89->Val2, pO89->Val3, pO89->Val4, pO89->Val5, pO89->Val6, pO89->Val7, pO89->Val8, pO89->Val9, pO89->Val10);
				}
				break;

				case CTriggerData::o_90:
				{
					O_90 *pO90 = (O_90 *)(pOperation->pParam);
					pOP = new op_90(pO90->Val1, pO90->Val2, pO90->Val3, pO90->Val4);
				}
				break;

				case CTriggerData::o_91:
				{
					O_91 *pO91 = (O_91 *)(pOperation->pParam);
					pOP = new op_91(pO91->Val1, pO91->Val2, pO91->Val3, pO91->Val4, pO91->Val5, pO91->Val6);
				}
				break;

				case CTriggerData::o_92:
				{
					O_92 *pO92 = (O_92 *)(pOperation->pParam);
					pOP = new op_92(pO92->Val1, pO92->Val2, pO92->Val3, pO92->Val4, pO92->Val5, pO92->Val6, pO92->Val7, pO92->Val8, pO92->Val9);
				}
				break;

				case CTriggerData::o_93:
				{
					O_93 *pO93 = (O_93 *)(pOperation->pParam);
					pOP = new op_93(pO93->Val1, pO93->Val2, pO93->Val3, pO93->Val4, pO93->Val5, pO93->Val6, pO93->Val7, pO93->Val8, pO93->Val9, pO93->Val10);
				}
				break;

				case CTriggerData::o_94:
				{
					O_94 *pO94 = (O_94 *)(pOperation->pParam);
					pOP = new op_94(pO94->Val1);
				}
				break;

				case CTriggerData::o_95:
				{
					O_95 *pO95 = (O_95 *)(pOperation->pParam);
					pOP = new op_95(pO95->Val1);
				}
				break;

				case CTriggerData::o_96:
				{
					O_96 *pO96 = (O_96 *)(pOperation->pParam);
					pOP = new op_96(pO96->Val1, pO96->Val2);
				}
				break;

				case CTriggerData::o_97:
				{
					O_97 *pO97 = (O_97 *)(pOperation->pParam);
					pOP = new op_97();
				}
				break;	
				
				case CTriggerData::o_98:
				{
					O_98 *pO98 = (O_98 *)(pOperation->pParam);
					pOP = new op_98(pO98->Val1);
				}
				break;

				case CTriggerData::o_99:
				{
					O_99 *pO99 = (O_99 *)(pOperation->pParam);
					pOP = new op_99(pO99->Val1, pO99->Val2, pO99->Val3, pO99->Val4, pO99->Val5, pO99->Val6, pO99->Val7, pO99->Val8, pO99->Val9, pO99->Val10, pO99->Val11, pO99->Val12, pO99->Val13, pO99->Val14, pO99->Val15, pO99->Val16, pO99->Val17, pO99->Val18, pO99->Val19, pO99->Val20);
				}
				break;

				case CTriggerData::o_100:
				{
					O_100 *pO100 = (O_100 *)(pOperation->pParam);
					pOP = new op_100(pO100->Val1, pO100->Val2);
				}
				break;

				case CTriggerData::o_101:
				{
					O_101 *pO101 = (O_101 *)(pOperation->pParam);
					pOP = new op_101(pO101->Val1, pO101->Val2);
				}
				break;

				
				case CTriggerData::o_102:
				{
					O_102 *pO102 = (O_102 *)(pOperation->pParam);
					pOP = new op_102(pO102->Val1, pO102->Val2);
				}
				break;

				case CTriggerData::o_103:
				{
					O_103 *pO103 = (O_103 *)(pOperation->pParam);
					pOP = new op_103(pO103->Val1, pO103->Val2, pO103->Val3);
				}
				break;

				case CTriggerData::o_104:
				{
					O_104 *pO104 = (O_104 *)(pOperation->pParam);
					pOP = new op_104(pO104->Val1, pO104->Val2, pO104->Val3, pO104->Val4, pO104->Val5, pO104->Val6, pO104->Val7, pO104->Val8, pO104->Val9, pO104->Val10, pO104->Val11, pO104->Val12, pO104->Val13, pO104->Val14, pO104->Val15, pO104->Val16, pO104->Val17, pO104->Val18, pO104->Val19, pO104->Val20, pO104->Val21, pO104->Val22, pO104->Val23, pO104->Val24, pO104->Val25, pO104->Val26);
				}
				break;

				case CTriggerData::o_105:
				{
					O_105 *pO105 = (O_105 *)(pOperation->pParam);
					pOP = new op_105(pO105->Val1, pO105->Val2, pO105->Val3, pO105->Val4, pO105->Val5);
				}
				break;
							

				default: //...
					printf("[policy_loader][ConvertOperation] unknown operation type %d\n", pOperation->iType);
					ASSERT(false);
					break;
		}
		
		if(pOP && pOP->RequireTarget())
		{
			pOP->SetTarget(tar);
		}
		else
		{
			delete tar;
		}
		return pOP;
		#undef OPARAM
	}

	trigger * ConvertTrigger(CPolicyData *pPolicyData, CTriggerData *pTriggerData)
	{
		CTriggerData::_s_tree_item* root = pTriggerData->GetConditonRoot();
		condition * cond = ConvertCondition(root);

		trigger * pTri = new trigger();
		pTri->SetData(pTriggerData->GetID(),cond);

		int numOperation = pTriggerData->GetOperaionNum();
		for( int j = 0; j < numOperation; ++j)
		{
			CTriggerData::_s_operation *pOperation = pTriggerData->GetOperaion(j);
			operation *pOP = ConvertOperation(pPolicyData,pOperation);
			if (!pOP) continue;
			pTri->AddOp(pOP);
		}
		return pTri;
	}
}

bool LoadAIPolicy(const char * path)
{
	CPolicyDataManager man;
	
	if(!man.Load(path))
	{
		printf("Failed to Load AIPolicy '%s'\n",path);
		return false;
	}
	
	int numPolicy = man.GetPolicyNum();
	for(int i = 0; i < numPolicy; i ++)
	{
		CPolicyData *pPolicyData = man.GetPolicy(i);
		policy * pPolicy = new policy(pPolicyData->GetID());

		//�������������
		int numTrigger = pPolicyData->GetTriggerPtrNum();
		for(int j = 0; j < numTrigger; j ++)
		{
			CTriggerData *pTriggerData = pPolicyData->GetTriggerPtr(j);
			if(pTriggerData->IsRun()) continue;
			trigger * pTrigger = ConvertTrigger(pPolicyData,pTriggerData);
			pTrigger->SetDefaultEnable(pTriggerData->IsActive());
			pTrigger->SetBattleEnable(pTriggerData->OnlyAttackValid());
			pPolicy->AddTrigger(pTrigger);
		}

		//�����Լ��뵽��������
		world_manager::GetTriggerMan().AddPolicy(pPolicy);
		//printf("�������%d\n",pPolicyData->GetID());
	}
	man.Release();
	return true;
}

