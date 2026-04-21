//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
39020  "ΨCindir a Montanha"39021  "^ff5c00ΨCindir a Montanha

^ffcb4aAlcance ^ffffffCorpo a Corpo
^ffcb4aCusto em MP ^96f5ff%d^ffffff
^ffcb4aCanalização ^ffffff0,3 ^ffcb4asegundos
^ffcb4aConjuração ^ffffff1 ^ffcb4asegundo
^ffcb4aEspera ^ffffff3 ^ffcb4asegundos
^ffcb4aChi Recebido ^ffffff10
^ffcb4aArma ^ffffffArmas Corpo a Corpo
^ffcb4aBônus de Runa ^ff5c00Poder do Dragão

^ffcb4aCultivo Necessário ^ffffffAlma Caótica
Corte o oponente com sua arma impregnada de magia,
causando dano físico base e mais ^96f5ff2800^ffffff de dano físico.

Tipos diferentes de armas causam efeitos diferentes.
Machado e Martelo: Diminui a velocidade do alvo em ^96f5ff 60%% ^ffffff. Dura ^96f5ff10^ffffff segundos. Tem ^96f5ff20%%^ffffff de chance de congelar o alvo por ^96f5ff3^ffffff segundos.
Arma de Haste: Aumenta o alcance para ^96f5ff20^ffffff metros. Causa ^96f5ff150%%^ffffff
de dano de arma.
Lâminas e Espadas: Causa um dano de arma adicional de ^96f5ff310%%^ffffff e mais ^96f5ff1864^ffffff.
Demais armas: O mesmo efeito do Machado e do Martelo.

^ffcb4aEfeito da Runa Áurea:@1
^ffcb4aNível 1 a 4: Causa um dano de arma adicional de ^96f5ff60%%^ffcb4a e mais ^96f5ff2208^ffcb4a.
^ffcb4aNível 5 a 7: Causa um dano de arma adicional de ^96f5ff80%%^ffcb4a e mais ^96f5ff3312^ffcb4a.
^ffcb4aNível 8 a 9: Causa um dano de arma adicional de ^96f5ff100%%^ffcb4a e mais ^96f5ff4416^ffcb4a.
^ffcb4a Nível 10: Causa um dano de arma adicional de ^96f5ff120%%^ffcb4a e mais ^96f5ff5520^ffcb4a.@1@2
^ffcb4aCausa um dano adicional de arma de ^96f5ff%d%%^ffcb4a e mais ^96f5ff%d^ffcb4a.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL3902 
#define __CPPGEN_GNET_SKILL3902 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill3902:public Skill 
    { 
    public: 
        enum { SKILL_ID = 3902 }; 
        Skill3902 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill3902Stub:public SkillStub 
    { 
    public: 
#define get_glyph(level, l1_4, l5_7, l8_9, lmax) level >= 0 && level <= 4 ? l1_4 : level >= 5 && level <= 7 ? l5_7 : level >= 8 && level <= 9 ? l8_9 : lmax; 

#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 300;
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
            
            float _glyph_value = get_glyph(skill->GetLevel (), 0.6, 0.8, 1.0, 1.2);
            int _glyph_value_2 = get_glyph(skill->GetLevel (), 2208, 3312, 4416, 5520);

            skill->GetPlayer ()->SetDecmp (112);
            skill->SetPlus (_glyph_value_2 + (2800 + skill->GetPlayer ()->GetCurweapon () == 1 ? 1864 : 0));
            //skill->SetRatio (_glyph_value + (skill->GetPlayer ()->GetCurweapon () == 5 ? 1.5 : skill->GetPlayer ()->GetCurweapon () == 1 ? 3.1 : 0));
            skill->SetRatio (skill->GetPlayer ()->GetCurweapon () == 5 ? (1.5 + _glyph_value) : skill->GetPlayer ()->GetCurweapon () == 1 ? (3.1 + _glyph_value) : _glyph_value);
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
    Skill3902Stub ():SkillStub (3902) 
    { 
        cls                 = 0; 
#ifdef _SKILL_CLIENT 
        name                = L"�񡤶���ն"; 
        nativename          = "�񡤶���ն"; 
        icon                = "�񡤶���ն���.dds"; 
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
        showorder           = 1106; 
        allow_forms         = 1; 
        apcost              = 0; 
        apgain              = 10; 
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
        effect              = "1����ն.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 0; 
        has_stateattack     = 0; 
        runes_attr          = 2; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = "ֱ���˺�����1.gfx"; 
        gfxhangpoint        = "HH_bind"; 
#endif 
        gfxtarget           = 0; 
        feature             = 1; 
        extra_effect        = 0; 
        immune_casting      = 0; 
        restrict_weapons.push_back (0); 
        restrict_weapons.push_back (1); 
        restrict_weapons.push_back (182); 
        restrict_weapons.push_back (5); 
        restrict_weapons.push_back (292); 
        restrict_weapons.push_back (9); 
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
        static int aarray[10] = { 3000,3000,3000,3000,3000,3000,3000,3000,3000,3000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 1500000,1500000,1500000,1500000,1500000,1500000,1500000,1500000,1500000,1500000 }; 
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
        static int aarray[10] = { 1500000,1500000,1500000,1500000,1500000,1500000,1500000,1500000,1500000,1500000 }; 
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
        return (float) ( ( skill->GetPlayer ()->GetCurweapon () == 5 ? (16 + skill->GetPlayer ()->GetCurweaponRange ()) : skill->GetPlayer ()->GetCurweaponRange ()));
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
    int GetIntroduction (Skill * skill, const wchar_t * buffer, int length, const wchar_t * format) const 
    { 
        return 0; 
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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPerformerCurWeapon () == 9 ? 60 : 0));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetRatio (0.6);
            skill->GetVictim ()->SetSlow (1);

            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPerformerCurWeapon () == 9 ? 20 : 0));
            skill->GetVictim ()->SetTime (3500);
            skill->GetVictim ()->SetFix (1);
            //skill->GetVictim ()->SetShowicon (1);
            //skill->GetVictim ()->SetGeloAtiradora (1);

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
            return (float) (14.9 + ( skill->GetPlayer ()->GetCurweapon () == 5 ? (skill->GetPlayer()->GetRange () + 16 + skill->GetPlayer ()->GetCurweaponRange ()) : (skill->GetPlayer()->GetRange () + skill->GetPlayer ()->GetCurweaponRange ())));
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
            return 6;
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
            return (float) (2.5);
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
