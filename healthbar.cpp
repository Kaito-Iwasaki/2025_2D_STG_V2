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

#define COLOR_REMAIN			D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_REMAIN_HEAL		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_REMAIN_DANGER		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_DAMAGE			D3DXCOLOR(0.3f, 0.0f, 0.0f, 1.0f)
#define COLOR_EMPTY				D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)

//*********************************************************************
// 
// ***** 列挙型定義 *****
// 
//*********************************************************************
typedef enum
{
	HEALTHBAR_TYPE_EMPTY = 0,
	HEALTHBAR_TYPE_DAMAGE,
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
	COLOR_EMPTY,
	COLOR_DAMAGE,
	COLOR_REMAIN
};

int g_nCounterDamaged = 0;
float g_fLastRemainSize = INIT_SIZE_X;
float g_fLastPlayerLife = PLAYER_HEAL_MAX;

//=====================================================================
// 
// ***** 初期化処理 *****
// 
//=====================================================================
void InitHealthbar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HEALTHBAR* pHealthbar = &g_aHealthbar[0];

	g_nCounterDamaged = 0;
	g_fLastRemainSize = INIT_SIZE_X;
	g_fLastPlayerLife = GetPlayer()->fLife;

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
	HEALTHBAR* pHealthbar = &g_aHealthbar[0];
	PLAYER* pPlayer = GetPlayer();

	g_nCounterDamaged++;
	if (pPlayer->fLife - g_fLastPlayerLife < 0)
	{
		g_nCounterDamaged = 0;
	}
	g_fLastPlayerLife = pPlayer->fLife;

	for (int nCount = 0; nCount < HEALTHBAR_TYPE_MAX; nCount++, pHealthbar++)
	{
		pHealthbar->nCountTime++;

		switch (nCount)
		{
		case HEALTHBAR_TYPE_DAMAGE:
			HEALTHBAR* pHealthBarRemain;

			pHealthBarRemain = &g_aHealthbar[HEALTHBAR_TYPE_REMAIN];

			pHealthbar->obj.pos.x = pHealthBarRemain->obj.pos.x + pHealthBarRemain->obj.size.x;

			if (g_nCounterDamaged < 60)
			{// 被ダメージ表示
				pHealthbar->obj.size.x = g_fLastRemainSize - pHealthBarRemain->obj.size.x;
			}
			else
			{// 被ダメージ表示を消していく
				pHealthbar->obj.size.x -= pHealthbar->obj.size.x * 0.1f;
				Clampf(&pHealthbar->obj.size.x, 0.0f, pHealthbar->obj.size.x);

				g_fLastRemainSize = pHealthBarRemain->obj.size.x;
			}
			break;

		case HEALTHBAR_TYPE_REMAIN:
			pHealthbar->obj.size.x = INIT_SIZE_X * (pPlayer->fLife / PLAYER_HEAL_MAX);
			Clampf(&pHealthbar->obj.size.x, 0.0f, pHealthbar->obj.size.x);

			if (pPlayer->state == PLAYERSTATE_NORMAL && pPlayer->fLife < PLAYER_HEAL_MAX)
			{
				if (pHealthbar->nCountTime % 3 == 0)
				{
					pHealthbar->obj.color = COLOR_REMAIN_HEAL;
				}
				else
				{
					if (pPlayer->fLife <= 1.0f)
					{
						pHealthbar->obj.color = COLOR_REMAIN_DANGER;
					}
					else
					{
						pHealthbar->obj.color = COLOR_REMAIN;
					}
				}
			}
			else
			{
				if (pPlayer->fLife <= 1.0f)
				{
					pHealthbar->obj.color = COLOR_REMAIN_DANGER;
				}
				else
				{
					pHealthbar->obj.color = COLOR_REMAIN;
				}
			}
			break;
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