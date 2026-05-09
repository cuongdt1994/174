//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
38100  "ΨPrisão Amaldiçoada"38101  "^ff5c00ΨPrisão Amaldiçoada^ffcb4aAlcance: ^ffffff4,5 metros^ffcb4aCusto de MP: ^96f5ff290^ffffff^ffcb4aCanalização: ^ffffff0,4 ^ffcb4asegundos^ffcb4aConjuração: ^ffffff2,5 ^ffcb4asegundos^ffcb4aEspera: ^ffffff30 ^ffcb4asegundos^ffcb4aArma: ^ffffffAdaga^ffcb4aBônus de Runa: ^ff5c00Poder do Dragão^ffcb4aCultivo necessário: ^ffffffAlma CelestialAtaque os pontos vitais do inimigo com uma miríade de golpesprecisos, atordoando o alvo por ^96f5ff6^ffffff segundose causando ^96f5ff200%%^ffffff de dano físico básico. Causa lentidão no alvopor ^96f5ff10^ffffff segundos, reduzindo a velocidade em ^96f5ff60%%^ffffff. O dano básicoda habilidade é igual a ^96f5ff375%%^ffffff de dano por arma e mais ^96f5ff7497^ffffff.^a6caf0Custa ^ffffff2^a6caf0 Chia^a6caf0 e ^ffffff50 ^a6caf0de Chi.^d618e7Consome todas as Cargas de Ataque para aumentar a efetividade. Ganha uma Carga de Ataque adicional quando completa.^ffeaabEfeito da Runa Argêntea:@1Tem ^96f5ff17%%^ffeaab de chance por Carga de Ataque de entrar em Furtividade por ^96f5ff10^ffeaab segundos.^ffeaabNível 1 a 4: Causa ^96f5ff240%%^ffeaab de dano básico da habilidade.^ffeaabNível 5 a 7: Causa ^96f5ff260%%^ffeaab de dano básico da habilidade.^ffeaabNível 8 a 9: Causa ^96f5ff280%%^ffeaab de dano básico da habilidade.^ffeaabNível 10: Causa ^96f5ff300%%^ffeaab de dano básico da habilidade.@1@2Tem ^96f5ff17%%^ffeaab de chance por Carga de Ataque de entrar em Furtividade por ^96f5ff10^ffeaab segundos.Causa ^96f5ff%d%%^ffeaab de dano básico da habilidade.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL810 
#define __CPPGEN_GNET_SKILL810 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill810:public Skill 
    { 
    public: 
        enum { SKILL_ID = 810 }; 
        Skill810 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill810Stub:public SkillStub 
    { 
    public: 
    Skill810Stub ():SkillStub (810) 
    { 
        cls                 = 255; 
#ifdef _SKILL_CLIENT 
        name                = L"ʴ��"; 
        nativename          = "ʴ��"; 
        icon                = ""; 
#endif 
        max_level           = 1; 
        type                = 6; 
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
        effect              = "����������.sgc"; 
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
        pre_skills.push_back (std::pair < ID, int >(0, 1)); 
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
            skill->GetVictim ()->SetProbability (1.0 * 5);
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (0.1);
            skill->GetVictim ()->SetDechp (1);
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
