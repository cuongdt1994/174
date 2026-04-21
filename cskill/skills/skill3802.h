//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
38020  "ΨAdaga Voadora"38021  "^ff5c00ΨAdaga Voadora
 
 ^ffcb4a Alcance ^96f5ff35^ffffff ^ffcb4a m
 ^ffcb4a Mana ^96f5ff85^ffffff
 ^ffcb4a Canalização ^ffffff0.1 ^ffcb4a s
 ^ffcb4a Conjuração ^ffffff0.9 ^ffcb4a s
 ^ffcb4a Intervalo ^ffffff10.0 ^ffcb4a s
 ^ffcb4a Ganho de Chi ^ffffff20
 ^ffcb4a Arma ^ffffff Adagas
 ^ffcb4a Bônus de Runa ^ff5c00 Poder do Dragão
 
 ^ffcb4a Cultivo ^ffffff Alma Celestial
Jogue sua adaga no alvo, causando dano físico básico, mais ^96f5ff1888^ffffff e interrompendo a invocação do alvo.
 
 ^d618e7 Consome todas as Cargas para aumentar a eficácia.
 
 ^00ffff Efeito da Runa Celeste @1
 Atinge todos os alvos em um raio de ^ffffff20^00ffff graus por Carga. 
 ^00ffffNv. 1-4: Causa dano adicional equivalente a ^ffffff100%%^00ffff do Ataque da Arma, mais ^ffffff1860^00ffff.
 ^00ffffNv. 5-7: Causa dano adicional equivalente a ^ffffff150%%^00ffff do Ataque da Arma, mais ^ffffff2790^00ffff. 
 ^00ffffNv. 8-9: Causa dano adicional equivalente a ^ffffff200%%^00ffff do Ataque da Arma, mais ^ffffff3720^00ffff.
 ^00ffff Nv. 10: Causa dano adicional equivalente a ^ffffff250%%^00ffff do Ataque da Arma, mais ^ffffff4650^00ffff.@1@2
Cada custo de Carga aumenta em ^ffffff20^00ffff graus do raio da área. Causa dano adicional equivalente a ^ffffff%d%%^00ffff do Ataque da Arma, mais ^ffffff%d^00ffff.@2


"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL3802 
#define __CPPGEN_GNET_SKILL3802 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill3802:public Skill 
    { 
    public: 
        enum { SKILL_ID = 3802 }; 
        Skill3802 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill3802Stub:public SkillStub 
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
                skill->GetPlayer ()->SetDecmp (17);
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
                return 900;
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
                float _glyph_value = get_glyph(skill->GetLevel (), 1, 1.5, 2, 2.5);
                int _glyph_value_2 = get_glyph(skill->GetLevel (), 1860, 2790, 3720, 4650);

                skill->GetPlayer ()->SetDecmp (68);
                skill->SetPlus (1888.5 + _glyph_value_2);
                skill->SetRatio (_glyph_value);
                skill->SetDamage (skill->GetAttack ());
                skill->GetPlayer ()->SetPerform (1);

                // Remove as cargas
                float _charge_merc_count = skill->GetPlayer()->GetChargeMerc();
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
    Skill3802Stub ():SkillStub (3802) 
    { 
        cls                 = 5; 
#ifdef _SKILL_CLIENT 
        name                = L"���������"; 
        nativename          = "���������"; 
        icon                = "�񡤶��ɵ���.dds"; 
#endif 
        max_level           = 1; 
        type                = 1; 
        allow_ride          = 0; 
        attr                = 1; 
        rank                = 41; 
        eventflag           = 0; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1609; 
        allow_forms         = 1; 
        apcost              = 0; 
        apgain              = 20; 
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
        effect              = "�̿�_���ɵ�.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 4; 
        has_stateattack     = 0; 
        runes_attr          = 2; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = "Ӱ�췶Χ����1.gfx"; 
        gfxhangpoint        = "HH_bind"; 
#endif 
        gfxtarget           = 0; 
        feature             = 601; 
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
            return 1000;
        }
    int GetCoolingtime (Skill * skill) const
        {
            return 10000;
        }
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 1000000,1000000,1000000,1000000,1000000,1000000,1000000,1000000,1000000,1000000 }; 
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
        static int aarray[10] = { 1000000,1000000,1000000,1000000,1000000,1000000,1000000,1000000,1000000,1000000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredRealmLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    }
    float GetRadius(Skill *skill) const
    {
        float _charge_merc_count = skill->GetPlayer()->GetChargeMerc();
        return (float)(_charge_merc_count > 0 ? 35 : 0);
    }
    float GetAttackdistance(Skill *skill) const
    {
        return (float)(35);
    }
    float GetAngle(Skill *skill) const
    {
        float _charge_merc_count = skill->GetPlayer()->GetChargeMerc();
        return (float)(1 - 0.0111111 * (20 * _charge_merc_count + 1));
    }
    float GetPraydistance(Skill *skill) const
    {
        return (float)(35);
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
            skill->GetVictim ()->SetBreak (1);
            
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
            return (float) (43);
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
            return (float) (3);
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
