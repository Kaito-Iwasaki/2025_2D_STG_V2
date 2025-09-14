//=====================================================================
//
// 敵の挙動処理 [enemyact.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"
#include "enemy.h"
#include "enemybullet.h"
#include "enemyact.h"
#include "player.h"
#include "util.h"
#include "spriteEffect.h"
#include "sound.h"
#include "particle.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
//*********************************************************************
// Enemy000
//*********************************************************************
#define E000_SHOOT_MAX			(20)			// ショット数
#define E000_SHOOT_RANGE		(30)			// ショット範囲
#define E000_SHOOT_OFFSET		(20.0f)			// ショット位置補正値
//*********************************************************************
// Enemy001
//*********************************************************************
#define E001_ROT_SPEED			(0.1f)			// 回転速度
//*********************************************************************
// Enemy002
//*********************************************************************
#define E002_MOVE_Y_MIN			(1.0f)			// Y方向の最低移動量
#define E002_MOVE_Y_DECAY		(0.5f)			// Y方向の移動量減衰率
#define E002_SHOOT_OFFSET		(10.0f)			// ショット位置補正値

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************

// 敵の挙動処理関数
void (*g_aActFunction[ENEMYTYPE_MAX])(ENEMY* pEnemy) = {
	Enemy000_Act,
	Enemy001_Act,
	Enemy002_Act,
	Enemy003_Act,
	Enemy004_Act,
	Enemy005_Act,
	Enemy006_Act,
	Enemy007_Act,
	Enemy008_Act,
	Enemy009_Act,
	Boss000_Act,
}; 

//=====================================================================
// 
// ***** 敵の挙動処理 *****
// 
//=====================================================================
void EnemyAct(ENEMY* pEnemy)
{
	if (g_aActFunction[pEnemy->type] == NULL) return;

	g_aActFunction[pEnemy->type](pEnemy);
}

//=====================================================================
// 
// ***** タイプ別敵の挙動処理 *****
// 
//=====================================================================
//=====================================================================
// カニ
//=====================================================================
void Enemy000_Act(ENEMY* pEnemy)
{
	pEnemy->obj.pos += pEnemy->move;

	if (pEnemy->nShot > E000_SHOOT_MAX) return;

	if (pEnemy->nCounterShoot % pEnemy->nShootInterval == 0)
	{
		pEnemy->nCounterShoot = 0;
		pEnemy->fShootRot = RandRange(-E000_SHOOT_RANGE, E000_SHOOT_RANGE) * 0.01f;
		if (SetEnemyBullet(
			ENEMYBULLET_TYPE_003,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E000_SHOOT_OFFSET,
			pEnemy->fShootSpeed,
			pEnemy->fShootRot
		))
		{
			pEnemy->nShot++;
		}
	}
}

//=====================================================================
// イカ
//=====================================================================
void Enemy001_Act(ENEMY* pEnemy)
{
	pEnemy->obj.pos.x = pEnemy->startPos.x + sin((float)pEnemy->nCounterState * pEnemy->move.x) * pEnemy->move.z;
	pEnemy->obj.pos.y += pEnemy->move.y;
	pEnemy->obj.rot.z += E001_ROT_SPEED;
}

//=====================================================================
// 黒
//=====================================================================
void Enemy002_Act(ENEMY* pEnemy)
{
	pEnemy->obj.pos.y += pEnemy->move.y + E002_MOVE_Y_MIN;

	if (pEnemy->nShot > 1) return;

	if (pEnemy->nCounterShoot % pEnemy->nShootInterval == 0)
	{
		pEnemy->nCounterShoot = 0;
		pEnemy->fShootRot = Angle(pEnemy->obj.pos, GetPlayer()->obj.pos);
		SetEnemyBullet(
			ENEMYBULLET_TYPE_002,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E002_SHOOT_OFFSET,
			pEnemy->fShootSpeed,
			pEnemy->fShootRot);
		SetEnemyBullet(
			ENEMYBULLET_TYPE_002,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E002_SHOOT_OFFSET,
			pEnemy->fShootSpeed,
			pEnemy->fShootRot + D3DX_PI / 2);
		SetEnemyBullet(
			ENEMYBULLET_TYPE_002,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E002_SHOOT_OFFSET,
			pEnemy->fShootSpeed,
			pEnemy->fShootRot - D3DX_PI / 2);
		pEnemy->nShot++;
	}

	pEnemy->move.y += (0.0f - pEnemy->move.y) * E002_MOVE_Y_DECAY;
}

//=====================================================================
// 魚
//=====================================================================
void Enemy003_Act(ENEMY* pEnemy)
{
	if (pEnemy->nCounterState == 1)
	{
		pEnemy->obj.pos.x = RandRange(640 - 250, 640 + 250);
		pEnemy->move.y = RandRange(pEnemy->move.y * 0.8f, pEnemy->move.y * 1.2f);
	}

	pEnemy->obj.pos.y += pEnemy->move.y;
}

//=====================================================================
// 貝
//=====================================================================
void Enemy004_Act(ENEMY* pEnemy)
{
	switch (pEnemy->nMode)
	{
	case 0:
		pEnemy->fShootRot = Angle(pEnemy->obj.pos, GetPlayer()->obj.pos);
		pEnemy->obj.pos.x += pEnemy->move.y * sinf(pEnemy->fShootRot);
		pEnemy->obj.pos.y += pEnemy->move.y * cosf(pEnemy->fShootRot);

		if (pEnemy->nCounterState > 60)
		{
			pEnemy->nMode = 1;
			pEnemy->nCounterState = 0;
		}
		break;

	case 1:
		SetEnemyBullet(
			ENEMYBULLET_TYPE_000,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * 10.0f,
			pEnemy->fShootSpeed,
			pEnemy->fShootRot
		);
		SetEnemyBullet(
			ENEMYBULLET_TYPE_000,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * 10.0f,
			pEnemy->fShootSpeed,
			pEnemy->fShootRot + 0.4f
		);
		SetEnemyBullet(
			ENEMYBULLET_TYPE_000,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * 10.0f,
			pEnemy->fShootSpeed,
			pEnemy->fShootRot - 0.4f
		);
		pEnemy->nShot++;

		pEnemy->nMode = 2;
		pEnemy->nCounterState = 0;
		break;

	case 2:
		pEnemy->disapperFlags = OOS_ALL;
		pEnemy->obj.pos += Direction(pEnemy->fShootRot + D3DX_PI) * pEnemy->move.y;
		break;
	}

	pEnemy->obj.rot.z = pEnemy->fShootRot;
}

//=====================================================================
// 丸
//=====================================================================
void Enemy005_Act(ENEMY* pEnemy)
{
	pEnemy->obj.pos += pEnemy->move;

	switch (pEnemy->nMode)
	{
	case 0:	// 出現
		pEnemy->obj.pos.x += pEnemy->move.x;
		pEnemy->move.x += (0.0f - pEnemy->move.x) * 0.05f;

		if (pEnemy->nCounterMode > 60)
		{
			pEnemy->nMode++;
			pEnemy->nCounterMode = 0;
		}

		break;

	case 1:	// 照準設定
		pEnemy->fShootRot = Angle(pEnemy->obj.pos, GetPlayer()->obj.pos);
		pEnemy->nShot = 0;
		pEnemy->nMode++;

	case 2:	// 弾発射
		if (pEnemy->nCounterMode % pEnemy->nShootInterval != 0) break;

		SetEnemyBullet(
			ENEMYBULLET_TYPE_003,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * 10.0f,
			pEnemy->fShootSpeed,
			pEnemy->fShootRot
		);
		pEnemy->nShot++;

		if (pEnemy->nShot % 5 == 0)
		{
			pEnemy->nMode++;
			pEnemy->nCounterMode = 0;
		}

		break;

	case 3:	// 弾クールダウン
		if (pEnemy->nCounterMode > 60)
		{
			pEnemy->nMode++;
			pEnemy->nCounterMode = 0;
		}

		break;

	case 4:	// 逃走
		pEnemy->obj.pos.y += pEnemy->move.y;
		pEnemy->move.y += 0.1f;
		break;
	}
}

//=====================================================================
// 四角
//=====================================================================
void Enemy006_Act(ENEMY* pEnemy)
{
	pEnemy->obj.pos.y += pEnemy->move.y;

	if (pEnemy->nCounterShoot % pEnemy->nShootInterval == 0)
	{
		pEnemy->fShootRot += (D3DX_PI / 12.0f);
		for (int nCount = 0; nCount < 12; nCount++)
		{
			SetEnemyBullet(
				ENEMYBULLET_TYPE_000,
				pEnemy->obj.pos,
				pEnemy->fShootSpeed,
				pEnemy->fShootRot + (D3DX_PI * 2 / 12.0f) * nCount
			);
		}
	}
}

void Enemy006_Died(ENEMY* pEnemy)
{
	
}

//=====================================================================
// さんかく
//=====================================================================
void Enemy007_Act(ENEMY* pEnemy)
{
	pEnemy->obj.pos.y += pEnemy->move.y;

	switch (pEnemy->nMode)
	{
	case 0:
		pEnemy->obj.pos.x = RandRange(640 - 200, 640 + 200);
		pEnemy->disapperFlags = OOS_LEFT | OOS_RIGHT;
		pEnemy->nMode++;

	case 1:
		if (pEnemy->nCounterState > 60)
		{
			pEnemy->nMode++;
			pEnemy->nCounterShoot = 0;
		}
		break;

	case 2:
		if (pEnemy->nCounterShoot % pEnemy->nShootInterval == 0)
		{
			pEnemy->nCounterShoot = 0;
			pEnemy->fShootRot = Angle(pEnemy->obj.pos, GetPlayer()->obj.pos);
			SetEnemyBullet(
				ENEMYBULLET_TYPE_001,
				pEnemy->obj.pos + Direction(pEnemy->fShootRot) * 10.0f,
				7,
				pEnemy->fShootRot);
			pEnemy->nShot++;

			if (pEnemy->nShot > 10)
			{
				pEnemy->nMode++;
			}
		}
		break;

	case 3:
		pEnemy->move.x *= 1.05f;
		pEnemy->obj.pos.x += pEnemy->move.x;
		break;
	}

	pEnemy->obj.rot.z += 0.2f;
	pEnemy->move.y += (0.0f - pEnemy->move.y) * 0.05f;
}

//=====================================================================
// 砲台
//=====================================================================
void Enemy008_Act(ENEMY* pEnemy)
{

}

//=====================================================================
// おっさん
//=====================================================================
void Enemy009_Act(ENEMY* pEnemy)
{

}

//=====================================================================
// ボス１
//=====================================================================
void Boss000_Act(ENEMY* pEnemy)
{
	if (pEnemy->obj.pos.y < 100.0f)
	{
		pEnemy->obj.pos.y += pEnemy->move.y;
	}
	else
	{
		if (pEnemy->obj.pos.x < SCREEN_CENTER - 200 || pEnemy->obj.pos.x > SCREEN_CENTER + 200)
		{
			pEnemy->move *= -1;
		}
		pEnemy->obj.pos.x += pEnemy->move.x;
	}


	if (pEnemy->nCounterShoot % pEnemy->nShootInterval == 0)
	{
		if (pEnemy->nCounterShoot % 20 == 0)
		{
			SetEnemyBullet(
				ENEMYBULLET_TYPE_000,
				pEnemy->obj.pos,
				pEnemy->fShootSpeed + 2,
				Angle(pEnemy->obj.pos, GetPlayer()->obj.pos),
				30
			);
			SetEnemyBullet(
				ENEMYBULLET_TYPE_000,
				pEnemy->obj.pos,
				pEnemy->fShootSpeed + 2,
				Angle(pEnemy->obj.pos, GetPlayer()->obj.pos) + 0.4f,
				30
			);
			SetEnemyBullet(
				ENEMYBULLET_TYPE_000,
				pEnemy->obj.pos,
				pEnemy->fShootSpeed + 2,
				Angle(pEnemy->obj.pos, GetPlayer()->obj.pos) - 0.4f,
				30
			);
		}

		pEnemy->fShootRot += 0.5f;
		SetEnemyBullet(
			ENEMYBULLET_TYPE_001,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * 30.0f,
			3.0f,
			pEnemy->fShootRot,
			15
		);
		SetEnemyBullet(
			ENEMYBULLET_TYPE_001,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot + D3DX_PI) * 30.0f,
			3.0f,
			pEnemy->fShootRot + D3DX_PI,
			15
		);
	}
}

void Boss000_Died(ENEMY* pEnemy)
{
	// 画面上の弾を全て削除
	DestroyAllEnemyBullet();

	// 爆発エフェクト位置の設定
	float fRandX = RandRange(-60, 60);
	float fRandY = RandRange(-60, 60);

	// エフェクト情報の設定
	EFFECTINFO info;
	info.fSpeed = 5.0f;
	info.fRotSpeed = 0.05f;
	info.fMaxScale = 0.3f;
	info.nMaxLife = 40;
	info.col = D3DXCOLOR(0.9f, 0.7f, 0.0f, 1.0f);

	// 色を赤色に変更
	pEnemy->obj.color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	if (pEnemy->nCounterState % 10 == 0)
	{// 爆発サウンド再生
		PlaySound(SOUND_LABEL_SE_HIT02);
	}

	// 爆発エフェクト
	SetSpriteEffect(SPRITEEFFECTYPE_EXPLOSION, pEnemy->obj.pos + D3DXVECTOR3(fRandX, fRandY, 0.0f), 1.0f);

	// 爆発パーティクル
	SetParticle(info, pEnemy->obj.pos, 0, D3DX_PI * 2, 1, 10);

	if (pEnemy->nCounterState > 120)
	{// 消滅
		SetSpriteEffect(SPRITEEFFECTYPE_EXPLOSION, pEnemy->obj.pos, 4.0f);
		SetParticle(info, pEnemy->obj.pos, 0, D3DX_PI * 2, 1, 30);

		pEnemy->bUsed = false;
	}
}