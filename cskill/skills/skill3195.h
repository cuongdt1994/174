//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
31950  "ΩRugido do Leão"31951  "^ffff00ΩRugido do Leão

^ffcb4aAlcance ^ffffff5 metros
^ffcb4aMP ^96f5ff%d^ffffff
^ffcb4aCanalização ^ffffff0,6 ^ffcb4asegundos
^ffcb4aConjuração ^ffffff1,2 ^ffcb4asegundos
^ffcb4aEspera ^96f5ff%d^ffffff ^ffcb4asegundos
^ffcb4aArma ^ffffffArmas Corpo a Corpo

^ffcb4aCultivo Necessário ^ffffffNobre
Um rugido furioso atordoa todos os inimigos em até ^96f5ff10^ffffff metros por
^96f5ff6^ffffff segundos. ^96f5ffA versão do Sábio Primordial tem a maior taxa de sucesso.^ffffff
Também desarma todos os inimigos a até ^96f5ff10^ffffff metros por ^96f5ff6^ffffff segundos.

Armas de Haste, Piques e Clavas: Aumenta o alcance da conjuração para ^96f5ff20^ffffff metros 
e causa dano a um único alvo, 
atordoando e desarmando por ^96f5ff5^ffffff segundos.
Lâminas e Espadas: Não atordoa nem desarma os inimigos. 
Reduz o tempo de Canalização para ^96f5ff0,1^ffffff segundo e o tempo de Conjuração para ^96f5ff1^ffffff segundo. 
Os inimigos em até ^96f5ff10^ffffff metros sofrem ^96f5ff200%%^ffffff de dano físico básico.
Não pode ser um acerto crítico, mas tem a chance de
dobrar a sua Taxa de Acerto Crítico e causar ^96f5ff1,5^ffffff vezes de dano.

^a6caf0Custa ^ffffff35^a6caf0 Chi.

^d618e7A versão Demônio reduz a espera em ^96f5ff1^d618e7 segundo e sempre é bem-sucedida."*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL3195 
#define __CPPGEN_GNET_SKILL3195 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill3195:public Skill 
    { 
    public: 
        enum { SKILL_ID = 3195 }; 
        Skill3195 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill3195Stub:public SkillStub 
    { 
    public: 
#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return skill->GetPlayer ()->GetCurWeapon() != 1 ? 600 : 100;
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
                skill->GetPlayer ()->SetDecmp (35);
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
                return skill->GetPlayer ()->GetCurWeapon() != 1 ? 1200 : 1000;
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
                skill->GetPlayer ()->SetDecmp (140);
                skill->GetPlayer()->SetPerform(1);
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
    Skill3195Stub ():SkillStub (3195) 
    { 
        cls                 = 0; 
#ifdef _SKILL_CLIENT 
        name                = L"�ġ�ʨ�Ӻ�"; 
        nativename          = "�ġ�ʨ�Ӻ�"; 
        icon                = "ʨ�Ӻ�2.dds"; 
#endif 
        max_level           = 1; 
        type                = 3; 
        allow_ride          = 0; 
        attr                = 1; 
        rank                = 30; 
        eventflag           = 0; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1126; 
        allow_forms         = 1; 
        apcost              = 35; 
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
        effect              = "2ʨ�Ӻ�.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 2; 
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
        pre_skills.push_back (std::pair < ID, int >(425, 1)); 
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
        return skill->GetPlayer ()->GetCurWeapon() != 1 ? 1200 : 1000;
    }
    int GetCoolingtime (Skill * skill) const 
    { 
        static int aarray[10] = { 14000,14000,14000,14000,14000,14000,14000,14000,14000,14000 }; 
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
        static int aarray[10] = { 49897,49897,49897,49897,49897,49897,49897,49897,49897,49897 }; 
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
            return (float) (10);
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetCurWeapon() != 5 ? 5 : 20);
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (0));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetCurWeapon() != 5 ? 5 : 20);
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
            return _snwprintf (buffer, length, format, 175, 15, 6);

        }
#endif 
#ifdef _SKILL_SERVER 
        int GetEnmity (Skill * skill) const
        {
            return 6000;
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
 
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPerformerCurWeapon () == 1 ? 100 : 0));
            skill->GetVictim ()->SetValue (10000 + (skill->GetPerformerCurWeapon () == 1 ? 2 : 1) * skill->GetPerformerCurWeapon() + skill->GetT0() );
            skill->GetVictim ()->SetPhysichurtRuge (1);

            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPerformerCurWeapon () == 1 ? 0 : 100));
            skill->GetVictim ()->SetTime (skill->GetPerformerCurWeapon () == 5 ? 5000 : 7000);
            skill->GetVictim ()->SetDizzy (1);
			
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPerformerCurWeapon () == 1 ? 0 : 100));
            skill->GetVictim ()->SetTime (skill->GetPerformerCurWeapon () == 5 ? 5000 : 7000);
            skill->GetVictim ()->SetWeapondisabled (1);

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
            return (float) (50);
        }
#endif 
#ifdef _SKILL_SERVER 
         float GetTalent0 (PlayerWrapper * player) const
        { //espada
            return (float) ((player->GetRand () < player->GetCrit () * 2 ? 1.5 : 1) * 1000);
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent1 (PlayerWrapper * player) const
        {
            return (float) ((player->GetAttackdegree () * 0.011 + 1.1) * (player->GetRand () < player->GetCrit () * 2 ? 1.5 : 1) * 1000);
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent2 (PlayerWrapper * player) const
        {
            int res = 0;
            res = player->GetCurWeapon();
            return res;
        }
#endif 
#ifdef _SKILL_SERVER 
    float GetTalent3 (PlayerWrapper * player) 
    { 
        return (float) (player->GetTargetregionplayernum ()); 
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
            return 9;
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
