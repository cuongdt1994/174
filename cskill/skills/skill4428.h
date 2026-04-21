//Skill Parser from Elementskill.dll 1.7.4(v352) 

/* SKILL DESCRIPTION BEGIN */ 
/* 
44280  "ΨMagia de Retorno da Lâmina"44281  "^ff5c00 ΨMagia de Retorno da Lâmina
 
 ^ffcb4a Mana ^96f5ff200^ffffff
 ^ffcb4a Canalização ^ffffff instantânea ^ffcb4a
 ^ffcb4a Intervalo ^ffffff3.0 ^ffcb4a minutos
 ^ffcb4a Arma ^ffffff Espada
 ^ffcb4a Bônus de Runa ^ff5c00 prestígio
 
 ^ffcb4a Cultivo ^ffffff Diabólico
 Esforce-se, está tudo na palma da sua mão! Após utilizar, irá zerar o tempo de intervalo de Passo no Vazio e Irrestrito. 
 Há também ^96f5ff10%%^ffffff de chance de zerar o tempo de intervalo de ΨMagia de Retorno da Lâmina, além de aumentar a Probabilidade do impacto de explosão em ^96f5ff15%%^ffffff por ^96f5ff12^ffffff segundos.
 
 
 ^ff0000 Efeito Runa Escarlate @1
 ^ff0000 tem uma probabilidade de zerar o tempo de intervalo do Segredo da Lâmina
 ^ff0000Nv. 1-4: aumenta a probabilidade de zerar para ^96f5ff20%%^ff0000
 ^ff0000Nv. 5-7: aumenta a probabilidade de zerar para ^96f5ff30%%^ff0000
 ^ff0000Nv. 8-9: aumenta a probabilidade de zerar para ^96f5ff40%%^ff0000
 ^ff0000 Nv. 10: aumenta a probabilidade de zerar para ^96f5ff50%%^ff0000@1@2
 ^ff0000 tem uma probabilidade de zerar o tempo de intervalo do Segredo da Lâmina, aumenta a probabilidade de zerar para ^96f5ff%d%%^ff0000@2

"*/ 
/* SKILL DESCRIPTION END */ 

#ifndef __CPPGEN_GNET_SKILL4428 
#define __CPPGEN_GNET_SKILL4428 
namespace GNET 
{ 
#ifdef _SKILL_SERVER 
    class Skill4428:public Skill 
    { 
    public: 
        enum { SKILL_ID = 4428 }; 
        Skill4428 ():Skill (SKILL_ID){ } 
    }; 
#endif 
    class Skill4428Stub:public SkillStub 
    { 
    public: 
#define get_glyph(level, l1_4, l5_7, l8_9, lmax) level >= 0 && level <= 4 ? l1_4 : level >= 5 && level <= 7 ? l5_7 : level >= 8 && level <= 9 ? l8_9 : lmax; 

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
                skill->GetPlayer ()->SetDecmp (200);
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
    Skill4428Stub ():SkillStub (4428) 
    { 
        cls                 = 8; 
#ifdef _SKILL_CLIENT 
        name                = L"�񡤹��о�"; 
        nativename          = "�񡤹��о�"; 
        icon                = "����о���.dds"; 
#endif 
        max_level           = 1; 
        type                = 2; 
        allow_ride          = 0; 
        attr                = 0; 
        rank                = 40; 
        eventflag           = 0; 
        is_senior           = 1; 
        is_inherent         = 0; 
        is_movingcast       = 0; 
        npcdelay            = 0; 
        showorder           = 1133; 
        allow_forms         = 1; 
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
        effect              = "����_���о�.sgc"; 
        aerial_effect       = ""; 
#endif 
        range.type          = 5; 
        has_stateattack     = 0; 
        runes_attr          = 1; 
#ifdef _SKILL_CLIENT 
        gfxfilename         = "����״̬Ч����ǿ2.gfx"; 
        gfxhangpoint        = "HH_spine"; 
#endif 
        gfxtarget           = 0; 
        feature             = 0; 
        extra_effect        = 0; 
        immune_casting      = 0; 
        restrict_weapons.push_back (1); 
#ifdef _SKILL_SERVER 
        statestub.push_back (new State1 ()); 
#endif 
    } 
    int GetExecutetime (Skill * skill) const 
    { 
        static int aarray[10] = { 0,0,0,0,0,0,0,0,0,0 }; 
        return aarray[skill->GetLevel () - 1]; 
    } 
    int GetCoolingtime (Skill * skill) const 
    { 
        static int aarray[10] = { 180000,180000,180000,180000,180000,180000,180000,180000,180000,180000 }; 
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
        static int aarray[10] = { 11208,11208,11208,11208,11208,11208,11208,11208,11208,11208 }; 
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
            return _snwprintf (buffer, length, format);
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
            skill->GetVictim ()->SetRatio (6);
            skill->GetVictim ()->SetAmount (500);
            skill->GetVictim ()->SetValue (6948);
            skill->GetVictim ()->SetSkillcooldownCount (1);	
            
            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetRatio (1690);
            skill->GetVictim ()->SetAmount (500);
            skill->GetVictim ()->SetValue (1362);
            skill->GetVictim ()->SetSkillcooldown (1);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetRatio (3258);
            skill->GetVictim ()->SetAmount (500);
            skill->GetVictim ()->SetValue (3259);
            skill->GetVictim ()->SetSkillcooldown (1);

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetRatio (1360);
            skill->GetVictim ()->SetAmount (500);
            skill->GetVictim ()->SetValue (1688);
            skill->GetVictim ()->SetSkillcooldown (1);	

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (12500);
            skill->GetVictim ()->SetRatio (15);
            skill->GetVictim ()->SetGlyphCritical3 (1);  

            // Efeito
            int _glyph_value = get_glyph(skill->GetLevel (), 20, 30, 40, 50);

            skill->GetVictim ()->SetProbability (1.0 * _glyph_value);
            skill->GetVictim ()->SetRatio (4424);
            skill->GetVictim ()->SetAmount (500);
            skill->GetVictim ()->SetValue (4425);
            skill->GetVictim ()->SetSkillcooldown (1);	

            skill->GetVictim ()->SetProbability (1.0 * _glyph_value);
            skill->GetVictim ()->SetRatio (4426);
            skill->GetVictim ()->SetAmount (500);
            skill->GetVictim ()->SetValue (4427);
            skill->GetVictim ()->SetSkillcooldown (1);	

             skill->GetVictim ()->SetProbability (1.0 * _glyph_value);
            skill->GetVictim ()->SetRatio (4428);
            skill->GetVictim ()->SetAmount (500);
            skill->GetVictim ()->SetValue (4429);
            skill->GetVictim ()->SetSkillcooldown (1);	

            skill->GetVictim ()->SetProbability (1.0 * 100);
            skill->GetVictim ()->SetTime (12500);
            skill->GetVictim ()->SetRatio (15);
            skill->GetVictim ()->SetGlyphCritical3 (1); 
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
            return (float) (0);
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
