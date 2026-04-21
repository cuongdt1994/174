//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
39410  "ΨLança Glacial"39411  "^ff5c00ΨLança Glacial

^ffcb4aAlcance ^96f5ff20^ffffff ^ffcb4ametros
^ffcb4aCusto em MP ^96f5ff%d^ffffff
^ffcb4aCanalização ^ffffff0,3 ^ffcb4asegundo
^ffcb4aConjuração ^ffffff1,2 ^ffcb4asegundo
^ffcb4aEspera ^ffffff30,0 ^ffcb4asegundos
^ffcb4aArma: ^ffffffArma de Haste
^ffcb4aBônus de Runa ^ff5c00Fúria do Tigre

^ffcb4aCultivo Necessário ^ffffffAlma Celestial
Concentre uma magia de gelo numa explosão de ^96f5ff12^ffffff metros de lanças glaciais, 
causando dano físico básico mais ^96f5ff100%%^ffffff de dano de arma mais
^96f5ff8504^ffffff. Recupera
^96f5ff50^ffffff Chi depois de usar. Esse ataque tem a garantia de acertar.

Se o alvo estiver a até ^96f5ff10^ffffff metros de você,
os danos Físico e Mágico dele serão reduzidos em^96f5ff%d%%^ffffff de dano da arma.
(Redução da Defesa Física é aumentada para ^96f5ff90%%^ffffff de dano de arma.
Redução da Defesa Mágica é aumentada para ^96f5ff60%%^ffffff de dano de arma.)
^ffffffCongela os alvos afligidos a ^96f5ff10^ffffff metros de distância por 
^96f5ff5^ffffff segundos.

^a6caf0Custa ^ffffff2^a6caf0 Chis.

^ff0000Efeito da Runa Escarlate:@1
^ff0000 Em ^96f5ff6^ff0000 segundos, o alcance do próximo ^96f5ffΨGolpe do Sopro do Dragão^ff0000 aumenta em ^96f5ff10^ff0000 metros.
^ff0000Nível 1 a 4: O dano do próximo ^96f5ffΨGolpe do Sopro do Dragão^ff0000 aumenta em ^96f5ff20%%.
^ff0000Nível 5 a 7: O dano do próximo^96f5ffΨGolpe do Sopro do Dragão^ff0000 aumenta em^96f5ff30%%.
^ff0000Nível 8 a 9: O dano do próximo^96f5ffΨGolpe do Sopro do Dragão^ff0000 aumenta em^96f5ff40%%.
^ff0000 Nível 10 :O dano do próximo^96f5ffΨGolpe do Sopro do Dragão^ff0000aumenta em^96f5ff50%%.@1@2
^ff0000Em ^96f5ff6^ff0000segundos, o alcance do próximo ^96f5ffΨGolpe do Sopro do Dragão^ff0000aumenta em
^96f5ff10^ff0000metros e o dano aumenta em ^96f5ff%d%%.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL3941 
#define __CPPGEN_GNET_SKILL3941 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill3941:public Skill 
    { 
    public: 
        enum { SKILL_ID = 3941 }; 
        Skill3941 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill3941Stub:public SkillStub 
    { 
    public: 
#define get_glyph(level, l1_4, l5_7, l8_9, lmax) level >= 0 && level <= 4 ? l1_4 : level >= 5 && level <= 7 ? l5_7 : level >= 8 && level <= 9 ? l8_9 : lmax; 

#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 300;
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
                skill->GetPlayer ()->SetDecmp (61);
                skill->GetPlayer ()->SetPray (1);
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
#ifdef _SKILL_SERVER 
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1200;
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
                skill->GetPlayer ()->SetDecmp (244);
                skill->SetPlus (8504);
                skill->SetRatio (1);
                skill->SetDamage (skill->GetAttack ());
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
    Skill3941Stub ():SkillStub (3941) 
    { 
        cls                 = 0; 
#ifdef _SKILL_CLIENT 
        name                = L"�񡤺�������"; 
        nativename          = "�񡤺�������"; 
        icon                = "�񡤺��������.dds"; 
#endif 
        max_level           = 1; 
        type                = 1; 
        allow_ride          = 0; 
        attr                = 7; 
        rank                = 41; 
        eventflag           = 0; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1116; 
        allow_forms         = 1; 
        apcost              = 200; 
        apgain              = 50; 
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
        effect              = "��_��������_����.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 3; 
        has_stateattack     = 0; 
        runes_attr          = 3; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = "����״̬Ч����ǿ1.gfx"; 
        gfxhangpoint        = "HH_spine"; 
#endif 
        gfxtarget           = 0; 
        feature             = 3; 
        extra_effect        = 0; 
        immune_casting      = 0; 
        restrict_weapons.push_back (5); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
        statestub.push_back (new State2 ()); 
        statestub.push_back (new State3 ()); 
#endif 
    } 
    int GetExecutetime (Skill * skill) const 
    {
        static int array[10] = { 1800,1800,1800,1800,1800,1800,1800,1800,1800,1800 };
        return array[skill->GetLevel () - 1];
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
        static int aarray[10] = { 100,100,100,100,100,100,100,100,100,100 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredItem (Skill * skill) const 
    { 
        static int aarray[10] = { 11208,11208,11208,11208,11208,11208,11208,11208,11208,11208 }; 
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
            return (float) (12);
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
            return (float) (20);
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
            return _snwprintf (buffer, length, format, 305, 50);

        }
#endif 
#ifdef _SKILL_SERVER 
        int GetEnmity (Skill * skill) const
        {
            return 10000;
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
			float Ratio1 = skill->GetPlayer ()->GetCls () == -1 ? 0.5 : 0.9;
			float Ratio2 = skill->GetPlayer ()->GetCls () == -1 ? 0.5 : 0.6;
           
			
			skill->GetVictim ()->SetProbability (1.0 * skill->GetT0 () <= 10 ? 100 : 0);
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetRatio (Ratio1);
            skill->GetVictim ()->SetDecdefence (1);

            skill->GetVictim ()->SetProbability (1.0 * skill->GetT0 () <= 10 ? 100 : 0);
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetRatio (Ratio2);
            skill->GetVictim ()->SetDecresist (1);

            skill->GetVictim ()->SetProbability (1.0 * skill->GetT0 () >= 10 && skill->GetT0 () <= 20 ? 100 : 0);
            skill->GetVictim ()->SetTime (5000);
            skill->GetVictim ()->SetShowicon (0);
            skill->GetVictim ()->SetGeloAtiradora (1);

            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        bool BlessMe (Skill * skill) const
        {
            skill->GetPlayer ()->SetProbability (1.0 * 100);
            skill->GetPlayer ()->SetTime (6100);
            skill->GetPlayer ()->SetAmount (3949); // Sopro do Dragão
            skill->GetPlayer ()->SetValue (0);
            skill->GetPlayer ()->SetRatio (10);
            skill->GetPlayer ()->SetNewValue (5); // Feature
            skill->GetVictim ()->SetGlyphRange2 (1);  

            float _glyph_value = get_glyph(skill->GetLevel (), 0.2, 0.3, 0.4, 0.5);


            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (6100);
			skill->GetVictim ()->SetAmount (3949); //Skill
            skill->GetVictim ()->SetRatio (_glyph_value);            
            skill->GetVictim ()->SetGlyphDamage (1);

            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
#endif 
#ifdef _SKILL_SERVER 
    float GetTalent0 (PlayerWrapper * player) const
        {
            return (float) (player->GetRangetotarget ());
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
            return (float) (3);
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
