#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_qiling.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"

DEFINE_SUBSTANCE(qiling_item,item_body,CLS_ITEM_QILING)

void qiling_item::OnRefreshItem()
{
	_raw_data.clear();
	raw_wrapper rw;
	Save(rw);
	rw.swap(_raw_data);

	CalcCRC();
}

bool qiling_item::Save(archive &ar)
{
	try
	{
		ar << _ess;		
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool qiling_item::Load(archive &ar)
{
	ASSERT(_tid > 0);
	ASSERT(ar.offset() == 0);

	_raw_data.clear();
	_raw_data.push_back(ar.data(), ar.size());

	try
	{
		ar >> _ess;
	}
	catch (...)
	{
		return false;
	}

	CalcCRC();
	return true;
}
