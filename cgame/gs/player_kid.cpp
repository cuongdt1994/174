#include <common/utf.h>
#include <threadpool.h>
#include <malloc.h>
#include <random>
#include <unordered_map>
#include <db_if.h>
#include "threadusage.h"
#include "world.h"
#include "worldmanager.h"
#include "arandomgen.h"
#include "player_imp.h"
#include "usermsg.h"
#include "public_quest.h"
#include "luamanager.h"
#include "emulate_settings.h"
#include "player_kid.h"
#include "cooldowncfg.h"

static const int card_level_1[] = { 66225, 66227, 66228, 66229, 66230, 66231 };
static const int card_level_2[] = { 66232, 66233, 66234, 66235, 66236, 66237, 66238, 66239 };
static const int card_level_3[] = { 66240, 66241, 66242, 66243, 66244, 66245, 66246 };
static const int card_level_4[] = { 66247, 66248, 66249, 66250, 66251, 66252, 66253 };
static const int card_level_5[] = { 66254 };

static const int exp_required_next_level[] = { 0, 2, 4, 8, 12, 20, 20, 20, 20, 20 };
static const int exp_min_level[]           = { 0, 0, 2, 6, 14, 26, 46, 66, 86, 106 };

void
gplayer_imp::KidAwakeningNameProtocol()
{
	_runner->kid_name_awakening(_kid.GetType(), _kid.GetNameLength(), _kid.GetName());
}

void
gplayer_imp::KidAwakeningInfoProtocol()
{
	if (_kid.GetNameLength() <= 0) return;

	packet_wrapper h1(512);

#pragma pack(push, 1)
	struct AWAKENING_INFO
	{
		struct COURSE_INFO
		{
			int  course_id;
			char course_level;
		};

		int         course_info[gplayer_kid::MAX_RANDOM_COURSE];
		char        course_level;
		char        course_random_cost;
		int         exp_course_required;
		COURSE_INFO course_equip[gplayer_kid::MAX_EQUIPED_COURSE];
		COURSE_INFO course_storage[gplayer_kid::MAX_STORAGE_COURSE];
		char        name[gplayer_kid::MAX_NAME_LENGTH];
		char        type;
		int         points_awakening;
		int         days_awakening;
		char        enabled_day;
		int         cash_awakening;
		char        block_day;
		char        reserve5;
	};
#pragma pack(pop)

	AWAKENING_INFO getinfo;
	memset(&getinfo, 0, sizeof(getinfo));

	for (unsigned int i = 0; i < gplayer_kid::MAX_RANDOM_COURSE; i++)
		getinfo.course_info[i] = _kid.GetRandomCourse(i);

	getinfo.course_level        = _kid.GetCardLevel();
	getinfo.course_random_cost  = _kid.GetCourseRandomCost();
	getinfo.exp_course_required = _kid.GetExpCourseRequired() + exp_min_level[_kid.GetCardLevel()];

	for (unsigned int i = 0; i < gplayer_kid::MAX_EQUIPED_COURSE; i++)
	{
		getinfo.course_equip[i].course_id    = _kid.GetEquipedCourse(i)->course_id;
		getinfo.course_equip[i].course_level = _kid.GetEquipedCourse(i)->course_level;
	}

	for (unsigned int i = 0; i < gplayer_kid::MAX_STORAGE_COURSE; i++)
	{
		getinfo.course_storage[i].course_id    = _kid.GetStorageCourse(i)->course_id;
		getinfo.course_storage[i].course_level = _kid.GetStorageCourse(i)->course_level;
	}

	memcpy(getinfo.name, _kid.GetName(), gplayer_kid::MAX_NAME_LENGTH);

	getinfo.type             = _kid.GetType() ? 1 : 4;
	getinfo.points_awakening = _kid.GetPointsAwakening();
	getinfo.days_awakening   = _kid.GetAwakeningDayCount();
	getinfo.enabled_day      = _kid.IsAwakening();
	getinfo.cash_awakening   = _kid.GetAwakeningCash();
	getinfo.block_day        = _kid.IsBlockDay();
	getinfo.reserve5         = true;

	h1.push_back(&getinfo, sizeof(getinfo));
	_runner->kid_awakening_info(h1.size(), h1.data());
}

void
gplayer_imp::KidCelestialInfoProtocol(int type)
{
	packet_wrapper h1(512);
	struct KID_INFO
	{
		int level;
		int rank;
		int exp;
		int idx;
		int atk;
		int atk_mag;
		int def;
		int def_mag[5];
		int hp;
		int crit;
	};

	int count = gplayer_kid::MAX_CELESTIAL;
	KID_INFO _kid_info[gplayer_kid::MAX_CELESTIAL];
	memset(_kid_info, 0, sizeof(_kid_info));

	for (unsigned int i = 0; i < gplayer_kid::MAX_CELESTIAL; i++)
	{
		_kid_info[i].level = _kid.GetCelestial(i)->level;
		_kid_info[i].rank  = _kid.GetCelestial(i)->rank;
		_kid_info[i].exp   = _kid.GetCelestial(i)->exp;
		_kid_info[i].idx   = _kid.GetCelestial(i)->idx;

		int level = _kid_info[i].level;
		int rank  = _kid_info[i].rank;

		DATA_TYPE data;
		const KID_PROPERTY_CONFIG *config = (const KID_PROPERTY_CONFIG *)
			world_manager::GetDataMan().get_data_ptr(_kid_info[i].idx, ID_SPACE_CONFIG, data);
		if (config && data == DT_KID_PROPERTY_CONFIG)
		{
			float mutiple_val = 0.0f;

			DATA_TYPE data2;
			const KID_UPGRADE_STAR_CONFIG *config2 = (const KID_UPGRADE_STAR_CONFIG *)
				world_manager::GetDataMan().get_data_ptr(gplayer_kid::IDX_KID_STAR_CONFIG, ID_SPACE_CONFIG, data2);
			if (config2 && data2 == DT_KID_UPGRADE_STAR_CONFIG)
			{
				if (rank > 0)
					mutiple_val += config2->upgrade_star_info[rank - 1].star_param;
				else
					mutiple_val += config2->zero_star_param;
			}

			_kid_info[i].atk     = (int)((config->damage        * level) * mutiple_val);
			_kid_info[i].atk_mag = (int)((config->magic_damage  * level) * mutiple_val);
			_kid_info[i].def     = (int)((config->defence        * level) * mutiple_val);

			for (unsigned int y = 0; y < 5; y++)
				_kid_info[i].def_mag[y] = (int)((config->magic_defence * level) * mutiple_val);

			_kid_info[i].hp   = (int)((config->hp * level) * mutiple_val);
			_kid_info[i].crit = (int)(static_cast<int>((config->crit_hit_probability * 100) * level) * mutiple_val);
		}
	}

	h1 << type << count;
	h1.push_back(&_kid_info, sizeof(_kid_info));
	_runner->kid_celestial_info(h1.size(), h1.data());
}

void
gplayer_imp::KidAwakeningCashProtocol()
{
	_runner->kid_awakening_cash_info(_kid.GetAwakeningCash(), _kid.GetAwakeningPotential());
}

void
gplayer_imp::KidAwakeningPercProtocol()
{
	_runner->kid_course_perc(_kid.GetCardLevel(), 0);
}

int
gplayer_imp::GetCardLevel(std::mt19937& rng, std::uniform_real_distribution<double>& dist, int level)
{
	std::uniform_int_distribution<int> d6(0, 5);
	std::uniform_int_distribution<int> d8(0, 7);
	std::uniform_int_distribution<int> d7(0, 6);

	double r = dist(rng) * 100.0;

	if (level == 1)  return (r <= 70) ? card_level_1[d6(rng)] : card_level_2[d8(rng)];
	if (level == 2)  return (r <= 60) ? card_level_1[d6(rng)] : ((r <= 95) ? card_level_2[d8(rng)] : card_level_3[d7(rng)]);
	if (level == 3)  return (r <= 50) ? card_level_1[d6(rng)] : ((r <= 90) ? card_level_2[d8(rng)] : card_level_3[d7(rng)]);
	if (level == 4)  return (r <= 40) ? card_level_1[d6(rng)] : ((r <= 80) ? card_level_2[d8(rng)] : card_level_3[d7(rng)]);
	if (level == 5)  return (r <= 30) ? card_level_1[d6(rng)] : ((r <= 65) ? card_level_2[d8(rng)] : ((r <= 95) ? card_level_3[d7(rng)] : card_level_4[d7(rng)]));
	if (level == 6)  return (r <= 30) ? card_level_1[d6(rng)] : ((r <= 60) ? card_level_2[d8(rng)] : ((r <= 90) ? card_level_3[d7(rng)] : card_level_4[d7(rng)]));
	if (level == 7)  return (r <= 24) ? card_level_1[d6(rng)] : ((r <= 54) ? card_level_2[d8(rng)] : ((r <= 84) ? card_level_3[d7(rng)] : ((r <= 99) ? card_level_4[d7(rng)] : card_level_5[0])));
	if (level == 8)  return (r <= 17) ? card_level_1[d6(rng)] : ((r <= 47) ? card_level_2[d8(rng)] : ((r <= 77) ? card_level_3[d7(rng)] : ((r <= 97) ? card_level_4[d7(rng)] : card_level_5[0])));
	if (level == 9)  return (r <= 15) ? card_level_1[d6(rng)] : ((r <= 40) ? card_level_2[d8(rng)] : ((r <= 70) ? card_level_3[d7(rng)] : ((r <= 95) ? card_level_4[d7(rng)] : card_level_5[0])));
	if (level == 10) return (r <= 10) ? card_level_1[d6(rng)] : ((r <= 35) ? card_level_2[d8(rng)] : ((r <= 60) ? card_level_3[d7(rng)] : ((r <= 90) ? card_level_4[d7(rng)] : card_level_5[0])));

	return -1;
}

void
gplayer_imp::GenerateCards(int level)
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<double> dist(0.0, 1.0);

	for (int i = 0; i < gplayer_kid::MAX_RANDOM_COURSE; ++i)
		_kid.SetRandomCourse(i, GetCardLevel(rng, dist, level));
}

bool
gplayer_imp::KidAwakeningCreate(char type, char name_len, const char name[])
{
	if (name_len > gplayer_kid::MAX_NAME_LENGTH)
	{
		_runner->error_message(633);
		return false;
	}

	if (_kid.GetNameLength() > 0 || _kid.GetAwakeningDayCount() > 0)
	{
		_runner->error_message(627);
		return false;
	}

	type = (type != 0) ? 1 : 0;

	_kid.SetNameLength(name_len);
	_kid.SetName(name);
	_kid.SetCardLevel(1);
	_kid.SetCourseRandomCost(false);
	GenerateCards(_kid.GetCardLevel());
	_kid.SetType(type);

	bool instant_awaken = EmulateSettings::GetInstance()->GetKidForceNewDay();
	if (instant_awaken)
	{
		int target = EmulateSettings::GetInstance()->GetChildAwakeningDays();
		_kid.SetAwakeningDayCount(target + 1);
		_kid.SetAwakening(false);
		_kid.SetBlockDay(true);
		_kid.SetCheckDay(false);
		_kid.SetAwakeningCash(EmulateSettings::GetInstance()->GetKidAwakeningCash());

		time_t nnow;
		time(&nnow);
		struct tm tm_buf = {};
		localtime_r(&nnow, &tm_buf);
		GetLua()->SetChildResetDay((char)tm_buf.tm_mday);
	}

	KidAwakeningNameProtocol();
	KidAwakeningInfoProtocol();

	if (instant_awaken)
	{
		KidAwakeningPercProtocol();
		KidAwakeningCashProtocol();
	}

	_runner->kid_created_info_dialog();
	return true;
}

void
gplayer_imp::KidUnlockNewDay()
{
	if (_kid.GetCheckDay())
	{
		_kid.SetCheckDay(false);
		_kid.SetBlockDay(true);
		_kid.SetAwakening(false);
		KidAwakeningInfoProtocol();
	}
}

bool
gplayer_imp::KidAwakeningNewDay()
{
	if (!_kid.GetCheckDay())
	{
		int target = EmulateSettings::GetInstance()->GetChildAwakeningDays();
		if (_kid.GetAwakeningDayCount() >= target + 1)
			return true;

		_kid.SetBlockDay(false);
		_kid.SetAwakening(true);
		_kid.SetAwakeningDayCount(_kid.GetAwakeningDayCount() + 1);
		_kid.SetAwakeningCash(EmulateSettings::GetInstance()->GetKidAwakeningCash());
		_kid.SetCourseRandomCost(false);
		_kid.SetCheckDay(true);

		KidAwakeningInfoProtocol();
		KidAwakeningPercProtocol();
		KidAwakeningCashProtocol();
	}

	return true;
}

int
gplayer_imp::KidGetSuitePoints()
{
	int points_recv = 0;
	std::unordered_map<unsigned int, int> suite_trigger_count;

	for (unsigned int i = 0; i < gplayer_kid::MAX_EQUIPED_COURSE; i++)
	{
		DATA_TYPE dt;
		COURSE_ESSENCE *ess = (COURSE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(
			_kid.GetEquipedCourse(i)->course_id, ID_SPACE_ESSENCE, dt);
		if (ess && dt == DT_COURSE_ESSENCE)
		{
			int cl = _kid.GetEquipedCourse(i)->course_level;
			if (cl >= 1 && cl <= 3)
				points_recv += ess->score[cl - 1];

			for (unsigned int suite_id : {66226, 66255, 66256, 66257, 66258, 66259, 66260, 66261, 66262, 66263, 66264, 66265})
			{
				DATA_TYPE dt2;
				COURSE_SUITE_ESSENCE *suite_ess = (COURSE_SUITE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(
					suite_id, ID_SPACE_ESSENCE, dt2);
				if (suite_ess && dt2 == DT_COURSE_SUITE_ESSENCE)
					if ((ess->course_mask & suite_ess->suite_mask) == suite_ess->suite_mask)
						suite_trigger_count[suite_id]++;
			}
		}
	}

	float mutiple_add = 0.0f;
	for (const auto &pair : suite_trigger_count)
	{
		DATA_TYPE dt3;
		COURSE_SUITE_ESSENCE *suite_ess = (COURSE_SUITE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(
			pair.first, ID_SPACE_ESSENCE, dt3);
		if (suite_ess && dt3 == DT_COURSE_SUITE_ESSENCE)
		{
			float max_bonus = 0.0f;
			for (int level = 0; level < 3; level++)
			{
				if (pair.second >= suite_ess->bonus_info[level].bonus_require_count)
				{
					float bonus = 0.0f;
					if (suite_ess->bonus_info[level].bonus_percent_add > 0)
						bonus = suite_ess->bonus_info[level].bonus_percent_add /
						        suite_ess->bonus_info[level].bonus_trigger_probability;
					if (bonus > max_bonus)
						max_bonus = bonus;
				}
			}
			if (max_bonus > 0.0f)
				mutiple_add += max_bonus;
		}
	}

	points_recv += (int)(points_recv * mutiple_add);

	if (points_recv > 0)
		points_recv = (int)(points_recv * (3.5f + 0.2f * _kid.GetAwakeningDayCount()));

	points_recv = (int)(points_recv * EmulateSettings::GetInstance()->GetKidPointsRate());

	return points_recv;
}

bool
gplayer_imp::KidAwakeningNewDay2()
{
	if (!_kid.GetCheckDay() || _kid.IsBlockDay())
		return true;

	int points_now  = _kid.GetPointsAwakening();
	int points_recv = 0;
	std::unordered_map<unsigned int, int> suite_trigger_count;

	for (unsigned int i = 0; i < gplayer_kid::MAX_EQUIPED_COURSE; i++)
	{
		DATA_TYPE dt;
		COURSE_ESSENCE *ess = (COURSE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(
			_kid.GetEquipedCourse(i)->course_id, ID_SPACE_ESSENCE, dt);
		if (ess && dt == DT_COURSE_ESSENCE)
		{
			int cl = _kid.GetEquipedCourse(i)->course_level;
			if (cl >= 1 && cl <= 3)
				points_recv += ess->score[cl - 1];

			for (unsigned int suite_id : {66226, 66255, 66256, 66257, 66258, 66259, 66260, 66261, 66262, 66263, 66264, 66265})
			{
				DATA_TYPE dt2;
				COURSE_SUITE_ESSENCE *suite_ess = (COURSE_SUITE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(
					suite_id, ID_SPACE_ESSENCE, dt2);
				if (suite_ess && dt2 == DT_COURSE_SUITE_ESSENCE)
					if ((ess->course_mask & suite_ess->suite_mask) == suite_ess->suite_mask)
						suite_trigger_count[suite_id]++;
			}
		}
	}

	float mutiple_add = 0.0f;
	for (const auto &pair : suite_trigger_count)
	{
		DATA_TYPE dt3;
		COURSE_SUITE_ESSENCE *suite_ess = (COURSE_SUITE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(
			pair.first, ID_SPACE_ESSENCE, dt3);
		if (suite_ess && dt3 == DT_COURSE_SUITE_ESSENCE)
		{
			float max_bonus = 0.0f;
			for (int level = 0; level < 3; level++)
			{
				if (pair.second >= suite_ess->bonus_info[level].bonus_require_count)
				{
					float bonus = 0.0f;
					if (suite_ess->bonus_info[level].bonus_percent_add > 0)
						bonus = suite_ess->bonus_info[level].bonus_percent_add /
						        suite_ess->bonus_info[level].bonus_trigger_probability;
					if (bonus > max_bonus)
						max_bonus = bonus;
				}
			}
			if (max_bonus > 0.0f)
				mutiple_add += max_bonus;
		}
	}

	points_recv += (int)(points_recv * mutiple_add);
	points_recv  = (int)(points_recv * EmulateSettings::GetInstance()->GetKidPointsRate());

	_kid.SetPointsAwakening(points_now + points_recv);
	_kid.SetAwakeningCash(0);
	_kid.SetBlockDay(true);

	_runner->kid_awakening_points(points_recv);
	return true;
}

bool
gplayer_imp::KidAwakeningCardLevel()
{
	char level_now     = _kid.GetCardLevel();
	if (level_now >= 10) return false;

	int exp_now        = _kid.GetExpCourseRequired();
	int cash_awakening = _kid.GetAwakeningCash();
	int cash_cost      = 4;

	if (cash_cost > cash_awakening) return false;
	_kid.SetAwakeningCash(cash_awakening - cash_cost);

	if (exp_now + 4 >= exp_required_next_level[level_now])
	{
		_kid.SetCardLevel(level_now + 1);
		int leftover = exp_now + 4 - exp_required_next_level[level_now];
		_kid.SetExpCourseRequired(leftover < 0 ? 0 : leftover);
	}
	else
	{
		_kid.SetExpCourseRequired(exp_now + 4);
	}

	{
		char new_level = _kid.GetCardLevel();
		int  base_exp  = (new_level < 10) ? exp_min_level[new_level] : 0;
		_runner->kid_course_perc(new_level, _kid.GetExpCourseRequired() + base_exp);
	}
	KidAwakeningCashProtocol();
	return true;
}

bool
gplayer_imp::KidAwakeningCardRandom()
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<double> dist(0.0, 1.0);

	int cash_awakening = _kid.GetAwakeningCash();
	int cash_cost      = 2;

	if (!_kid.GetCourseRandomCost())
	{
		cash_cost -= 2;
		_kid.SetCourseRandomCost(true);
	}

	if (cash_cost > cash_awakening) return false;
	_kid.SetAwakeningCash(cash_awakening - cash_cost);

	unsigned int cards[gplayer_kid::MAX_RANDOM_COURSE];
	for (int i = 0; i < gplayer_kid::MAX_RANDOM_COURSE; ++i)
	{
		cards[i] = GetCardLevel(rng, dist, _kid.GetCardLevel());
		_kid.SetRandomCourse(i, cards[i]);
	}

	_runner->kid_course_info(cards, gplayer_kid::MAX_RANDOM_COURSE);
	KidAwakeningCashProtocol();
	return true;
}

bool
gplayer_imp::KidAwakeningCardPutInventory(char old_slot)
{
	if ((unsigned int)old_slot >= gplayer_kid::MAX_RANDOM_COURSE)
		return false;

	int cash_awakening = _kid.GetAwakeningCash();
	int cash_cost      = 0;

	DATA_TYPE dt;
	COURSE_ESSENCE *ess = (COURSE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(
		_kid.GetRandomCourse(old_slot), ID_SPACE_ESSENCE, dt);
	if (!ess || dt != DT_COURSE_ESSENCE) return false;

	cash_cost += ess->cost;
	if (cash_cost > cash_awakening) return false;
	_kid.SetAwakeningCash(cash_awakening - cash_cost);

	int new_slot = -1;
	for (unsigned int i = 0; i < gplayer_kid::MAX_STORAGE_COURSE; i++)
	{
		if (_kid.GetStorageCourse(i)->course_id == 0 && new_slot < 0)
		{
			new_slot = i;
			break;
		}
	}
	if (new_slot < 0) return false;

	_kid.SetStorageCourse(new_slot, _kid.GetRandomCourse(old_slot), 1);
	_kid.SetRandomCourse(old_slot, 0);

	_runner->kid_course_change(old_slot, new_slot + 6);
	KidAwakeningCashProtocol();
	_runner->kid_system_points_notify(KidGetSuitePoints());
	return true;
}

void
gplayer_imp::KidAwakeningCardSwitchInventory(char new_slot, char old_slot1, char old_slot2)
{
	if (new_slot < 6 || (unsigned int)(new_slot  - 6) >= gplayer_kid::MAX_STORAGE_COURSE) return;
	if (old_slot1 < 6 || (unsigned int)(old_slot1 - 6) >= gplayer_kid::MAX_STORAGE_COURSE) return;
	if (old_slot2 < 6 || (unsigned int)(old_slot2 - 6) >= gplayer_kid::MAX_STORAGE_COURSE) return;
	if (new_slot == old_slot1 || new_slot == old_slot2 || old_slot1 == old_slot2) return;

	int get_level = _kid.GetStorageCourse(new_slot - 6)->course_level;
	int get_id    = _kid.GetStorageCourse(new_slot - 6)->course_id;

	if (get_level > 3 || get_id <= 0) return;

	int mat1_id    = _kid.GetStorageCourse(old_slot1 - 6)->course_id;
	int mat1_level = _kid.GetStorageCourse(old_slot1 - 6)->course_level;
	int mat2_id    = _kid.GetStorageCourse(old_slot2 - 6)->course_id;
	int mat2_level = _kid.GetStorageCourse(old_slot2 - 6)->course_level;

	if (mat1_id != get_id || mat2_id != get_id) return;
	if (mat1_level != get_level || mat2_level != get_level) return;

	_kid.SetStorageCourse(new_slot  - 6, get_id, get_level + 1);
	_kid.SetStorageCourse(old_slot1 - 6, 0, 0);
	_kid.SetStorageCourse(old_slot2 - 6, 0, 0);

	_runner->kid_course_switch(new_slot, old_slot1, old_slot2);
	_runner->kid_system_points_notify(KidGetSuitePoints());
}

bool
gplayer_imp::KidAwakeningCardRemoveInventory(char old_slot)
{
	const int max_slot = gplayer_kid::MAX_EQUIPED_COURSE + gplayer_kid::MAX_STORAGE_COURSE;
	if (old_slot < 0 || old_slot >= max_slot) return false;

	int cash_awakening = _kid.GetAwakeningCash();
	int cash_cost      = 0;
	int course_id      = 0;
	int course_level   = 0;

	if (old_slot < 6)
	{
		course_id    = _kid.GetEquipedCourse(old_slot)->course_id;
		course_level = _kid.GetEquipedCourse(old_slot)->course_level;
	}
	else
	{
		course_id    = _kid.GetStorageCourse(old_slot - 6)->course_id;
		course_level = _kid.GetStorageCourse(old_slot - 6)->course_level;
	}

	DATA_TYPE dt;
	COURSE_ESSENCE *ess = (COURSE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(
		course_id, ID_SPACE_ESSENCE, dt);
	if (!ess || dt != DT_COURSE_ESSENCE) return false;

	cash_cost += ess->cost;
	_kid.SetAwakeningCash(cash_awakening + cash_cost);

	if (old_slot < 6)
		_kid.SetEquipedCourse(old_slot, 0, 0);
	else
		_kid.SetStorageCourse(old_slot - 6, 0, 0);

	_runner->kid_course_remove(old_slot);
	KidAwakeningCashProtocol();
	_runner->kid_system_points_notify(KidGetSuitePoints());
	return true;
}

void
gplayer_imp::KidAwakeningCardMoveEquipInventory(char old_slot, char new_slot)
{
	const int max_slot = gplayer_kid::MAX_EQUIPED_COURSE + gplayer_kid::MAX_STORAGE_COURSE;
	if (old_slot < 0 || old_slot >= max_slot) return;
	if (new_slot < 0 || new_slot >= max_slot) return;
	if (old_slot == new_slot) return;

	if (old_slot < 6)
	{
		int  course_id    = _kid.GetEquipedCourse(old_slot)->course_id;
		char course_level = _kid.GetEquipedCourse(old_slot)->course_level;

		if (new_slot < 6)
			_kid.SetEquipedCourse(new_slot, course_id, course_level);
		else
			_kid.SetStorageCourse(new_slot - 6, course_id, course_level);

		_kid.SetEquipedCourse(old_slot, 0, 0);
	}
	else
	{
		int  course_id    = _kid.GetStorageCourse(old_slot - 6)->course_id;
		char course_level = _kid.GetStorageCourse(old_slot - 6)->course_level;

		if (new_slot < 6)
			_kid.SetEquipedCourse(new_slot, course_id, course_level);
		else
			_kid.SetStorageCourse(new_slot - 6, course_id, course_level);

		_kid.SetStorageCourse(old_slot - 6, 0, 0);
	}

	_runner->kid_course_insert(old_slot, new_slot);
	_runner->kid_system_points_notify(KidGetSuitePoints());
}

bool
gplayer_imp::KidAwakeningNewDay3()
{
	if (_kid.GetAwakeningDayCount() < EmulateSettings::GetInstance()->GetChildAwakeningDays())
		return true;

	int get_points = _kid.GetPointsAwakening();
	int gender     = _kid.GetType();
	int get_pos    = -1;

	if (gender != 0 && gender != 1) return false;

	DATA_TYPE data;
	const KID_QUALITY_CONFIG *config = (const KID_QUALITY_CONFIG *)
		world_manager::GetDataMan().get_data_ptr(gplayer_kid::IDX_KID_QUALITY_CONFIG, ID_SPACE_CONFIG, data);
	if (!config || data != DT_KID_QUALITY_CONFIG) return false;

	for (unsigned int i = 0; i < 4; i++)
	{
		if ((unsigned int)get_points >= config->list[i].require_score_min &&
		    (unsigned int)get_points <= config->list[i].require_score_max)
		{
			get_pos = i;
			break;
		}
	}

	if (get_pos < 0)
	{
		unsigned int best_min = 0;
		bool found = false;
		for (unsigned int i = 0; i < 4; i++)
		{
			if (config->list[i].require_score_min <= (unsigned int)get_points &&
			    (!found || config->list[i].require_score_min >= best_min))
			{
				best_min = config->list[i].require_score_min;
				get_pos  = (int)i;
				found    = true;
			}
		}
	}

	if (get_pos < 0 || get_pos >= 4) return false;

	int finish_idx = abase::RandSelect(
		&(config->list[get_pos].gender_list[gender].kid[0].probability),
		sizeof(config->list[get_pos].gender_list[gender].kid[0]), 8);
	if (finish_idx < 0 || finish_idx >= 8) return false;

	int  idx_item = config->list[get_pos].gender_list[gender].kid[finish_idx].id;
	bool item_get = false;

	DATA_TYPE data2;
	const KID_PROPERTY_CONFIG *config2 = (const KID_PROPERTY_CONFIG *)
		world_manager::GetDataMan().get_data_ptr(idx_item, ID_SPACE_CONFIG, data2);
	if (!config2 || data2 != DT_KID_PROPERTY_CONFIG) return false;

	int slot = config2->kid_debri_type;
	if (slot < 0 || slot >= (int)gplayer_kid::MAX_CELESTIAL) return false;

	if (_kid.GetCelestial(slot)->idx > 0)
	{
		if (_kid.GetCelestial(slot)->idx < idx_item)
		{
			_kid.SetCelestial(slot,
				_kid.GetCelestial(slot)->level,
				_kid.GetCelestial(slot)->rank,
				_kid.GetCelestial(slot)->exp + config2->kid_debri_exp,
				idx_item);
			item_get = true;
		}
		InvPlayerGiveItem(config2->kid_debri_id, 1);
	}
	else
	{
		if (config2->broadcast > 0)
		{
			SendClientMsgChild(_kid.GetName(), _kid.GetNameLength(), idx_item);
		}

		DATA_TYPE data3;
		const KID_LEVEL_MAX_CONFIG *config3 = (const KID_LEVEL_MAX_CONFIG *)
			world_manager::GetDataMan().get_data_ptr(6877, ID_SPACE_CONFIG, data3);
		if (!config3 || data3 != DT_KID_LEVEL_MAX_CONFIG) return false;

		unsigned int rahk = config2->rahk;
		if (rahk >= 10) return false;

		int newlevel = config3->level_max[rahk];
		_kid.SetCelestial(slot, newlevel < 1 ? 1 : newlevel, rahk >= 3 ? 1 : 0, 0, idx_item);
	}

	_kid.ClearAwakening();
	KidCelestialInfoProtocol(0);
	_runner->kid_celestial_awakening(item_get ? _kid.GetCelestial(slot)->rank : 0, idx_item);

	return true;
}

void
gplayer_imp::KidCelestialActivityProtocol()
{
	if (_kid.GetActivity()->active_slot < 0) return;
	_runner->kid_active_info(_kid.GetActivity()->active_slot, _kid.GetActivity()->reserved);
}

bool
gplayer_imp::KidCelestialActivity(int val1, int val2, int val3)
{
	if (val1 < 0 || val1 >= (int)gplayer_kid::MAX_CELESTIAL) return false;
	_kid.SetActivity(val1, -1);
	KidCelestialActivityProtocol();
	return true;
}

bool
gplayer_imp::KidCelestialUpgradeRank(int pos, int where, int inv_idx)
{
	if (pos < 0 || pos >= (int)gplayer_kid::MAX_CELESTIAL)
		return false;

	item_list &_trashbox = GetTrashInventory(IL_TRASH_BOX8);

	if (inv_idx < 0 || (unsigned int)inv_idx >= _trashbox.Size())
		return false;

	item &item_box = _trashbox[inv_idx];
	if (item_box.type <= 0 || item_box.count <= 0)
		return false;

	int celestial_idx = _kid.GetCelestial(pos)->idx;
	if (celestial_idx <= 0) return false;

	DATA_TYPE dt;
	const KID_DEBRIS_ESSENCE *ess = (KID_DEBRIS_ESSENCE *)world_manager::GetDataMan().get_data_ptr(
		item_box.type, ID_SPACE_ESSENCE, dt);
	if (!ess || dt != DT_KID_DEBRIS_ESSENCE) return false;

	DATA_TYPE dt2;
	const KID_PROPERTY_CONFIG *config2 = (const KID_PROPERTY_CONFIG *)world_manager::GetDataMan().get_data_ptr(
		celestial_idx, ID_SPACE_CONFIG, dt2);
	if (!config2 || dt2 != DT_KID_PROPERTY_CONFIG) return false;

	int total_count    = item_box.count;
	int base_stone_exp = ess->swallow_exp;
	if (base_stone_exp <= 0) return false;

	int current_exp   = _kid.GetCelestial(pos)->exp;
	int current_star  = _kid.GetCelestial(pos)->rank;
	int initial_star  = current_star;
	int new_idx       = celestial_idx;
	int stones_used   = 0;
	int protocol_mode = 0;

	while (total_count > 0 && config2->id_kid_upgrade != 0)
	{
		int required_exp = config2->upgrade_exp;
		if (required_exp <= 0) break;

		if (current_exp + base_stone_exp >= required_exp)
		{
			current_exp = (current_exp + base_stone_exp) - required_exp;
			new_idx     = config2->id_kid_upgrade;
			DATA_TYPE dt2_next;
			const KID_PROPERTY_CONFIG *config2_next = (const KID_PROPERTY_CONFIG *)world_manager::GetDataMan().get_data_ptr(
				new_idx, ID_SPACE_CONFIG, dt2_next);
			if (!config2_next || dt2_next != DT_KID_PROPERTY_CONFIG) break;
			config2 = config2_next;
			protocol_mode = 1;
		}
		else
		{
			current_exp += base_stone_exp;
			protocol_mode = 1;
		}
		total_count--;
		stones_used++;
	}

	if (total_count > 0 && config2->id_kid_upgrade == 0 && config2->kid_upgrade_star_config != 0)
	{
		DATA_TYPE dt3;
		const KID_UPGRADE_STAR_CONFIG *config3 = (const KID_UPGRADE_STAR_CONFIG *)world_manager::GetDataMan().get_data_ptr(
			config2->kid_upgrade_star_config, ID_SPACE_CONFIG, dt3);
		if (config3 && dt3 == DT_KID_UPGRADE_STAR_CONFIG)
		{
			while (total_count > 0)
			{
				if (current_star >= 6)
				{
					current_star = 6;
					current_exp  = 0;
					break;
				}

				int required_exp = config3->upgrade_star_info[current_star].start_exp;
				if (required_exp <= 0) break;

				if (current_exp + base_stone_exp >= required_exp)
				{
					current_exp = (current_exp + base_stone_exp) - required_exp;
					current_star++;
				}
				else
				{
					current_exp += base_stone_exp;
				}
				stones_used++;
				total_count--;
			}
		}
	}

	if (stones_used <= 0) return false;

	int fee_cost = 0;
	if (current_star > initial_star)
	{
		DATA_TYPE dt_exp;
		const KID_EXP_CONFIG *pCfgExp = (const KID_EXP_CONFIG *)world_manager::GetDataMan().get_data_ptr(
			gplayer_kid_addons::IDX_MAX_LEVEL_MONEY_COST, ID_SPACE_CONFIG, dt_exp);
		if (pCfgExp && dt_exp == DT_KID_EXP_CONFIG)
		{
			for (int i = initial_star; i < current_star && i < 150; ++i)
				fee_cost += pCfgExp->exp[i];
		}
	}

	if (fee_cost > 0 && !EmulateSettings::GetInstance()->GetKidFreeCelestialLevel())
	{
		if (GetAllMoney() < (unsigned int)fee_cost)
		{
			_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return false;
		}
		SpendAllMoney(fee_cost, true);
		SelfPlayerMoney();
	}

	if (SpendTrashBoxItem2(IL_TRASH_BOX8, inv_idx, stones_used))
	{
		_kid.SetCelestial(pos, _kid.GetCelestial(pos)->level, current_star, current_exp, new_idx);
		KidCelestialInfoProtocol(protocol_mode);
		return true;
	}

	return false;
}

void
gplayer_imp::KidCelestialTransformation(int mode)
{
	object_interface obj_if(this);

	// -------------------------------------------------------------------------
	// Deactivation
	// -------------------------------------------------------------------------
	if (!mode)
	{
		if (!_kid_transformation) return;

		_kid_transformation      = 0;
		_kid_transformation_time = 0;

		// read skill data from filter before removing it
		const int neg_count = _filters.QueryFilter(FILTER_KIDFORM, 0);
		int neg_skills[32] = {};
		for (int i = 0; i < neg_count; ++i)
		{
			neg_skills[2 * i]     =  _filters.QueryFilter(FILTER_KIDFORM, 1 + 2 * i);
			neg_skills[2 * i + 1] = -_filters.QueryFilter(FILTER_KIDFORM, 2 + 2 * i);
		}

		_filters.RemoveFilter(FILTER_KIDFORM);

		if (neg_count > 0)
			_runner->player_world_speak_info(1, 1, 1, neg_count, neg_skills);

		_runner->get_skill_data();
		PlayerGetProperty();
		return;
	}

	// -------------------------------------------------------------------------
	// Validation
	// -------------------------------------------------------------------------
	if (_kid_transformation) return;

	const bool has_conflicting_form =
		obj_if.IsFilterExist(FILTER_BEASTIEFORM) ||
		obj_if.IsFilterExist(FILTER_TIGERFORM)   ||
		obj_if.IsFilterExist(FILTER_FOXFORM)      ||
		obj_if.IsFilterExist(FILTER_SWIFTFORM)    ||
		obj_if.IsFilterExist(FILTER_FISHFORM)     ||
		obj_if.IsFilterExist(FILTER_THUNDERFORM)  ||
		obj_if.IsFilterExist(FILTER_SHADOWFORM)   ||
		obj_if.IsFilterExist(FILTER_FAIRYFORM)    ||
		obj_if.IsFilterExist(FILTER_GIANTFORM);

	if (has_conflicting_form)           { _runner->error_message(627); return; }
	if (!CheckCoolDown(COOLDOWN_INDEX_KID_TRANSFORMATION))
	                                    { _runner->error_message(628); return; }

	const int slot = _kid.GetActivity()->active_slot;
	if (slot < 0 || slot >= (int)gplayer_kid::MAX_CELESTIAL)
	                                    { _runner->error_message(53);  return; }

	const gplayer_kid::KID_STRUCT *celestial = _kid.GetCelestial(slot);
	if (!celestial || celestial->idx <= 0 || celestial->level <= 0)
	                                    { _runner->error_message(53);  return; }

	DATA_TYPE dt;
	const KID_PROPERTY_CONFIG *cfg = (const KID_PROPERTY_CONFIG *)
		world_manager::GetDataMan().get_data_ptr(celestial->idx, ID_SPACE_CONFIG, dt);
	if (!cfg || dt != DT_KID_PROPERTY_CONFIG)
	                                    { _runner->error_message(53);  return; }

	// -------------------------------------------------------------------------
	// Star rank multiplier
	// -------------------------------------------------------------------------
	float star_param = 0.0f;
	{
		DATA_TYPE dt_star;
		const KID_UPGRADE_STAR_CONFIG *cfg_star = (const KID_UPGRADE_STAR_CONFIG *)
			world_manager::GetDataMan().get_data_ptr(gplayer_kid::IDX_KID_STAR_CONFIG, ID_SPACE_CONFIG, dt_star);
		if (cfg_star && dt_star == DT_KID_UPGRADE_STAR_CONFIG)
		{
			star_param = (celestial->rank >= 1 && celestial->rank <= 6)
				? cfg_star->upgrade_star_info[celestial->rank - 1].star_param
				: cfg_star->zero_star_param;
		}
	}

	// -------------------------------------------------------------------------
	// Essence stats: base config value scaled by celestial level and star rank
	// -------------------------------------------------------------------------
	const int kid_lvl     = celestial->level;
	const int ess_hp      = (int)(cfg->hp                   * kid_lvl * star_param);
	const int ess_dmg     = (int)(cfg->damage               * kid_lvl * star_param);
	const int ess_dmg_mag = (int)(cfg->magic_damage         * kid_lvl * star_param);
	const int ess_def     = (int)(cfg->defence              * kid_lvl * star_param);
	const int ess_resist  = (int)(cfg->magic_defence        * kid_lvl * star_param);
	const int ess_crit    = (int)(cfg->crit_hit_probability * 100.0f * kid_lvl * star_param);

	// Blend: (essence + player_base) scaled by enhancement percent
	#define KID_STAT(essence, base, pct)  ((int)(((essence) + (base)) * 0.01f * (100 + (pct)) + 0.5f))

	const int enh_def     = (int)((3 * _cur_prop.strength + 2 * _cur_prop.vitality) * 0.04f + 0.5f);
	const int enh_res     = (int)((3 * _cur_prop.energy   + 2 * _cur_prop.vitality) * 0.04f + 0.5f);
	const int sum_atk_def = _attack_degree       + _defend_degree;
	const int sum_anti    = _anti_defense_degree + _anti_resistance_degree;

	// -------------------------------------------------------------------------
	// Filter buffer layout (consumed by SetKidFilter):
	//   [0]       unk1
	//   [1]       attack_type
	//   [2..12]   hp, dmg_lo, dmg_hi, mag_lo, mag_hi, def, resist[5]
	//   [13]      crit delta
	//   [14]      attack_speed delta
	//   [15..16]  range delta, speed delta (stored as float bits)
	//   [17..21]  degree/rank deltas
	//   [22]      skill_count
	//   [23+2*i]  skill_id[i],  [24+2*i] skill_level[i]
	// -------------------------------------------------------------------------
	int buf[56] = {};

	buf[0]  = cfg->unk1;
	buf[1]  = cfg->attack_type;
	buf[2]  = KID_STAT(ess_hp,      _cur_prop.max_hp,            _en_percent.max_hp);
	buf[3]  = KID_STAT(ess_dmg,     _cur_prop.damage_low,        _en_percent.damage + _en_percent.base_damage);
	buf[4]  = KID_STAT(ess_dmg,     _cur_prop.damage_high,       _en_percent.damage + _en_percent.base_damage);
	buf[5]  = KID_STAT(ess_dmg_mag, _cur_prop.damage_magic_low,  _en_percent.base_magic + _en_percent.magic_dmg);
	buf[6]  = KID_STAT(ess_dmg_mag, _cur_prop.damage_magic_high, _en_percent.base_magic + _en_percent.magic_dmg);
	buf[7]  = KID_STAT(ess_def,     _cur_prop.defense,           _en_percent.defense    + enh_def);
	buf[8]  = KID_STAT(ess_resist,  _cur_prop.resistance[0],     _en_percent.resistance[0] + enh_res);
	buf[9]  = KID_STAT(ess_resist,  _cur_prop.resistance[1],     _en_percent.resistance[1] + enh_res);
	buf[10] = KID_STAT(ess_resist,  _cur_prop.resistance[2],     _en_percent.resistance[2] + enh_res);
	buf[11] = KID_STAT(ess_resist,  _cur_prop.resistance[3],     _en_percent.resistance[3] + enh_res);
	buf[12] = KID_STAT(ess_resist,  _cur_prop.resistance[4],     _en_percent.resistance[4] + enh_res);
	buf[13] = ess_crit - _crit_rate - _base_crit_rate;
	buf[14] = _cur_prop.attack_speed - (int)(cfg->attack_interval * 20.0f + 0.00001f);

	{
		float range_delta = cfg->attack_dist - _cur_prop.attack_range;
		float speed_delta = cfg->walk_speed  - _cur_prop.run_speed;
		memcpy(&buf[15], &range_delta, sizeof(float));
		memcpy(&buf[16], &speed_delta, sizeof(float));
	}

	buf[17] = (int)(sum_atk_def * cfg->attack_lvl_rank_param)  - _attack_degree;
	buf[18] = (int)(sum_atk_def * cfg->defence_lvl_rank_param) - _defend_degree;
	buf[19] = (int)(sum_anti    * cfg->anti_defence_param)      - _anti_defense_degree;
	buf[20] = (int)(sum_anti    * cfg->anti_magic_param)        - _anti_resistance_degree;
	buf[21] = (int)(cfg->enchant_time_reduce * 100.0f)          - _skill.GetPraySpeed();

	#undef KID_STAT

	// -------------------------------------------------------------------------
	// Skills: for each slot, find the highest level unlocked by kid_lvl
	// -------------------------------------------------------------------------
	int skill_count = 0;
	if (cfg->id_kid_skill)
	{
		DATA_TYPE dt_sk;
		const KID_SKILL_CONFIG *cfg_sk = (const KID_SKILL_CONFIG *)
			world_manager::GetDataMan().get_data_ptr(cfg->id_kid_skill, ID_SPACE_CONFIG, dt_sk);

		if (cfg_sk && dt_sk == DT_KID_SKILL_CONFIG)
		{
			for (int i = 0; i < 16 && skill_count < 16; ++i)
			{
				if (cfg_sk->skill[i].id <= 0) continue;

				for (int j = 9; j >= 0; --j)
				{
					if (cfg_sk->skill[i].level[j] > 0 && kid_lvl >= cfg_sk->skill[i].level[j])
					{
						int *skill_entry = &buf[23 + 2 * skill_count];
						skill_entry[0] = cfg_sk->skill[i].id;
						skill_entry[1] = j + 1;
						++skill_count;
						break;
					}
				}
			}
		}
	}
	buf[22] = skill_count;

	// -------------------------------------------------------------------------
	// Activate transformation
	// -------------------------------------------------------------------------
	_kid_transformation      = 1;
	_kid_transformation_time = 29;

	_filters.ClearSpecFilter(filter::FILTER_MASK_DEBUFF);
	SetCoolDown(COOLDOWN_INDEX_KID_TRANSFORMATION, IDX_TIME_COOLDOWN);
	_skill.SetKidFilter(obj_if, buf);

	_basic.hp = _cur_prop.max_hp;

	if (skill_count > 0)
		_runner->player_world_speak_info(1, 1, 1, skill_count, buf + 23);

	PlayerGetProperty();
}

void
gplayer_imp::KidTransformEnd(int skill_count, int *skills)
{
	if (!_kid_transformation) return;

	_kid_transformation      = 0;
	_kid_transformation_time = 0;

	if (skill_count > 0)
	{
		int neg_skills[32] = {};
		for (int i = 0; i < skill_count; ++i)
		{
			neg_skills[2 * i]     =  skills[2 * i];
			neg_skills[2 * i + 1] = -skills[2 * i + 1];
		}
		_runner->player_world_speak_info(1, 1, 1, skill_count, neg_skills);
	}

	_runner->get_skill_data();
	PlayerGetProperty();
}

void
gplayer_imp::FixChildSystem()
{
	for (unsigned int i = 0; i < 6; i++)
	{
		if (_kid.GetCelestial(i)->idx <= 0) continue;

		DATA_TYPE data2;
		const KID_PROPERTY_CONFIG *config2 = (const KID_PROPERTY_CONFIG *)world_manager::GetDataMan().get_data_ptr(
			_kid.GetCelestial(i)->idx, ID_SPACE_CONFIG, data2);
		if (!config2 || data2 != DT_KID_PROPERTY_CONFIG) continue;

		DATA_TYPE data3;
		const KID_LEVEL_MAX_CONFIG *config3 = (const KID_LEVEL_MAX_CONFIG *)world_manager::GetDataMan().get_data_ptr(
			6877, ID_SPACE_CONFIG, data3);
		if (!config3 || data3 != DT_KID_LEVEL_MAX_CONFIG) return;

		if (config2->rahk >= 10) continue;
		int newlevel  = config3->level_max[config2->rahk];
		int raw_rank  = _kid.GetCelestial(i)->rank;
		if (raw_rank < 0 || raw_rank >= 10) continue;
		int old_level = config3->level_max[raw_rank];

		if (_kid.GetCelestial(i)->level > old_level)
		{
			_kid.SetCelestial(i,
				newlevel < 1 ? 1 : newlevel,
				config2->rahk >= 3 ? 1 : 0,
				_kid.GetCelestial(i)->exp,
				_kid.GetCelestial(i)->idx);
		}
	}
	KidCelestialInfoProtocol(0);
}

void
gplayer_imp::SendClientMsgChild(char *child_name, int child_name_len, int kid_id)
{
	struct
	{
		char player_name[MAX_USERNAME_LENGTH];
		char child_name[MAX_USERNAME_LENGTH_NOTIFY];
		int kid_id;
	} data;
	memset(&data, 0, sizeof(data));

	unsigned int len = _username_len;
	if (len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
	memcpy(data.player_name, _username, len);

	unsigned int len2 = (child_name_len > 0) ? (unsigned int)child_name_len : 0;
	if (len2 > MAX_USERNAME_LENGTH_NOTIFY) len2 = MAX_USERNAME_LENGTH_NOTIFY;
	memcpy(data.child_name, child_name, len2);

	data.kid_id = kid_id;

	packet_wrapper buf(sizeof(data));
	buf.push_back(&data, sizeof(data));

	broadcast_chat_msg(CHILD_AWAKENING_CHAT_MSG_ID, buf.data(), buf.size(), GMSV::CHAT_CHANNEL_SYSTEM, 0, 0, 0);
}
}
