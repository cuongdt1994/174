//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
65040  "ΔSopro Pulsante"65041  "^ff0000ΔSopro Pulsante             Nível %d|if&dynskilllv()>0&^ffcb4a +%d&|

^ffcb4aRequer forma de Macaco de Neve
^ffcb4aMP ^96f5ff225^ffffff
^ffcb4aCanalização ^ffffffInstantânea
^ffcb4aEspera ^ffffff300 ^ffcb4asegundos
^ffcb4aArma ^ffffffBastão, Desarmado
^ffcb4aCusto do Item ^96f5ffPoeira Sedimentada^ffffff

^ffcb4aCultivo Necessário ^ffffffMahayana
Imediatamente remove efeitos negativos como Paralisia, Desarme, Silenciamento,
Congelamento, Atordoamento e Sono.
Habilita todas as habilidades de forma humana exceto Forma de Macaco da Neve, Conjurar Sombras e Rasante do Ganso.
Dura ^96f5ff|if&(ski()+())< 2& 10&20^ffffff | segundos.

>Nível |if&(skilllv()+dynskilllv()) 2&3: ^ffffffDurante o período, ganha o estado Yin e Yang a cada segundo.
Zera a espera de Punição Cadente, Abismo Celestial, União Cósmica e
Travessia Planar. Nível &^8080803: Durante o período, ganha o estado Yin e Yang a cada segundo.
Zera a espera de Punição Cadente, Abismo Celestial, União Cósmica e
Travessia Planar. |

^a6caf0| Usado quando você está sob efeitos de controle."*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL6504 
#define __CPPGEN_GNET_SKILL6504 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill6504:public Skill 
    { 
    public: 
        enum { SKILL_ID = 6504 }; 
        Skill6504 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill6504Stub:public SkillStub 
    { 
    public: 
#ifdef _SKILL_SERVER 
        class State1:public SkillStub::State
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
                skill->GetPlayer ()->SetDecmp (1 * 225);
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
    Skill6504Stub ():SkillStub (6504) 
    { 
        cls                 = 14; 
#ifdef _SKILL_CLIENT 
        name                = L"������һ��˫Ϣ"; 
        nativename          = "������һ��˫Ϣ"; 
        icon                = "����һ��˫Ϣ.dds"; 
#endif 
        max_level           = 2; 
        type                = 2; 
        allow_ride          = 0; 
        attr                = 1; 
        rank                = 8; 
        eventflag           = 0; 
        is_senior           = 0; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1501; 
        allow_forms         = 2; 
        apcost              = 0; 
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
        time_type           = 4; 
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
        effect              = ""; 
        aerial_effect       = ""; 
#endif 
        range.type          = 5; 
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
        restrict_weapons.push_back (69843); 
        restrict_weapons.push_back (0); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
#endif 
    } 
    int GetExecutetime (Skill * skill) const 
    {
        static int array[10] = { 900,900,900,900,900,900,900,900,900,900 };
        return array[skill->GetLevel () - 1];
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
        static int aarray[10] = { 70226,70227,70227,70227,70227,70227,70227,70227,70227,70227 }; 
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
            return (float) (16);
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
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetCleardebuff2 (1);
            
			skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetFeAtiradora (1);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (13000 * skill->GetLevel());
            skill->GetVictim ()->SetRatio (20);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetValue (6505);
            skill->GetVictim ()->SetAurabless (1);

            // Define Icon / Gfx Buff
            skill->GetVictim ()->SetTime(13000 * skill->GetLevel()); // Tempo
            skill->GetVictim ()->SetRatio (556); // Icon Macaco
            skill->GetVictim ()->SetAmount (0); // Buff
            skill->GetVictim ()->SetValue (1); // Definir
            skill->GetVictim ()->SetInsertvstate (1);

            skill->GetVictim ()->SetTime(13000 * skill->GetLevel()); // Tempo
            skill->GetVictim ()->SetRatio (558); // Icon
            skill->GetVictim ()->SetAmount (0); // Buff
            skill->GetVictim ()->SetValue (1); // Definir
            skill->GetVictim ()->SetInsertvstate (1);
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
