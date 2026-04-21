/*
 * FILE: Terrain.cpp
 *
 * DESCRIPTION: server-side terrain class
 *
 * CREATED BY: Hedi, 2004/11/22
 *
 * HISTORY:
 *   2024: Height map storage changed from float to uint16_t (saves 50% RAM).
 *         Init(row,col) now uses piece-reference mode instead of copying data
 *         (saves 100% for assembled instance-map terrains).
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "terrain.h"

CTerrain::CTerrain()
{
	m_pHeights      = NULL;
	m_fHeightScale  = 0.f;
	m_fHeightOffset = 0.f;
	m_nNumVertX     = 0;
	m_nNumVertZ     = 0;
	m_ox            = 0.f;
	m_oz            = 0.f;
	m_vGridSizeInv  = 1.f;
	m_bPieceRef     = false;
	m_ppPieces      = NULL;
	m_pPieceIdx     = NULL;
	m_nPieceRows    = 0;
	m_nPieceCols    = 0;
}

CTerrain::~CTerrain()
{
	Release();
}

bool CTerrain::Release()
{
	if (!m_bPieceRef)
	{
		delete [] m_pHeights;
	}
	m_pHeights  = NULL;
	m_bPieceRef = false;
	m_ppPieces  = NULL;
	delete [] m_pPieceIdx;
	m_pPieceIdx = NULL;
	return true;
}

// ---------------------------------------------------------------------------
// Load a full (or region-clipped) terrain from hmap files.
// ---------------------------------------------------------------------------
bool CTerrain::Init(const TERRAINCONFIG& config, float xmin, float zmin, float xmax, float zmax)
{
	m_config       = config;
	m_vGridSizeInv = 1.0f / config.vGridSize;

	float t_ox      = -config.nAreaWidth  * config.nNumCols * config.vGridSize * 0.5f;
	float t_oz      =  config.nAreaHeight * config.nNumRows * config.vGridSize * 0.5f;
	float areawidth  = config.nAreaWidth  * config.vGridSize;
	float areaheight = config.nAreaHeight * config.vGridSize;

	int nAreaHStart = (int)((xmin - t_ox) / areawidth);
	if (nAreaHStart < 0) nAreaHStart = 0;
	int nAreaHEnd   = (int)((xmax - t_ox) / areawidth);
	if (nAreaHEnd >= config.nNumCols) nAreaHEnd = config.nNumCols - 1;

	int nAreaVStart = (int)((t_oz - zmax) / areaheight);
	if (nAreaVStart < 0) nAreaVStart = 0;
	int nAreaVEnd   = (int)((t_oz - zmin) / areaheight);
	if (nAreaVEnd >= config.nNumRows) nAreaVEnd = config.nNumRows - 1;

	m_ox = t_ox + nAreaHStart * areawidth;
	m_oz = t_oz - nAreaVStart * areaheight;

	m_nNumVertX = (nAreaHEnd - nAreaHStart + 1) * config.nAreaWidth  + 1;
	m_nNumVertZ = (nAreaVEnd - nAreaVStart + 1) * config.nAreaHeight + 1;

	m_pHeights = new uint16_t[m_nNumVertZ * m_nNumVertX];
	if (!m_pHeights)
		return false;

	float range = config.vHeightMax - config.vHeightMin;
	m_fHeightScale  = range / 65535.0f;
	m_fHeightOffset = config.vHeightMin;

	// Temp row buffer for float→uint16 conversion
	float * tmp = new float[config.nAreaWidth + 1];

	char szMapName[256];
	for (int v = nAreaVStart; v <= nAreaVEnd; v++)
	{
		for (int h = nAreaHStart; h <= nAreaHEnd; h++)
		{
			int idArea = v * config.nNumCols + h;
			sprintf(szMapName, "%s/%d.hmap", config.szMapPath, idArea + 1);

			FILE * fpHMap = fopen(szMapName, "rb");
			if (!fpHMap)
			{
				delete [] tmp;
				return false;
			}

			uint16_t * pDst = m_pHeights
				+ (v - nAreaVStart) * config.nAreaHeight * m_nNumVertX
				+ (h - nAreaHStart) * config.nAreaWidth;

			int rowLen = config.nAreaWidth + 1;
			for (int i = 0; i <= config.nAreaHeight; i++)
			{
				fread(tmp, sizeof(float), rowLen, fpHMap);
				// Convert normalized 0..1 float to quantized uint16
				for (int j = 0; j < rowLen; j++)
				{
					float val = tmp[j] * 65535.0f + 0.5f;
					if (val > 65535.f) val = 65535.f;
					if (val < 0.f)    val = 0.f;
					pDst[j] = (uint16_t)(int)val;
				}
				pDst += m_nNumVertX;
			}
			fclose(fpHMap);
		}
	}

	delete [] tmp;
	return true;
}

// ---------------------------------------------------------------------------
// Load a single piece (used for random/maze map templates).
// ---------------------------------------------------------------------------
bool CTerrain::InitPiece(const TERRAINCONFIG& config, int piece_idx)
{
	m_config          = config;
	m_config.nNumAreas = 1;
	m_config.nNumRows  = 1;
	m_config.nNumCols  = 1;
	m_vGridSizeInv    = 1.0f / config.vGridSize;

	m_ox = -config.nAreaWidth  * config.vGridSize * 0.5f;
	m_oz =  config.nAreaHeight * config.vGridSize * 0.5f;

	m_nNumVertX = config.nAreaWidth  + 1;
	m_nNumVertZ = config.nAreaHeight + 1;

	int total = m_nNumVertX * m_nNumVertZ;
	m_pHeights = new uint16_t[total];
	if (!m_pHeights)
		return false;

	float range = config.vHeightMax - config.vHeightMin;
	m_fHeightScale  = range / 65535.0f;
	m_fHeightOffset = config.vHeightMin;

	char szMapName[256];
	sprintf(szMapName, "%s/%d.hmap", config.szMapPath, piece_idx + 1);

	FILE * fpHMap = fopen(szMapName, "rb");
	if (!fpHMap)
		return false;

	// Read all floats at once then convert
	float * tmp = new float[total];
	fread(tmp, sizeof(float), total, fpHMap);
	fclose(fpHMap);

	for (int i = 0; i < total; i++)
	{
		float val = tmp[i] * 65535.0f + 0.5f;
		if (val > 65535.f) val = 65535.f;
		if (val < 0.f)     val = 0.f;
		m_pHeights[i] = (uint16_t)(int)val;
	}
	delete [] tmp;
	return true;
}

// ---------------------------------------------------------------------------
// Assemble terrain from pieces using ZERO-COPY piece-reference mode.
//
// Instead of duplicating all height data into a new flat array, this stores
// a borrowed reference to the source piece array and transforms coordinates
// at query time.  For instance servers with 200 concurrent worlds this
// eliminates ~1 MB per world instance (×200 = 200 MB per server).
// ---------------------------------------------------------------------------
bool CTerrain::Init(int row, int col, const int * piece_indexes, CTerrain ** terrain_pieces)
{
	m_config          = terrain_pieces[0]->m_config;
	m_config.nNumAreas = row * col;
	m_config.nNumRows  = row;
	m_config.nNumCols  = col;

	m_vGridSizeInv = 1.0f / m_config.vGridSize;

	m_ox = -m_config.nAreaWidth  * col * m_config.vGridSize * 0.5f;
	m_oz =  m_config.nAreaHeight * row * m_config.vGridSize * 0.5f;

	m_nNumVertX = col * m_config.nAreaWidth  + 1;
	m_nNumVertZ = row * m_config.nAreaHeight + 1;

	m_fHeightScale  = terrain_pieces[0]->m_fHeightScale;
	m_fHeightOffset = terrain_pieces[0]->m_fHeightOffset;

	// Reference the piece array — lifetime must outlive this object
	m_bPieceRef  = true;
	m_ppPieces   = terrain_pieces;
	m_nPieceRows = row;
	m_nPieceCols = col;

	m_pPieceIdx = new int[row * col];
	memcpy(m_pPieceIdx, piece_indexes, row * col * sizeof(int));

	return true;
}

// ---------------------------------------------------------------------------
// Piece-reference GetHeightAt: find which piece covers (x,z), transform
// coordinates into piece-local space, and delegate.
// ---------------------------------------------------------------------------
float CTerrain::GetHeightAtPieceRef(float x, float z)
{
	float h = (x - m_ox) * m_vGridSizeInv;
	float v = (m_oz - z) * m_vGridSizeInv;

	if (h < 0 || v < 0 || (int)h >= m_nNumVertX || (int)v >= m_nNumVertZ)
		return m_config.vHeightMin;

	int pc = (int)(h / m_config.nAreaWidth);
	int pr = (int)(v / m_config.nAreaHeight);
	if (pc >= m_nPieceCols) pc = m_nPieceCols - 1;
	if (pr >= m_nPieceRows) pr = m_nPieceRows - 1;

	CTerrain * piece = m_ppPieces[m_pPieceIdx[pr * m_nPieceCols + pc]];

	// Transform assembled-world position to piece-local position.
	// Each piece is centered at origin with m_ox = -W*gs/2, m_oz = H*gs/2.
	// Assembled terrain has m_ox = -cols*W*gs/2, m_oz = rows*H*gs/2.
	float W = m_config.nAreaWidth  * m_config.vGridSize;
	float H = m_config.nAreaHeight * m_config.vGridSize;
	float piece_x = x + W * 0.5f * (m_nPieceCols - 2 * pc - 1);
	float piece_z = z + H * 0.5f * (2 * pr + 1 - m_nPieceRows);

	return piece->GetHeightAt(piece_x, piece_z);
}

// ---------------------------------------------------------------------------
// GetHeightAt: bilinear interpolation over quantized height grid.
// ---------------------------------------------------------------------------
float CTerrain::GetHeightAt(float x, float z)
{
	if (m_bPieceRef)
		return GetHeightAtPieceRef(x, z);

	float h = (x - m_ox) * m_vGridSizeInv;
	float v = (m_oz - z) * m_vGridSizeInv;

	int nX  = (int)h;
	int nZ  = (int)v;
	float dx = h - nX;
	float dz = v - nZ;

	if (nX < 0 || nZ < 0 || nX >= m_nNumVertX || nZ >= m_nNumVertZ)
		return m_config.vHeightMin;

	int v0 = nZ * m_nNumVertX + nX;
	float h0, h1, h2;

	//	0-----1
	//	| \   |
	//	|  \  |
	//	|   \ |
	//	2-----3
	if (dx < dz)
	{
		// left-top triangle
		h0 = DequantHeight(m_pHeights[v0 + m_nNumVertX]);
		h1 = DequantHeight(m_pHeights[v0 + m_nNumVertX + 1]);
		h2 = DequantHeight(m_pHeights[v0]);
		dz = 1.0f - dz;
	}
	else
	{
		// right-bottom triangle
		h0 = DequantHeight(m_pHeights[v0 + 1]);
		h1 = DequantHeight(m_pHeights[v0]);
		h2 = DequantHeight(m_pHeights[v0 + m_nNumVertX + 1]);
		dx = 1.0f - dx;
	}

	return h0 + (h1 - h0) * dx + (h2 - h0) * dz;
}
