/*
 * FILE: Terrain.h
 *
 * DESCRIPTION: header for terrain class on server side
 *
 * CREATED BY: Hedi, 2004/11/22
 *
 * HISTORY:
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <stdint.h>

typedef struct _TERRAINCONFIG
{
	int			nNumAreas;		// 地图一共有多少块
	int			nNumRows;		// 几行几块
	int			nNumCols;		// 几列几块
	int			nAreaWidth;		// 每块的宽度（以格点）
	int			nAreaHeight;	// 每块的高度（以格点）
	float		vGridSize;		// 每小格的尺寸

	float		vHeightMin;		// 0.0 对应的高度
	float		vHeightMax;		// 1.0 对应的高度

	char		szMapPath[256];	// 地图数据的路径，不要加最后一个斜线

} TERRAINCONFIG;

class CTerrain
{
private:
	// Heights stored as quantized uint16 to save 50% memory vs float.
	// actual_height = m_pHeights[i] * m_fHeightScale + m_fHeightOffset
	uint16_t *		m_pHeights;
	float			m_fHeightScale;		// (vHeightMax - vHeightMin) / 65535.0f
	float			m_fHeightOffset;	// vHeightMin

	int				m_nNumVertX;		// how many points in one row
	int				m_nNumVertZ;		// how many points in one column

	float			m_ox;				// origin left-top x
	float			m_oz;				// origin left-top z

	float			m_vGridSizeInv;

	TERRAINCONFIG	m_config;

	// Piece-reference mode: assembled terrains reference source pieces
	// instead of copying data, saving memory for random/maze world instances.
	bool			m_bPieceRef;
	CTerrain **		m_ppPieces;		// borrowed pointer to piece array (not owned)
	int *			m_pPieceIdx;	// owned copy of piece index array
	int				m_nPieceRows;
	int				m_nPieceCols;

	float GetHeightAtPieceRef(float x, float z);
	inline float DequantHeight(uint16_t q) const { return q * m_fHeightScale + m_fHeightOffset; }

public:
	inline int GetNumVertX()	{ return m_nNumVertX; }
	inline int GetNumVertZ()	{ return m_nNumVertZ; }

public:
	CTerrain();
	~CTerrain();

	// Load full or partial map from hmap files
	bool Init(const TERRAINCONFIG& config, float xmin, float zmin, float xmax, float zmax);
	// Load a single piece (for random/maze maps)
	bool InitPiece(const TERRAINCONFIG& config, int piece_idx);
	// Assemble from pieces — uses piece-reference mode (zero copy)
	bool Init(int row, int col, const int * piece_indexes, CTerrain ** terrain_pieces);
	bool Release();

	float GetHeightAt(float x, float z);
};

#endif//_TERRAIN_H_
