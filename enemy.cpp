//=====================================================================
//
// 画像オブジェクト [enemy.cpp]
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

#include "player.h"
#include "enemy.h"
#include "enemybullet.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE_X				(48.0f)
#define INIT_SIZE_Y				(48.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

#define INIT_ENEMY_LIFE				(10)
#define INIT_ENEMY_SHOOT_INTERVAL	(20)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffEnemy[ENEMYTYPE_MAX] = {};
ENEMY g_aEnemy[MAX_ENEMY] = {};

const char* g_aEnemyFileName[ENEMYTYPE_MAX] = {
	"data\\TEXTURE\\enemy000.png",
	"data\\TEXTURE\\enemy001.png",
};

//=====================================================================
// 初期化処理
//=====================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス
	VERTEX_2D* pVtx;							// 頂点情報
	ENEMY* pEnemy = &g_aEnemy[0];

	// 構造体の初期化
	memset(pEnemy, 0, sizeof(ENEMY) * MAX_ENEMY);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitEnemy(void)
{
	if (g_pVtxBuffEnemy != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

	for (int nCount = 0; nCount < ENEMYTYPE_MAX; nCount++)
	{
		if (g_pTexBuffEnemy[nCount] != NULL)
		{// テクスチャバッファの破棄
			g_pTexBuffEnemy[nCount]->Release();
			g_pTexBuffEnemy[nCount] = NULL;
		}
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateEnemy(void)
{
	ENEMY* pEnemy = &g_aEnemy[0];

	for (int nCount = 0; nCount < MAX_ENEMY; nCount++, pEnemy++)
	{
		if (pEnemy->bUsed == false) continue;

		switch (pEnemy->type)
		{
		case ENEMYTYPE_000:
			pEnemy->nCounterShoot++;
			if (pEnemy->nCounterShoot % pEnemy->nShootInterval == 0)
			{
				pEnemy->nCounterShoot = 0;
				SetEnemyBullet(ENEMYBULLET_TYPE_001, pEnemy->obj.pos, 5.0f, Direction(pEnemy->obj.pos, GetPlayer()->obj.pos));
			}

			break;

		case ENEMYTYPE_001:

			break;
		}
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	ENEMY* pEnemy = &g_aEnemy[0];

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_ENEMY; nCount++, pEnemy++, pVtx += 4)
	{
		// 頂点情報を設定
		SetVertexPos(pVtx, pEnemy->obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pEnemy->obj.color);
		SetVertexTexturePos(pVtx);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pEnemy = &g_aEnemy[0];
	for (int nCount = 0; nCount < MAX_ENEMY; nCount++, pEnemy++)
	{
		if (pEnemy->obj.bVisible == true && pEnemy->bUsed == true)
		{// ポリゴン描画
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffEnemy[pEnemy->type]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//=====================================================================
// オブジェクト取得処理
//=====================================================================
ENEMY* GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=====================================================================
//敵の数取得処理
//=====================================================================
int GetEnemyLeft(void)
{
	ENEMY* pEnemy = &g_aEnemy[0];
	int nLeft = 0;

	for (int nCount = 0; nCount < MAX_ENEMY; nCount++, pEnemy++)
	{
		if (pEnemy->bUsed == true)
		{
			nLeft++;
		}
	}

	return nLeft;
}

//=====================================================================
// 敵の設定処理
//=====================================================================
ENEMY* SetEnemy(ENEMYTYPE type, D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ENEMY* pEnemy = &g_aEnemy[0];
	for (int nCount = 0; nCount < MAX_ENEMY; nCount++, pEnemy++)
	{
		if (pEnemy->bUsed == false)
		{
			if (g_pTexBuffEnemy[type] == NULL)
			{// テクスチャの生成
				D3DXCreateTextureFromFile(
					pDevice,
					g_aEnemyFileName[type],
					&g_pTexBuffEnemy[type]
				);
			}

			memset(pEnemy, 0, sizeof(ENEMY));
			pEnemy->bUsed = true;
			pEnemy->obj.pos = pos;
			pEnemy->obj.size = { INIT_SIZE_X, INIT_SIZE_Y, 0.0f };
			pEnemy->obj.rot = D3DXVECTOR3_ZERO;
			pEnemy->obj.color = INIT_COLOR;
			pEnemy->obj.bVisible = true;

			pEnemy->fLife = INIT_ENEMY_LIFE;
			pEnemy->nShootInterval = INIT_ENEMY_SHOOT_INTERVAL;
			pEnemy->type = type;

			return pEnemy;
		}
	}

	return NULL;
}

void HitEnemy(ENEMY* pEnemy)
{
	pEnemy->fLife -= 1;
	
	if (pEnemy->fLife <= 0)
	{
		pEnemy->bUsed = false;
	}
}