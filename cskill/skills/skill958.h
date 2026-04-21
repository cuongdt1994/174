//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
39580  "Î¨LÃ¢mina de Cascalho"39581  "^ff5c00Î¨LÃ¢mina de Cascalho^ffcb4aAlcance ^96f5ff28^ffffff ^ffcb4ametros^ffcb4aCusto em MP ^96f5ff75^ffffff^ffcb4aCanalizaÃ§Ã£o ^ffffff0,3 ^ffcb4asegundo^ffcb4aConjuraÃ§Ã£o ^ffffff0,7 ^ffcb4asegundo^ffcb4aEspera ^ffffff3,0 ^ffcb4asegundos^ffcb4aChi Recebido ^ffffff20^ffcb4aArma NecessÃ¡ria: ^ffffffEspada^ffcb4aBÃ´nus de Runa ^ff5c00Maestria Marcial^ffcb4aCultivo NecessÃ¡rio ^ffffffAlma CaÃ³ticaDesfira um golpe devastador no inimigo, causando ^96f5ff110%%^ffffff de dano fÃ­sico bÃ¡sico mais ^96f5ff100%%^ffffff de dano de arma mais ^96f5ff1650^ffffff.Tem ^96f5ff100%%^ffffff de chance de aplicar o efeito Î¨EspÃ­rito Arruinado,fazendo com que seu ^96f5ffÎ¨Ataque Geminiano^ffffff cause adicional de^96f5ff7950 ^ffffffem Dano de Metal e reduza o nÃ­vel de Ataquee Defesa do alvo em ^96f5ff12^ffffff.^00ff00Efeito da Runa Verdejante:@1^00ff00NÃ­vel 1 a 4: O EspÃ­rito Arruinado dura ^96f5ff60 ^00ff00segundos.^00ff00NÃ­vel 5 a 7: O EspÃ­rito Arruinado dura ^96f5ff90 ^00ff00segundos.^00ff00NÃ­vel 8 a 9: O EspÃ­rito Arruinado dura ^96f5ff120 ^00ff00segundos.^00ff00NÃ­vel 10: O EspÃ­rito Arruinado dura ^96f5ff150 ^00ff00segundos.@1@2^00ff00O EspÃ­rito Arruinado dura ^96f5ff%d ^00ff00segundos.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL958 
#define __CPPGEN_GNET_SKILL958 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill958:public Skill 
    { 
    public: 
        enum { SKILL_ID = 958 }; 
        Skill958 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill958Stub:public SkillStub 
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
                skill->GetPlayer ()->SetDecelfmp (70);
                skill->GetPlayer ()->SetDecelfap (500);
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
    Skill958Stub ():SkillStub (958) 
    { 
        cls                 = 258; 
#ifdef _SKILL_CLIENT 
        name                = L"ÖÎÓú"; 
        nativename          = "ÖÎÓú"; 
        icon                = "½â³ýÁ÷Ñª.dds"; 
#endif 
        max_level           = 10; 
        type                = 2; 
        allow_ride          = 0; 
        attr                = 7; 
        rank                = 0; 
        eventflag           = 0; 
        is_senior           = 0; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 0; 
        allow_forms         = 1; 
        apcost              = 500000; 
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
        effect              = "ÖÎÓú.sgc"; 
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
            return 30000;
        }
    int GetRequiredSp (Skill * skill) const 
    { 
        static int aarray[10] = { 60,240,560,1040,1680,2560,3720,5260,7300,9300 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetRequiredLevel (Skill * skill) const 
    { 
        static int aarray[10] = { 10005,10010,10015,10020,10025,10030,10035,10040,10045,10050 }; 
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
            return (float) (0);
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
            return _snwprintf (buffer, length, format, skill->GetLevel (), 40 + skill->GetLevel () * 2);

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
            skill->GetVictim ()->SetProbability (1.0 * 40 + 2 * skill->GetLevel () + skill->GetT0 () * 0.25 + skill->GetT1 () * 0.25);
            skill->GetVictim ()->SetClearbleeding (1);
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
            return (float) (40);
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
