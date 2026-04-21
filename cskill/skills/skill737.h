//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
37370  "Œ®Cria da Terra"37371  "^ff5c00Œ®Cria da Terra^ffcb4aAlcance ^96f5ff%d^ffffff ^ffcb4ametros^ffcb4aCusto em MP ^96f5ff%d^ffffff^ffcb4aCanaliza√ß√£o ^ffffff1,5 ^ffcb4asegundo^ffcb4aConjura√ß√£o ^ffffff0,8 ^ffcb4asegundo^ffcb4aEspera ^ffffff12,0 ^ffcb4asegundos^ffcb4aChi Recebido ^ffffff15^ffcb4aArma: ^ffffffDesarmado, Instrumentos M√°gicos^ffcb4aB√¥nus de Runa ^ff5c00Poder do Drag√£o^ffcb4aCultivo ^ffffffAlma CelestialLance um inseto t√≥xico que envenena o inimigo por ^96f5ff15 ^ffffffsegundos,causando dano m√°gico b√°sico mais ^96f5ff100%% ^ffffffdo dano da arma mais^96f5ff4320 ^ffffffem Dano de Terra. Atordoa o alvo por ^96f5ff4 ^ffffffsegundos. N√£o pode serum acerto cr√≠tico, mas tem a chance de ^96f5ffdobrar ^ffffffsua Taxa de Acerto Cr√≠ticopara causar ^96f5ff1,5 ^ffffffvezes mais dano. Este efeito acumula at√© ^96f5ff10 ^ffffffvezes com outrosefeitos de dano por tempo e pode ser acionada por certas habilidades.As habilidades do grupo Golpe Atordoante compartilham os mesmos ^96f5ff12^ffffff segundosde tempo de espera com as do grupo Cria da Terra.^ffeaabEfeito da Runa Arg√™ntea:@1^ffeaab^ffeaabN√≠vel 1 a 4: Causa dano direto igual a ^96f5ff20%% ^ffeaabdo ataque m√°gico b√°sico.^ffeaabN√≠vel 5 a 7: Causa dano direto igual a ^96f5ff30%% ^ffeaabdo ataque m√°gico b√°sico.^ffeaabN√≠vel 8 a 9: Causa dano direto igual a ^96f5ff40%% ^ffeaabdo ataque m√°gico b√°sico.^ffeaabN√≠vel 10: Causa dano direto igual a ^96f5ff50%% ^ffeaabdo ataque m√°gico b√°sico.@1@2^ffeaabCausa dano direto igual a ^96f5ff%d%% ^ffeaabdo ataque m√°gico b√°sico.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL737 
#define __CPPGEN_GNET_SKILL737 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill737:public Skill 
    { 
    public: 
        enum { SKILL_ID = 737 }; 
        Skill737 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill737Stub:public SkillStub 
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
    Skill737Stub ():SkillStub (737) 
    { 
        cls                 = 127; 
#ifdef _SKILL_CLIENT 
        name                = L"ŒÔ∆∑ººƒ‹-Õ¡∑¿Ã·…˝“©º¡"; 
        nativename          = "ŒÔ∆∑ººƒ‹-Õ¡∑¿Ã·…˝“©º¡"; 
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
        effect              = "π÷ŒÔ÷Œ¡∆ ı.sgc"; 
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
            skill->GetVictim ()->SetTime (20000);
            skill->GetVictim ()->SetRatio (3 * skill->GetLevel ());
            skill->GetVictim ()->SetEnhancesoil (1);
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
