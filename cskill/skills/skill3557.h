//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
35570  "ΨMar Sem-Fim"35571  "^ff5c00ΨMar Sem-Fim

^ff0000Requer a Dança Violeta.
^ffcb4aAlcance: ^96f5ff%d^ffffff ^ffcb4ametros
^ffcb4aCusto de MP: ^96f5ff%d^ffffff
^ffcb4aCanalização: ^ffffff2,5 ^ffcb4asegundos
^ffcb4aConjuração: ^ffffff1 ^ffcb4asegundo
^ffcb4aEspera: ^ffffff1 ^ffcb4asegundo
^ffcb4aVitalidade: ^ffffff20
^ffcb4aArma: Necessária  ^ffffffDesarmado, Instrumento Mágico
^ffcb4aBônus de Runa: ^ff5c00Maestria Marcial

^ffcb4aCultivo necessário: ^ffffffAlma Celestial
Invoca uma tempestade violenta que eletrocuta seu inimigo,
causando dano mágico básico como dano por Metal. Causa dano adicional
aos jogadores, baseado no seu HP máximo ou no HP
atual do alvo, o que for menor, penetrando a Defesa.
Causa uma porcentagem adicional de dano por arma em alvos que não sejam jogadores.

^ffeaabEfeito da Runa Argêntea:@1
^ffeaabNível 1 a 4: Causa dano aos alvos jogadores, equivalente a ^96f5ff2%%^ffeaab do menor valor de HP.
       Causa dano em alvos que não sejam jogadores, equivalente a ^96f5ff160%%^ffeaab do seu dano por arma.
^ffeaabNível 5 a 7: Causa dano aos alvos jogadores, equivalente a ^96f5ff3%%^ffeaab do menor valor de HP.
       Causa dano em alvos que não sejam jogadores, equivalente a ^96f5ff240%%^ffeaab do seu dano por arma.
^ffeaabNível 8 a 9: Causa dano aos alvos jogadores, equivalente a ^96f5ff4%%^ffeaab do menor valor de HP.
       Causa dano em alvos que não sejam jogadores, equivalente a ^96f5ff320%%^ffeaab do seu dano por arma.
^ffeaabNível 10: Causa dano aos alvos jogadores, equivalente a ^96f5ff5%%^ffeaab do menor valor de HP.
      Causa dano em alvos que não sejam jogadores, equivalente a ^96f5ff400%%^ffeaab do seu dano por arma.@1@2
Causa dano aos alvos jogadores, equivalente a ^96f5ff%d%%^ffeaab do menor valor de HP.
Causa dano em alvos que não sejam jogadores, equivalente a ^96f5ff%d%%^ffeaab do seu dano por arma.@2





"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL3557 
#define __CPPGEN_GNET_SKILL3557 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill3557:public Skill 
    { 
    public: 
        enum { SKILL_ID = 3557 }; 
        Skill3557 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill3557Stub:public SkillStub 
    { 
    public: 
#define get_glyph(level, l1_4, l5_7, l8_9, lmax) level >= 0 && level <= 4 ? l1_4 : level >= 5 && level <= 7 ? l5_7 : level >= 8 && level <= 9 ? l8_9 : lmax; 

#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 2500;
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
                skill->GetPlayer ()->SetDecmp (250);
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
                skill->GetPlayer ()->SetDecmp (750);
                skill->SetGolddamage (skill->GetMagicattack ());
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
    Skill3557Stub ():SkillStub (3557) 
    { 
        cls                 = 7; 
#ifdef _SKILL_CLIENT 
        name                = L"�񡤺��ɰٴ�"; 
        nativename          = "�񡤺��ɰٴ�"; 
        icon                = "�񡤺��ɰٴ���.dds"; 
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
        showorder           = 1407; 
        allow_forms         = 2; 
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
        effect              = "���ɰٴ�_��_����.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 0; 
        has_stateattack     = 0; 
        runes_attr          = 1; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = ""; 
        gfxhangpoint        = "0"; 
#endif 
        gfxtarget           = 0; 
        feature             = 0; 
        extra_effect        = 0; 
        immune_casting      = 0; 
        restrict_weapons.push_back (0); 
        restrict_weapons.push_back (292); 
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
            return 1000;
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
            return (float) (10 + 0.5 * skill->GetLevel ());
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
    int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif 
#ifdef _SKILL_SERVER 
    int GetEnmity (Skill * skill) const
        {
            return 2700;
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
            int target_max_hp = skill->GetTarget ()->GetHp ();
            int performer_max_hp = skill->GetPlayer ()->GetHp ();

            float _glyph_value_player = get_glyph(skill->GetLevel (), 0.02, 0.03, 0.04, 0.05);
            float _glyph_value_monster = get_glyph(skill->GetLevel (), 1.6, 2.4, 3.2, 4);

            skill->GetVictim ()->SetValue (skill->GetTarget ()->GetCls () != -1 ? ((target_max_hp < performer_max_hp ? target_max_hp : performer_max_hp) * _glyph_value_player) : skill->GetPlayer ()->GetWeapondamage () * _glyph_value_monster);
            skill->GetVictim ()->SetGoldhurt (1);

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
            return (float) (35);
        }		
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent0 (PlayerWrapper * player) const
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
