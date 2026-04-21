 /*
 * FILE: NPCChaseOnGroundAgent.h
 *
 * DESCRIPTION:   On-ground NPC chasing movement agent.
 *                Upgraded 2026: true A* algorithm, per-agent node pool,
 *                line-of-sight path smoothing, and repath-on-block.
 *
 * CREATED BY: He wenfeng, 2005/4/18
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#ifndef _NPCCHASEONGROUNDAGENT_H_
#define _NPCCHASEONGROUNDAGENT_H_

#include <math.h>
#include "NPCMove.h"
#include "NPCMoveMap.h"
#include "SortVectorPathNode.h"

typedef vector<POS2D> vecPOS2D;

class CNPCChaseOnGroundAgent: public CNPCChaseAgent
{
public:
	CNPCChaseOnGroundAgent(CMap * pMap);
	virtual ~CNPCChaseOnGroundAgent();

	virtual void Release();
	virtual void MoveOneStep();

	virtual void SetMoveStep(float fStep)
	{
		CNPCMoveAgent::SetMoveStep(fStep);
		m_StepPixels = (int)(m_fMoveStep / m_pMap->GetGroundPixelSize() + 0.5f);

		if (m_StepPixels == 0)
		{
			m_bStepTooSmall = true;
			m_iStepsOneGrid = (int)(m_pMap->GetGroundPixelSize() / m_fMoveStep + 0.5f);
			m_iCurStep = 0;
			m_StepPixels = 1;
		}
		else
			m_bStepTooSmall = false;
	}

	virtual bool StartPathFinding(int SearchPixels = 10);

	bool CanGoStraightForward();
	bool CanGoStraightForward(const POS2D& posFrom, const POS2D& posTo);

protected:

	virtual void SetMoveDir(const A3DVECTOR3& vGoal)
	{
		m_vMoveDir = vGoal - m_vCurPos;
		m_vMoveDir.y = 0.0f;
		m_vMoveDir.Normalize();
	}

	virtual void AdjustCurPos()
	{
		POS2D curPos = m_pMap->GetGroundMapPos(m_vCurPos);
		float fDeltaHeight = m_pMap->GetGroundPosDeltaHeight(curPos);

		if (fDeltaHeight == 0)
			m_vCurPos.y = m_pMap->GetTerrainHeight(m_vCurPos.x, m_vCurPos.z);
		else
		{
			A3DVECTOR3 vPixelCenter = m_pMap->GetGroundPixelCenter(curPos);
			m_vCurPos.y = m_pMap->GetTerrainHeight(vPixelCenter.x, vPixelCenter.z) + fDeltaHeight;
		}
	}

	virtual bool CurPosGetToGoal()
	{
		A3DVECTOR3 vDelta = m_vGoal - m_vCurPos;
		vDelta.y = 0.0f;
		bool bCloseToGoal = vDelta.SquaredMagnitude() <= (m_fSqrMinDist2Goal + 2*RELAX_ERROR*m_fMinDist2Goal + SQR_RELAX_ERROR);
		bool bOverGo = m_bGoStraightForward ? (DotProduct(vDelta, m_vMoveDir) < 0) : false;
		return bCloseToGoal || bOverGo;
	}

	bool AdjustCurPosGetToGoal(const POS2D& curMapPos, const POS2D& lastMapPos);

	void FollowFoundPath();
	void GeneratePath(LPPathNode lpPN);
	void PredictPath();
	void BestFirstSearchPath();   // implements A* (name kept for compatibility)
	void SmoothPath();            // line-of-sight string-pulling after A* finds path
	void RestartSearch();         // reset and re-seed A* from current position (repath)
	void HandleBlockedState();    // repath only when A* open list is also exhausted

	// Manhattan distance (kept for PredictPath direction)
	int GetDist(int u, int v, const POS2D& pos)
	{
		return abs(u - pos.u) + abs(v - pos.v);
	}

	// Octile distance — admissible heuristic for 8-directional grid with costs 10/14
	int GetHeuristic(int u, int v, const POS2D& goal)
	{
		int du = abs(u - goal.u);
		int dv = abs(v - goal.v);
		int mn = (du < dv) ? du : dv;
		int mx = (du > dv) ? du : dv;
		return 10 * mx + 4 * mn;
	}

	int GetEuclidDistSqr(int u, int v, const POS2D& pos)
	{
		int du = u - pos.u, dv = v - pos.v;
		return (du*du + dv*dv);
	}

	int GetEuclidDistSqr(const POS2D& pos1, const POS2D& pos2)
	{
		return GetEuclidDistSqr(pos1.u, pos1.v, pos2);
	}

	bool PosGetToGoal(const POS2D& pos)
	{
		return (GetEuclidDistSqr(pos, m_p2Goal) <= m_fSqrMinDist2Goal);
	}

	bool PosGetToGoal(int u, int v)
	{
		return (GetEuclidDistSqr(u, v, m_p2Goal) <= m_fSqrMinDist2Goal);
	}

	// Allocate a node from the pool — zero heap allocations in search hot path
	LPPathNode AllocNode()
	{
		if (m_iPoolTop < NODE_POOL_SIZE)
			return &m_aNodePool[m_iPoolTop++];
		return NULL; // pool exhausted (shouldn't happen within normal search budget)
	}

protected:

	bool m_bGoStraightForward;

	// A* open / closed lists
	CSortVectorPathNode m_Open;
	CSortVectorPathNode m_Close;
	vecPOS2D m_PathFound;
	vecPOS2D m_PathPredict;

	POS2D m_p2Start;
	POS2D m_p2Goal;

	char m_du;
	char m_dv;

	char m_StepPixels;
	int  m_iCurInFoundPath;

	int  m_iBlockCount;   // consecutive blocked steps; triggers repath when >= m_iMaxBlockTimes

	bool m_bStepTooSmall;
	int  m_iStepsOneGrid;
	int  m_iCurStep;
	A3DVECTOR3 m_vSmallStep;

	// Per-agent node pool: eliminates heap allocation inside the A* search loop.
	// 1024 nodes covers the default max budget (900 pixels) with room to spare.
	enum { NODE_POOL_SIZE = 1024 };
	PathNode m_aNodePool[NODE_POOL_SIZE];
	int      m_iPoolTop;
};


#endif
