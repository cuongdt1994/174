//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
10180  "Bênção do Arco-Íris"10181  "^ffffffBênção do Arco-Íris (Nível %d)^ffcb4aAlcance: ^ffffff30 metros^ffcb4aCusto de SP: ^96f5ff%d^ffffff^ffcb4aCusto de EP: ^96f5ff200^ffffff^ffcb4aInstantâneo^ffcb4aEspera: ^ffffff30 ^ffcb4a segundos^ffcb4aClasse Requisitada: ^ffffffSacerdoteFornece uma bênção aleatória para o alvo.Ataque físico aumentado em %d%%.Ataque mágico aumentado em %d%%.Chance de acerto crítico aumentada em %d%%.Tempo de canalização reduzido em %d%%.Dura 8 segundos.A bênção do acerto crítico sempre será aplicada;as outras três podem ou não ser também aplicadas.Não pode ser acumulada com efeitos similares.^72fe00Destreza: A cada 2 pontos de Destreza de Daimon, a taxa de sucesso da bênção do ataque físico é aumentada em 1%%.^72fe00Destreza: A cada 2 pontos de Destreza de Daimon, a taxa de sucesso da bênção do ataque mágico é aumentada em 1%%.^72fe00Destreza: A cada 4 pontos de Destreza de Daimon, a taxa de sucesso da bênção da velocidade de canalização é aumentada em 1%%."*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL1018 
#define __CPPGEN_GNET_SKILL1018 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill1018:public Skill 
    { 
    public: 
        enum { SKILL_ID = 1018 }; 
        Skill1018 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill1018Stub:public SkillStub 
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
                skill->GetPlayer ()->SetDecelfmp (55 + (skill->GetLevel () - 1) * 4);
                skill->GetPlayer ()->SetDecelfap (200);
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
    Skill1018Stub ():SkillStub (1018) 
    { 
        cls                 = 258; 
#ifdef _SKILL_CLIENT 
        name                = L"�ʺ�ף��"; 
        nativename          = "�ʺ�ף��"; 
        icon                = "�ʺ�ף��.dds"; 
#endif 
        max_level           = 10; 
        type                = 2; 
        allow_ride          = 0; 
        attr                = 5; 
        rank                = 0; 
        eventflag           = 0; 
        is_senior           = 0; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 0; 
        allow_forms         = 1; 
        apcost              = 200000; 
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
        clslimit            = 128; 
        commoncooldown      = 0; 
        commoncooldowntime  = 0; 
        itemcost            = 0; 
        itemcostcount       = 0; 
        combosk_preskill    = 0; 
        combosk_interval    = 0; 
        combosk_nobreak     = 0; 
#ifdef _SKILL_CLIENT 
        effect              = "�ʺ�ף��.sgc"; 
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
            return 30000;
        }
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 1040,1680,2560,3720,5260,7300,10000,13620,18480,25000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 1022020,1022025,1022030,1022035,1022040,1022045,1022050,1022055,1022060,1022065 }; 
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
            return (float) (30);
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
                               55 + (skill->GetLevel () - 1) * 4,
                               10 + 2 * skill->GetLevel (), 10 + 2 * skill->GetLevel (), skill->GetLevel (), skill->GetLevel () * 2);

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
            skill->GetVictim ()->SetProbability (1.0 * 5 + skill->GetLevel () + skill->GetT1 () * 0.5);
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetRatio (0.1 + 0.02 * skill->GetLevel ());
            skill->GetVictim ()->SetShowicon (1);
            skill->GetVictim ()->SetIncattack2 (1);
            skill->GetVictim ()->SetProbability (1.0 * 5 + skill->GetLevel () + skill->GetT1 () * 0.5);
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetRatio (0.1 + 0.02 * skill->GetLevel ());
            skill->GetVictim ()->SetShowicon (1);
            skill->GetVictim ()->SetIncmagic2 (1);
            skill->GetVictim ()->SetProbability (1.0 * skill->GetT1 () * 0.25);
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetRatio (skill->GetLevel () * 0.02);
            skill->GetVictim ()->SetFastpray2 (1);
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetValue (skill->GetLevel ());
            skill->GetVictim ()->SetIncsmite2 (1);
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
            return (float) (33);
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
