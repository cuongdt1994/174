    /*
 * FILE: SpatialPathFinding.cpp
 *
 * DESCRIPTION:   a class which provides a engine for full-3D spatial path finding.
 *							We use Best-First Search algorithm to implement path finding.
 *							The passable map of 3D space is described by CompactSpacePassableOctree!
 *
 * CREATED BY:	  He wenfeng, 2005/10/11
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "SpatialPathFinding.h"
#include "GlobalSPMap.h"
#include "NPCMoveMap.h"
#include "GlobalWaterAreaMap.h"
#include "NPCMove.h"
#include "NPCChaseSpatiallyPFAgent.h"

CSpatialPathFinding::CSpatialPathFinding(NPCMoveMap::CMap * pMap):m_pMap(pMap)
{

}

CSpatialPathFinding::~CSpatialPathFinding()
{
	Release();
}


void CSpatialPathFinding::Init(const Pos3DInt& posStart,const Pos3DInt& posGoal, int iVoxelSize)
{
	Release();

	m_posStart = posStart;
	m_posGoal = posGoal;
	m_iVoxelSize = iVoxelSize;

	m_bSearchOver = false;
	m_bFoundPath = false;
	
	CSPOctreeTravNode StartOctrTravNode;
	// Verify the start and goal pos are passable
	if(!(ExtraPassableTest(posStart) && ExtraPassableTest(posGoal) &&
		 //CGlobalSPMap::GetInstance()->IsPosPassable(posStart,StartOctrTravNode) &&
		 m_pMap->IsPosPassable(posStart,StartOctrTravNode) &&
		 //CGlobalSPMap::GetInstance()->IsPosPassable(posGoal,m_GoalOctrTravNode,&StartOctrTravNode)))
		 m_pMap->IsPosPassable(posGoal,m_GoalOctrTravNode,&StartOctrTravNode)))
	{
		m_bSearchOver = true;		// searching is over, but no path founded!
		m_bFoundPath = false;		
		return;
	}
	
	// Init A* with start node (g=0)
	CSPOctreeTravNode *pOctrTravNode = new CSPOctreeTravNode(StartOctrTravNode);
	m_Open.SortPush(posStart, 0, GetOctileDist3D(posStart, m_posGoal), pOctrTravNode, NULL);

	m_iVoxelsSearched = 0;
}

void CSpatialPathFinding::StepBestFirstSearch(int iSearchVoxels)
{
	if(m_bSearchOver) return;

	int iCounter = 0;
	PtrSpatialPathNode pPathNode;
	Pos3DInt posNeighbor;
	CSPOctreeTravNode NeighborOctrTravNode;
	int CurH, MinH = 0;
	Pos3DInt posMinH;
	bool bFoundNearGoal;

	while (!m_Open.empty() && iCounter < iSearchVoxels && m_iVoxelsSearched < MAX_SEARCH_VOXEL_NUM)
	{
		pPathNode = m_Open.PopFront();

		if(pPathNode->pos == m_posGoal)
		{
			m_bFoundPath = true;
			BuildPath(pPathNode);
			delete pPathNode;
			m_bSearchOver = true;
			return;
		}

		++iCounter;
		++m_iVoxelsSearched;

		bFoundNearGoal = false;

		for(int i = 0; i < 27; ++i)
		{
			if(i == 13) continue; // skip self

			int dx = i / 9 - 1;
			int dy = (i % 9) / 3 - 1;
			int dz = i % 3 - 1;

			posNeighbor.x = pPathNode->pos.x + dx * m_iVoxelSize;
			posNeighbor.y = pPathNode->pos.y + dy * m_iVoxelSize;
			posNeighbor.z = pPathNode->pos.z + dz * m_iVoxelSize;

			if(!ExtraPassableTest(posNeighbor)) continue;

			// g cost: cardinal=10, face-diagonal=14, space-diagonal=17
			int nonzero = (dx != 0) + (dy != 0) + (dz != 0);
			int stepCost = (nonzero == 1) ? 10 : (nonzero == 2) ? 14 : 17;
			int newG = pPathNode->g + stepCost;

			// Octree shortcut: neighbor is inside the goal's voxel — can reach goal directly
			if(m_GoalOctrTravNode.IsPosInside(posNeighbor))
			{
				CurH = GetOctileDist3D(posNeighbor, m_posGoal);
				if(!bFoundNearGoal || CurH < MinH)
				{
					MinH = CurH;
					posMinH = posNeighbor;
				}
				bFoundNearGoal = true;
				continue;
			}

			bool bPosPassable = m_pMap->IsPosPassable(posNeighbor, NeighborOctrTravNode, pPathNode->pOctrTravNode);
			if(!bPosPassable) continue;

			// Octree shortcut: neighbor is a sibling of the goal's voxel node
			if(m_GoalOctrTravNode.IsNodeNeighborSibling(NeighborOctrTravNode))
			{
				CurH = GetOctileDist3D(posNeighbor, m_posGoal);
				if(!bFoundNearGoal || CurH < MinH)
				{
					MinH = CurH;
					posMinH = posNeighbor;
				}
				bFoundNearGoal = true;
				continue;
			}

			if(m_Close.FindByPos(posNeighbor)) continue;

			int h = GetOctileDist3D(posNeighbor, m_posGoal);

			// Update if already in open with worse cost; otherwise push new node
			if(!m_Open.UpdateIfBetter(posNeighbor, newG, h, pPathNode))
			{
				CSPOctreeTravNode* pNeighborOctrTravNode = new CSPOctreeTravNode(NeighborOctrTravNode);
				m_Open.SortPush(posNeighbor, newG, h, pNeighborOctrTravNode, pPathNode);
			}
		}

		if(bFoundNearGoal)
		{
			m_bFoundPath = true;
			BuildPath(pPathNode);
			if(m_PathFound.empty() || !(m_PathFound.back() == m_posGoal))
			{
				m_PathFound.push_back(posMinH);
				m_PathFound.push_back(m_posGoal);
			}
			delete pPathNode;
			m_bSearchOver = true;
			return;
		}

		m_Close.push_back(pPathNode);
	}

	if(m_Open.empty() && !m_bFoundPath)
		m_bSearchOver = true;
}

void CSpatialPathFinding::BuildPath(PtrSpatialPathNode pPathNode)
{
	VecPos3DInt InvPath;
	while(pPathNode)
	{
		InvPath.push_back(pPathNode->pos);
		pPathNode = pPathNode->pLastPathNode;
	}

	while(!InvPath.empty())
	{

		m_PathFound.push_back(InvPath.back());
		InvPath.pop_back();
	}
}

bool CAerialPathFinding::ExtraPassableTest(const Pos3DInt& pos)
{
	// Verify the pos must be above the terrain and water
	return CNPCChaseSpatiallyPFAgent::IsPosOnAir(A3DVECTOR3(pos.x,pos.y,pos.z), m_pMap);
}

bool CUnderwaterPathFinding::ExtraPassableTest(const Pos3DInt& pos)
{
	// Verify the pos must be above the terrain while under the water
	return CNPCChaseSpatiallyPFAgent::IsPosInWater(A3DVECTOR3(pos.x,pos.y,pos.z), m_pMap);
}
