//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
10360  "√Årea Congelante"10361  "^ffffff√Årea Congelante (N√≠vel %d)^ffcb4aAlcance: ^ffffff18 metros^ffcb4aCusto de SP: ^96f5ff%d^ffffff^ffcb4aCusto de EP: ^96f5ff%d^ffffff^ffcb4aInstant√¢neo^ffcb4aEspera: ^ffffff20 ^ffcb4a segundos^ffcb4aClasse Requisitada: ^ffffffTodasCongela o ar em um raio de 18 metros √† sua volta.Causa %.1f de dano a todos os alvos na √°rea.Reduz as velocidades de ataque e canaliza√ß√£o em %d%% por 3 segundose oferece 60%% de chance de imobilizar os alvos por 3 segundos.^72fe00Destreza: A cada ponto de Destreza de Daimon, o dano √© aumentado em 13.4.^72fe00Destreza: A cada 4 pontos de Destreza de Daimon, o efeito de redu√ß√£o de velocidade √© aumentado em 1%%.^ff0000Us√°vel apenas na √°gua."*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL1036 
#define __CPPGEN_GNET_SKILL1036 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill1036:public Skill 
    { 
    public: 
        enum { SKILL_ID = 1036 }; 
        Skill1036 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill1036Stub:public SkillStub 
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
                skill->GetPlayer ()->SetDecelfmp (125 + (skill->GetLevel () - 1) * 3);
                skill->GetPlayer ()->SetDecelfap (801 + 99 * (skill->GetLevel () - 1));
                skill->SetWaterdamage ((78 + (skill->GetLevel () - 1) * 54 + skill->GetT1 () * 2) * 6.7);
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
    Skill1036Stub ():SkillStub (1036) 
    { 
        cls                 = 258; 
#ifdef _SKILL_CLIENT 
        name                = L"∫Æ±˘¡Ï”Ú"; 
        nativename          = "∫Æ±˘¡Ï”Ú"; 
        icon                = "∫Æ±˘¡Ï”Ú.dds"; 
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
        apcost              = 801099; 
        apgain              = 0; 
        doenchant           = 1; 
        dobless             = 0; 
        arrowcost           = 0; 
        allow_land          = 0; 
        allow_air           = 0; 
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
        effect              = "∫Æ±˘¡Ï”Ú.sgc"; 
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
        static int aarray[10] = { 20000,20000,20000,20000,20000,20000,20000,20000,20000,20000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 11320,15380,20800,28400,38400,65400,106600,169400,263600,359000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 60552,60557,60562,60567,60572,60577,60582,60587,60592,60597 }; 
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
            return (float) (20);
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
        static float aarray[10] = { 4,4,4,4,4,4,4,4,4,4 }; 
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
                               125 + (skill->GetLevel () - 1) * 3,
                               801 + 99 * (skill->GetLevel () - 1), (78 + 54 * (skill->GetLevel () - 1)) * 6.7, 20 + skill->GetLevel () * 3);

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
            skill->GetVictim ()->SetProbability (1.0 * 60);
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetFix (1);
            skill->GetVictim ()->SetProbability (1.0 * 60 + skill->GetT1 () * 0.2);
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetRatio (0.2 + skill->GetLevel () * 0.03 + skill->GetT1 () * 0.0025);
            skill->GetVictim ()->SetSlowattack (1);
            skill->GetVictim ()->SetProbability (1.0 * 60 + skill->GetT1 () * 0.2);
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetRatio (0.2 + skill->GetLevel () * 0.03 + skill->GetT1 () * 0.0025);
            skill->GetVictim ()->SetSlowpray (1);
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
            return (float) (21);
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
