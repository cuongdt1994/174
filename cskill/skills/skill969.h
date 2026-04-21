//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
39690  "ΨLâmina do Vento"39691  "^ff5c00ΨLâmina do Vento^ffcb4aAlcance ^96f5ff28^ffffff ^ffcb4ametros^ffcb4aCusto em MP ^96f5ff70^ffffff^ffcb4aCanalização ^ffffff0,2 ^ffcb4asegundo^ffcb4aConjuração ^ffffff0,8 ^ffcb4asegundo^ffcb4aEspera ^ffffff2,0 ^ffcb4asegundos^ffcb4aChi Recebido ^ffffff10^ffcb4aArma Necessária: ^ffffffEspada^ffcb4aBônus de Runa ^ff5c00Maestria Marcial^ffcb4aCultivo ^ffffffAlma CelestialCorte o inimigo com um golpe ágil e poderoso,causando ^96f5ff110%% ^ffffffde dano físico básico mais ^96f5ff100%% ^ffffffdo dano daarma mais ^96f5ff1815^ffffff. Tem ^96f5ff40%% ^ffffffde chance de causar um adicional de^96f5ff2060 ^ffffffde dano, imobilizar o alvo por ^96f5ff4 ^ffffffsegundos e reduziro nível de Defesa em ^96f5ff20 ^ffffff por ^96f5ff8 ^ffffffsegundos.^ffcb4aEfeito da Runa Áurea:@1^ffcb4aNível 1 a 4: Causa um dano de arma adicional de ^96f5ff20%% ^ffcb4a mais ^96f5ff1720^ffcb4a.^ffcb4aNível 5 a 7: Causa um dano de arma adicional de ^96f5ff30%% ^ffcb4a mais ^96f5ff2580^ffcb4a.^ffcb4aNível 8 a 9: Causa um dano de arma adicional de ^96f5ff40%% ^ffcb4a mais ^96f5ff3440^ffcb4a.^ffcb4aNível 10: Causa um dano de arma adicional de ^96f5ff50%% ^ffcb4amais ^96f5ff4300^ffcb4a.@1@2^ffcb4aCausa um dano de arma adicional de ^96f5ff%d%% ^ffcb4amais ^96f5ff%d^ffcb4a.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL969 
#define __CPPGEN_GNET_SKILL969 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill969:public Skill 
    { 
    public: 
        enum { SKILL_ID = 969 }; 
        Skill969 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill969Stub:public SkillStub 
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
                skill->GetPlayer ()->SetDecelfmp (70);
                skill->GetPlayer ()->SetDecelfap (200 + 15 * (skill->GetLevel () - 1));
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
    Skill969Stub ():SkillStub (969) 
    { 
        cls                 = 258; 
#ifdef _SKILL_CLIENT 
        name                = L"�޳���"; 
        nativename          = "�޳���"; 
        icon                = "�޳���.dds"; 
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
        apcost              = 200015; 
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
        effect              = "�޳���.sgc"; 
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
        static int aarray[10] = { 8000,8000,8000,8000,8000,8000,8000,8000,8000,8000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 2560,3720,5260,7300,10000,13620,18480,25000,34000,53200 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 700030,700035,700040,700045,700050,700055,700060,700065,700070,700075 }; 
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
            return _snwprintf (buffer, length, format,
                               skill->GetLevel (),
                               200 + 15 * (skill->GetLevel () - 1),
                               25 + 10 * skill->GetLevel (),
                               3 * skill->GetLevel (), 2 * skill->GetLevel (), 10 * skill->GetLevel (), 3 * skill->GetLevel ());

        }
#endif 
#ifdef _SKILL_SERVER 
        int GetEnmity (Skill * skill) const
        {
            return 10;
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
            skill->GetVictim ()->SetProbability (1.0 * skill->GetT2 () > 66 ? 100 : 0);
            skill->GetVictim ()->SetTime (9000);
            skill->GetVictim ()->SetShowicon (0);
            skill->GetVictim ()->SetAmount (2 * skill->GetLevel () + skill->GetT0 ());
            skill->GetVictim ()->SetValue (1);
            skill->GetVictim ()->SetApleakcont (1);
            skill->GetVictim ()->SetProbability (1.0 * skill->GetT2 () > 33 ? (skill->GetT2 () < 67 ? 100 : 0) : 0);
            skill->GetVictim ()->SetTime (9000);
            skill->GetVictim ()->SetAmount (3 * skill->GetLevel () * skill->GetT0 ());
            skill->GetVictim ()->SetMagicleak (1);
            skill->GetVictim ()->SetProbability (1.0 * skill->GetT2 () < 34 ? 100 : 0);
            skill->GetVictim ()->SetTime (9000);
            skill->GetVictim ()->SetAmount (25 + 10 * skill->GetLevel () * skill->GetT1 ());
            skill->GetVictim ()->SetToxic (1);
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (9000 + skill->GetT1 () * 50);
            skill->GetVictim ()->SetRatio (0.3);
            skill->GetVictim ()->SetSlow (1);
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
        float GetTalent2 (PlayerWrapper * player) const
        {
            return (float) (player->GetRand ());
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
