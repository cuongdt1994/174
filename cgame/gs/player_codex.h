#ifndef __ONLINEGAME_GS_PLAYER_CODEX_H__
#define __ONLINEGAME_GS_PLAYER_CODEX_H__

#include <uchar.h>
#include <db_if.h>
#include <gsp_if.h>
#include <glog.h>

#include "luamanager.h"

class gplayer_codex
{
public:
	enum
	{
		MAX_STORAGE_FASHION = 1536,
		MAX_STORAGE_FLY = 512,
		MAX_STORAGE_MOUNT = 512,
		MAX_STORAGE_PET = 512,

	};

public:
	struct FASHION_OCTETS
	{
		struct FASHIONS
		{
			int fashion_id;
			int fashion_count_pos;
			int fashion_color[3];
		} fashions[MAX_STORAGE_FASHION];

		int fashion_first_reward;
		int fashion_title_reward;
		//int fashion_points; /*176+*/
	};

	struct FLYS_OCTETS
	{
#pragma pack(1)
		struct FLYS
		{
			int fly_id;
			char apprimored_level; // 0
			int max_time_speed;

		} flys[MAX_STORAGE_FLY];
#pragma pack()

		int fly_active; // ??
		int fly_first_reward;
		int fly_title_reward;
		//int fly_points; /*176+*/
	};

	struct MOUNTS_OCTETS
	{
#pragma pack(1)
		struct MOUNTS
		{
			int mount_id;
			int mount_pos;
			int mount_color;
			unsigned short pet_name_len; // 0
			char name[];
		} mounts[MAX_STORAGE_MOUNT];
#pragma pop()

		int mount_first_reward;
		int mount_title_reward;
		//int mount_points; /*176+*/
	};

	struct PETS_OCTETS
	{
#pragma pack(1)
		struct PETS
		{
			int pet_id;
			int pet_pos;
			int pet_color;
			unsigned short pet_name_len; // 0
			char name[];

		} pets[MAX_STORAGE_PET];
#pragma pop()

		int pet_first_reward;
		int pet_title_reward;
	};

	struct ACTIVE_FASHION
	{
		struct FASHION
		{
			int fashion_id;
			int fashion_color;
		};

		int mode_classic;
		FASHION fashions[5];
	};

public:
	int fashion_count;
	FASHION_OCTETS fashions;

	int fly_count;
	FLYS_OCTETS flys;

	int mount_count;
	MOUNTS_OCTETS mounts;

	int pet_count;
	PETS_OCTETS pets;

	ACTIVE_FASHION activate_fashion;

public:
	gplayer_codex() { memset(this, 0x00, sizeof(*this)); }
	~gplayer_codex() {}

	inline void Init()
	{
	}

	inline FASHION_OCTETS::FASHIONS *GetCodexFashions(int pos) { return &fashions.fashions[pos]; }
	inline ACTIVE_FASHION *GetCodexActivateFashion() { return &activate_fashion; }

	inline void SetCodexFashions(int pos, int fashionid, int fashioncolor, int fashioncountpos)
	{
		if (fashionid == 0)
			return;

		if(fashioncountpos > 3) fashioncountpos = 3;

		fashions.fashions[pos].fashion_id = fashionid;
		fashions.fashions[pos].fashion_count_pos = fashioncountpos;
		fashions.fashions[pos].fashion_color[fashioncountpos - 1] = fashioncolor;

		fashion_count++;
	}

	inline void SetNewColorFashion(int pos, int fashioncolor, int fashioncountpos)
	{		
		if(fashioncountpos > 3) fashioncountpos = 3;

		fashions.fashions[pos].fashion_count_pos = fashioncountpos;
		fashions.fashions[pos].fashion_color[fashioncountpos - 1] = fashioncolor;
	}

	inline void SetNewDyeColorFashion(int pos, int fashioncolor, int fashioncountpos)
	{		
		if(fashioncountpos > 3) fashioncountpos = 3;

		if(fashions.fashions[pos].fashion_count_pos < fashioncountpos)
		{
			fashions.fashions[pos].fashion_count_pos = fashioncountpos;
		}
		fashions.fashions[pos].fashion_color[fashioncountpos - 1] = fashioncolor;
	}

	inline bool SetCodexActivateFashion(int mode_classic, int *fashions)
	{
		ACTIVE_FASHION::FASHION *new_fashions = (ACTIVE_FASHION::FASHION *)fashions;

		activate_fashion.mode_classic = mode_classic;

		for (unsigned int i = 0; i < 5; i++)
		{
			activate_fashion.fashions[i].fashion_id = new_fashions[i].fashion_id;
			activate_fashion.fashions[i].fashion_color = new_fashions[i].fashion_color;
		}

		return true;
	}

	inline void SetCodexActivateFashionUnique(int pos)
	{
		activate_fashion.fashions[pos].fashion_id = 0;
		activate_fashion.fashions[pos].fashion_color = 0;
	}

	// Recompensas Roupas
	inline int GetCodexFashionsPoints() { return 0/*fashions.fashion_points176*/; }
	inline void SetCodexFashionPoints(int plus) { return/*fashions.fashion_points += plus176*/; }

	inline int GetCodexFashionsFirstReward() { return fashions.fashion_first_reward; }
	inline void SetCodexFashionsFirstReward(int idx) { fashions.fashion_first_reward = idx; }

	inline int GetCodexFashionsTitleReward() { return fashions.fashion_title_reward; }
	inline void SetCodexFashionsTitleReward() { fashions.fashion_title_reward++; }

	// Recompensas Voos
	inline int GetCodexFlysPoints() { return 0/*flys.fly_points 176*/; }
	inline void SetCodexFlysPoints(int plus) { return/*flys.fly_points += plus 176*/; }

	inline int GetCodexFlysFirstReward() { return flys.fly_first_reward; }
	inline void SetCodexFlysFirstReward(int idx) { flys.fly_first_reward = idx; }

	inline int GetCodexFlysTitleReward() { return flys.fly_title_reward; }
	inline void SetCodexFlysTitleReward() { flys.fly_title_reward++; }

	// Recompensas Montarias
	inline int GetCodexMountsPoints() { return 0 /*mounts.mount_points 176*/; }
	inline void SetCodexMountsPoints(int plus) { return /*mounts.mount_points += plus176*/; }

	inline int GetCodexMountsFirstReward() { return mounts.mount_first_reward; }
	inline void SetCodexMountsFirstReward(int idx) { mounts.mount_first_reward = idx; }

	inline int GetCodexMountsTitleReward() { return mounts.mount_title_reward; }
	inline void SetCodexMountsTitleReward() { mounts.mount_title_reward++; }

	// Recompensas Pets
	inline int GetCodexPetsFirstReward() { return pets.pet_first_reward; }
	inline void SetCodexPetsFirstReward(int idx) { pets.pet_first_reward = idx; }

	inline int GetCodexPetsTitleReward() { return pets.pet_title_reward; }
	inline void SetCodexPetsTitleReward() { pets.pet_title_reward++; }

	inline int GetCodexFashionsCount() { return fashion_count; }
	inline int GetCodexFlysCount() { return fly_count; }
	inline int GetCodexMountsCount() { return mount_count; }
	inline int GetCodexPetsCount() { return pet_count; }

	// Montarias
	inline MOUNTS_OCTETS::MOUNTS *GetCodexMounts(int pos) { return &mounts.mounts[pos]; }
	inline void SetCodexMounts(int pos, int mountid, int mountcolor, int mountcountpos)
	{
		if (mountid == 0)
			return;

		mounts.mounts[pos].mount_id = mountid;
		mounts.mounts[pos].mount_pos = mountcountpos;
		mounts.mounts[pos].mount_color = mountcolor;

		mount_count++;
	}

	// Pets
	inline PETS_OCTETS::PETS *GetCodexPets(int pos) { return &pets.pets[pos]; }
	inline void SetCodexPets(int pos, int petid, int petcolor, int petcountpos)
	{
		if (petid == 0)
			return;

		pets.pets[pos].pet_id = petid;
		pets.pets[pos].pet_pos = petcountpos;
		pets.pets[pos].pet_color = petcolor;

		pet_count++;
	}

	// Rename Pet
	inline bool SetCodexNamePet(int pos, const char *name, short name_len)
	{
		if (name_len > 16)
			name_len = 16;

		if (name_len > 0)
		{
			pets.pets[pos].pet_name_len = name_len;
			memcpy(pets.pets[pos].name, name, name_len);
			return true;
		}
		return false;
	}

	inline const char *GetCodexNamePet(int pos)
	{
		if (pets.pets[pos].pet_name_len > 0)
			return pets.pets[pos].name;
		return "";
	}

	inline short GetCodexNamePetLen(int pos)
	{
		return pets.pets[pos].pet_name_len;
	}

	// Voos
	inline FLYS_OCTETS::FLYS *GetCodexFlys(int pos) { return &flys.flys[pos]; }
	inline void SetCodexFlys(int pos, int flyid, char apprimored_level, int max_time_speed)
	{
		if (flyid == 0)
			return;

		flys.flys[pos].fly_id = flyid;
		flys.flys[pos].apprimored_level = apprimored_level;
		flys.flys[pos].max_time_speed = static_cast<unsigned int>(max_time_speed);

		fly_count++;
	}

	inline void SetCodexFlySpeed(int flyid, int new_max_time_speed)
	{
		for (int i = 0; i < MAX_STORAGE_FLY; i++)
		{
			if (flys.flys[i].fly_id == flyid)
			{
				flys.flys[i].max_time_speed = static_cast<unsigned int>(new_max_time_speed);
				break;
			}
		}
	}

	inline void AddApprimoredLevel(int pos)
	{
		if (pos < 0)
			return;

		flys.flys[pos].apprimored_level++;
	}
	inline int GetFlyActive() { return flys.fly_active; }
	inline void SetFlyActive(int fly_id)
	{
		flys.fly_active = fly_id;
	}

	inline void SendProtocolStorage(int roleid)
	{
		packet_wrapper h1_fashion(25600);

		struct OCTETS_FASH
		{
			struct FASHIONS
			{
				int fashion_id;
				int fashion_count_pos;
				int fashion_color[];

			} fashions[MAX_STORAGE_FASHION];

			int fashion_first_reward;
			int fashion_title_reward;
			//int fashion_points;  /*176+*/
		};

		OCTETS_FASH newfashions;

		for (unsigned int i = 0; i < fashion_count; i++)
		{
			newfashions.fashions[i].fashion_id = fashions.fashions[i].fashion_id;
			newfashions.fashions[i].fashion_count_pos = fashions.fashions[i].fashion_count_pos;

			h1_fashion.push_back(&newfashions.fashions[i].fashion_id, sizeof(int));
			h1_fashion.push_back(&newfashions.fashions[i].fashion_count_pos, sizeof(int));

			if (fashions.fashions[i].fashion_count_pos > 0)
			{
				for (unsigned int j = 0; j < fashions.fashions[i].fashion_count_pos; j++)
				{
					newfashions.fashions[i].fashion_color[j] = fashions.fashions[i].fashion_color[j];
					h1_fashion.push_back(&newfashions.fashions[i].fashion_color[j], sizeof(int));
				}
			}			
		}

		newfashions.fashion_first_reward = fashions.fashion_first_reward;
		newfashions.fashion_title_reward = fashions.fashion_title_reward;
		//newfashions.fashion_points = fashions.fashion_points;  /*176+*/

		h1_fashion.push_back(&newfashions.fashion_first_reward, sizeof(int));
		h1_fashion.push_back(&newfashions.fashion_title_reward, sizeof(int));
		//h1_fashion.push_back(&newfashions.fashion_points, sizeof(int));  /*176+*/

		packet_wrapper h1_flys(8192);

		struct OCTETS_FLS
		{
#pragma pack(1)
			struct FLYS
			{
				int fly_id;
				char apprimored_level; // 0
				int max_time_speed;

			} flys[MAX_STORAGE_FLY];
#pragma pop()

			int fly_active; // ??
			int fly_first_reward;
			int fly_title_reward;
			//int fly_points;  /*176+*/
		};

		OCTETS_FLS newflys;

		memcpy(newflys.flys, flys.flys, sizeof(OCTETS_FLS::FLYS) * fly_count);

		newflys.fly_active = flys.fly_active;
		newflys.fly_first_reward = flys.fly_first_reward;
		newflys.fly_title_reward = flys.fly_title_reward;
		//newflys.fly_points = flys.fly_points;  /*176+*/

		h1_flys.push_back(&newflys, sizeof(OCTETS_FLS::FLYS) * fly_count);
		h1_flys.push_back(&newflys.fly_active, sizeof(int));
		h1_flys.push_back(&newflys.fly_first_reward, sizeof(int));
		h1_flys.push_back(&newflys.fly_title_reward, sizeof(int));
		//h1_flys.push_back(&newflys.fly_points, sizeof(int));  /*176+*/

		packet_wrapper h1_mount(8192);

		struct OCTETS_MT
		{
#pragma pack(1)
			struct MOUNTS
			{
				int mount_id;
				int mount_pos;
				int mount_color;
				unsigned short pet_name_len; // 0
				char name[];

			} mounts[MAX_STORAGE_MOUNT];
#pragma pop()

			int mount_first_reward;
			int mount_title_reward;
			//int mount_points; /*176+*/
		};

		OCTETS_MT newmounts;

		memcpy(newmounts.mounts, mounts.mounts, sizeof(OCTETS_MT::MOUNTS) * mount_count);

		newmounts.mount_first_reward = mounts.mount_first_reward;
		newmounts.mount_title_reward = mounts.mount_title_reward;
		//newmounts.mount_points = mounts.mount_points;  /*176+*/

		int total_size = sizeof(OCTETS_MT::MOUNTS) * mount_count;
		for (int i = 0; i < mount_count; i++)
		{
			if (newmounts.mounts[i].pet_name_len > 0)
			{
				total_size += sizeof(char) * newmounts.mounts[i].pet_name_len;
			}
		}

		h1_mount.push_back(&newmounts, total_size);
		h1_mount.push_back(&newmounts.mount_first_reward, sizeof(int));
		h1_mount.push_back(&newmounts.mount_title_reward, sizeof(int));
		//h1_mount.push_back(&newmounts.mount_points, sizeof(int));  /*176+*/

		packet_wrapper h1_pets(8192);

		struct OCTETS_PT
		{
#pragma pack(1)
			struct PETS
			{
				int pet_id;
				int pet_pos;
				int pet_color;
				unsigned short pet_name_len; // 0
				char name[];

			} pets[MAX_STORAGE_PET];
#pragma pop()

			int pet_first_reward;
			int pet_title_reward;
		};

		OCTETS_PT newpets;
		int total_size_pet = sizeof(OCTETS_PT::PETS) * pet_count;

		for (int i = 0; i < pet_count; i++)
		{
			newpets.pets[i].pet_id = pets.pets[i].pet_id;
			newpets.pets[i].pet_pos = pets.pets[i].pet_pos;
			newpets.pets[i].pet_color = pets.pets[i].pet_color;
			newpets.pets[i].pet_name_len = pets.pets[i].pet_name_len;

			if (pets.pets[i].pet_name_len > 0)
			{
				total_size_pet += sizeof(char) * newpets.pets[i].pet_name_len;
				memcpy(newpets.pets[i].name, pets.pets[i].name, pets.pets[i].pet_name_len);
			}
		}

		newpets.pet_first_reward = pets.pet_first_reward;
		newpets.pet_title_reward = pets.pet_title_reward;

		h1_pets.push_back(&newpets, total_size_pet);
		h1_pets.push_back(&newpets.pet_first_reward, sizeof(int));
		h1_pets.push_back(&newpets.pet_title_reward, sizeof(int));

		GMSV::CodexRequestStorage(roleid, fashion_count, h1_fashion.size(), h1_fashion.data(),
								  fly_count, h1_flys.size(), h1_flys.data(), mount_count, h1_mount.size(), h1_mount.data(), pet_count, h1_pets.size(), h1_pets.data());
	}

public:
};

#endif
