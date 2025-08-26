//=====================================================================
//
// 描画オブジェクト [baseObject.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "baseObject.h"

//=====================================================================
// 頂点位置の設定処理
//=====================================================================
void SetVertexPos(VERTEX_2D* pVtx, BASEOBJECT obj)
{
	float fLength = sqrtf(obj.size.x * obj.size.x + obj.size.y * obj.size.y) * 0.5f;
	float fAngle = atan2f(obj.size.x, obj.size.y);

	pVtx[0].pos.x = obj.pos.x + sinf(obj.rot.z + D3DX_PI + fAngle) * fLength;
	pVtx[0].pos.y = obj.pos.y + cosf(obj.rot.z + D3DX_PI + fAngle) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = obj.pos.x + sinf(obj.rot.z + D3DX_PI - fAngle) * fLength;
	pVtx[1].pos.y = obj.pos.y + cosf(obj.rot.z + D3DX_PI - fAngle) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = obj.pos.x + sinf(obj.rot.z - fAngle) * fLength;
	pVtx[2].pos.y = obj.pos.y + cosf(obj.rot.z - fAngle) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = obj.pos.x + sinf(obj.rot.z + fAngle) * fLength;
	pVtx[3].pos.y = obj.pos.y + cosf(obj.rot.z + fAngle) * fLength;
	pVtx[3].pos.z = 0.0f;
}

//=====================================================================
// 頂点RHWの設定処理
//=====================================================================
void SetVertexRHW(VERTEX_2D* pVtx, float rhw)
{
	pVtx[0].rhw = rhw;
	pVtx[1].rhw = rhw;
	pVtx[2].rhw = rhw;
	pVtx[3].rhw = rhw;
}

//=====================================================================
// 頂点カラーの設定処理
//=====================================================================
void SetVertexColor(VERTEX_2D* pVtx, D3DXCOLOR col)
{
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
}

//=====================================================================
// テクスチャ位置の設定処理（自動）
//=====================================================================
void SetVertexTexturePos(VERTEX_2D* pVtx)
{
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=====================================================================
// テクスチャ位置の設定処理（手動）
//=====================================================================
void SetVertexTexturePos(VERTEX_2D* pVtx, D3DXVECTOR2 leftTop, D3DXVECTOR2 rightTop, D3DXVECTOR2 leftBottom, D3DXVECTOR2 rightBottom)
{
	pVtx[0].tex = leftTop;
	pVtx[1].tex = rightTop;
	pVtx[2].tex = leftBottom;
	pVtx[3].tex = rightBottom;
}

//=====================================================================
// テクスチャ位置の設定処理（テクスチャアニメーション）
//=====================================================================
void SetVertexTexturePos(VERTEX_2D* pVtx, int nTexture, int nMaxTexture)
{
	pVtx[0].tex = D3DXVECTOR2((1.0f / nMaxTexture) * nTexture, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / nMaxTexture) * (nTexture + 1), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((1.0f / nMaxTexture) * nTexture, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / nMaxTexture) * (nTexture + 1), 1.0f);
}
