#include "world.h"
#include "player_single_arena_imp.h"
#include "clstab.h"

DEFINE_SUBSTANCE(gplayer_single_arena_imp, gplayer_imp, 74); //DEFINE_SUBSTANCE(gplayer_single_arena_imp, gplayer_pvp_imp, 74);

/*
gactive_imp::attack_judge gplayer_single_arena_imp::GetPetAttackHook()
{
	return gplayer_single_arena_imp::__GetPetAttackHook;
}

gactive_imp::enchant_judge gplayer_single_arena_imp::GetPetEnchantHook()
{
	return gplayer_single_arena_imp::__GetPetEnchantHook;
}


bool gplayer_single_arena_imp::__GetPetAttackHook(gactive_imp* __this, const MSG& msg, attack_msg& amsg)
{
	amsg.is_invader = false;
	amsg.target_faction = -1;
	return true;
}

bool gplayer_single_arena_imp::__GetPetEnchantHook(gactive_imp* __this, const MSG& msg, enchant_msg& emsg)
{
	if(!emsg.helpful)
	{
		emsg.target_faction = -1;
	}
	
	emsg.is_invader = false;
	return true;
}

int gplayer_single_arena_imp::MessageHandler(world* pPlane, const MSG& msg)
{
	switch (msg.message)
	{
		case GM_MSG_ATTACK:
		{
			attack_msg& amsg = *(attack_msg*)msg.content;
			if(!(amsg.attacker_mode & attack_msg::PVP_FREE))
			{
				return gplayer_imp::MessageHandler(pPlane,msg);
			}
			
			attack_msg ack_msg = *(attack_msg*)msg.content;
			_filters.EF_TransRecvAttack(msg.source, ack_msg);
			
			XID attacker = ack_msg.ainfo.attacker;
			
			if (attacker.id == _parent->ID.id)
				return 0;
			
			//TODO
		    //(*((void (__stdcall **)(XID *))this->_vptr_substance + 232))(&rhs);
		    //if ( XID::operator==(&attacker, &rhs) )
		    //    return 0;
			if (attacker.id == OI_GetPetID().id)
				return 0;
		    //TODO END
		    
			if (attacker.IsPlayerClass())
			{
				if (IsInTeam() && IsMember(attacker))
				{
					return 0;
				}
				
				//if(!(ack_msg.target_faction & (GetFaction())))
				//{
				//	return 0;
				//}
				
				ack_msg.vigour = -1;
			}
			
			((gplayer_controller*)_commander)->TrySelect(msg.source);
			HandleAttackMsg(pPlane, msg, &ack_msg);
			return 0;
		}
		break;
		
		case GM_MSG_ENCHANT:
		{
			ASSERT(msg.content_length >= sizeof(enchant_msg));
			
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			_filters.EF_TransRecvEnchant(msg.source, ech_msg);
			
			if (!ech_msg.helpful)
			{
		    	XID attacker = ech_msg.ainfo.attacker;
		    	
				if (attacker.id == _parent->ID.id)
					return 0;
				
				//TODO
				//(*((void (__stdcall **)(XID *))this->_vptr_substance + 232))(&v5);
				//if ( XID::operator==(&attacker, &v5) )
				//    return 0;
				if (attacker.id == OI_GetPetID().id)
					return 0;
				//TODO END
				
				if (attacker.IsPlayerClass())
				{
					if (IsInTeam() && IsMember(attacker))
					{
						return 0;
					}
					
					//if(!(ech_msg.target_faction & (GetFaction())))
					//{
					//	return 0;
					//}
					
					ech_msg.target_faction = -1;
				}
				
				((gplayer_controller*)_commander)->TrySelect(msg.source);
			}
			else
			{
		    	XID attacker = ech_msg.ainfo.attacker;
		    	if (attacker.id != _parent->ID.id)
		    	{
					if (ech_msg.helpful == 1)
					{
						if ((_refuse_bless & C2S::REFUSE_NON_TEAMMATE_BLESS) 
								&& (!IsInTeam() || !IsMember(attacker))) return 0;
					}
					else if (ech_msg.helpful == 2)
					{
						if (_refuse_bless & C2S::REFUSE_NEUTRAL_BLESS) return 0;
					}
		    	}
			}
			
			ech_msg.is_invader = false;
			HandleEnchantMsg(pPlane,msg,&ech_msg);
			return 0;
		}
	}
	
	return gplayer_imp::MessageHandler(pPlane,msg);
}*/

int gplayer_single_arena_imp::MessageHandler(world* pPlane, const MSG& msg)
{
	switch(msg.message)
	{
		/*case GM_MSG_ATTACK:
			{
				attack_msg ack_msg = *(attack_msg*)msg.content;
				_filters.EF_TransRecvAttack(msg.source, ack_msg);
				XID attacker = ack_msg.ainfo.attacker;

				//这里无需区分玩家和NPC
				//自己不允许攻击
				if(attacker.id == _parent->ID.id) return 0;
				if(!(ack_msg.target_faction  & (GetFaction())))
				{
					//派系不正确则返回
					return 0;
				}
				ack_msg.target_faction = 0xFFFFFFFF;
				ack_msg.is_invader =  false;

				//试着选择对象
				((gplayer_controller*)_commander)->TrySelect(msg.source);
				HandleAttackMsg(pPlane,msg,&ack_msg);
				return 0;
			}
			break;*/
		case GM_MSG_ENCHANT:
			{
				enchant_msg ech_msg = *(enchant_msg*)msg.content;
				_filters.EF_TransRecvEnchant(msg.source, ech_msg);
				
				if (!ech_msg.helpful)
				{
					return gplayer_imp::MessageHandler(pPlane, msg);
				}
				else
				{
					XID attacker = ech_msg.ainfo.attacker;
					if (attacker.id != _parent->ID.id)
					{
						if (ech_msg.helpful == 1)
						{
							if ((_refuse_bless & C2S::REFUSE_NON_TEAMMATE_BLESS) && (!IsInTeam() || !IsMember(attacker)))
							{
								return 0;
							}
						}
						else if (ech_msg.helpful == 2)
						{
							if (_refuse_bless & C2S::REFUSE_NEUTRAL_BLESS)
							{
								return 0;
							}
						}
					}
				}
				ech_msg.is_invader = false;
				HandleEnchantMsg(pPlane, msg, &ech_msg);
				return 0;
			}
	}
	return gplayer_imp::MessageHandler(pPlane, msg);
}

void gplayer_single_arena_imp::OnHeartbeat(unsigned int tick)
{
	if (_parent->IsZombie())
	{
		if (!world_manager::GetWorldLimit().noauto_resurrect)
		{
			SendTo<0>(GM_MSG_GM_RESURRECT, _parent->ID, 0);
		}
	}
	
	gplayer_imp::OnHeartbeat(tick);
}
gactive_imp::attack_judge gplayer_single_arena_imp::GetPetAttackHook()
{
	return gplayer_single_arena_imp::__GetPetAttackHook;
}

gactive_imp::enchant_judge gplayer_single_arena_imp::GetPetEnchantHook()
{
	return gplayer_single_arena_imp::__GetPetEnchantHook;
}

template <typename MESSAGE,typename EFFECT_MSG>
inline static  bool PetTestHarmfulEffect(gactive_imp * __this, const MESSAGE & msg, EFFECT_MSG & emsg)
{
	emsg.is_invader =  false;
	//进行处理
	XID attacker = emsg.ainfo.attacker;
	//考虑组队
	if(attacker.IsPlayerClass())
	{
		//如果不是强制攻击则不允许攻击
		if(!emsg.force_attack) return false;

		//如果是自身或者队伍则攻击无效
		if(__this->OI_IsInTeam() && __this->OI_IsMember(attacker))
		{
			return 0;
		}

		//如果受攻击者是白名并且攻击消息是保护白名那么不会被攻击到
		int invader_state = __this->OI_GetInvaderState();
		//如果受攻击者是白名并且攻击消息是保护白名那么不会被攻击到
		if(invader_state == gactive_imp::INVADER_LVL_0 && (emsg.force_attack & C2S::FORCE_ATTACK_NO_WHITE))
		{
		        return false;
		}

		//如果开启了帮派保护，并且攻击者和被攻击者的帮派相同，则不会被攻击到
		int idmafia = emsg.ainfo.mafia_id;
		if(idmafia)
		{
			if(emsg.force_attack&C2S::FORCE_ATTACK_NO_MAFIA 
					&& idmafia == __this->OI_GetMafiaID()) return false;	
			if(emsg.force_attack&C2S::FORCE_ATTACK_NO_MAFIA_ALLIANCE 
					&& __this->OI_IsFactionAlliance(idmafia)) return false;	
		}

		//如果开启了势力保护，则同势力玩家不会被攻击
		if(emsg.ainfo.force_id && emsg.ainfo.force_id == __this->OI_GetForceID()) return false;
		
		//修正敌对目标
		emsg.target_faction = 0xFFFFFFFF;
	}
	return true;
}

bool gplayer_single_arena_imp::__GetPetAttackHook(gactive_imp * __this, const MSG & msg, attack_msg & amsg)
{
	//if( (!(amsg.attacker_mode & attack_msg::PVP_FREE)) || (amsg.attacker_mode & attack_msg::PVP_DUEL))
	//{
		//按照通常的判定来进行
		return gplayer_imp::__GetPetAttackHook(__this,msg,amsg);
	//}
	
	return PetTestHarmfulEffect(__this,msg,amsg);
}

bool gplayer_single_arena_imp::__GetPetEnchantHook(gactive_imp * __this, const MSG & msg,enchant_msg & emsg)
{
	/*if ( (!(emsg.attacker_mode & attack_msg::PVP_FREE)) || (emsg.attacker_mode & attack_msg::PVP_DUEL) )
	{
		return gplayer_imp::__GetPetEnchantHook(__this, msg,emsg);
	}*/
	
	if (!emsg.helpful)
		return PetTestHarmfulEffect(__this,msg,emsg);
	
	//有益法术可以随便使用，所以没有任何过滤和判断了
	emsg.is_invader = false;
	return true;
}
