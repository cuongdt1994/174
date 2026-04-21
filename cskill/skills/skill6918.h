//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
69180  "ΨLacre dos Deuses"69181  "^ff5c00ΨLacre dos Deuses

^ffcb4aAlcance ^96f5ff%d^ffffff ^ffcb4ametros
^ffcb4aCusto em MP ^96f5ff%d^ffffff
^ffcb4aCanalização ^ffffff1 ^ffcb4asegundo
^ffcb4aConjuração ^ffffff1 ^ffcb4asegundo
^ffcb4aEspera ^ffffff30,0 ^ffcb4asegundos
^ffcb4aArma ^ffffffDesarmado, Instrumentos Mágicos
^ffcb4aBônus de Runa ^ff5c00Fúria do Dragão

^ffcb4aCultivo ^ffffffAlma Celestial
Eras atrás, durante a batalha contra uma certa 
aparição extremamente poderosa, um grupo de sacerdotes 
descobriu como criar um novo e mais poderoso selo ao manipular 
a energia divina para parcialmente removê-la da realidade. Apesar 
dos sacerdotes terem tido diversas discussões sobre este feitiço 
nos anos seguintes, ninguém negou a sua utilidade.
Manipule a magia divina do próprio Perfect World para 
parcialmente selar o alvo entre as dimensões. Congela 
o alvo, mas reduz o dano recebido por ele em ^96f5ff90%%^ffffff. Dura ^96f5ff15^ffffff segundo.

^a6caf0Custa ^ffffff1^a6caf0 Chi.

^ff0000Efeito da Runa Escarlate:@1
^ff0000Nível 1 a 4: Recupera ^96f5ff40^ff0000 Chi após o uso.
^ff0000Nível 5 a 7: Recupera ^96f5ff60^ff0000 Chi após o uso.
^ff0000Nível 8 a 9: Recupera ^96f5ff80^ff0000 Chi após o uso.
^ff0000Nível 10: Recupera ^96f5ff100^ff0000 Chi após o uso.@1@2
Recupera ^96f5ff%d^ff0000 Chi após o uso.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL6918 
#define __CPPGEN_GNET_SKILL6918 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill6918:public Skill 
    { 
    public: 
        enum { SKILL_ID = 6918 }; 
        Skill6918 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill6918Stub:public SkillStub 
    { 
    public: 
#define get_glyph(level, l1_4, l5_7, l8_9, lmax) level >= 0 && level <= 4 ? l1_4 : level >= 5 && level <= 7 ? l5_7 : level >= 8 && level <= 9 ? l8_9 : lmax; 

#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1000;
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
                skill->GetPlayer ()->SetDecmp (200);
                skill->GetPlayer ()->SetPray (1);
            }
            bool Interrupt (Skill * skill) const
            {
                return skill->GetRand () < 25 - 2 * skill->GetLevel ();
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
                return 1000;
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
                skill->GetPlayer ()->SetDecmp (800);
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
    Skill6918Stub ():SkillStub (6918) 
    { 
        cls                 = 7; 
#ifdef _SKILL_CLIENT 
        name                = L"����֮��ӡ"; 
        nativename          = "����֮��ӡ"; 
        icon                = "����֮��ӡ��.dds"; 
#endif 
        max_level           = 1; 
        type                = 3; 
        allow_ride          = 0; 
        attr                = 2; 
        rank                = 41; 
        eventflag           = 0; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 2654; 
        allow_forms         = 3; 
        apcost              = 100; 
        apgain              = 0; 
        doenchant           = 1; 
#ifdef _SKILL_SERVER
        dobless             = 1; 
#endif
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
        effect              = "������Ԫ.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 0; 
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
        restrict_weapons.push_back (292); 
        restrict_weapons.push_back (0); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
        statestub.push_back (new State2 ()); 
        statestub.push_back (new State3 ()); 
#endif 
    } 
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
    int GetCoolingtime (Skill * skill) const
        {
            return 30000;
        }
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 4000000,4000000,4000000,4000000,4000000,4000000,4000000,4000000,4000000,4000000 }; 
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
            return (float) (8 + 0.4 * skill->GetLevel ());
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (4 + 0.3 * skill->GetLevel ());
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (0));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (30);
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
            return _snwprintf (buffer, length, format, 30, 1000);

        }
#endif 
#ifdef _SKILL_SERVER 
        int GetEnmity (Skill * skill) const
        {
            return 1;
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
            skill->GetVictim ()->SetTime (15000);
            skill->GetVictim ()->SetShowicon (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetSealed2 (1);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (15000);
            skill->GetVictim ()->SetShowicon (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetFix2 (1);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (15000);
            skill->GetVictim ()->SetRatio (0.9);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetDechurt2 (1);
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
    bool BlessMe (Skill * skill) const 
    { 
        int _glyph_value = get_glyph(skill->GetLevel (), 40, 60, 80, 100);

        skill->GetVictim ()->SetProbability (1.0 * 100);
        skill->GetVictim ()->SetValue (_glyph_value);
        skill->GetVictim ()->SetAp (1);
        
        return true; 
    } 
#endif 
#ifdef _SKILL_SERVER 
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (35);
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
            return 16;
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
