  /*
 * FILE: SortVectorSpatialPathNode.h
 *
 * DESCRIPTION:		a set of classes which implement a sorted vector of 3D Path Node, which
 *					can be used as the prototype of Open list of BFS algorithm
 *
 * CREATED BY:		He wenfeng, 2005/10/11
 *
 * HISTORY:
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#ifndef	_SORTVECTORSPATIALPATHNODE_H_
#define _SORTVECTORSPATIALPATHNODE_H_

#include "CompactSpacePassableOctree.h"
using namespace SPOctree;

#include <vector.h>
using namespace abase;

struct SpatialPathNode
{
	SpatialPathNode()
	{
		pOctrTravNode = NULL;
		pLastPathNode = NULL;
	}

	SpatialPathNode(const Pos3DInt& posPara, int gPara, int hPara, CSPOctreeTravNode* pOctrTravNodePara, SpatialPathNode *pLastPathNodePara)
		: pos(posPara), g(gPara), h(hPara), f(gPara + hPara)
	{
		pOctrTravNode = pOctrTravNodePara;
		pLastPathNode = pLastPathNodePara;
	}

	~SpatialPathNode()
	{
		if(pOctrTravNode)
			delete pOctrTravNode;
	}

	Pos3DInt pos;
	int g;   // actual cost from start
	int h;   // heuristic cost to goal
	int f;   // f = g + h (sort key)

	CSPOctreeTravNode * pOctrTravNode;
	SpatialPathNode * pLastPathNode;
};
typedef SpatialPathNode* PtrSpatialPathNode;
typedef vector<PtrSpatialPathNode> VecPtrSpatialPathNode;

// Sorted vector of 3D spatial path-finding node (A* open list).
// Sorted by f = g + h (ascending); no hard cap on size.
class CSortVectorSpatialPathNode: public VecPtrSpatialPathNode
{
public:

	bool SortPush(const Pos3DInt& pos, int g, int h, CSPOctreeTravNode* pOctrTravNode, SpatialPathNode *pLastPathNode)
	{
		int f = g + h;
		PtrSpatialPathNode pNewPathNode = new SpatialPathNode(pos, g, h, pOctrTravNode, pLastPathNode);
		for(VecPtrSpatialPathNode::iterator it = begin(); it != end(); ++it)
		{
			if(f < (*it)->f)
			{
				insert(it, pNewPathNode);
				return true;
			}
		}
		push_back(pNewPathNode);
		return true;
	}

	PtrSpatialPathNode PopFront()
	{
		if(empty()) return NULL;
		PtrSpatialPathNode pPathNode = front();
		erase(begin());
		return pPathNode;
	}

	PtrSpatialPathNode FindByPos(const Pos3DInt& pos)
	{
		for(VecPtrSpatialPathNode::iterator it = begin(); it != end(); ++it)
		{
			if((*it)->pos == pos)
				return *it;
		}
		return NULL;
	}

	// Update cost if new path is cheaper; returns true if node was found (whether updated or not).
	bool UpdateIfBetter(const Pos3DInt& pos, int newG, int newH, SpatialPathNode* pParent)
	{
		int newF = newG + newH;
		for(VecPtrSpatialPathNode::iterator it = begin(); it != end(); ++it)
		{
			if((*it)->pos == pos)
			{
				if(newF < (*it)->f)
				{
					(*it)->g = newG;
					(*it)->h = newH;
					(*it)->f = newF;
					(*it)->pLastPathNode = pParent;
					// re-sort: remove then re-insert at correct position
					PtrSpatialPathNode pNode = *it;
					erase(it);
					for(VecPtrSpatialPathNode::iterator jt = begin(); jt != end(); ++jt)
					{
						if(newF < (*jt)->f)
						{
							insert(jt, pNode);
							return true;
						}
					}
					push_back(pNode);
				}
				return true;
			}
		}
		return false;
	}

	void Release()
	{
		for(VecPtrSpatialPathNode::iterator it = begin(); it != end(); ++it)
		{
			delete (*it);
		}
		clear();
	}
};

#endif
