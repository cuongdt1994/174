//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
38130  "ΨCorte Inferior"38131  "^ff5c00ΨCorte Inferior

^ffcb4aAlcance: ^ffffff4,5^ffcb4a metros
^ffcb4aCusto de MP: ^96f5ff115^ffffff
^ffcb4aCanalização: ^ffffff0,1 ^ffcb4asegundos
^ffcb4aConjuração: ^ffffff1,5 ^ffcb4asegundos
^ffcb4aEspera: ^ffffff15 ^ffcb4asegundos
^ffcb4aVitalidade: ^ffffff50
^ffcb4aArma: ^ffffffAdaga
^ffcb4aBônus de Runa: ^ff5c00Poder do Dragão

^ffcb4aCultivo necessário: ^ffffffAlma Caótica
Corte o inimigo com um golpe vigoroso
das suas adagas, causando dano físico básico mais ^96f5ff5211^ffffff.
Imobiliza o alvo por ^96f5ff10^ffffff segundos e causa lentidão
por ^96f5ff20^ffffff segundos, reduzindo a velocidade de movimento em ^96f5ff50%%^ffffff.

^d618e7Consome todas as Cargas de Ataque para aumentar a efetividade. Ganha uma Carga de Ataque adicional quando completa.

^00ff00Efeito da Runa Verdejante:@1
^00ff00Nível 1 a 4: Causa ^96f5ff120%%^00ff00 a mais de dano por arma e mais ^96f5ff1430^00ff00.
       Cada Carga de Ataque aumenta em ^96f5ff40%%^00ff00 o dano por arma.
^00ff00Nível 5 a 7: Causa ^96f5ff180%%^00ff00 a mais de dano por arma e mais ^96f5ff2145^00ff00.
       Cada Carga de Ataque aumenta em ^96f5ff60%%^00ff00 o dano por arma.
^00ff00Nível 8 a 9: Causa ^96f5ff240%%^00ff00 a mais de dano por arma e mais ^96f5ff2860^00ff00.
       Cada Carga de Ataque aumenta em ^96f5ff80%%^00ff00 o dano por arma.
^00ff00Nível 10: Causa ^96f5ff300%%^00ff00 a mais de dano por arma e mais ^96f5ff3575^00ff00.
     Cada Carga de Ataque aumenta em ^96f5ff100%%^00ff00 o dano por arma.@1@2
Causa ^96f5ff%d%%^00ff00 de dano adicional por arma mais ^96f5ff%d^00ff00.
Cada Carga de Ataque aumenta em ^96f5ff%d%%^00ff00 o dano por arma.@2
"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL3813 
#define __CPPGEN_GNET_SKILL3813 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill3813:public Skill 
    { 
    public: 
        enum { SKILL_ID = 3813 }; 
        Skill3813 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill3813Stub:public SkillStub 
    { 
    public: 
#define get_glyph(level, l1_4, l5_7, l8_9, lmax) level >= 0 && level <= 4 ? l1_4 : level >= 5 && level <= 7 ? l5_7 : level >= 8 && level <= 9 ? l8_9 : lmax; 

#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 133;
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
                skill->GetPlayer ()->SetDecmp (15);
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
                return 1533;
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
                float _glyph_value = get_glyph(skill->GetLevel (), 1.2, 1.8, 2.4, 3);
                int _glyph_value_2 = get_glyph(skill->GetLevel (), 1430, 2145, 2860, 3575);

                float _charge_merc_count = skill->GetPlayer()->GetChargeMerc();
                float _glyph_plus_from_charge = get_glyph(skill->GetLevel (), 0.4, 0.6, 0.8, 1.0);

                skill->GetPlayer ()->SetDecmp (100);
                skill->SetPlus (5210.8 + _glyph_value_2);
                skill->SetRatio (_glyph_value + (_glyph_plus_from_charge * _charge_merc_count));
                skill->SetDamage (skill->GetAttack ());
                skill->GetPlayer ()->SetPerform (1);

                // Remove as cargas
                skill->GetPlayer ()->SpendChargeMerc(_charge_merc_count);
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
    Skill3813Stub ():SkillStub (3813) 
    { 
        cls                 = 5; 
#ifdef _SKILL_CLIENT 
        name                = L"�񡤰���ն"; 
        nativename          = "�񡤰���ն"; 
        icon                = "�񡤰���ն��.dds"; 
#endif 
        max_level           = 1; 
        type                = 1; 
        allow_ride          = 0; 
        attr                = 1; 
        rank                = 40; 
        eventflag           = 0; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1613; 
        allow_forms         = 1; 
        apcost              = 0; 
        apgain              = 50; 
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
        effect              = "�̿�_�Ͻ�.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 0; 
        has_stateattack     = 0; 
        runes_attr          = 2; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = "����������������.gfx"; 
        gfxhangpoint        = "HH_spine"; 
#endif 
        gfxtarget           = 0; 
        feature             = 0; 
        extra_effect        = 0; 
        immune_casting      = 0; 
        restrict_weapons.push_back (23749); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
        statestub.push_back (new State2 ()); 
        statestub.push_back (new State3 ()); 
#endif 
    } 
    int GetExecutetime (Skill * skill) const 
    {
        static int array[10] = { 1333,1333,1333,1333,1333,1333,1333,1333,1333,1333 };
        return array[skill->GetLevel () - 1];
    }
    int GetCoolingtime (Skill * skill) const
        {
            return 15000;
        }
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 1500000,1500000,1500000,1500000,1500000,1500000,1500000,1500000,1500000,1500000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 92,92,92,92,92,92,92,92,92,92 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredItem (Skill * skill) const 
    { 
        static int aarray[10] = { 11208,11208,11208,11208,11208,11208,11208,11208,11208,11208 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredMoney (Skill * skill) const 
    { 
        static int aarray[10] = { 1500000,1500000,1500000,1500000,1500000,1500000,1500000,1500000,1500000,1500000 }; 
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
            return (float) (4.5);
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
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (11000);
            skill->GetVictim ()->SetFix (1);
            
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (20000);
            skill->GetVictim ()->SetRatio (0.5);
            skill->GetVictim ()->SetSlow (1);
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
bool BlessMe (Skill * skill) const
        {            
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (4.5 + 1.6 * 5.5);
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
            return (float) (4);
        }
#endif 
#ifdef _SKILL_SERVER 
    void ComboSkEndAction (Skill * skill) const 
    { 
        return; 
    } 
#endif 
#ifdef _SKILL_SERVER 
    int GetChargeMerc  (Skill * skill) const 
    { 
        return 0; 
    } 
#endif 
    }; 
} 
#endif 
