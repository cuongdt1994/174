#include "countrybattle_manager.h"
#include "../world.h"
#include "../player_imp.h"
#include "../usermsg.h"
#include "countrybattle_ctrl.h"

/*
 *	´¦ÀíÊÀ½çÏûÏ¢
 */
int
countrybattle_world_message_handler::RecvExternMessage(int msg_tag, const MSG & msg)
{
	//¶ÔÓÚ¸±±¾£¬Ö»½ÓÊÜ¸øÍò¼ÒµÄÏûÏ¢
	if(msg.target.type != GM_TYPE_PLAYER && msg.target.type != GM_TYPE_SERVER ) return 0;
	if(msg_tag != world_manager::GetWorldTag())
	{
		//¸ô¶ÏÄ³Ğ©ÏûÏ¢.......
	}

	//»¹ĞèÒªÒª¸ô¶ÏÄ³Ğ©ÏûÏ¢
	//ÕâÀï»¹Ó¦¸ÃÖ±½Ó´¦ÀíÄ³Ğ©ÏûÏ¢
	//Êı¾İ×ª·¢ĞèÒª¾­¹ıÅĞ¶¨

	//ÓĞĞ©ÏûÏ¢ĞèÒª¾­¹ıÌØÊâ×ª·¢
	switch(msg.message)
	{
		case GM_MSG_SWITCH_USER_DATA:
			{
				if(msg.content_length < sizeof(instance_key)) return 0;
				instance_key * key = (instance_key*)msg.content;
				//ÏûÏ¢µÄÍ·²¿±ØĞëÊÇinstance_key
				//ºóÃæ¸½¼Ó×ÅÍæ¼ÒÊı¾İ
				//ASSERT(key->target.key_level1 == msg.source.id);
				//ÕâÀï²»ÏŞÖÆÁË£¬Ö÷ÒªÊÇGM»áÓĞ´ËĞĞÎª
				instance_hash_key hkey;
				_manager->TransformInstanceKey(key->target, hkey);
				int index = _manager->GetWorldByKey(hkey);
				if(index < 0) return 0;
				return _manager->GetWorldByIndex(index)->DispatchMessage(msg);
			}

		case GM_MSG_PLANE_SWITCH_REQUEST:
		//È·¶¨ÇĞ»»ÇëÇó 
		//¼ì²é¸±±¾ÊÀ½çÊÇ·ñ´æÔÚ£¬Èç¹û²»´æÔÚ£¬Ôò·ÅÈëµÈ´ıÁĞ±í£
		//Èç¹ûÒÑ¾­´æÔÚ£¬ÔòË¢ĞÂÒ»ÏÂ·şÎñÆ÷µÄÊ±¼ä±êÖ¾£¬²¢·µ»Ø³É¹¦±êÖ¾
		//Õâ¸ö²Ù×÷Òª½øĞĞËø¶¨£¬ÒÔÃâÉ¾³ıÊÀ½ç 
		{
			if(msg.content_length != sizeof(instance_key)) 
			{
				ASSERT(false);
				return 0;
			}
			instance_key * key = (instance_key*)msg.content;
			int rst = _manager->CheckPlayerSwitchRequest(msg.source,key,msg.pos,msg.param);
			if(rst == 0)
			{
				//½øĞĞ»ØÀ¡ÏûÏ¢
				MSG nmsg = msg;
				nmsg.target = msg.source;
				nmsg.source = msg.target;
				nmsg.message = GM_MSG_PLANE_SWITCH_REPLY;
				_manager->SendRemotePlayerMsg(msg.source.id, nmsg);
			}
			else if(rst > 0)
			{
				MSG nmsg;
				BuildMessage(nmsg,GM_MSG_ERROR_MESSAGE,msg.source,msg.target,msg.pos,rst);
				_manager->SendRemotePlayerMsg(msg.source.id, nmsg);
			}
			//Èç¹ûrstĞ¡ÓÚ0£¬±íÃ÷µ±Ç°ÎŞ·¨È·¶¨ÊÇ·ñÄÜ¹»´´½¨ÊÀ½ç£¬ĞèÒªµÈ´ı ËùÒÔ·´¶øÊ²Ã´¶¼²»×÷
		}
		return 0;

		case GM_MSG_CREATE_COUNTRYBATTLE:
		{
			country_battle_param & param = *(country_battle_param*) msg.content;
			_manager->CreateCountryBattle(param);
		}
		return 0;

		default:
		if(msg.target.type == GM_TYPE_PLAYER)
		{
			int index = _manager->GetPlayerWorldIdx(msg.target.id);
			if(index < 0) return 0;
			return _manager->GetWorldByIndex(index)->DispatchMessage(msg);
		}
		//·şÎñÆ÷ÏûÏ¢ÉĞÎ´´¦Àí ..........
	}
	return 0;
}

int
countrybattle_world_message_handler::HandleMessage(world * pPlane,const MSG & msg)
{
	//ÓĞĞ©ÏûÏ¢²Ù×÷¿ÉÄÜ»á±È½Ï·ÑÊ±¼ä£¬ÊÇ·ñ¿ÉÒÔ¿¼ÂÇTaskÍê³É£¬²»¹ıÓÃÏß³ÌµÄ»°¾ÍÒª¿¼ÂÇmsgµÄÊı¾İÎÊÌâÁË¡£

	switch(msg.message)
	{
		case GM_MSG_SWITCH_USER_DATA:
			return PlayerComeIn(_manager,pPlane,msg);

		default:
			world_message_handler::HandleMessage(pPlane,msg);
	}
	return 0;
}

void 
countrybattle_world_message_handler::PlayerPreEnterServer(gplayer * pPlayer, gplayer_imp * pImp,instance_key & ikey)
{	
	world * pPlane = pImp->_plane;
	
	//¸ù¾İÍæ¼ÒµÄ°ïÅÉÉè¶¨¹¥·½ºÍÊØ·½
	//Ôö¼ÓÈËÊıµÄ²Ù×÷ÔÚ Íæ¼ÒµÄEnterServer²Ù×÷ÖĞÍê³É
	countrybattle_ctrl * pCtrl = (countrybattle_ctrl*)(pPlane->w_ctrl);

	int id = pPlayer->GetCountryId();
	if(id)
	{
		if(id == pCtrl->_data.country_attacker)
		{
			//¹¥·½
			pPlayer->SetBattleOffense();
			//¼ì²éÈËÊıÉÏÏŞ ×¢Òâ¼õÉÙÈËÊıÉÏÏŞÊÇÔÚplayer_battleÀïµÄPlayerLeaveWorldÀï×öµÄ
			if(!pCtrl->AddAttacker())
			{
				//ÈËÊıÒÑÂú,Çå³ıÀïÃæµÄ¹ú¼ÒÄÚÈİ
				ikey.target.key_level5 = 0;

				//Çå³ıÕ½³¡µÄ±êÖ¾(ºóÃæÓĞÓÃ)
				pPlayer->ClrBattleMode();
			}
		}
		else
		if(id == pCtrl->_data.country_defender)
		{	
			//ÊØ·½
			pPlayer->SetBattleDefence();
			//¼ì²éÈËÊıÉÏÏŞ ×¢Òâ¼õÉÙÈËÊıÉÏÏŞÊÇÔÚplayer_battleÀïµÄPlayerLeaveWorldÀï×öµÄ
			if(!pCtrl->AddDefender())
			{
				//ÈËÊıÒÑÂú,Çå³ıÀïÃæµÄ¹ú¼ÒÄÚÈİ
				ikey.target.key_level5 = 0;

				//Çå³ıÕ½³¡µÄ±êÖ¾(ºóÃæÓĞÓÃ)
				pPlayer->ClrBattleMode();
			}
		}
	}
}

