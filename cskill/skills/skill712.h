//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
37120  "Œ®Peso da Montanha"37121  "^ff5c00Œ®Peso da Montanha^ffcb4aAlcance ^96f5ff%d^ffffff ^ffcb4ametros^ffcb4aCusto em MP ^96f5ff%d^ffffff^ffcb4aCanaliza√ß√£o ^ffffff4,0 ^ffcb4asegundos^ffcb4aConjura√ß√£o ^ffffff1,0 ^ffcb4asegundo^ffcb4aEspera ^ffffff30,0 ^ffcb4asegundos^ffcb4aArma ^ffffffVarinha ou Desarmado^ffcb4aB√¥nus de Runa ^ff5c00Maestria Marcial^ffcb4aCultivo ^ffffffAlma CelestialConcentre seu Chi para invocar uma enorme massa de uma montanha pr√≥ximae a jogar sobre seu alvo e todos os inimigos a at√© ^96f5ff12^ffffff metros.Causa dano m√°gico b√°sico, mais ^96f5ff500%%^ffffff de dano da arma e mais ^96f5ff12149^ffffffde dano de Terra. Tem ^96f5ff95%%^ffffff de chance de atordoar o alvo por ^96f5ff6^ffffff segundos.Recupera ^96f5ff40^ffffff Chi ap√≥s o uso.^a6caf0Custa ^ffffff2^a6caf0 Chis^ff0000Efeito de Runa Escarlate@1^ff0000N√≠vel 1 a 4: Em ^96f5ff10^ff0000 segundos, reduz o custo de canaliza√ß√£o da pr√≥xima ^96f5ffŒ®Mar em Chamas^ff0000 para ^96f5ff160^ff0000N√≠vel 5 a 7: Em ^96f5ff10^ff0000 segundos, reduz o custo de canaliza√ß√£o da pr√≥xima ^96f5ffŒ®Mar em Chamas^ff0000 para^96f5ff140^ff0000N√≠vel 8 a 9: Em ^96f5ff10^ff0000 segundos, reduz o custo de canaliza√ß√£o da pr√≥xima ^96f5ffŒ®Mar em Chamas^ff0000 para^96f5ff120^ff0000 N√≠vel 10: Em ^96f5ff10^ff0000 segundos, reduz o custo de canaliza√ß√£o do seu pr√≥ximo ^96f5ffŒ®Mar de Chamas^ff0000 para ^96f5ff100@1@2^ff0000Em ^96f5ff10^ff0000 segundos, reduz o custo de canaliza√ß√£o do seu pr√≥ximo ^96f5ffŒ®Mar de Chamas^ff0000 para ^96f5ff%d@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL712 
#define __CPPGEN_GNET_SKILL712 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill712:public Skill 
    { 
    public: 
        enum { SKILL_ID = 712 }; 
        Skill712 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill712Stub:public SkillStub 
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
    Skill712Stub ():SkillStub (712) 
    { 
        cls                 = 127; 
#ifdef _SKILL_CLIENT 
        name                = L"ŒÔ∆∑ººƒ‹-Œﬁµ–“©º¡"; 
        nativename          = "ŒÔ∆∑ººƒ‹-Œﬁµ–“©º¡"; 
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
        effect              = "œ….sgc"; 
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
            skill->GetVictim ()->SetTime (2000 + 2000 * skill->GetLevel ());
            skill->GetVictim ()->SetShowicon (1);
            skill->GetVictim ()->SetInvincible2 (1);
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
