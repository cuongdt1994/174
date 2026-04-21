//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
38320  "ΨTudo ou Nada"38321  "^ff5c00ΨTudo ou Nada

^ffcb4aCusto de MP: ^96f5ff200^ffffff
^ffcb4aCanalização: ^ffffffInstantânea
^ffcb4aEspera: ^ffffff60 ^ffcb4asegundos
^ffcb4aVitalidade: ^ffffff230
^ffcb4aArma: ^ffffffAdaga
^ffcb4aBônus de Runa: ^ff5c00Fúria do Tigre

^ffcb4aCultivo necessário: ^ffffffAlma Celestial
Medite para encontrar a paz interior, gerando instantaneamente Chi
e recuperando ^96f5ff10%%^ffffff do seu HP máximo.

^d618e7^d618e7Consome todas as Cargas de Ataque para aumentar a efetividade. Ganha uma Carga de Ataque adicional quando completa.

^00ffffEfeito da Runa Celeste:@1
^00ffffNível 1 a 4: Gera ^ffffff240^00ffff Chi.
       Cura ^ffffff10%%^00ffff do seu HP máximo.
       Gera ^ffffff10^00ffff de Chi adicional por Carga de Ataque.
^00ffffNível 5 a 7: Gera ^ffffff240^00ffff Chi.
       Cura ^ffffff15%%^00ffff do seu HP máximo.
       Gera ^ffffff15^00ffff de Chi adicional por Carga de Ataque.
^00ffffNível 8 a 9: Gera ^ffffff250^00ffff Chi.
       Cura ^ffffff15%%^00ffff do seu HP máximo.
       Gera ^ffffff20^00ffff de Chi adicional por Carga de Ataque.
^00ffffNível 10: Gera ^ffffff250^00ffff Chi.
     Cura ^ffffff20%%^00ffff do seu HP máximo.
     Gera ^ffffff25^00ffff de Chi adicional por Carga de Ataque.@1@2
Gera ^ffffff%d^00ffff de Chi.
Cura ^ffffff%d%%^00ffff do seu HP máximo.
Gera ^ffffff%d^00ffff de Chi adicional por Carga de Ataque.@2
"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL3832 
#define __CPPGEN_GNET_SKILL3832 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill3832:public Skill 
    { 
    public: 
        enum { SKILL_ID = 3832 }; 
        Skill3832 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill3832Stub:public SkillStub 
    { 
    public: 
#define get_glyph(level, l1_4, l5_7, l8_9, lmax) level >= 0 && level <= 4 ? l1_4 : level >= 5 && level <= 7 ? l5_7 : level >= 8 && level <= 9 ? l8_9 : lmax; 

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
                skill->GetPlayer ()->SetDecmp (200);
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
    Skill3832Stub ():SkillStub (3832) 
    { 
        cls                 = 5; 
#ifdef _SKILL_CLIENT 
        name                = L"�񡤱�ˮһս"; 
        nativename          = "�񡤱�ˮһս"; 
        icon                = "�񡤶�����.dds"; 
#endif 
        max_level           = 1; 
        type                = 2; 
        allow_ride          = 0; 
        attr                = 1; 
        rank                = 41; 
        eventflag           = 0; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1625; 
        allow_forms         = 1; 
        apcost              = 0; 
        apgain              = 230; 
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
        auto_attack         = 1; 
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
        effect              = "�̿�_��ˮһս.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 5; 
        has_stateattack     = 0; 
        runes_attr          = 3; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = "����״̬Ч����ǿ1.gfx"; 
        gfxhangpoint        = "HH_spine"; 
#endif 
        gfxtarget           = 0; 
        feature             = 0; 
        extra_effect        = 0; 
        immune_casting      = 0; 
        restrict_weapons.push_back (23749); 
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
            return 60000;
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
            return (float) (skill->GetPlayer ()->GetRange () + skill->GetLevel ());
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
            int _glyph_value = get_glyph(skill->GetLevel (), 10, 10, 20, 20);
			float _charge_merc_count = skill->GetPlayer()->GetChargeMerc();
            int _glyph_plus_from_charge = get_glyph(skill->GetLevel (), 10, 15, 20, 25);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetValue (_glyph_value + (_glyph_plus_from_charge * _charge_merc_count));
            skill->GetVictim ()->SetAp (1);
            
            float _glyph_value_2 = get_glyph(skill->GetLevel (), 0.1, 0.15, 0.15, 0.2);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetRatio (_glyph_value_2);
            skill->GetVictim ()->SetScaleinchp (1);

            // Remove as cargas
            skill->GetPlayer ()->SpendChargeMerc(_charge_merc_count);

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
            return (float) (12);
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
            return (float) (0);
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
