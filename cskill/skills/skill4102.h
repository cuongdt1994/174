//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
41020  "ΨOde Perene"41021  "^ff5c00ΨOde Perene

^ffcb4aAlcance: ^ffffffCorpo a Corpo    
^ffcb4aCusto de MP: ^96f5ff160^ffffff    
^ffcb4aCanalização: ^ffffff0,1 ^ffcb4asegundos    
^ffcb4aConjuração: ^ffffff0,5 ^ffcb4asegundos    
^ffcb4aEspera: ^ffffff6 ^ffcb4a segundos    
^ffcb4aVitalidade: ^ffffff25
^ffcb4aArma: ^ffffffSabre
^ffcb4aBônus de Runa: ^ff5c00Maestria Marcial

^ffcb4aCultivo necessário: ^ffffffAlma Caótica
Corte novamente pelo arco de seu ataque, atingindo
todos os inimigos em uma área com raio de ^96f5ff120^ffffff graus e alcance de até ^96f5ff8^ffffff
metros. Causa dano físico básico, mais ^96f5ff4521^ffffff
e drena ^96f5ff5^ffffff de Chi do alvo.

^ffa4a9Habilidade combinada: Atalho padrão ^59EE4C Q
^ffa4a9Habilidade necessária: ^59EE4CMarca do Vendaval^96f5ff
Enquanto estiver na Forma Sombria, a espera é reduzida para ^ffffff1^96f5ff segundo.

^00ff00Efeito da Runa Verdejante:@1
^00ff00Nível 1 a 4: O alcance aumenta em ^96f5ff0,5^00ff00 metros. A área aumenta em ^96f5ff1^00ff00 metro.
^00ff00Nível 5 a 7: O alcance aumenta em ^96f5ff1,0^00ff00 metros. A área aumenta em ^96f5ff2^00ff00 metro.
^00ff00Nível 8 a 9: O alcance aumenta em ^96f5ff1,5^00ff00 metros. A área aumenta em ^96f5ff3^00ff00 metro.
^00ff00Nível 10: O alcance aumenta em ^96f5ff2,0^00ff00 metros. A área aumenta em ^96f5ff4^00ff00 metros.@1@2
O alcance aumenta em ^96f5ff%.1f^00ff00 metros. O área aumenta em ^96f5ff%d^00ff00 metros.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL4102 
#define __CPPGEN_GNET_SKILL4102 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill4102:public Skill 
    { 
    public: 
        enum { SKILL_ID = 4102 }; 
        Skill4102 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill4102Stub:public SkillStub 
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
                skill->GetPlayer ()->SetDecmp (0.2 * 160);
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
                return 501;
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
                skill->GetPlayer ()->SetDecmp (0.8 * 160);
                skill->SetPlus (4521);
                skill->SetRatio (0);
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
    Skill4102Stub ():SkillStub (4102) 
    { 
        cls                 = 10; 
#ifdef _SKILL_CLIENT 
        name                = L"�񡤾Ÿ�"; 
        nativename          = "�񡤾Ÿ�"; 
        icon                = "�񡤾Ÿ���.dds"; 
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
        showorder           = 1014; 
        allow_forms         = 3; 
        apcost              = 0; 
        apgain              = 25; 
        doenchant           = 1; 
#ifdef _SKILL_SERVER
        dobless             = 1;
#endif 
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
        combosk_preskill    = 2563; 
        combosk_interval    = 6000; 
        combosk_nobreak     = 0; 
#ifdef _SKILL_CLIENT 
        effect              = "ҹӰ_�Ÿ�_����.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 4; 
        has_stateattack     = 0; 
        runes_attr          = 1; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = "ʩ�ž�������.gfx"; 
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
        static int array[10] = { 501,501,501,501,501,501,501,501,501,501 };
        return array[skill->GetLevel () - 1];
    }
    int GetCoolingtime (Skill * skill) const 
    { 
        static int aarray[2][10] = { {6000,6000,6000,6000,6000,6000,6000,6000,6000,6000} , {1000,1000,1000,1000,1000,1000,1000,1000,1000,1000} }; 
        return aarray[skill->GetPlayer ()->GetForm ()][skill->GetLevel () - 1]; 
    } 
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 30000000,30000000,30000000,30000000,30000000,30000000,30000000,30000000,30000000,30000000 }; 
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
        int _glyph_value = get_glyph(skill->GetLevel (), 1, 2, 3, 4);
        return (float) (8 + _glyph_value);
    }
    float GetAttackdistance (Skill * skill) const
    {
            return (float) (0);
    }
    float GetAngle (Skill * skill) const
    {
            return (float) (1 - 0.0111111 * (90));
        }
    float GetPraydistance (Skill * skill) const
    {
        float _glyph_value = get_glyph(skill->GetLevel (), 0.5, 1, 1.5, 2);
        return (float) ((skill->GetPlayer ()->GetRange () - 1) + _glyph_value);
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
            skill->GetVictim ()->SetValue (-5);
            skill->GetVictim ()->SetAp (1);
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
    bool BlessMe (Skill * skill) const 
    { 
            // Garante o ataque crítico
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetValue (4106);
            skill->GetVictim ()->SetNewValue (6);
            skill->GetVictim ()->SetGlyphCriticalValid (1);
            return true;
    } 
#endif 
#ifdef _SKILL_SERVER 
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (10 + 2);
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
            return (float) (2);
        }
#endif 
#ifdef _SKILL_SERVER 
        void ComboSkEndAction (Skill * skill) const
        {
            skill->GetPlayer ()->SetComboid (2564);
			return;
        }
#endif 
    }; 
} 
#endif 
