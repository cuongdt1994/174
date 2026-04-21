//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
51870  "ΨCântico Mortal"51871  "^ff5c00ΨCântico Mortal

^ffcb4aAlcance ^ffffffCorpo a Corpo
^ffcb4aMP ^96f5ff95,6^ffffff
^ffcb4aCanalização ^ffffff0,1 ^ffcb4asegundo(s)
^ffcb4aConjuração ^ffffff0,7 ^ffcb4asegundos
^ffcb4aEspera ^ffffff1,0 ^ffcb4asegundo(s)
^ffcb4aVitalidade ^ffffff20
^ffcb4aArma ^ffffffEspada e Escudo
^ffcb4aBônus de Runa    ^ff5c00Fúria do Tigre

^ffcb4aCultivo  ^ffffffAlma Caótica
Corta com uma espada pesada, causando Dano Físico equivalente ao
seu Ataque Físico base, mais ^96f5ff50%%^ffffff do Ataque do equipamento e ^96f5ff7.850^ffffff.

^ff0000Efeito da Runa Escarlate:@1
^ff0000Nível 1 a 4: Ao acertar um golpe crítico, causa dano adicional de Transposição equivalente a
      ^96f5ff2%%^ff0000 do seu HP Máximo, mas não mais do que
      ^96f5ff10%% do HP Máximo do alvo. Não é eficaz contra monstros.
^ff0000Nível 5 a 7: Ao acertar um golpe crítico, causa dano adicional de Transposição equivalente a
      ^96f5ff3%%^ff0000 do seu HP Máximo, mas não mais do que
      ^96f5ff10%% do HP Máximo do alvo. Não é eficaz contra monstros.
^ff0000Nível 8 a 9: Ao acertar um golpe crítico, causa dano adicional de Transposição equivalente a
      ^96f5ff4%%^ff0000 do seu HP Máximo, mas não mais do que
      ^96f5ff10%% do HP Máximo do alvo. Não é eficaz contra monstros.
^ff0000Nível 10: Ao acertar um golpe crítico, causa dano adicional de Transposição equivalente a
      ^96f5ff5%%^ff0000 do seu HP Máximo, mas não mais do que
      ^96f5ff10%% do HP Máximo do alvo. Não é eficaz contra monstros. @1@2
Ao acertar um golpe crítico, causa dano adicional de Transposição equivalente a
^96f5ff%d%%^ff0000 do seu HP Máximo, mas não mais do que
^96f5ff10%%^96f5ff do HP Máximo do alvo. Não é eficaz contra monstros. @2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL5187 
#define __CPPGEN_GNET_SKILL5187 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill5187:public Skill 
    { 
    public: 
        enum { SKILL_ID = 5187 }; 
        Skill5187 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill5187Stub:public SkillStub 
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
                skill->GetPlayer ()->SetDecmp (0.2 * (95.6));
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
                return 800;
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
                skill->GetPlayer ()->SetDecmp (0.8 * (95.6));
                skill->SetPlus (7850);
                skill->SetRatio (0.5);
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
    Skill5187Stub ():SkillStub (5187) 
    { 
        cls                 = 13; 
#ifdef _SKILL_CLIENT 
        name                = L"���˾�ʽ"; 
        nativename          = "���˾�ʽ"; 
        icon                = "���˾�ʽ��.dds"; 
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
        showorder           = 0; 
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
        effect              = "���_��ͨһ��_����.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 0; 
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
        pre_skills.push_back (std::pair < ID, int >(5133, 1)); 
        pre_skills.push_back (std::pair < ID, int >(5134, 1)); 
        restrict_weapons.push_back (59830); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
        statestub.push_back (new State2 ()); 
        statestub.push_back (new State3 ()); 
#endif 
    } 
        int GetExecutetime (Skill * skill) const
        {
            return 800;
        }
    int GetCoolingtime (Skill * skill) const
        {
            return 1000;
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
            return (float) (skill->GetPlayer ()->GetRange ());
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
            return _snwprintf (buffer, length, format, skill->GetLevel (), 95.6);

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
            float _glyph_value = get_glyph(skill->GetLevel (), 0.02, 0.03, 0.04, 0.05);

			skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->CheckCriticalDamage () ? 100 : 0));
            skill->GetVictim ()->SetValue (skill->GetTarget ()->GetCls () != -1 || skill->GetTarget ()->GetCls () != 127 ? ((skill->GetPlayer ()->GetMaxhp () * _glyph_value) > 
                                (skill->GetTarget ()->GetMaxhp () * 0.1) ? (skill->GetTarget ()->GetMaxhp () * 0.1) : (skill->GetPlayer ()->GetMaxhp () * _glyph_value)) : 1);
            skill->GetVictim ()->SetPurehurtFromHp (1);

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
            return (float) (skill->GetPlayer ()->GetRange () + 10);
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
            return 3;
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
