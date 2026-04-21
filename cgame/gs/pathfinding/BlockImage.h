  /*
 * FILE: BlockImage.h
 *
 * DESCRIPTION:   A  class to realize a model to save, load and 
 *							quickly look up a sparse large-size image.
 *							The basic idea is divide the original image into set of blocks!
 *
 * CREATED BY: He wenfeng, 2005/3/31
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#ifndef _BLOCKIMAGE_H_
#define _BLOCKIMAGE_H_

#include <stdio.h>
#include <memory.h>

#include <vector.h>
using namespace abase;

// Define a 16bits (10.6)  fix dot number type 
typedef unsigned short FIX16;

#define FLOATTOINT(x) ((int) floor((x)+0.5f))						// ��������

#define FLOATTOFIX16(x) (FIX16)FLOATTOINT(x*64.0f)
#define FIX16TOFLOAT(x) ( (x)/64.0f )		

#define NULL_ID -1

// Version Info (DWORD)

// Current Version
#define BLOCKIMAGE_VER 0x00000001	

// Old Version

// class laid under the namespace NPCMoveMap
namespace NPCMoveMap
{

typedef unsigned int DWORD;
typedef unsigned char UCHAR;


template<class T>
class CBlockImage
{

public:
	
	CBlockImage() { m_BlockIDs=NULL; SetBlockSizeExp(6); }			// default size is set to 64*64
	virtual ~CBlockImage() { Release(); }
	
	void SetBlockSizeExp(int iExp) 
	{ 
		m_iBlockSizeExp=iExp;
		m_iBlockSize=1 << iExp;
	}
	
	void Release()
	{
		if(m_BlockIDs) 
		{
			delete [] m_BlockIDs;
			m_BlockIDs=NULL;
		}
		
		for(DWORD i=0;i<m_arrBlocks.size();i++)
			delete m_arrBlocks[i];
		
		m_arrBlocks.clear();
	}

	T GetPixel(int u, int v)
	{
		int uBlkID=u>>m_iBlockSizeExp;
		int vBlkID=v>>m_iBlockSizeExp;
		int BlkID = m_BlockIDs[vBlkID*m_iWidth+uBlkID];
		if(BlkID == NULL_ID)
		{
			T tZero;
			memset(&tZero, 0, sizeof(T));
			return tZero;
		}
		
		//int uBlkOffset= u- (uBlkID<<m_iBlockSizeExp);
		//int vBlkOffset= v- (vBlkID<<m_iBlockSizeExp);
		int uBlkOffset= u & (m_iBlockSize -1);
		int vBlkOffset= v & (m_iBlockSize -1);
		
		//return m_arrBlocks[BlkID][vBlkOffset*m_iBlockSize+uBlkOffset];
		return m_arrBlocks[BlkID][(vBlkOffset<<m_iBlockSizeExp)+uBlkOffset];
	}

	void Init(T* pImage, int iWidth, int iLength, float fPixelSize = 1.0f);
	inline void InitZero(int iWidth, int iLength, float fPixelSize = 1.0f);

	void GetImageSize( int& width, int& length)
	{
		width = m_iImageWidth;
		length = m_iImageLength;
	}
	
	float GetPixelSize()
	{
		return m_fPixelSize;
	}

	// Load and Save, using FILE
	bool Save( FILE *pFileToSave );
	bool Load( FILE *pFileToLoad );

protected:
	vector<T* > m_arrBlocks;
	int* m_BlockIDs;
	int m_iBlockSize;				// the size of the block, only the width or length, while not the width*length
	int m_iBlockSizeExp;		  // ��2Ϊ�׵�m_iBlockSize��ָ��
	int m_iWidth;					  // blocks in width
	int m_iLength;					  // blocks in length

	float m_fPixelSize;					// Each pixel of the image is a square area
	int m_iImageWidth;				// Image width ( in pixels)
	int m_iImageLength;				// Image length ( in pixels)

};

// typedef CBlockImage<FIX16> FIX16BlockImage;

template<class T>
void CBlockImage<T>::Init(T* pImage, int iWidth, int iLength,float fPixelSize)
{
	// Firstly, we release myself
	Release();

	m_iImageWidth=iWidth;
	m_iImageLength=iLength;
	m_fPixelSize=fPixelSize;

	m_iWidth=iWidth >> m_iBlockSizeExp;
	//int iLastBlkWidth=iWidth - (m_iWidth << m_iBlockSizeExp);
	int iLastBlkWidth=iWidth & (m_iBlockSize - 1);
	if(iLastBlkWidth!=0)
		m_iWidth++;
	else
		iLastBlkWidth=m_iBlockSize;		

	m_iLength=iLength >> m_iBlockSizeExp;
	//int  iLastBlkLength= iLength - (m_iLength << m_iBlockSizeExp) ;
	int  iLastBlkLength= iLength & (m_iBlockSize - 1);
	if(iLastBlkLength!=0)	
		m_iLength++;
	else
		iLastBlkLength=m_iBlockSize;

	m_BlockIDs= new int[m_iWidth*m_iLength];
	int iPixelsNumInBlock=m_iBlockSize*m_iBlockSize;

	int iCurCpyWidth, iCurCpyLength;
	for(int i=0; i < m_iLength; i++)
		for(int j=0; j < m_iWidth; j++)
		{
			iCurCpyLength=(i==m_iLength-1)?iLastBlkLength:m_iBlockSize;
			iCurCpyWidth=(j==m_iWidth-1)?iLastBlkWidth:m_iBlockSize;
			T* Block=new T[iPixelsNumInBlock];
			T* ZeroBlock=new T[iPixelsNumInBlock];
			memset(Block, 0, iPixelsNumInBlock* sizeof(T));
			memset(ZeroBlock, 0, iPixelsNumInBlock* sizeof(T));
			
			// copy the data to the block
			int iStartCpyPos= ( i * iWidth + j ) * m_iBlockSize;	// In fact it is i*m_iBlockSize*iWidth + j* m_iBlockSize;
			int iDestCpyPos=0;
			for(int l=0; l<iCurCpyLength; l++)
			{
				memcpy(Block+iDestCpyPos, pImage+iStartCpyPos, iCurCpyWidth*sizeof(T));
				iStartCpyPos+= iWidth;
				iDestCpyPos+=m_iBlockSize;
			}

			if(memcmp(ZeroBlock, Block, iPixelsNumInBlock* sizeof(T)))
			{
				// not identical
				m_arrBlocks.push_back(Block);
				m_BlockIDs[i*m_iWidth+j]=m_arrBlocks.size()-1;
			}
			else
			{
				// identical which means the Block is a Zero Block
				delete [] Block;
				m_BlockIDs[i*m_iWidth+j]=NULL_ID;
			}
			delete [] ZeroBlock;
		}
		
}

template<class T>
inline void CBlockImage<T>::InitZero(int iWidth, int iLength, float fPixelSize)
{
	/*
	int iSize = iWidth * iLength;
	T* pImage = new T[iSize];
	memset(pImage, 1, iSize*sizeof(T));
	Init(pImage, iWidth, iLength, fPixelSize);
	delete [] pImage;
	*/

	// Firstly, we release myself
	Release();
	
	m_iImageWidth=iWidth;
	m_iImageLength=iLength;
	m_fPixelSize=fPixelSize;

	m_iWidth=iWidth >> m_iBlockSizeExp;
	if(iWidth & (m_iBlockSize - 1))	m_iWidth++;

	m_iLength=iLength >> m_iBlockSizeExp;
//	int  iLastBlkLength= iLength & (m_iBlockSize - 1);
	if(iLength & (m_iBlockSize - 1))	m_iLength++;

	int iSize = m_iWidth * m_iLength;
	m_BlockIDs= new int[iSize];
	for(int i=0; i< iSize; i++)
		m_BlockIDs[i]=NULL_ID;
	//*/
}

template<class T>
bool CBlockImage<T>::Save( FILE *pFileToSave )
{
	if(!pFileToSave) return false;
	
	DWORD dwWrite, dwWriteLen;
	
	// write the Version info
	dwWrite=BLOCKIMAGE_VER;
	dwWriteLen = fwrite(&dwWrite, 1, sizeof(DWORD), pFileToSave);
	if(dwWriteLen != sizeof(DWORD))
		return false;

	// write the buf size
	int BlkIDSize= m_iWidth*m_iLength*sizeof(int);
	int BlkSize=m_iBlockSize*m_iBlockSize*sizeof(T);
	DWORD BufSize= 5*sizeof(int)+sizeof(float) + BlkIDSize+sizeof(int)+m_arrBlocks.size()*BlkSize;
	dwWrite= BufSize;
	dwWriteLen = fwrite(&dwWrite, 1, sizeof(DWORD), pFileToSave);
	if(dwWriteLen != sizeof(DWORD))
		return false;

	// write the following info in order
	// 1. m_iWidth
	// 2. m_iLength
	// 3. m_iBlockSizeExp
	// 4. m_iImageWidth
	// 5. m_iImageLength
	// 6. m_fPixelSize
	// 7. data in m_BlockIDs
	// 8. size of m_arrBlocks
	// 9. data in m_arrBlocks
	
	UCHAR *buf=new UCHAR[BufSize];
	int cur=0;
	* (int *) (buf+cur) = m_iWidth;
	cur+=sizeof(int);

	* (int *) (buf+cur) = m_iLength;
	cur+=sizeof(int);	

	* (int *) (buf+cur) = m_iBlockSizeExp;
	cur+=sizeof(int);	

	* (int *) (buf+cur) = m_iImageWidth;
	cur+=sizeof(int);	

	* (int *) (buf+cur) = m_iImageLength;
	cur+=sizeof(int);	

	* (float *) (buf+cur) = m_fPixelSize;
	cur+=sizeof(float);	

	memcpy(buf+cur, m_BlockIDs, BlkIDSize);
	cur+=BlkIDSize;
	
	* (int *) (buf+cur) = m_arrBlocks.size();
	cur+=sizeof(int);

	for(DWORD i=0; i<m_arrBlocks.size(); i++)
	{
		memcpy(buf+cur, m_arrBlocks[i], BlkSize);
		cur+=BlkSize;
	}

	dwWriteLen = fwrite(buf, 1, BufSize, pFileToSave);
	if(dwWriteLen != BufSize)
	{
		delete [] buf;
		return false;
	}	

	delete [] buf;
	return true;
}

template<class T>
bool CBlockImage<T>::Load( FILE *pFileToLoad )
{
	if(!pFileToLoad) return false;

	DWORD dwRead, dwReadLen;

	// Read the Version
	dwReadLen = fread(&dwRead, 1, sizeof(DWORD), pFileToLoad);
	if(dwReadLen != sizeof(DWORD))
		return false;

	if(dwRead == BLOCKIMAGE_VER)
	{
		// Skip BufSize — read fields directly to avoid a large temporary
		// buffer that would double peak RAM (each dhmap submap can be MBs).
		dwReadLen = fread(&dwRead, 1, sizeof(DWORD), pFileToLoad);
		if(dwReadLen != sizeof(DWORD))
			return false;

		Release();

		// Read header fields directly into members
		if(fread(&m_iWidth,        sizeof(int),   1, pFileToLoad) != 1) return false;
		if(fread(&m_iLength,       sizeof(int),   1, pFileToLoad) != 1) return false;
		if(fread(&m_iBlockSizeExp, sizeof(int),   1, pFileToLoad) != 1) return false;
		m_iBlockSize = 1 << m_iBlockSizeExp;
		if(fread(&m_iImageWidth,   sizeof(int),   1, pFileToLoad) != 1) return false;
		if(fread(&m_iImageLength,  sizeof(int),   1, pFileToLoad) != 1) return false;
		if(fread(&m_fPixelSize,    sizeof(float), 1, pFileToLoad) != 1) return false;

		// Read BlockID index array
		int blkIDCount = m_iWidth * m_iLength;
		m_BlockIDs = new int[blkIDCount];
		if((int)fread(m_BlockIDs, sizeof(int), blkIDCount, pFileToLoad) != blkIDCount)
		{
			delete [] m_BlockIDs;
			m_BlockIDs = NULL;
			return false;
		}

		// Read each non-zero block directly into its own allocation
		int arrBlkSize = 0;
		if(fread(&arrBlkSize, sizeof(int), 1, pFileToLoad) != 1) return false;

		int pixPerBlock = m_iBlockSize * m_iBlockSize;
		for(int i = 0; i < arrBlkSize; i++)
		{
			T* pTBlock = new T[pixPerBlock];
			if((int)fread(pTBlock, sizeof(T), pixPerBlock, pFileToLoad) != pixPerBlock)
			{
				delete [] pTBlock;
				for(DWORD j = 0; j < m_arrBlocks.size(); j++) delete [] m_arrBlocks[j];
				m_arrBlocks.clear();
				delete [] m_BlockIDs; m_BlockIDs = NULL;
				return false;
			}
			m_arrBlocks.push_back(pTBlock);
		}

		return true;
	}
	return false;
}

}	// end of the namespace

#endif
