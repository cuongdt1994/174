

class OOG
{
private:	
	
enum
	{
	OOG_MAX_LIMIT = 9400u,			//Максимальный лимит ООГ пакетов
	OOG_TIMER_LIMIT = 310u,			//Уменьшение ООГ лимита по таймеру
	OOG_LOW_LIMIT = 2u,				//Сколько добавлять в лимит за легкий пакет
	OOG_HORMAL_LIMIT = 10u,			//Сколько добавлять в лимит за средний пакет
	OOG_HIGHT_LIMIT = 40u,			//Сколько добавлять в лимит за тяжелый пакет
	OOG_HIGHT_COUNT = 128u,			//Количество тяжелых пакетов
	OOG_MAX_LIMIT_KICKOUT = 44000u, //Лимит при котором кикать игрока
	};
	
	struct
	{
		int type;
		size_t load_state;
	} pool[OOG_HIGHT_COUNT];	

public:	

	void Init();
	int Check(int type, size_t & load_state);
	void HeartBeat(size_t & load_state);

	static OOG * GetInstance()
	{
		if (!instance)
		instance = new OOG();
		return instance;
	}
	static OOG * instance;
	
};









