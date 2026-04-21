//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
40290  "ΨAbsorver Alma"40291  "^ff5c00ΨAbsorver Alma

^ffcb4aAlcance: ^96f5ff27^ffffff ^ffcb4ametros
^ffcb4aCusto de MP: ^96f5ff300^ffffff
^ffcb4aCanalização: ^ffffff5,0 ^ffcb4asegundos
^ffcb4aConjuração: ^ffffff0,8 ^ffcb4asegundos
^ffcb4aEspera: ^ffffff2 ^ffcb4asegundos
^ffcb4aVitalidade: ^ffffff20
^ffcb4aArma: Necessária  ^ffffffDesarmado, Instrumento Mágico
^ffcb4aBônus de Runa: ^ff5c00Fúria do Tigre

^ffcb4aCultivo necessário: ^ffffffAlma Caótica
Infunde uma parte da sua própria alma em um poderoso ataque mágico.
Causa ^96f5ff125%%^ffffff de dano mágico básico mais ^96f5ff2900^ffffff em alvos jogadores.
Causa ^96f5ff200%%^ffffff de dano mágico básico mais ^96f5ff5800^ffffff em alvos que não sejam jogadores.
Não pode ser um ataque crítico, mas tem a chance de
dobrar sua Taxa de Acerto Crítico para causar ^96f5ff1,3^ffffff vezes mais dano.
Também interrompe a canalização do alvo.

^ffcb4aEfeito da Runa Áurea:@1
^ffcb4aNível 1 a 4: Causa ^96f5ff10%%^ffcb4a de dano mágico básico adicional em alvos jogadores.
^ffcb4aNível 5 a 7: Causa ^96f5ff15%%^ffcb4a de dano mágico básico adicional em alvos jogadores.
^ffcb4aNível 8 a 9: Causa ^96f5ff20%%^ffcb4a de dano mágico básico adicional em alvos jogadores.
^ffcb4aNível 10: Causa ^96f5ff25%%^ffcb4a de dano mágico básico adicional em alvos jogadores.@1@2
^ffcb4aCausa ^96f5ff%d%%^ffcb4a de dano mágico básico adicional em alvos jogadores.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL4029 
#define __CPPGEN_GNET_SKILL4029 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill4029:public Skill 
    { 
    public: 
        enum { SKILL_ID = 4029 }; 
        Skill4029 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill4029Stub:public SkillStub 
    { 
    public: 
#define get_glyph(level, l1_4, l5_7, l8_9, lmax) level >= 0 && level <= 4 ? l1_4 : level >= 5 && level <= 7 ? l5_7 : level >= 8 && level <= 9 ? l8_9 : lmax; 

#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 5000;
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
                skill->GetPlayer ()->SetDecmp (0.2 * 300);
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
                skill->GetPlayer ()->SetDecmp (0.8 * 300);
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
    Skill4029Stub ():SkillStub (4029) 
    { 
        cls                 = 9; 
#ifdef _SKILL_CLIENT 
        name                = L"�������"; 
        nativename          = "�������"; 
        icon                = "��������.dds"; 
#endif 
        max_level           = 1; 
        type                = 3; 
        allow_ride          = 0; 
        attr                = 3; 
        rank                = 40; 
        eventflag           = 0; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1422; 
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
        effect              = "����_��������.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 0; 
        has_stateattack     = 0; 
        runes_attr          = 3; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = "ֱ���˺�����1.gfx"; 
        gfxhangpoint        = "HH_bind"; 
#endif 
        gfxtarget           = 0; 
        feature             = 901; 
        extra_effect        = 0; 
        immune_casting      = 0; 
        restrict_weapons.push_back (292); 
        restrict_weapons.push_back (0); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
        statestub.push_back (new State2 ()); 
        statestub.push_back (new State3 ()); 
#endif 
    } 
    int GetExecutetime (Skill * skill) const 
    {
        static int array[10] = { 800,800,800,800,800,800,800,800,800,800 };
        return array[skill->GetLevel () - 1];
    }
    int GetCoolingtime (Skill * skill) const
        {
            return 2000;
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
            return (float) (3 + 0.4 * skill->GetLevel ());
        }
    float GetAttackdistance (Skill * skill) const
    {
            return (float) (4 + 0.3 * skill->GetLevel ());
        }
    float GetAngle (Skill * skill) const
    {
            return (float) (1 - 0.0111111 * (0));
        }
    float GetPraydistance (Skill * skill) const
    {
            return (float) (27);
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
            return 120;
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
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount ((skill->GetT2 () *
                                            (1 +
                                             0.01 * (skill->GetT1 () - skill->GetPlayer ()->GetDefenddegree () >
                                                     0 ? skill->GetT1 () - skill->GetPlayer ()->GetDefenddegree () : 0))) * 1.25);
            skill->GetVictim ()->SetValue ((skill->GetT0 () *
                                           (1 +
                                            0.01 * (skill->GetT1 () - skill->GetPlayer ()->GetDefenddegree () >
                                                    0 ? skill->GetT1 () - skill->GetPlayer ()->GetDefenddegree () : 0))) * 1.25 );
            skill->GetVictim ()->SetSpecialphysichurt (1);
            skill->GetVictim ()->SetProbability (1.0 * 80);
            skill->GetVictim ()->SetBreak (1);
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
            return (float) (36);
        }		
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent0 (PlayerWrapper * player) const
        {
            float _glyph_value = get_glyph(player->GetSkilllevel (4029), 0.1, 0.15, 0.2, 0.25);
            return (float) ((player->GetPuremagicattack () * (1.25 + _glyph_value) + 2900) * (player->GetRand () < player->GetCrit () * 3 ? 1.3 : 1));
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent1 (PlayerWrapper * player) const
        {
            return (float) (player->GetAttackdegree ());
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent2 (PlayerWrapper * player) const
        {
            return (float) ((player->GetPuremagicattack () * 2 + 5800) * (player->GetRand () < player->GetCrit () * 3 ? 1.3 : 1));
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
            return (float) (1);
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
