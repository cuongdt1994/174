//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
39950  "Î¨Dardo da Nuvem Negra"39951  "^ff5c00Î¨Dardo da Nuvem Negra^ffcb4aAlcance ^ffffff28^ffcb4a  metros^ffcb4aCusto em MP ^96f5ff115^ffffff^ffcb4aCanalizaÃ§Ã£o ^ffffff0,3 ^ffcb4asegundo^ffcb4aConjuraÃ§Ã£o ^ffffff0,7 ^ffcb4asegundo^ffcb4aEspera ^ffffff6,0  ^ffcb4asegundos^ffcb4aChi Recebido ^ffffff10^ffcb4aArma necessÃ¡ria: ^ffffffEspada^ffcb4aBÃ´nus de Runa ^ff5c00Poder do DragÃ£o^ffcb4aCultivo NecessÃ¡rio ^ffffffAlma CaÃ³ticaInvoca uma tempestade sÃºbita para atacar todos os inimigos a atÃ© ^96f5ff12 ^ffffff metros ao redor do alvo..Causa ^96f5ff110%% ^ffffff de dano fÃ­sico bÃ¡sico mais ^96f5ff100%% ^ffffffdo dano da arma mais ^96f5ff2700^ffffff. Tem ^96f5ff50%% ^ffffffde chance de interromper a canalizaÃ§Ã£o do alvo.Congela o alvo com o status de Olho do CÃ©u do Norte, causando dano adicional e reduzindo o NÃ­vel de Defesa.^00ffffEfeito da Runa Celeste:@1^00ffffNÃ­vel 1 a 4: O alcance aumenta em ^ffffff2^00ffff metros.^00ffffNÃ­vel 5 a 7: O alcance aumenta em ^ffffff3^00ffff metros.^00ffffNÃ­vel 8 a 9: O alcance aumenta em ^ffffff4^00ffff metros.^00ffffNÃ­vel 10: O alcance aumenta em ^ffffff5 ^00ffffmetros.@1@2^00ffffO alcance aumenta em ^ffffff%d^00ffff metros.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL995 
#define __CPPGEN_GNET_SKILL995 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill995:public Skill 
    { 
    public: 
        enum { SKILL_ID = 995 }; 
        Skill995 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill995Stub:public SkillStub 
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
                skill->GetPlayer ()->SetDecelfmp (30 + (skill->GetLevel () - 1) * 3);
                skill->GetPlayer ()->SetDecelfap (90 + 32 * (skill->GetLevel () - 1));
                skill->SetFiredamage ((25 + (skill->GetLevel () - 1) * 30 + skill->GetT0 () + skill->GetT1 ()) * 3.1);
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
    Skill995Stub ():SkillStub (995) 
    { 
        cls                 = 258; 
#ifdef _SKILL_CLIENT 
        name                = L"·ÙÐÄÖä"; 
        nativename          = "·ÙÐÄÖä"; 
        icon                = "·ÙÐÄÖä.dds"; 
#endif 
        max_level           = 10; 
        type                = 1; 
        allow_ride          = 0; 
        attr                = 5; 
        rank                = 0; 
        eventflag           = 0; 
        is_senior           = 0; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 0; 
        allow_forms         = 1; 
        apcost              = 90032; 
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
        time_type           = 1; 
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
        effect              = "·ÙÐÄÖä.sgc"; 
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
        pre_skills.push_back (std::pair < ID, int >(0, 0)); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
#endif 
    } 
        int GetExecutetime (Skill * skill) const
        {
            return 0;
        }
    int GetCoolingtime (Skill * skill) const
        {
            return 10000;
        }
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 60,240,560,1040,1680,2560,3720,5260,7300,10000 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 2005,2010,2015,2020,2025,2030,2035,2040,2045,2050 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredItem (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredMoney (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
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
            return (float) (18);
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
            return _snwprintf (buffer, length, format,
                               skill->GetLevel (),
                               30 + (skill->GetLevel () - 1) * 3,
                               90 + 32 * (skill->GetLevel () - 1), (25 + (skill->GetLevel () - 1) * 30) * 3.1, skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (9000 + (skill->GetT0 () + skill->GetT1 ()) * 50);
            skill->GetVictim ()->SetRatio (skill->GetLevel () * 0.01 + skill->GetT0 () * 0.0005 + skill->GetT1 () * 0.0005);
            skill->GetVictim ()->SetSlowattackpray (1);
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
            return (float) (20);
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent0 (PlayerWrapper * player) const
        {
            return (float) (player->GetElfstr ());
        }
#endif 
#ifdef _SKILL_SERVER 
        float GetTalent1 (PlayerWrapper * player) const
        {
            return (float) (player->GetElfagi ());
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
            return 20;
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
