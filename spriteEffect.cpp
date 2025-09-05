//=====================================================================
// 
// スプライトエフェクト [spriteEffect.cpp]
// Author : Kaito Iwaskai
//
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "spriteEffect.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define INIT_SIZE		D3DXVECTOR3(64.0f, 64.0f, 0.0f)

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	const char* aSpriteEffectFilename;
	int nMaxPattern;
	int nAnimSpeed;
}SPRITEEFFECT_INFO;


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSpriteEffect = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffSpriteEffect[MAX_SPRITEEFFECT] = {};
SPRITEEFFECT g_aSpriteEffect[MAX_SPRITEEFFECT];

SPRITEEFFECT_INFO g_aSpriteEffectInfo[SPRITEEFFECTTYPE_MAX] = {
	{"data\\TEXTURE\\explosion000.png", 5, 3},
};


//=====================================================================
// 
// ***** 初期化処理 *****
// 
//=====================================================================
void InitSpriteEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	memset(&g_aSpriteEffect[0], 0, sizeof(SPRITEEFFECT) * MAX_SPRITEEFFECT);

	for (int nCount = 0; nCount < SPRITEEFFECTTYPE_MAX; nCount++)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			g_aSpriteEffectInfo[nCount].aSpriteEffectFilename,
			&g_pTexBuffSpriteEffect[nCount]
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_SPRITEEFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSpriteEffect,
		NULL
	);
}

//=====================================================================
// 
// ***** 終了処理 *****
// 
//=====================================================================
void UninitSpriteEffect(void)
{
	memset(&g_aSpriteEffect[0], 0, sizeof(SPRITEEFFECT) * MAX_SPRITEEFFECT);

	if (g_pVtxBuffSpriteEffect != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffSpriteEffect->Release();
		g_pVtxBuffSpriteEffect = NULL;
	}

	for (int nCount = 0; nCount < SPRITEEFFECTTYPE_MAX; nCount++)
	{
		if (g_pTexBuffSpriteEffect[nCount] != NULL)
		{// テクスチャバッファの破棄
			g_pTexBuffSpriteEffect[nCount]->Release();
			g_pTexBuffSpriteEffect[nCount] = NULL;
		}
	}
}

//=====================================================================
// 
// ***** 更新処理 *****
// 
//=====================================================================
void UpdateSpriteEffect(void)
{
	SPRITEEFFECT* pSpriteEffect = &g_aSpriteEffect[0];

	for (int nCount = 0; nCount < MAX_SPRITEEFFECT; nCount++, pSpriteEffect++)
	{
		if (pSpriteEffect->bUsed == false) continue;

		pSpriteEffect->nCounterAnim++;

		if (pSpriteEffect->nCounterAnim % pSpriteEffect->nAnimSpeed == 0)
		{
			pSpriteEffect->nPatternAnim++;
		}

		if (pSpriteEffect->nPatternAnim > pSpriteEffect->nMaxPattern - 1)
		{
			pSpriteEffect->bUsed = false;
			continue;
		}
	}
}

//=====================================================================
// 
// ***** 描画処理 *****
// 
//=====================================================================
void DrawSpriteEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;
	SPRITEEFFECT* pSpriteEffect = &g_aSpriteEffect[0];

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffSpriteEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_SPRITEEFFECT; nCount++, pSpriteEffect++, pVtx += 4)
	{
		// 頂点情報を設定
		SetVertexPos(pVtx, pSpriteEffect->obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pSpriteEffect->obj.color);
		SetVertexTexturePos(pVtx, pSpriteEffect->nPatternAnim, pSpriteEffect->nMaxPattern);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffSpriteEffect->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSpriteEffect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pSpriteEffect = &g_aSpriteEffect[0];
	for (int nCount = 0; nCount < MAX_SPRITEEFFECT; nCount++, pSpriteEffect++)
	{
		if (pSpriteEffect->obj.bVisible == true && pSpriteEffect->bUsed == true)
		{// ポリゴン描画
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffSpriteEffect[pSpriteEffect->type]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//=====================================================================
// 
// ***** スプライトエフェクト設定処理 *****
// 
//=====================================================================
void SetSpriteEffect(SPRITEEFFECTTYPE type, D3DXVECTOR3 pos, float fScale)
 {
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SPRITEEFFECT* pSpriteEffect = &g_aSpriteEffect[0];

	for (int nCount = 0; nCount < MAX_SPRITEEFFECT; nCount++, pSpriteEffect++)
	{
		if (pSpriteEffect->bUsed == true) continue;

		memset(&g_aSpriteEffect[0], 0, sizeof(SPRITEEFFECT) * MAX_SPRITEEFFECT);

		pSpriteEffect->bUsed = true;
		pSpriteEffect->type = type;
		pSpriteEffect->obj.pos = pos;
		pSpriteEffect->obj.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pSpriteEffect->obj.size = INIT_SIZE * fScale;
		pSpriteEffect->obj.bVisible = true;
		pSpriteEffect->nMaxPattern = g_aSpriteEffectInfo[type].nMaxPattern;
		pSpriteEffect->nAnimSpeed = g_aSpriteEffectInfo[type].nAnimSpeed;

		break;
	}
}