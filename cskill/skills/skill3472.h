//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
34720  "ΨSupercélula"34721  "^ff5c00ΨSupercélula

^ffcb4aAlcance ^96f5ff27^ffffff ^ffcb4ametros 
^ffcb4aCusto em MP ^96f5ff594^ffffff
^ffcb4aCanalização ^ffffff3,5^ffcb4a segundos 
^ffcb4aConjuração ^ffffff1,4^ffcb4a segundo 
^ffcb4aEspera ^ffffff30,0^ffcb4a segundos 
^ffcb4aArma ^ffffffFoice ou Desarmado
^ffcb4aBônus de Runa ^ff5c00Maestria Marcial

^ffcb4aCultivo ^ffffffAlma Celestial
Bombardeie todos os inimigos a até ^96f5ff10^ffffff metros do seu alvo
com uma tempestade intensa, causando dano mágico básico mais
^96f5ff230%%^ffffff do dano da arma e mais ^96f5ff5694^ffffff 
como Dano de Metal. Tem efeitos adicionais 
baseado nas suas cargas.

^96f5ff3^ffffff Cargas de Trovão: Também causa dano mágico básico
mais ^96f5ff230%%^ffffff do dano da arma e mais ^96f5ff5694^ffffff.
^96f5ff3^ffffff Cargas de Gelo: Imobiliza o alvo por ^96f5ff4^ffffff segundos.
^96f5ff2^ffffff Cargas de Trovão, ^96f5ff1^ffffff Carga de Gelo: Aumenta o seu
dano de habilidades em ^96f5ff30%%^ffffff por ^96f5ff20^ffffff segundos.
^96f5ff1^ffffff Cargas de Trovão, ^96f5ff2^ffffff Carga de Gelo: Cura metade do seu HP Máximo
e reduz o dano recebido em ^96f5ff40%%^ffffff por ^96f5ff6^ffffff segundos.

Tem ^96f5ff25%%^ffff00 de chance de ativar a segunda Supercélula
com uma canalização de ^96f5ff2,5^ffff00 segundos que causa metade do dano
e custa 0 de Chi. Dura ^96f5ff30^ffff00 segundos.

^a6caf0Custa ^ffffff2^a6caf0 Chi.

^ff0000Efeito da Runa Escarlate:@1
Se você tiver ^96f5ff1 Carga de Trovão e 2 Cargas de Gelo:^ff0000, você receberá cura e se tornará invencível por ^96f5ff3^ff0000 segundos.
^ff0000Nível 1 a 4: Recupera ^96f5ff55%%^ff0000 do seu HP máximo.
^ff0000Nível 5 a 7: Recupera ^96f5ff60%%^ff0000 do seu HP máximo.
^ff0000Nível 8 a 9: Recupera ^96f5ff75%%^ff0000 do seu HP máximo.
^ff0000Nível 10: Recupera ^96f5ff100%%^ff0000 do seu HP máximo.@1@2
Se você tiver ^96f5ff1 Carga de Trovão e 2 Cargas de Gelo^ff0000, você receberá cura
em ^96f5ff%d%%^ff0000 do seu HP máximo e se tornará invencível por ^96f5ff3^ff0000 segundos.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL3472 
#define __CPPGEN_GNET_SKILL3472 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill3472:public Skill 
    { 
    public: 
        enum { SKILL_ID = 3472 }; 
        Skill3472 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill3472Stub:public SkillStub 
    { 
    public: 
#define get_glyph(level, l1_4, l5_7, l8_9, lmax) level >= 0 && level <= 4 ? l1_4 : level >= 5 && level <= 7 ? l5_7 : level >= 8 && level <= 9 ? l8_9 : lmax; 

#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 3467;
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
                skill->GetPlayer ()->SetDecmp (119);
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
                return 1402;
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
                skill->GetPlayer ()->SetDecmp (475);
                skill->SetPlus (5694);
                skill->SetRatio (2.3);
                skill->SetGolddamage (skill->GetMagicattack ());
                skill->SetDamage ((skill->GetPlayer ()->GetBalls () == 15 ? 1 : 0) * skill->GetMagicattack ());
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
    Skill3472Stub ():SkillStub (3472) 
    { 
        cls                 = 11; 
#ifdef _SKILL_CLIENT 
        name                = L"�񡤵��µӻ�"; 
        nativename          = "�񡤵��µӻ�"; 
        icon                = "�񡤵��µӻ��.dds"; 
#endif 
        max_level           = 1; 
        type                = 1; 
        allow_ride          = 0; 
        attr                = 2; 
        rank                = 41; 
        eventflag           = 0; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1026; 
        allow_forms         = 3; 
        apcost              = 200; 
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
        combosk_nobreak     = 1; 
#ifdef _SKILL_CLIENT 
        effect              = "��_���µӻ�_����.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 3; 
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
        restrict_weapons.push_back (44879); 
        restrict_weapons.push_back (0); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
        statestub.push_back (new State2 ()); 
        statestub.push_back (new State3 ()); 
#endif 
    } 
    int GetExecutetime (Skill * skill) const 
    {
        static int array[10] = { 1402,1402,1402,1402,1402,1402,1402,1402,1402,1402 };
        return array[skill->GetLevel () - 1];
    }
    int GetCoolingtime (Skill * skill) const
        {
            return 30000;
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
            return (float) (10);
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
            skill->GetVictim ()->SetProbability (1.0 * skill->GetT0 () == 3 ? 100 : 0);
            skill->GetVictim ()->SetTime (5402);
            skill->GetVictim ()->SetShowicon (0);
            skill->GetVictim ()->SetGeloAtiradora (1);             
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * skill->GetT0 () == 11 ? 100 : 0);
            skill->GetVictim ()->SetTime (21000);
            skill->GetVictim ()->SetRatio (0.3);
            skill->GetVictim ()->SetEnhanceskilldamagePlayer (1);

            float _glyph_value = get_glyph(skill->GetLevel (), 0.55, 0.60, 0.75, 1);

            skill->GetVictim ()->SetProbability (1.0 * skill->GetT0 () == 7 ? 100 : 0);
            skill->GetVictim ()->SetRatio (_glyph_value);
            skill->GetVictim ()->SetScaleinchp (1);

            skill->GetVictim ()->SetProbability (1.0 * skill->GetT0 () == 7 ? 100 : 0);
            skill->GetVictim ()->SetTime (7401);
            skill->GetVictim ()->SetRatio (0.4);
            skill->GetVictim ()->SetDechurt (1);

            skill->GetVictim ()->SetProbability (1.0 * skill->GetT0 () == 7 ? 100 : 0);
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetShowicon (0);
            skill->GetVictim ()->SetInvincible (1);
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (38 + skill->GetPlayer ()->GetPrayrangeplus ());
        }		
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent0 (PlayerWrapper * player) const
        {
            return (float) (player->GetBalls ());
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent1 (PlayerWrapper * player) const
        {
            return (float) (player->GetMagicattack ());
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
            skill->GetPlayer ()->GetComboarg ()->SetValue (0, skill->GetPlayer ()->GetRand ());
			return;
        }
#endif 
    }; 
} 
#endif 
