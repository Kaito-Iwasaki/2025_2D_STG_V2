//=====================================================================
// 
// 汎用関数 [util.h]
// Author : Kaito Iwasaki
//
//=====================================================================
#ifndef _UTIL_H_
#define _UTIL_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
int Clamp(int nNum, int nMin, int nMax);
int RandRange(int nMin, int nMax);
float Magnitude(D3DXVECTOR3 from, D3DXVECTOR3 to);
float Direction(D3DXVECTOR3 from, D3DXVECTOR3 to);
bool LoadBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount);
bool SaveBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount);

#endif