//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
34620  "ΨLâmina da Lua"34621  "^ff5c00ΨLâmina da Lua         

^ffcb4aAlcance ^96f5ff27^ffffff ^ffcb4ametros 
^ffcb4aCusto em MP ^96f5ff276^ffffff
^ffcb4aCanalização ^ffffff1,1^ffcb4a segundos 
^ffcb4aConjuração ^ffffff2,1^ffcb4a segundo 
^ffcb4aEspera ^ffffff15.0^ffcb4a segundos 
^ffcb4aArma ^ffffffFoice ou Desarmado
^ffcb4aBônus de Runa ^ff5c00Maestria Marcial

^ffcb4aCultivo ^ffffffAlma Celestial
Conjure quatro lâminas mágicas da lua para atacar o inimigo consecutivamente
causando dano mágico básico mais ^96f5ff250%%^ffffff do dano 
da arma e mais ^96f5ff10859^ffffff como Dano de Metal e Água. O primeiro 
ataque tem ^96f5ff25%%^ffffff de chance de aumentar o dano recebido 
pelo alvo em ^96f5ff30%%^ffffff por ^96f5ff5^ffffff segundos. O 
quarto ataque tem ^96f5ff25%%^ffffff de chance de
recuperar HP igual a ^96f5ff12%%^ffffff do seu ataque mágico básico mais ^96f5ff1303^ffffff.
Este efeito pode coexistir com a Chama do Paraíso.
Este efeito cancelará o Veneno Profundo.

Enquanto você estiver na Forma Sombria,
o tempo de espera é reduzido para ^96f5ff6^ffffff segundos 
Sem Cargas: Tem ^96f5ff100%%^ffffff de chance de acionar o efeito do primeiro ataque.
^96f5ff1^ffffff Carga de Trovão e ^96f5ff1^ffffff Carga de Gelo: Tem ^96f5ff100%%^ffffff de chance de acionar o efeito de cura do quarto ataque.
Quando houver mais Cargas de Trovão do que Cargas de Gelo: Tem ^96f5ff100%%^ffffff de chance de acionar o efeito do primeiro ataque.
Quando houver mais Cargas de Gelo do que Cargas de Trovão: Tem ^96f5ff100%%^ffffff de chance de acionar o efeito de cura do quarto ataque.

^a6caf0Custa ^ffffff50^a6caf0 Chi.

^ffa4a9Pode ser conjurada em movimento."*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL3462 
#define __CPPGEN_GNET_SKILL3462 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill3462:public Skill 
    { 
    public: 
        enum { SKILL_ID = 3462 }; 
        Skill3462 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill3462Stub:public SkillStub 
    { 
    public: 
#define get_glyph(level, l1_4, l5_7, l8_9, lmax) level >= 0 && level <= 4 ? l1_4 : level >= 5 && level <= 7 ? l5_7 : level >= 8 && level <= 9 ? l8_9 : lmax; 

#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1101;
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
                skill->GetPlayer ()->SetDecmp (276);
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
                return 801;
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
                skill->SetSection (1);
                skill->SetPlus (10859);
                skill->SetRatio (2.5);
                skill->SetWaterdamage (0.05 * skill->GetMagicattack ());
                skill->SetGolddamage (0.05 * skill->GetMagicattack ());
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
#ifdef _SKILL_SERVER 
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 268;
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
                skill->SetSection (2);
                skill->SetPlus (10859);
                skill->SetRatio (2.5);
                skill->SetWaterdamage (0.1 * skill->GetMagicattack ());
                skill->SetGolddamage (0.1 * skill->GetMagicattack ());
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
#ifdef _SKILL_SERVER 
        class State5:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 301;
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
                skill->SetSection (3);
                skill->SetPlus (10859);
                skill->SetRatio (2.5);
                skill->SetWaterdamage (0.15 * skill->GetMagicattack ());
                skill->SetGolddamage (0.15 * skill->GetMagicattack ());
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
#ifdef _SKILL_SERVER 
        class State6:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 701;
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
                skill->SetSection (4);
                skill->SetPlus (10859);
                skill->SetRatio (2.5);
                skill->SetWaterdamage (0.2 * skill->GetMagicattack ());
                skill->SetGolddamage (0.2 * skill->GetMagicattack ());
                skill->GetPlayer ()->SetPerform (0);
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
        class State7:public SkillStub::State
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
    Skill3462Stub ():SkillStub (3462) 
    { 
        cls                 = 11; 
#ifdef _SKILL_CLIENT 
        name                = L"���»�����"; 
        nativename          = "���»�����"; 
        icon                = "���»�����.dds"; 
#endif 
        max_level           = 1; 
        type                = 1; 
        allow_ride          = 0; 
        attr                = 2; 
        rank                = 41; 
        eventflag           = 0; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 1; 
        npcdelay            = 0; 
        showorder           = 1024; 
        allow_forms         = 3; 
        apcost              = 50; 
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
        effect              = "����_�»�����_����.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 0; 
        has_stateattack     = 0; 
        runes_attr          = 1; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = ""; 
        gfxhangpoint        = "0"; 
#endif 
        gfxtarget           = 0; 
        feature             = 0; 
        extra_effect        = 0; 
        immune_casting      = 0; 
        pre_skills.push_back (std::pair < ID, int >(2913, 1)); 
        pre_skills.push_back (std::pair < ID, int >(2914, 1)); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
        statestub.push_back (new State2 ()); 
        statestub.push_back (new State3 ()); 
        statestub.push_back (new State4 ()); 
        statestub.push_back (new State5 ()); 
        statestub.push_back (new State6 ()); 
        statestub.push_back (new State7 ()); 
#endif 
    } 
    int GetExecutetime (Skill * skill) const 
    {
        static int array[10] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
        return array[skill->GetLevel () - 1];
    }
    int GetCoolingtime (Skill * skill) const 
    { 
        static int aarray[2][10] = { {15000,15000,15000,15000,15000,15000,15000,15000,15000,15000} , {6000,6000,6000,6000,6000,6000,6000,6000,6000,6000} }; 
        return aarray[skill->GetPlayer ()->GetForm ()][skill->GetLevel () - 1]; 
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
        static int aarray[10] = { 54004,54004,54004,54004,54004,54004,54004,54004,54004,54004 }; 
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
            return (float) (27 + skill->GetPlayer ()->GetPrayrangeplus ());
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
			skill->GetVictim ()->SetProbability (1.0 * skill->GetSection () == 1 ? 
                                                        skill->GetT0 () == 1 ? 
                                                         skill->GetT1 () == 0 ? 100 : 
                                                         skill->GetT1 () >= 10 ? 100 : 25 : 25 : 0);
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetRatio (0.3);
            skill->GetVictim ()->SetDeeppoision (1);
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
bool BlessMe (Skill * skill) const
        {            
			skill->GetVictim ()->SetProbability (1.0 * skill->GetSection () == 4 ? 
                                                        skill->GetT0 () == 1 ? 
                                                         skill->GetT1 () == 2 ? 100 : 
                                                         skill->GetT1 () == 7 ? 100 : 25 : 25 : 0);
            skill->GetVictim ()->SetValue (0.12 * skill->GetT1 () + 1303);
            skill->GetVictim ()->SetHeal (1);
           return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (36.5 + skill->GetPlayer ()->GetPrayrangeplus ());
        }		
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent0 (PlayerWrapper * player) const
        {
            return (float) (player->GetForm ());
        }
#endif 
#ifdef _SKILL_SERVER 
    float GetTalent1 (PlayerWrapper * player) 
    { 
        return (int) (player->GetBalls ());
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
