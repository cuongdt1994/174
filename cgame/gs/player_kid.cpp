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
#include "player_kid.h"
const int player_kid::exp_required_next_level[] = { 0, 2, 4, 8, 12, 20, 20, 20, 20, 20 };
const int player_kid::exp_min_level[]           = { 0, 0, 2, 6, 14, 26, 46, 66, 86, 106 };
const int player_kid::card_level_1[] = { 66225, 66227, 66228, 66229, 66230, 66231 };
const int player_kid::card_level_2[] = { 66232, 66233, 66234, 66235, 66236, 66237, 66238, 66239 };
const int player_kid::card_level_3[] = { 66240, 66241, 66242, 66243, 66244, 66245, 66246 };
const int player_kid::card_level_4[] = { 66247, 66248, 66249, 66250, 66251, 66252, 66253 };
const int player_kid::card_level_5[] = { 66254 };
const int* player_kid::card_pools[]  = { card_level_1, card_level_2, card_level_3, card_level_4, card_level_5 };
const int  player_kid::card_counts[] = { 6, 8, 7, 7, 1 };
const int player_kid::suite_idx_course[] = {
    66226, 66255, 66256, 66257, 66258, 66259, 66260, 66261,
    66262, 66263, 66264, 66265
};
const int player_kid::suite_idx_mask[] = {
    1, 2, 4, 8, 16, 32, 64, 128,
    65536, 131072, 262144, 524288
};
const unsigned int player_kid::KID_REWARD_ID[] = { 6878, 6977, 6979, 6978, 6980, 6981 };
bool player_kid::Save(archive& ar)
{
    ar.push_back(&_kid_data, DB_DATA_SIZE);
	return true;
}
bool player_kid::Load(archive& ar)
{
    ar.pop_back(&_kid_data, DB_DATA_SIZE);
	return true;
}
void player_kid::Swap(player_kid& rhs)
{
    memcpy(&_kid_data, &rhs._kid_data, DB_DATA_SIZE);
}
const void* player_kid::SaveToDB(size_t& size)
{
	size = DB_DATA_SIZE;
	return &_kid_data;
}
void player_kid::InitFromDB(const void* buf, size_t size)
{
    spin_autolock l(_lock_data_map);
    if (size == DB_DATA_SIZE)
    {
        memcpy(&_kid_data, buf, DB_DATA_SIZE);
        ActivateAllAddon();
    }
}
void player_kid::Heartbeat(int cur_time)
{
    if (_update_time && cur_time >= _update_time)
    {
        _kid_data._new_day = 0;
        _update_time = 0;
        _kid_data._type = 0;
        ClientSync(1);
    }
}
bool player_kid::CreateKid(const void* buf)
{
    spin_autolock l(_lock_data_map);
    if (_kid_data._status)
        return true;
    memset(&_kid_data, 0, sizeof(_kid_data));
    const char* data = (const char*)buf;
    int count = data[1];
    if (count > MAX_NAME_LENGTH)
        count = MAX_NAME_LENGTH;
    memcpy(_kid_data._name, data + 2, count);
    _kid_data._pool_lvl   = 1;
    _kid_data._gender     = (data[0] != 0);
    _kid_data._status     = 1;
    _kid_data._free_times = 1;
    RePool();
    _kid_data._free_times = 1;
    _owner->_runner->kid_created_info_dialog();
    ClientSync(1);
    return false;
}
bool player_kid::StartDay()
{
    if (!_kid_data._status || _kid_data._new_day)
        return false;
    if (_kid_data._growth_days > 14)
        return false;
    ++_kid_data._growth_days;
    _kid_data._free_times = 1;
    _kid_data._new_day    = 1;
    _kid_data._energy     = 20;
    _update_time          = NEXT_DAY_TIME();
    DATA_TYPE dt;
    itemdataman* DataMan = world_manager::GetDataMan();
    const KID_SYSTEM_CONFIG* cfg = (const KID_SYSTEM_CONFIG*)
        itemdataman::get_data_ptr(DataMan, 6858, ID_SPACE::ID_SPACE_CONFIG, dt);
    if (!cfg || dt != DT_KID_SYSTEM_CONFIG)
        return true;
    if (_kid_data._pool_lvl > 9)
        return true;
    if (++_kid_data._pool_exp >= cfg->level[_kid_data._pool_lvl].require_exp)
        ++_kid_data._pool_lvl;
    return true;
}
bool player_kid::OnCreateKid()
{
    if (!_kid_data._status || _kid_data._new_day)
        return false;
    DATA_TYPE dt;
    itemdataman* DataMan = world_manager::GetDataMan();
    const KID_QUALITY_CONFIG* cfg = (const KID_QUALITY_CONFIG*)
        itemdataman::get_data_ptr(DataMan, IDX_KID_QUALITY_CONFIG, ID_SPACE::ID_SPACE_CONFIG, dt);
    if (!cfg || dt != DT_KID_QUALITY_CONFIG)
        return false;
    for (size_t i = 0; i <= 3; ++i)
    {
        if (_kid_data._exp < cfg->list[i].require_score_min
         || _kid_data._exp > cfg->list[i].require_score_max)
            continue;
        int idx = abase::RandSelect(&cfg->list[i].kid[_kid_data._gender][0].probability, 8, 8);
        int tmp_id = cfg->list[i].kid[_kid_data._gender][idx].id;
        const KID_PROPERTY_CONFIG* cfg2 = (const KID_PROPERTY_CONFIG*)
            itemdataman::get_data_ptr(world_manager::GetDataMan(), tmp_id, ID_SPACE::ID_SPACE_CONFIG, dt);
        if (!cfg2 || dt != DT_KID_PROPERTY_CONFIG)
            return false;
        if (cfg2->kid_debri_type >= MAX_CELESTIAL)
        {
            GLog::log(3, "kid_debri_type ERR  id:%d   %d\n", tmp_id, cfg2->kid_debri_type);
            return false;
        }
        if (_kid_data._gender != cfg2->gender)
        {
            GLog::log(3, "gender ERR  id:%d   %d   %d\n", tmp_id, _kid_data._gender, cfg2->gender);
            return false;
        }
        kid_ess* tmp_kid = &_kid_ess[cfg2->kid_debri_type];
        if (tmp_kid->_tid)
        {
            size_t count = cfg2->kid_debri_exp;
            AddDebri(cfg2->kid_debri_type, count);
            if (count == cfg2->kid_debri_exp)
            {
                item_list& inv = _owner->GetInventory();
                if (inv.IsFull())
                    return false;
                item_tag_t tag = 0;
                item_data* data = itemdataman::generate_item_from_player(
                    world_manager::GetDataMan(), cfg2->kid_debri_id, &tag, 2);
                if (!data)
                    return false;
                if (count > data->pile_limit)
                    count = data->pile_limit;
                data->count = count;
                int rst = inv.Push(*data);
                _owner->FirstAcquireItem(data);
                if (rst >= 0)
                {
                    item* it = &inv[rst];
                    _owner->_runner->pickup_item(data->type, 0, count - data->count, it->count, 0, rst);
                }
                FreeItem(data);
            }
        }
        else
        {
            tmp_kid->_lvl        = 1;
            tmp_kid->_rahk_lvl   = 0;
            tmp_kid->_debris_exp = 0;
            tmp_kid->_tid        = tmp_id;
        }
        UpdateKid(cfg2->kid_debri_type);
        _owner->_runner->kid_celestial_awakening(0, tmp_id);
        ClientSync(5);
        memset(&_kid_data, 0, sizeof(_kid_data));
    }
    return true;
}
bool player_kid::UpPool()
{
    if (!_kid_data._status || _kid_data._type)
        return false;
    if (_kid_data._energy <= 3)
        return false;
    DATA_TYPE dt;
    itemdataman* DataMan = world_manager::GetDataMan();
    const KID_SYSTEM_CONFIG* cfg = (const KID_SYSTEM_CONFIG*)
        itemdataman::get_data_ptr(DataMan, 6858, ID_SPACE::ID_SPACE_CONFIG, dt);
    if (!cfg || dt != DT_KID_SYSTEM_CONFIG)
        return false;
    if (_kid_data._pool_exp >= cfg->level[9].require_exp)
        return false;
    _kid_data._energy   -= 4;
    _kid_data._pool_exp += 4;
    if (_kid_data._pool_exp >= cfg->level[_kid_data._pool_lvl].require_exp)
        ++_kid_data._pool_lvl;
    return true;
}
bool player_kid::RePool()
{
    if (!_kid_data._status || _kid_data._type)
        return false;
    if (!_kid_data._free_times && _kid_data._energy <= 1)
        return false;
    DATA_TYPE dt;
    itemdataman* DataMan = world_manager::GetDataMan();
    const KID_SYSTEM_CONFIG* cfg = (const KID_SYSTEM_CONFIG*)
        itemdataman::get_data_ptr(DataMan, 6858, ID_SPACE::ID_SPACE_CONFIG, dt);
    if (!cfg || dt != DT_KID_SYSTEM_CONFIG)
        return false;
    int tmp_p[MAX_RANDOM_COURSE];
    for (int i = 0; i <= 4; ++i)
    {
        int tmp = abase::Rand(0, 9999);
        tmp_p[i] = 0;
        for (int j = 0; j < 5; ++j)
        {
            int prob = cfg->unk[6 * _kid_data._pool_lvl + 14 + j];
            if (tmp <= prob)
            {
                tmp_p[i] = j;
                break;
            }
            tmp -= prob;
        }
    }
    for (int i = 0; i <= 4; ++i)
    {
        _kid_data._course_new[i] = card_pools[tmp_p[i]][abase::Rand(0, card_counts[tmp_p[i]] - 1)];
    }
    if (_kid_data._free_times)
        _kid_data._free_times = 0;
    else
        _kid_data._energy -= 2;
    return true;
}
char player_kid::BuyCourse(int num)
{
    if (!_kid_data._status || _kid_data._type)
        return 0;
    if ((unsigned int)num >= MAX_RANDOM_COURSE)
        return 0;
    int tmp_id = _kid_data._course_new[num];
    if (!tmp_id)
        return 0;
    DATA_TYPE dt;
    itemdataman* DataMan = world_manager::GetDataMan();
    const COURSE_ESSENCE* cfg = (const COURSE_ESSENCE*)
        itemdataman::get_data_ptr(DataMan, tmp_id, ID_SPACE::ID_SPACE_ESSENCE, dt);
    if (!cfg || dt != DT_COURSE_ESSENCE)
        return 0;
    if (cfg->cost > _kid_data._energy)
        return 0;
    int tmp_num = -1;
    for (int i = MAX_EQUIPED_COURSE; i < MAX_COURSE_INV; ++i)
    {
        if (!_kid_data._course_inv[i]._tid)
        {
            tmp_num = i;
            break;
        }
    }
    if (tmp_num == -1)
        return 0; 
    _kid_data._energy -= cfg->cost;
    _kid_data._course_inv[tmp_num]._tid = tmp_id;
    _kid_data._course_inv[tmp_num]._lvl = 1;
    _kid_data._course_new[num] = 0;
    return tmp_num;
}
bool player_kid::SellCourse(int num)
{
    if (!_kid_data._status || _kid_data._type)
        return false;
    if ((unsigned int)num >= MAX_COURSE_INV)
        return false;
    course_ess* tmp = &_kid_data._course_inv[num];
    if (!tmp->_tid || !tmp->_lvl)
        return false;
    DATA_TYPE dt;
    itemdataman* DataMan = world_manager::GetDataMan();
    const COURSE_ESSENCE* cfg = (const COURSE_ESSENCE*)
        itemdataman::get_data_ptr(DataMan, tmp->_tid, ID_SPACE::ID_SPACE_ESSENCE, dt);
    if (!cfg || dt != DT_COURSE_ESSENCE)
        return false;
    _kid_data._energy += cfg->cost + tmp->_lvl - 1;
    tmp->_tid = 0;
    tmp->_lvl = 0;
    return true;
}
bool player_kid::MoveCourse(int src_num, int dst_num)
{
    if (!_kid_data._status || _kid_data._type)
        return false;
    if ((unsigned int)src_num >= MAX_COURSE_INV)
        return false;
    if ((unsigned int)dst_num >= MAX_COURSE_INV)
        return false;
    course_ess tmp = _kid_data._course_inv[src_num];
    _kid_data._course_inv[src_num] = _kid_data._course_inv[dst_num];
    _kid_data._course_inv[dst_num] = tmp;
    return true;
}
bool player_kid::UpCourse(int num1, int num2, int num3)
{
    if (!_kid_data._status || _kid_data._type)
        return false;
    if ((unsigned int)num1 >= MAX_COURSE_INV)
        return false;
    if ((unsigned int)num2 >= MAX_COURSE_INV)
        return false;
    if ((unsigned int)num3 >= MAX_COURSE_INV)
        return false;
    course_ess& main = _kid_data._course_inv[num1];
    course_ess& mat1 = _kid_data._course_inv[num2];
    course_ess& mat2 = _kid_data._course_inv[num3];
    if (main._lvl > 2)
        return false;
    if (main._tid != mat1._tid || main._tid != mat2._tid)
        return false;
    if (main._lvl != mat1._lvl || main._lvl != mat2._lvl)
        return false;
    ++main._lvl;
    mat1._tid = 0;  mat1._lvl = 0;
    mat2._tid = 0;  mat2._lvl = 0;
    return true;
}
int player_kid::EndTeach()
{
    if (!_kid_data._status || _kid_data._type)
        return 0;
    int sutie_num[32];
    memset(sutie_num, 0, sizeof(sutie_num));
    int sutie_tmp[MAX_EQUIPED_COURSE];
    for (int i = 0; i < MAX_EQUIPED_COURSE; ++i)
        sutie_tmp[i] = _kid_data._course_inv[i]._tid;
    for (int i = 1; i < MAX_EQUIPED_COURSE; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            if (sutie_tmp[i] == sutie_tmp[j])
                sutie_tmp[i] = 0;
        }
    }
    int total_score = 0;
    bool b_num_score = true;
    for (int i = 0; i < 12; ++i)
    {
        DATA_TYPE dt;
        itemdataman* DataMan = world_manager::GetDataMan();
        const COURSE_SUITE_ESSENCE* cfg = (const COURSE_SUITE_ESSENCE*)
            itemdataman::get_data_ptr(DataMan, suite_idx_course[i], ID_SPACE::ID_SPACE_ESSENCE, dt);
        if (!cfg || dt != DT_COURSE_SUITE_ESSENCE)
            continue;
        if (cfg->type_mask != suite_idx_mask[i])
        {
            GLog::log(3, "COURSE_SUTIE_MASK ERR  id:%d   0x%X   0x%X\n",
                suite_idx_course[i], cfg->type_mask, suite_idx_mask[i]);
            continue;
        }
        int tmp_num  = 0;    
        int tmp_num2 = 0;    
        size_t max_score = 0;
        for (int s = 0; s < MAX_EQUIPED_COURSE; ++s)
        {
            unsigned int tmp_id = _kid_data._course_inv[s]._tid;
            int tmp_lv = _kid_data._course_inv[s]._lvl;
            if (!tmp_id || !tmp_lv || tmp_lv > 3)
                continue;
            DataMan = world_manager::GetDataMan();
            _DWORD* k = (_DWORD*)itemdataman::get_data_ptr(DataMan, tmp_id, ID_SPACE::ID_SPACE_ESSENCE, dt);
            if (!k || dt != DT_COURSE_ESSENCE)
                continue;
            if (b_num_score)
                total_score += k[tmp_lv + 51];
            if (k[tmp_lv + 51] > max_score)
                max_score = k[tmp_lv + 51];
            if ((k[51] & cfg->type_mask) != 0)
            {
                ++tmp_num2;
                if (sutie_tmp[s])
                    ++tmp_num;
            }
        }
        if (total_score)
            b_num_score = false;
        if (!tmp_num)
            continue;
        for (int t = 2; t >= 0; --t)
        {
            if (!cfg->bonus[t].probability)
                continue;
            if (tmp_num < cfg->bonus[t].min_count)
                continue;
            if (cfg->max_count && cfg->max_count < tmp_num2)
                continue;
            for (int s = 0; s < MAX_EQUIPED_COURSE; ++s)
            {
                unsigned int tmp_id = _kid_data._course_inv[s]._tid;
                int tmp_lv = _kid_data._course_inv[s]._lvl;
                if (!tmp_id || !tmp_lv || tmp_lv > 3)
                    continue;
                DataMan = world_manager::GetDataMan();
                const COURSE_ESSENCE* cfg2 = (const COURSE_ESSENCE*)
                    itemdataman::get_data_ptr(DataMan, tmp_id, ID_SPACE::ID_SPACE_ESSENCE, dt);
                if (!cfg2 || dt != DT_COURSE_ESSENCE)
                    continue;
                int course_score = *(&cfg2->type + tmp_lv);
                switch (cfg->bonus[t].type)
                {
                case 0:
                    if ((cfg2->type & cfg->type_mask) != 0
                     && abase::Rand(0, 9999) < cfg->bonus[t].probability)
                    {
                        total_score += cfg->bonus[t].increase * course_score / 10000;
                    }
                    break;
                case 1: 
                    if (abase::Rand(0, 9999) < cfg->bonus[t].probability)
                        total_score += cfg->bonus[t].increase * course_score / 10000;
                    break;
                case 2: 
                    if (abase::Rand(0, 9999) < cfg->bonus[t].probability
                     && (size_t)course_score == max_score)
                    {
                        total_score += cfg->bonus[t].increase * course_score / 10000;
                    }
                    break;
                }
            }
            break;
        }
    }
    _kid_data._exp += total_score;
    _kid_data._type = 1;
    return total_score;
}
void player_kid::ClientSync(int type)
{
    if (type == 1)
    {
        if (_kid_data._status)
            _owner->_runner->kid_awakening_info(sizeof(kid_data), &_kid_data);
    }
    else if (type == 4)
    {
        if (_select >= 0)
            _owner->_runner->kid_active_info(_select, -1);
    }
    else if (type == 5 || type == 15)
    {
        if (type == 5)
        {
            _owner->KidCelestialInfoProtocol(0);
        }
        abase::vector<unsigned int, abase::fast_alloc<4, 128>> addon_mask_data;
        unsigned int zero = 0;
        addon_mask_data.push_back(zero);
        addon_mask_data.push_back(zero); 
        int kid_count = 0;
        for (int i = 0; i < MAX_CELESTIAL; ++i)
        {
            if (!_kid_ess[i]._tid)
                continue;
            addon_mask_data.push_back((unsigned int)i);
            int count_pos = addon_mask_data.size();
            addon_mask_data.push_back(zero); 
            int addon_count = 0;
            for (int j = 0; j < MAX_REWARD_BIT; ++j)
            {
                if ((_addon_mask[i] >> j) & 1)
                {
                    addon_mask_data.push_back((unsigned int)j);
                    ++addon_count;
                }
            }
            addon_mask_data[count_pos] = addon_count;
            ++kid_count;
        }
        addon_mask_data[1] = kid_count;
        if (kid_count)
        {
            _owner->_runner->kid_award_addon(
                addon_mask_data.size(),
                addon_mask_data.begin());
        }
    }
}
bool player_kid::KidModify(int cmd_type, const void* buf, size_t size)
{
    spin_autolock l(_lock_data_map);
    switch (cmd_type)
    {
    case CMD_KID_BASIC:
    {
        if (size != sizeof(KidModify::mma_1))
            break;
        const KidModify::mma_1* pkt = (const KidModify::mma_1*)buf;
        __PRINTF("----- KidModify   %d   %d   %d\n", cmd_type, pkt->type_1, pkt->type_2);
        switch (pkt->type_1)
        {
        case 0: 
        {
            char ret = BuyCourse(pkt->type_2);
            if (ret)
            {
                _owner->_runner->kid_course_insert(pkt->type_2, ret);
                ClientSync(1);
            }
            break;
        }
        case 1: 
            if (SellCourse(pkt->type_2))
            {
                _owner->_runner->kid_course_remove(pkt->type_2);
                ClientSync(1);
            }
            break;
        case 2: 
            if (UpPool())
            {
                _owner->_runner->kid_course_perc(_kid_data._pool_lvl, 0);
                ClientSync(1);
            }
            break;
        case 3: 
            if (RePool())
            {
                _owner->_runner->kid_course_info((unsigned int*)_kid_data._course_new, MAX_RANDOM_COURSE);
                ClientSync(1);
            }
            break;
        case 4: 
        {
            int ret_0 = EndTeach();
            if (ret_0)
                _owner->_runner->kid_awakening_points(ret_0);
            break;
        }
        case 5: 
            OnCreateKid();
            ClientSync(5);
            break;
        case 6: 
            if (StartDay())
                ClientSync(1);
            break;
        default:
            break;
        }
        goto DONE;
    }
    case CMD_MOVE_COURSE:
    {
        if (size != sizeof(KidModify::mma_1))
            break;
        const KidModify::mma_1* pkt = (const KidModify::mma_1*)buf;
        __PRINTF("----- KidModify   %d   %d   %d\n", CMD_MOVE_COURSE, pkt->type_1, pkt->type_2);
        if (MoveCourse(pkt->type_1, pkt->type_2))
            _owner->_runner->kid_course_change(pkt->type_1, pkt->type_2);
        goto DONE;
    }
    case CMD_UP_COURSE:
    {
        if (size != sizeof(KidModify::mma_0))
            break;
        const KidModify::mma_0* pkt = (const KidModify::mma_0*)buf;
        __PRINTF("----- KidModify   %d   %d   %d\n", cmd_type, pkt->type_1, pkt->type_2);
        if (UpCourse(pkt->type_1, pkt->type_2, pkt->type_3))
            _owner->_runner->kid_course_switch(pkt->type_1, pkt->type_2, pkt->type_3);
        goto DONE;
    }
    case CMD_KID_EXTEND:
    {
        if (size != sizeof(KidModify::mma))
            break;
        const KidModify::mma* pkt = (const KidModify::mma*)buf;
        __PRINTF("----- KidModify   %d   %d   %d\n", cmd_type, pkt->type, pkt->num);
        switch (pkt->type)
        {
        case 0: 
            UpKidLvl(pkt->num, pkt->arg_1);
            ClientSync(5);
            break;
        case 1: 
            UseDebri(pkt->num, pkt->arg_1, pkt->arg_2);
            ClientSync(5);
            break;
        case 2: 
            _select = pkt->num;
            ClientSync(4);
            break;
        case 4: 
            ActivateTransform();
            break;
        case 5: 
            ActivateReward(pkt->num, pkt->arg_1);
            ClientSync(15);
            break;
        }
        goto DONE;
    }
    default:
        break;
    }
DONE:
    return false;
}
void player_kid::KidDeubug(int cmd_type)
{
    spin_autolock l(_lock_data_map);
    if (!_kid_data._status)
        return;
    switch (cmd_type)
    {
    case 0: 
        _kid_data._new_day = 0;
        _update_time = 0;
        _kid_data._type = 0;
        ClientSync(1);
        break;
    case 1: 
        _kid_data._energy += 2000;
        ClientSync(1);
        break;
    }
}
void player_kid::UpdateKid(int num)
{
    if ((unsigned int)num >= MAX_CELESTIAL)
        return;
    kid_ess* tmp_kid = &_kid_ess[num];
    if (!tmp_kid->_tid)
        return;
    DATA_TYPE dt;
    itemdataman* DataMan = world_manager::GetDataMan();
    const KID_PROPERTY_CONFIG* cfg = (const KID_PROPERTY_CONFIG*)
        itemdataman::get_data_ptr(DataMan, tmp_kid->_tid, ID_SPACE::ID_SPACE_CONFIG, dt);
    if (!cfg || dt != DT_KID_PROPERTY_CONFIG)
        return;
    tmp_kid->_physic_damage = tmp_kid->_lvl * cfg->damage;
    tmp_kid->_magic_damage  = tmp_kid->_lvl * cfg->magic_damage;
    tmp_kid->_defence       = tmp_kid->_lvl * cfg->defence;
    for (int i = 0; i < MAX_MAGIC_DEF; ++i)
        tmp_kid->_magic_defences[i] = tmp_kid->_lvl * cfg->magic_defence;
    tmp_kid->_HP   = tmp_kid->_lvl * cfg->hp;
    tmp_kid->_crit = (int)((double)tmp_kid->_lvl * cfg->crit_hit_probability * 100.0);
    if (!cfg->id_kid_upgrade_star)
        return;
    const KID_UPGRADE_STAR_CONFIG* cfg2 = (const KID_UPGRADE_STAR_CONFIG*)
        itemdataman::get_data_ptr(world_manager::GetDataMan(),
            cfg->id_kid_upgrade_star, ID_SPACE::ID_SPACE_CONFIG, dt);
    if (!cfg2 || dt != DT_KID_UPGRADE_STAR_CONFIG)
        return;
    if (tmp_kid->_rahk_lvl <= 0 || tmp_kid->_rahk_lvl > MAX_KID_STAR)
        return;
    int star_idx = tmp_kid->_rahk_lvl - 1;
    double ratio = cfg2->list[star_idx].refine_ratio;
    if (ratio > 0.0)
    {
        tmp_kid->_physic_damage = (int)(tmp_kid->_physic_damage * ratio);
        tmp_kid->_magic_damage  = (int)(tmp_kid->_magic_damage  * ratio);
        tmp_kid->_defence       = (int)(tmp_kid->_defence       * ratio);
        for (int i = 0; i < MAX_MAGIC_DEF; ++i)
            tmp_kid->_magic_defences[i] = (int)(tmp_kid->_magic_defences[i] * ratio);
        tmp_kid->_HP   = (int)(tmp_kid->_HP * ratio);
        tmp_kid->_crit = (int)((double)tmp_kid->_lvl * cfg->crit_hit_probability * ratio * 100.0);
    }
}
bool player_kid::UpKidLvl(size_t num, int count)
{
    if (num >= MAX_CELESTIAL)
        return false;
    if (!_kid_ess[num]._tid || _kid_ess[num]._lvl <= 0)
        return false;
    if (count < 0)
        return false;
    DATA_TYPE dt;
    itemdataman* DataMan = world_manager::GetDataMan();
    const KID_PROPERTY_CONFIG* cfg2 = (const KID_PROPERTY_CONFIG*)
        itemdataman::get_data_ptr(DataMan, _kid_ess[num]._tid, ID_SPACE::ID_SPACE_CONFIG, dt);
    if (!cfg2 || dt != DT_KID_PROPERTY_CONFIG)
        return false;
    unsigned int tmp_lv = cfg2->rahk + _kid_ess[num]._rahk_lvl;
    const KID_LEVEL_MAX_CONFIG* cfg = (const KID_LEVEL_MAX_CONFIG*)
        itemdataman::get_data_ptr(world_manager::GetDataMan(), 6877, ID_SPACE::ID_SPACE_CONFIG, dt);
    if (!cfg || dt != DT_KID_LEVEL_MAX_CONFIG)
        return false;
    if (tmp_lv >= 10)
        tmp_lv = 9;
    int tmp_max_lv = cfg->level_max[tmp_lv];
    if (_kid_ess[num]._lvl + count < tmp_max_lv)
        tmp_max_lv = _kid_ess[num]._lvl + count;
    const KID_EXP_CONFIG* cfg3 = (const KID_EXP_CONFIG*)
        itemdataman::get_data_ptr(world_manager::GetDataMan(), 6875, ID_SPACE::ID_SPACE_CONFIG, dt);
    if (!cfg3 || dt != DT_KID_EXP_CONFIG)
        return false;
    if ((unsigned int)tmp_max_lv > MAX_KID_LEVEL)
        tmp_max_lv = MAX_KID_LEVEL;
    if (tmp_max_lv <= _kid_ess[num]._lvl)
        return false;
    int fee_cost = 0;
    for (int i = _kid_ess[num]._lvl - 1; i < tmp_max_lv - 1; ++i)
        fee_cost += cfg3->exp[i];
    if (fee_cost <= 0)
        return false;
    if (_owner->GetAllMoney() < fee_cost)
        return false;
    _kid_ess[num]._lvl = tmp_max_lv;
    UpdateKid(num);
    _owner->SpendAllMoney(fee_cost, true);
    return true;
}
bool player_kid::UseDebri(size_t num, int where, size_t inv_index)
{
    if (num >= MAX_CELESTIAL)
        return false;
    if (!_kid_ess[num]._tid || _kid_ess[num]._lvl <= 0)
        return false;
    if (where != 11)
        return false;
    item_list& inv = _owner->GetTrashInventory(11);
    if (inv_index >= inv.Size())
        return false;
    int _id    = inv[inv_index].type;
    int _count = inv[inv_index].count;
    if (_id <= 0 || _count <= 0)
        return false;
    DATA_TYPE dt;
    itemdataman* DataMan = world_manager::GetDataMan();
    const KID_DEBRIS_ESSENCE* cfg = (const KID_DEBRIS_ESSENCE*)
        itemdataman::get_data_ptr(DataMan, _id, ID_SPACE::ID_SPACE_ESSENCE, dt);
    if (!cfg || dt != DT_KID_DEBRIS_ESSENCE)
        return false;
    const KID_PROPERTY_CONFIG* cfg2 = (const KID_PROPERTY_CONFIG*)
        itemdataman::get_data_ptr(world_manager::GetDataMan(),
            _kid_ess[num]._tid, ID_SPACE::ID_SPACE_CONFIG, dt);
    if (!cfg2 || dt != DT_KID_PROPERTY_CONFIG)
        return false;
    if (cfg2->kid_debri_type != num)
        return false;
    if (!(1 << num) && (cfg->type & 1) != 0)
        return false;
    size_t count = cfg->swallow_exp;
    AddDebri(num, count);
    if (count == cfg->swallow_exp)
        return false;  
    inv.DecAmount(inv_index, 1);
    _owner->_runner->OnTrashInvUpdate(11, inv_index, _id, 1, 1);
    UpdateKid(num);
    return true;
}
void player_kid::AddDebri(size_t num, size_t& count)
{
    if (!count)
        return;
    if (num >= MAX_CELESTIAL)
        return;
    DATA_TYPE dt;
    itemdataman* DataMan = world_manager::GetDataMan();
    const KID_PROPERTY_CONFIG* cfg2 = (const KID_PROPERTY_CONFIG*)
        itemdataman::get_data_ptr(DataMan, _kid_ess[num]._tid, ID_SPACE::ID_SPACE_CONFIG, dt);
    if (!cfg2 || dt != DT_KID_PROPERTY_CONFIG)
        return;
    if (cfg2->id_kid_upgrade)
    {
        const KID_PROPERTY_CONFIG* cfg3 = (const KID_PROPERTY_CONFIG*)
            itemdataman::get_data_ptr(world_manager::GetDataMan(),
                cfg2->id_kid_upgrade, ID_SPACE::ID_SPACE_CONFIG, dt);
        if (!cfg3 || dt != DT_KID_PROPERTY_CONFIG)
            return;
        if (cfg3->kid_debri_type != num)
            return;
        if (count + _kid_ess[num]._debris_exp < cfg2->require_exp)
        {
            _kid_ess[num]._debris_exp += count;
            count = 0;
        }
        else
        {
            count = count + _kid_ess[num]._debris_exp - cfg2->require_exp;
            _kid_ess[num]._debris_exp = 0;
            _kid_ess[num]._tid = cfg2->id_kid_upgrade;
        }
        if (count)
            AddDebri(num, count);
    }
    else if (cfg2->id_kid_upgrade_star)
    {
        const KID_UPGRADE_STAR_CONFIG* cfg3 = (const KID_UPGRADE_STAR_CONFIG*)
            itemdataman::get_data_ptr(world_manager::GetDataMan(),
                cfg2->id_kid_upgrade_star, ID_SPACE::ID_SPACE_CONFIG, dt);
        if (!cfg3 || dt != DT_KID_UPGRADE_STAR_CONFIG)
            return;
        if (_kid_ess[num]._rahk_lvl > MAX_KID_STAR - 1)
            return;
        size_t req = cfg3->list[_kid_ess[num]._rahk_lvl].require_exp;
        if (count + _kid_ess[num]._debris_exp < req)
        {
            _kid_ess[num]._debris_exp += count;
            count = 0;
        }
        else
        {
            count = count + _kid_ess[num]._debris_exp - req;
            _kid_ess[num]._debris_exp = 0;
            ++_kid_ess[num]._rahk_lvl;
        }
        if (count)
            AddDebri(num, count);
    }
}
bool player_kid::ActivateReward(size_t num2, size_t num)
{
    if (num >= MAX_CELESTIAL)
        return false;
    if (!_kid_ess[num]._tid || _kid_ess[num]._lvl <= 0)
        return false;
    if (num2 >= MAX_REWARD_BIT)
        return false;
    DATA_TYPE dt;
    itemdataman* DataMan = world_manager::GetDataMan();
    const KID_LEVEL_REWARD_CONFIG* cfg = (const KID_LEVEL_REWARD_CONFIG*)
        itemdataman::get_data_ptr(DataMan, KID_REWARD_ID[num], ID_SPACE::ID_SPACE_CONFIG, dt);
    if (!cfg || dt != DT_KID_LEVEL_REWARD_CONFIG)
        return false;
    if (cfg->reward[num2].require_level > _kid_ess[num]._lvl)
        return false;
    if (cfg->reward[num2].item_id && cfg->reward[num2].item_count)
    {
        item_list& inv = _owner->GetInventory();
        if (inv.IsFull())
            return false;
        item_tag_t tag = 0;
        item_data* data = itemdataman::generate_item_from_player(
            world_manager::GetDataMan(), cfg->reward[num2].item_id, &tag, 2);
        if (!data)
            return false;
        unsigned int count = cfg->reward[num2].item_count;
        if (count > data->pile_limit)
            count = data->pile_limit;
        data->count = count;
        int rst = inv.Push(*data);
        _owner->FirstAcquireItem(data);
        if (rst >= 0)
        {
            item* it = &inv[rst];
            _owner->_runner->pickup_item(data->type, 0, count - data->count, it->count, 0, rst);
        }
        FreeItem(data);
    }
    if (cfg->reward[num2].addon_id)
    {
        Activate(cfg->reward[num2].addon_id);
        _owner->KidRefreshEquipment();
    }
    _addon_mask[num] |= 1LL << num2;
    return true;
}
void player_kid::ActivateAllAddon()
{
    for (int i = 0; i < MAX_CELESTIAL; ++i)
    {
        DATA_TYPE dt;
        itemdataman* DataMan = world_manager::GetDataMan();
        const KID_LEVEL_REWARD_CONFIG* cfg = (const KID_LEVEL_REWARD_CONFIG*)
            itemdataman::get_data_ptr(DataMan, KID_REWARD_ID[i], ID_SPACE::ID_SPACE_CONFIG, dt);
        if (!cfg || dt != DT_KID_LEVEL_REWARD_CONFIG)
            continue;
        for (int j = 0; j < MAX_REWARD_BIT; ++j)
        {
            if ((_addon_mask[i] >> j) & 1)
                Activate(cfg->reward[j].addon_id);
        }
    }
}
void player_kid::Activate(int addon_id)
{
    if (!addon_id)
        return;
    addon_data data;
    itemdataman* DataMan = world_manager::GetDataMan();
    if (!itemdataman::generate_addon(DataMan, addon_id, &data))
    {
        __PRINTINFO("kid addon_id  err1   %d \n", addon_id);
        return;
    }
    if (addon_manager::TestUpdate(&data) == 2)
        addon_manager::Activate(&data, 0, _owner, 1.0);
    else
        __PRINTINFO("kid addon_id  err2   %d \n", addon_id);
}
void player_kid::Deactivate(int addon_id)
{
    if (!addon_id)
        return;
    addon_data data;
    itemdataman* DataMan = world_manager::GetDataMan();
    if (!itemdataman::generate_addon(DataMan, addon_id, &data))
    {
        __PRINTINFO("kid addon_id  err1   %d \n", addon_id);
        return;
    }
    if (addon_manager::TestUpdate(&data) == 2)
        addon_manager::Deactivate(&data, 0, _owner, 1.0);
    else
        __PRINTINFO("kid addon_id  err2   %d \n", addon_id);
}
void player_kid::ActivateTransform()
{
    if ((unsigned int)_select >= MAX_CELESTIAL || !_kid_ess[_select]._tid)
        return;
    if (_owner->GetForm())
    {
        filter_man::RemoveFilter(&_owner->_filters, 4550);
        return;
    }
    if (!CheckCoolDown(COOLDOWN_INDEX_KID_TRANSFORMATION))
    {
        _owner->_runner->error_message(53);
        return;
    }
    filter_man::ClearSpecFilter(&_owner->_filters, 12288, 100000);
    filter_man::RemoveFilter(&_owner->_filters, 4267);
    filter_man::RemoveFilter(&_owner->_filters, 4268);
    filter_man::RemoveFilter(&_owner->_filters, 4269);
    filter_man::RemoveFilter(&_owner->_filters, 4270);
    filter_man::RemoveFilter(&_owner->_filters, 4320);
    filter_man::RemoveFilter(&_owner->_filters, 4262);
    filter_man::RemoveFilter(&_owner->_filters, 4325);
    filter_man::RemoveFilter(&_owner->_filters, 4333);
    DATA_TYPE dt;
    itemdataman* DataMan = world_manager::GetDataMan();
    const KID_PROPERTY_CONFIG* cfg = (const KID_PROPERTY_CONFIG*)
        itemdataman::get_data_ptr(DataMan, _kid_ess[_select]._tid, ID_SPACE::ID_SPACE_CONFIG, dt);
    if (!cfg || dt != DT_KID_PROPERTY_CONFIG)
        return;
    kid_ess& sel = _kid_ess[_select];
    tmp_data.shape       = cfg->shape_type;
    tmp_data.attack_type = cfg->attack_type;
    tmp_data.hp = Result(sel._HP,
        _owner->_cur_prop.max_hp,
        _owner->_en_percent.max_hp);
    tmp_data.damage_low = Result(sel._physic_damage,
        _owner->_cur_prop.damage_low,
        _owner->_en_percent.damage + _owner->_en_percent.base_damage);
    tmp_data.damage_high = Result(sel._physic_damage,
        _owner->_cur_prop.damage_high,
        _owner->_en_percent.damage + _owner->_en_percent.base_damage);
    tmp_data.damage_magic_low = Result(sel._magic_damage,
        _owner->_cur_prop.damage_magic_low,
        _owner->_en_percent.base_magic + _owner->_en_percent.magic_dmg);
    tmp_data.damage_magic_high = Result(sel._magic_damage,
        _owner->_cur_prop.damage_magic_high,
        _owner->_en_percent.base_magic + _owner->_en_percent.magic_dmg);
    int enh = (int)((double)(3 * _owner->_cur_prop.strength + 2 * _owner->_cur_prop.vitality)
              * 0.04 + 0.5);
    tmp_data.defence = Result(sel._defence,
        _owner->_cur_prop.defense,
        _owner->_en_percent.defense + enh);
    enh = (int)((double)(3 * _owner->_cur_prop.energy + 2 * _owner->_cur_prop.vitality)
          * 0.04 + 0.5);
    for (int i = 0; i < MAX_RESISTANCE; ++i)
    {
        tmp_data.resistance[i] = Result(sel._magic_defences[i],
            _owner->_cur_prop.resistance[i],
            _owner->_en_percent.resistance[i] + enh);
    }
    tmp_data.crit_hit = sel._crit
                      - _owner->_crit_rate
                      - _owner->_base_crit_rate;
    tmp_data.attack_speed = _owner->_cur_prop.attack_speed
                          - (int)(cfg->attack_speed * 20.0 + 0.00001);
    tmp_data.attack_range = cfg->attack_range - _owner->_cur_prop.attack_range;
    tmp_data.speed        = cfg->run_speed    - _owner->_cur_prop.run_speed;
    enh = _owner->_attack_degree + _owner->_defend_degree;
    tmp_data.attack_degree = (int)((double)enh * cfg->atack_degree_inherit_rate)
                           - _owner->_attack_degree;
    tmp_data.defend_degree = (int)((double)enh * cfg->defend_degree_inherit_rate)
                           - _owner->_defend_degree;
    enh = _owner->_anti_defense_degree + _owner->_anti_resistance_degree;
    tmp_data.phy_inherit = (int)((double)enh * cfg->physical_penetration_inherit_rate)
                         - _owner->_anti_defense_degree;
    tmp_data.mag_inherit = (int)((double)enh * cfg->magic_penetration_inherit_rate)
                         - _owner->_anti_resistance_degree;
    tmp_data.time_reduce = (int)(cfg->enchant_time_reduce * 100.0)
                         - GNET::SkillWrapper::GetPraySpeed(&_owner->_skill);
    tmp_data.skill_count = 0;
    if (cfg->id_kid_skill)
    {
        const KID_SKILL_CONFIG* cfg2 = (const KID_SKILL_CONFIG*)
            itemdataman::get_data_ptr(world_manager::GetDataMan(),
                cfg->id_kid_skill, ID_SPACE::ID_SPACE_CONFIG, dt);
        if (cfg2 && dt == DT_KID_SKILL_CONFIG)
        {
            for (int i = 0; i <= 15; ++i)
            {
                if (cfg2->skill[i].id <= 0)
                    continue;
                for (int j = 9; j >= 0; --j)
                {
                    if (cfg2->skill[i].level[j] && sel._lvl >= cfg2->skill[i].level[j])
                    {
                        tmp_data.skill[2 * tmp_data.skill_count]     = cfg2->skill[i].id;
                        tmp_data.skill[2 * tmp_data.skill_count + 1] = j + 1;
                        ++tmp_data.skill_count;
                        break;
                    }
                }
            }
        }
    }
    SetCoolDown((COOLDOWN_INDEX_KID_TRANSFORMATION, IDX_TIME_COOLDOWN);
    object_interface player(_owner);
    GNET::SkillWrapper::SetKidFilter(&_owner->_skill, player, &tmp_data.shape);
    _owner->_basic.hp = _owner->_cur_prop.max_hp;
    _owner->_runner->player_world_speak_info(1, 1, 1, tmp_data.skill_count, tmp_data.skill);
    _owner->PlayerGetProperty();
}
void player_kid::DeactivateTransform()
{
    if (!tmp_data.skill_count)
        return;
    for (int i = 0; i < tmp_data.skill_count; ++i)
        tmp_data.skill[2 * i + 1] = -tmp_data.skill[2 * i + 1];
    _owner->_runner->player_world_speak_info(1, 1, 1, tmp_data.skill_count, tmp_data.skill);
    _owner->PlayerGetProperty();
}
int player_kid::NEXT_DAY_TIME()
{
    time_t now = time(0);
    tm dt;
    localtime_r(&now, &dt);
    dt.tm_sec  = 0;
    dt.tm_min  = 0;
    dt.tm_hour = 0;
    return mktime(&dt) + 86400;
}