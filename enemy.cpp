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
#include "enemyact.h"
#include "bg.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE				D3DXVECTOR3(48.0f, 48.0f, 0.0f)
#define INIT_COLOR				D3DXCOLOR(0.0f, 0.0f, 0.0f,1.0f)

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
	// 通常
	"data\\TEXTURE\\enemy000.png",
	"data\\TEXTURE\\enemy001.png",
	"data\\TEXTURE\\enemy002.png",
	"data\\TEXTURE\\enemy003.png",
	"data\\TEXTURE\\enemy004.png",
	"data\\TEXTURE\\enemy005.png",
	"data\\TEXTURE\\enemy006.png",
	"data\\TEXTURE\\enemy007.png",
	"data\\TEXTURE\\enemy001.png",
	"data\\TEXTURE\\enemy001.png",
	"data\\TEXTURE\\enemy007.png",
	// ボス
	"data\\TEXTURE\\boss000.png",
	"data\\TEXTURE\\boss001.png",	// 001A
	"data\\TEXTURE\\boss001.png",	// 001B
};

// 敵の情報[サイズ | 移動量 | 体力 | スコア]
ENEMYINFO g_aEnemyInfo[ENEMYTYPE_MAX] = {
	{ INIT_SIZE,		{0.0f, 2.0f, 0.0f},		10.0f,		200},						// 000
	{ INIT_SIZE,		{0.05f, 2.0f, 100.0f},	2.0f,		100},						// 001
	{ INIT_SIZE,		{0.0f, 10.0f, 0.0f},	10.0f,		200},						// 002
	{ INIT_SIZE,		{0.0f, 7.0f, 0.0f},		4.0f,		100},						// 003
	{ INIT_SIZE * 1.2f,	{0.0f, 5.0f, 0.0f},		25.0f,		1000},						// 004
	{ INIT_SIZE * 1.2f,	{10.0f, 0.0f, 0.0f},	30.0f,		500},						// 005
	{ INIT_SIZE * 2.0f,	{0.0f, 1.5f, 0.0f},		100.0f,		800},						// 006
	{ INIT_SIZE * 1.2f,	{1.0f, 8.0f, 0.0f},		15.0f,		300},						// 007
	{ INIT_SIZE,		{3.0f, 0.0f, 0.0f},		14.0f,		0,		BonusEnemy_Died},	// 008
	{ INIT_SIZE * 1.0f,	{0.0f, 7.0f, 0.0f},		5.0f,		100},						// 009
	{ INIT_SIZE * 1.5f,	{25.0f, 0.0f, 0.0f},	80.0f,		1500},						// 010
	{ INIT_SIZE * 3.0f, {2.0f, 3.0f, 0.0f},		800.0f,		10000,	Boss000_Died},		// boss000
	{ INIT_SIZE * 2.0f, {0, 0, 0},				250.0f,		5000,	Boss010A_Died},		// boss001A
	{ INIT_SIZE * 1.5f, {0, 0, 0},				1000000.0f,	0},							// boss001B
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

		if (IsObjectOutOfScreen(pEnemy->obj, rectScreen, pEnemy->disapperFlags))
		{// 画面外に出たら削除
			pEnemy->bUsed = false;
			continue;
		}

		if (BoxCollision(pEnemy->obj.pos, pEnemy->hitboxSize, pPlayer->obj.pos, pPlayer->hitBoxSize) &&
			pEnemy->state != ENEMYSTATE_DIED && 
			pEnemy->bHitEnabled == true)
		{// プレイヤーとの衝突判定
			HitPlayer();
		}

		pEnemy->nCounterState++;
		pEnemy->nCounterShoot++;
		pEnemy->nCounterMode++;

		// 敵の状態別処理
		switch (pEnemy->state)
		{
		case ENEMYSTATE_NORMAL:
			pEnemy->obj.color = pEnemy->originalColor;
			break;

		case ENEMYSTATE_DAMAGED:
			pEnemy->obj.color = ENEMY_COLOR_DAMAGED;
			pEnemy->state = ENEMYSTATE_NORMAL;
			break;

		case ENEMYSTATE_DIED:
			pEnemy->pfDied(pEnemy);
			break;
		}

		// 敵の行動別処理
		if (pEnemy->bActEnabled == true)
		{
			EnemyAct(pEnemy);
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

	// テクスチャステージステートの適用
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

	// テクスチャステージステートの解除
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
// 敵の数取得処理
//=====================================================================
int GetEnemyLeft(void)
{
	ENEMY* pEnemy = &g_aEnemy[0];
	int nLeft = 0;

	for (int nCount = 0; nCount < MAX_ENEMY; nCount++, pEnemy++)
	{
		if (pEnemy->bUsed == true && pEnemy->state != ENEMYSTATE_DIED)
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
			pEnemy->bActEnabled = true;
			pEnemy->bHitEnabled = true;
			pEnemy->bDamageEnabled = true;
			pEnemy->obj.pos = pos;
			pEnemy->startPos = pos;
			pEnemy->move = g_aEnemyInfo[type].move;
			pEnemy->obj.size = g_aEnemyInfo[type].size;
			pEnemy->obj.rot = D3DXVECTOR3_ZERO;
			pEnemy->obj.color = INIT_COLOR;
			pEnemy->originalColor = INIT_COLOR;
			pEnemy->obj.bVisible = true;

			pEnemy->fLife = g_aEnemyInfo[type].fLife;
			pEnemy->fMaxLife = pEnemy->fLife;
			pEnemy->fShootRot = 0;
			pEnemy->nScore = g_aEnemyInfo[type].nScore;
			pEnemy->type = type;
			pEnemy->disapperFlags = OOS_BOTTOM;
			pEnemy->pfDied = g_aEnemyInfo[type].pfDied;
			pEnemy->hitboxSize = pEnemy->obj.size;

			return pEnemy;
		}
	}

	return NULL;
}

void HitEnemy(ENEMY* pEnemy, float fDamage)
{
	if (IsObjectOutOfScreen(pEnemy->obj) || pEnemy->fLife <= 0.0f)
	{
		return;
	}
	else if (pEnemy->bDamageEnabled == false)
	{
		return;
	}

	pEnemy->fLife -= fDamage;
	
	if (pEnemy->fLife <= 0)
	{
		AddScore(pEnemy->nScore);

		if (pEnemy->pfDied != NULL)
		{
			pEnemy->state = ENEMYSTATE_DIED;
			pEnemy->nCounterState = 0;
			pEnemy->bActEnabled = false;
			return;
		}

		PlaySound(SOUND_LABEL_SE_HIT00);
		SetSpriteEffect(SPRITEEFFECTYPE_EXPLOSION, pEnemy->obj.pos, 1.0f, 0.0f, GetBackgroundSpeed());
		pEnemy->bUsed = false;
	}
	else
	{
		PlaySound(SOUND_LABEL_SE_HIT00, 0.25f);
		pEnemy->state = ENEMYSTATE_DAMAGED;
	}
}

void DestroyAllEnemy(void)
{
	ENEMY* pEnemy = &g_aEnemy[0];

	for (int nCount = 0; nCount < MAX_ENEMY; nCount++, pEnemy++)
	{
		if (pEnemy->bUsed == false) continue;

		HitEnemy(pEnemy, pEnemy->fLife);
	}
}