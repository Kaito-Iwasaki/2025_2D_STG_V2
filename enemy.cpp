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
#include "baseScene.h"
#include "collision.h"

#include "player.h"
#include "enemy.h"
#include "enemybullet.h"
#include "spriteEffect.h"
#include "score.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE				D3DXVECTOR3(48.0f, 48.0f, 0.0f)
#define INIT_SIZE_X				(48.0f)
#define INIT_SIZE_Y				(48.0f)
#define INIT_COLOR				D3DXCOLOR(0.0f, 0.0f, 0.0f,1.0f)

#define INIT_ENEMY_LIFE				(30)
#define INIT_ENEMY_SHOOT_INTERVAL	(20)

#define ENEMY_COLOR_NORMAL		INIT_COLOR
#define ENEMY_COLOR_DAMAGED		D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffEnemy[ENEMYTYPE_MAX] = {};
ENEMY g_aEnemy[MAX_ENEMY] = {};

// 敵のテクスチャ
const char* g_aEnemyFileName[ENEMYTYPE_MAX] = {
	"data\\TEXTURE\\enemy000.png",
	"data\\TEXTURE\\enemy001.png",
	"data\\TEXTURE\\enemy002.png",
	"data\\TEXTURE\\enemy003.png",
	"data\\TEXTURE\\enemy004.png",
	"data\\TEXTURE\\enemy005.png"
};

// 敵の情報[サイズ | 移動量 | 体力 | 弾数 | ショット間隔 | スコア]
ENEMYINFO g_aEnemyInfo[ENEMYTYPE_MAX] = {
	{ INIT_SIZE, {0.0f, 1.0f, 0.0f},		10.0f,	10,	 5,	100},
	{ INIT_SIZE, {0.05f, 2.0f, 100.0f},		2.0f,	0,	 0,	100},
	{ INIT_SIZE, {0.0f, 10.0f, 0.0f},		10.0f,	1,	 30,100},
	{ INIT_SIZE, {0.0f, 2.0f, 0.0f},		5.0f,	0,	 00,100},
	{ INIT_SIZE * 1.2f, {0.0f, 3.0f, 0.0f},	25.0f,	0,	 0,	100},
	{ INIT_SIZE * 3.0f, {0.0f, 3.0f, 0.0f},	500.0f,	0,	 3,	1000},
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

	// テクスチャの読み込み
	for (int nCount = 0; nCount < ENEMYTYPE_MAX; nCount++)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			g_aEnemyFileName[nCount],
			&g_pTexBuffEnemy[nCount]
		);
	}

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
	PLAYER* pPlayer = GetPlayer();
	RECT rectScreen = GAME_SCREEN_RECT;

	for (int nCount = 0; nCount < MAX_ENEMY; nCount++, pEnemy++)
	{
		if (pEnemy->bUsed == false) continue;

		if (IsObjectOutOfScreen(pEnemy->obj, rectScreen, OOS_BOTTOM))
		{// 画面外に出たら削除
			pEnemy->bUsed = false;
			continue;
		}

		if (BoxCollision(pEnemy->obj.pos, pEnemy->obj.size, pPlayer->obj.pos, pPlayer->hitBoxSize))
		{// プレイヤーとの衝突判定
			HitPlayer();
		}

		pEnemy->nCounterState++;
		pEnemy->nCounterShoot++;

		// 敵の状態別処理
		switch (pEnemy->state)
		{
		case ENEMYSTATE_NORMAL:
			pEnemy->obj.color = ENEMY_COLOR_NORMAL;
			break;

		case ENEMYSTATE_DAMAGED:
			pEnemy->obj.color = ENEMY_COLOR_DAMAGED;

			if (pEnemy->nCounterState % 1 == 0)
			{
				pEnemy->state = ENEMYSTATE_NORMAL;
			}
			break;
		}

		// 敵の行動別処理
		switch (pEnemy->type)
		{
		case ENEMYTYPE_000:
			pEnemy->obj.pos += pEnemy->move;

			if (pEnemy->nShootLeft < 1) break;

			if (pEnemy->nCounterShoot % pEnemy->nShootInterval == 0)
			{
				pEnemy->nCounterShoot = 0;
				pEnemy->fShootRot = RandRange(-100, 100) * 0.01f;
				if (SetEnemyBullet(
					ENEMYBULLET_TYPE_001,
					pEnemy->obj.pos,
					pEnemy->fShootSpeed,
					pEnemy->fShootRot))
				{
					pEnemy->nShootLeft--;
				}
			}
			break;

		case ENEMYTYPE_001:
			pEnemy->obj.pos.x = pEnemy->startPos.x + sin((float)pEnemy->nCounterState * pEnemy->move.x) * pEnemy->move.z;
			pEnemy->obj.pos.y += pEnemy->move.y;
			break;

		case ENEMYTYPE_002:
			pEnemy->move.y += (0.0f - pEnemy->move.y) * 0.05f;
			pEnemy->obj.pos.y += pEnemy->move.y + 1.0f;

			if (pEnemy->nShootLeft < 1) break;

			if (pEnemy->nCounterShoot % pEnemy->nShootInterval == 0)
			{
				pEnemy->nCounterShoot = 0;
				pEnemy->fShootRot = Direction(pEnemy->obj.pos, pPlayer->obj.pos);
				if (SetEnemyBullet(
					ENEMYBULLET_TYPE_002,
					pEnemy->obj.pos,
					pEnemy->fShootSpeed,
					pEnemy->fShootRot))
				{
					SetEnemyBullet(
						ENEMYBULLET_TYPE_002,
						pEnemy->obj.pos,
						pEnemy->fShootSpeed,
						pEnemy->fShootRot + D3DX_PI / 2);
					SetEnemyBullet(
						ENEMYBULLET_TYPE_002,
						pEnemy->obj.pos,
						pEnemy->fShootSpeed,
						pEnemy->fShootRot - D3DX_PI / 2);
					pEnemy->nShootLeft--;
				}
			}
			break;

		case ENEMYTYPE_003:
			if (pEnemy->nCounterState == 1)
			{
				pEnemy->obj.pos.x = RandRange(640 - 250, 640 + 250);
				pEnemy->move.y += RandRange(pEnemy->move.y - 1, pEnemy->move.y + 1);
			}

			pEnemy->obj.pos.y += pEnemy->move.y;
			break;

		case ENEMYTYPE_004:
			pEnemy->fShootRot = 4.0f;
			pEnemy->fShootRot = Direction(pEnemy->obj.pos, pPlayer->obj.pos);

			switch (pEnemy->nMode)
			{
			case 0:
				pEnemy->obj.pos.x += pEnemy->move.y * sinf(pEnemy->fShootRot);
				pEnemy->obj.pos.y += pEnemy->move.y * cosf(pEnemy->fShootRot);

				if (pEnemy->nCounterState > 30)
				{
					pEnemy->nMode = 1;
					pEnemy->nCounterState = 0;
				}
				break;

			case 1:
				SetEnemyBullet(
					ENEMYBULLET_TYPE_000,
					pEnemy->obj.pos,
					pEnemy->fShootSpeed,
					pEnemy->fShootRot
				);
				SetEnemyBullet(
					ENEMYBULLET_TYPE_000,
					pEnemy->obj.pos,
					pEnemy->fShootSpeed,
					pEnemy->fShootRot + 0.4f
				);
				SetEnemyBullet(
					ENEMYBULLET_TYPE_000,
					pEnemy->obj.pos,
					pEnemy->fShootSpeed,
					pEnemy->fShootRot - 0.4f
				);
				pEnemy->nMode = 2;
				pEnemy->nCounterState = 0;
				break;

			case 2:
				if (pEnemy->nCounterState > 30)
				{
					pEnemy->nMode = 0;
					pEnemy->nCounterState = 0;
				}
				break;
			}
			break;

		case ENEMYTYPE_005:
			if (pEnemy->obj.pos.y < 100.0f)
			{
				pEnemy->obj.pos.y += pEnemy->move.y;
			}

			if (pEnemy->nCounterShoot % pEnemy->nShootInterval == 0)
			{
				pEnemy->nShootLeft++;

				if (pEnemy->nShootLeft % 30 == 0)
				{
					SetEnemyBullet(
						ENEMYBULLET_TYPE_001,
						pEnemy->obj.pos,
						pEnemy->fShootSpeed + 2,
						Direction(pEnemy->obj.pos, pPlayer->obj.pos),
						30
					);
					SetEnemyBullet(
						ENEMYBULLET_TYPE_001,
						pEnemy->obj.pos,
						pEnemy->fShootSpeed + 2,
						Direction(pEnemy->obj.pos, pPlayer->obj.pos) + 0.4f,
						30
					);
					SetEnemyBullet(
						ENEMYBULLET_TYPE_001,
						pEnemy->obj.pos,
						pEnemy->fShootSpeed + 2,
						Direction(pEnemy->obj.pos, pPlayer->obj.pos) - 0.4f,
						30
					);
				}

				pEnemy->nCounterShoot = 0;
				pEnemy->fShootRot += 0.5f;
				SetEnemyBullet(
					ENEMYBULLET_TYPE_001,
					pEnemy->obj.pos,
					pEnemy->fShootSpeed,
					pEnemy->fShootRot,
					15
				);
			}

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

	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);

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

	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

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
			memset(pEnemy, 0, sizeof(ENEMY));
			pEnemy->bUsed = true;
			pEnemy->obj.pos = pos;
			pEnemy->startPos = pos;
			pEnemy->move = g_aEnemyInfo[type].move;
			pEnemy->obj.size = g_aEnemyInfo[type].size;
			pEnemy->obj.rot = D3DXVECTOR3_ZERO;
			pEnemy->obj.color = INIT_COLOR;
			pEnemy->obj.bVisible = true;

			pEnemy->fLife = g_aEnemyInfo[type].fLife;
			pEnemy->nShootLeft = g_aEnemyInfo[type].nShootLeft;
			pEnemy->nShootInterval = g_aEnemyInfo[type].nShootInterval;
			pEnemy->fShootSpeed = 2.0f;
			pEnemy->fShootRot = 0;
			pEnemy->nScore = 100;
			pEnemy->type = type;

			return pEnemy;
		}
	}

	return NULL;
}

void HitEnemy(ENEMY* pEnemy)
{
	if (IsObjectOutOfScreen(pEnemy->obj))
	{
		return;
	}

	pEnemy->fLife -= 1;
	
	if (pEnemy->fLife <= 0)
	{
		PlaySound(SOUND_LABEL_SE_HIT00);
		SetSpriteEffect(SPRITEEFFECTYPE_EXPLOSION, pEnemy->obj.pos, 1.0f);
		AddScore(pEnemy->nScore);
		pEnemy->bUsed = false;
	}
	else
	{
		PlaySound(SOUND_LABEL_SE_HIT00, 0.25f);
		pEnemy->state = ENEMYSTATE_DAMAGED;
	}
}