#ifndef __SKILL_SFILTERDEF_H__
#define __SKILL_SFILTERDEF_H__

enum filter_class_id{
	CLASS_SLOW            = 4096,   // ���٣����ͱ���
	CLASS_SLEEP           = 4097,   // ˯��
	CLASS_DIZZY           = 4098,   // ����
	CLASS_FIX             = 4099,   // ����
	CLASS_SEALED          = 4100,   // �޷�����
	CLASS_BLIND           = 4101,   // Ŀä 
	CLASS_RETORT          = 4102,   // ����

	CLASS_THUNDER         = 4103,   // �׻����������˺�
	CLASS_TOXIC           = 4104,   // �ж�
	CLASS_BURNING         = 4105,   // ȼ�գ��������˺�
	CLASS_FALLEN          = 4106,   // �������˺�
	CLASS_MAGICLEAK       = 4107,   // ������ȡMP
	CLASS_BLEEDING        = 4108,   // ��Ѫ

	CLASS_DECATTACK       = 4109,   // ���͹����������ͱ���
	CLASS_DECDEFENCE      = 4110,   // ���������������ͱ���
	CLASS_DECMAGIC        = 4111,   // ���ͷ��������������ͱ���
	CLASS_DECRESIST       = 4112,   // ���ͷ��������������ͱ���
	CLASS_DECDODGE        = 4113,   // ���Ͷ���ʣ�������ֵ
	CLASS_DECACCURACY     = 4114,   // ������������׼ȷ�ȣ�

	CLASS_TARDY           = 4115,   // ��������
	CLASS_SLOWPRAY        = 4116,   // ��������
	CLASS_INCHURT         = 4117,   // �����˺��ӱ�

	CLASS_FASTHPGEN       = 4118,   // MP�ָ��ӿ죬���� ������/ÿ�룩
	CLASS_FASTMPGEN       = 4119,   // HP�ָ��ӿ죬���� ������/ÿ�룩
	CLASS_INCHP           = 4120,   // ����HP���ֵ������
	CLASS_INCMP           = 4121,   // ����MP���ֵ������
	CLASS_INCATTACK       = 4122,   // ��ǿ������, ���ӱ���
	CLASS_INCDEFENCE      = 4123,   // ��ǿ������, ���ӱ���
	CLASS_INCMAGIC        = 4124,   // ��ǿ������, ���ӱ���
	CLASS_INCRESIST       = 4125,   // ��ǿ���������
	CLASS_INCDODGE        = 4126,   // �����ǿ������

	CLASS_FEATHERSHIELD   = 4127,   // ���
	CLASS_SOILSHIELD      = 4128,   // ��ʯ����
	CLASS_ICEBLADE        = 4129,   // ����ˮ�˺�

	CLASS_DECHP           = 4130,   // ����HP����
	CLASS_DECMP           = 4131,   // ����MP����
	CLASS_CRAZY           = 4132,   // ��������
	CLASS_SPEEDUP         = 4133,   // ���٣� ���ӱ���
	CLASS_ACTIVATESKILL   = 4134,   // �����
	CLASS_TIGERFORM       = 4135,   // �׻���
	CLASS_FRENETIC        = 4136,   // ��սʿ
	CLASS_TOXICBLADE      = 4137,   // ���Ӷ��˺�

	CLASS_ENHANCEGOLD     = 4138,   // ��ǿ�����
	CLASS_ENHANCEWOOD     = 4139,   // ��ǿľ����
	CLASS_ENHANCEWATER    = 4140,   // ��ǿˮ����?	
	CLASS_ENHANCEFIRE     = 4141,   // ��ǿ�����
	CLASS_ENHANCESOIL     = 4142,   // ��ǿ������

	CLASS_REDUCEGOLD     = 4143,    // ���ͽ����
	CLASS_REDUCEWOOD     = 4144,    // ����ľ����
	CLASS_REDUCEWATER    = 4145,    // ����ˮ����?	
	CLASS_REDUCEFIRE     = 4146,    // ���ͻ����
	CLASS_REDUCESOIL     = 4147,    // ����������
	CLASS_INCACCURACY    = 4148,    // ����׼ȷ
	CLASS_FIREBLADE      = 4151,    // ���ӻ��˺�
	CLASS_FASTPRAY       = 4152,    // ��������
	CLASS_DECHURT        = 4153,    // �����˺�����
	CLASS_HPGEN          = 4154,    // �����ָ�HP
	CLASS_MPGEN          = 4155,    // �����ָ�MP
	CLASS_YIJIN          = 4156,    // �׽
	CLASS_XISUI          = 4157,    // ϴ�辭
	CLASS_FIRESHIELD     = 4158,    // ���滤��
	CLASS_ICESHIELD      = 4159,    // ��������
	CLASS_APGEN          = 4160,    // ����֮ŭ
	CLASS_ANTIWATER      = 4161,    // ��ˮ��
	CLASS_POWERUP        = 4162,    // ��Ԫ����
	CLASS_STONESKIN      = 4163,    // ��׷�

	CLASS_IRONSHIELD     = 4164,    // ������
	CLASS_GIANT          = 4165,    // ��������
	CLASS_DEVILSTATE     = 4166,    // ��Ѫ��ħ
	CLASS_BLESSMAGIC     = 4167,    // ������
	CLASS_WINGSHIELD     = 4168,    // ����
	CLASS_FIREARROW      = 4169,    // ����֮ʸ
	CLASS_EAGLECURSE     = 4170,    // ������ӥ
	CLASS_FREEMOVE       = 4171,    // �����ж��谭״̬
	CLASS_FROZEN         = 4172,    // �������˺�
	CLASS_INCSMITE       = 4173,    // ���ӱ�����

	CLASS_CANTI          = 4174,    // ������
	CLASS_APLEAK         = 4175,    // �ܹ�����Ԫ��
	CLASS_FOXFORM        = 4176,    // �����
	CLASS_NOREGAIN       = 4177,    // �Զ��ָ�ֹͣ
	CLASS_JINGJI         = 4178,    // ������
	CLASS_ICON           = 4179,    // ������ʾͼ��

	CLASS_SWIFTFORM      = 4180,    // ������
	CLASS_FASTRIDE       = 4181,    // ��˼���
	CLASS_SHARPBLADE     = 4182,    // �����������
	CLASS_ADDATTACKDEGREE  = 4183,
	CLASS_SUBATTACKDEGREE  = 4184,
	CLASS_ADDDEFENCEDEGREE = 4185,
	CLASS_SUBDEFENCEDEGREE = 4186,
	CLASS_IMMUNESEALED 		= 4187,	//���߷�ӡ//lgc
	CLASS_IMMUNESLEEP		= 4188,	//����˯��
	CLASS_IMMUNESLOWDIZZY	= 4189,	//�����ƶ����ٺ���
	
	CLASS_IMMUNEWOUND		= 4190,	//���߰�������Ѫ
	CLASS_IMMUNEALL			= 4191,	//�������и���״̬
	CLASS_IMMUNEPHYSICAL	= 4192,	//���������˺�
	CLASS_IMMUNEFIRE		= 4193,	//���߻��˺�
	CLASS_IMMUNEWATER		= 4194,	//����ˮ�˺�
	CLASS_IMMUNEMETAL		= 4195,	//���߽��˺�
	CLASS_IMMUNEWOOD		= 4196,	//����ľ�˺�
	CLASS_IMMUNESOIL		= 4197,	//�������˺�
	CLASS_IMMUNEMAGICAL		= 4198,	//���������˺�
	CLASS_ARROGANT			= 4199,	//����һ��״̬

	CLASS_SLOWSWIM			= 4200,	//��Ӿ��������
	CLASS_FASTSWIM			= 4201,	//��Ӿ��������
	CLASS_SLOWFLY			= 4202,	//���б�������
	CLASS_FASTFLY			= 4203,	//���б�������
	CLASS_SLOWRIDE			= 4204, //��˱�������
	CLASS_APGENCONT			= 4205,	//��������Ԫ��
	CLASS_APLEAKCONT		= 4206, //��������Ԫ��
	CLASS_INCELFSTR			= 4207,	//����С��������
	CLASS_INCELFAGI			= 4208,	//����С��������
	CLASS_INCDEFENCE2		= 4209,	//������������
	
	CLASS_WEAKELEMENT		= 4210,	//Ԫ�������������������� ���������½�
	CLASS_DEEPPOISION 		= 4211,	//����ܵ��˺�����
	CLASS_ROOTED			= 4212,	//����������ͬʱ�˺�����ͬʱ�����˺�
	CLASS_EARTHGUARD		= 4213,	//����ػ����ظ�hp�˺����������ͷ��������½�
	CLASS_FURY				= 4214,	//�񱩣�Ŀ������ͷ�����������ͬʱ�����ȼ�����
	CLASS_SANDSTORM			= 4215,	//ɳ�������н���ʩ���ٶȱ���
	CLASS_HOMEFEELING		= 4216,	//���飬�����������ظ��ٶ������ƶ��ٶ�����
	CLASS_REDUCEWATER2		= 4217,	//����ˮ������
	CLASS_INCSMITE2			= 4218, //���ˣ���������
	CLASS_DECDEFENCE2		= 4219,	//�����������������
	
	CLASS_REDUCEFIRE2		= 4220,	//�𻨣����������
	CLASS_SLOWATTACKPRAY	= 4221,	//�Ļ������ͷ��������ٶ��½�
	CLASS_BURNING2			= 4222,	//���֣��������˺�
	CLASS_BURNINGFEET		= 4223,	//ȼ�㣬��������ͣ��ƶ��ٶȽ���
	CLASS_HARDENSKIN		= 4224,	//��ڣ����������ͷ����˺������������ƶ��ٶ�
	CLASS_REDUCEGOLD2		= 4225,	//���У����������
	CLASS_LEAFDANCE			= 4226,	//Ҷ�裬�ٻ�+��Ѫ
	CLASS_CHARRED			= 4227,	//�ս��������½������½��ƶ��ٶ�����
	CLASS_VACUUM			= 4228,	//��գ�ʩ���ٶȱ��� �ƶ����� �ܵ��˺�����
	CLASS_IMMUNEBLOODING	= 4229,	//������Ѫ
	
	CLASS_ABSORBPHYSICDAMAGE	= 4230,	//���������˺�
	CLASS_ABSORBMAGICDAMAGE		= 4231,	//���շ����˺�
	CLASS_RETORTMAGIC		= 4232,		//���������˺�
	CLASS_WINDSHIELD		= 4233,		//��ܣ��˺��������������ٶȺ���������
	CLASS_AIRSTREAMLOCK		= 4234,		//��������Ŀ�궨�����м��ʱ���ӡ
	CLASS_CLOSED			= 4235,		//��գ���ӡ����������
	CLASS_INSERTVSTATE		= 4236,		//����״̬Ч��	
	CLASS_IMMUNEWEAK		= 4237,		//�����˺�����
	CLASS_BEFROZEN			= 4238,		//���ᣬͬѣ��
	CLASS_FALLEN2			= 4239,		//�������˺����ܹ����ȼ�Ӱ��ͬburning2
	CLASS_SEALED2			= 4240,		//���ܹ���2
	CLASS_FIX2             	= 4241,   	// ����2
	CLASS_DECHURT2        	= 4242,    	// �����˺�����2
	CLASS_INCHURT2        	= 4243,   	// �����˺��ӱ�2
	CLASS_INCHP2           	= 4244,   	// ����HP���ֵ2������
	CLASS_INCATTACK2       	= 4245,   	// ��ǿ������2, ���ӱ���
	CLASS_INCMAGIC2        	= 4246,   	// ��ǿ������2, ���ӱ���
	CLASS_FASTPRAY2       	= 4247,    	// ��������2
	CLASS_SPEEDUP2         	= 4248,   	// ����2�� ���ӱ���
	CLASS_AURAFIREATTACK   	= 4249,		//�𹥻��⻷
	CLASS_AURABLESS			= 4250,		//ף���⻷
	CLASS_AURACURSE			= 4251,		//����⻷
	CLASS_INCANTIINVISIBLEACTIVE = 4252, //�����������ӷ����ȼ�
	CLASS_INCHPSTEAL		= 4253,		//������Ѫ	
	CLASS_INCCRITDAMAGE		= 4254,		//���ӱ����˺�
	CLASS_INCDAMAGEDODGE	= 4255,		//�����˺�����
	CLASS_INCDEBUFFDODGE	= 4256,		//����״̬����
	CLASS_REBIRTH			= 4257,		//����������
	CLASS_DEEPENBLESS		= 4258,		//ף������
	CLASS_WEAKENBLESS		= 4259,		//ף������
	CLASS_HURTWHENUSESKILL	= 4260,		//ʹ�ü���ʱ���˺�
	
	CLASS_INTERRUPTWHENUSESKILL = 4261,	//ʹ�ü��ܱ�����ʱ���ж�����жϼ���
	CLASS_SOULRETORT		= 4262,		//����֮��
	CLASS_SOULSEALED		= 4263,		//��ӡ֮��
	CLASS_SOULBEATBACK		= 4264,		//����֮��
	CLASS_SOULSTUN			= 4265,		//����֮��
	CLASS_FISHFORM			= 4266,		//���˱�
	CLASS_DEEPICETHRUST		= 4267,		//��ȱ���
	CLASS_ADJUSTATTACKDEFEND = 4268,	//�����ȼ�����
	CLASS_DELAYHURT   		= 4269,		//�ӳ��˺�
	CLASS_BEASTIEFORM		= 4270,		//���С����
	CLASS_INCHURTPHYSICGOLD	= 4271,		//���������Թ����˺�����
	CLASS_INCHURTWOODWATER	= 4272,		//ľˮ���Թ����˺�����
	CLASS_INCHURTFIREEARTH	= 4273,		//�������Թ����˺�����
	CLASS_ATTACKATTACHSTATE = 4274,		//����ʱ����״̬����
	CLASS_BEATTACKEDATTACHSTATE = 4275,	//������ʱ����״̬����
	CLASS_POISIONSEED		= 4276,		//����
	CLASS_HPGENSEED			= 4277,		//��Ѫ��
	CLASS_FASTPRAYINCMAGIC	= 4278,		//�ӿ��������ӷ���	
	CLASS_INCWOODWATERDEFENSE	= 4279,	//����ľ ˮ ���
	CLASS_SPECIALSLOW		= 4280,		//����ļ���
	CLASS_INCDEFENCESMITE	= 4281,		//�����������
	CLASS_INCRESISTMAGIC	= 4282,		//���ӷ�������
	CLASS_TRANSPORTMPTOPET	= 4283,		//���Լ��ĳ�������mp
	CLASS_TRANSPORTDAMAGETOPET = 4284,	//���������Ǳ��˵ģ������˺�
	CLASS_ABSORBDAMAGEINCDEFENSE = 4285,//���˺����﷨����
	CLASS_INCREMENTALHPGEN	= 4286,		//�����Ļ�Ѫ
	CLASS_CHANCEOFREBIRTH	= 4287,		//��ľ�괺
	CLASS_ACTIVATEREBOUNDSKILL = 4288,      //���������
	CLASS_SPECIALPHYSICHURTTRIGGER = 4289,	//����������˺�������
	CLASS_INCCOUNTEDSMITE	= 4290,		//�����ض������ı���
	CLASS_WEAPONDISABLED	= 4291,		//����ʧЧ
	CLASS_INCAGGROONDAMAGE	= 4292,		//�����ܵ��˺�ʱ�����ĳ��
	CLASS_ENHANCESKILLDAMAGE = 4293,	//���Ӽ��ܶ�npc���˺�
	CLASS_DETECTINVISIBLE	= 4294,		//�����Χ�������
	CLASS_DECAPPERHIT		= 4295,		//����ÿ���չ���õ�Ԫ��
	CLASS_FASTMPGEN2       	= 4296,   	// HP�ָ��ӿ죬���� ������/ÿ�룩
	CLASS_POSITIONROLLBACK	= 4297,
	CLASS_HPROLLBACK		= 4298,
	CLASS_NOFLY				= 4299,
	CLASS_NOCHANGESELECT	= 4300,
	CLASS_HEALABSORB		= 4301,
	CLASS_REPELONNORMALATTACK	= 4302,
	CLASS_INCCRITRESISTANCE		= 4303,
	CLASS_DECCRITRESISTANCE		= 4304,
	CLASS_TRANSMITSKILLATTACK	= 4305,
	CLASS_ADDITIONALHEAL		= 4306,
	CLASS_ADDITIONALATTACK		= 4307,
	CLASS_TRANSPORTDAMAGETOLEADER = 4308,
	CLASS_FORBIDBESELECTED		= 4309,
	CLASS_ENHANCESKILLDAMAGE2	= 4310,
	CLASS_DELAYEARTHHURT		= 4311,
	CLASS_DIZZYINCHURT			= 4312,
	CLASS_SOULRETORT2			= 4313,
	CLASS_FIXONDAMAGE			= 4314,
	CLASS_APGEN2				= 4315,
	CLASS_INCATTACK3			= 4316,
	CLASS_INCATTACKONDAMAGE		= 4317,
	CLASS_REBIRTH2				= 4318,
	CLASS_HEALSTEAL				= 4319,
	CLASS_DROPMONEYONDEATH		= 4320,
	CLASS_INCATTACKRANGE		= 4321,
	CLASS_THUNDERFORM			= 4322,
	CLASS_DELAYTRANSMIT			= 4323,
	CLASS_DECNORMALATTACKHURT	= 4324,
	CLASS_FREEMOVEAPGEN			= 4325,
	CLASS_INCATKDEFHP			= 4326,
	CLASS_DENYATTACKCMD			= 4327,
	CLASS_HPMPGENNOTINCOMBAT	= 4328,
	CLASS_INCHPMP				= 4329,
	CLASS_INCHURT3				= 4330,
	CLASS_INCRESIST2			= 4331,
	CLASS_FLAGER				= 4332,
	CLASS_SUBDEFENCEDEGREE2		= 4333,
	CLASS_INCATKDEFHP2			= 4334,
	CLASS_INCSMITE3				= 4335,
	CLASS_INCPENRES				= 4336,
	CLASS_INCMAXHPATKDFDLEVEL	= 4337,
	CLASS_DECHURT3				= 4338,
	CLASS_ATTACHSTATETOSELF		= 4339,
	CLASS_ATTACHSTATETOTARGET	= 4340,
	CLASS_IMMUNEPHYSICAL2		= 4341,	//���������˺�2
	CLASS_IMMUNEMETAL2			= 4342,	//���߽��˺�2
	CLASS_IMMUNEWOOD2			= 4343,	//����ľ�˺�2
	CLASS_IMMUNEWATER2			= 4344,	//����ˮ�˺�2
	CLASS_IMMUNEFIRE2			= 4345,	//���߻��˺�2
	CLASS_IMMUNESOIL2			= 4346,	//�������˺�2
	CLASS_RETORT2				= 4347,
	CLASS_ADDATTACKDEFENDDEGREE = 4348,
	CLASS_CHARIOTFORM			= 4349,
	CLASS_PALSY					= 4350, //̱��
	CLASS_APGENCONT2			= 4351,	//��������Ԫ��
	CLASS_INCHURTFROMSKILL		= 4352, //�����˺�����
	CLASS_INCBECRITRATE			= 4353, //���ܹ����ػ�����
	CLASS_MODIFYSPECSKILLPRAY	= 4354,
	CLASS_INCSPECSKILLDAMAGE	= 4355,
	CLASS_FIRESHIELD2			= 4356, // ���滤��2
	CLASS_ICESHIELD2 	 		= 4357, // ��������2
	CLASS_HEALSHIELD			= 4358,
	CLASS_INCFLYSPEED			= 4359,
	CLASS_INCVIGOUR				= 4360,
	CLASS_MINECARPROTECT		= 4361,
	CLASS_INCVIGOUR2			= 4362,
	CLASS_SHORTJUMP				= 4363,
	CLASS_SHORTJUMP2			= 4364,
	CLASS_MOVEPUNISH			= 4365,
	CLASS_STANDPUNISH			= 4366,
	CLASS_STANDPUNISH2			= 4367,
	CLASS_CHANTSHIELD			= 4368,
	CLASS_INTERVALPALSY			= 4369,
	CLASS_INTERNALINJURY		= 4370,
	CLASS_ATKDAMAGEREDUCE		= 4371,
	CLASS_DEATHRESETCD			= 4372,
	CLASS_APPENDENCHANT			= 4373,
	CLASS_APPENDDAMAGE			= 4374,
	CLASS_COOLDOWNAWARD			= 4375,
	CLASS_HUNTERSOUL			= 4376,
	CLASS_CHANGESELFAGGRO		= 4377,
	CLASS_NEVERDEAD				= 4378,
	CLASS_CHANGEMODEL			= 4379,
	CLASS_SCREENEFFECT			= 4380,
    CLASS_SHADOWFORM            = 4381,
    CLASS_FAIRYFORM             = 4382,
    CLASS_ADDFROSTEFFECT        = 4383,
    CLASS_INCSPECSKILLCRIT      = 4384,
    CLASS_MOONGOD               = 4385,
    CLASS_DELAYSKILLEFFECT      = 4386,
    CLASS_ENHANCESKILLDAMAGE3   = 4387,
	CLASS_CRITDAMAGEREDUCE		= 4388,
    CLASS_INCPHYSICALDAMAGE     = 4389,
    CLASS_INCMAGICALDAMAGE      = 4390,
	CLASS_COOLDOWNPUNISH		= 4391,
	CLASS_ANTICLEARBUF			= 4392,
	CLASS_INCENCHANTRANGE		= 4393,
    CLASS_INCPHYSICALMAGICALDEFENSE = 4394,
    CLASS_REDUCEGOLD3           = 4395,
    CLASS_REDUCEWATER3          = 4396,
    CLASS_GENHPAP               = 4397,
	CLASS_DETAINDART            = 4398,
    CLASS_EXTRAEXPFACTOR        = 4399,
	CLASS_WEAKENBLESS2			= 4400,
	CLASS_SOLO_INCATTACKANDMAGIC    = 4401,
	CLASS_SOLO_INCDEFENCE       = 4402,
	CLASS_SOLO_ENHANCERESIST    = 4403,
	CLASS_SOLO_INCMAXHP         = 4404,
	CLASS_SOLO_INVINCIBLE       = 4405,
	CLASS_SOLO_HPGEN            = 4406,
	CLASS_SOLO_DECHURT          = 4407,
	CLASS_SOLO_ADDATTACKRANGE   = 4408,
	CLASS_THUNDER3              = 4409,
	CLASS_TOXIC3                = 4410,
	CLASS_FLOOD3                = 4411,
	CLASS_BURNING3              = 4412,
	CLASS_FALLEN3               = 4413,
    CLASS_INCHURT6              = 4414,
    CLASS_INCANTIDEFENSEDEGREE  = 4415,
    CLASS_INCANTIRESISTANCEDEGREE = 4416,
	CLASS_MNFACTION_DECRESIST   = 4417,
	CLASS_INCATTACK4			= 4418,
	CLASS_INCMAGIC3				= 4419,
	CLASS_SOLO_ADDATTACKDEGREE  = 4420,
	CLASS_SOLO_ADDDEFENCEDEGREE = 4421,
    CLASS_ENHANCESKILLDAMAGE4   = 4422,
    CLASS_SLOW2                 = 4423,
    CLASS_DECHURT4              = 4424,
	CLASS_CANNONFORM            = 4425,
	
	CLASS_SHATTEREDDREAM		   = 4460, // Sonho
	CLASS_REDUCESPEEDDDEBUFF	   = 4461, // Buff velocidade + redução
	CLASS_SHIELDBUFFNEW1		   = 4462, // Shield Buff 1
	CLASS_SETNEWBUFFPALADIN	       = 4463,
	CLASS_SHIELDBUFFNEW2		   = 4464, // Shield Buff 2
	CLASS_SHIELDBUFFNEW3		   = 4465, // Shield Buff 3
	CLASS_NEWBUFFTELEPORT		   = 4466,
	CLASS_NEWBUFFSPEED			   = 4467,
	CLASS_IRAATIRADORA			   = 4468,
	CLASS_GELOATIRADORA			   = 4469,
	CLASS_FEATIRADORA			   = 4470,
	CLASS_POSITIONROLLBACK2		   = 4471,
	CLASS_SETREMOVENEW			   = 4472,
	CLASS_ULTIMATEPALADIN  	   = 4473, // Ultimate Paladino
	CLASS_ATTACKPLUSATR  	   = 4474, // Ultimate Atiradora
	CLASS_ADDPRAGA	= 4475, // Ultimate Atiradora
	CLASS_SKILLPLUSATR  	   = 4476, // Ultimate Atiradora
	CLASS_NEWBUFFTELEPORT2		   = 4477,

	CLASS_ACCUMULATEATTACK		   = 4438, //ULTWR
	CLASS_ACCUMULATEATTACK2		   = 4439, //ULTMG
	CLASS_INCPLUSMAXHP			   = 4440, //UltEP
	CLASS_AURACURSE4ASN			   = 4441, //UltMC
	CLASS_INCMAGIC4				   = 4442, //UltESP
	CLASS_DECSOUL				   = 4443, //UltESP
	CLASS_GIANTFORM				   = 4444, //UltSK
	CLASS_DECREGIONDMG			   = 4445, //CURARWF
	CLASS_TRANSDMG2PET			   = 4446, //UltESP
	CLASS_DECREGIONDMG2			   = 4447, 
	CLASS_LOADDMG4MASTER		   = 4448,
	CLASS_DECFARDMG				   = 4449, //ULTRT
	CLASS_DECPHYSICALMAGICALDEFENSE	= 4450, //ULTTM
	CLASS_ACTIVATESKILL2   = 5134,   // �����
	
	//G17
	CLASS_MAGICFRENETIC	= 4451,
	CLASS_DECHEAL	= 4452,
	CLASS_INCSMITEDAMAGE	= 4453, 
	CLASS_DECPHYSICALMAGICALDEFENSE2	= 4454,
	CLASS_FASTPRAY3       = 4455,
	CLASS_INCDEBUFFDODGE2	= 4456,
	CLASS_INCDAMAGEDODGE2	= 4457,
	CLASS_DEBITHURT	= 4458,
	CLASS_HEALNURSE	= 4459,


	// Novo Atributo 
	CLASS_HIGHESTLEVEL        = 4900,
	
	// Monkey New Skills

	CLASS_MONKEYFORM				= 4480,
	CLASS_MARCADOVENTO				= 4481,
	CLASS_MARCADAFLORESTA				= 4482,
	CLASS_MARCADOFOGO				= 4483,
	CLASS_MARCADAMONTANHA				= 4484,
	CLASS_MARCAYIN				= 4485,
	CLASS_MARCAYANG				= 4486,
	CLASS_FREEMOVEMONKEY				= 4488,

	// Passiva 

	CLASS_MONKEYDEFENSE				= 4487,	
	CLASS_FASTSWIMPASSIVE			= 4489,
	CLASS_MOVIMENTODASMAOS			= 4490,
	CLASS_SHATTEREDDREAMMONKEY  	   = 4491, // Teleport
	CLASS_FEMONKEY  	   = 4492, // FE
	CLASS_COMBOMONKEY				= 4493, // Combo das Marcas
	CLASS_COMBOYINYANG		= 4494, // Combo Yin Yang

	CLASS_REDUZDANOADISTANCIA			= 4495, // Reduz a distancia
	CLASS_COMBOYINYANG2		= 4496, // Combo Yin Yang 2
	CLASS_INVICIBLEMONKEYNEW				= 4497,
	CLASS_INCCRITREDUCE2				= 4498,


	// Novas Habilidades 1.6.6 > 

	CLASS_INCATTACKTODEFMF				= 4499,
	CLASS_INCNEWSHIELDMAGE				= 4500,
	CLASS_INCNEWSHIELDMAGE2				= 4501,
	CLASS_SETNEWSHIELDMAGE				= 4502,
	CLASS_NEWBUFFATIRADORA				= 4503,
	CLASS_NEWBUFFPALADIN				= 4504,

	CLASS_SHATTEREDDREAM2  	   = 4505, // Teleport
	CLASS_AURACURSEMONKEY		= 4506, //Aura Monkey
	CLASS_MONKEYFORMULTI				= 4507,

	// Nova Passiva 

	CLASS_REDUCEDEFENSEPASSIVE		= 4509,	
	CLASS_ARMADILHAPVP				= 4508,

	// Versos Paladino 

	CLASS_VERSOPALADIN01				= 4521,
	CLASS_VERSOPALADIN02				= 4522,
	CLASS_VERSOPALADIN03				= 4523,
	CLASS_VERSOPALADIN04				= 4524,
	CLASS_VERSOPALADIN05				= 4525,
	CLASS_VERSOPALADIN06				= 4526,
	
	CLASS_RETORT3						= 4530,
	CLASS_ELFLOCK						= 4531,
	CLASS_FREECOOLDOWN					= 4532,
	
	CLASS_SLEEP2						= 4533,
	CLASS_INCHURTMONKEY			        = 4534,

	// Novas Armas do WR
	CLASS_SETNEWWEAPONADDON				= 4537,
	CLASS_GLYPH_RANGE					= 4538,
	CLASS_GLYPH_DAMAGE					= 4539,
	CLASS_GLYPH_CASTING					= 4540,
	CLASS_GLYPH_CRITICAL				= 4541,
	CLASS_GLYPH_RANGE2					= 4542,
	CLASS_GLYPH_APCOST					= 4543,

	CLASS_APPLYDAMAGEFORRANGE			= 4544,
	CLASS_SHIELDDAMAGEREDUCE			= 4545,
	CLASS_GLYPH_CRITICAL2				= 4546,
	CLASS_GLYPH_CRITICALATR				= 4547,
	CLASS_GLYPH_JUMPROLLBACK			= 4548,

	CLASS_INVINCIBLEATR					= 4549,
	CLASS_ADDDEFENCEDEGREEATR			= 4550,
	CLASS_ADDATTACKDEGREEATR			= 4551,

	CLASS_BARBIANASSAULT				= 4552,
	CLASS_BARBIANJUMPTOTARGET			= 4553,
	CLASS_SHIELDDAMAGEREDUCEBARBIAN		= 4554,
	CLASS_SHEILDTRANSFERDEBUFFBARBIAN	= 4555,

	CLASS_ENHANCESKILLDAMAGEBARBIAN		= 4556,
	CLASS_INVINCIBLEWF					= 4557,

	CLASS_CONTINUOUSDAMAGEWF1			= 4558,
	CLASS_CONTINUOUSDAMAGEWF2			= 4559,
	CLASS_CONTINUOUSDAMAGEWF3			= 4560,
	CLASS_CONTINUOUSDAMAGEWF4			= 4561,
	CLASS_CONTINUOUSDAMAGEWF5			= 4562,

	CLASS_BARBIANJUMPTOTARGETWF			= 4563,
	CLASS_ELMODISABLED					= 4564,
	CLASS_GLYPHINVISIBLE				= 4565,

	CLASS_DEBUFFGLYPHSK1				= 4566,
	CLASS_DEBUFFGLYPHSK2				= 4567,
	CLASS_DEBUFFGLYPHSK3				= 4568,
	CLASS_DEBUFFGLYPHSK4				= 4569,
	CLASS_GLYPH_CRITICAL3				= 4570,
	CLASS_DEATHRESETCD2					= 4571,

	CLASS_FREECLEARDEBUFF				= 4572,
	CLASS_GLYPH_CRITICAL4				= 4573,

	CLASS_IMMUNEROAR	 	   = 4574,
	CLASS_NOCHANGESELECTMOB	= 4575,

	CLASS_DIZZYNOIMMUNE = 4576,
	CLASS_CANTIBLOW          = 4577,
	CLASS_DEATHRESETCDVOIDSTEP			= 4578,

	CLASS_ENHANCESKILLDAMAGEPELOTAO   = 4579,

	CLASS_GLYPH_APCOST_MULTI					= 4580,

	CLASS_DIZZYONDAMAGE			= 4581,

	CLASS_SETSWORDFORM		= 4582,
	CLASS_SETAXEFORM		= 4583,
	CLASS_SETFISTFORM		= 4584,
	CLASS_SETPOLEARMFORM		= 4585,
	CLASS_IMMUNETRANSFWB			= 4586,

	CLASS_ADDDEFENCEDEGREE2 = 4587, 
	CLASS_INCDEFENSEDEGREEONDAMAGE = 4588, 
	CLASS_PHYSMAGREDUCWITHSTACKWB = 4589,
	
	CLASS_NEWSTUN = 4590,
	CLASS_SHIELDDAMAGEMYSTIC = 4591,

	CLASS_ANTICLEARBUF2			= 4592,
	CLASS_CONVERTATAQMTOATAQF	= 4593,
	CLASS_CONVERTATAQMTOATAQF2	= 4594,

	CLASS_DECDEFENCENEW      = 4595,

	CLASS_INCDEFENSERESITWITHDAMAGESTART = 4696,
	CLASS_INCDEFENSERESITWITHDAMAGE = 4697,

	CLASS_ELEMENTDAMAGE = 4698,
	CLASS_NEWSHAPEANDSKILLS = 4699,

	CLASS_SOULBEATBACKWF = 4705,
	CLASS_SOULBEATBACKPALADIN = 4706,
	CLASS_ENHANCESKILLDAMAGEPLAYER = 4707,

	CLASS_INCFARSKILLDMGREDUCE = 4708,
	CLASS_SETINVISIBLEFORCED = 4709,
	CLASS_SETFORCETRANSFORM = 4710,
	CLASS_SETRECOVERYAPPERSEC = 4711,
	CLASS_SETADJUSTDAMAGESENDRECV = 4712,

	CLASS_SETADJUSTHPFROMMP = 4713,
	CLASS_SETINCDAMAGEREDUCEPERC = 4714,
	CLASS_SETIIMMUNENEGATIVEEFFECT = 4715,
	CLASS_SETRECUCENORMALDAMAGE = 4716,
	CLASS_SETSPEEDNOCOMBAT = 4717,
	CLASS_SETADJUSTDAMAGESENDRECV2 = 4718,
	CLASS_SETPLUSDAAMAGEFROMHP = 4719,
	CLASS_GUILDREDUCEDEFENSEPASSIVE = 4720,

	CLASS_SETBATTLEPASSFILTER = 4721,

};

enum filter_id{
	FILTER_SLOW            = 4096,   // ���٣����ͱ���
	FILTER_SLEEP           = 4097,   // ˯��
	FILTER_DIZZY           = 4098,   // ����
	FILTER_FIX             = 4099,   // ����
	FILTER_SEALED          = 4100,   // �޷�����
	FILTER_BLIND           = 4101,   // Ŀä 
	FILTER_RETORT          = 4102,   // ����

	FILTER_THUNDER         = 4103,   // �׻����������˺�
	FILTER_TOXIC           = 4104,   // �ж�
	FILTER_BURNING         = 4105,   // ȼ�գ��������˺�
	FILTER_FALLEN          = 4106,   // �������˺�
	FILTER_MAGICLEAK       = 4107,   // ������ȡMP
	FILTER_BLEEDING        = 4108,   // ��Ѫ

	FILTER_DECATTACK       = 4109,   // ���͹����������ͱ���
	FILTER_DECDEFENCE      = 4110,   // ���������������ͱ���
	FILTER_DECMAGIC        = 4111,   // ���ͷ��������������ͱ���
	FILTER_DECRESIST       = 4112,   // ���ͷ��������������ͱ���
	FILTER_DECDODGE        = 4113,   // ���Ͷ���ʣ�������ֵ
	FILTER_DECACCURACY     = 4114,   // ������������׼ȷ�ȣ�

	FILTER_TARDY           = 4115,   // ��������
	FILTER_SLOWPRAY        = 4116,   // ��������
	FILTER_INCHURT         = 4117,   // �����˺��ӱ�

	FILTER_FASTHPGEN       = 4118,   // MP�ָ��ӿ죬���� ������/ÿ�룩
	FILTER_FASTMPGEN       = 4119,   // HP�ָ��ӿ죬���� ������/ÿ�룩
	FILTER_INCHP           = 4120,   // ����HP���ֵ������
	FILTER_INCMP           = 4121,   // ����MP���ֵ������
	FILTER_INCATTACK       = 4122,   // ��ǿ������, ���ӱ���
	FILTER_INCDEFENCE      = 4123,   // ��ǿ������, ���ӱ���
	FILTER_INCMAGIC        = 4124,   // ��ǿ������, ���ӱ���
	FILTER_INCRESIST       = 4125,   // ��ǿ���������
	FILTER_INCDODGE        = 4126,   // �����ǿ������

	FILTER_FEATHERSHIELD   = 4127,   // ���
	FILTER_MAGICSHIELD      = 4128,   // ��ʯ/����/���滤��
	FILTER_ICEBLADE        = 4129,   // ����ˮ�˺�

	FILTER_DECHP           = 4130,   // ����HP����
	FILTER_DECMP           = 4131,   // ����MP����
	FILTER_CRAZY           = 4132,   // ��������
	FILTER_SPEEDUP         = 4133,   // ���٣� ���ӱ���
	FILTER_ACTIVATESKILL   = 4134,   // �����
	FILTER_TIGERFORM       = 4135,   // �׻���
	FILTER_FRENETIC        = 4136,   // ��սʿ
	FILTER_TOXICBLADE      = 4137,   // ���Ӷ��˺�

	FILTER_ENHANCEGOLD     = 4138,   // ��ǿ�����
	FILTER_ENHANCEWOOD     = 4139,   // ��ǿľ����
	FILTER_ENHANCEWATER    = 4140,   // ��ǿˮ����?	
	FILTER_ENHANCEFIRE     = 4141,   // ��ǿ�����
	FILTER_ENHANCESOIL     = 4142,   // ��ǿ������

	FILTER_REDUCEGOLD      = 4143,   // ���ͽ����
	FILTER_REDUCEWOOD      = 4144,   // ����ľ����
	FILTER_REDUCEWATER     = 4145,   // ����ˮ����?	
	FILTER_REDUCEFIRE      = 4146,   // ���ͻ����
	FILTER_REDUCESOIL      = 4147,   // ����������
	FILTER_INCACCURACY     = 4148,   // ����׼ȷ
	FILTER_FIREBLADE       = 4151,   // ���ӻ��˺�
	FILTER_FASTPRAY        = 4152,   // ��������
	FILTER_DECHURT         = 4153,   // �����˺�����
	FILTER_HPGEN           = 4154,   // �����ָ�HP
	FILTER_MPGEN           = 4155,   // �����ָ�MP
	FILTER_YIJIN           = 4156,   // �׽
	FILTER_XISUI           = 4156,   // ϴ�辭
	FILTER_APGEN           = 4157,   // ����֮ŭ
	FILTER_ANTIWATER       = 4158,   // ��ˮ��
	FILTER_POWERUP         = 4159,   // ��Ԫ����
	FILTER_STONESKIN       = 4160,   // ��׷� 

	FILTER_IRONSHIELD       = 4161,    // ������
	FILTER_GIANT            = 4162,    // ��������
	FILTER_DEVILSTATE       = 4163,    // ��Ѫ��ħ
	FILTER_BLESSMAGIC       = 4164,    // ������
	FILTER_WINGSHIELD       = 4165,    // ����
	FILTER_FIREARROW        = 4166,    // ����֮ʸ
	FILTER_EAGLECURSE       = 4167,    // ������ӥ
	FILTER_FREEMOVE         = 4168,    // �����ж��谭״̬

	FILTER_THUNDER_MERGE    = 4169,   // �׻����������˺�
	FILTER_TOXIC_MERGE      = 4170,   // �ж�
	FILTER_BURNING_MERGE    = 4171,   // ȼ�գ��������˺�
	FILTER_FALLEN_MERGE     = 4172,   // �������˺�
	FILTER_BLEEDING_MERGE   = 4173,   // ��Ѫ
	FILTER_EAGLECURSE_MERGE = 4174,   // ������ӥ

	FILTER_FROZEN           = 4175,   // �������˺�
	FILTER_FROZEN_MERGE     = 4176,   // �������˺�

	FILTER_INCSMITE         = 4177,   // ���ӱ�����
	FILTER_SPIRITCURSE      = 4178,   // ������/�ܹ�����Ԫ��/����
	FILTER_FOXFORM          = 4179,   // �����
	FILTER_ICON             = 4180,   // ������ʾͼ��͸���״̬
	FILTER_FLOWER           = 4181,   // �ʻ�

	FILTER_SWIFTFORM        = 4182,   // ������
	FILTER_FASTRIDE         = 4183,   // ��˼���
	FILTER_SHARPBLADE       = 4184,   // �����������
	FILTER_ADDATTACKDEGREE  = 4185, 
	FILTER_SUBATTACKDEGREE  = 4186, 
	FILTER_ADDDEFENCEDEGREE = 4187, 
	FILTER_SUBDEFENCEDEGREE = 4188, 
	
	FILTER_IMMUNESEALED		= 4189,	//���߷�ӡ//lgc
	FILTER_IMMUNESLEEP		= 4190,	//����˯��
	FILTER_IMMUNESLOWDIZZY	= 4191,	//�����ƶ����ٺ���
	
	FILTER_IMMUNEWOUND		= 4192,	//���߰�������Ѫ
	FILTER_IMMUNEALL		= 4193,	//�������и���״̬
	FILTER_IMMUNEPHYSICAL	= 4194,	//���������˺�
	FILTER_IMMUNEFIRE		= 4195,	//���߻��˺�
	FILTER_IMMUNEWATER		= 4196,	//����ˮ�˺�
	FILTER_IMMUNEMETAL		= 4197,	//���߽��˺�
	FILTER_IMMUNEWOOD		= 4198,	//����ľ�˺�
	FILTER_IMMUNESOIL		= 4199,	//�������˺�
	FILTER_IMMUNEMAGICAL	= 4200,	//���������˺�
	FILTER_ARROGANT			= 4201,	//����һ��״̬

	FILTER_SLOWSWIM			= 4202,	//��Ӿ��������
	FILTER_FASTSWIM			= 4203,	//��Ӿ��������
	FILTER_SLOWFLY			= 4204,	//���б�������
	FILTER_FASTFLY			= 4205,	//���б�������
	FILTER_SLOWRIDE			= 4206, //��˱�������
	FILTER_APGENCONT		= 4207,	//��������Ԫ��
	FILTER_APLEAKCONT		= 4208, //��������Ԫ��
	FILTER_INCELFSTR		= 4209,	//����С��������
	FILTER_INCELFAGI		= 4210,	//����С��������
	FILTER_INCDEFENCE2		= 4211,	//������������
	
	FILTER_WEAKELEMENT		= 4212,	//Ԫ�������������������� ���������½�
	FILTER_DEEPPOISION 		= 4213,	//����ܵ��˺�����
	FILTER_ROOTED			= 4214,	//����������ͬʱ�˺�����ͬʱ�����˺�
	FILTER_EARTHGUARD		= 4215,	//����ػ����ظ�hp�˺����������ͷ��������½�
	FILTER_FURY				= 4216,	//�񱩣�Ŀ������ͷ�����������ͬʱ�����ȼ�����
	FILTER_SANDSTORM		= 4217,	//ɳ�������н���ʩ���ٶȱ���
	FILTER_HOMEFEELING		= 4218,	//���飬�����������ظ��ٶ������ƶ��ٶ�����
	FILTER_REDUCEWATER2		= 4219,	//����ˮ������
	FILTER_INCSMITE2		= 4220, //���ˣ���������
	FILTER_DECDEFENCE2		= 4221,	//�����������������
	
	FILTER_REDUCEFIRE2		= 4222,	//�𻨣����������
	FILTER_SLOWATTACKPRAY	= 4223,	//�Ļ������ͷ��������ٶ��½�
	FILTER_BURNING2			= 4224,	//���֣��������˺�
	FILTER_BURNINGFEET		= 4225,	//ȼ�㣬��������ͣ��ƶ��ٶȽ���
	FILTER_HARDENSKIN		= 4226,	//��ڣ����������ͷ����˺������������ƶ��ٶ�
	FILTER_REDUCEGOLD2		= 4227,	//���У����������
	FILTER_LEAFDANCE		= 4228,	//Ҷ�裬�ٻ�+��Ѫ
	FILTER_CHARRED			= 4229,	//�ս��������½������½��ƶ��ٶ�����
	FILTER_VACUUM			= 4230,	//��գ�ʩ���ٶȱ��� �ƶ����� �ܵ��˺�����
	FILTER_IMMUNEBLOODING	= 4231,	//������Ѫ
	
	FILTER_ABSORBPHYSICDAMAGE	= 4232,	//���������˺�
	FILTER_ABSORBMAGICDAMAGE	= 4233,	//���շ����˺�
	FILTER_RETORTMAGIC		= 4234,		//���������˺�
	FILTER_WINDSHIELD		= 4235,		//��ܣ��˺��������������ٶȺ���������
	FILTER_AIRSTREAMLOCK	= 4236,		//��������Ŀ�궨�����м��ʱ���ӡ
	FILTER_CLOSED			= 4237,		//��գ���ӡ����������
	FILTER_INSERTVSTATE		= 4238,		//����״̬Ч��
	FILTER_IMMUNEWEAK		= 4239,		//�����˺�����
	FILTER_BEFROZEN			= 4240,		//���ᣬͬѣ��
	FILTER_BURNING2_MERGE 	= 4241,		//���֣��������˺�
	FILTER_LEAFDANCE_MERGE 	= 4242,		//Ҷ�裬�ٻ�����Ѫ
	FILTER_FALLEN2			= 4243,		//�������˺����ܹ����ȼ�Ӱ��ͬburning2
	FILTER_FALLEN2_MERGE	= 4244,		//�������˺����ܹ����ȼ�Ӱ��ͬburning2
	FILTER_SEALED2			= 4245,		//���ܹ���2
	FILTER_FIX2             = 4246,   	// ����2
	FILTER_DECHURT2        	= 4247,    	// �����˺�����2
	FILTER_INCHURT2        	= 4248,   	// �����˺��ӱ�2
	FILTER_INCHP2           = 4249,   	// ����HP���ֵ2������
	FILTER_INCATTACK2       = 4250,   	// ��ǿ������2, ���ӱ���
	FILTER_INCMAGIC2        = 4251,   	// ��ǿ������2, ���ӱ���
	FILTER_FASTPRAY2       	= 4252,    	// ��������2
	FILTER_SPEEDUP2         = 4253,   	// ����2�� ���ӱ���
	FILTER_AURAFIREATTACK   = 4254,		//�𹥻��⻷
	FILTER_AURABLESS		= 4255,		//ף���⻷
	FILTER_AURACURSE		= 4256,		//����⻷
	FILTER_INCANTIINVISIBLEACTIVE = 4257, //�����������ӷ����ȼ�
	FILTER_INCHPSTEAL		= 4258,		//������Ѫ	
	FILTER_INCCRITDAMAGE	= 4259,		//���ӱ����˺�
	FILTER_INCDAMAGEDODGE	= 4260,		//�����˺�����
	FILTER_INCDEBUFFDODGE	= 4261,		//����״̬����
	FILTER_REBIRTH			= 4262,		//����������
	FILTER_DEEPENBLESS		= 4263,		//ף������
	FILTER_WEAKENBLESS		= 4264,		//ף������
	FILTER_HURTWHENUSESKILL	= 4265,		//ʹ�ü���ʱ���˺�
	
	FILTER_INTERRUPTWHENUSESKILL = 4266,//ʹ�ü��ܱ�����ʱ���ж�����жϼ���
	FILTER_SOULRETORT		= 4267,		//����֮��
	FILTER_SOULSEALED		= 4268,		//��ӡ֮��
	FILTER_SOULBEATBACK		= 4269,		//����֮��
	FILTER_SOULSTUN			= 4270,		//����֮��
	FILTER_FISHFORM			= 4271,		//���˱�
	FILTER_DEEPICETHRUST	= 4272,		//��ȱ���
	FILTER_ADJUSTATTACKDEFEND = 4273,	//�����ȼ�����
	FILTER_DELAYHURT   		= 4274,		//�ӳ��˺�
	FILTER_BEASTIEFORM		= 4275,		//���С����
	FILTER_INCHURTPHYSICGOLD= 4276,		//���������Թ����˺�����
	FILTER_INCHURTWOODWATER	= 4277,		//ľˮ���Թ����˺�����
	FILTER_INCHURTFIREEARTH	= 4278,		//�������Թ����˺�����
	FILTER_ATTACKATTACHSTATE = 4279,	//����ʱ����״̬����
	FILTER_BEATTACKEDATTACHSTATE1 = 4280,//������ʱ����״̬����1
	FILTER_BEATTACKEDATTACHSTATE2 = 4281,//������ʱ����״̬����2
	FILTER_BEATTACKEDATTACHSTATE3 = 4282,//������ʱ����״̬����3
	FILTER_POISIONSEED		= 4283,		//����
	FILTER_HPGENSEED		= 4284,		//��Ѫ��
	FILTER_FASTPRAYINCMAGIC	= 4285,		//�ӿ��������ӷ���	
	FILTER_INCWOODWATERDEFENSE	= 4286,	//����ľ ˮ ���
	FILTER_SPECIALSLOW		= 4287,		//����ļ���
	FILTER_INCDEFENCESMITE	= 4288,		//�����������
	FILTER_INCRESISTMAGIC	= 4289,		//���ӷ�������
	FILTER_TRANSPORTMPTOPET	= 4290,		//���Լ��ĳ�������mp
	FILTER_TRANSPORTDAMAGETOPET = 4291,	//���������Ǳ��˵ģ������˺�
	FILTER_ABSORBDAMAGEINCDEFENSE = 4292,//���˺����﷨����
	FILTER_INCREMENTALHPGEN	= 4293,		//�����Ļ�Ѫ
	FILTER_CHANCEOFREBIRTH	= 4294,		//��ľ�괺
	FILTER_SPECIALPHYSICHURTTRIGGER = 4295,	//����������˺�������
	FILTER_INCCOUNTEDSMITE	= 4296,		//�����ض������ı���
	FILTER_BEATTACKEDATTACHSTATE4 = 4297,//������ʱ����״̬����4
	FILTER_WEAPONDISABLED	= 4298,		//����ʧЧ
	FILTER_INCAGGROONDAMAGE	= 4299,		//�����ܵ��˺�ʱ�����ĳ��
	FILTER_ENHANCESKILLDAMAGE = 4300,	//���Ӽ��ܶ�npc���˺�
	FILTER_DETECTINVISIBLE	= 4301,		//�����Χ�������
	FILTER_DECAPPERHIT		= 4302,		//����ÿ���չ���õ�Ԫ��
	FILTER_FASTMPGEN2       = 4303,   	// HP�ָ��ӿ죬���� ������/ÿ�룩
	FILTER_POSITIONROLLBACK	= 4304,
	FILTER_HPROLLBACK		= 4305,
	FILTER_NOFLY			= 4306,
	FILTER_NOCHANGESELECT	= 4307,
	FILTER_HEALABSORB		= 4308,
	FILTER_REPELONNORMALATTACK	= 4309,
	FILTER_INCCRITRESISTANCE	= 4310,
	FILTER_DECCRITRESISTANCE	= 4311,
	FILTER_TRANSMITSKILLATTACK	= 4312,
	FILTER_ADDITIONALHEAL		= 4313,
	FILTER_ADDITIONALATTACK		= 4314,
	FILTER_TRANSPORTDAMAGETOLEADER = 4315,
	FILTER_FORBIDBESELECTED		= 4316,
	FILTER_ENHANCESKILLDAMAGE2	= 4317,
	FILTER_DELAYEARTHHURT		= 4318,
	FILTER_DIZZYINCHURT			= 4319,
	FILTER_SOULRETORT2			= 4320,
	FILTER_FIXONDAMAGE			= 4321,
	FILTER_APGEN2				= 4322,
	FILTER_INCATTACK3			= 4323,
	FILTER_INCATTACKONDAMAGE	= 4324,
	FILTER_REBIRTH2				= 4325,
	FILTER_HEALSTEAL			= 4326,
	FILTER_DROPMONEYONDEATH		= 4327,
	FILTER_INCATTACKRANGE		= 4328,
	FILTER_THUNDERFORM			= 4329,
	FILTER_DELAYTRANSMIT		= 4330,
	FILTER_DECNORMALATTACKHURT	= 4331,
	FILTER_FREEMOVEAPGEN		= 4332,
	FILTER_INCATKDEFHP			= 4333,
	FILTER_DENYATTACKCMD		= 4334,
	FILTER_HPMPGENNOTINCOMBAT	= 4335,
	FILTER_INCHPMP				= 4336,
	FILTER_INCHURT3				= 4337,
	FILTER_INCRESIST2      		= 4338,
	FILTER_FLAGER				= 4339,
	FILTER_SUBDEFENCEDEGREE2	= 4340, 
	FILTER_INCATKDEFHP2			= 4341,
	FILTER_INCSMITE3			= 4342,
	FILTER_INCPENRES			= 4343,
	FILTER_INCMAXHPATKDFDLEVEL	= 4344,
	FILTER_DECHURT3				= 4345,
	FILTER_ATTACHSTATETOSELF	= 4346,
	FILTER_ATTACHSTATETOTARGET	= 4347,
	FILTER_IMMUNEPHYSICAL2		= 4348,	//���������˺�2
	FILTER_IMMUNEMETAL2			= 4349,	//���߽��˺�2
	FILTER_IMMUNEWOOD2			= 4350,	//����ľ�˺�2
	FILTER_IMMUNEWATER2			= 4351,	//����ˮ�˺�2
	FILTER_IMMUNEFIRE2			= 4352,	//���߻��˺�2
	FILTER_IMMUNESOIL2			= 4353,	//�������˺�2
	FILTER_RETORT2				= 4354,
	FILTER_ADDATTACKDEFENDDEGREE= 4355,
	FILTER_PALSY				= 4356, //̱��
	FILTER_APGENCONT2			= 4357,	//��������Ԫ��2
	FILTER_INCHURT4				= 4358, //�˺�����4
	FILTER_INCBECRITRATE		= 4359, //���ܹ����ػ�����
	FILTER_MODIFYSPECSKILLPRAY	= 4360,
	FILTER_INCSPECSKILLDAMAGE	= 4361,
	FILTER_INCFLYSPEED			= 4362,
	FILTER_INCVIGOUR			= 4363,
	FILTER_MINECARPROTECT		= 4364,
	FILTER_INCVIGOUR2			= 4365,
	FILTER_SHORTJUMP			= 4366,
	FILTER_SHORTJUMP2			= 4367,
	FILTER_MOVEPUNISH			= 4368,
	FILTER_STANDPUNISH			= 4369,
	FILTER_STANDPUNISH2			= 4370,
	FILTER_CHANTSHIELD			= 4371,
	FILTER_INTERVALPALSY		= 4372,
	FILTER_INTERNALINJURY		= 4373,
	FILTER_ATKDAMAGEREDUCE		= 4374,
	FILTER_DEATHRESETCD			= 4375,
	FILTER_APPENDENCHANT		= 4376,
	FILTER_APPENDDAMAGE			= 4377,
	FILTER_COOLDOWNAWARD		= 4378,
	FILTER_HUNTERSOUL			= 4379,
	FILTER_CHANGESELFAGGRO		= 4380,
	FILTER_NEVERDEAD			= 4381,
	FILTER_CHANGEMODEL			= 4382,
	FILTER_SCREENEFFECT			= 4383,
    FILTER_SHADOWFORM           = 4384,
    FILTER_FAIRYFORM            = 4385,
    FILTER_ADDFROSTEFFECT       = 4386,
    FILTER_INCSPECSKILLCRIT     = 4387,
    FILTER_MOONGOD              = 4388,
    FILTER_DELAYSKILLEFFECT     = 4380,  
    FILTER_ENHANCESKILLDAMAGE3  = 4390,
	FILTER_CRITDAMAGEREDUCE		= 4391,
	FILTER_IMMUNEMAGICAL2		= 4392,	//���������˺�2
    FILTER_INCPHYSICALDAMAGE    = 4393,
    FILTER_INCMAGICALDAMAGE     = 4394,
	FILTER_INCHURT5				= 4395,
	FILTER_COOLDOWNPUNISH		= 4396,
	FILTER_ANTICLEARBUF			= 4397,
	FILTER_INCENCHANTRANGE		= 4398,
    FILTER_INCPHYSICALMAGICALDEFENSE = 4399,
    FILTER_REDUCEGOLD3          = 4400,
    FILTER_REDUCEWATER3         = 4401,
    FILTER_GENHPAP              = 4402,
	FILTER_DETAINDART           = 4403,
    FILTER_EXTRAEXPFACTOR       = 4404,
	FILTER_WEAKENBLESS2			= 4405,
	FILTER_SOLO_INCATTACKANDMAGIC    = 4406,
	FILTER_SOLO_INCDEFENCE      = 4407,
	FILTER_SOLO_ENHANCERESIST   = 4408,
	FILTER_SOLO_INCMAXHP        = 4409,
	FILTER_SOLO_INVINCIBLE      = 4410,
	FILTER_SOLO_HPGEN           = 4411,
	FILTER_SOLO_DECHURT         = 4412,
	FILTER_SOLO_ADDATTACKRANGE  = 4413,
	FILTER_SOLO_ADDATTACKDEGREE = 4414,
	FILTER_SOLO_ADDDEFENCEDEGREE  = 4415,
	FILTER_SOLO_RESERVE2        = 4416,
	FILTER_SOLO_RESERVE3        = 4417,
	FILTER_SOLO_RESERVE4        = 4418,
	FILTER_SOLO_RESERVE5        = 4419,
	FILTER_SOLO_RESERVE6        = 4420,
	FILTER_SOLO_RESERVE7        = 4421,
	FILTER_THUNDER3             = 4422,
	FILTER_TOXIC3               = 4423,
	FILTER_FLOOD3               = 4424,
	FILTER_BURNING3             = 4425,
	FILTER_FALLEN3              = 4426,
    FILTER_INCHURT6             = 4427,
    FILTER_INCANTIDEFENSEDEGREE = 4428,
    FILTER_INCANTIRESISTANCEDEGREE = 4429,
	FILTER_MNFACTION_DECRESIST  = 4430,
	FILTER_INCATTACK4			= 4431,
	FILTER_INCMAGIC3			= 4432,
    FILTER_ENHANCESKILLDAMAGE4  = 4433,
    FILTER_SLOW2                = 4434,
    FILTER_DECHURT4             = 4435,
	
	
	FILTER_ACCUMULATEATTACK		= 4436, //UltWR
	FILTER_ACCUMULATEATTACK2	= 4437, //UltMG
	FILTER_INCPLUSMAXHP			= 4438, //UltEP
	FILTER_AURACURSE4ASN		= 4439, //UltMC
	FILTER_INCMAGIC4			= 4440, //UltESP
	FILTER_DECSOUL				= 4441, //UltESP
	FILTER_GIANTFORM			= 4442, //UltSK
	FILTER_TRANSDMG2PET			= 4443, //UltMS
	FILTER_LOADDMG4MASTER		= 4444,
	FILTER_DECREGIONDMG			= 4445, //CURAWF
	FILTER_DECREGIONDMG2		= 4446, 
	FILTER_DECFARDMG			= 4447, //ULTRT
	FILTER_DECPHYSICALMAGICALDEFENSE	= 4448, //ULTTM
	//G17
	FILTER_DECHEAL = 4450,
	FILTER_MAGICFRENETIC = 4451,	
	FILTER_DECPHYSICALMAGICALDEFENSE2	= 4449,
	FILTER_INCSMITEDAMAGE = 4452,
	FILTER_FASTPRAY3        = 4453,
	FILTER_INCDEBUFFDODGE2	= 4454,
	FILTER_INCDAMAGEDODGE2	= 4455,
	FILTER_DEBITHURT	= 4456,
	FILTER_HEALNURSE    = 4457,
	FILTER_ACTIVATESKILL2   = 4459,   // �����


	
	FILTER_SHATTEREDDREAM   	   = 4460, // Sonho
	FILTER_REDUCESPEEDDDEBUFF  	   = 4461, // Buff velocidade + redução
	FILTER_SETNEWBUFFPALADIN	   = 4462,	 
	FILTER_SHIELDBUFFNEW1		   = 4463, // Shield Buff 1
	FILTER_SHIELDBUFFNEW2		   = 4464, // Shield Buff 2
	FILTER_SHIELDBUFFNEW3		   = 4465, // Shield Buff 3
	FILTER_NEWSPEEDBUFF			   = 4466,
	FILTER_IRAATIRADORA 		   = 4467,
	FILTER_GELOATIRADORA 		   = 4468,
	FILTER_FEATIRADORA 		 	   = 4469,
	FILTER_POSITIONROLLBACK2	   = 4470,
	FILTER_SETREMOVENEW			   = 4471,

	FILTER_ULTIMATEPALADIN  	   = 4473, // Ultimate Paladino
	FILTER_ATTACKPLUSATR	= 4474, // Ultimate Atiradora
	FILTER_ADDPRAGA		= 4475, // Ultimate Atiradora
	FILTER_SKILLPLUSATR	= 4476, // Ultimate Atiradora

	// Novo Atributo 
	FILTER_HIGHESTLEVEL        = 4900,
	FILTER_CANNONFORM          = 4550,
 
	// Monkey New Skills

	FILTER_MONKEYFORM				= 4480,
	FILTER_MONKEYFORMULTI				= 4507,
	FILTER_MARCADOVENTO				= 4481,
	FILTER_MARCADAFLORESTA				= 4482,
	FILTER_MARCADOFOGO				= 4483,
	FILTER_MARCADAMONTANHA				= 4484,
	FILTER_MARCAYIN				= 4485,
	FILTER_MARCAYANG				= 4486,
	FILTER_FREEMOVEMONKEY				= 4488,
	FILTER_AURACURSEMONKEY		= 4506, //Aura Monkey

	// Passiva 

	FILTER_MONKEYDEFENSE				= 4487,
	FILTER_FASTSWIMPASSIVE			= 4489,
	FILTER_MOVIMENTODASMAOS			= 4490,
	FILTER_SHATTEREDDREAMMONKEY  	   = 4491, // Teleport
	FILTER_FEMONKEY  	   = 4492, // FE
	FILTER_COMBOMONKEY				= 4493, // Combo das Marcas
	FILTER_COMBOYINYANG		= 4494, // Combo Yin Yang
	FILTER_REDUZDANOADISTANCIA		= 4495, // Reduz a distancia
	FILTER_COMBOYINYANG2		= 4496, // Combo Yin Yang
	FILTER_INVICIBLEMONKEYNEW				= 4497,
	FILTER_INCCRITREDUCE2				= 4498,

	// Novas Habilidades 1.6.6 > 

	FILTER_INCATTACKTODEFMF				= 4499,
	FILTER_INCNEWSHIELDMAGE				= 4500,
	FILTER_INCNEWSHIELDMAGE2				= 4501,
	FILTER_NEWBUFFATIRADORA				= 4502,
	FILTER_NEWBUFFPALADIN				= 4503,
	FILTER_SHATTEREDDREAM2  	   = 4505, // Teleport

	FILTER_ARMADILHAPVP				= 4508,


	// Nova Passiva 

	FILTER_REDUCEDEFENSEPASSIVE				= 4478, 

	// Versos Paladino 

	FILTER_VERSOPALADIN				= 4520,
	FILTER_ARENAGUANYU				= 4521,
	
	FILTER_RETORT3					= 4530,
	FILTER_ELFLOCK					= 4531,
	FILTER_FREECOOLDOWN				= 4532,
	FILTER_SLEEP2					= 4533,
	FILTER_INCHURTMONKEY			= 4534,

	// Novas Runas
	FILTER_SETDOUBLEAPCOST			= 4535,

	// Novas Armas do WR
	FILTER_SETNEWWEAPONADDON		= 4537,
	FILTER_GLYPH_RANGE				= 4538,
	FILTER_GLYPH_DAMAGE				= 4539,
	FILTER_GLYPH_CASTING			= 4540,
	FILTER_GLYPH_CRITICAL			= 4541,
	FILTER_GLYPH_RANGE2				= 4542,
	FILTER_GLYPH_APCOST				= 4543,

	FILTER_APPLYDAMAGEFORRANGE		= 4544,
	FILTER_SHIELDDAMAGEREDUCE		= 4545,
	FILTER_GLYPH_CRITICAL2			= 4546,
	FILTER_GLYPH_JUMPROLLBACK		= 4547,
	FILTER_INVINCIBLEATR			= 4548,

	FILTER_ADDDEFENCEDEGREEATR		= 4550,
	FILTER_ADDATTACKDEGREEATR		= 4551,	

	FILTER_COOLDOWNATR				= 4552,
	FILTER_BARBIANASSAULT			= 4553,
	FILTER_BARBIANJUMPTOTARGET		= 4554,

	FILTER_BARBIANFORBIDASSAULT		= 4555,
	FILTER_BARBIANDECHURT			= 4556,
	FILTER_SHIELDDAMAGEREDUCEBARBIAN	= 4557,
	FILTER_SHEILDTRANSFERDEBUFFBARBIAN	= 4558,

	FILTER_ENHANCESKILLDAMAGEBARBIAN	= 4559,
	FILTER_INVINCIBLEWF					= 4560,

	FILTER_CONTINUOUSDAMAGEWF1			= 4561,
	FILTER_CONTINUOUSDAMAGEWF2			= 4562,
	FILTER_CONTINUOUSDAMAGEWF3			= 4563,
	FILTER_CONTINUOUSDAMAGEWF4			= 4564,
	FILTER_CONTINUOUSDAMAGEWF5			= 4565,

	FILTER_ELMODISABLED					= 4566,
	FILTER_GLYPHINVISIBLE				= 4567,

	FILTER_DEBUFFGLYPHSK1				= 4568,
	FILTER_DEBUFFGLYPHSK2				= 4569,
	FILTER_DEBUFFGLYPHSK3				= 4570,
	FILTER_DEBUFFGLYPHSK4				= 4571,
	FILTER_GLYPH_CRITICAL3				= 4572,
	FILTER_DEATHRESETCD2				= 4573,

	FILTER_FREECLEARDEBUFF				= 4574,
	FILTER_GLYPH_CRITICAL4				= 4575,

	FILTER_IMMUNEROAR	 	   = 4576,
	FILTER_NOCHANGESELECTMOB	= 4577,
	FILTER_DIZZYNOIMMUNE = 4578,
	FILTER_DEATHRESETCDVOIDSTEP = 4579,

	FILTER_GLYPH_APCOST_MULTI				= 4580,

	FILTER_DIZZYONDAMAGE			= 4581,

	FILTER_SETSWORDFORM		= 4582,
	FILTER_SETAXEFORM		= 4583,
	FILTER_SETFISTFORM		= 4584,
	FILTER_SETPOLEARMFORM		= 4585,
	FILTER_IMMUNETRANSFWB	 = 4586,

	FILTER_ADDDEFENCEDEGREE2 = 4587, 
	FILTER_INCDEFENSEDEGREEONDAMAGE = 4588, 
	FILTER_PHYSMAGREDUCWITHSTACKWB = 4589,
	
	FILTER_NEWSTUN = 4590,
	FILTER_SHIELDDAMAGEMYSTIC = 4591,

	FILTER_ANTICLEARBUF2			= 4592,
	FILTER_CONVERTATAQMTOATAQF	= 4593,
	FILTER_CONVERTATAQMTOATAQF2	= 4594,

	FILTER_DECDEFENCENEW      = 4595,

	FILTER_INCDEFENSERESITWITHDAMAGESTART = 4696,
	FILTER_INCDEFENSERESITWITHDAMAGE = 4697,

	FILTER_ELEMENTDAMAGE = 4698,
	FILTER_ELEMENTMETAL = 4699,
	FILTER_ELEMENTWOOD = 4700,
	FILTER_ELEMENTWATER = 4701,
	FILTER_ELEMENTFIRE = 4702,
	FILTER_ELEMENTEARTH = 4703,

	FILTER_NEWSHAPEANDSKILLS = 4704,

	FILTER_SOULBEATBACKWF = 4705,
	FILTER_SOULBEATBACKPALADIN = 4706,
	FILTER_ENHANCESKILLDAMAGEPLAYER = 4707,
	FILTER_INCFARSKILLDMGREDUCE = 4708,

	FILTER_SETINVISIBLEFORCED = 4709,
	FILTER_SETFORCETRANSFORM = 4710,
	FILTER_SETRECOVERYAPPERSEC = 4711,
	FILTER_SETADJUSTDAMAGESENDRECV = 4712,

	FILTER_SETADJUSTHPFROMMP = 4713,
	FILTER_SETINCDAMAGEREDUCEPERC = 4714,
	FILTER_SETIIMMUNENEGATIVEEFFECT = 4715,
	FILTER_SETRECUCENORMALDAMAGE = 4716,
	FILTER_SETSPEEDNOCOMBAT = 4717,
	FILTER_SETADJUSTDAMAGESENDRECV2 = 4718,
	FILTER_SETPLUSDAMAGEFROMHP = 4719,
	FILTER_GUILDREDUCEDEFENSEPASSIVE = 4720,
	FILTER_SETBATTLEPASSFILTER = 4721,

};

enum filter_modifier{
	FMID_ANTIDOTE = 1,              // �ⶾһ��
    FMID_COMPLETE_TRAVEL = 2,       //��������
    FMID_SPEEDUP_FLY = 3,
    FMID_NORMAL_FLY = 4,  
    FMID_DUEL_END = 5,    
    FMID_CLEAR_AEBF = 6,
    FMID_PET_HONOR = 7,
    FMID_CLEAR_AEFF = 8,
    FMID_CLEAR_AECB = 9,
	FMID_CLEAR_AETB = 10,
	FMID_DEC_ANTICLEAR = 11,
	FMID_CRIT_FEEDBACK = 12,
	FMID_CLEAR_AEMF    = 13,
	FMID_CLEAR_AEAA    = 14,
	FMID_SKILLMAX = 1024, 
};
#endif
