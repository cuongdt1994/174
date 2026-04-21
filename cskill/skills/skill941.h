//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
39410  "ΨLança Glacial"39411  "^ff5c00ΨLança Glacial^ffcb4aAlcance ^96f5ff20^ffffff ^ffcb4ametros^ffcb4aCusto em MP ^96f5ff%d^ffffff^ffcb4aCanalização ^ffffff0,3 ^ffcb4asegundo^ffcb4aConjuração ^ffffff1,2 ^ffcb4asegundo^ffcb4aEspera ^ffffff30,0 ^ffcb4asegundos^ffcb4aArma: ^ffffffArma de Haste^ffcb4aBônus de Runa ^ff5c00Fúria do Tigre^ffcb4aCultivo Necessário ^ffffffAlma CelestialConcentre uma magia de gelo numa explosão de ^96f5ff12^ffffff metros de lanças glaciais, causando dano físico básico mais ^96f5ff100%%^ffffff de dano de arma mais^96f5ff8504^ffffff. Recupera^96f5ff50^ffffff Chi depois de usar. Esse ataque tem a garantia de acertar.Se o alvo estiver a até ^96f5ff10^ffffff metros de você,os danos Físico e Mágico dele serão reduzidos em^96f5ff%d%%^ffffff de dano da arma.(Redução da Defesa Física é aumentada para ^96f5ff90%%^ffffff de dano de arma.Redução da Defesa Mágica é aumentada para ^96f5ff60%%^ffffff de dano de arma.)^ffffffCongela os alvos afligidos a ^96f5ff10^ffffff metros de distância por ^96f5ff5^ffffff segundos.^a6caf0Custa ^ffffff2^a6caf0 Chis.^ff0000Efeito da Runa Escarlate:@1^ff0000 Em ^96f5ff6^ff0000 segundos, o alcance do próximo ^96f5ffΨGolpe do Sopro do Dragão^ff0000 aumenta em ^96f5ff10^ff0000 metros.^ff0000Nível 1 a 4: O dano do próximo ^96f5ffΨGolpe do Sopro do Dragão^ff0000 aumenta em ^96f5ff20%%.^ff0000Nível 5 a 7: O dano do próximo^96f5ffΨGolpe do Sopro do Dragão^ff0000 aumenta em^96f5ff30%%.^ff0000Nível 8 a 9: O dano do próximo^96f5ffΨGolpe do Sopro do Dragão^ff0000 aumenta em^96f5ff40%%.^ff0000 Nível 10 :O dano do próximo^96f5ffΨGolpe do Sopro do Dragão^ff0000aumenta em^96f5ff50%%.@1@2^ff0000Em ^96f5ff6^ff0000segundos, o alcance do próximo ^96f5ffΨGolpe do Sopro do Dragão^ff0000aumenta em^96f5ff10^ff0000metros e o dano aumenta em ^96f5ff%d%%.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL941 
#define __CPPGEN_GNET_SKILL941 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill941:public Skill 
    { 
    public: 
        enum { SKILL_ID = 941 }; 
        Skill941 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill941Stub:public SkillStub 
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
    Skill941Stub ():SkillStub (941) 
    { 
        cls                 = 127; 
#ifdef _SKILL_CLIENT 
        name                = L"ʱ����Ƭ.����"; 
        nativename          = "ʱ����Ƭ.����"; 
        icon                = ""; 
#endif 
        max_level           = 10; 
        type                = 4; 
        allow_ride          = 0; 
        attr                = 0; 
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
        allow_air           = 0; 
        allow_water         = 0; 
        notuse_in_combat    = 0; 
        restrict_corpse     = 0; 
        restrict_change     = 0; 
        restrict_attach     = 0; 
        auto_attack         = 0; 
        time_type           = 0; 
        long_range          = 1; 
        posdouble           = 0; 
        clslimit            = 0; 
        commoncooldown      = 0; 
        commoncooldowntime  = 2; 
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
            return (float) (1 - 0.0111111 * (0));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (0);
        }
    float GetMpcost (Skill * skill) const 
    { 
        static float aarray[10] = { 1,1,1,1,1,1,1,1,1,1 }; 
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
            skill->GetVictim ()->SetTime (300000);
            skill->GetVictim ()->SetRatio (1.5);
            skill->GetVictim ()->SetIncresist (1);
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
            return (float) (0);
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
