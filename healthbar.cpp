//=====================================================================
// 
// 体力バー [Healthbar.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "healthbar.h"
#include "player.h"
#include "util.h"
#include "baseScene.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define INIT_SIZE_X		(128.0f)
#define INIT_SIZE_Y		(16.0f)
#define INIT_POS_X		(GAME_SCREEN_START + 10)
#define INIT_POS_Y		(SCREEN_HEIGHT - INIT_SIZE_Y - 10)

#define COLOR_REMAIN	D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_DAMAGE	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)

//*********************************************************************
// 
// ***** 列挙型定義 *****
// 
//*********************************************************************
typedef enum
{
	HEALTHBAR_TYPE_DAMAGE = 0,
	HEALTHBAR_TYPE_REMAIN,
	HEALTHBAR_TYPE_MAX
}HEALTHBAR_TYPE;


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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHealthbar = NULL;		// 頂点バッファへのポインタ
HEALTHBAR g_aHealthbar[HEALTHBAR_TYPE_MAX];

D3DXCOLOR g_aColorHealthbar[HEALTHBAR_TYPE_MAX] = {
	COLOR_DAMAGE,
	COLOR_REMAIN
};

//=====================================================================
// 
// ***** 初期化処理 *****
// 
//=====================================================================
void InitHealthbar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HEALTHBAR* pHealthbar = &g_aHealthbar[0];

	// 構造体の初期化
	memset(pHealthbar, 0, sizeof(HEALTHBAR) * HEALTHBAR_TYPE_MAX);
	for (int nCount = 0; nCount < HEALTHBAR_TYPE_MAX; nCount++, pHealthbar++)
	{
		pHealthbar->obj.pos = { INIT_POS_X, INIT_POS_Y, 0 };
		pHealthbar->obj.size = { INIT_SIZE_X, INIT_SIZE_Y, 0 };
		pHealthbar->obj.color = g_aColorHealthbar[nCount];
		pHealthbar->obj.bVisible = true;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * HEALTHBAR_TYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHealthbar,
		NULL
	);
}

//=====================================================================
// 
// ***** 終了処理 *****
// 
//=====================================================================
void UninitHealthbar(void)
{
	if (g_pVtxBuffHealthbar != NULL)
	{
		g_pVtxBuffHealthbar->Release();
		g_pVtxBuffHealthbar = NULL;
	}
}

//=====================================================================
// 
// ***** 更新処理 *****
// 
//=====================================================================
void UpdateHealthbar(void)
{
	PLAYER* pPlayer = GetPlayer();
	HEALTHBAR* pHealthbar = &g_aHealthbar[0];

	for (int nCount = 0; nCount < HEALTHBAR_TYPE_MAX; nCount++, pHealthbar++)
	{
		if (nCount == HEALTHBAR_TYPE_REMAIN)
		{
			pHealthbar->obj.size.x = INIT_SIZE_X * (GetPlayer()->fLife / PLAYER_HEAL_MAX);
		}
	}
}

//=====================================================================
// 
// ***** 描画処理 *****
// 
//=====================================================================
void DrawHealthbar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;
	HEALTHBAR* pHealthbar = &g_aHealthbar[0];

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffHealthbar->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < HEALTHBAR_TYPE_MAX; nCount++, pHealthbar++, pVtx += 4)
	{
		// 頂点情報を設定
		SetVertexPos(pVtx,
			pHealthbar->obj.pos,
			pHealthbar->obj.pos + D3DXVECTOR3(pHealthbar->obj.size.x, 0, 0),
			pHealthbar->obj.pos + D3DXVECTOR3(0, pHealthbar->obj.size.y, 0),
			pHealthbar->obj.pos + pHealthbar->obj.size
		);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pHealthbar->obj.color);
		SetVertexTexturePos(pVtx);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffHealthbar->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHealthbar, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pHealthbar = &g_aHealthbar[0];
	for (int nCount = 0; nCount < HEALTHBAR_TYPE_MAX; nCount++, pHealthbar++)
	{
		if (pHealthbar->obj.bVisible == false) continue;

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
}