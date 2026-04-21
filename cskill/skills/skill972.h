//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
39720  "Î¨LÃ¢mina do Vento"39721  "^ff5c00Î¨LÃ¢mina do Vento^ffcb4aAlcance ^96f5ff28^ffffff ^ffcb4ametros^ffcb4aMP ^96f5ff70^ffffff^ffcb4aCanalizaÃ§Ã£o ^ffffff0,2 ^ffcb4asegundo^ffcb4aConjuraÃ§Ã£o ^ffffff0,8 ^ffcb4asegundo^ffcb4aEspera ^ffffff2,0 ^ffcb4asegundos^ffcb4aChi Recebido ^ffffff10^ffcb4aArma NecessÃ¡ria ^ffffffEspada^ffcb4aBÃ´nus de Runa ^ff5c00Maestria Marcial^ffcb4aCultivo NecessÃ¡rio ^ffffffAlma CelestialCorte seu inimigo com um ataque rÃ¡pido e poderoso,causando ^96f5ff110%%^ffffff de dano fÃ­sico basemais ^96f5ff100%%^ffffff de dano de arma mais ^96f5ff1.815^ffffff de dano de ^ffcb4aMetal ^ffffff.^ff0000NÃ£o se aplica a nenhum efeito de status. Nunca erra.^ff0000Efeito da Runa Escarlate:@1^ff0000NÃ­vel 1 a 4: Causa um dano de arma adicional de ^96f5ff60%% ^ff0000 mais ^96f5ff1720^ff0000.^ff0000NÃ­vel 5 a 7: Causa um dano de arma adicional de ^96f5ff90%% ^ff0000 mais ^96f5ff2580^ff0000.^ff0000NÃ­vel 8 a 9: Causa um dano de arma adicional de ^96f5ff120%% ^ff0000 mais ^96f5ff3440^ff0000.^ff0000NÃ­vel 10: Causa um dano de arma adicional de ^96f5ff150%% ^ff0000mais ^96f5ff4300^ff0000.@1@2^ff0000Causa um dano de arma adicional de ^96f5ff%d%% ^ff0000mais ^96f5ff%d^ff0000.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL972 
#define __CPPGEN_GNET_SKILL972 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill972:public Skill 
    { 
    public: 
        enum { SKILL_ID = 972 }; 
        Skill972 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill972Stub:public SkillStub 
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
                skill->GetPlayer ()->SetDecelfmp (50);
                skill->GetPlayer ()->SetDecelfap (100 + skill->GetLevel () * 50);
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
    Skill972Stub ():SkillStub (972) 
    { 
        cls                 = 258; 
#ifdef _SKILL_CLIENT 
        name                = L"ÃðÆÇÕæ¾÷"; 
        nativename          = "ÃðÆÇÕæ¾÷"; 
        icon                = "ÃðÆÇÕæ¾÷.dds"; 
#endif 
        max_level           = 10; 
        type                = 3; 
        allow_ride          = 0; 
        attr                = 3; 
        rank                = 0; 
        eventflag           = 0; 
        is_senior           = 0; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 0; 
        allow_forms         = 1; 
        apcost              = 150050; 
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
        effect              = "ÃðÆÇÕæ¾÷.sgc"; 
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
            return 1000;
        }
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 560,1040,1680,2560,3720,5260,7300,10000,13620,18480 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 400015,400020,400025,400030,400035,400040,400045,400050,400055,400060 }; 
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
            return (float) (1 - 0.0111111 * (0));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (25);
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
            return _snwprintf (buffer, length, format, skill->GetLevel (), 50, 100 + skill->GetLevel () * 50, 1 + skill->GetLevel () * 0.5);

        }
#endif 
#ifdef _SKILL_SERVER 
        int GetEnmity (Skill * skill) const
        {
            return 100;
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
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (1000 + skill->GetLevel () * 500 + skill->GetT1 () * 100);
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetDeeppoision (1);
            return true;
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
            return (float) (28);
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
