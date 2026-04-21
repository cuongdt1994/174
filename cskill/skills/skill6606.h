//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
66060  "ΨRasante do Ganso"66061  "^ff5c00ΨRasante do Ganso              

^ffcb4aAlcance ^96f5ffCorpo a corpo
^ffcb4aMP ^96f5ff%d^ffffff    
^ffcb4aCanalização ^ffffffInstantânea   
^ffcb4aEspera ^ffffff120 ^ffcb4asegundos    
^ffcb4aArma ^ffffffBastão

^ffcb4aCultivo Necessário ^ffffffAlma Celestial
Balance seu bastão para se defender. O dano de alcance direto causado por jogadores a
^96f5ff8^ffffff metros de distância é reduzido em ^96f5ff90%%^ffffff.
Enquanto conjura, fica imune a efeitos negativos e recupera 30 Chi
a cada ^96f5ff1,5^ffffff segundos. Dura até ^96f5ff20^ffffff segundos.

^a6caf0Custa ^ffffff1 ^a6caf0Chi.

^00ff00Efeito de Runa Verdejante@1
^00ff00Nível 1 a 4: Enquanto conjura, ganha um adicional de ^96f5ff100%%^00ff00 em defesa física e mágica de equipamentos.
^00ff00Nível 5 a 7: Enquanto conjura, ganha um adicional de ^96f5ff200%%^00ff00 em defesa física e mágica de equipamentos.
^00ff00Nível 8 a 9: Enquanto conjura, ganha um adicional de ^96f5ff300%%^00ff00 em defesa física e mágica de equipamentos.
^00ff00Nível 10: Enquanto conjura, ganha um adicional de ^96f5ff400%%^00ff00 em defesa física e mágica de equipamentos.@1@2
^00ff00Enquanto conjura, ganha um adicional de ^96f5ff%d%%^00ff00 em defesa física e mágica de equipamentos.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL6606 
#define __CPPGEN_GNET_SKILL6606 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill6606:public Skill 
    { 
    public: 
        enum { SKILL_ID = 6606 }; 
        Skill6606 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill6606Stub:public SkillStub 
    { 
    public: 
#define get_glyph(level, l1_4, l5_7, l8_9, lmax) level >= 0 && level <= 4 ? l1_4 : level >= 5 && level <= 7 ? l5_7 : level >= 8 && level <= 9 ? l8_9 : lmax; 

#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 100;
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
                return 50;
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
                skill->GetPlayer ()->SetPerform (2);
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
                return 1450;
            }
            bool Quit (Skill * skill) const
            {
                return false;
            }
            bool Loop (Skill * skill) const
            {
                return skill->GetPlayer ()->GetMp () > 300 && skill->GetSection () < 14;
            }
            bool Bypass (Skill * skill) const
            {
                return false;
            }
            void Calculate (Skill * skill) const
            {
				skill->SetSection (skill->GetSection () + 1);
                skill->GetPlayer ()->SetDecmp (0.5 * (300));
                skill->GetPlayer ()->SetCombat (1);
                skill->GetPlayer ()->SetPerform (0);
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
    Skill6606Stub ():SkillStub (6606) 
    { 
        cls                 = 14; 
#ifdef _SKILL_CLIENT 
        name                = L"��ƽɳ����"; 
        nativename          = "��ƽɳ����"; 
        icon                = "��ƽɳ������.dds"; 
#endif 
        max_level           = 1; 
        type                = 2; 
        allow_ride          = 0; 
        attr                = 1; 
        rank                = 41; 
        eventflag           = 4; 
        is_senior           = 0; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1501; 
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
        effect              = ""; 
        aerial_effect       = ""; 
#endif 
        range.type          = 5; 
        has_stateattack     = 0; 
        runes_attr          = 2; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = ""; 
        gfxhangpoint        = "0"; 
#endif 
        gfxtarget           = 0; 
        feature             = 0; 
        extra_effect        = 0; 
        immune_casting      = 0; 
        pre_skills.push_back (std::pair < ID, int >(6496, 1)); 
        pre_skills.push_back (std::pair < ID, int >(6497, 1)); 
        restrict_weapons.push_back (69843); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
        statestub.push_back (new State2 ()); 
        statestub.push_back (new State3 ()); 
#endif 
    } 
    int GetExecutetime (Skill * skill) const 
    {
        static int array[10] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
        return array[skill->GetLevel () - 1];
    }
    int GetCoolingtime (Skill * skill) const 
    { 
        static int aarray[10] = { 120000,120000,120000,120000,120000,120000,120000,120000,120000,120000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 50000000,50000000,50000000,50000000,50000000,50000000,50000000,50000000,50000000,50000000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 100,100,100,100,100,100,100,100,100,100 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredItem (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredMoney (Skill * skill) const 
    { 
        static int aarray[10] = { 10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000 }; 
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
            return (float) (skill->GetPlayer ()->GetRange () + 10);
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
            //skill->GetVictim ()->SetProbability (1.0 * (skill->GetSection () < 2 ? 100 : 0));
            //skill->GetVictim ()->SetTime (20000);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (5050);
            skill->GetVictim ()->SetRatio (0.9);
            skill->GetVictim ()->SetReduzDanoaDistancia (1);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (5050);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetIncdebuffdodge (1);

            skill->GetVictim ()->SetProbability (1.0 * 100); // Adiciona Chi a cada 1.5 Segundos
            skill->GetVictim ()->SetValue (30);
            skill->GetVictim ()->SetAp (1);

            float _glyph_value = get_glyph(skill->GetLevel (), 1.0, 2.0, 3.0, 4.0);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (4050);
            skill->GetVictim ()->SetRatio (_glyph_value);
            skill->GetVictim ()->SetIncdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (4050);
            skill->GetVictim ()->SetRatio (_glyph_value);
            skill->GetVictim ()->SetIncresist (1);

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
