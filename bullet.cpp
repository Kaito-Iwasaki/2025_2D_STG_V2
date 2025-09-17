//=====================================================================
//
// 弾オブジェクト [bullet.cpp]
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
#include "collision.h"
#include "baseScene.h"
#include "baseObject.h"

#include "bullet.h"
#include "enemy.h"
#include "enemybullet.h"
#include "spriteEffect.h"
#include "effect.h"
#include "player.h"
#include "particle.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\bullet000.png"

#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE_X				(4.0f)
#define INIT_SIZE_Y				(32.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

#define INIT_HITBOX_SIZE		D3DXVECTOR3(8, 8, 0);

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffBullet = NULL;
BULLET g_aBullet[MAX_BULLET] = {};

//=====================================================================
// 初期化処理
//=====================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス
	BULLET* pBullet = &g_aBullet[0];

	// 構造体の初期化
	memset(pBullet, 0, sizeof(BULLET) * MAX_BULLET);
	for (int nCount = 0; nCount < MAX_BULLET; nCount++, pBullet++)
	{
		pBullet->obj.size = { INIT_SIZE_X, INIT_SIZE_Y, 0.0f };
		pBullet->obj.color = INIT_COLOR;
		pBullet->hitBoxSize = INIT_HITBOX_SIZE;
	}

	// テクスチャの読み込み
	if (TEXTURE_FILENAME)
	{// テクスチャ作成
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffBullet
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitBullet(void)
{
	if (g_pVtxBuffBullet != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

	if (g_pTexBuffBullet != NULL)
	{// テクスチャバッファの破棄
		g_pTexBuffBullet->Release();
		g_pTexBuffBullet = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateBullet(void)
{
	BULLET* pBullet = &g_aBullet[0];
	PLAYER* pPlayer;
	ENEMY* pEnemy;
	ENEMYBULLET* pEnemyBullet;
	RECT rectScreen = GAME_SCREEN_RECT;
	EFFECTINFO info;

	for (int nCountBullet = 0; nCountBullet < MAX_BULLET; nCountBullet++, pBullet++)
	{
		if (pBullet->bUsed == false) continue;		// 未使用の敵ならスキップ

		// オブジェクトを取得
		pPlayer = GetPlayer();
		pEnemy = GetEnemy();
		pEnemyBullet = GetEnemyBullet();

		pBullet->obj.pos += D3DXVECTOR3(sinf(pBullet->fAngle), cosf(pBullet->fAngle), 0.0f) * pBullet->fSpeed;
		pBullet->obj.rot.z = pBullet->fAngle + D3DX_PI;

		if (pBullet->obj.pos.y < 0)
		{// 画面外に出たら削除
			pBullet->bUsed = false;
			continue;
		}

		// 敵との衝突判定
		for (int nCountEnemy = 0; nCountEnemy < MAX_ENEMY; nCountEnemy++, pEnemy++)
		{
			if (pEnemy->bUsed && BoxCollision(pBullet->obj, pEnemy->obj) && pEnemy->bDamageEnabled)
			{
				float fAngleEnemyToBullet = Angle(pEnemy->obj.pos, pBullet->obj.pos);

				HitEnemy(pEnemy);

				info.fSpeed = 5.0f;
				info.fRotSpeed = 0.05f;
				info.fMaxScale = 0.3f;
				info.nMaxLife = 25;
				info.col = D3DXCOLOR(0.9f, 0.4f, 0.0f, 0.8f);

				SetParticle(
					info,
					pBullet->obj.pos,
					Angle(pEnemy->obj.pos, pBullet->obj.pos),
					0.3f,
					1,
					3
				);

				if (pEnemy->bThroughBullet == false)
				{
					pBullet->bUsed = false;
				}
			}
		}

		// 敵弾との衝突判定
		for (int nCountEnemyBullet = 0; nCountEnemyBullet < MAX_ENEMY; nCountEnemyBullet++, pEnemyBullet++)
		{
			if (pEnemyBullet->bUsed && BoxCollision(pBullet->obj, pEnemyBullet->obj))
			{
				HitEnemyBullet(pEnemyBullet);

				info.fSpeed = 3.0f;
				info.fRotSpeed = 0.05f;
				info.fMaxScale = 0.3f;
				info.nMaxLife = 15;
				info.col = D3DXCOLOR(0.7f, 0.3f, 0.0f, 0.7f);

				SetParticle(
					info,
					pBullet->obj.pos,
					Angle(pEnemyBullet->obj.pos, pBullet->obj.pos),
					0.3f,
					1,
					3
				);
			}
		}
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	BULLET* pBullet = &g_aBullet[0];

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_BULLET; nCount++, pBullet++, pVtx += 4)
	{
		// 頂点情報を設定
		SetVertexPos(pVtx, pBullet->obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pBullet->obj.color);
		SetVertexTexturePos(pVtx);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pBullet = &g_aBullet[0];
	for (int nCount = 0; nCount < MAX_BULLET; nCount++, pBullet++)
	{
		if (pBullet->bUsed == true && pBullet->obj.bVisible == true)
		{// ポリゴン描画
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffBullet);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//=====================================================================
// オブジェクト取得処理
//=====================================================================
BULLET* GetBullet(void)
{
	return &g_aBullet[0];
}

//=====================================================================
// 弾の設定処理
//=====================================================================
void SetBullet(D3DXVECTOR3 pos, float fSpeed, float fAngle)
{
	BULLET* pBullet = &g_aBullet[0];
	for (int nCount = 0; nCount < MAX_BULLET; nCount++, pBullet++)
	{
		if (pBullet->bUsed == false)
		{
			memset(pBullet, 0, sizeof(BULLET));
			pBullet->bUsed = true;
			pBullet->obj.pos = pos;
			pBullet->obj.size = { INIT_SIZE_X, INIT_SIZE_Y, 0.0f };
			pBullet->obj.color = INIT_COLOR;
			pBullet->fSpeed = fSpeed;
			pBullet->fAngle = fAngle;
			pBullet->fDamage = 1.0f;
			pBullet->obj.bVisible = true;

			break;
		}
	}
}
