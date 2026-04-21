//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
39170  "ΨDragão Repentino"39171  "^ff5c00ΨDragão Repentino

^ffcb4aAlcance ^96f5ff%d^ffffff ^ffcb4ametros
^ffcb4aMP ^96f5ff%d^ffffff
^ffcb4aCanalização ^ffffff0,2 ^ffcb4asegundos
^ffcb4aConjuração ^ffffff0,5 ^ffcb4asegundo
^ffcb4aEspera ^ffffff5,0 ^ffcb4asegundos
^ffcb4aChi Recebido ^ffffff10
^ffcb4aArma ^ffffffArmas Corpo a Corpo
^ffcb4aBônus de Runa ^ff5c00Maestria Marcial

^ffcb4aCultivo Necessário ^ffffffAlma Caótica
Crie uma lâmina psiônica rápida para atacar o inimigo
de longe e causar dano físico básico, mais ^96f5ff6.747^ffffff.
Tem ^96f5ff66%%^ffffff de chance de imobilizar o alvo por ^96f5ff3^ffffff segundos.
O alcance é aumentado pelo ^96f5ffdobro^ffffff do alcance da arma.

^ff0000Efeito da Runa Escarlate:@1
^ff0000Custa ^96f5ff50%%^ff0000 do Chi atual. A precisão é aumentada em ^96f5ff10%%^ff0000 por ponto de Chi gasto.
^ff0000Nível 1 a 4: Para cada ^96f5ff100^ff0000 de Chi gasto, causa ^96f5ff125%%^ff0000 de dano físico básico.
^ff0000Nível 5 a 7: Para cada ^96f5ff100^ff0000 de Chi gasto, causa ^96f5ff150%%^ff0000 de dano físico básico.
^ff0000Nível 8 a 9: Para cada ^96f5ff100^ff0000 de Chi gasto, causa ^96f5ff175%%^ff0000 de dano físico básico.
^ff0000Nível 10: Para cada ^96f5ff100^ff0000 Chi gasto, causa ^96f5ff200%%^ff0000 de dano físico base.@1@2
^ff0000Custa ^96f5ff50%%^ff0000 do Chi atual. A precisão é aumentada em ^96f5ff10%%^ff0000 por ponto de Chi gasto.
Para cada ^96f5ff100^ff0000 Chi gasto, causa ^96f5ff%d%%^ff0000 de dano físico base.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL3917 
#define __CPPGEN_GNET_SKILL3917 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill3917:public Skill 
    { 
    public: 
        enum { SKILL_ID = 3917 }; 
        Skill3917 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill3917Stub:public SkillStub 
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
                skill->GetPlayer ()->SetDecmp (61);
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
                return 1200;
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
                float _glyph_value = get_glyph(skill->GetLevel (), 1.25, 1.5, 1.75, 2);
                int _get_ap = skill->GetPlayer()->GetAp();
                int _ap_result = _get_ap / 100;
                float _res = _ap_result * _glyph_value;
                            
                skill->GetPlayer ()->SetDecmp (100);
                skill->SetPlus (6747);
                skill->SetRatio (_res);
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
    Skill3917Stub ():SkillStub (3917) 
    { 
        cls                 = 0; 
#ifdef _SKILL_CLIENT 
        name                = L"������"; 
        nativename          = "������"; 
        icon                = "�����ֺ�.dds"; 
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
        showorder           = 1105; 
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
        effect              = "1����.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 0; 
        has_stateattack     = 0; 
        runes_attr          = 1; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = "����Ԫ������˺�.gfx"; 
        gfxhangpoint        = "HH_bind"; 
#endif 
        gfxtarget           = 0; 
        feature             = 0; 
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
        static int array[10] = {1200};
        return array[skill->GetLevel () - 1];
    }
    int GetCoolingtime (Skill * skill) const
        {
            return 5000;
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
            return (float) (1 - 0.0111111 * (0));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (12 + (skill->GetPlayer ()->GetCurweaponRange () * 2));
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
        return 0; 
    } 
#endif 
#ifdef _SKILL_SERVER 
        int GetEnmity (Skill * skill) const
        {
            return 10000;
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
            skill->GetVictim ()->SetProbability (1.0 * 66);
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetFix (1);			
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        bool BlessMe (Skill * skill) const
        {
            int _get_ap = (skill->GetPlayer()->GetAp() * 0.5);

            skill->GetVictim()->SetProbability(1.0 * 100);
            skill->GetVictim()->SetTime(3100);
            skill->GetVictim()->SetRatio(0.1 * _get_ap);
            skill->GetVictim()->SetIncaccuracy(1);

            skill->GetVictim()->SetProbability(1.0 * 100);
            skill->GetVictim()->SetValue(-_get_ap);
            skill->GetVictim()->SetAp(1);
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (19 + (skill->GetPlayer ()->GetCurweaponRange () * 2));
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
            return 16;
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
