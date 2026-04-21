//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
12460  "‚óèF√∫ria da √Ågua"12461  "^8080ff‚óèF√∫ria da √Ågua^ffcb4aAlcance: ^96f5ff28.5^ffffff^ffcb4a metros^ffcb4aCusto em MP: ^96f5ff680^ffffff^ffcb4aConjura√ß√£o: ^ffffff1.8^ffcb4a segundos^ffcb4aDura√ß√£o: ^ffffff1.2^ffcb4a segundos^ffcb4aEspera: ^ffffff30.0^ffcb4a segundos^ffcb4aRestri√ß√µes da Arma: ^ffffffOrbe^ffcb4aCultivo necess√°rio: ^ffffffIluminadoAtinge todos os alvos num raio de ^96f5ff8^ffffff metros. Causa dano m√°gico b√°sico mais ^96f5ff200%%^ffffff de dano por arma mais ^96f5ff6578^ffffff de dano por √Ågua. Oferece ^96f5ff80%%^ffffff de chance de faz√™-los sangrar por danos equivalentes √† sua For√ßa da Almapor ^96f5ff9^ffffff segundos.^a6caf0Custa ^ffffff2^a6caf0 Chi^f7dfa5Oferece^96f5ff50%%^f7dfa5 de chance de custar apenas 1 Chi"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL1246 
#define __CPPGEN_GNET_SKILL1246 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill1246:public Skill 
    { 
    public: 
        enum { SKILL_ID = 1246 }; 
        Skill1246 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill1246Stub:public SkillStub 
    { 
    public: 
#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1800;
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
                skill->GetPlayer ()->SetDecmp (136);
                skill->GetPlayer ()->SetPray (1);
            }
            bool Interrupt (Skill * skill) const
            {
                return skill->GetRand () < 5;
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
#ifdef _SKILL_SERVER 
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1133;
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
                skill->GetPlayer ()->SetDecmp (544);
                skill->SetPlus (6578);
                skill->SetRatio (2);
                skill->SetWaterdamage (skill->GetMagicattack ());
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
#ifdef _SKILL_SERVER 
        class State3:public SkillStub::State
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
    Skill1246Stub ():SkillStub (1246) 
    { 
        cls                 = 2; 
#ifdef _SKILL_CLIENT 
        name                = L"’Ê°§ÀÆ∆Ÿ„¸„Û"; 
        nativename          = "’Ê°§ÀÆ∆Ÿ„¸„Û"; 
        icon                = "ÀÆ∆Ÿ ıœ…ººƒ‹.dds"; 
#endif 
        max_level           = 1; 
        type                = 1; 
        allow_ride          = 0; 
        attr                = 4; 
        rank                = 21; 
        eventflag           = 0; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1720; 
        allow_forms         = 1; 
        apcost              = 200; 
        apgain              = 0; 
        doenchant           = 1; 
        dobless             = 1; 
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
        effect              = "Œ◊ ¶_ÀÆ∆Ÿ ı.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 3; 
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
        pre_skills.push_back (std::pair < ID, int >(1134, 10)); 
        restrict_weapons.push_back (25333); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
        statestub.push_back (new State2 ()); 
        statestub.push_back (new State3 ()); 
#endif 
    } 
        int GetExecutetime (Skill * skill) const
        {
            return 1133;
        }
    int GetCoolingtime (Skill * skill) const
        {
            return 30000;
        }
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 2000000,2000000,2000000,2000000,2000000,2000000,2000000,2000000,2000000,2000000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 99,99,99,99,99,99,99,99,99,99 }; 
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
            return (float) (8);
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
            return (float) (28.5);
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
            float percent = 1.5f;
			if(skill->GetPlayer ()->GetCls () == 0) percent = 2.0f; // ß”ß‡ß⁄ßﬂ
			if(skill->GetPlayer ()->GetCls () == 1) percent = 1.5f; // ßﬁß—ß‘
			if(skill->GetPlayer ()->GetCls () == 2) percent = 1.5f; // ßÍß—ßﬁß—ßﬂ
			if(skill->GetPlayer ()->GetCls () == 3) percent = 1.5f; // ß’ß‚ßÂß⁄ß’	
			if(skill->GetPlayer ()->GetCls () == 4) percent = 2.5f; // ß‰ß—ßﬂß‹	
			if(skill->GetPlayer ()->GetCls () == 5) percent = 1.5f; // ß„ß⁄ßﬂ			
			if(skill->GetPlayer ()->GetCls () == 6) percent = 1.5f; // ß›ßÂß‹			
			if(skill->GetPlayer ()->GetCls () == 7) percent = 1.5f; // ßÿß‚ß÷ßË			
			if(skill->GetPlayer ()->GetCls () == 8) percent = 1.5f; // ß„ß‰ß‚ß—ßÿ				
			if(skill->GetPlayer ()->GetCls () == 9) percent = 1.5f; // ßﬁß⁄ß„ß‰ß⁄ß‹		
			if(skill->GetPlayer ()->GetCls () == 10) percent = 1.5f; // ß·ß‚ß⁄ßŸß‚ß—ß‹
			if(skill->GetPlayer ()->GetCls () == 11) percent = 1.5f; // ßÿßﬂß÷ßË			
			
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (9000);
            skill->GetVictim ()->SetAmount ((skill->GetT0 () * 1) * percent);
            skill->GetVictim ()->SetBleeding (1);
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * 50);
            skill->GetVictim ()->SetTime (1000);
            skill->GetVictim ()->SetShowicon (0);
            skill->GetVictim ()->SetAmount (100);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetApgencont2 (1);
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (45);
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent0 (PlayerWrapper * player) const
        {
            return (float) (player->GetSoulpower ());
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
            return 6;
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
