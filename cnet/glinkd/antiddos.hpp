#ifndef __GNET_ANTIDDOS_HPP
#define __GNET_ANTIDDOS_HPP

//------------------------------------------------------------------------------------------------------------------------
//--ANTIDDOS (C) 2021 deadraky
//------------------------------------------------------------------------------------------------------------------------

class AntiDDoS
{
private:

	enum
	{
		HEARTBEAT_TIMER = 10u,			//таймер в течении которого обновляются фильтры
		MAX_RAND_POOL = 256u,			//количество рандомных переменных
		L7_MAX_CONNECT = 2048u,			//Максимальное количество коннектов
		L7_MAX_IP = 128u,				//Максимальное количество одновременных соединений
		L7_EVENT = 6u,					//Обновление таймера в секундах
	};

	struct L7_POOL
	{
		size_t timer;
		size_t cnt;
		size_t max_cnt;

		struct
		{
			size_t cnt;
			int table[MAX_RAND_POOL];
			
			void Init()
			{
				cnt = 0;
				for (size_t i = 0;i < MAX_RAND_POOL; i++)
				{
					table[i] = rand();
				}
			}
			
			inline int Get()
			{
				return table[cnt++ % MAX_RAND_POOL];
			}
		}	rand_pool;

		
		inline void init()
		{
			rand_pool.Init();
			timer = 0;
			cnt = 0;
			max_cnt = 0;
		}

		inline void heartbeat()
		{
			if(--timer > HEARTBEAT_TIMER)
			{
				timer = ((rand_pool.Get() % L7_EVENT) + 1u);
				cnt = ((rand_pool.Get() % L7_MAX_IP) / 3 + 1u);
				max_cnt = ((rand_pool.Get() % L7_MAX_CONNECT) / 3 + 1u);
			}
		}
		
		inline bool add() //set transport
		{
			return (++cnt > ((rand_pool.Get() % (L7_MAX_IP / 2)) + (L7_MAX_IP / 2)));
		}
		
		inline bool add_max() //add session
		{
			return (++max_cnt > L7_MAX_CONNECT);
		}
		
		inline void sub() //online anounce 
		{
			if(cnt > 0) --cnt;
		}
		
		inline void sub_max() //del session
		{
			if(max_cnt > 0) --max_cnt;
		}
		
		inline void log()
		{
			printf("POOL[L7]LOG: timer = %lld , cnt = %lld , max_cnt = %lld \n", timer, cnt, max_cnt);
		}
		
	};
	
	L7_POOL l7_pool;

public:

	void L7Init();
	bool L7SetTransport(const char *ip);
	void L7OnlineAnounce(const char *ip);
	bool L7AddSession();
	void L7DelSession();
	void L7Timer();
	
	static AntiDDoS * GetInstance()
	{
		if (!instance)
		instance = new AntiDDoS();
		return instance;
	}
	static AntiDDoS * instance;
};



#endif

