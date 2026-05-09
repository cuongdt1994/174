//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
69660  "ΨEscudo Zênite"69661  "^ff5c00ΨEscudo Zênite^ffcb4aCusto em MP ^96f5ff%d^ffffff^ffcb4aCanalização ^ffffff0,2 ^ffcb4asegundo^ffcb4aConjuração ^ffffff0,3 ^ffcb4asegundo^ffcb4aEspera ^ffffff3,0 ^ffcb4asegundos^ffcb4aArma: ^ffffffDesarmado, Instrumentos Mágicos^ffcb4aBônus de Runa ^ff5c00Fúria do Tigre^ffcb4aCultivo ^ffffffAlma CelestialOs Magos Habilidosos criaram um método para unir três em um.Para armas, ganha um adicional de ^96f5ff120%%^ffffffde Defesa de Água, Fogo e Terra mais um adicional de ^96f5ff120%%^ffffff de dano físico. A cada ^96f5ff3^ffffff segundos,recebe ^96f5ff11^ffffff de Chi e aumenta a Taxa de Acertos Críticos em ^96f5ff11%%^ffffff para ^96f5ff15^ffffff minutos.^ff0000Efeito da Runa Escarlate:@1^ff0000Nível 1 a 4: O efeito de aumento da Defesa de Água, Fogo e Terra e a Defesa Física baseados no equipamento aumenta para^96f5ff135%%^ff0000^ff0000Nível 5 a 7: O efeito de aumento da Defesa de Água, Fogo e Terra e a Defesa Física baseados no equipamento aumenta para^96f5ff140%%^ff0000^ff0000Nível 8 a 9: O efeito de aumento da Defesa de Água, Fogo e Terra e a Defesa Física baseados no equipamento aumenta para^96f5ff145%%^ff0000^ff0000Nível 10: O efeito de aumento da Defesa de Água, Fogo e Terra e a Defesa Física baseados no equipamento aumenta para^96f5ff150%%^ff0000@1@2^ff0000O efeito de aumento da Defesa de Água, Fogo e Terra e a Defesa Física baseados no equipamento aumenta para^96f5ff%d%%^ff0000@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL966 
#define __CPPGEN_GNET_SKILL966 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill966:public Skill 
    { 
    public: 
        enum { SKILL_ID = 966 }; 
        Skill966 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill966Stub:public SkillStub 
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
                skill->GetPlayer ()->SetDecelfmp (75 + (skill->GetLevel () - 1) * 3);
                skill->GetPlayer ()->SetDecelfap (500 + 99 * (skill->GetLevel () - 1));
                skill->SetGolddamage ((25 + (skill->GetLevel () - 1) * 29 + skill->GetT0 () * 2) * 3.7);
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
    Skill966Stub ():SkillStub (966) 
    { 
        cls                 = 258; 
#ifdef _SKILL_CLIENT 
        name                = L"���׻�"; 
        nativename          = "���׻�"; 
        icon                = "���׻�.dds"; 
#endif 
        max_level           = 10; 
        type                = 1; 
        allow_ride          = 0; 
        attr                = 2; 
        rank                = 0; 
        eventflag           = 0; 
        is_senior           = 0; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 0; 
        allow_forms         = 1; 
        apcost              = 500099; 
        apgain              = 0; 
        doenchant           = 0; 
        dobless             = 0; 
        arrowcost           = 0; 
        allow_land          = 0; 
        allow_air           = 1; 
        allow_water         = 0; 
        notuse_in_combat    = 0; 
        restrict_corpse     = 0; 
        restrict_change     = 0; 
        restrict_attach     = 0; 
        auto_attack         = 0; 
        time_type           = 1; 
        long_range          = 2; 
        posdouble           = 2; 
        clslimit            = 4087; 
        commoncooldown      = 0; 
        commoncooldowntime  = 0; 
        itemcost            = 0; 
        itemcostcount       = 0; 
        combosk_preskill    = 0; 
        combosk_interval    = 0; 
        combosk_nobreak     = 0; 
#ifdef _SKILL_CLIENT 
        effect              = "���׻�.sgc"; 
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
            return 10000;
        }
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 2780,4000,5620,7780,10640,14480,19660,26600,36200,59000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 7000031,7000036,7000041,7000046,7000051,7000056,7000061,7000066,7000071,7000076 }; 
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
        static float aarray[10] = { 2,2,2,2,2,2,2,2,2,2 }; 
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
                               75 + (skill->GetLevel () - 1) * 3, 500 + 99 * (skill->GetLevel () - 1), (25 + (skill->GetLevel () - 1) * 29) * 3.7);

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
