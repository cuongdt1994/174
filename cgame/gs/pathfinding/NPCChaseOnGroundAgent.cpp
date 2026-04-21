/*
 * FILE: NPCChaseOnGroundAgent.cpp
 *
 * DESCRIPTION:   On-ground NPC chasing movement agent.
 *                Upgraded 2026: true A* algorithm replacing the old greedy
 *                BFS, per-agent node pool (zero heap alloc in search loop),
 *                line-of-sight path smoothing (string pulling), and
 *                automatic repath when the NPC becomes blocked.
 *
 * CREATED BY: He wenfeng, 2005/4/18
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "NPCChaseOnGroundAgent.h"

// ---------------------------------------------------------------------------
// Construction / destruction
// ---------------------------------------------------------------------------

CNPCChaseOnGroundAgent::CNPCChaseOnGroundAgent(CMap * pMap)
	: CNPCChaseAgent(pMap)
{
	m_bFoundPath       = false;
	m_bGoStraightForward = false;
	m_iPoolTop         = 0;
	m_iBlockCount      = 0;
	m_iCurInFoundPath  = -1;
	m_bStepTooSmall    = false;
	m_iStepsOneGrid    = 1;
	m_iCurStep         = 0;
}

CNPCChaseOnGroundAgent::~CNPCChaseOnGroundAgent()
{
	Release();
}

// ---------------------------------------------------------------------------
// Release: pool-safe clear — no per-node delete, pool counter reset instead
// ---------------------------------------------------------------------------

void CNPCChaseOnGroundAgent::Release()
{
	m_Open.ClearNoDelete();
	m_Close.ClearNoDelete();
	m_iPoolTop = 0;
	m_PathFound.clear();
	m_PathPredict.clear();
}

// ---------------------------------------------------------------------------
// StartPathFinding
// ---------------------------------------------------------------------------

bool CNPCChaseOnGroundAgent::StartPathFinding(int SearchPixels)
{
	if (GetToGoal()) return false;

	Release();

	m_p2Start = m_pMap->GetGroundMapPos(m_vCurPos);
	m_p2Goal  = m_pMap->GetGroundMapPos(m_vGoal);

	// Fast path: straight line to goal with no obstacle
	if (CanGoStraightForward())
	{
		m_bGoStraightForward = true;
		m_bBlocked           = false;
		m_bFoundPath         = true;
		return false;
	}
	m_bGoStraightForward = false;

	// Fallback direction for PredictPath
	m_du = (m_p2Goal.u > m_p2Start.u) ? 1 : (m_p2Goal.u < m_p2Start.u) ? -1 : 0;
	m_dv = (m_p2Goal.v > m_p2Start.v) ? 1 : (m_p2Goal.v < m_p2Start.v) ? -1 : 0;

	// Seed the open list with the start node
	LPPathNode pStart = AllocNode();
	pStart->u         = m_p2Start.u;
	pStart->v         = m_p2Start.v;
	pStart->g         = 0;
	pStart->h         = GetHeuristic(pStart->u, pStart->v, m_p2Goal);
	pStart->f         = pStart->h;
	pStart->pLastNode = NULL;
	m_Open.SortPush(pStart);

	m_bFoundPath      = false;
	m_bGetToGoal      = false;
	m_bBlocked        = false;
	m_iBlockCount     = 0;

	m_bIsGoalReachable = m_pMap->IsGroundPosNeighborsReachable(m_p2Goal, 1);
	m_PFPixels         = SearchPixels;
	m_iCurInFoundPath  = -1;

	return true;
}

// ---------------------------------------------------------------------------
// MoveOneStep
// ---------------------------------------------------------------------------

void CNPCChaseOnGroundAgent::MoveOneStep()
{
	if (GetToGoal()) return;

	if (m_bGoStraightForward)
	{
		CNPCChaseAgent::MoveOneStep();
		return;
	}

	if (m_bStepTooSmall)
	{
		if (m_iCurStep == 0)
		{
			A3DVECTOR3 vLastPos = m_vCurPos;

			if (m_bIsGoalReachable)
			{
				BestFirstSearchPath();

				if (m_bFoundPath)
				{
					FollowFoundPath();
					m_bBlocked    = false;
					m_iBlockCount = 0;
				}
				else
				{
					PredictPath();
					HandleBlockedState();
				}
			}
			else
				PredictPath();

			if (m_bGetToGoal) return;

			m_vSmallStep   = m_vCurPos - vLastPos;
			m_vSmallStep.y = 0.0f;
			m_vCurPos      = vLastPos;
			m_vSmallStep  /= m_iStepsOneGrid;
		}

		m_vCurPos += m_vSmallStep;
		m_iCurStep++;
		if (m_iCurStep == m_iStepsOneGrid)
			m_iCurStep = 0;
	}
	else
	{
		if (m_bIsGoalReachable)
		{
			BestFirstSearchPath();

			if (m_bFoundPath)
			{
				FollowFoundPath();
				m_bBlocked    = false;
				m_iBlockCount = 0;
			}
			else
			{
				PredictPath();
				HandleBlockedState();
			}
		}
		else
			PredictPath();
	}

	AdjustCurPos();
}

// ---------------------------------------------------------------------------
// RestartSearch — reset A* from current position after being blocked
// ---------------------------------------------------------------------------

void CNPCChaseOnGroundAgent::RestartSearch()
{
	m_iBlockCount = 0;
	m_bBlocked    = false;
	m_bFoundPath  = false;

	m_Open.ClearNoDelete();
	m_Close.ClearNoDelete();
	m_iPoolTop = 0;
	m_PathFound.clear();
	m_PathPredict.clear();
	m_iCurInFoundPath = -1;

	m_p2Start = m_pMap->GetGroundMapPos(m_vCurPos);

	LPPathNode pStart = AllocNode();
	if (!pStart) return;
	pStart->u         = m_p2Start.u;
	pStart->v         = m_p2Start.v;
	pStart->g         = 0;
	pStart->h         = GetHeuristic(pStart->u, pStart->v, m_p2Goal);
	pStart->f         = pStart->h;
	pStart->pLastNode = NULL;
	m_Open.SortPush(pStart);
}

// ---------------------------------------------------------------------------
// HandleBlockedState — repath logic decoupled from MoveOneStep
//
// RestartSearch is triggered only when:
//   (a) the NPC is physically blocked (PredictPath couldn't move it), AND
//   (b) the A* open list is exhausted (no more nodes to explore)
// While A* still has work to do, blocking is transient — just wait.
// ---------------------------------------------------------------------------

void CNPCChaseOnGroundAgent::HandleBlockedState()
{
	if (!m_bBlocked)
	{
		m_iBlockCount = 0; // freely moving, reset counter
		return;
	}

	// A* is still exploring — blocking during search is normal, don't repath yet
	if (!m_Open.empty())
		return;

	// A* exhausted AND NPC is stuck: time to restart from current position
	m_iBlockCount++;
	if (m_iBlockCount >= m_iMaxBlockTimes)
		RestartSearch();
}

// ---------------------------------------------------------------------------
// BestFirstSearchPath — true A* implementation
//
// Movement costs: cardinal = 10, diagonal = 14 (≈ 10√2).
// Heuristic: octile distance (admissible for 8-directional grid).
// Diagonal moves are blocked when either adjacent cardinal cell is solid
// (prevents clipping through corners).
// Nodes come from the per-agent pool — no heap allocation in the search loop.
// ---------------------------------------------------------------------------

void CNPCChaseOnGroundAgent::BestFirstSearchPath()
{
	if (m_bFoundPath) return;

	static const int du8[8] = { 1,  1,  0, -1, -1, -1,  0,  1 };
	static const int dv8[8] = { 0,  1,  1,  1,  0, -1, -1, -1 };
	static const int gc8[8] = {10, 14, 10, 14, 10, 14, 10, 14 };

	int        searchPixels = 0;
	LPPathNode lpPN         = NULL;

	while (!m_Open.empty() && searchPixels < m_PFPixels)
	{
		lpPN = m_Open.PopFront();

		if (PosGetToGoal(lpPN->u, lpPN->v))
		{
			m_bFoundPath = true;
			break;
		}

		searchPixels++;

		for (int i = 0; i < 8; i++)
		{
			int nu = lpPN->u + du8[i];
			int nv = lpPN->v + dv8[i];

			POS2D pos(nu, nv);
			if (!m_pMap->IsGroundPosReachable(pos))
				continue;

			// Prevent corner-cutting: both adjacent cardinal cells must be passable
			if (gc8[i] == 14)
			{
				POS2D pc1(lpPN->u + du8[i], lpPN->v);
				POS2D pc2(lpPN->u,           lpPN->v + dv8[i]);
				if (!m_pMap->IsGroundPosReachable(pc1) || !m_pMap->IsGroundPosReachable(pc2))
					continue;
			}

			if (m_Close.FindByPos(nu, nv))
				continue;

			int newG = lpPN->g + gc8[i];
			int newH = GetHeuristic(nu, nv, m_p2Goal);
			int newF = newG + newH;

			// If already in open with a better score, skip; if worse, update
			LPPathNode existing = m_Open.FindByPos(nu, nv);
			if (existing)
			{
				if (newF < existing->f)
				{
					existing->g         = newG;
					existing->h         = newH;
					existing->f         = newF;
					existing->pLastNode = lpPN;
					// Re-insert at correct position
					m_Open.Remove(existing);
					m_Open.SortPush(existing);
				}
				continue;
			}

			LPPathNode lpNew = AllocNode();
			if (!lpNew) continue; // pool exhausted — skip this neighbour
			lpNew->u         = nu;
			lpNew->v         = nv;
			lpNew->g         = newG;
			lpNew->h         = newH;
			lpNew->f         = newF;
			lpNew->pLastNode = lpPN;
			m_Open.SortPush(lpNew);
		}

		m_Close.push_back(lpPN);
	}

	if (m_bFoundPath)
		GeneratePath(lpPN);
}

// ---------------------------------------------------------------------------
// GeneratePath — trace parent chain from goal back to start, then smooth
// ---------------------------------------------------------------------------

void CNPCChaseOnGroundAgent::GeneratePath(LPPathNode lpPN)
{
	// Walk the parent chain: lpPN is the goal node, chain ends at start
	// Result: m_PathFound[0] = goal, m_PathFound.back() = start
	while (lpPN)
	{
		POS2D pos(lpPN->u, lpPN->v);
		m_PathFound.push_back(pos);
		lpPN = lpPN->pLastNode;
	}

	// Remove unnecessary intermediate waypoints via line-of-sight checks
	SmoothPath();

	// PredictPath may have moved the NPC away from m_p2Start while A* was
	// computing.  Search the smoothed path for the node closest to the NPC's
	// current grid cell so FollowFoundPath starts from the right position.
	// This O(n) search runs exactly once per path found, not every step.
	POS2D curGrid = m_pMap->GetGroundMapPos(m_vCurPos);
	int   n       = (int)m_PathFound.size();
	m_iCurInFoundPath = n - 1; // safe default: A* start node

	int bestDist = 0x7fffffff;
	for (int i = n - 1; i >= 0; i--)
	{
		int d = GetEuclidDistSqr(curGrid, m_PathFound[i]);
		if (d < bestDist)
		{
			bestDist          = d;
			m_iCurInFoundPath = i;
		}
		if (d == 0) break; // exact match, stop early
	}

	if (m_iCurInFoundPath == 0)
		m_bGetToGoal = true;
}

// ---------------------------------------------------------------------------
// SmoothPath — line-of-sight string pulling
//
// Walks from start (m_PathFound.back()) toward goal (m_PathFound[0]).
// Skips any waypoint that can be seen directly from the current anchor,
// which removes the staircase effect produced by grid-based A*.
// ---------------------------------------------------------------------------

void CNPCChaseOnGroundAgent::SmoothPath()
{
	int n = (int)m_PathFound.size();
	if (n < 3) return;

	// m_PathFound[0] = goal, m_PathFound[n-1] = start
	vecPOS2D smoothed;

	int from = n - 1; // start from the start node
	while (from > 0)
	{
		smoothed.push_back(m_PathFound[from]);

		// Find the furthest waypoint toward goal with clear line-of-sight
		int reach = from - 1;
		for (int to = 0; to < from - 1; to++) // scan from goal side inward
		{
			if (CanGoStraightForward(m_PathFound[from], m_PathFound[to]))
			{
				reach = to;
				break; // furthest visible found
			}
		}
		from = reach;
	}
	smoothed.push_back(m_PathFound[0]); // always include goal

	// Rebuild m_PathFound in original order: goal at [0], start at back
	m_PathFound.clear();
	for (int i = (int)smoothed.size() - 1; i >= 0; i--)
		m_PathFound.push_back(smoothed[i]);
}

// ---------------------------------------------------------------------------
// PredictPath — greedy fallback with wall-sliding
//
// Tries to move toward the goal each step.  When the direct cell is blocked,
// slides along whichever axis is free (u-only or v-only) instead of stopping
// immediately.  This lets the NPC hug walls and navigate around obstacles
// while A* is still computing the proper path.
// ---------------------------------------------------------------------------

void CNPCChaseOnGroundAgent::PredictPath()
{
	if (m_bBlocked) return;

	POS2D posLast = m_PathPredict.empty() ? m_p2Start : m_PathPredict.back();

	for (int i = 0; i < m_StepPixels; i++)
	{
		// Desired next cell (toward goal, clamped to avoid overshooting)
		int tu = (posLast.u == m_p2Goal.u) ? posLast.u : posLast.u + m_du;
		int tv = (posLast.v == m_p2Goal.v) ? posLast.v : posLast.v + m_dv;
		POS2D posCur(tu, tv);

		if (!m_pMap->IsGroundPosReachable(posCur))
		{
			// Direct path blocked — try wall-sliding along each axis
			POS2D posU(posLast.u + m_du, posLast.v);
			POS2D posV(posLast.u,         posLast.v + m_dv);

			bool uOk = (m_du != 0) && m_pMap->IsGroundPosReachable(posU);
			bool vOk = (m_dv != 0) && m_pMap->IsGroundPosReachable(posV);

			if (uOk && vOk)
			{
				// Both axes free: choose the one that moves closer to goal
				int hu = GetHeuristic(posU.u, posU.v, m_p2Goal);
				int hv = GetHeuristic(posV.u, posV.v, m_p2Goal);
				posCur = (hu <= hv) ? posU : posV;
			}
			else if (uOk)
				posCur = posU;
			else if (vOk)
				posCur = posV;
			else
			{
				// All options blocked — NPC truly cannot move
				m_bBlocked = true;
				return;
			}
		}

		m_vCurPos.x += (posCur.u - posLast.u) * m_pMap->GetGroundPixelSize();
		m_vCurPos.z += (posCur.v - posLast.v) * m_pMap->GetGroundPixelSize();

		if (PosGetToGoal(posCur))
		{
			m_bGetToGoal = true;
			m_vCurPos.y  = m_pMap->GetTerrainHeight(m_vCurPos.x, m_vCurPos.z)
			             + m_pMap->GetGroundPosDeltaHeight(posCur);
			AdjustCurPosGetToGoal(posCur, posLast);
			return;
		}

		m_PathPredict.push_back(posCur);
		posLast = posCur;
	}
}

// ---------------------------------------------------------------------------
// FollowFoundPath — advance along the smoothed A* path
//
// m_iCurInFoundPath starts at (size-1) = start node and counts down to 0
// (goal).  No O(n) initial scan needed because GeneratePath sets the index.
// ---------------------------------------------------------------------------

void CNPCChaseOnGroundAgent::FollowFoundPath()
{
	int  iMovePixels = 0;
	POS2D posCur, posLast;

	while (m_iCurInFoundPath > 0 && iMovePixels < (int)m_StepPixels)
	{
		posLast = m_PathFound[m_iCurInFoundPath];
		m_iCurInFoundPath--;
		posCur = m_PathFound[m_iCurInFoundPath];

		iMovePixels++;
		m_vCurPos.x += (posCur.u - posLast.u) * m_pMap->GetGroundPixelSize();
		m_vCurPos.z += (posCur.v - posLast.v) * m_pMap->GetGroundPixelSize();
	}

	if (m_iCurInFoundPath == 0 && m_PathFound.size() > 1)
	{
		m_bGetToGoal = true;
		AdjustCurPosGetToGoal(m_PathFound[0], m_PathFound[1]);
	}
}

// ---------------------------------------------------------------------------
// AdjustCurPosGetToGoal — fine-tune final position on arrival
// ---------------------------------------------------------------------------

bool CNPCChaseOnGroundAgent::AdjustCurPosGetToGoal(const POS2D& curMapPos, const POS2D& lastMapPos)
{
	if (m_fMinDist2Goal < ZERO_DIST_ERROR && curMapPos == m_p2Goal)
	{
		m_vCurPos = m_vGoal;
		return true;
	}

	A3DVECTOR3 vCurPixelCenter  = m_pMap->GetGroundPixelCenter(curMapPos);
	A3DVECTOR3 vLastPixelCenter = m_pMap->GetGroundPixelCenter(lastMapPos);

	A3DVECTOR3 vDelta = m_vGoal - vCurPixelCenter;
	vDelta.y = 0.0f;
	if (vDelta.SquaredMagnitude() >= m_fSqrMinDist2Goal)
	{
		m_vCurPos = vCurPixelCenter;
	}
	else
	{
		vDelta = m_vGoal - vLastPixelCenter;
		vDelta.y = 0.0f;
		if (vDelta.SquaredMagnitude() <= m_fSqrMinDist2Goal)
		{
			m_vCurPos = vLastPixelCenter;
		}
		else
		{
			float dx    = vLastPixelCenter.x - vCurPixelCenter.x;
			float dz    = vLastPixelCenter.z - vCurPixelCenter.z;
			float cx_gx = vCurPixelCenter.x  - m_vGoal.x;
			float cz_gz = vCurPixelCenter.z  - m_vGoal.z;
			float a     = dx*dx + dz*dz;
			float b     = 2*(dx*cx_gx + dz*cz_gz);
			float c     = cx_gx*cx_gx + cz_gz*cz_gz - m_fSqrMinDist2Goal;
			float D     = b*b - 4*a*c;
			if (D < 0) return false;

			float r = (-b - sqrt(D)) / (2*a);
			bool bHaveRoot = false;
			if (r > 0.0f && r < 1.0f)
			{
				bHaveRoot = true;
			}
			else
			{
				r = (-b / a) - r;
				if (r > 0.0f && r < 1.0f)
					bHaveRoot = true;
			}

			if (bHaveRoot)
			{
				m_vCurPos.x = vCurPixelCenter.x + dx * r;
				m_vCurPos.z = vCurPixelCenter.z + dz * r;
				m_vCurPos.y = 0.0f;
			}
			else
				return false;
		}
	}

	return true;
}

// ---------------------------------------------------------------------------
// CanGoStraightForward
// ---------------------------------------------------------------------------

bool CNPCChaseOnGroundAgent::CanGoStraightForward()
{
	return CanGoStraightForward(m_p2Start, m_p2Goal);
}

bool CNPCChaseOnGroundAgent::CanGoStraightForward(const POS2D& posFrom, const POS2D& posTo)
{
	return m_pMap->CanGroundGoStraightForward(posFrom, posTo);
}
