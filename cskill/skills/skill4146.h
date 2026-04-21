//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
41460  "ΨNuvem Distante"41461  "^ff5c00ΨNuvem Distante

^ffcb4aAlcance: ^96f5ff12^ffffff ^ffcb4ametros   
^ffcb4aCusto de MP: ^96f5ff174^ffffff    
^ffcb4aCanalização: ^ffffff0,1 ^ffcb4asegundos    
^ffcb4aConjuração: ^ffffff1,8 ^ffcb4asegundos    
^ffcb4aEspera: ^ffffff60 ^ffcb4asegundos    
^ffcb4aArma: ^ffffffSabre
^ffcb4aBônus de Runa: ^ff5c00Fúria do Tigre

^ffcb4aCultivo necessário: ^ffffffAlma Celestial
Lance o alvo no ar com um golpe poderoso,
causando dano físico básico, mais ^96f5ff200%%^ffffff de
dano por arma e mais ^96f5ff8933^ffffff. Aumenta a sua Taxa de
Acerto Crítico em ^96f5ff40%%^ffffff por ^96f5ff6^ffffff segundos. Zera a espera
da Deriva Implacável e remove a espera
da próxima Deriva Implacável nos ^96f5ff6^ffffff segundos seguintes.

^a6caf0Custa ^ffffff50^a6caf0 de Chi.

^ff0000Efeito da Runa Escarlate:@1
^ff0000Nível 1 a 4: Aumenta sua Taxa de Acerto Crítico em ^96f5ff2^ff0000 segundos.
^ff0000Nível 5 a 7: Aumenta sua Taxa de Acerto Crítico em ^96f5ff3^ff0000 segundos.
^ff0000Nível 8 a 9: Aumenta sua Taxa de Acerto Crítico em ^96f5ff4^ff0000 segundos.
^ff0000Nível 10: Aumenta sua Taxa de Acerto Crítico em ^96f5ff5^ff0000 segundos.@1@2
Aumenta sua Taxa de Acerto Crítico em ^96f5ff%d^ff0000 segundos.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL4146 
#define __CPPGEN_GNET_SKILL4146 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill4146:public Skill 
    { 
    public: 
        enum { SKILL_ID = 4146 }; 
        Skill4146 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill4146Stub:public SkillStub 
    { 
    public: 
#define get_glyph(level, l1_4, l5_7, l8_9, lmax) level >= 0 && level <= 4 ? l1_4 : level >= 5 && level <= 7 ? l5_7 : level >= 8 && level <= 9 ? l8_9 : lmax; 

#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 67;
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
                skill->GetPlayer ()->SetDecmp (34.79);
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
                return 2001;
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
                skill->GetPlayer ()->SetDecmp (139.2);
                skill->SetPlus (8933);
                skill->SetRatio (2);
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
    Skill4146Stub ():SkillStub (4146) 
    { 
        cls                 = 10; 
#ifdef _SKILL_CLIENT 
        name                = L"�����ƺ�"; 
        nativename          = "�����ƺ�"; 
        icon                = "�����ƺ���.dds"; 
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
        showorder           = 1011; 
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
        effect              = "ҹӰ_��_���ƺ�_����.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 0; 
        has_stateattack     = 0; 
        runes_attr          = 3; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = "״̬����ʱ������1.gfx"; 
        gfxhangpoint        = "HH_bind"; 
#endif 
        gfxtarget           = 0; 
        feature             = 0; 
        extra_effect        = 0; 
        immune_casting      = 0; 
        restrict_weapons.push_back (44878); 
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
            return 60000;
        }
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 3000000,3000000,3000000,3000000,3000000,3000000,3000000,3000000,3000000,3000000 }; 
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
            return (float) (skill->GetPlayer ()->GetRange () + 10);
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
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetValue (2);
            skill->GetVictim ()->SetPalsy (1);
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
bool BlessMe (Skill * skill) const
        {            
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetRatio(0);
            skill->GetVictim ()->SetAmount(500);
            skill->GetVictim ()->SetValue(4136);
            skill->GetVictim ()->SetSkillcooldown(1);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetRatio(0);
            skill->GetVictim ()->SetAmount(500);
            skill->GetVictim ()->SetValue(4137);
            skill->GetVictim ()->SetSkillcooldown(1);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetRatio(0);
            skill->GetVictim ()->SetAmount(500);
            skill->GetVictim ()->SetValue(4138);
            skill->GetVictim ()->SetSkillcooldown(1);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetRatio(0);
            skill->GetVictim ()->SetAmount(500);
            skill->GetVictim ()->SetValue(4139);
            skill->GetVictim ()->SetSkillcooldown(1);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetRatio(0);
            skill->GetVictim ()->SetAmount(500);
            skill->GetVictim ()->SetValue(4140);
            skill->GetVictim ()->SetSkillcooldown(1);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetRatio(0);
            skill->GetVictim ()->SetAmount(500);
            skill->GetVictim ()->SetValue(4141);
            skill->GetVictim ()->SetSkillcooldown(1);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetRatio (0.1);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetValue (4231);
            skill->GetVictim ()->SetAurabless2 (1);

			int _glyph_value = get_glyph(skill->GetLevel (), 2000, 3000, 4000, 5000);

            float value = 40;
			skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (7000 + _glyph_value);
            skill->GetVictim ()->SetValue (value);
            skill->GetVictim ()->SetIncsmite (1);
            return true;
          
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 12.375 + 10);
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
    }; 
} 
#endif 
