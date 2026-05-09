//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
39300  "Œ®Tornado de L√¢minas"39301  "^ff5c00Œ®Tornado de L√¢minas^ffcb4aCusto em MP ^96f5ff495^ffffff^ffcb4aCanaliza√ß√£o ^ffffff0.4 ^ffcb4aSegundos^ffcb4aConjura√ß√£o ^ffffff1.7 ^ffcb4aSegundos^ffcb4aEspera ^ffffff180.0 ^ffcb4aSegundos^ffcb4aArma ^ffffffDesarmado, Arma corpo a corpo^ffcb4aB√¥nus de Runa ^ff5c00Maestria Marcial^ffcb4aCultivo ^ffffffAlma CelestialCria uma tempestade devastadora de ira ardente por ^96f5ff12^ffffff segundos. A cada ^96f5ff3^ffffff segundo,os inimigos a at√© ^96f5ff10^ffffff metros receber√£o dano. Causadano da arma mais ^96f5ff450%%^ffffffdo Ataque da arma mais^96f5ff20000^ffffffde dano f√≠sico e diminui a velocidade do alvo em ^96f5ff30%%^ffffff. Dura ^96f5ff3^ffffff segundos.Reduz o dano recebido em ^96f5ff50%%^ffffff edeixa voc√™ imune a todos os efeitos de controle. N√£o pode ser um acerto cr√≠tico, mas tem a chance dedobrar a sua Taxa de Acerto Cr√≠tico e causar ^96f5ff1,5^ffffff vezes de dano.Recarrega ^96f5ff2^ffffff Chis ao longo de ^96f5ff12^ffffff segundos.^a6caf0Custa ^ffffff3^a6caf0Chis.^ffeaabEfeito da Runa Arg√™ntea:@1^ffeaabN√≠vel 1 a 4: Ao acertar um alvo, tem ^96f5ff40%%^ffeaab de chance de atordo√°-lo por ^96f5ff4^ffeaab segundos.^ffeaabN√≠vel 5 a 7: Ao acertar um alvo, tem ^96f5ff60%%^ffeaab de chance de atordo√°-lo por ^96f5ff4^ffeaab segundos.^ffeaabN√≠vel 8 a 9: Ao acertar um alvo, tem ^96f5ff80%%^ffeaab de chance de atordo√°-lo por ^96f5ff4^ffeaab segundos.^ffeaabN√≠vel 10: Ao acertar um alvo, tem ^96f5ff100%%^ffeaab de chance de atordo√°-lo por ^96f5ff4^ffeaab segundos.@1@2^ffeaabAo acertar um alvo, tem ^96f5ff%d%%^ffeaab de chance de atordo√°-lo por ^96f5ff4^ffeaabsegundos.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL930 
#define __CPPGEN_GNET_SKILL930 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill930:public Skill 
    { 
    public: 
        enum { SKILL_ID = 930 }; 
        Skill930 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill930Stub:public SkillStub 
    { 
    public: 
    Skill930Stub ():SkillStub (930) 
    { 
        cls                 = 4; 
#ifdef _SKILL_CLIENT 
        name                = L"◊˜∑œ"; 
        nativename          = "◊˜∑œ"; 
        icon                = ""; 
#endif 
        max_level           = 1; 
        type                = 2; 
        allow_ride          = 0; 
        attr                = 1; 
        rank                = 22; 
        eventflag           = 0; 
        is_senior           = 0; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 0; 
        allow_forms         = 1; 
        apcost              = 100; 
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
        effect              = "æﬁ¡È…Ò¡¶.sgc"; 
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
        restrict_weapons.push_back (9); 
        restrict_weapons.push_back (0); 
#ifdef _SKILL_SERVER 
#endif 
    } 
        int GetExecutetime (Skill * skill) const
        {
            return 0;
        }
    int GetCoolingtime (Skill * skill) const 
    { 
        static int aarray[10] = { 180000,180000,180000,180000,180000,180000,180000,180000,180000,180000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 2,2,2,2,2,2,2,2,2,2 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 95,95,95,95,95,95,95,95,95,95 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredItem (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredMoney (Skill * skill) const 
    { 
        static int aarray[10] = { 1,1,1,1,1,1,1,1,1,1 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredRealmLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
        float GetRadius (Skill * skill) const
        {
            return (float) (16);
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
            return (float) (5);
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
            return 15;
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
