//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
60840  "Dharma Mortal"60841  "|if&skilllv()>0&^00FF00&^ffffff||if&skilllv()>1&^00FFFF&||if&skilllv()>2&^d618e7&||if&skilllv()>3&^FF6600&|Darma Mortal                  ^ffffffNÃ­vel %d^ffcb4aAlcance ^ffffff30 ^ffcb4ametros^ffcb4aCanalizaÃ§Ã£o ^ffffff1,0 ^ffcb4asegundo^ffcb4aConjuraÃ§Ã£o ^ffffff1,0 ^ffcb4asegundo^ffcb4aEspera ^ffffff2,0 ^ffcb4asegundos   ^ffffffLanÃ§a uma onda de luz calorosa em seus aliados, recuperando seu HP em^96f5ff10%%^ffffffde seu Ataque MÃ¡gico. Depois disso, a onda de luz ricochetearÃ¡ para outra unidade aliadadentro de^96f5ff5^ffffff metros ao redor do alvo, recuperando o HP em ^96f5ff4%%^ffffffde seu Ataque MÃ¡gico. Pode ricochetear atÃ© no mÃ¡ximo ^96f5ff5^ffffff vezes.Os aliados recebendo o efeito de cura ganharÃ£o imunidade contraEfeitos de Congelado e Atordoado por ^96f5ff4^ffffff segundos.|if&skilllv()>1&LV2: ^ffffffCura em ^96f5ff30%%^ffffff do Ataque MÃ¡gico. ^ffffffRicochete cura em^96f5ff24%%^ffffff do Ataque MÃ¡gico. &^808080Nv2: Cura o HP em 30%% do Ataque MÃ¡gico. RicocheteCura 24%% do Ataque MÃ¡gico.||if&skilllv()>2&LV3: ^ffffffRicocheteia atÃ© ^96f5ff10^ffffff vezes. E ^808080Nv.3: Ricocheteia atÃ© 10 vezes. ||if&skilllv()>3&LV4: ^ffffffDistÃ¢ncia de Ricochete aumentada para ^96f5ff10^ffffff metros. E^808080Nv.4: A distÃ¢ncia do ricochete aumentada para 10 metros. |^ffcb4aCom o BebÃª Celestial no Nv.^ffffff30/70/90/105^ffcb4a, a Habilidade: Nv.^ffffff1/2/3/4^ffcb4a serÃ¡ liberada."*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL6084 
#define __CPPGEN_GNET_SKILL6084 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill6084:public Skill 
    { 
    public: 
        enum { SKILL_ID = 6084 }; 
        Skill6084 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill6084Stub:public SkillStub 
    { 
    public: 
#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1000;
            }
            bool Quit (Skill * skill) const
            {
                return false;
            }
            bool Loop (Skill * skill) const
            {
                return false;
            }
            bool Bypass (Skill * skill) const
            {
                return false;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Interrupt (Skill * skill) const
            {
                return false;
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
            bool Skip (Skill * skill) const
            {
                return 0;
            }
        };
#endif 
#ifdef _SKILL_SERVER 
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1000;
            }
            bool Quit (Skill * skill) const
            {
                return false;
            }
            bool Loop (Skill * skill) const
            {
                return false;
            }
            bool Bypass (Skill * skill) const
            {
                return false;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Interrupt (Skill * skill) const
            {
                return false;
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
            bool Skip (Skill * skill) const
            {
                return 0;
            }
        };
#endif 
#ifdef _SKILL_SERVER 
    class State3:public SkillStub::State 
    { 
    public: 
        int GetTime (Skill * skill) const { return 0; } 
        bool Quit (Skill * skill) const { return false; } 
        bool Loop (Skill * skill) const { return false; } 
        bool Bypass (Skill * skill) const { return false; } 
        void Calculate (Skill * skill) const { } 
        bool Interrupt (Skill * skill) const { return false; } 
        bool Cancel (Skill * skill) const { return 1; } 
        bool Skip (Skill * skill) const { return 0; } 
    }; 
#endif 
    Skill6084Stub ():SkillStub (6084) 
    { 
        cls                 = 262; 
#ifdef _SKILL_CLIENT 
        name                = L"ÖÚÉúÔµ"; 
        nativename          = "ÖÚÉúÔµ"; 
        icon                = "ÏÉÍ¯ÖÚÉúÔµ.dds"; 
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
        time_type           = 0; 
        long_range          = 0; 
        posdouble           = 0; 
        clslimit            = 0; 
        commoncooldown      = 0; 
        commoncooldowntime  = 0; 
        itemcost            = 0; 
        itemcostcount       = 0; 
        combosk_preskill    = 0; 
        combosk_interval    = 0; 
        combosk_nobreak     = 1; 
#ifdef _SKILL_CLIENT 
        effect              = "¸¨Öúº¢×Ó_ÖÎÁÆÁ´»÷ÖÐ01.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 0; 
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
        statestub.push_back (new State2 ()); 
        statestub.push_back (new State3 ()); 
#endif 
    } 
    int GetExecutetime (Skill * skill) const 
    { 
        static int aarray[10] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetCoolingtime (Skill * skill) const
        {
            return 2000;
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
        static float aarray[10] = { 30.00,30.00,30.00,30.00,30.00,30.00,30.00,30.00,30.00,30.00 }; 
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
		skill->GetVictim()->SetTime(4000.0);
		skill->GetVictim()->SetFreemove(1);

		int _v = skill->GetLevel() <= 1 ? skill->GetT0() / 10 : 3 * skill->GetT0() / 10;
		skill->GetVictim()->SetProbability(100.0);
		skill->GetVictim()->SetValue((float)_v);
		skill->GetVictim()->SetHeal(1);

		int _range = skill->GetLevel() <= 2 ? 5 : 10;
		int _a2 = skill->GetLevel() | (_range << 16);
		skill->GetVictim()->SetRandBless(skill->GetT0(), _a2, skill->GetT2());
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
        return 35; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    float GetTalent0(PlayerWrapper* player) const
	{
		return player->GetMagicattack();
	} 
#endif 
#ifdef _SKILL_SERVER 
    float GetTalent1(PlayerWrapper* player) const
	{
		return player->GetAttackdegree();
	}
#endif 
#ifdef _SKILL_SERVER 
    float GetTalent2(PlayerWrapper* player) const
	{
		return player->GetId() + 0.001;
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
