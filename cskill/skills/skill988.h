//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
69880  "Î¨Mente Taoista"69881  "^ff5c00Î¨Mente Taoista^ffcb4aCusto em MP ^96f5ff175^ffffff^ffcb4aCanalizaÃ§Ã£o ^ffffff0,1 ^ffcb4asegundo^ffcb4aConjuraÃ§Ã£o ^ffffff0,9 ^ffcb4asegundo^ffcb4aEspera ^ffffff120 ^ffcb4asegundos^ffcb4aChi Recebido ^ffffff10^ffcb4aArma ^ffffffAdagas^ffcb4aBÃ´nus de Runa ^ff5c00FÃºria do Tigre^ffcb4aCultivo ^ffffffAlma CelestialAumenta a sua chance de resistir a efeitos negativos em ^96f5ff100%%^ffffff. ^00ff00Efeito da Runa Verdejante:@1^00ff00NÃ­vel 1 a 4: A reduÃ§Ã£o do dano dura ^96f5ff25^00ff00 segundos.^00ff00NÃ­vel 5 a 7: A reduÃ§Ã£o do dano dura ^96f5ff30^00ff00 segundos.^00ff00NÃ­vel 8 a 9: A reduÃ§Ã£o do dano dura ^96f5ff35^00ff00 segundos.^00ff00NÃ­vel 10: A reduÃ§Ã£o de dano dura ^96f5ff40^00ff00 segundos.@1@2A reduÃ§Ã£o de dano dura ^96f5ff%d^00ff00 segundos.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL988 
#define __CPPGEN_GNET_SKILL988 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill988:public Skill 
    { 
    public: 
        enum { SKILL_ID = 988 }; 
        Skill988 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill988Stub:public SkillStub 
    { 
    public: 
#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 0;
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
                skill->GetPlayer ()->SetDecelfmp (60 + (skill->GetLevel () - 1) * 2);
                skill->GetPlayer ()->SetDecelfap (175 + 99 * (skill->GetLevel () - 1));
                skill->SetWaterdamage ((24 + (skill->GetLevel () - 1) * 22 + skill->GetT1 ()) * 5.2);
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Interrupt (Skill * skill) const
            {
                return false;
            }
            bool Cancel (Skill * skill) const
            {
                return 0;
            }
            bool Skip (Skill * skill) const
            {
                return 0;
            }
        };
#endif 
    Skill988Stub ():SkillStub (988) 
    { 
        cls                 = 258; 
#ifdef _SKILL_CLIENT 
        name                = L"ÊÉÁé¾÷"; 
        nativename          = "ÊÉÁé¾÷"; 
        icon                = "ÊÉÁé¾÷.dds"; 
#endif 
        max_level           = 10; 
        type                = 1; 
        allow_ride          = 0; 
        attr                = 4; 
        rank                = 0; 
        eventflag           = 0; 
        is_senior           = 0; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 0; 
        allow_forms         = 1; 
        apcost              = 175099; 
        apgain              = 0; 
        doenchant           = 0; 
        dobless             = 1; 
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
        posdouble           = 4; 
        clslimit            = 0; 
        commoncooldown      = 0; 
        commoncooldowntime  = 0; 
        itemcost            = 0; 
        itemcostcount       = 0; 
        combosk_preskill    = 0; 
        combosk_interval    = 0; 
        combosk_nobreak     = 0; 
#ifdef _SKILL_CLIENT 
        effect              = "ÊÉÁé¾÷.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 2; 
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
        pre_skills.push_back (std::pair < ID, int >(0, 0)); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
#endif 
    } 
        int GetExecutetime (Skill * skill) const
        {
            return 0;
        }
    int GetCoolingtime (Skill * skill) const
        {
            return 15000;
        }
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 1680,2560,3720,5260,7300,10000,13620,18480,25000,34000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 60025,60030,60035,60040,60045,60050,60055,60060,60065,60070 }; 
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
            return (float) (18);
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (0);
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (0));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (18);
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
#ifdef _SKILL_CLIENT 
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format) const
        {
            return _snwprintf (buffer, length, format,
                               skill->GetLevel (),
                               60 + (skill->GetLevel () - 1) * 2, 175 + 99 * (skill->GetLevel () - 1), (24 + (skill->GetLevel () - 1) * 22) * 5.2);

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
    bool StateAttack (Skill * skill) const 
    { 
        return 1; 
    } 
#endif 
#ifdef _SKILL_SERVER 
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp () * (0.01 + skill->GetT1 () * 0.01 * 0.1));
            skill->GetVictim ()->SetHeal (1);
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (40);
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent0 (PlayerWrapper * player) const
        {
            return (float) (player->GetElfstr ());
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent1 (PlayerWrapper * player) const
        {
            return (float) (player->GetElfagi ());
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
            return 20;
        }
#endif 
#ifdef _SKILL_SERVER 
        bool TakeEffect (Skill * skill) const
        {;
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetHitrate (Skill * skill) const
        {
            return (float) (1.0);
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
