//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
39110  "ΨEsmagador de Exércitos"39111  "^ff5c00ΨEsmagador de Exércitos^ffcb4aAlcance: ^ffffffCorpo a Corpo^ffcb4aCusto de MP: ^96f5ff%d^ffffff^ffcb4aConjuração: ^ffffff1 ^ffcb4asegundo^ffcb4aEspera: ^ffffff3^ffcb4asegundos^ffcb4aVitalidade: ^ffffff10^ffcb4aArma: ^ffffffArmas corpo a corpo^ffcb4aBônus de Runa: ^ff5c00Poder do Dragão^ffcb4aCultivo necessário: ^ffffffAlma CaóticaGire sua arma em um arco enorme, lançandoum poderoso ataque contra todos os inimigos, a até ^96f5ff12^ffffffmetros, causando dano físico básico mais ^96f5ff7241^ffffff.^ff0000Efeito da Runa Escarlate:@1^ff0000Custa ^96f5ff50%%^ff0000 do Chi atual. Para cada Chi gasto o acerto é aumentado em ^96f5ff10%%^ff0000，^ff0000Nível 1 a 4: Para cada ^96f5ff100^ff0000 de Chi gasto, causa ^96f5ff125%%^ff0000 de dano físico básico.^ff0000Nível 5 a 7: Para cada ^96f5ff100^ff0000 de Chi gasto, causa ^96f5ff150%%^ff0000 de dano físico básico.^ff0000Nível 8 a 9: Para cada ^96f5ff100^ff0000 de Chi gasto, causa ^96f5ff175%%^ff0000 de dano físico básico.^ff0000Nível 10: Para cada ^96f5ff100^ff0000 de Chi gasto, causa ^96f5ff200%%^ff0000 de dano físico básico.@1@2^ff0000Custa ^96f5ff50%%^ff0000 do Chi atual. Para cada Chi gasto o acerto é aumentado em ^96f5ff10%%^ff0000，Para cada ^96f5ff100^ff0000 de Chi gasto, causa ^96f5ff%d%%^ff0000 de dano físico básico.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL911 
#define __CPPGEN_GNET_SKILL911 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill911:public Skill 
    { 
    public: 
        enum { SKILL_ID = 911 }; 
        Skill911 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill911Stub:public SkillStub 
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
                skill->GetPlayer ()->SetDecmp (800);
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
    Skill911Stub ():SkillStub (911) 
    { 
        cls                 = 3; 
#ifdef _SKILL_CLIENT 
        name                = L"�ʺ�ơ���"; 
        nativename          = "�ʺ�ơ���"; 
        icon                = "���.dds"; 
#endif 
        max_level           = 1; 
        type                = 3; 
        allow_ride          = 0; 
        attr                = 4; 
        rank                = 8; 
        eventflag           = 0; 
        is_senior           = 0; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 2452; 
        allow_forms         = 1; 
        apcost              = 20; 
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
        effect              = "���.sgc"; 
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
        restrict_weapons.push_back (292); 
        restrict_weapons.push_back (0); 
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
        static int aarray[10] = { 1000000,1000000,1000000,1000000,1000000,1000000,1000000,1000000,1000000,1000000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 79,79,79,79,79,79,79,79,79,79 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredItem (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredMoney (Skill * skill) const 
    { 
        static int aarray[10] = { 10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000 }; 
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
            return _snwprintf (buffer, length, format, 25, 800);

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
            skill->GetVictim ()->SetProbability (1.0 * 35);
            skill->GetVictim ()->SetTime (9000);
            skill->GetVictim ()->SetAmount (4500);
            skill->GetVictim ()->SetToxic (1);
            skill->GetVictim ()->SetProbability (1.0 * 35);
            skill->GetVictim ()->SetTime (9000);
            skill->GetVictim ()->SetAmount (4500);
            skill->GetVictim ()->SetBleeding (1);
            skill->GetVictim ()->SetProbability (1.0 * 15);
            skill->GetVictim ()->SetTime (9000);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetDecdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * 15);
            skill->GetVictim ()->SetTime (9000);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetDecresist (1);
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
            return (float) (32);
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
            return (float) (1);
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
