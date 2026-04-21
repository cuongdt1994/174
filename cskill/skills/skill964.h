//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
69640  "Î¨Escudo ZÃªnite"69641  "^ff5c00Î¨Escudo ZÃªnite^ffcb4aCusto em MP ^96f5ff%d^ffffff^ffcb4aCanalizaÃ§Ã£o ^ffffff0,2^ffcb4asegundo^ffcb4aConjuraÃ§Ã£o ^ffffff0,3^ffcb4asegundo^ffcb4aEspera ^ffffff3,0^ffcb4asegundos^ffcb4aArma ^ffffffDesarmado, Instrumentos MÃ¡gicos^ffcb4aBÃ´nus de Runa ^ff5c00FÃºria do Tigre^ffcb4aCultivo ^ffffffAlma CelestialOs Magos Habilidosos criaram um mÃ©todo para unir trÃªs em um.Para armas, ganha um adicional de ^96f5ff120%%^ffffffde Defesa de Ãgua, Fogo e Terra mais um adicional de ^96f5ff120%%^ffffff de dano fÃ­sico. A cada ^96f5ff3^ffffff segundos,recebe ^96f5ff11^ffffff de Chi e aumenta a Taxa de Acertos CrÃ­ticos em ^96f5ff11%%^ffffff para ^96f5ff15^ffffff minutos.^00ff00Efeito da Runa Verdejante:@1^00ff00NÃ­vel 1 a 4: Receba um adicional de ^96f5ff60%%^00ff00 de Defesa de Madeira baseado no equipamento.^00ff00NÃ­vel 5 a 7: Receba um adicional de ^96f5ff80%%^00ff00 de Defesa de Madeira baseado no equipamento^00ff00NÃ­vel 8 a 9: Receba um adicional de ^96f5ff100%%^00ff00 de Defesa de Madeira baseado no equipamento^00ff00NÃ­vel 10: Receba um adicional de ^96f5ff120%%^00ff00 de Defesa de Madeira baseado no equipamento@1@2^00ff00Receba um adicional de ^96f5ff%d%%^00ff00 de Defesa de Madeira baseado no equipamento.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL964 
#define __CPPGEN_GNET_SKILL964 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill964:public Skill 
    { 
    public: 
        enum { SKILL_ID = 964 }; 
        Skill964 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill964Stub:public SkillStub 
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
                skill->GetPlayer ()->SetDecelfmp (65 + (skill->GetLevel () - 1) * 4);
                skill->GetPlayer ()->SetDecelfap (10 + 80 * skill->GetLevel ());
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
    Skill964Stub ():SkillStub (964) 
    { 
        cls                 = 258; 
#ifdef _SKILL_CLIENT 
        name                = L"Æø¸¿¾÷"; 
        nativename          = "Æø¸¿¾÷"; 
        icon                = "Æø¸¿¾÷.dds"; 
#endif 
        max_level           = 10; 
        type                = 3; 
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
        apcost              = 10080; 
        apgain              = 0; 
        doenchant           = 1; 
        dobless             = 1; 
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
        long_range          = 0; 
        posdouble           = 0; 
        clslimit            = 4092; 
        commoncooldown      = 0; 
        commoncooldowntime  = 0; 
        itemcost            = 0; 
        itemcostcount       = 0; 
        combosk_preskill    = 0; 
        combosk_interval    = 0; 
        combosk_nobreak     = 0; 
#ifdef _SKILL_CLIENT 
        effect              = "Æø¸¿¾÷.sgc"; 
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
            return 15000;
        }
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 2360,3460,4920,6840,9400,12800,17400,23600,32000,47800 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 6000029,6000034,6000039,6000044,6000049,6000054,6000059,6000064,6000069,6000074 }; 
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
            return (float) (18);
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
                               skill->GetLevel (), 65 + (skill->GetLevel () - 1) * 4, 10 + 80 * skill->GetLevel (), 100 - skill->GetLevel () * 5);

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
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (5000);
            skill->GetVictim ()->SetFix (1);
            skill->GetVictim ()->SetProbability (1.0 * skill->GetT0 () * 0.25);
            skill->GetVictim ()->SetTime (3000 + skill->GetT0 () * 10);
            skill->GetVictim ()->SetSealed (1);
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * 100 - skill->GetLevel () * 5);
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetFix (1);
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
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
