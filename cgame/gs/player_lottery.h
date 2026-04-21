#ifndef __PLAYER_LOTTERY_H__
#define __PLAYER_LOTTERY_H__

class TreasureConfig
{
private:
	enum 
	{
		MAX_LOT = 100,
		MAX_COST = 10,
	};
private:
	struct LOT
	{
		float prob;
		unsigned int id;
		unsigned int count;
		unsigned int rank;
	};
	
	struct REQ
	{
		unsigned int id;
		unsigned int count;
		unsigned int cost[MAX_COST];
	};
	
private:
	bool enable; //Ligar ~ Desligar
	REQ req;
	LOT lot[MAX_LOT];
public:	
	void Init();
	bool IsEnable() { return enable; }
	bool GetItem(unsigned int & id, unsigned int & count, unsigned int & rank);
	unsigned int GetReqItem() { return req.id; }
	unsigned int GetReqCount() { return req.count; }
	unsigned int GetReqCost( unsigned int open_lots ) { return open_lots < MAX_COST ? req.cost[open_lots] : -1; }
	
	static TreasureConfig * GetInstance()
	{
		if (!instance)
		instance = new TreasureConfig();
		return instance;
	}
	static TreasureConfig * instance;
};

//---------------------------------------------------------------------

class LotteryConfig
{
private:
	enum 
	{
		MAX_AWARD = 10,
		MAX_PROB = 4,
		MAX_RANK = 5,
		MAX_LOT = 30,
	};
private:	
	struct AWARD
	{
		unsigned int id;	
		unsigned int score;
		unsigned int count;	
	};
	
	struct LOT
	{
		float prob;
		unsigned int id;
		unsigned int count;
		unsigned int rank;
	};
private:
	bool enable; //Ligar desligar
	float prob[MAX_PROB]; //Chances de queda de cartas
	AWARD award[MAX_AWARD]; //recompensas de pontos de sorte
	LOT lot[MAX_RANK][MAX_LOT]; //prêmios para cartões participantes da loteria
public:
	void Init();
	bool IsEnable() { return enable; }
	unsigned int GetProb();
	bool GetAward( unsigned int score, unsigned int & level, unsigned int & item, unsigned int & count );
	bool GetLot( unsigned int i, unsigned int & id, unsigned int & count, unsigned int & rank );

	static LotteryConfig * GetInstance()
	{
		if (!instance)
		instance = new LotteryConfig();
		return instance;
	}
	static LotteryConfig * instance;
};

//---------------------------------------------------------------------

class gplayer_lottery
{
public:
	enum 
	{
		MAX_LOT = 5,
		MAX_RANK = 5,
		
		BLUE_LOT = 0,
		WHITE_LOT = 1,
		VIO_LOT = 2,
		GOLT_LOT = 3,
		RED_LOT = 4,
		
		COOLDOWN_IDX = 7140,
		COOLDOWN_TIME = 500,
	};
private:
	struct LOT
	{
		unsigned int id; // ID do item // 0 não aberto
		unsigned int rank_item; //aberto 0 - cinza, 1 - azul, 2 violeta, 3 dourado, 4 laranja, 5 vermelho
		unsigned int rank_lot; //fechar 1 - cinza, 0 - azul
		unsigned int is_open; //0 fechado, 1 aberto
		unsigned int count;
	};
private:
	unsigned int level;	//Nível de sorte, para verificar a emissão de uma recompensa
	unsigned int manager; //2 gerente número 1 - baús, 2 cartões
	unsigned int score; //Número de pontos
	unsigned int count; //Número de ítens
	LOT lot[MAX_LOT]; //Cartões de loteria
public:	
	bool Init();
	bool AddLot(unsigned int rank_lot, unsigned int pos);
	bool StartX1();
	bool StartX5();
	void OpenLot(unsigned int i, unsigned int & item_id, unsigned int & item_count);
	bool IsStart();
	void GetInfo(unsigned int & manager, unsigned int & score, unsigned int & count, void * buf);
	unsigned int GetLotCount();
	unsigned int GetLotRank(unsigned int i);
	unsigned int GetLevel() { return level; }
	void SetLevel(unsigned int v) { level = v; }
	unsigned int GetScore() { return score; }
	void SetScore(unsigned int v) { score = v; }
};

//---------------------------------------------------------------------

class gplayer_treasure
{
public:
	enum 
	{
		MAX_LOT = 10,
		
		COOLDOWN_IDX = 7141,
		COOLDOWN_TIME = 500,
	};
private:
	struct LOT
	{
		unsigned int id; //id do item
		unsigned int count; //número de ítens
		unsigned int box; //número do peito à esquerda
		unsigned int lot; //número do peito à direita
		unsigned int rank; //classificação do item (usado pelo cliente)
		unsigned int is_open; //é o peito aberto
	};
private:
	unsigned char manager; //gerente (0 apenas informações, 1 caça ao tesouro iniciada, 2 tesouros abertos
	unsigned int score; //pontos de sorte
	unsigned int free_count; //tentativas grátis
	unsigned int lot_count; //número de lotes fechados
	unsigned int box_count; //número de lotes abertos
	unsigned int num_open; //máscara de fila de itens abertos
	LOT lot[MAX_LOT];
public:		
	void Init();
	void InitLot(unsigned int _free_count);
	void RebuildLot();
	void OpenLot(unsigned int param, unsigned int & item_id, unsigned int & item_count);
	void GetInfo(unsigned char & _manager, unsigned int & _score, unsigned int & _reserve, unsigned int & _lot_count, unsigned int & _box_count, void * __lot, void * __box);
	unsigned int GetOpenLotCount() { return box_count; }
	unsigned int GetManager() { return manager; }
	void SetManager( unsigned char mgr ) { manager = mgr; }
	unsigned int GetScore() { return score; }
	void SetScore( unsigned int v ) { score = v; }
	unsigned int GetFreeCount() { return free_count; }
};

//---------------------------------------------------------------------

class gplayer_lottery_items
{
public:
	enum 
	{
		MAX_LOT = MAX_SLOT_LOTERY_STORAGE
	};
private:	
	struct ITEM
	{
		unsigned int id;
		unsigned int count;
	};
private:	
	unsigned int count;
	ITEM item[MAX_LOT];
public:	
	void Init();
	unsigned int Rebuild();
	unsigned int Add(unsigned int id , unsigned int count);
	unsigned int Del(unsigned int id);
	unsigned int GetCleanSlot();
	unsigned int GetCountItems();
	void GetInfo( unsigned int & cnt , void * buf );
	
};

//---------------------------------------------------------------------





#endif