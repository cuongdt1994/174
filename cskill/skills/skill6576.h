//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
65760  "ΨPunição Cadente"65761  "^ff5c00ΨPunição Cadente              

^ffcb4aAlcance ^ffffffCorpo-a-corpo
^ffcb4aCusto em MP ^96f5ff%.1f^ffffff    
^ffcb4aCanalização ^ffffff0,3 ^ffcb4asegundos    
^ffcb4aConjuração ^ffffff1,3 ^ffcb4asegundos    
^ffcb4aEspera ^ffffff45,0 ^ffcb4asegundos    
^ffcb4aArma ^ffffffClavas

^ffcb4aCultivo  ^ffffffAlma Caótica
Dê um grande salto e esmague o chão a até ^96f5ff10^ffffff metros,causando dano físico
básico mais ^96f5ff250%%^ffffff do dano da arma mais ^96f5ff%d^ffffff
aos inimigos. 
Se você tiver a Marca de Yin ou de Yang, a marca será removida e a velocidade de movimento dos inimigos
limitada a ^96f5ff0^ffffff m/s e eles não poderão usar habilidades de Locomoção. Dura ^96f5ff10^ffffff segundo.

^a6caf0A habilidade só faz efeito em jogadores.

^a6caf0Custa ^ffffff1 ^a6caf0Chi.

^f7dfa5Este ataque tem a garantia de acertar.

^00ff00Efeito da Runa Verdejante:@1
^00ff00Nível 1 a 4: O alcance aumenta em ^96f5ff1^00ff00 metros.
^00ff00Nível 5 a 7: O alcance aumenta em ^96f5ff2^00ff00 metros.
^00ff00Nível 8 a 9: O alcance aumenta em ^96f5ff3^00ff00 metros.
^00ff00Nível 3: O alcance aumenta em ^96f5ff4^00ff00 metros.@1@2
^00ff00O alcance aumenta em ^96f5ff%d ^00ff00metros.@2"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL6576 
#define __CPPGEN_GNET_SKILL6576 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill6576:public Skill 
    { 
    public: 
        enum { SKILL_ID = 6576 }; 
        Skill6576 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill6576Stub:public SkillStub 
    { 
    public: 
#define get_glyph(level, l1_4, l5_7, l8_9, lmax) level >= 0 && level <= 4 ? l1_4 : level >= 5 && level <= 7 ? l5_7 : level >= 8 && level <= 9 ? l8_9 : lmax; 

#ifdef _SKILL_SERVER 
        class State1 : public SkillStub::State
        {
        public:
            int GetTime(Skill *skill) const
            {
                return 300;
            }
            bool Quit(Skill *skill) const
            {
                return false;
            }
            bool Loop(Skill *skill) const
            {
                return false;
            }
            bool Bypass(Skill *skill) const
            {
                return false;
            }
            void Calculate(Skill *skill) const
            {
                skill->GetPlayer()->SetDecmp(0.2 * ((9 * skill->GetLevel()) + 191.3));
                skill->GetPlayer()->SetPray(1);
            }
            bool Interrupt(Skill *skill) const
            {
                return false;
            }
            bool Cancel(Skill *skill) const
            {
                return 1;
            }
            bool Skip(Skill *skill) const
            {
                return 0;
            }
        };
#endif 
#ifdef _SKILL_SERVER 
        class State2 : public SkillStub::State
        {
        public:
            int GetTime(Skill *skill) const
            {
                return 1300;
            }
            bool Quit(Skill *skill) const
            {
                return false;
            }
            bool Loop(Skill *skill) const
            {
                return false;
            }
            bool Bypass(Skill *skill) const
            {
                return false;
            }
            void Calculate(Skill *skill) const
            {
                skill->GetPlayer()->SetDecmp(0.8 * ((9 * skill->GetLevel()) + 191.3));
                skill->SetPlus(9245);
                skill->SetRatio(2.5);
                skill->SetDamage(skill->GetAttack());
                skill->GetPlayer()->SetPerform(1);
            }
            bool Interrupt(Skill *skill) const
            {
                return false;
            }
            bool Cancel(Skill *skill) const
            {
                return 0;
            }
            bool Skip(Skill *skill) const
            {
                return 0;
            }
        };
#endif 
#ifdef _SKILL_SERVER 
        class State3 : public SkillStub::State
        {
        public:
            int GetTime(Skill *skill) const
            {
                return 0;
            }
            bool Quit(Skill *skill) const
            {
                return false;
            }
            bool Loop(Skill *skill) const
            {
                return false;
            }
            bool Bypass(Skill *skill) const
            {
                return false;
            }
            void Calculate(Skill *skill) const
            {
            }
            bool Interrupt(Skill *skill) const
            {
                return false;
            }
            bool Cancel(Skill *skill) const
            {
                return 0;
            }
            bool Skip(Skill *skill) const
            {
                return 0;
            }
        };
#endif 
    Skill6576Stub ():SkillStub (6576) 
    { 
        cls                 = 14; 
#ifdef _SKILL_CLIENT 
        name                = L"��׹�ǻ���"; 
        nativename          = "��׹�ǻ���"; 
        icon                = "��׹�ǻ�����.dds"; 
#endif 
        max_level           = 1; 
        type                = 1; 
        allow_ride          = 0; 
        attr                = 7; 
        rank                = 40; 
        eventflag           = 0; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1501; 
        allow_forms         = 1; 
        apcost              = 100; 
        apgain              = 0; 
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
        effect              = ""; 
        aerial_effect       = ""; 
#endif 
        range.type          = 2; 
        has_stateattack     = 0; 
        runes_attr          = 2; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = ""; 
        gfxhangpoint        = "0"; 
#endif 
        gfxtarget           = 0; 
        feature             = 1401; 
        extra_effect        = 0; 
        immune_casting      = 0; 
        pre_skills.push_back (std::pair < ID, int >(6489, 1)); 
        restrict_weapons.push_back (69843); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
        statestub.push_back (new State2 ()); 
        statestub.push_back (new State3 ()); 
#endif 
    } 
    int GetExecutetime (Skill * skill) const 
    {
        static int array[10] = { 1300,1300,1300,1300,1300,1300,1300,1300,1300,1300 };
        return array[skill->GetLevel () - 1];
    }
    int GetCoolingtime (Skill * skill) const 
    { 
        static int aarray[10] = { 45000,45000,45000,45000,45000,45000,45000,45000,45000,45000 }; 
        return aarray[skill->GetLevel () - 1]; 
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
    float GetRadius(Skill *skill) const
    {
        return (float)(10);
    }
    float GetAttackdistance(Skill *skill) const
    {
        return (float)(0);
    }
    float GetAngle(Skill *skill) const
    {
        return (float)(1 - 0.0111111 * (0));
    }
    float GetPraydistance(Skill *skill) const
    {
        int _glyph_value = get_glyph((skill->GetLevel ()), 1, 2, 3, 4);

        return (float)(skill->GetPlayer()->GetRange() + _glyph_value);
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
    int GetIntroduction(Skill *skill, wchar_t *buffer, int length, wchar_t *format) const
    {
            return _snwprintf(buffer, length, format);
    }
#endif 
#ifdef _SKILL_SERVER 
    int GetEnmity(Skill *skill) const
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
            skill->GetVictim ()->SetProbability (1.0 * skill->GetPlayer ()->GetCls () == -1 ? 0 : skill->GetT0 () && skill->GetT1 () ? 100 : 0);
            skill->GetVictim ()->SetTime (11000);
            skill->GetVictim ()->SetShowicon (0);
            skill->GetVictim ()->SetComboYinYang2 (1);

        return true;
    }  
#endif 
#ifdef _SKILL_SERVER 
    bool BlessMe (Skill * skill) const
    {
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetRemoveFilterMonkey (1);
            return true;
    }   
#endif 
#ifdef _SKILL_SERVER 
    float GetEffectdistance(Skill *skill) const
    {
        return (float)(skill->GetPlayer()->GetRange());
    }
#endif 
#ifdef _SKILL_SERVER 
    float GetTalent0 (PlayerWrapper * player) const
    {
        return (float) (player->GetFilterID (4485));
    }
#endif 
#ifdef _SKILL_SERVER 
    float GetTalent1 (PlayerWrapper * player) const
    {
        return (float) (player->GetFilterID (4486));
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
    int GetAttackspeed(Skill *skill) const
    {
        return 0;
    }
#endif 
#ifdef _SKILL_SERVER 
    bool TakeEffect(Skill *skill) const
    {;
        return true;
    }
#endif 
#ifdef _SKILL_SERVER 
    float GetHitrate(Skill *skill) const
    {
        return (float)(1.0);
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
