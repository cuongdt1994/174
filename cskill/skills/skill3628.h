//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
36280  "ΨAtaque de Impacto"36281  "^ff5c00ΨAtaque de Impacto

^ffcb4aAlcance ^96f5ff28,5^ffffff ^ffcb4ametros
^ffcb4aCusto em MP ^96f5ff320^ffffff
^ffcb4aCanalização ^ffffff0,5 ^ffcb4asegundos
^ffcb4aConjuração ^ffffff1,0 ^ffcb4asegundos
^ffcb4aEspera ^ffffff2,0 ^ffcb4asegundos
^ffcb4aChi Recebido ^ffffff10
^ffcb4aArma ^ffffffEsfera
^ffcb4aBônus de Runa ^ff5c00Poder do Dragão

^ffcb4aCultivo ^ffffffAlma Celestial
Dispara uma explosão de água em seu alvo, causando
dano mágico básico, mais ^96f5ff100%%^ffffff de dano de arma e mais^96f5ff3573^ffffff
de dano de Água. Desacelera o alvo em ^96f5ff50%%^ffffff por ^96f5ff8^ffffffsegundos.
Ao atingir o alvo, tem ^96f5ff20%%^ffffff de chance de reduzir o Chi dele em^96f5ff30^ffffff.

A Taxa de Acerto Crítico contra
alvos não jogadores aumenta em ^96f5ff15%%^ffffff.

^ff0000Efeito da Runa Escarlate:@1
^ff0000Nível 1 a 4: Gera ^96f5ff15 Chi.
^ff0000Nível 5 a 7: Gera ^96f5ff20 Chi.
^ff0000Nível 8 a 9: Gera ^96f5ff25 Chi.
^ff0000Nível 10: Gera ^96f5ff30 de Chi.@1@2
Gera ^96f5ff%d de Chi.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL3628 
#define __CPPGEN_GNET_SKILL3628 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill3628:public Skill 
    { 
    public: 
        enum { SKILL_ID = 3628 }; 
        Skill3628 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill3628Stub:public SkillStub 
    { 
    public: 
#define get_glyph(level, l1_4, l5_7, l8_9, lmax) level >= 0 && level <= 4 ? l1_4 : level >= 5 && level <= 7 ? l5_7 : level >= 8 && level <= 9 ? l8_9 : lmax; 

#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
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
                skill->GetPlayer ()->SetDecmp (64);
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
                skill->GetPlayer ()->SetDecmp (256);
                skill->SetPlus (3573);
                skill->SetRatio (1);
                skill->SetWaterdamage (skill->GetMagicattack ());
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
    Skill3628Stub ():SkillStub (3628) 
    { 
        cls                 = 2; 
#ifdef _SKILL_CLIENT 
        name                = L"�񡤳����"; 
        nativename          = "�񡤳����"; 
        icon                = "�񡤳������.dds"; 
#endif 
        max_level           = 1; 
        type                = 1; 
        allow_ride          = 0; 
        attr                = 4; 
        rank                = 41; 
        eventflag           = 0; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1701; 
        allow_forms         = 1; 
        apcost              = 0; 
        apgain              = 10; 
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
        effect              = "��_�弼��_����.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 0; 
        has_stateattack     = 0; 
        runes_attr          = 2; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = "���Ԫ������.gfx"; 
        gfxhangpoint        = "HH_bind"; 
#endif 
        gfxtarget           = 0; 
        feature             = 0; 
        extra_effect        = 0; 
        immune_casting      = 0; 
        restrict_weapons.push_back (25333); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
        statestub.push_back (new State2 ()); 
        statestub.push_back (new State3 ()); 
#endif 
    } 
    int GetExecutetime (Skill * skill) const 
    {
        static int array[10] = { 1200,1200,1200,1200,1200,1200,1200,1200,1200,1200 };
        return array[skill->GetLevel () - 1];
    }
    int GetCoolingtime (Skill * skill) const
        {
            return 2000;
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
            return (float) (1 - 0.0111111 * (120));
        }
    float GetPraydistance (Skill * skill) const
    {
            return (float) (28.5);
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
            skill->GetVictim ()->SetTime (9000);
            skill->GetVictim ()->SetRatio (0.4);
            skill->GetVictim ()->SetSlow (1);
            
            skill->GetVictim ()->SetProbability (1.0 * skill->GetPlayer ()->GetCls () == -1 ? 100 : 0);
            skill->GetVictim ()->SetTime (9000);
            skill->GetVictim ()->SetAmount (15);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetIncbecritrate (1);

            skill->GetVictim ()->SetProbability (1.0 * 20);
            skill->GetVictim ()->SetValue (-30);
            skill->GetVictim ()->SetAp (1);
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
bool BlessMe (Skill * skill) const
        {            

            int _glyph_value = get_glyph(skill->GetLevel (), 5, 10, 15, 20);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetValue (_glyph_value);
            skill->GetVictim ()->SetAp (1);

			return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (39.5);
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
