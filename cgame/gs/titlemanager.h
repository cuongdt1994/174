#ifndef __ONLINEGAME_GS_TITLE_MANAGER_H__
#define __ONLINEGAME_GS_TITLE_MANAGER_H__
#include <hashtab.h>
#include <unordered_map>
#include <amemory.h>

typedef unsigned short TITLE_ID;
typedef abase::pair<TITLE_ID,int> TITLE_EXPIRE;

class gplayer_imp;
class itemdataman;
class title_essence
{
public:
	title_essence(int pd,int md,int pdf,int mdf,int at,int ar,int hp,bool ra)
		: _phy_damage(pd), _magic_damage(md), _phy_defence(pdf),_magic_defence(mdf),
		  _attack(at), _armor(ar), _hp(hp), _rare(ra) {}
	~title_essence() {}

public:
	void 	ActivateEssence(gplayer_imp *pImp) const ;
	void 	DeactivateEssence(gplayer_imp *pImp) const;
	bool	IsRare() const { return _rare; }
private:
	int		_phy_damage;		//	�����﹥
	int		_magic_damage;		//	���ӷ���
	int		_phy_defence;		//	�������
	int		_magic_defence;		//	���ӷ���
	int		_attack;			//	��������
	int		_armor;				//	��������
	int		_hp;				//	Novo
	bool	_rare;				//	�Ƿ�㲥(type=bool)
};

class player_title;
class title_manager
{
public:
	~title_manager()
	{
		TITLE_MAP::iterator iter = _title_map.begin();
		TITLE_MAP::iterator iend = _title_map.end();
		
		for(;iter!= iend; ++iter)
		{
			delete iter->second;
		}

		_title_map.clear();
		_sub_title_map.clear();
		_super_title_cond_map.clear();
	}
public:
	bool InitTitle(itemdataman & dataman);

	const title_essence* GetTitleEssence(TITLE_ID tid)	
	{
		TITLE_MAP::iterator iter = _title_map.find(tid);
		return iter == _title_map.end() ? NULL : iter->second;
	}

	bool TouchSuperTitle(player_title* pt,TITLE_ID tid);

	bool IsVailidTitle(TITLE_ID tid) 
	{ 
		return _title_map.find(tid) != _title_map.end();
	}

	bool IsSubTitle(TITLE_ID tid) 
	{ 
		return _sub_title_map.find(tid) != _sub_title_map.end(); 
	}
 
	void GiveAllTittle(player_title* pt);
protected:
	bool CheckSubTitleValid();  // ����ӳƺ��Ƿ����
private:
	typedef std::unordered_map<TITLE_ID, title_essence*, abase::_hash_function > TITLE_MAP;
	TITLE_MAP _title_map;
	
	typedef std::vector<TITLE_ID > SUPER_TITLE_LIST;
	typedef std::unordered_map<TITLE_ID, SUPER_TITLE_LIST, abase::_hash_function > SUB_TITLE_MAP;
	SUB_TITLE_MAP _sub_title_map;

	static const int SUB_TITLE_COUNT = 20;
	struct super_title_cond{
		unsigned int	sub_titles[SUB_TITLE_COUNT];
	};
	typedef std::unordered_map<TITLE_ID, super_title_cond, abase::_hash_function > SUPER_TITLE_COND_MAP;
	SUPER_TITLE_COND_MAP _super_title_cond_map;
};

#endif

