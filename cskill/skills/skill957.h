//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
39570  "Œ®L√¢mina de Cascalho"39571  "^ff5c00Œ®L√¢mina de Cascalho^ffcb4aAlcance ^96f5ff28^ffffff ^ffcb4ametros^ffcb4aCusto em MP ^96f5ff75^ffffff^ffcb4aCanaliza√ß√£o ^ffffff0,3 ^ffcb4asegundo^ffcb4aConjura√ß√£o ^ffffff0,7 ^ffcb4asegundo^ffcb4aEspera ^ffffff3,0 ^ffcb4asegundos^ffcb4aChi Recebido ^ffffff20^ffcb4aArma Necess√°ria: ^ffffffEspada^ffcb4aB√¥nus de Runa ^ff5c00Maestria Marcial^ffcb4aCultivo Necess√°rio ^ffffffAlma Ca√≥ticaDesfira um golpe devastador no inimigo, causando ^96f5ff110%%^ffffff de dano f√≠sico b√°sico mais ^96f5ff100%%^ffffff de dano de arma mais ^96f5ff1650^ffffff.Tem ^96f5ff60%%^ffffff de chance de aplicar o efeito Œ®Esp√≠rito Arruinado,fazendo com que seu ^96f5ffŒ®Ataque Geminiano^ffffff cause adicional de^96f5ff7950 ^ffffffem Dano de Metal e reduza o n√≠vel de Ataquee Defesa do alvo em ^96f5ff12^ffffff por ^96f5ff3^ffffff minutos.Tem ^96f5ff25%% ^ffffffde chance de aplicar o efeitoŒ®Gota de Sangue, fazendo com que seu ^96f5ffŒ®Ataque de Stalag^ffffff d√™um adicional de ^96f5ff3750 ^ffffffde dano eatordoe o alvo por ^96f5ff5^ffffff segundos.^ffcb4aEfeito da Runa √Åurea:@1^ffcb4aN√≠vel 1 a 4: Causa um dano de arma adicional de ^96f5ff30%%^ffcb4a e mais ^96f5ff3780^ffcb4a.^ffcb4aN√≠vel 5 a 7: Causa um dano de arma adicional de ^96f5ff45%% ^ffcb4a mais ^96f5ff4670^ffcb4a.^ffcb4aN√≠vel 8 a 9: Causa um dano de arma adicional de ^96f5ff60%% ^ffcb4a mais ^96f5ff5560^ffcb4a.^ffcb4aN√≠vel 10: Causa um dano de arma adicional de ^96f5ff75%% ^ffcb4amais ^96f5ff6450^ffcb4a.@1@2^ffcb4aCausa um dano de arma adicional de ^96f5ff%d%% ^ffcb4amais ^96f5ff%d^ffcb4a.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL957 
#define __CPPGEN_GNET_SKILL957 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill957:public Skill 
    { 
    public: 
        enum { SKILL_ID = 957 }; 
        Skill957 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill957Stub:public SkillStub 
    { 
    public: 
#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 50;
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
                return 0;
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
                return 50;
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
#ifdef _SKILL_SERVER 
        class State3:public SkillStub::State
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
    Skill957Stub ():SkillStub (957) 
    { 
        cls                 = 127; 
#ifdef _SKILL_CLIENT 
        name                = L"º”ÀŸ√‚‘Œºı…Àº”∂‹≥¨º∂±£ª§"; 
        nativename          = "º”ÀŸ√‚‘Œºı…Àº”∂‹≥¨º∂±£ª§"; 
        icon                = ""; 
#endif 
        max_level           = 10; 
        type                = 3; 
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
        effect              = "–Ó∆¯.sgc"; 
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
        pre_skills.push_back (std::pair < ID, int >(0, 1)); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
        statestub.push_back (new State2 ()); 
        statestub.push_back (new State3 ()); 
#endif 
    } 
        int GetExecutetime (Skill * skill) const
        {
            return 50;
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
        static int aarray[10] = { 1,2,3,4,5,6,7,8,9,10 }; 
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
            return (float) (60);
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
            return (float) (45);
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
            skill->GetVictim ()->SetTime (skill->GetMagicdamage ());
            skill->GetVictim ()->SetRatio (0.2 * skill->GetLevel ());
            skill->GetVictim ()->SetSpeedup (1);
            skill->GetVictim ()->SetTime (skill->GetMagicdamage ());
            skill->GetVictim ()->SetAmount (20000 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (1);
            skill->GetVictim ()->SetWingshield (1);
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (skill->GetMagicdamage ());
            skill->GetVictim ()->SetRatio (0.1 * skill->GetLevel ());
            skill->GetVictim ()->SetDechurt (1);
            skill->GetVictim ()->SetTime (skill->GetMagicdamage ());
            skill->GetVictim ()->SetFreemove (1);
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
            return (float) (30);
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
