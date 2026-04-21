//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
37390  "Î¨Praga dos Gafanhotos"37391  "^ff5c00Î¨Praga dos Gafanhotos ^ffcb4aAlcance ^96f5ff%d^ffffff ^ffcb4ametros^ffcb4aCusto em MP ^96f5ff%d^ffffff^ffcb4aCanalizaÃ§Ã£o ^ffffff3,0 ^ffcb4asegundos^ffcb4aConjuraÃ§Ã£o ^ffffff1,2 ^ffcb4asegundos^ffcb4aEspera ^ffffff30,0 ^ffcb4asegundos^ffcb4aArma ^ffffffDesarmado, Instrumentos MÃ¡gicos^ffcb4aBÃ´nus de Runa ^ff5c00FÃºria do Tigre^ffcb4aCultivo ^ffffffAlma CelestialInvoque um enxame de parasitas tÃ³xicos para atacar os inimigosa atÃ© ^96f5ff12 ^ffffffmetros do alvo. Causa dano mÃ¡gicobÃ¡sico mais ^96f5ff300%% ^ffffffdo dano da arma mais ^96f5ff8606 ^ffffffem Dano deMadeira. Enlouquece os alvos por ^96f5ff8^ffffff segundos, os impedindo de se moverem ou atacarem.Em ^96f5ff8 ^ffffffsegundos, ^ffffffcausa ^96f5ff30%% ^ffffffde dano adicional.Este efeito pode coexistir com a Chama do ParaÃ­so.Este efeito cancelarÃ¡ o Veneno Profundo.Gera ^96f5ff50 ^ffffffde Chi.As habilidades do grupo Alma da Raposa compartilham os ^96f5ff30^ffffff segundosde tempo de espera com as do grupo Praga dos Gafanhotos.^a6caf0Custa ^ffffff2 ^a6caf0Chis.^ffcb4aEfeito da Runa Ãurea:@1^ffcb4aNÃ­vel 1 a 4: Causa ^96f5ff60%% ^ffcb4ade dano de arma adicional.^ffcb4aNÃ­vel 5 a 7: Causa ^96f5ff90%% ^ffcb4ade dano de arma adicional.^ffcb4aNÃ­vel 8 a 9: Causa ^96f5ff120%% ^ffcb4ade dano de arma adicional.^ffcb4aNÃ­vel 10: Causa ^96f5ff150%% ^ffcb4ade dano adicional de arma.@1@2^ffcb4aCausa ^96f5ff%d%% ^ffcb4ade dano adicional de arma.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL739 
#define __CPPGEN_GNET_SKILL739 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill739:public Skill 
    { 
    public: 
        enum { SKILL_ID = 739 }; 
        Skill739 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill739Stub:public SkillStub 
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
    Skill739Stub ():SkillStub (739) 
    { 
        cls                 = 127; 
#ifdef _SKILL_CLIENT 
        name                = L"ÎïÆ·¼¼ÄÜ-Ë®¹¥¸½¼ÓÒ©¼Á"; 
        nativename          = "ÎïÆ·¼¼ÄÜ-Ë®¹¥¸½¼ÓÒ©¼Á"; 
        icon                = ""; 
#endif 
        max_level           = 5; 
        type                = 4; 
        allow_ride          = 0; 
        attr                = 1; 
        rank                = 0; 
        eventflag           = 0; 
        is_senior           = 0; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 0; 
        allow_forms         = 0; 
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
        combosk_nobreak     = 0; 
#ifdef _SKILL_CLIENT 
        effect              = ""; 
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
        pre_skills.push_back (std::pair < ID, int >(0, 1)); 
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
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 1,2,3,4,5,5,5,5,5,5 }; 
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
            return (float) (0);
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
            return _snwprintf (buffer, length, format);
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
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (119000);
            skill->GetVictim ()->SetRatio (0.4 * skill->GetLevel ());
            skill->GetVictim ()->SetIceblade (1);
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
            return (float) (250);
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
        {;
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetHitrate (Skill * skill) const
        {
            return (float) (0);
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
