//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
36790  "Œ®Tempestade de Granizo"36791  "^ff5c00Œ®Tempestade de Granizo^ffcb4aAlcance ^96f5ff%d^ffffff ^ffcb4ametros^ffcb4aCusto em MP ^96f5ff%d^ffffff^ffcb4aCanaliza√ß√£o ^ffffff1,8 ^ffcb4asegundo^ffcb4aConjura√ß√£o ^ffffff1,0 ^ffcb4asegundo^ffcb4aEspera ^ffffff12,0 ^ffcb4asegundos^ffcb4aChi Recebido ^ffffff5^ffcb4aArma: ^ffffffDesarmado, Instrumentos M√°gicos^ffcb4aB√¥nus de Runa ^ff5c00F√∫ria do Drag√£o^ffcb4aCultivo ^ffffffAlma CelestialInvoque uma tempestade de granizo em todos os inimigos pr√≥ximos ao alvo.Causa dano m√°gico b√°sico mais ^96f5ff4740 ^ffffffcomo Dano de √Ågua.Tem ^96f5ff50%% ^ffffffde chance de congelar os alvos por ^96f5ff4 ^ffffffsegundos.^ffcb4aEfeito da Runa √Åurea:@1^ffcb4aN√≠vel 1 a 4: Causa um dano de arma adicional de ^96f5ff72%% ^ffcb4amais ^96f5ff1036^ffcb4a.^ffcb4aN√≠vel 5 a 7: Causa um dano de arma adicional de ^96f5ff108%% ^ffcb4amais ^96f5ff1554^ffcb4a.^ffcb4aN√≠vel 8 a 9: Causa um dano de arma adicional de ^96f5ff144%% ^ffcb4amais ^96f5ff2072^ffcb4a.^ffcb4aN√≠vel 10: Causa um dano de arma adicional de ^96f5ff180%% ^ffcb4amais ^96f5ff2590^ffcb4a.@1@2^ffcb4aCausa um dano de arma adicional de ^96f5ff%d%% ^ffcb4amais ^96f5ff%d^ffcb4a.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL679 
#define __CPPGEN_GNET_SKILL679 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill679:public Skill 
    { 
    public: 
        enum { SKILL_ID = 679 }; 
        Skill679 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill679Stub:public SkillStub 
    { 
    public: 
    Skill679Stub ():SkillStub (679) 
    { 
        cls                 = 3; 
#ifdef _SKILL_CLIENT 
        name                = L"øÒ°§»‚≤´æ´Õ®"; 
        nativename          = "øÒ°§»‚≤´æ´Õ®"; 
        icon                = "»≠ ıæ´Õ®2.dds"; 
#endif 
        max_level           = 1; 
        type                = 5; 
        allow_ride          = 0; 
        attr                = 0; 
        rank                = 30; 
        eventflag           = 4; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1629; 
        allow_forms         = 0; 
        apcost              = 0; 
        apgain              = 0; 
        doenchant           = 0; 
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
        effect              = ""; 
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
        pre_skills.push_back (std::pair < ID, int >(324, 10)); 
#ifdef _SKILL_SERVER 
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
        static int aarray[10] = { 1500000,1500000,1500000,1500000,1500000,1500000,1500000,1500000,1500000,1500000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 92,92,92,92,92,92,92,92,92,92 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredItem (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredMoney (Skill * skill) const 
    { 
        static int aarray[10] = { 2000000,2000000,2000000,2000000,2000000,2000000,2000000,2000000,2000000,2000000 }; 
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
            return _snwprintf (buffer, length, format, 150);

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
        {
            skill->GetPlayer ()->SetIncfight (0.3);
            skill->GetPlayer ()->SetInccrit (3);
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
