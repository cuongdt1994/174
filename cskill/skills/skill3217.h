//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
32170  "ΔConjuração Suprema"
32171  "^ff0000ΔConjuração Suprema     Nível %d|if&dynskilllv()>0&^ffcb4a +%d&|

^ffcb4aAlcance ^ffffff28 metros
^ffcb4aMP ^96f5ff450^ffffff
^ffcb4aCanalização ^ffffff1 ^ffcb4asegundos    
^ffcb4aConjuração ^ffffff4 ^ffcb4asegundos    
^ffcb4aEspera ^ffffff300 ^ffcb4asegundos    
^ffcb4aArma ^ffffffDesarmado, Instrumentos Mágicos
^ffcb4aItem ^96f5ffPoeira Sedimentada^ffffff

^ffcb4aCultivo Necessário ^ffffffMahayana
Detone seu alvo ^96f5ff5^ffffff vezes, causando ^96f5ff2x^ffffff dano mágico básico,
 mais ^96f5ff850%%^ffffff de dano de arma e mais ^96f5ff8242^ffffff de dano de Madeira.
Imune a todos os status negativos ao conjurar.

No ^a6caf0Nível 2^ffffff, invoca várias mascotes:
O primeiro ataque: invoca a Fênix Iluminada, atacando todos os inimigos em até ^96f5ff12^ffffff metros
 e imobilizando-os por ^96f5ff5^ffffff segundos;
O segundo ataque: invoca o Gigante Celestial, atacando todos os inimigos em até ^96f5ff12^ffffff metros
 e aumentando o dano recebido em ^96f5ff30%%^ffffff por ^96f5ff6^ffffff segundos;
O terceiro ataque: invoca o Abutre Empíreo, atacando todos os inimigos em até ^96f5ff12^ffffff metros
 e removendo ^96f5ff1 Chi^ffffff.
O quarto ataque: invoca o Avatar da Guerra, atacando todos os inimigos em até ^96f5ff12^ffffff metros
 e reduzindo Resistência Mágica em ^96f5ff100%%^ffffff por ^96f5ff6^ffffff segundos.
O quinto ataque: invoca o Bot de Batalha, atacando todos os inimigos em até ^96f5ff12^ffffff metros
Remove todos os status Positivos em cada ataque realizado desta habilidade.

^a6caf0As mascotes invocadas por habilidades são todas feras guardiãs Selvagens.
Podem ser invocadas sem que os tenha.

|if&(skilllv()+dynskilllv())>Nível 2&3: ^ffffffinvoca várias mascotes:^ffffff
O primeiro ataque: invoca a Fênix Iluminada, atacando todos os inimigos em até ^96f5ff12^ffffff metros,
imobilizando-os por^96f5ff5^ffffff segundos e removendo todos os status Positivos.
O segundo ataque: invoca o Gigante Celestial, atacando todos os inimigos em até ^96f5ff12^ffffff metros,
aumentando o dano recebido em ^96f5ff30%%^ffffff por ^96f5ff6^ffffff segundos
e removendo todos os status Positivos.
O terceiro ataque: invoca o Abutre Empíreo, atacando todos os inimigos
em até ^96f5ff12^ffffff metros, removendo 1 Chi 
e removendo todos os status Positivos.
O quarto ataque: invoca o Avatar da Guerra, atacando todos os inimigos em até ^96f5ff12^ffffff metros,
reduzindo Resistência Mágica em ^96f5ff100%%^ffffff por ^96f5ff6^ffffff segundos
e removendo todos os status Positivos.
O quinto ataque: invoca o Bot de Batalha, atacando todos os inimigos em até ^96f5ff12^ffffff metros,
removendo todos os status Positivos e paralisando-os por ^96f5ff3^ffffff segundos.
&^808080Nível 3: invoca várias mascotes:
O primeiro ataque: invoca a Fênix Iluminada, atacando todos os inimigos em até 12 metros,
imobilizando-os por 5 segundos e removendo todos os status Positivos.
O segundo ataque: invoca o Gigante Celestial, atacando todos os inimigos em até 12 metros,
aumentando o dano recebido em 30% por 6 segundos
e removendo todos os status Positivos.
O terceiro ataque: invoca o Abutre Empíreo, atacando todos os inimigos
em até 12 metros, removendo 1 Chi
e removendo todos os status Positivos.
O quarto ataque: invoca o Avatar da Guerra, atacando todos os inimigos em até 12 metros, 
reduzindo Resistência Mágica em 100% por 6 segundos 
e removendo todos os status Positivos.
O quinto ataque: invoca o Bot de Batalha, atacando todos os inimigos em até 12 metros,
removendo todos os status Positivos e paralisando-os por 3 segundos.
"
*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL3217 
#define __CPPGEN_GNET_SKILL3217 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill3217:public Skill 
    { 
    public: 
        enum { SKILL_ID = 3217 }; 
        Skill3217 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill3217Stub:public SkillStub 
    { 
    public: 
#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
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
                return 50;
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
                skill->GetPlayer ()->SetPerform (2);
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
                skill->SetSection(1);
                skill->GetPlayer ()->SetDecmp (450);
                skill->SetPlus(8242);
                skill->SetRatio(8.5);
                skill->SetWooddamage(skill->GetMagicattack() * 0.2);
                skill->GetPlayer ()->SetPerform (0);
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
        class State4:public SkillStub::State
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
                skill->SetSection(2);
                skill->SetPlus(8242);
                skill->SetRatio(8.5);
                skill->SetWooddamage(skill->GetMagicattack() * 0.2);
                skill->GetPlayer ()->SetPerform (0);
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
        class State5:public SkillStub::State
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
                skill->SetSection(3);
                skill->SetPlus(8242);
                skill->SetRatio(8.5);
                skill->SetWooddamage(skill->GetMagicattack() * 0.2);
                skill->GetPlayer ()->SetPerform (0);
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
        class State6:public SkillStub::State
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
                skill->SetSection(4);
                skill->SetPlus(8242);
                skill->SetRatio(8.5);
                skill->SetWooddamage(skill->GetMagicattack() * 0.2);
                skill->GetPlayer ()->SetPerform (0);
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
        class State7:public SkillStub::State
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
                skill->SetSection(5);
                skill->SetPlus(8242);
                skill->SetRatio(8.5);
                skill->SetWooddamage(skill->GetMagicattack() * 0.2);
                skill->GetPlayer ()->SetPerform (0);
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
        class State8:public SkillStub::State
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
    Skill3217Stub ():SkillStub (3217) 
    { 
        cls                 = 3; 
#ifdef _SKILL_CLIENT 
        name                = L"������������"; 
        nativename          = "������������"; 
        icon                = "����������.dds"; 
#endif 
        max_level           = 2; 
        type                = 1; 
        allow_ride          = 0; 
        attr                = 3; 
        rank                = 8; 
        eventflag           = 0; 
        is_senior           = 0; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1125; 
        allow_forms         = 1; 
        apcost              = 0; 
        apgain              = 0; 
        doenchant           = 0; 
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
        time_type           = 2; 
        long_range          = 0; 
        posdouble           = 0; 
        clslimit            = 0; 
        commoncooldown      = 0; 
        commoncooldowntime  = 0; 
        itemcost            = 49982; 
        itemcostcount       = 1; 
        combosk_preskill    = 0; 
        combosk_interval    = 0; 
        combosk_nobreak     = 0; 
#ifdef _SKILL_CLIENT 
        effect              = "0"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 0; 
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
        pre_skills.push_back (std::pair < ID, int >(0, 1)); 
        restrict_weapons.push_back (0); 
        restrict_weapons.push_back (292); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
        statestub.push_back (new State2 ()); 
        statestub.push_back (new State3 ()); 
        statestub.push_back (new State4 ()); 
        statestub.push_back (new State5 ()); 
        statestub.push_back (new State6 ()); 
        statestub.push_back (new State7 ()); 
        statestub.push_back (new State8 ()); 
#endif 
    } 
        int GetExecutetime (Skill * skill) const
        {
            return -1;
        }
    int GetCoolingtime (Skill * skill) const 
    { 
        static int aarray[10] = { 300000,300000,300000,300000,300000,300000,300000,300000,300000,300000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 1000000,2000000,2000000,2000000,2000000,2000000,2000000,2000000,2000000,2000000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 100,100,100,100,100,100,100,100,100,100 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredItem (Skill * skill) const 
    { 
        static int aarray[10] = { 49922,49923,49923,49923,49923,49923,49923,49923,49923,49923 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredMoney (Skill * skill) const 
    { 
        static int aarray[10] = { 1000000,2000000,2000000,2000000,2000000,2000000,2000000,2000000,2000000,2000000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredRealmLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
        float GetRadius (Skill * skill) const
        {
            return (float) (12);
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (0);
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1);
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
            
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetSection () == 1 ? 100 : 0));
            skill->GetVictim ()->SetTime (5000);
            skill->GetVictim ()->SetDizzy (1);
            
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetSection () == 2 ? 100 : 0));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetInchurt (1);

            skill->GetVictim ()->SetProbability (1.0 * (skill->GetSection () == 3 ? 100 : 0));
            skill->GetVictim ()->SetValue (-100);
            skill->GetVictim ()->SetAp (1);

            skill->GetVictim ()->SetProbability (1.0 * (skill->GetSection () == 4 ? 100 : 0));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetDecresist (1);

            skill->GetVictim ()->SetProbability (1.0 * (skill->GetSection () == 5 ? 100 : 0));
            skill->GetVictim ()->SetClearbuff (1);
            
            return true;
        }
#endif 
#ifdef _SKILL_SERVER 
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetValue( skill->GetSection() + 49658);
            skill->GetVictim ()->SetSummonplantpet(1);

            skill->GetVictim ()->SetTime (skill->GetSection () == 1 ? 6100 : 0);
            skill->GetVictim ()->SetFeAtiradora (1);
           
            return true;
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
