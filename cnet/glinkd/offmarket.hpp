//------------------------------------------------------------------------------------------------------------------------
//--Offline Market (C) 2021 deadraky
//------------------------------------------------------------------------------------------------------------------------

class OfflineMarket
{
public:
	enum
	{
		CMD_OFFLINE_ROLE = 721,
		CMD_OFFLINE_MARKET = 722,
		SELF_OPEN_MARKET = 167,
		PLAYER_CANCEL_MARKET = 168,
		PLAYER_MARKET_INFO = 169,
		MIN_ROLEID = 1024,
		MAX_ITEMS_MARKET = 32,
	};
private:	
	#pragma pack(push, 1)
	
	union  market_goods
	{
		struct 
		{
			int type;		//物品类型  如果是0 表示没有内容了
		}empty_item;
		
		struct 
		{
			int type;		//物品类型
			int count;		//剩余多少个 负数表示是买
			unsigned int  price;		//单价
			unsigned short content_length;
			char content[];
		}item;

		struct 
		{
			int type;		//物品类型
			int count;		//剩余多少个 负数表示是买
			unsigned int  price;		//单价
		}order_item;
		
	};
	
	struct player_cancel_market
	{
		short cmd;
		int pid;
	};
	
	struct self_open_market
	{
		short cmd;
		unsigned short count;
		struct 
		{
			int type;		//物品类型
			unsigned short index;	//如果是0xFFFF，表示是购买
			unsigned int count;	//卖多少个
			unsigned int  price;		//单价
		} item_list[];
		
	};
	
	struct player_market_info
	{
		short cmd;
		int pid;
		int market_id;
		unsigned int  count;
		int item_list[];
		//market_goods item_list[];
	};
	
	#pragma pack(pop)

public:

	void Cancel(void * data, size_t size);
	void Open(void * data, size_t size, int sid);
	void Info(void * data, size_t size);

	
	static OfflineMarket * GetInstance()
	{
		if (!instance)
		instance = new OfflineMarket();
		return instance;
	}
	static OfflineMarket * instance;
};


