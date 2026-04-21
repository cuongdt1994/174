//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
23750  "ΩDragão Nascente"
23751  "^ffff00ΩDragão Nascente

^ffcb4aAlcance ^ffffffCorpo a Corpo
^ffcb4aMP ^96f5ff140^ffffff
^ffcb4aCanalização ^ffffff0,4 ^ffcb4asegundo
^ffcb4aConjuração ^ffffff1,5 ^ffcb4asegundos
^ffcb4aEspera ^ffffff15,0 ^ffcb4asegundos
^ffcb4aArma ^ffffffArmas Corpo a Corpo

^ffcb4aCultivo Necessário ^ffffffDiabólico
Golpeie com sua arma em um ataque ascendente, causando
dano físico base mais ^96f5ff105%%^ffffff do Ataque da Arma mais ^96f5ff5.834^ffffff
de dano Físico e Paralisando o alvo por ^96f5ff4^ffffff segundos.

Tipos diferentes de armas causam efeitos diferentes.
Machados, Achas e Armas de Haste atordoam o alvo por ^96f5ff6^ffffff segundos.
Lâminas Afiadas, Piques e Clavas possuem um alcance de conjuração estendido em ^96f5ff20^ffffff metros, e causam um dano Físico adicional igual a ^96f5ff100%%^ffffff do Ataque da Arma.
Lâminas e Espadas reduzem o tempo de canalização para ^96f5ff0,2 ^ffffffsegundos
e tempo de conjuração para ^96f5ff1.4 ^ffffffsegundos e concede um bônus de ^96f5ff300%% ^ffffff do dano da arma. 

^a6caf0Custa ^ffffff1 ^a6caf0Chi.

^d618e7A duração da Paralisia é aumentada em ^96f5ff1,5^d618e7 segundos.
*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL2375 
#define __CPPGEN_GNET_SKILL2375 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill2375:public Skill 
    { 
    public: 
        enum { SKILL_ID = 2375 }; 
        Skill2375 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill2375Stub:public SkillStub 
    { 
    public: 
#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return skill->GetPlayer ()->GetCurWeapon () == 1 ? 200 : 400;
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
                return skill->GetPlayer ()->GetCurWeapon () == 1 ? 1400 : 1500;
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
                skill->SetRatio (skill->GetPlayer ()->GetCurWeapon () == 5 ? 1 : (skill->GetPlayer ()->GetCurWeapon () == 1 ? 3 : 1.05));
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
    Skill2375Stub ():SkillStub (2375) 
    { 
        cls                 = 0; 
#ifdef _SKILL_CLIENT 
        name                = L"�ġ�������"; 
        nativename          = "�ġ�������"; 
        icon                = "�ġ�������.DDS"; 
#endif 
        max_level           = 1; 
        type                = 1; 
        allow_ride          = 0; 
        attr                = 7; 
        rank                = 30; 
        eventflag           = 0; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1117; 
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
        effect              = "��_��������.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 0; 
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
        pre_skills.push_back (std::pair < ID, int >(407, 1)); 
        pre_skills.push_back (std::pair < ID, int >(415, 1)); 
        pre_skills.push_back (std::pair < ID, int >(399, 1)); 
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
            return 1500;
        }
    int GetCoolingtime (Skill * skill) const
        {
            return 15000;
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
        static int aarray[10] = { 42136,42136,42136,42136,42136,42136,42136,42136,42136,42136 }; 
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
            return 3000;
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
            skill->GetVictim ()->SetTime (6500 + ((int)skill->GetT0 () == 9 ? 2000 : 0) + 1500);
            skill->GetVictim ()->SetValue (2);
            skill->GetVictim ()->SetPalsy (1);
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
