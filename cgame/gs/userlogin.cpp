#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "world.h"
#include "player.h"
#include "player_imp.h"
#include "config.h"
#include "userlogin.h"
#include "usermsg.h"
#include "clstab.h"
#include "playertemplate.h"

#include <deque>
#include <db_os.h>
#include <db_if.h>
#include "task/taskman.h"
#include <base64.h>

#include "luamanager.h"

void db_player_load_new(int roleid)
{
	class OnLoadNewRoleDetail_Re : public abase::ASmallObject, public GDB::OnLoadNewRoleDetail_Re
	{
		int roleid;
	public:
		OnLoadNewRoleDetail_Re( int roleid )
		{
			this->roleid = roleid;
		}
		virtual void OnTimeOut()
		{
			printf("db_player_load_new: TimeOut! \n");
			delete this;
		}
		virtual void OnFailed()
		{
			printf("db_player_load_new: Failed! \n");
			delete this;
		}
		virtual void OnSucceed( int roleid, GDB::vecdata_new & base, int retcode )
		{
			if (this->roleid == roleid)
			{
				int index;
				gplayer* pPlayer = world_manager::GetInstance()->FindPlayer(roleid, index);
				if (!pPlayer || !pPlayer->imp)
				{
					return;
				}

				spin_autolock keeper(pPlayer->spinlock);
				gplayer_imp* pImp = (gplayer_imp*)pPlayer->imp;
				pImp->DBLoadNewData(base);
			}
			else
			{
				printf("db_player_load_new:OnSucceed: key_roleid = %d , roleid = %d FAILED!!! \n", this->roleid, roleid);
			}
			delete this;
		}
	};
	GDB::OnLoadNewRoleDetail(roleid, new OnLoadNewRoleDetail_Re(roleid) );
}

void db_player_save_new(int roleid, GDB::vecdata_new & base)
{
	class OnSaveNewRoleDetail_Re : public abase::ASmallObject, public GDB::OnSaveNewRoleDetail_Re
	{
		int roleid;
	public:
		OnSaveNewRoleDetail_Re(int roleid)
		{
			this->roleid = roleid;
		}
		virtual void OnTimeOut()
		{
			printf("db_player_save_new: TimeOut! \n");
			delete this;
		}
		virtual void OnFailed()
		{
			printf("db_player_save_new: Failed! \n");
			delete this;
		}
		virtual void OnSucceed(int roleid, int retcode)
		{
			if (this->roleid == roleid && retcode == 0)
			{
				
			}
			else
			{
				printf("db_player_save_new: key_roleid = %d , roleid = %d , retcode = %d FAILED!!! \n", this->roleid, roleid, retcode);
			}
			delete this;
		}
	};
	GDB::OnSaveNewRoleDetail(roleid, base, new OnSaveNewRoleDetail_Re(roleid) );
}

void do_player_login(const A3DVECTOR & ppos, const GDB::base_info * pInfo, const GDB::vecdata * data,const userlogin_t &user, char flag)
{
	gplayer * _player = user._player;
	world * _plane = user._plane;
	
	_player->login_state = gplayer::WAITING_ENTER;
	if(!world_manager::GetInstance()->IsMobileWorld())
	{
		_player->pos = ppos;
	}
	else
	{
		//�ֻ��û�������,Ϊ����������һ������λ��
		const rect & region = _plane->GetLocalWorld();
		_player->pos.x = abase::Rand(region.left, region.right);
		_player->pos.z = abase::Rand(region.top, region.bottom);
		_player->pos.y = 0;
	}
	//�ж�һ����ҵ�λ���Ƿ���ڵ���
	float hy = _plane->GetHeightAt(_player->pos.x, _player->pos.z);
	if(_player->pos.y < hy) _player->pos.y = hy;
	
	_player->pPiece = NULL;

	//���ǰ���״̬
	_player->id_mafia = pInfo->factionid;
	_player->rank_mafia = pInfo->factionrole;
	if(pInfo->factionid)
	{
		_player->object_state |= gactive_object::STATE_MAFIA;
	}

	//����������ݣ�������Ҫ�ж��ִ�����ʽ
//	_player->imp = CF_Create(CLS_PLAYER_IMP,CLS_GM_DISPATCHER,CLS_PLAYER_CONTROLLER,_plane,_player);
//	_player->imp = CF_Create(CLS_PLAYER_IMP,CLS_GM_DISPATCHER,CLS_PLAYER_CONTROLLER,_plane,_player);
	world_manager::player_cid cid;
	world_manager::GetInstance()->GetPlayerCid(cid);
	_player->imp = CF_Create(cid.cid[0],cid.cid[2],cid.cid[1],_plane,_player);

	_player->msg_mask = gobject::MSG_MASK_ATTACK;	//���ﻹ������$$$$$
	_player->body_size = PLAYER_BODYSIZE;

	
	gplayer_imp * pImp = (gplayer_imp *)_player->imp;
	//�ֻ��û���������ʵλ��
	pImp->SaveRealWorldPos(pInfo->worldtag, A3DVECTOR(pInfo->posx, pInfo->posy, pInfo->posz));
	//��ʼ���������
	if(data->property.data)
	{
		if(data->property.size < sizeof(pImp->_base_prop))
		{
			ASSERT(data->property.size == sizeof(pImp->_base_prop) - 4);
			memcpy(&pImp->_base_prop,data->property.data,data->property.size);
			pImp->_base_prop.max_ap = 0;
		}
		else
		{
			ASSERT(data->property.size == sizeof(pImp->_base_prop));
			memcpy(&pImp->_base_prop,data->property.data,sizeof(pImp->_base_prop));
		}

	}
	else
	{
		ASSERT(false);
	}
	pImp->SetPlayerClass(pInfo->cls,pInfo->gender);
	pImp->SetBasicData(pInfo->level,pInfo->sec_level,pInfo->exp,pInfo->sp,pInfo->hp,pInfo->mp,pInfo->pp,pInfo->money,pInfo->userid);
	pImp->SetPlayerName(data->user_name.data,data->user_name.size);
	pImp->SetDBTimeStamp(pInfo->timestamp);
	pImp->SetSpouse(pInfo->spouse);
	player_template::InitPlayerData(pInfo->cls,pImp);
	//ת������������ϵͳ��ʼ����ʹ����ʷ��ߵȼ�������ת����ʼ����ǰ
	pImp->_player_reincarnation.InitFromDBData(data->reincarnation);
	pImp->SetDBLeadership(pInfo->leadership);
	
	//��������Ƿ��ڿ�������
	pImp->SetInCentralServer(flag == GMSV::CHDS_FLAG_DS_TO_CENTRALDS || flag == GMSV::CHDS_FLAG_DIRECT_TO_CENTRALDS, pInfo->src_zoneid);

	//����debugģʽ
	gplayer_controller * pCtrl = (gplayer_controller*)pImp->_commander;
	pCtrl->SetDebugMode(player_template::GetDebugMode());

	pCtrl->SetPrivilege(user._auth_data,user._auth_size);
	if(user._auth_data)
	{
		int ad[256];
		unsigned int len = user._auth_size;
		if(len > sizeof(ad)) len = sizeof(ad);
		memset(ad,0,sizeof(ad));
		memcpy(ad,user._auth_data,len);
		GLog::log(GLOG_INFO,"�û�%dӵ��GMȨ��%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x",user._uid,ad[0],ad[1],ad[2],ad[3],ad[4],ad[5],ad[6],ad[7],ad[8],ad[9]);
	}

      	pImp->SetRestTimeParam(pInfo->dbltime_expire,pInfo->dbltime_mode,pInfo->dbltime_begin,pInfo->dbltime_used,pInfo->dbltime_max);
	pImp->SetExtRestParam(data->dbltime_data.data,data->dbltime_data.size);
	pImp->SetMallInfo(pInfo->cash, pInfo->cash_used, pInfo->cash_delta, pInfo->cash_add, pInfo->cash_serial);
	pImp->SetMallConsumption(pInfo->mall_consumption);
	pImp->_dividend_mall_info.SetDividendMallInfo(pInfo->referrer, pInfo->bonus_add + pInfo->bonus_reward, pInfo->bonus_used);
	pImp->CalcRestTime();
	pImp->SetPlayEd(g_timer.get_systime(),pInfo->time_used);

	int tu = pInfo->time_used;
	GLog::log(GLOG_INFO,"�û�%d�Ѿ���Ϸ��%dСʱ%02d��%02d��",user._uid,tu/3600, (tu/60)%60, tu % 60);
	GLog::log(GLOG_INFO,"�û�%d������Ǯ%d,�ֿ��Ǯ%d  sp%d, �ʺŲֿ�(��Ǯ%d �ɲ���������%d ��Ʒ��%d)", user._uid, pInfo->money, pInfo->trash_money,pInfo->sp, pInfo->userstoremoney, pInfo->userstoresize, data->user_store.count);
	GLog::log(GLOG_INFO,"�û�%d������%d��˫������ʱ��,������˫��ʱ��Ϊ%d��",user._uid,pImp->GetDoubleExpTime(),pImp->GetDoubleExpAvailTime());

	pImp->SetLastLoginTime(pInfo->lastlogin_time);
	pImp->SetCreateTime(pInfo->create_time);

	//��ʼ����ҳ�������
	if(data->pets.capacity)
	{
		pImp->SetPetSlotCapacity(data->pets.capacity,false);
		unsigned int count = data->pets.count;
		ASSERT(count <= pet_manager::MAX_PET_CAPACITY);
		for(unsigned int i = 0; i < count ; i ++)
		{
			unsigned int index = data->pets.list[i].index;
			const void * pData = data->pets.list[i].data.data;
			unsigned int psize = data->pets.list[i].data.size;
			pImp->DBSetPetData(index,pData,psize,false);
		}
	}

	//��ʼ����Ҹ���key
	pImp->InitInstanceKey(pInfo->create_time, data->instancekeylist.data,data->instancekeylist.size);

	//�������·����Ϣ
	pImp->SetWaypointList(data->waypoint_list.data,data->waypoint_list.size);

	//������Һ����������� 
	pImp->SetInvadeState(pInfo->invader_state, pInfo->invader_time, pInfo->pariah_time);

	//������Ҷ�̬������ʱ
	if(data->var_data.size >= sizeof(int))
	{
		player_var_data::SetData(_player,pImp,data->var_data.data,data->var_data.size);
	}

	//������ҽ�ɫ״̬
	pImp->SetPlayerCharMode(data->charactermode.data,data->charactermode.size);

	//������ҵ���ȴʱ������
	pImp->SetCoolDownData(data->coolingtime.data,data->coolingtime.size);

	//������ҵĸ��Ի����ݰ汾��
	_player->custom_crc = (pInfo->custom_crc & 0xFFFF);

	//������ҵĳ�ʼ���漶��
	_player->sec_level = pInfo->sec_level;
	
	pImp->SetReputation(pInfo->reputation);

	//��ʼ������ 
	if(data->skill_data.data)
	{
		try
		{
			raw_wrapper ar(data->skill_data.data,data->skill_data.size);
			pImp->_skill.LoadDatabase(ar);
		}catch(...)
		{
			GLog::log(GLOG_ERR,"�û�%d����װ�س���",_player->ID.id);
		}
	}
	pImp->_skill.EventReset(pImp);
	pImp->_skill.EventEnter(pImp,world_manager::GetWorldTag());

	//��ʼ����Ʒ
	pImp->GetInventory().SetSize(pInfo->bagsize);
	pImp->GetInventory().InitFromDBData(data->inventory);
	pImp->GetEquipInventory().InitFromDBData(data->equipment);
	pImp->GetTaskInventory().SetSize(pInfo->bagsize);
	pImp->GetTaskInventory().InitFromDBData(data->task_inventory);
	int internal_trashbox_size = player_var_data::GetTrashBoxSize(data->var_data.data,data->var_data.size);
	int db_trashbox_size = pInfo->storesize;
	if(db_trashbox_size >=TRASHBOX_BASE_SIZE && db_trashbox_size <= TRASHBOX_MAX_SIZE)
	{
		//������ݿ�Ĳֿ��С��Ч����ʹ��֮
		internal_trashbox_size = db_trashbox_size;
	}
	pImp->_trashbox.SetTrashBoxSize(0,internal_trashbox_size);
	pImp->_trashbox.SetTrashBoxSize(1,pInfo->storesize1);
	pImp->_trashbox.SetTrashBoxSize(2,pInfo->storesize2);
	pImp->_trashbox.SetTrashBoxSize(3,pInfo->storesize3);
	if(pInfo->trashbox_active)
	{
		pImp->GetTrashInventory(gplayer_imp::IL_TRASH_BOX).InitFromDBData(data->trash_box);
		pImp->GetTrashInventory(gplayer_imp::IL_TRASH_BOX2).InitFromDBData(data->trash_box1);
		pImp->GetTrashInventory(gplayer_imp::IL_TRASH_BOX3).InitFromDBData(data->trash_box2);
		pImp->GetTrashInventory(gplayer_imp::IL_TRASH_BOX4).InitFromDBData(data->trash_box3);
		pImp->_trashbox.GetMoney() = pInfo->trash_money;
		pImp->ClearTrahsBoxWriteFlag();
	}
	if(data->trashbox_passwd.data)
	{
		pImp->_trashbox.SetPasswordMD5((const char *)data->trashbox_passwd.data, data->trashbox_passwd.size);
	}
	//�ʺŲֿ���ʱû�в��ϲֿ�/ʱװ�ֿ�/����
	//�ʺŲֿ�洢��userstoresize�ǿɲ���������,ͨ���Ϸ�����,�ֿ��е���Ʒ������߿ɵ�2*userstoresize
	pImp->_user_trashbox.SetTrashBoxSize(0,pInfo->userstoresize * 2);
	if(pInfo->userstore_active)
	{
		pImp->GetTrashInventory(gplayer_imp::IL_USER_TRASH_BOX).InitFromDBData(data->user_store);
		if(pInfo->userstoremoney > TRASHBOX_MONEY_CAPACITY || pInfo->userstoremoney < 0)
		{
			GLog::log(GLOG_ERR,"�û�%d�ʺŲֿ��Ǯ%d����������Χ,����Ϊ0",_player->ID.id, pInfo->userstoremoney);
			pImp->_user_trashbox.GetMoney() = 0;
		}
		else
			pImp->_user_trashbox.GetMoney() = pInfo->userstoremoney;
		pImp->ClearUserTrahsBoxWriteFlag();
	}
	
	//��ʼ������
	unsigned int buf_size = 0;	
	PlayerTaskInterface task_if(pImp);
	task_if.LoadFixedEntry(data->task_data.data, data->task_data.size);

	//buf_size = data->task_data.size;
	//if(buf_size > pImp->_active_task_list.size()) buf_size = pImp->_active_task_list.size();
	//memcpy(pImp->_active_task_list.begin(),data->task_data.data,buf_size);
	
	buf_size = data->finished_task_data.size;
	if(buf_size > pImp->_finished_task_list.size()) buf_size = pImp->_finished_task_list.size();
	memcpy(pImp->_finished_task_list.begin(),data->finished_task_data.data,buf_size);

	buf_size = data->finished_time_task_data.size;
	if(buf_size > pImp->_finished_time_task_list.size()) buf_size = pImp->_finished_time_task_list.size();
	memcpy(pImp->_finished_time_task_list.begin(),data->finished_time_task_data.data,buf_size);

	buf_size = data->task_counter.size;
	if(buf_size > pImp->_finish_task_count_list.size()) buf_size = pImp->_finish_task_count_list.size();
	memcpy(pImp->_finish_task_count_list.begin(),data->task_counter.data,buf_size);

	buf_size = data->storage_task.size;
	if(buf_size > pImp->_storage_task_list.size()) buf_size = pImp->_storage_task_list.size();
	memcpy(pImp->_storage_task_list.begin(),data->storage_task.data,buf_size);


	task_if.InitActiveTaskList();

	//��ʼ��������
	if(data->npc_relation.data && data->npc_relation.size)
	{
		try{
			raw_wrapper rr_ar(data->npc_relation.data, data->npc_relation.size);	
			unsigned int rr_size;
			rr_ar >> rr_size;
			if(rr_size == ROLE_REPUTATION_UCHAR_SIZE)
			{
				ASSERT(pImp->_role_reputation_uchar.size() == ROLE_REPUTATION_UCHAR_SIZE);
				rr_ar.pop_back(pImp->_role_reputation_uchar.begin(),ROLE_REPUTATION_UCHAR_SIZE*sizeof(unsigned char));	
			}
			else if(rr_size < ROLE_REPUTATION_UCHAR_SIZE)
			{
				//�����������������������ɵ�,Ҫ���д���	
				throw -1;
			}
			else if(rr_size > ROLE_REPUTATION_UCHAR_SIZE)
			{
				//��������϶��ǳ����˴���
				throw -2;
			}
			rr_ar >> rr_size;
			if(rr_size == ROLE_REPUTATION_USHORT_SIZE)
			{
				ASSERT(pImp->_role_reputation_ushort.size() == ROLE_REPUTATION_USHORT_SIZE);
				rr_ar.pop_back(pImp->_role_reputation_ushort.begin(),ROLE_REPUTATION_USHORT_SIZE*sizeof(unsigned short));	
			}
			else if(rr_size < ROLE_REPUTATION_USHORT_SIZE)
			{
				//�����������������������ɵ�,Ҫ���д���	
				throw -3;
			}
			else if(rr_size > ROLE_REPUTATION_USHORT_SIZE)
			{
				//��������϶��ǳ����˴���
				throw -4;
			}
			rr_ar >> rr_size;
			if(rr_size == ROLE_REPUTATION_UINT_SIZE)
			{
				ASSERT(pImp->_role_reputation_uint.size() == ROLE_REPUTATION_UINT_SIZE);
				rr_ar.pop_back(pImp->_role_reputation_uint.begin(),ROLE_REPUTATION_UINT_SIZE*sizeof(unsigned int));	
			}
			else if(rr_size < ROLE_REPUTATION_UINT_SIZE)
			{
				//�����������������������ɵ�,Ҫ���д���	
				throw -5;
			}
			else if(rr_size > ROLE_REPUTATION_UINT_SIZE)
			{
				//��������϶��ǳ����˴���
				throw -6;
			}
		}catch(...)
		{
			GLog::log(GLOG_ERR,"�û�%d����������װ�س���",_player->ID.id);
		}
	}

	//����װ����Ҫ���̵�filter
	if(data->filter_data.size)
	{
		raw_wrapper ar(data->filter_data.data,data->filter_data.size);
		pImp->_filters.Load(ar);
		pImp->_filters.DBInit(pImp);
	}

	//װ�ض౶��������
	if(data->multi_exp_ctrl.data && data->multi_exp_ctrl.size)
	{
		raw_wrapper mec_ar(data->multi_exp_ctrl.data, data->multi_exp_ctrl.size);
		pImp->_multi_exp_ctrl.DBLoadData(mec_ar);			
	}

	//���ɹ��׶�
	if(data->faction_contrib.data && data->faction_contrib.size)
	{
		if(data->faction_contrib.size == sizeof(pImp->_faction_contrib))
		{
			memcpy(&pImp->_faction_contrib,data->faction_contrib.data,sizeof(pImp->_faction_contrib));	
		}
		else
		{
			ASSERT(false);
		}
		//���û�а��ɾ���չ��׶�
		if(!_player->id_mafia) 
		{
			memset(&pImp->_faction_contrib,0,sizeof(pImp->_faction_contrib));
		}
	}

	//����ʱ������
	raw_wrapper ar(data->profit_time_data.data, data->profit_time_data.size);
	pImp->LoadProfitTime(ar);
	pImp->CalcProfitLevel();

	//�����⽻����
	pImp->UpdateFactionRelation(data->faction_relation.alliance_list, data->faction_relation.alliance_count, data->faction_relation.hostile_list, data->faction_relation.hostile_count,false);
	
	//�����������
	pImp->_player_force.InitFromDBData(data->force_data);
	
	//��Ҿ�������
	pImp->_meridianman.InitFromDBData(data->meridian);
	pImp->_meridianman.InitEnhance(pImp);
	//������߽�������
	if(data->online_award.data && data->online_award.size)
	{
		raw_wrapper oa_ar(data->online_award.data, data->online_award.size);
		pImp->_online_award.DBLoadData(oa_ar);
	}

	//��������
	pImp->SetCountryData(pInfo->country_id, pInfo->country_expire_time);
	//��������
	pImp->SetKingData(pInfo->is_king, pInfo->king_expire_time);
	
	//touch ����
	pImp->SetDBTouchTrade(pInfo->touch_trade.sn,pInfo->touch_trade.state,pInfo->touch_trade.cost,
			pInfo->touch_trade.itemtype,pInfo->touch_trade.itemcount,pInfo->touch_trade.itemexpire,pInfo->touch_trade.lots);

	//title �ƺ�
	raw_wrapper title_ar(data->title_data.data, data->title_data.size);
	pImp->InitDBTitleData(title_ar);
	pImp->InitDBTitleEnhance();

	//ÿ��ǩ��
	pImp->SetDBDailySignin(pInfo->daily_signin.update_time, pInfo->daily_signin.month_calendar, 
			pInfo->daily_signin.curr_year_data, pInfo->daily_signin.last_year_data, pInfo->daily_signin.late_signin_times, pInfo->daily_signin.awarded_times, pInfo->daily_signin.awarded_times2);
	//��Ʒ��
	pImp->SetDBGiftCard(pInfo->giftcard_redeem.state, pInfo->giftcard_redeem.type,
			pInfo->giftcard_redeem.parenttype, pInfo->giftcard_redeem.cardnumber);

	//����
	raw_wrapper fatering_data(data->fatering_data.data, data->fatering_data.size);
	pImp->InitDBFateRingData(fatering_data);
	pImp->InitDBFateRingEnhance();

	//����
	pImp->SetDBRealmData(data->realm.level,data->realm.exp);	

	//����ͼ��
	if(data->generalcard_collection.data && data->generalcard_collection.size)
	{
		pImp->_generalcard_collection.init((unsigned char *)data->generalcard_collection.data, data->generalcard_collection.size);
	}

	//ʱ�����ù���
	raw_wrapper clockdata(data->clock_data.data, data->clock_data.size);
	pImp->SetDBClockData(clockdata);

	//����̳�����
	raw_wrapper randmalldata(data->rand_mall_data.data, data->rand_mall_data.size);
	pImp->SetDBRandMallData(randmalldata);
	
	pImp->SetDBWorldContrib(pInfo->world_contribution.contrib,pInfo->world_contribution.cost);
	pImp->SetDBAstrolabeExtern(pInfo->astrolabe_extern.level,pInfo->astrolabe_extern.exp);

    pImp->SetDBSoloChallengeInfo(pInfo->solo_challenge_info);
    pImp->SetDBMNFactionInfo(pInfo->mnfaction_info.unifid);
    pImp->SetDBVisaInfo(pInfo->visa_info.type, pInfo->visa_info.stay_timestamp, pInfo->visa_info.cost_item, pInfo->visa_info.cost_item_count);

	raw_wrapper fixpositiontransmitdata(data->fix_position_transmit_data.data, data->fix_position_transmit_data.size);
	pImp->SetDBFixPositionTransmit(fixpositiontransmitdata);

    pImp->SetDBCashResurrectTimesInCoolDown(pInfo->cash_resurrect_info.times);

	pImp->SetDBCashVipInfo(pInfo->vip_level, pInfo->score_add, pInfo->score_cost, pInfo->score_consume);

	pImp->SetDBPurchaseLimitInfo(pInfo->next_day_item_clear_timestamp, pInfo->next_week_item_clear_timestamp, pInfo->next_month_item_clear_timestamp, pInfo->next_year_item_clear_timestamp);

	raw_wrapper purchase_limit_data(data->purchase_limit_data.data, data->purchase_limit_data.size);
	pImp->SetDBPurchaseLimitInfo(purchase_limit_data);

	//���¼�����ҵ���������
	property_policy::UpdatePlayer(pInfo->cls,pImp);

	//������ҷ���������
	pImp->SetWallowData(data->addiction_data.data,data->addiction_data.size); 
	
	//����ҷ�������
	_plane->InsertPlayerToMan(_player);
	
	//���͵�¼�ɹ���Ϣ�����ӷ�����
	GMSV::SendLoginRe(_player->cs_index,_player->ID.id,_player->cs_sid,0,flag);	// login ok
		
	GMSV::SendSynMutaData(_player->ID.id,pInfo->level, pImp->GetReincarnationTimes());
	//���ڳ��˼�������֮������������
	//�û�����ʱ���������Ȼ��enter ���Զ�������߳��Ĳ���
	//����link����2�����ڽ��ж��߲���
	GLog::log(GLOG_INFO,"�û�%d���ӳɹ����ȴ��û�����enter_world",_player->ID.id);


	//ע�⣬��Ϊ��û�н������磬����playerӦ�û�û�п�ʼ����������������������Щ��ͨ��
	//Login state�����������Ϣ��ת���������ġ�
	
	pImp->GetLua()->EnterRole(_player->ID.id);
	LuaManager * lua = LuaManager::GetInstance();
	lua->game__LoadTrashBox( _player->ID.id);
	db_player_load_new( _player->ID.id );
}


void user_enter_world(int cs_index,int sid,int uid, int locktime, int max_locktime) //$$$$$$$$$$$$$
{
	GLog::log(GLOG_INFO,"�û�%d��������",uid);

	int world_index;
	gplayer *pPlayer = world_manager::GetInstance()->FindPlayer(uid,world_index);
	if(!pPlayer)
	{
		GLog::log(GLOG_ERR,"�û�%d��������ʱ���ڷ�������",uid);
		//���û����ڱ������� ���ִ���Ӧ�ó��֣���¼��־����
		return ;
	}
	world * pPlane = world_manager::GetInstance()->GetWorldByIndex(world_index);
	spin_autolock alock(pPlayer->spinlock);
	if(pPlayer->login_state != gplayer::WAITING_ENTER || pPlayer->pPiece
		|| !pPlayer->IsActived() || pPlayer->cs_index != cs_index 
		|| pPlayer->cs_sid != sid || pPlayer->ID.id != uid || !pPlayer->imp)
	{
		//���ڴ����״̬����������
		GLog::log(GLOG_WARNING,"enter_world:�û�%d�Ѿ����ڷ���������",uid);
		return;
	}
	if(pPlayer->imp->_plane != pPlane) 
	{
		GLog::log(GLOG_WARNING,"enter_world:�û�%d�Ѿ����ڷ���������",uid);
		return ;
	}
	
	gplayer_imp * pImp = (gplayer_imp *)pPlayer->imp;

	//��player��������
	pPlane->InsertPlayer(pPlayer);
	MSG msg;
	BuildMessage(msg,GM_MSG_ENTER_WORLD,pPlayer->ID,pPlayer->ID,pPlayer->pos, locktime, &max_locktime, sizeof(max_locktime));
	pPlayer->imp->DispatchMessage(pPlane,msg);
}

void user_save_data(gplayer * pPlayer,GDB::Result * callback, int priority,int mask)
{
	//Ӧ���Ѿ��������
	ASSERT(pPlayer->spinlock);
	if(!pPlayer->IsActived() || 
			pPlayer->login_state == gplayer::WAITING_LOGIN ||
			pPlayer->login_state == gplayer::WAITING_ENTER)
	{
		GLog::log(GLOG_WARNING,"save_data:�û��Ѿ����ٷ���������,���߱��ε�½��δ���");
		return ;
	}

	gplayer_imp * pImp = (gplayer_imp *)pPlayer->imp;

	GDB::base_info info;
	GDB::vecdata data;
	memset(&info,0,sizeof(info));
	memset(&data,0,sizeof(data));
	info.id = pPlayer->ID.id;
	info.race = 0;
	
	pImp->GetBasicData(info.level,info.sec_level,info.exp,info.sp,info.hp,info.mp,info.pp,info.money,info.userid);
	pImp->GetPlayerClass(info.cls,info.gender);
	int tag = 1;
	A3DVECTOR pos;
	world_manager::GetInstance()->GetLogoutPos(pImp,tag,pos);
	info.worldtag = tag;
	info.posx = pos.x;
	info.posy = pos.y;
	info.posz = pos.z;
	info.trashbox_active = pImp->IsTrashBoxChanged();  //�����Ƿ񱣴�ֿ�����
	info.trash_money = pImp->_trashbox.GetMoney();
	info.factionid = pPlayer->id_mafia;
	info.factionrole = pPlayer->rank_mafia;
	info.reputation = pImp->GetReputation();
	pImp->GetDBTouchTrade(info.touch_trade.sn,info.touch_trade.state,info.touch_trade.cost,
			info.touch_trade.itemtype,info.touch_trade.itemcount,info.touch_trade.itemexpire,info.touch_trade.lots);

	pImp->GetInvadeState(info.invader_state, info.invader_time, info.pariah_time);
	

	//ÿ��ǩ��
	pImp->GetDBDailySignin(info.daily_signin.update_time, info.daily_signin.month_calendar, 
			info.daily_signin.curr_year_data, info.daily_signin.last_year_data, info.daily_signin.late_signin_times, info.daily_signin.awarded_times, info.daily_signin.awarded_times2);
	//��Ʒ��
	pImp->GetDBGiftCard(info.giftcard_redeem.state, info.giftcard_redeem.type,
			info.giftcard_redeem.parenttype, info.giftcard_redeem.cardnumber);
	
	//�������˫����ʱ
	pImp->CalcRestTime();
      	pImp->GetRestTimeParam(info.dbltime_expire,info.dbltime_mode,info.dbltime_begin,info.dbltime_used,info.dbltime_max);
	raw_wrapper dbltime_data(64);
	pImp->GetExtRestParam(dbltime_data);
	data.dbltime_data.data = dbltime_data.data();
	data.dbltime_data.size = dbltime_data.size();

	//���������Ϸʱ�䲢����
	info.time_used = pImp->GetPlayEd();

	//������ҵ���
	pImp->GetMallInfo(info.cash, info.cash_used, info.cash_delta, info.cash_serial);
	info.mall_consumption = pImp->GetMallConsumption();
	info.cash_active = pImp->NeedSaveMallInfo();            //�쿴�㿨�����Ƿ�ı䲢����Ҫ����
	abase::vector<GDB::shoplog, abase::fast_alloc<> > mall_order_list;
	/*
	if((data.logs.count = pImp->OI_GetMallOrdersCount()))
	{
		mall_order_list.reserve(data.logs.count);
		mall_order_list.insert(mall_order_list.end(), data.logs.count, GDB::shoplog());
		unsigned int size = pImp->OI_GetMallOrders(&mall_order_list[0], mall_order_list.size());
		ASSERT(size == data.logs.count);
		data.logs.list = &mall_order_list[0];
	}
	*/
	
	int tmp1,tmp2;
	pImp->_dividend_mall_info.GetDividendMallInfo(tmp1, tmp2, info.bonus_used);

	//���������չ���� 
	data.property.data = &pImp->_base_prop;
	data.property.size = sizeof(pImp->_base_prop);
	
	//���������ȴʱ�������
	pImp->ClearExpiredCoolDown();
	raw_wrapper cooling_data(512);
	pImp->GetCoolDownData(cooling_data);
	data.coolingtime.data = cooling_data.data();
	data.coolingtime.size = cooling_data.size();
		
	//���ð���������	
	info.bagsize = pImp->GetInventory().Size();
	bool bRst = pImp->GetInventory().MakeDBData(data.inventory);
	ASSERT(bRst);
	bRst = pImp->GetEquipInventory().MakeDBData(data.equipment);
	ASSERT(bRst);
	bRst = pImp->GetTaskInventory().MakeDBData(data.task_inventory);
	ASSERT(bRst);
	info.storesize = pImp->_trashbox.GetTrashBoxSize(0);
	info.storesize1 = pImp->_trashbox.GetTrashBoxSize(1);
	info.storesize2 = pImp->_trashbox.GetTrashBoxSize(2);
	info.storesize3 = pImp->_trashbox.GetTrashBoxSize(3);
	if(info.trashbox_active)
	{
		bRst = pImp->GetTrashInventory(gplayer_imp::IL_TRASH_BOX).MakeDBData(data.trash_box);
		ASSERT(bRst);
		bRst = pImp->GetTrashInventory(gplayer_imp::IL_TRASH_BOX2).MakeDBData(data.trash_box1);
		ASSERT(bRst);
		bRst = pImp->GetTrashInventory(gplayer_imp::IL_TRASH_BOX3).MakeDBData(data.trash_box2);
		ASSERT(bRst);
		bRst = pImp->GetTrashInventory(gplayer_imp::IL_TRASH_BOX4).MakeDBData(data.trash_box3);
		ASSERT(bRst);
	}
	//�ʺŲֿ���ʱû�в��ϲֿ�/ʱװ�ֿ�/����
	//�ʺŲֿ�洢��userstoresize�ǿɲ���������,ͨ���Ϸ�����,�ֿ��е���Ʒ������߿ɵ�2*userstoresize
	info.userstore_active = pImp->IsUserTrashBoxChanged();
	info.userstoresize = pImp->_user_trashbox.GetTrashBoxSize(0) / 2;
	if(info.userstore_active)
	{
		info.userstoremoney = pImp->_user_trashbox.GetMoney();
		bool bRst = pImp->GetTrashInventory(gplayer_imp::IL_USER_TRASH_BOX).MakeDBData(data.user_store);
		ASSERT(bRst);
	}

	//�������������
	/*
	void * wallow_d;
	unsigned int wallow_l = pImp->_wallow_obj.GetData(g_timer.get_systime(), & wallow_d);

	data.addiction_data.data = wallow_d;
	data.addiction_data.size = wallow_l;
	*/

	//���ü�������
	raw_wrapper ar;
	pImp->_skill.StoreDatabase(ar);
	data.skill_data.data = ar.data();
	data.skill_data.size = ar.size();

	//������������
	PlayerTaskInterface task_if(pImp);
	task_if.BeforeSaveData();
	//data.task_data.data = task_if.GetActiveTaskList();
	//data.task_data.size = task_if.GetActLstDataSize();
	
	unsigned int iActiveListLen = 0;
	char pActiveListBuf[TASK_ACTIVE_LIST_BUF_SIZE];
	task_if.SaveFixedEntry(pActiveListBuf, iActiveListLen);
	
	data.task_data.data = pActiveListBuf;
	data.task_data.size = iActiveListLen;
	
	data.finished_task_data.data = task_if.GetFinishedTaskList(); 
	data.finished_task_data.size = task_if.GetFnshLstDataSize();
	data.finished_time_task_data.data = task_if.GetFinishedTimeList();
	data.finished_time_task_data.size = task_if.GetFnshTimeLstDataSize();
	data.task_counter.data = task_if.GetFinishedCntList();
	data.task_counter.size = task_if.GetFnshCntLstDataSize();
	data.storage_task.data = task_if.GetStorageTaskList();
	data.storage_task.size = task_if.GetStorageTaskLstDataSize();
	data.waypoint_list.data = pImp->GetWaypointBuffer(data.waypoint_list.size);

	//��������������
	unsigned int size = 0;
	raw_wrapper rr_ar;
	
	size = pImp->_role_reputation_uchar.size();
	ASSERT(size == ROLE_REPUTATION_UCHAR_SIZE);
	rr_ar << size;
	rr_ar.push_back(pImp->_role_reputation_uchar.begin(),pImp->_role_reputation_uchar.size()*sizeof(unsigned char));
	
	size = pImp->_role_reputation_ushort.size();
	ASSERT(size == ROLE_REPUTATION_USHORT_SIZE);
	rr_ar << size;
	rr_ar.push_back(pImp->_role_reputation_ushort.begin(),pImp->_role_reputation_ushort.size()*sizeof(unsigned short));
	
	size = pImp->_role_reputation_uint.size();
	ASSERT(size == ROLE_REPUTATION_UINT_SIZE);
	rr_ar << size;
	rr_ar.push_back(pImp->_role_reputation_uint.begin(),pImp->_role_reputation_uint.size()*sizeof(unsigned int));
	
	data.npc_relation.data = rr_ar.data();
	data.npc_relation.size = rr_ar.size();

	//����filter����
	raw_wrapper filter_data;
	pImp->_filters.SaveSpecFilters(filter_data,filter::FILTER_MASK_SAVE_DB_DATA); 

	data.filter_data.data = filter_data.data();
	data.filter_data.size = filter_data.size();

	//����౶��������
	raw_wrapper mec_ar;
	pImp->_multi_exp_ctrl.DBSaveData(mec_ar);
	data.multi_exp_ctrl.data = mec_ar.data();
	data.multi_exp_ctrl.size = mec_ar.size();

	//���ɹ��׶�
	data.faction_contrib.data = &pImp->_faction_contrib;
	data.faction_contrib.size = sizeof(pImp->_faction_contrib);
	
	//�����������
	pImp->_player_force.MakeDBData(data.force_data);
	
	pImp->_meridianman.MakeDBData(data.meridian);
	//��������ʱ������
	raw_wrapper profit_time_data;
	pImp->SaveProfitTime(profit_time_data);
	data.profit_time_data.data = profit_time_data.data();
	data.profit_time_data.size = profit_time_data.size();

	//������߽�������
	raw_wrapper oa_ar;
	pImp->_online_award.DBSaveData(oa_ar);
	data.online_award.data = oa_ar.data();
	data.online_award.size = oa_ar.size();
	
	//��������
	info.country_id = pImp->GetCountryId();
	info.country_expire_time = pImp->GetCountryExpireTime();
	//��������
	info.is_king = pPlayer->IsKing() ? 1 : 0;
	info.king_expire_time = pImp->GetKingExpireTime();
	
	//�����Զ�������
	player_var_data var_data;
	var_data.MakeData(pPlayer,pImp);
	data.var_data.data = &var_data;
	data.var_data.size = sizeof(var_data);
	data.trashbox_passwd.data = (void*) pImp->_trashbox.GetPassword(data.trashbox_passwd.size);

	//����Ч������ �������ֻд�̲��ö� ��Ϊʵ�����Ǹ��������õ�
	data.custom_status.data = pPlayer->effect_list;
	data.custom_status.size = pPlayer->effect_count * sizeof(short);

	//������ҽ�ɫ״̬
	raw_wrapper char_data;
	pImp->GetPlayerCharMode(char_data);
	data.charactermode.data = char_data.data();
	data.charactermode.size = char_data.size();

	//������ҵ����и�������
	raw_wrapper ins_data;
	pImp->SaveInstanceKey(ins_data);
	data.instancekeylist.data = ins_data.data();
	data.instancekeylist.size = ins_data.size();

	//������ҳƺ�����
	raw_wrapper title_ar;
	pImp->GetDBTitleData(title_ar);
	data.title_data.data = title_ar.data();
	data.title_data.size = title_ar.size();

	//���������������
	raw_wrapper fatering_data;
	pImp->GetDBFateRingData(fatering_data);
	data.fatering_data.data = fatering_data.data();
	data.fatering_data.size = fatering_data.size();

	//ת��
	pImp->_player_reincarnation.MakeDBData(data.reincarnation);

	//����
	pImp->GetDBRealmData(data.realm.level,data.realm.exp);

	//����ͼ��
	info.leadership = pImp->GetDBLeadership();
	data.generalcard_collection.data = pImp->_generalcard_collection.data(data.generalcard_collection.size);
	
	//������ҵĳ�������
	GDB::pet pet_list[pet_manager::MAX_PET_CAPACITY];
	unsigned int pet_count = 0;
	for(unsigned int i = 0; i < pet_manager::MAX_PET_CAPACITY; i ++)
	{
		pet_data * pData = pImp->GetPetData(i);
		if(!pData)  continue;
		pet_list[pet_count].index = i;
		pet_list[pet_count].data.data = pData;
		pet_list[pet_count].data.size = sizeof(pet_data);
		pet_count ++;
	}
	data.pets.capacity = pImp->GetPetSlotCapacity();
	data.pets.count = pet_count;
	data.pets.list = pet_list;
	info.timestamp = pImp->OI_InceaseDBTimeStamp();

	// clock ����
	raw_wrapper clockdata;
	pImp->GetDBClockData(clockdata);
	data.clock_data.data = clockdata.data();
	data.clock_data.size = clockdata.size();

	//����̳�����
	raw_wrapper randmalldata;
	pImp->GetDBRandMallData(randmalldata);
	data.rand_mall_data.data = randmalldata.data();
	data.rand_mall_data.size = randmalldata.size();

	pImp->GetDBWorldContrib(info.world_contribution.contrib,info.world_contribution.cost);
	pImp->GetDBAstrolabeExtern(info.astrolabe_extern.level,info.astrolabe_extern.exp);

    pImp->GetDBSoloChallengeInfo(info.solo_challenge_info);
    pImp->GetDBMNFactionInfo(info.mnfaction_info.unifid);
    pImp->GetDBVisaInfo(info.visa_info.type, info.visa_info.stay_timestamp, info.visa_info.cost_item, info.visa_info.cost_item_count);

	raw_wrapper fixpositiontransmitdata;
	pImp->GetDBFixPositionTransmit(fixpositiontransmitdata);
	data.fix_position_transmit_data.data = fixpositiontransmitdata.data();
	data.fix_position_transmit_data.size = fixpositiontransmitdata.size();

    pImp->GetDBCashResurrectTimesInCoolDown(info.cash_resurrect_info.times);

	pImp->GetDBCashVipInfo(info.vip_level, info.score_add, info.score_cost, info.score_consume);

	pImp->GetDBPurchaseLimitData(info.next_day_item_clear_timestamp, info.next_week_item_clear_timestamp, info.next_month_item_clear_timestamp, info.next_year_item_clear_timestamp);
	
	raw_wrapper purchase_limit_data;
	pImp->GetDBPurchaseLimitInfo(purchase_limit_data);
	data.purchase_limit_data.data = purchase_limit_data.data();
	data.purchase_limit_data.size = purchase_limit_data.size();

	// Salvamento antigo
	pImp->GetLua()->SaveRole();

	if (info.trashbox_active)
	{
		pImp->SaveTrashBox();
	}

	GDB::vecdata_new new_info;
	pImp->DBSaveNewData(new_info);
	db_player_save_new(pPlayer->ID.id, new_info);

	/*---------------------------������������------------------------------------*/
	GDB::put_role(info.id,&info,&data,callback,priority,mask);
/*-------------------------���������������----------------------------------*/

	//�ͷŰ�����������ڴ�
	pImp->GetInventory().ReleaseDBData(data.inventory);
	pImp->GetEquipInventory().ReleaseDBData(data.equipment);
	pImp->GetTaskInventory().ReleaseDBData(data.task_inventory);
	if(info.trashbox_active)
	{
		pImp->GetTrashInventory(gplayer_imp::IL_TRASH_BOX).ReleaseDBData(data.trash_box);
		pImp->GetTrashInventory(gplayer_imp::IL_TRASH_BOX2).ReleaseDBData(data.trash_box1);
		pImp->GetTrashInventory(gplayer_imp::IL_TRASH_BOX3).ReleaseDBData(data.trash_box2);
		pImp->GetTrashInventory(gplayer_imp::IL_TRASH_BOX4).ReleaseDBData(data.trash_box3);
	}
	if(info.userstore_active)
	{
		pImp->GetTrashInventory(gplayer_imp::IL_USER_TRASH_BOX).ReleaseDBData(data.user_store);
	}
	pImp->_player_force.ReleaseDBData(data.force_data);
	pImp->_player_reincarnation.ReleaseDBData(data.reincarnation);

	//�ָ���������
	task_if.InitActiveTaskList();

	GLog::log(GLOG_INFO,"save_data:%d���ͱ����û�%d�������� %s�����ֿ���Ϣ �ȼ�%d ��Ǯ%u sp%d %s�����ʺŲֿ���Ϣ(��Ǯ%d �ɲ���������%d ��Ʒ��%d)",
			world_manager::GetWorldTag(),
			pPlayer->ID.id,
			info.trashbox_active?"":"��",
			info.level,
			pImp->GetGold() + pImp->GetTrashBox().GetMoney(),
			info.sp,
			info.userstore_active?"":"��",
			pImp->_user_trashbox.GetMoney(),
			pImp->_user_trashbox.GetTrashBoxSize(0)/2,
			pImp->_user_trashbox.GetBackpack(0).GetItemCount()
			);

}

void	user_login(int cs_index,int cs_sid,int uid,const void * auth_data, unsigned int auth_size, bool isshielduser, char flag)
{
	world_manager::GetInstance()->UserLogin(cs_index,cs_sid,uid,auth_data, auth_size, isshielduser, flag);
}

#define MAX_BASE_PP 50

static bool __login_check_data(const GDB::base_info * pInfo, const GDB::vecdata * data)
{
	int level = pInfo->level;
	int pp = pInfo->pp;
	int reincarnation_times = data->reincarnation.count;
	extend_prop prop;
	memset(&prop,0,sizeof(prop));
	if(data->property.data)
	{
		if(data->property.size < sizeof(prop))
		{
			//ASSERT(data->property.size == sizeof(prop) - 4);
			memcpy(&prop,data->property.data,data->property.size);
			prop.max_ap = 0;
		}
		else
		{
			//ASSERT(data->property.size == sizeof(prop));
			memcpy(&prop,data->property.data,sizeof(prop));
		}

	}
	else
	{
		return false;
	}

	if( (level -1) * 5 + MAX_BASE_PP + player_reincarnation::MaxExtraPPoint()*reincarnation_times < pp + prop.vitality + prop.energy + prop.strength + prop.agility)
	{
		//��������
		return false;
	}

	if(!player_template::CheckData(pInfo->cls,level,prop))
	{	
		return false;
	}
	return true;
}

bool 	do_login_check_data(const GDB::base_info * pInfo, const GDB::vecdata * data)
{
	bool bRst = __login_check_data(pInfo,data);
/*	if(!bRst) GLog::log(GLOG_ERR,"[3-23����]��¼ʱ �����û�%d�����쳣��Ŀǰ�����д���",pInfo->id);
	return true;
	if(!bRst && player_template::GetDebugMode()) 
	{
		GLog::log(GLOG_ERR,"����״̬�·����û�%d�����쳣���˽��������֮",pInfo->id);
		//return bRst;
		return true;
		
	}
	*/
	return bRst;
}
