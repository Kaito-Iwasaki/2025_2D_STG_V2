//=====================================================================
//
// baseObject.cppのヘッダファイル [baseObject.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _BASEOBJECT_H_
#define _BASEOBJECT_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define OOS_TOP			(0x0001)	// 画面外判定（上）
#define OOS_BOTTOM		(0x0002)	// 画面外判定（下）
#define OOS_LEFT		(0x0004)	// 画面外判定（左）
#define OOS_RIGHT		(0x0008)	// 画面外判定（右）

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
//*********************************************************************
// 描画オブジェクト
//*********************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 size;		// サイズ
	D3DXCOLOR	color;		// 色
	bool bVisible;			// 表示状態
	bool bInversed;			// テクスチャ反転
}BASEOBJECT;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void SetVertexPos(VERTEX_2D* pVtx, BASEOBJECT obj);
void SetVertexRHW(VERTEX_2D* pVtx, float rhw);
void SetVertexColor(VERTEX_2D* pVtx, D3DXCOLOR col);
void SetVertexTexturePos(VERTEX_2D* pVtx, bool bInversed = false);
void SetVertexTexturePos(VERTEX_2D* pVtx, D3DXVECTOR2 leftTop, D3DXVECTOR2 rightTop, D3DXVECTOR2 leftBottom, D3DXVECTOR2 rightBottom, bool bInversed = false);
void SetVertexTexturePos(VERTEX_2D* pVtx, int nTexture, int nMaxTexture, bool bInversed = false);
bool IsObjectOutOfScreen(BASEOBJECT obj, DWORD flags);

#endif
