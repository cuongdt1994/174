#include <common/utf.h>
#include <threadpool.h>
#include <malloc.h>
#include <unordered_map>
#include <db_if.h>
#include "threadusage.h"
#include "world.h"
#include "worldmanager.h"
#include "arandomgen.h"
#include "threadusage.h"
#include "player_imp.h"
#include "usermsg.h"
#include "public_quest.h"
#include "luamanager.h"
#include "player_kid_addons.h"
#include "emulate_settings.h"
#include <glog.h>

void gplayer_kid_addons::GenerateKidsAddons(int roleid)
{
	int windex1;
	gplayer *gPlayer = world_manager::GetInstance()->FindPlayer(roleid, windex1);
	if (!gPlayer || !gPlayer->imp)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::GenerateKidsAddons: failed to get player");
		return;
	}

	gplayer_imp *pImp = (gplayer_imp *)gPlayer->imp;
	if (!pImp)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::GenerateKidsAddons: failed to get player imp");
		return;
	}

	// clear
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			pImp->_kids_addons[i]._total_addon[j].clear();
		}
	}

	const int IDXS[] = {6878, 6977, 6979, 6978, 6980, 6981};
	DATA_TYPE dt;
	for (int i = 0; i < 6; i++)
	{
		// addons[i].pos is the kid index (0..5); slot index i is just storage order.
		// Old code used IDXS[i], which loaded the wrong reward config when the slot
		// index didn't match the kid index — fix per 173 ref ActivateReward semantics.
		if (addons[i].pos < 0 || addons[i].pos >= (int)gplayer_kid::MAX_CELESTIAL)
			continue;

		KID_LEVEL_REWARD_CONFIG *pCfg = (KID_LEVEL_REWARD_CONFIG *)world_manager::GetDataMan().get_data_ptr(IDXS[addons[i].pos], ID_SPACE_CONFIG, dt);
		if (dt != DT_KID_LEVEL_REWARD_CONFIG || !pCfg)
			continue;

		int cnt = (int)addons[i].addons_count;
		if (cnt < 0) cnt = 0;
		if (cnt > 8) cnt = 8;
		for (int j = 0; j < cnt; j++)
		{
			int addon_pos = (int)addons[i].addons_pos[j];
			if (addon_pos < 0 || addon_pos >= 64)
				continue;

			addon_data new_data;
			if (!world_manager::GetDataMan().generate_addon(pCfg->reward[addon_pos].addon_id, new_data))
			{
				GLog::log(GLOG_ERR, "gplayer_kid_addons::GenerateKidsAddons: failed to generate addon data");
				continue;
			}
			pImp->_kids_addons[i]._total_addon[j].push_back(new_data);
		}
	}
}

void gplayer_kid_addons::ActivateKidsAddons(int roleid)
{
	int windex1;
	gplayer *gPlayer = world_manager::GetInstance()->FindPlayer(roleid, windex1);
	if (!gPlayer || !gPlayer->imp)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::SetCelestialNewLevel: failed to get player");
		return;
	}

	gplayer_imp *pImp = (gplayer_imp *)gPlayer->imp;
	if (!pImp)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::SetCelestialNewLevel: failed to get player imp");
		return;
	}

	gactive_imp *imp = (gactive_imp *)pImp->_commander->_imp;
	if (!imp)
		return;

	GenerateKidsAddons(roleid);

	for (int j = 0; j < 6; j++)
	{
		for (int z = 0; z < 8; z++)
		{
			for (int y = 0; y < pImp->_kids_addons[j]._total_addon[z].size(); y++)
			{
				addon_manager::Activate(pImp->_kids_addons[j]._total_addon[z][y], NULL, imp);
			}
		}
	}
}

void gplayer_kid_addons::DeactivateKidsAddons(int roleid)
{
	int windex1;
	gplayer *gPlayer = world_manager::GetInstance()->FindPlayer(roleid, windex1);
	if (!gPlayer || !gPlayer->imp)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::SetCelestialNewLevel: failed to get player");
		return;
	}

	gplayer_imp *pImp = (gplayer_imp *)gPlayer->imp;
	if (!pImp)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::SetCelestialNewLevel: failed to get player imp");
		return;
	}

	GenerateKidsAddons(roleid);

	gactive_imp *imp = (gactive_imp *)pImp->_commander->_imp;
	if (!imp)
		return;

	for (int j = 0; j < 6; j++)
	{
		for (int z = 0; z < 8; z++)
		{
			for (int y = 0; y < pImp->_kids_addons[j]._total_addon[z].size(); y++)
			{
				addon_manager::Deactivate(pImp->_kids_addons[j]._total_addon[z][y], NULL, imp);
			}
		}
	}
}

void gplayer_kid_addons::UpdateKidsAddonsProtocol(int roleid)
{
	int windex1;
	gplayer *gPlayer = world_manager::GetInstance()->FindPlayer(roleid, windex1);
	if (!gPlayer || !gPlayer->imp)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::SetCelestialNewLevel: failed to get player");
		return;
	}

	gplayer_imp *pImp = (gplayer_imp *)gPlayer->imp;
	if (!pImp)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::SetCelestialNewLevel: failed to get player imp");
		return;
	}

	packet_wrapper h1(256);
	h1 << (int)0;
	h1 << addons_count;

	for (int i = 0; i < addons_count; i++)
	{
		h1 << addons[i].pos;
		h1 << addons[i].addons_count;

		for (int j = 0; j < addons[i].addons_count; j++)
		{
			h1 << addons[i].addons_pos[j];
		}
	}
	pImp->_runner->kid_award_addon(h1.size(), h1.data());
}

void gplayer_kid_addons::SetRecvKidsAddons(int roleid, int pos, int addon_pos)
{
	int windex1;
	gplayer *gPlayer = world_manager::GetInstance()->FindPlayer(roleid, windex1);
	if (!gPlayer || !gPlayer->imp)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::SetRecvKidsAddons: failed to get player");
		return;
	}

	gplayer_imp *pImp = (gplayer_imp *)gPlayer->imp;
	if (!pImp)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::SetRecvKidsAddons: failed to get player imp");
		return;
	}

	// Bounds: pos is the kid slot index (0..MAX_CELESTIAL-1),
	//         addon_pos indexes KID_LEVEL_REWARD_CONFIG.reward[64].
	// 173 ref (player_kid::ActivateReward) checks: num > 5 -> reject, num2 > 0x3F -> reject.
	if (pos < 0 || pos >= (int)gplayer_kid::MAX_CELESTIAL)
		return;
	if (addon_pos < 0 || addon_pos >= 64)
		return;

	// Kid must be unlocked. 173 ref rejects when _kid_ess[num]._tid == 0; here idx mirrors _tid.
	if (pImp->GetKid()->GetCelestial(pos)->idx <= 0)
		return;

	const int IDXS[] = {6878, 6977, 6979, 6978, 6980, 6981};
	DATA_TYPE dt;
	KID_LEVEL_REWARD_CONFIG *pCfg = (KID_LEVEL_REWARD_CONFIG *)world_manager::GetDataMan().get_data_ptr(IDXS[pos], ID_SPACE_CONFIG, dt);
	if (dt != DT_KID_LEVEL_REWARD_CONFIG || !pCfg)
		return;

	unsigned int require_level = pCfg->reward[addon_pos].require_level;

	if ((unsigned int)pImp->GetKid()->GetCelestial(pos)->level < require_level)
		return;

	// Locate slot for this kid (or pick a fresh one). Match 173 _addon_mask idempotent
	// semantics by rejecting an addon that has already been claimed for the same kid.
	int slot = -1;
	for (int i = 0; i < 6; i++)
	{
		if (addons[i].pos == pos)
		{
			int cnt = (int)addons[i].addons_count;
			if (cnt > 8) cnt = 8;
			for (int j = 0; j < cnt; j++)
			{
				if ((int)addons[i].addons_pos[j] == addon_pos)
					return; // already claimed -> idempotent no-op
			}
			slot = i;
			break;
		}
	}

	if (slot < 0)
	{
		for (int i = 0; i < 6; i++)
		{
			if (addons[i].pos == -1)
			{
				slot = i;
				break;
			}
		}
		if (slot < 0)
			return; // no free slot
	}
	else if (addons[slot].addons_count >= 8)
	{
		return; // capacity reached for this kid (addons_pos[8])
	}

	DeactivateKidsAddons(roleid);

	if (addons[slot].pos == -1)
	{
		addons[slot].pos = pos;
		addons[slot].addons_pos[0] = (unsigned int)addon_pos;
		addons[slot].addons_count = 1;
		addons_count++;
	}
	else
	{
		addons[slot].addons_pos[addons[slot].addons_count] = (unsigned int)addon_pos;
		addons[slot].addons_count++;
	}

	UpdateKidsAddonsProtocol(roleid);
	ActivateKidsAddons(roleid);

	pImp->OnRefreshEquipment();
}

void gplayer_kid_addons::SetCelestialNewLevel(int roleid, int pos, int level)
{
	int windex1;
	gplayer *gPlayer = world_manager::GetInstance()->FindPlayer(roleid, windex1);
	if (!gPlayer || !gPlayer->imp)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::SetCelestialNewLevel: failed to get player");
		return;
	}

	gplayer_imp *pImp = (gplayer_imp *)gPlayer->imp;
	if (!pImp)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::SetCelestialNewLevel: failed to get player imp");
		return;
	}

	DATA_TYPE dt;
	KID_EXP_CONFIG *pCfg = (KID_EXP_CONFIG *)world_manager::GetDataMan().get_data_ptr(IDX_MAX_LEVEL_MONEY_COST, ID_SPACE_CONFIG, dt);
	if (dt != DT_KID_EXP_CONFIG || !pCfg)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::SetCelestialNewLevel: failed to get data pointer from data manager");
		return;
	}

	int currentl = pImp->GetKid()->GetCelestial(pos)->level;
	int newl = currentl + level;

	if (currentl < 0 || newl >= 150)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::SetCelestialNewLevel: invalid level");
		return;
	}



	int totalmoneycost = 0;
	for (int i = currentl; i < newl; ++i)
	{
		totalmoneycost += pCfg->exp[i];
	}

	if (!EmulateSettings::GetInstance()->GetKidFreeCelestialLevel())
	{
		if (pImp->GetAllMoney() < totalmoneycost)
		{
			pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return;
		}

		pImp->SpendAllMoney(totalmoneycost, true);
		pImp->SelfPlayerMoney();
	}

	int set_new_level = pImp->GetKid()->GetCelestial(pos)->level + level;	
	if(set_new_level > gplayer_kid::MAX_KID_LEVEL)
	{
		set_new_level = gplayer_kid::MAX_KID_LEVEL;
	}	

	pImp->GetKid()->SetCelestial(pos, set_new_level, pImp->GetKid()->GetCelestial(pos)->rank, pImp->GetKid()->GetCelestial(pos)->exp, pImp->GetKid()->GetCelestial(pos)->idx);
	pImp->KidCelestialInfoProtocol(0);
}

void gplayer_kid_addons::SetKidsWipe(int roleid)
{
	int windex1;
	gplayer *gPlayer = world_manager::GetInstance()->FindPlayer(roleid, windex1);
	if (!gPlayer || !gPlayer->imp)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::SetKidsWipe: failed to get player");
		return;
	}

	gplayer_imp *pImp = (gplayer_imp *)gPlayer->imp;
	if (!pImp)
	{
		GLog::log(GLOG_ERR, "gplayer_kid_addons::SetKidsWipe: failed to get player imp");
		return;
	}
	
	if (kids_wipe == 0)
	{
		if(kids_wipe != 0)
			return;
		
		if(pImp->GetReincarnationTimes () < 1)
		{
			kids_wipe = 1;			
			return;
		}

		for (unsigned int i = 0; i < gplayer_kid::MAX_CELESTIAL; i++)
		{
			int idx = pImp->GetKid()->GetCelestial(i)->idx;

			if (idx > 0)
			{
				int rank_level = pImp->GetKid()->GetCelestial(i)->rank;
				int count_itens = 0;

				unsigned int required_rank[] = {9, 90, 900, 1000, 2000, 4000, 8000, 16000, 32000};
				unsigned int item_id[] = {67573, 67577, 67585, 67581, 67589, 67593};

				if (rank_level > 0)
				{
					unsigned int total_exp = pImp->GetKid()->GetCelestial(i)->exp;
					for (int j = 0; j < rank_level; j++)
					{
						total_exp += required_rank[j];
					}
					count_itens = total_exp / 100;
				}

				DATA_TYPE data2;
				const KID_PROPERTY_CONFIG *config2 = (const KID_PROPERTY_CONFIG *)world_manager::GetDataMan().get_data_ptr(idx, ID_SPACE_CONFIG, data2);
				if (config2 && data2 == DT_KID_PROPERTY_CONFIG)
				{
					DATA_TYPE data3;
					const KID_LEVEL_MAX_CONFIG *config3 = (const KID_LEVEL_MAX_CONFIG *)world_manager::GetDataMan().get_data_ptr(6877, ID_SPACE_CONFIG, data3);
					if (config3 && data3 == DT_KID_LEVEL_MAX_CONFIG)
					{
						int newexp = 0;
						int newlevel = 0;
						newlevel += config3->level_max[config2->rahk];

						pImp->GetKid()->SetCelestial(i, newlevel < 1 ? 1 : newlevel, config2->rahk >= 3 ? 1 : 0, newexp, idx);
						pImp->KidCelestialInfoProtocol(0);

						if(count_itens > 0)
						{
							pImp->InvPlayerGiveItem(item_id[config2->kid_debri_type], count_itens);
						}						
					}
				}
			}
		}

		kids_wipe = 1;
	}
}