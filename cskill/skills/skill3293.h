//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
32930  "ΣLuar Crescente"32931  "^ffff00ΣLuar Crescente

^ffcb4aCusto em MP: ^96f5ff400^ffffff
^ffcb4aConjuração: ^ffffffInstantâneo
^ffcb4aEspera: ^ffffff90,0^ffcb4a segundos
^ffcb4aArma: ^ffffff Foice, Desarmado

^ffcb4aCultivo: ^ffffffMahayana

Conforme os poderes de Ceifadores diminuiram, os Tormentadores de antigamente
os compensaram invocando os poderes da lua para fortalecer seus ataques. Esta 
técnica tem sido passada de geração para geração dos Sombrios, por milhares de anos.

Envolva sua foice com a luz do luar para aumentar o alcance e a área das suas habilidades em ^96f5ff15^ffffff metros
por ^96f5ff8^ffffff segundos. Reduz o tempo de canalização em ^96f5ff50%%^ffffff por ^96f5ff3^ffffff segundos por Carga de Gelo. 
Aumenta o Nível de Ataque em ^96f5ff10^ffffff por Carga de Trovão por ^96f5ff9^ffffff segundos.

^a6caf0Custa ^ffffff50^a6caf0 Chi."*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL3293 
#define __CPPGEN_GNET_SKILL3293 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill3293:public Skill 
    { 
    public: 
        enum { SKILL_ID = 3293 }; 
        Skill3293 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill3293Stub:public SkillStub 
    { 
    public: 
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
                skill->GetPlayer ()->SetDecmp (400);
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
    Skill3293Stub ():SkillStub (3293) 
    { 
        cls                 = 11; 
#ifdef _SKILL_CLIENT 
        name                = L"������������"; 
        nativename          = "������������"; 
        icon                = "��������.dds"; 
#endif 
        max_level           = 1; 
        type                = 2; 
        allow_ride          = 0; 
        attr                = 1; 
        rank                = 8; 
        eventflag           = 0; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 0; 
        allow_forms         = 3; 
        apcost              = 50; 
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
        effect              = "����_��������_����.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 5; 
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
        pre_skills.push_back (std::pair < ID, int >(2629, 1)); 
        restrict_weapons.push_back (0); 
        restrict_weapons.push_back (44879); 
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
        static int aarray[10] = { 90000,90000,90000,90000,90000,90000,90000,90000,90000,90000 }; 
        return aarray[skill->GetLevel () - 1]; 
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
        static int aarray[10] = { 49978,49978,49978,49978,49978,49978,49978,49978,49978,49978 }; 
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
            return (float) (28);
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
            float balls;
            float balls1;
            balls = 0;
            balls1 = 0;
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (9000);
            skill->GetVictim ()->SetAmount (2);
            skill->GetVictim ()->SetValue (15);
            skill->GetVictim ()->SetNewValue (1);
            skill->GetVictim ()->SetIncenchantrange (1);
            skill->GetVictim ()->SetProbability (1.0 * 100);
            if(skill->GetPlayer()->GetBalls() / 5 == 1)
            {
                balls = 3000;
            }
            else 
            {
                if(skill->GetPlayer()->GetBalls() / 5 == 2)
                {
                balls = 6000; 
                }
                else
                {
                    if(skill->GetPlayer()->GetBalls() / 5 == 3)
                    {
                        balls = 9000;
                    }
                    else
                    {
                        balls = 100;
                    }
                }
            }
            skill->GetVictim ()->SetTime(balls);
            skill->GetVictim ()->SetRatio(0.5);
            skill->GetVictim ()->SetFastpray(1);
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime(9000);
            skill->GetVictim ()->SetShowicon(0);
            if(skill->GetPlayer()->GetBalls() / 5 == 1)
            {
                balls1 = 10;
            }
            else 
            {
                if(skill->GetPlayer()->GetBalls() / 5 == 2)
                {
                balls1 = 20; 
                }
                else
                {
                    if(skill->GetPlayer()->GetBalls() / 5 == 3)
                    {
                        balls1 = 30;
                    }
                    else
                    {
                        balls1 = 1;
                    }
                }
            }
            skill->GetVictim ()->SetValue(balls1);
            skill->GetVictim ()->SetAddattackdegree(1);
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
