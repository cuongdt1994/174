#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_kid_dribis_generator.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"
#include "../luamanager.h"
#include "../emulate_settings.h"

DEFINE_SUBSTANCE(kid_dribis_generator_item,item_body,CLS_ITEM_KID_DEBRIS_GENERATOR)

int
kid_dribis_generator_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	DATA_TYPE dt;
	const KID_DEBRIS_GENERATOR_ESSENCE & ess = *(const KID_DEBRIS_GENERATOR_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_KID_DEBRIS_GENERATOR_ESSENCE);

	gplayer_imp* pImp = (gplayer_imp*)obj;
	LuaManager *lua = LuaManager::GetInstance();

	if (pImp)
	{
		unsigned int item_id, item_count;
        item_id = item_count = 0;
        int item_idx = abase::RandSelect(&(ess.list[0].probability),sizeof(ess.list[0]), 256);
        item_id = ess.list[item_idx].id;
        item_count = 1;
        
        if (item_id)
        {
			char MsgStr[256];
			if (EmulateSettings::GetInstance()->GetMsgLanguage() == 1) // PT-BR
				snprintf(MsgStr, sizeof(MsgStr), "^80ff80Abertura feita com sucesso, você pode verificar o item diretamente no inventário dos Celestiais. \n");
			else // US-UK
				snprintf(MsgStr, sizeof(MsgStr), "^80ff80Opening successful, you can check the item directly in the Celestials inventory. \n");

			lua->game__ChatMsg(0, pImp->_parent->ID.id, 0, MsgStr, -1);

            pImp->GiveTrashBoxItem(gplayer_imp::IL_TRASH_BOX8, item_id, item_count);

			return 1;
        }
	}

	char MsgStr2[256];
	if (EmulateSettings::GetInstance()->GetMsgLanguage() == 1) // PT-BR
		snprintf(MsgStr2, sizeof(MsgStr2), "^FF0000Houve uma Falha ao tentar fazer a abertura do item. \n");
	else // US-UK
		snprintf(MsgStr2, sizeof(MsgStr2), "^FF0000There was a Failure when trying to open the item. \n");

	obj->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
	return -1;
}