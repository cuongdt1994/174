/************************************************************************\
 *	SortVectorPathNode.h
 *
 *		Upgraded 2026: A* priority queue replacing the original BFS
 *		sorted vector. Nodes now carry g (actual cost) and f = g + h,
 *		open list is sorted ascending by f (lowest f = best candidate).
 *		Pool-safe: ClearNoDelete() empties the list without freeing nodes
 *		so the per-agent node pool can be reset independently.
 \************************************************************************/

#ifndef _SORTVECTORPATHNODE_H_
#define _SORTVECTORPATHNODE_H_

struct PathNode
{
	int u;
	int v;
	int g;				// actual cost from start
	int h;				// heuristic cost to goal
	int f;				// f = g + h  (sort key)
	PathNode* pLastNode;
};

typedef PathNode* LPPathNode;

#include <vector.h>
using namespace abase;

typedef vector<LPPathNode> vecLPPN;

class CSortVectorPathNode : public vecLPPN
{
public:

	CSortVectorPathNode() {}
	~CSortVectorPathNode() {}

	// Insert in ascending f-order (lowest f at front = best candidate)
	bool SortPush(LPPathNode lpPN)
	{
		for (vecLPPN::iterator it = begin(); it != end(); it++)
		{
			if (lpPN->f < (*it)->f)
			{
				insert(it, lpPN);
				return true;
			}
		}
		push_back(lpPN);
		return true;
	}

	LPPathNode PopFront()
	{
		if (empty()) return NULL;
		LPPathNode lpPN = front();
		erase(begin());
		return lpPN;
	}

	LPPathNode FindByPos(int u, int v)
	{
		for (vecLPPN::iterator it = begin(); it != end(); it++)
		{
			if ((*it)->u == u && (*it)->v == v)
				return *it;
		}
		return NULL;
	}

	// Remove a specific node pointer from the list (used when updating f-cost)
	bool Remove(LPPathNode lpPN)
	{
		for (vecLPPN::iterator it = begin(); it != end(); it++)
		{
			if (*it == lpPN)
			{
				erase(it);
				return true;
			}
		}
		return false;
	}

	// Free heap-allocated nodes (use only when nodes were new'd directly)
	void Release()
	{
		for (vecLPPN::iterator it = begin(); it != end(); it++)
			delete (*it);
		clear();
	}

	// Clear without freeing — for use with the node pool allocator
	void ClearNoDelete() { clear(); }
};

#endif
