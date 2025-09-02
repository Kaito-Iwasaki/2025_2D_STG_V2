//=====================================================================
//
// 背景処理 [bg.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "input.h"
#include "sound.h"
#include "util.h"

#include "bg.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_NUM				(2)
#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE_X				(540 * TEXTURE_NUM)
#define INIT_SIZE_Y				(720 * TEXTURE_NUM)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

#define BG_SCROLL_SPEED			(3.0f)
#define BG_SCROLL_OFFSET		(1.0f)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBackground = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffBackground[MAX_BACKGROUND] = {};
BACKGROUND g_aBackground[MAX_BACKGROUND] = {};

const char* g_aBackgroundFileName[MAX_BACKGROUND] = {
	NULL,
	"data\\TEXTURE\\bg000.png",
	"data\\TEXTURE\\bg001.png",
	"data\\TEXTURE\\bg002.png",
};

//=====================================================================
// 初期化処理
//=====================================================================
void InitBackground(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス
	BACKGROUND* pBackground = &g_aBackground[0];

	// 構造体の初期化
	memset(pBackground, 0, sizeof(BACKGROUND) * MAX_BACKGROUND);

	for (int nCount = 0; nCount < MAX_BACKGROUND; nCount++, pBackground++)
	{
		pBackground->obj.pos = { INIT_POS_X, INIT_POS_Y, 0.0f };
		pBackground->obj.size = { INIT_SIZE_X, INIT_SIZE_Y, 0.0f };
		pBackground->obj.color = INIT_COLOR;
		pBackground->obj.bVisible = true;

		if (nCount == 0)
		{
			pBackground->obj.color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		}

		D3DXCreateTextureFromFile(
			pDevice,
			g_aBackgroundFileName[nCount],
			&g_pTexBuffBackground[nCount]
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BACKGROUND,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBackground,
		NULL
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitBackground(void)
{
	if (g_pVtxBuffBackground != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffBackground->Release();
		g_pVtxBuffBackground = NULL;
	}

	for (int nCount = 0; nCount < MAX_BACKGROUND; nCount++)
	{
		if (g_pTexBuffBackground[nCount] != NULL)
		{// テクスチャバッファの破棄
			g_pTexBuffBackground[nCount]->Release();
			g_pTexBuffBackground[nCount] = NULL;
		}
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateBackground(void)
{
	BACKGROUND* pBackground = &g_aBackground[0];

	for (int nCount = 0; nCount < MAX_BACKGROUND; nCount++, pBackground++)
	{
		pBackground->obj.pos.y += (nCount * BG_SCROLL_OFFSET);

		if (pBackground->obj.pos.y >= SCREEN_HEIGHT)
		{
			pBackground->obj.pos.y = 0;
		}
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawBackground(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	BACKGROUND* pBackground = &g_aBackground[0];

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffBackground->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_BACKGROUND; nCount++, pBackground++, pVtx += 4)
	{
		// 頂点情報を設定
		SetVertexPos(pVtx, pBackground->obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pBackground->obj.color);
		SetVertexTexturePos(pVtx,
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(1.0f * TEXTURE_NUM, 0.0f),
			D3DXVECTOR2(0.0f, 1.0f * TEXTURE_NUM),
			D3DXVECTOR2(1.0f * TEXTURE_NUM, 1.0f * TEXTURE_NUM)
		);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBackground->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBackground, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pBackground = &g_aBackground[0];
	for (int nCount = 0; nCount < MAX_BACKGROUND; nCount++, pBackground++)
	{
		if (pBackground->obj.bVisible == true)
		{// ポリゴン描画
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffBackground[nCount]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//=====================================================================
// オブジェクト取得処理
//=====================================================================
BACKGROUND* GetBackground(void)
{
	return &g_aBackground[0];
}