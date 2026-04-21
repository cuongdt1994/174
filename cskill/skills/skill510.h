//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
45100  "ΨMaldição: Petrificação"45101  "^ff5c00ΨMaldição: Petrificação  ^ffcb4a Requer forma de Raposa ^ffcb4a Alcance ^96f5ff%d^ffffff ^ffcb4a m ^ffcb4a Mana ^96f5ff%d^ffffff ^ffcb4a Canalização ^ffffff1 ^ffcb4a s ^ffcb4a Conjuração ^ffffff1.3 ^ffcb4a s ^ffcb4a Intervalo ^ffffff2 ^ffcb4a minutos ^ffcb4a Arma ^ffffff Desarmado, Instrumentos Mágicos  ^ffcb4a Cultivo ^ffffff Alma Celestial  Causa no alvo um ataque mágico de ^96f5ff100%%^ffffff de dano de arma mais ^96f5ff150%%^ffffff mais ^96f5ff20%%^ffffff dano nos ataques mágicos de madeira, Remoção simultânea de magias e outras defesas de magia para melhorar o estado e estender o intervalo de ataque ^96f5ff100%%^ffffff, redução da precisão ^96f5ff300%%^ffffff, mantida por ^96f5ff15^ffffff segundos. Essa habilidade nunca erra.  ^a6caf0 Necessidade de usar ^ffffff um ponto ^a6caf0 de chi.  ^00ff00 Efeito Runa Verdejante @1 ^00ff00Nv. 1-4: reduz a força do ataque físico do alvo ^96f5ff100%%^00ff00 por ^96f5ff10^00ff00 segundos ^00ff00Nv. 5-7: reduz a força do ataque físico do alvo ^96f5ff150%%^00ff00 por ^96f5ff10^00ff00 segundos ^00ff00Nv. 8-9: reduz a força do ataque físico do alvo ^96f5ff200%%^00ff00 por ^96f5ff10^00ff00 segundos ^00ff00 Nv. 10: reduz a força do ataque físico do alvo ^96f5ff250%%^00ff00 por ^96f5ff10^00ff00 segundos@1@2 ^00ff00 reduz a força do ataque físico do alvo ^96f5ff%d%%^00ff00 por ^96f5ff10^00ff00 segundos^96f5ff^00ff00@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL510 
#define __CPPGEN_GNET_SKILL510 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill510:public Skill 
    { 
    public: 
        enum { SKILL_ID = 510 }; 
        Skill510 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill510Stub:public SkillStub 
    { 
    public: 
    Skill510Stub ():SkillStub (510) 
    { 
        cls                 = 4; 
#ifdef _SKILL_CLIENT 
        name                = L"�桤Ұ�Իظ�"; 
        nativename          = "�桤Ұ�Իظ�"; 
        icon                = "Ұ�Իظ�1.dds"; 
#endif 
        max_level           = 1; 
        type                = 5; 
        allow_ride          = 0; 
        attr                = 1; 
        rank                = 20; 
        eventflag           = 1; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1510; 
        allow_forms         = 1; 
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
        pre_skills.push_back (std::pair < ID, int >(109, 10)); 
        restrict_weapons.push_back (9); 
#ifdef _SKILL_SERVER 
#endif 
    } 
        int GetExecutetime (Skill * skill) const
        {
            return 1500;
        }
    int GetCoolingtime (Skill * skill) const 
    { 
        static int aarray[10] = { 4000,4000,4000,4000,4000,4000,4000,4000,4000,4000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 1000000,1000000,1000000,1000000,1000000,1000000,1000000,1000000,1000000,1000000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 89,89,89,89,89,89,89,89,89,89 }; 
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
            return (float) (1 - 0.0111111 * (20 + 2.5 * skill->GetLevel ()));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 3 + 0.2 * skill->GetLevel ());
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
            return _snwprintf (buffer, length, format, 25);

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
            return (float) (25);
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
            skill->GetPlayer ()->SetInchpgen (50);
            skill->GetPlayer ()->SetIncmpgen (20);
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
