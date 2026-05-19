//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
60360  "Esconderijo DemonÃ­aco"60361  "|if&skilllv()>0&^00FF00&^ffffff||if&skilllv()>1&^00FFFF&||if&skilllv()>2&^d618e7&||if&skilllv()>3&^FF6600&|Esconderijo DemonÃ­aco                    ^ffffffNÃ­vel %d  ^ffcb4aCanalizaÃ§Ã£o ^ffffffInstantÃ¢nea^ffcb4aEspera ^96f5ff%.1f ^ffcb4asegundos   ^ffffffNanxin fica embaÃ§ada. Em ^96f5ff5^ffffff segundos, aumenta o seu NÃ­vel de Furtividade em ^96f5ff10^ffffff.|if&skilllv()>1&LV2: ^ffffffA Velocidade de Movimento Ã© aumentada em ^96f5ff50%%^ffffff.&^808080Nv. 2: Movimento de Nanxin aumenta em 50%%. if&skilllv()>2&LV3: Em ^ffffff^96f5ff10^ffffff segundos, o prÃ³ximo Dano CrÃ­tico Ã© aumentado em^96f5ff250%%^ffffff. E ^808080Nv.3: Em 10 segundos, o prÃ³ximo Dano CrÃ­tico aumenta em 250%%.|if&skilllv()>3&LV4: ^ffffffO movimento de Nanxin aumenta ao mÃ¡ximo enÃ£o pode ser selecionado. E^808080Nv.4: A velocidade de movimento de Nanxin aumenta ao mÃ¡x., enÃ£o pode ser selecionado.^ffcb4aNo NÃ­vel de BebÃª Celestial Nv.^ffffff30/70/90/105^ffcb4a, O Nv. De Habilidade:^ffffff1/2/3/4^ffcb4a serÃ¡ liberado."*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL6036 
#define __CPPGEN_GNET_SKILL6036 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill6036:public Skill 
    { 
    public: 
        enum { SKILL_ID = 6036 }; 
        Skill6036 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill6036Stub:public SkillStub 
    { 
    public: 
#ifdef _SKILL_SERVER 
    class State1:public SkillStub::State 
    { 
    public: 
        int GetTime (Skill * skill) const { return 0; } 
        bool Quit (Skill * skill) const { return false; } 
        bool Loop (Skill * skill) const { return false; } 
        bool Bypass (Skill * skill) const { return false; } 
        void Calculate (Skill * skill) const
        {
            skill->GetPlayer ()->SetPerform (1);
        } 
        bool Interrupt (Skill * skill) const { return false; } 
        bool Cancel (Skill * skill) const { return 1; } 
        bool Skip (Skill * skill) const { return 0; } 
    }; 
#endif 
    Skill6036Stub ():SkillStub (6036) 
    { 
        cls                 = 262; 
#ifdef _SKILL_CLIENT 
        name                = L"ÐÞÂÞÒþ"; 
        nativename          = "ÐÞÂÞÒþ"; 
        icon                = "ÏÉÍ¯ÐÞÂÞÒþ.dds"; 
#endif 
        max_level           = 4; 
        type                = 2; 
        allow_ride          = 0; 
        attr                = 7; 
        rank                = 0; 
        eventflag           = 0; 
        is_senior           = 0; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 0; 
        allow_forms         = 8; 
        apcost              = 0; 
        apgain              = 0; 
        doenchant           = 1; 
        dobless             = 0; 
        arrowcost           = 0; 
        allow_land          = 1; 
        allow_air           = 1; 
        allow_water         = 1; 
        notuse_in_combat    = 0; 
        restrict_corpse     = 0; 
        restrict_change     = 0; 
        restrict_attach     = 0; 
        auto_attack         = 0; 
        time_type           = 1; 
        long_range          = 0; 
        posdouble           = 0; 
        clslimit            = 0; 
        commoncooldown      = 0; 
        commoncooldowntime  = 0; 
        itemcost            = 0; 
        itemcostcount       = 0; 
        combosk_preskill    = 0; 
        combosk_interval    = 0; 
        combosk_nobreak     = 0; 
#ifdef _SKILL_CLIENT 
        effect              = "´Ì¿Íº¢×Ó_ÒþÉí.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 5; 
        has_stateattack     = 0; 
        runes_attr          = 0; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = ""; 
        gfxhangpoint        = "0"; 
#endif 
        gfxtarget           = 0; 
        feature             = 0; 
        extra_effect        = 0; 
        immune_casting      = 0; 
        restrict_weapons.push_back (1); 
        restrict_weapons.push_back (5); 
        restrict_weapons.push_back (9); 
        restrict_weapons.push_back (13); 
        restrict_weapons.push_back (182); 
        restrict_weapons.push_back (291); 
        restrict_weapons.push_back (292); 
        restrict_weapons.push_back (0); 
        restrict_weapons.push_back (23749); 
        restrict_weapons.push_back (25333); 
        restrict_weapons.push_back (44878); 
        restrict_weapons.push_back (44879); 
        restrict_weapons.push_back (59830); 
        restrict_weapons.push_back (59831); 
        restrict_weapons.push_back (69843); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
#endif 
    } 
    int GetExecutetime (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetCoolingtime (Skill * skill) const 
    { 
        static int aarray[10] = { 26000,22000,18000,14000,14000,14000,14000,14000,14000,14000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredItem (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredMoney (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredRealmLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    float GetRadius (Skill * skill) const 
    { 
        return (float) (0); 
    } 
    float GetAttackdistance (Skill * skill) const 
    { 
        return (float) (0); 
    } 
    float GetAngle (Skill * skill) const 
    { 
        return (float) (1); 
    } 
    float GetPraydistance (Skill * skill) const 
    { 
        static float aarray[10] = { 3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    float GetMpcost (Skill * skill) const 
    { 
        static float aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    bool CheckComboSkExtraCondition (Skill * skill) const 
    { 
        return 1; 
    } 
    int GetCoolDownLimit (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetCostShieldEnergy (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    }
	bool CheckHpCondition (int hp, int max_hp) const
    {
		return 1;
	}
#ifdef _SKILL_CLIENT 
    int GetIntroduction (Skill * skill, const wchar_t * buffer, int length, const wchar_t * format) const 
    { 
        return 0; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    int GetEnmity (Skill * skill) const 
    { 
        return 0; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    int GetMaxAbility (Skill * skill) const 
    { 
        return 0; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    bool StateAttack(Skill* skill) const
	{
		skill->GetVictim()->SetProbability(100.0);
		skill->GetVictim()->SetTime(5500.0);
		skill->GetVictim()->SetRatio(0.0);
		skill->GetVictim()->SetAmount(0.0);
		skill->GetVictim()->SetValue(5.0);
		skill->GetVictim()->SetInvisible2(1);

		float _float1;
		if (skill->GetLevel() <= 1)
			_float1 = 0.0;
		else
			_float1 = 100.0;
		skill->GetVictim()->SetProbability(_float1);
		skill->GetVictim()->SetTime(5500.0);

		float _float2;
		if (skill->GetLevel() <= 3)
			_float2 = 0.5;
		else
			_float2 = 2.0;
		skill->GetVictim()->SetRatio(_float2);
		skill->GetVictim()->SetSpeedup(1);
		skill->GetVictim()->SetTime(10500.0);
		skill->GetVictim()->SetRatio(0.5);
		skill->GetVictim()->SetAmount(1.0);
		skill->GetVictim()->SetInccritdamage2(1);

		float _float3;
		if (skill->GetLevel() <= 3)
			_float3 = 0.0;
		else
			_float3 = 100.0;
		skill->GetVictim()->SetProbability(_float3);
		skill->GetVictim()->SetTime(5500.0);
		skill->GetVictim()->SetForbidbeselected(1);

		return 1;
	}
#endif 
#ifdef _SKILL_SERVER 
    bool BlessMe (Skill * skill) const 
    { 
        return 1; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    float GetEffectdistance (Skill * skill) const 
    { 
        return 8; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    float GetTalent0 (PlayerWrapper * player) 
    { 
        return 0; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    float GetTalent1 (PlayerWrapper * player) 
    { 
        return 0; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    float GetTalent2 (PlayerWrapper * player) 
    { 
        return 0; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    float GetTalent3 (PlayerWrapper * player) 
    { 
        return 0; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    float GetTalent4 (PlayerWrapper * player) 
    { 
        return 0; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    int GetAttackspeed (Skill * skill) const 
    { 
        return 0; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    bool TakeEffect (Skill * skill) const 
    { 
        return 1; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    float GetHitrate (Skill * skill) const 
    { 
        return 3; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    void ComboSkEndAction (Skill * skill) const 
    { 
        return; 
    } 
#endif 
    }; 
} 
#endif 
