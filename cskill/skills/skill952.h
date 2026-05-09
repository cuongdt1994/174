//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
39520  "ΨGolpe de Sopro do Dragão"39521  "^ff5c00ΨGolpe de Sopro do Dragão^ffcb4aAlcance ^ffffffCorpo-a-corpo^ffcb4aCusto em MP ^96f5ff%d^ffffff^ffcb4aCanalização ^ffffff0.6 ^ffcb4asegundo^ffcb4aConjuração ^ffffff1,0 ^ffcb4asegundo^ffcb4aEspera ^ffffff30,0 ^ffcb4asegundos^ffcb4aArma: ^ffffffDesarmado, Punhos ou Garras^ffcb4aBônus de Runa ^ff5c00Fúria do Tigre^ffcb4aCultivo ^ffffffAlma CelestialConcentre sua energia em seus punhos e a libere emtodos os inimigos em uma linha de ^96f5ff15^ffffff-metros. Causa dano físico básico mais^96f5ff100%% ^ffffffdo dano da arma mais ^96f5ff11504^ffffff. Metade do dano é Dano deFogo. Pelos próximos ^96f5ff15 ^ffffffsegundos, todos os seus ataques normaiscausam dano de fogo extra igual a ^96f5ff120%% ^ffffffdo dano da sua arma.Esse ataque tem a garantia de acertar.^a6caf0Custa ^ffffff2^a6caf0 Chis.^00ffff^00ffffEfeito da Runa Celeste:@1^00ffffNível 1 a 4: Se usado em até ^ffffff4 ^00ffffsegundos, suas próximas ^ffffffΨDez Mil Lâminas ^00ffffnão terão tempo de espera.^00ffffNível 5 a 7: Se usado em até ^ffffff5 ^00ffffsegundos, suas próximas ^ffffffΨDez Mil Lâminas ^00ffffnão terá tempo de espera.^00ffffNível 8 a 9: Se usado em até ^ffffff6 ^00ffffsegundos, suas próximas ^ffffffΨDez Mil Lâminas ^00ffffnão terá tempo de espera.^00ffffNível 10: Se usado em até ^ffffff7 ^00ffffsegundos, suas próximas ^ffffffΨDez Mil Lâminas ^00ffffnão terão tempo de espera.@1@2^00ffffSe usado em até ^ffffff%d ^00ffffsegundos, suas próximas ^ffffffΨDez Mil Lâminas ^00ffffnão terão tempo de espera.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL952 
#define __CPPGEN_GNET_SKILL952 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill952:public Skill 
    { 
    public: 
        enum { SKILL_ID = 952 }; 
        Skill952 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill952Stub:public SkillStub 
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
    Skill952Stub ():SkillStub (952) 
    { 
        cls                 = 127; 
#ifdef _SKILL_CLIENT 
        name                = L"ҩƷ.ʱЧ���ȼ�������"; 
        nativename          = "ҩƷ.ʱЧ���ȼ�������"; 
        icon                = ""; 
#endif 
        max_level           = 10; 
        type                = 4; 
        allow_ride          = 0; 
        attr                = 0; 
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
        allow_air           = 0; 
        allow_water         = 0; 
        notuse_in_combat    = 0; 
        restrict_corpse     = 0; 
        restrict_change     = 0; 
        restrict_attach     = 0; 
        auto_attack         = 0; 
        time_type           = 0; 
        long_range          = 1; 
        posdouble           = 0; 
        clslimit            = 0; 
        commoncooldown      = 0; 
        commoncooldowntime  = 2; 
        itemcost            = 0; 
        itemcostcount       = 0; 
        combosk_preskill    = 0; 
        combosk_interval    = 0; 
        combosk_nobreak     = 0; 
#ifdef _SKILL_CLIENT 
        effect              = ""; 
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
        static int aarray[10] = { 1,2,3,4,5,6,7,8,9,10 }; 
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
        static float aarray[10] = { 1,1,1,1,1,1,1,1,1,1 }; 
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
            skill->GetVictim ()->SetTime (200000 - 20000 * (skill->GetLevel () - 1));
            skill->GetVictim ()->SetRatio (0.2 * skill->GetLevel ());
            skill->GetVictim ()->SetSpeedup (1);
            skill->GetVictim ()->SetTime (200000 - 20000 * (skill->GetLevel () - 1));
            skill->GetVictim ()->SetFreemove (1);
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
