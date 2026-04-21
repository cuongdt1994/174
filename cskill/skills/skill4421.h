//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
44210  "ΨDragão Nascente"44211  "^ff5c00ΨDragão Ascendente

^ffcb4aAlcance ^ffffffCorpo a corpo
^ffcb4aMP ^96f5ff140^ffffff
^ffcb4aCanalização ^ffffff0,4 ^ffcb4asegundos
^ffcb4aConjuração ^ffffff1,2 ^ffcb4asegundos
^ffcb4aEspera ^ffffff15 ^ffcb4asegundos
^ffcb4aArma ^ffffffMachado/Martelo, Arma de Haste, Lâmina e Espada
^ffcb4aBônus de Runa ^ff5c00Maestria Marcial

^ffcb4aCultivo Necessário ^ffffffAlma Celestial
Golpeie com sua arma em um ataque ascendente, causando
dano físico básico, mais ^96f5ff105%%^ffffff de dano de arma e mais^96f5ff5834^ffffff.
Atordoa o alvo por ^96f5ff5^ffffff segundos.
Nunca erra.

Tipos diferentes de armas causam efeitos diferentes.
Achas, Martelos e Armas de Haste atordoam o alvo por ^96f5ff7^ffffff segundos.
Armas de Haste: O alcance é aumentado para ^96f5ff20^ffffff metros e paralisa o alvo por ^96f5ff5^ffffff segundos.
Lâminas e Espadas não paralisam o alvo, mas o tempo de canalização é reduzido para ^96f5ff0,1^ffffff segundo.
O tempo de conjuração é reduzido para ^96f5ff1^ffffff segundo, causando dano físico adicional igual a ^96f5ff700%%^ffffff
 de dano de arma.

^a6caf0Custa ^ffffff80^a6caf0 Chi

^00ffffEfeito da Runa Celeste@1
^00ffffNível 1 a 4: Lâminas e Espadas também aumentam o Tempo de Canalização do alvo por ^ffffff10^00ffff segundos em ^ffffff20%%
^00ffffNível 5 a 7: Lâminas e Espadas também aumentam o Tempo de Canalização do alvo por ^ffffff10^00ffff segundos em ^ffffff30%%
^00ffffNível 8 a 9: Lâminas e Espadas também aumentam o Tempo de Canalização do alvo por ^ffffff10^00ffff segundos em ^ffffff40%%
^00ffff Nível 10: Lâminas e Espadas também aumentam o Tempo de Canalização do alvo por ^ffffff10^00ffff segundos em ^ffffff50%%@1@2
^00ffffLâminas e Espadas também aumentam o Tempo de Canalização do alvo por ^ffffff10^00ffff segundos em ^ffffff%d%%@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL4421 
#define __CPPGEN_GNET_SKILL4421 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill4421:public Skill 
    { 
    public: 
        enum { SKILL_ID = 4421 }; 
        Skill4421 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill4421Stub:public SkillStub 
    { 
    public: 
#define get_glyph(level, l1_4, l5_7, l8_9, lmax) level >= 0 && level <= 4 ? l1_4 : level >= 5 && level <= 7 ? l5_7 : level >= 8 && level <= 9 ? l8_9 : lmax; 

#ifdef _SKILL_SERVER 
    class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return skill->GetPlayer ()->GetCurWeapon () == 1 ? 100 : 400;
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
                skill->GetPlayer ()->SetDecmp (28);
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
                return skill->GetPlayer ()->GetCurWeapon () == 1 ? 1000 : 1200;
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
                skill->GetPlayer ()->SetDecmp (112);
                skill->SetPlus (5834);
                skill->SetRatio (skill->GetPlayer ()->GetCurWeapon () == 1 ? 7 : 1.05);
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
    Skill4421Stub ():SkillStub (4421) 
    { 
        cls                 = 0; 
#ifdef _SKILL_CLIENT 
        name                = L"��������"; 
        nativename          = "��������"; 
        icon                = "����������.dds"; 
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
        showorder           = 1117; 
        allow_forms         = 1; 
        apcost              = 80; 
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
        effect              = "��_��������.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 0; 
        has_stateattack     = 0; 
        runes_attr          = 1; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = "����״̬Ч����ǿ1.gfx"; 
        gfxhangpoint        = "HH_spine"; 
#endif 
        gfxtarget           = 0; 
        feature             = 0; 
        extra_effect        = 0; 
        immune_casting      = 0; 
        restrict_weapons.push_back (9); 
        restrict_weapons.push_back (1); 
        restrict_weapons.push_back (5); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
        statestub.push_back (new State2 ()); 
        statestub.push_back (new State3 ()); 
#endif 
    } 
    int GetExecutetime (Skill * skill) const 
    {
        static int array[10] = { 1500 };
        return array[skill->GetLevel () - 1];
    }
    int GetCoolingtime (Skill * skill) const
        {
            return 15000;
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
        static int aarray[10] = { 11208,11208,11208,11208,11208,11208,11208,11208,11208,11208 }; 
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
            return (float) (skill->GetPlayer ()->GetCurWeapon () == 5 ? 20 : skill->GetPlayer ()->GetRange ());
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
            return 7000;
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
            float _glyph_value = get_glyph(skill->GetLevel (), 0.2, 0.3, 0.4, 0.5);

            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT0 () == 5 ? 100 : skill->GetT0 () == 9 ? 100 : 0));
            skill->GetVictim ()->SetTime (5000 + (skill->GetT0 () == 9 ? 2000 : 0));
            skill->GetVictim ()->SetValue (2);
            skill->GetVictim ()->SetPalsy (1);

            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT0 () == 1 ? 100 : 0));
            skill->GetVictim ()->SetTime (11000);
            skill->GetVictim ()->SetRatio (_glyph_value);
            skill->GetVictim ()->SetSlowpray (1);

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
            return (float) (skill->GetPlayer ()->GetCurWeapon () == 5 ? 28 : skill->GetPlayer ()->GetRange () + 8);
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent0 (PlayerWrapper * player) const
        {
            int res = 0;
            res = player->GetCurWeapon();

            return res;
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent1 (PlayerWrapper * player) const
        {
            return (float) 0;
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent2 (PlayerWrapper * player) const
        {
            return 0;
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent3 (PlayerWrapper * player) const
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
            return 13;
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
            return (float) (1.7);
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
