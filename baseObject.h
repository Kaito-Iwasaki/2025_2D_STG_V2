//========================================================================
//
// baseObject.cppのヘッダファイル [baseObject.h]
// Author : Kaito Iwasaki
// 
//========================================================================
#ifndef _BASEOBJECT_H_
#define _BASEOBJECT_H_

//*******************************************
// 
// ***** インクルードファイル *****
// 
//*******************************************
#include "main.h"

//*******************************************
// 
// ***** 構造体 *****
// 
//*******************************************
//*******************************************
// 描画オブジェクト
//*******************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;
	LPDIRECT3DTEXTURE9	pTexBuff;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 size;
	D3DXCOLOR	color;
	bool bVisible;
}BASEOBJECT;

//*******************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*******************************************
void SetVertexPos(VERTEX_2D* pVtx, BASEOBJECT obj);
void SetVertexRHW(VERTEX_2D* pVtx, float rhw);
void SetVertexColor(VERTEX_2D* pVtx, D3DXCOLOR col);
void SetVertexTexturePos(VERTEX_2D* pVtx);
void SetVertexTexturePos(VERTEX_2D* pVtx, D3DXVECTOR2 leftTop, D3DXVECTOR2 rightTop, D3DXVECTOR2 leftBottom, D3DXVECTOR2 rightBottom);

#endif
