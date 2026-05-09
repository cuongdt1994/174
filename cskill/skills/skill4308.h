//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
43080  "Transformação da Missão_Picotador"43081  "Transformação do Picotador para completar a missão"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL4308 
#define __CPPGEN_GNET_SKILL4308 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill4308:public Skill 
    { 
    public: 
        enum { SKILL_ID = 4308 }; 
        Skill4308 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill4308Stub:public SkillStub 
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
                return 1;
            }
            bool Skip (Skill * skill) const
            {
                return 0;
            }
        };
#endif   
    Skill4308Stub ():SkillStub (4308) 
    { 
        cls                 = 127; 
#ifdef _SKILL_CLIENT 
        name                = L"�������_��ľ��"; 
        nativename          = "�������_��ľ��"; 
        icon                = ""; 
#endif 
        max_level           = 10; 
        type                = 4; 
        allow_ride          = 0; 
        attr                = 7; 
        rank                = 0; 
        eventflag           = 0; 
        is_senior           = 0; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 0; 
        allow_forms         = 1; 
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
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
#endif 
    } 
    int GetExecutetime (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
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
        return (float) (10); 
    } 
    float GetAttackdistance (Skill * skill) const 
    { 
        return (float) (0); 
    } 
    float GetAngle (Skill * skill) const 
    { 
        return (float) (0); 
    } 
    float GetPraydistance (Skill * skill) const 
    { 
        return (float) (10); 
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
    int GetIntroduction (Skill * skill, const wchar_t * buffer, int length, const wchar_t * format) const 
    { 
        return 0; 
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
        skill->GetVictim ()->SetProbability (1.0 * skill->GetLevel () < 6 ? 100 : 0);
        skill->GetVictim ()->SetRatio (skill->GetLevel () == 5 ? 1 : 2);
        skill->GetVictim ()->SetValue (188); // Shape Type
        skill->GetVictim ()->SetNewValue (4309); // Lâmina de Serra
        skill->GetVictim ()->SetNewValue2 (4313); // Excesso de Velocidade
        skill->GetVictim ()->SetNewShapeAndSkills (1);      

        skill->GetVictim ()->SetProbability (1.0 * skill->GetLevel () < 6 ? 100 : 0);
        skill->GetVictim ()->SetTime (skill->GetLevel () == 1 ? 7100 : 999999);
        skill->GetVictim ()->SetShowicon (1);
        skill->GetVictim ()->SetInvincible (1);
       
        skill->GetVictim ()->SetProbability (1.0 * skill->GetLevel () > 6 ? 100 : 0);
        skill->GetVictim ()->SetRatio (skill->GetLevel () == 7 ? 1 : 2);
        skill->GetVictim ()->SetValue (46); // Shape Type
        skill->GetVictim ()->SetNewValue (5795); // 
        skill->GetVictim ()->SetNewValue2 (5664); // 
        skill->GetVictim ()->SetNewShapeAndSkills (1);  

        skill->GetVictim ()->SetProbability (1.0 * skill->GetLevel () > 6 ? 100 : 0);
        skill->GetVictim ()->SetTime (skill->GetLevel () == 8 ? 3100 : 999999);
        skill->GetVictim ()->SetValue (200000);
        skill->GetVictim ()->SetHpgen (1);

        skill->GetVictim ()->SetProbability (1.0 * skill->GetLevel () > 6 ? 100 : 0);
        skill->GetVictim ()->SetTime (skill->GetLevel () == 8 ? 3100 : 999999);
        skill->GetVictim ()->SetRatio (10);
        skill->GetVictim ()->SetInchp (1);

        skill->GetVictim ()->SetProbability (1.0 * skill->GetLevel () > 6 ? 100 : 0);
        skill->GetVictim ()->SetTime (skill->GetLevel () == 8 ? 3100 : 999999);
        skill->GetVictim ()->SetRatio (10);
        skill->GetVictim ()->SetIncdefence (1);
		
        skill->GetVictim ()->SetProbability (1.0 * skill->GetLevel () > 6 ? 100 : 0);
        skill->GetVictim ()->SetValue (99999);
        skill->GetVictim ()->SetHeal (1);

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
        return (float) (15); 
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
        return 1; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    float GetHitrate (Skill * skill) const 
    { 
        return 1; 
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
