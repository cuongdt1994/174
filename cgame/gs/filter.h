#ifndef __ONLINEGAME_GS_FILTER_H__
#define __ONLINEGAME_GS_FILTER_H__

#include <ASSERT.h>
#include <common/base_wrapper.h>
#include "attack.h"
#include "property.h"
#include "substance.h"
#include "obj_interface.h"

class filter : public substance
{
protected:
	int _mask;	//������������
	int _filter_id;	//filter ��ID
	bool _active;
	bool _is_deleted;	//�Ƿ���Ϊɾ��
	object_interface _parent;
	
	bool IsDeleted() { return _is_deleted;}
public:
DECLARE_SUBSTANCE(filter);
	enum
	{	
		FILTER_MASK_TRANSLATE_SEND_MSG  = 0x0001,
		FILTER_MASK_TRANSLATE_RECV_MSG	= 0x0002,
		FILTER_MASK_HEARTBEAT		= 0x0004,
		FILTER_MASK_ADJUST_DAMAGE	= 0x0008,
		FILTER_MASK_DO_DAMAGE		= 0x0010,
		FILTER_MASK_ADJUST_EXP		= 0x0020,
		FILTER_MASK_ADJUST_MANA_COST	= 0x0040,
		FILTER_MASK_BEFORE_DEATH	= 0x0080,
		FILTER_MASK_TRANSLATE_ENCHANT 	= 0x0100,
		FILTER_MASK_TRANSLATE_SEND_ENCHANT= 0x0200,
		FILTER_MASK_ADJUST_HEAL		= 0x0400,
		FILTER_MASK_DO_ENCHANT		= 0x0800,
		FILTER_MASK_ALL			= 0x0FFF,


		FILTER_MASK_DEBUFF		= 0x001000,
		FILTER_MASK_BUFF		= 0x002000,
		FILTER_MASK_UNIQUE		= 0x004000,		//����ԭ��������ͬfilter_id  filter
		FILTER_MASK_WEAK		= 0x008000,		//���ԭ������ͬ��filter_id  �򲻽��м���
		FILTER_MASK_REMOVE_ON_DEATH	= 0x010000,		//����ʱ�ᱻ�Զ�ɾ��
		FILTER_MASK_MERGE		= 0x020000,		//���������һ�µ�filter_id  ����ͼ�ں�
		FILTER_MASK_SKILL1		= 0x040000,
		FILTER_MASK_SKILL2		= 0x080000,
		FILTER_MASK_SKILL3		= 0x100000,
		FILTER_MASK_SAVE_DB_DATA	= 0x200000,		//�Ƿ���Ҫ���浽���ݿ���
		FILTER_MASK_NOSAVE		= 0x400000,		//ת�Ʒ�����ʱ������
		FILTER_MASK_TRANSFERABLE_BUFF = 0x00800000,
		FILTER_MASK_TRANSFERABLE_DEBUFF = 0x01000000,
		FILTER_MASK_BLACK_STUN = 0x02000000,
		FILTER_MASK_DEBUFF2 = 0x04000000,
	};

	enum
	{	
		FILTER_QUERY_LEVEL,
        FILTER_QUERY_DAMAGE,
	};
	
	int GetMask() { return _mask; }
	int GetFilterID() { return _filter_id; }
	virtual bool Save(archive & ar)
	{
		ar << _mask << _filter_id << _active << _is_deleted;
		return true;
	}

	virtual bool Load(archive & ar)
	{
		ar >> _mask >> _filter_id >> _active >> _is_deleted;
		return true;
	}
private:
	/*
	*	filter�н������õĺ���
	*	��Щ���������ú�������ط�0������ڵ��ú�ɾ�� �ͷ�
	*/
	virtual void TranslateSendAttack(const XID & target,attack_msg & msg){ASSERT(false);}	//�ڷ���������Ϣ֮ǰ����һ����Ϣ����
	virtual void TranslateRecvAttack(const XID & attacker,attack_msg & msg){ASSERT(false);}	//�ڽ��յ���Ϣ�����ȴ���һ��
	virtual void TranslateEnchant(const XID & attacker,enchant_msg & msg){ASSERT(false);}	//�ڽ��յ���Ϣ�����ȴ���һ��
	virtual void TranslateSendEnchant(const XID & attacker,enchant_msg & msg){ASSERT(false);}//�ڷ���enchant����Ϣ֮ǰ
	virtual void Heartbeat(int tick){ASSERT(false);}			//������ʱ������,tick��ʾ���μ������
	virtual void AdjustDamage(damage_entry & dmg,const XID & attacker, const attack_msg & msg,float damage_adjust){ASSERT(false);}		//�����˺�֮ǰ����һ��
	virtual void DoDamage(const damage_entry & dmg){ASSERT(false);}		//�����յ��˺�������Ӱ���������
	virtual void AdjustExp(int type, int & exp){ASSERT(false);}		//�Ծ���ֵ��������
	virtual void AdjustManaCost(int &mana){ASSERT(false);}			//�ںķ�manaǰ������
	virtual void BeforeDeath(const XID & attacker, char attacker_mode){ASSERT(false);}				//�ڵ���������OnDeathǰ����
	virtual void AdjustHeal(int& heal,char heal_type){ASSERT(false);}
	virtual void DoEnchant(const XID & attacker,enchant_msg & msg){ASSERT(false);}

	virtual void Merge(filter * f) { ASSERT(false);}
	virtual void OnAttach() = 0;
	virtual void OnRelease() {}
	void Release() 
	{
		OnRelease();
		delete this;
	}
	
	virtual void  OnModify(int ctrlname,void * ctrlval,unsigned int ctrllen) { }
	virtual int   OnQuery(int index) { return index == FILTER_QUERY_LEVEL ? 1 : 0;} //��ѯ�ȼ�Ĭ�Ϸ���1
protected:
	filter(object_interface parent,int mask):_mask(mask),_filter_id(0),
						 _active(false),_is_deleted(false),_parent(parent)
	{}
	virtual ~filter(){};
	inline void  Modify(int ctrlname,void * ctrlval,unsigned int ctrllen) 
	{
		OnModify(ctrlname,ctrlval,ctrllen);
	}
	friend class filter_man;
	filter(){}
};

class timeout_filter : public filter
{

protected:
	int _timeout;
	timeout_filter(object_interface object,int timeout,int mask)
			:filter(object,mask),_timeout(timeout)
	{
		ASSERT(mask & FILTER_MASK_HEARTBEAT);
	}

	virtual bool Save(archive & ar)
	{
		filter::Save(ar);
		ar << _timeout;
		return true;
	}

	virtual bool Load(archive & ar)
	{
		filter::Load(ar);
		ar >> _timeout;
		return true;
	}

	timeout_filter(){}

	static inline int GetTimeOut(timeout_filter * rhs)
	{
		return rhs->_timeout;
	}

protected:
	virtual void Heartbeat(int tick)
	{
		_timeout -= tick;
		if(_timeout <=0) _is_deleted = true;
	}
};


//for filter_man
enum
{
	FILTER_IDX_TRANSLATE_SEND_MSG 	,
	FILTER_IDX_TRANSLATE_RECV_MSG	,
	FILTER_IDX_HEARTBEAT		,
	FILTER_IDX_ADJUST_DAMAGE	,
	FILTER_IDX_DO_DAMAGE		,
	FILTER_IDX_ADJUST_EXP		,
	FILTER_IDX_ADJUST_MANA_COST	,
	FILTER_IDX_BEFORE_DEATH		,
	FILTER_IDX_TRANSLATE_ENCHANT	,
	FILTER_IDX_TRANSLATE_SEND_ENCHANT,
	FILTER_IDX_ADJUST_HEAL,
	FILTER_IDX_DO_ENCHANT,

	FILTER_IDX_MAX	
};

#endif
