//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
60  "Maestria em Lâminas"61  "^ffffffMaestria em Lâminas (Nível %d)^ffcb4aEfeito Passivo^ffcb4aEstima: ^ffffffLúcido^ffffffTodos os ataques com lâminas ou espadas recebem um bônus de danos equivalente a ^96f5ff%d%%^ffffff dos danos de armas."540  "Maré Crescente"541  "^ffffffMaré Crescente (Nível %d)  ^ffcb4aAlcance: ^ffffffCorpo a Corpo ^ffcb4aCusto em MP: ^96f5ff%.1f^ffffff^ffcb4aConjuração: ^ffffff0.2 ^ffcb4asegundos^ffcb4aDuração: ^ffffff1.0 ^ffcb4asegundos^ffcb4aTEspera: ^ffffff3.0 ^ffcb4asegundos ^ffcb4aRestrições da Arma: ^ffffffArmas Corpo a Corpo ^ffcb4aCultivo necessário: ^ffffffAstutoUm ataque violento causa dano físico básico mais^96f5ff%.1f^ffffff. Aumenta o seu nível de ameaça.^a6caf0Requer ^ffffff30^a6caf0 Chi para conjurar."550  "Dragão Violento"551  "^ffffffDragão Violento (Nível %d)  ^ffcb4aAlcance: ^ffffffCorpo a Corpo ^ffcb4aCusto em MP: ^96f5ff%.1f^ffffff^ffcb4aConjuração: ^ffffff0.3 ^ffcb4asegundos^ffcb4aDuração: ^ffffff0.7 ^ffcb4asegundos^ffcb4aEspera: ^ffffff8.0 ^ffcb4asegundos ^ffcb4aVitalidade: ^ffffff10^ffcb4aRestrições da Arma: ^ffffffArmas Corpo a Corpo ^ffcb4aCultivo necessário: ^ffffffLúcidoAtinge os oponentes com a força de um dragão enfurecido causando dano físico básico mais^96f5ff%.1f^ffffff. Afeta inimigos com um jato frontal."560  "Golpes Rompantes"561  "^ffffffGolpes Rompantes (Nível %d)^ffcb4aAlcance: ^ffffffCorpo a Corpo^ffcb4aCusto em MP: ^96f5ff%.1f^ffffff^ffcb4aConjuração: ^ffffff0.3 ^ffcb4asegundos^ffcb4aDuração: ^ffffff0.9 ^ffcb4asegundos^ffcb4aEspera: ^ffffff8.0 ^ffcb4asegundos^ffcb4aVitalidade: ^ffffff12^ffcb4aRestrições da Arma: ^ffffffArmas Corpo a Corpo^ffcb4aCultivo necessário: ^ffffffEnigmáticoUsa toda a sua força para um golpe lateral, causando dano físico básico mais ^96f5ff8^ffffffa todos os inimigos em um raio de ^96f5ff%.1f^ffffff metros."*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL6 
#define __CPPGEN_GNET_SKILL6 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill6:public Skill 
    { 
    public: 
        enum { SKILL_ID = 6 }; 
        Skill6 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill6Stub:public SkillStub 
    { 
    public: 
    Skill6Stub ():SkillStub (6) 
    { 
        cls                 = 0; 
#ifdef _SKILL_CLIENT 
        name                = L"������ͨ"; 
        nativename          = "������ͨ"; 
        icon                = "������ͨ.dds"; 
#endif 
        max_level           = 10; 
        type                = 5; 
        allow_ride          = 0; 
        attr                = 0; 
        rank                = 3; 
        eventflag           = 2; 
        is_senior           = 0; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1134; 
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
        pre_skills.push_back (std::pair < ID, int >(0, 1)); 
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
        static int aarray[10] = { 11800,17300,24600,34200,47000,64000,87000,118000,160000,239000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 29,34,39,44,49,54,59,64,69,74 }; 
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
            return _snwprintf (buffer, length, format, skill->GetLevel (), 6 * skill->GetLevel ());

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
            skill->GetPlayer ()->SetIncsword (0.06 * skill->GetLevel ());
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
