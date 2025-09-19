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
#include "collision.h"
#include "score.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
//*********************************************************************
// Enemy000
//*********************************************************************
#define E000_SHOOT_MAX			(20)			// ショット数
#define E000_SHOOT_SPEED		(3.0f)
#define E000_SHOOT_RANGE		(30)			// ショット範囲
#define E000_SHOOT_OFFSET		(20.0f)			// ショット位置補正値
#define E000_SHOOT_INTERVAL		(1)				// ショット間隔

//*********************************************************************
// Enemy001
//*********************************************************************
#define E001_ROT_SPEED			(0.1f)			// 回転速度
//*********************************************************************
// Enemy002
//*********************************************************************
#define E002_MOVE_Y_MIN			(2.0f)			// Y方向の最低移動量
#define E002_MOVE_Y_DECAY		(0.05f)			// Y方向の移動量減衰率
#define E002_SHOOT_SPEED		(2.0f)
#define E002_SHOOT_OFFSET		(10.0f)			// ショット位置補正値
#define E002_SHOOT_MAX			(1)
#define E002_SHOOT_INTERVAL		(30)
//*********************************************************************
// Enemy003
//*********************************************************************
#define E003_INIT_POS_X_RANGE	(250)
#define E003_MOVE_Y_MIN			(0.8f)
#define E003_MOVE_Y_MAX			(1.2f)
//*********************************************************************
// Enemy004
//*********************************************************************
#define E004_MOVE_INTERVAL		(60)
#define E004_SHOOT_SPEED		(3.0f)
#define E004_SHOOT_OFFSET		(10.0f)			// ショット位置補正値
#define E004_SHOOT_RANGE		(0.4f)			// ショット範囲
//*********************************************************************
// Enemy005
//*********************************************************************
#define E005_MOVE_INTERVAL01	(60)
#define E005_MOVE_INTERVAL02	(60)
#define E005_MOVE_Y_DECAY		(0.05f)			// Y方向の移動量減衰率
#define E005_MOVE_SPEED_Y_ADD	(0.1f)			// Y方向の移動量上昇率
#define E005_SHOOT_SPEED		(7.0f)
#define E005_SHOOT_OFFSET		(10.0f)			// ショット位置補正値
#define E005_SHOOT_INTERVAL01	(2)
#define E005_SHOOT_INTERVAL02	(10)				// ショット範囲

//*********************************************************************
// Enemy006
//*********************************************************************
#define E006_SHOOT_SPEED		(2.0f)
#define E006_SHOOT_NUM			(12.0f)
#define E006_SHOOT_INTERVAL		(80)

//*********************************************************************
// Enemy007
//*********************************************************************
#define E007_INIT_POS_X_RANGE	(200)
#define E007_MOVE_INTERVAL		(60)
#define E007_SHOOT_MAX			(20)
#define E007_SHOOT_OFFSET		(10.0f)
#define E007_SHOOT_SPEED		(7)
#define E007_SHOOT_INTERVAL		(20)

//*********************************************************************
// Enemy008
//*********************************************************************


//*********************************************************************
// Enemy009
//*********************************************************************


//*********************************************************************
// BOSS000
//*********************************************************************
#define BOSS000_MOVE_APPEAR				(100)
#define BOSS000_MOVE_RANGE				(200)
#define BOSS000_SHOOT_SPEED01			(5)			// 自機狙い
#define BOSS000_SHOOT_SPEED02			(3)
#define BOSS000_SHOOT_INTERVAL01		(3)
#define BOSS000_SHOOT_INTERVAL02		(60)

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
	Enemy010_Act,
	Boss000_Act,
	Boss010A_Act,
	Boss010B_Act,
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
// E000 | カニ
//=====================================================================
void Enemy000_Act(ENEMY* pEnemy)
{
	pEnemy->obj.pos += pEnemy->move;

	if (pEnemy->nShot > E000_SHOOT_MAX) return;

	if (pEnemy->nCounterShoot % E000_SHOOT_INTERVAL == 0)
	{
		pEnemy->nCounterShoot = 0;
		pEnemy->fShootRot = RandRange(-E000_SHOOT_RANGE, E000_SHOOT_RANGE) * 0.01f;
		if (SetEnemyBullet(
			ENEMYBULLET_TYPE_003,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E000_SHOOT_OFFSET,
			E000_SHOOT_SPEED,
			pEnemy->fShootRot
		))
		{
			pEnemy->nShot++;
		}
	}
}

//=====================================================================
// E001 | イカ
//=====================================================================
void Enemy001_Act(ENEMY* pEnemy)
{
	pEnemy->obj.pos.x = pEnemy->startPos.x + sin((float)pEnemy->nCounterState * pEnemy->move.x) * pEnemy->move.z;
	pEnemy->obj.pos.y += pEnemy->move.y;
	pEnemy->obj.rot.z += E001_ROT_SPEED;
}

//=====================================================================
// E002 | 黒
//=====================================================================
void Enemy002_Act(ENEMY* pEnemy)
{
	pEnemy->obj.pos.y += pEnemy->move.y + E002_MOVE_Y_MIN;
	pEnemy->move.y += (0.0f - pEnemy->move.y) * E002_MOVE_Y_DECAY;

	if (pEnemy->nShot > E002_SHOOT_MAX) return;

	if (pEnemy->nCounterShoot % E002_SHOOT_INTERVAL == 0)
	{
		pEnemy->nCounterShoot = 0;
		pEnemy->fShootRot = Angle(pEnemy->obj.pos, GetPlayer()->obj.pos);
		SetEnemyBullet(
			ENEMYBULLET_TYPE_002,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E002_SHOOT_OFFSET,
			E002_SHOOT_SPEED,
			pEnemy->fShootRot);
		SetEnemyBullet(
			ENEMYBULLET_TYPE_002,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E002_SHOOT_OFFSET,
			E002_SHOOT_SPEED,
			pEnemy->fShootRot + D3DX_PI / 2);
		SetEnemyBullet(
			ENEMYBULLET_TYPE_002,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E002_SHOOT_OFFSET,
			E002_SHOOT_SPEED,
			pEnemy->fShootRot - D3DX_PI / 2);
		pEnemy->nShot++;
	}

}

//=====================================================================
// E003 | 魚
//=====================================================================
void Enemy003_Act(ENEMY* pEnemy)
{
	if (pEnemy->nCounterState == 1)
	{
		pEnemy->obj.pos.x = RandRange(
			SCREEN_CENTER - E003_INIT_POS_X_RANGE,
			SCREEN_CENTER + E003_INIT_POS_X_RANGE
		);
		pEnemy->move.y = RandRange(
			pEnemy->move.y * E003_MOVE_Y_MIN,
			pEnemy->move.y * E003_MOVE_Y_MAX
		);
	}

	pEnemy->obj.pos.y += pEnemy->move.y;
}

//=====================================================================
// E004 | 貝
//=====================================================================
void Enemy004_Act(ENEMY* pEnemy)
{
	switch (pEnemy->nMode)
	{
	case 0:
		pEnemy->fShootRot = Angle(pEnemy->obj.pos, GetPlayer()->obj.pos);
		pEnemy->obj.pos.x += pEnemy->move.y * sinf(pEnemy->fShootRot);
		pEnemy->obj.pos.y += pEnemy->move.y * cosf(pEnemy->fShootRot);

		if (pEnemy->nCounterState > E004_MOVE_INTERVAL)
		{
			pEnemy->nMode++;
			pEnemy->nCounterState = 0;
		}
		break;

	case 1:
		SetEnemyBullet(
			ENEMYBULLET_TYPE_000,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E004_SHOOT_OFFSET,
			E004_SHOOT_SPEED,
			pEnemy->fShootRot
		);
		SetEnemyBullet(
			ENEMYBULLET_TYPE_000,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E004_SHOOT_OFFSET,
			E004_SHOOT_SPEED,
			pEnemy->fShootRot + E004_SHOOT_RANGE
		);
		SetEnemyBullet(
			ENEMYBULLET_TYPE_000,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E004_SHOOT_OFFSET,
			E004_SHOOT_SPEED,
			pEnemy->fShootRot - E004_SHOOT_RANGE
		);
		pEnemy->nShot++;

		pEnemy->nMode++;
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
// E005 | 丸
//=====================================================================
void Enemy005_Act(ENEMY* pEnemy)
{
	pEnemy->obj.pos += pEnemy->move;

	switch (pEnemy->nMode)
	{
	case 0:	// 出現
		pEnemy->obj.pos.x += pEnemy->move.x;
		pEnemy->move.x += (0.0f - pEnemy->move.x) * E005_MOVE_Y_DECAY;

		if (pEnemy->nCounterMode > E005_MOVE_INTERVAL01)
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
		if (pEnemy->nCounterMode % E005_SHOOT_INTERVAL01 != 0) break;

		SetEnemyBullet(
			ENEMYBULLET_TYPE_003,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E005_SHOOT_OFFSET,
			E005_SHOOT_SPEED,
			pEnemy->fShootRot
		);
		pEnemy->nShot++;

		if (pEnemy->nShot % E005_SHOOT_INTERVAL02 == 0)
		{
			pEnemy->nMode++;
			pEnemy->nCounterMode = 0;
		}

		break;

	case 3:	// 弾クールダウン
		if (pEnemy->nCounterMode > E005_MOVE_INTERVAL02)
		{
			pEnemy->nMode++;
			pEnemy->nCounterMode = 0;
		}

		break;

	case 4:	// 逃走
		pEnemy->obj.pos.y += pEnemy->move.y;
		pEnemy->move.y += E005_MOVE_SPEED_Y_ADD;
		break;
	}
}

//=====================================================================
// E006 | 四角
//=====================================================================
void Enemy006_Act(ENEMY* pEnemy)
{
	pEnemy->obj.pos.y += pEnemy->move.y;

	if (pEnemy->nCounterShoot % E006_SHOOT_INTERVAL == 0)
	{
		pEnemy->fShootRot += (D3DX_PI / E006_SHOOT_NUM);
		for (int nCount = 0; nCount < E006_SHOOT_NUM; nCount++)
		{
			SetEnemyBullet(
				ENEMYBULLET_TYPE_000,
				pEnemy->obj.pos,
				E006_SHOOT_SPEED,
				pEnemy->fShootRot + (D3DX_PI * 2 / E006_SHOOT_NUM) * nCount
			);
		}
	}
}

void Enemy006_Died(ENEMY* pEnemy)
{
	
}

//=====================================================================
// E007 | さんかく
//=====================================================================
void Enemy007_Act(ENEMY* pEnemy)
{
	switch (pEnemy->nMode)
	{
	case 0:	// 出現
		pEnemy->obj.pos.x = RandRange(
			SCREEN_CENTER - E007_INIT_POS_X_RANGE,
			SCREEN_CENTER + E007_INIT_POS_X_RANGE
		);
		pEnemy->disapperFlags = OOS_LEFT | OOS_RIGHT;
		pEnemy->nMode++;

	case 1:	// 待機
		if (pEnemy->nCounterState > E007_MOVE_INTERVAL)
		{
			pEnemy->nMode++;
			pEnemy->nCounterShoot = 0;
		}
		break;

	case 2:	// 弾発射
		if (pEnemy->nCounterShoot % E007_SHOOT_INTERVAL == 0)
		{
			pEnemy->nCounterShoot = 0;
			pEnemy->fShootRot = Angle(pEnemy->obj.pos, GetPlayer()->obj.pos);
			SetEnemyBullet(
				ENEMYBULLET_TYPE_001,
				pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E007_SHOOT_OFFSET,
				E007_SHOOT_SPEED,
				pEnemy->fShootRot);
			pEnemy->nShot++;

			if (pEnemy->nShot > E007_SHOOT_MAX)
			{
				pEnemy->nMode++;
			}
		}
		break;

	case 3:	// 逃走
		pEnemy->move.x *= 1.05f;
		pEnemy->obj.pos.x += pEnemy->move.x;
		break;
	}

	pEnemy->obj.pos.y += pEnemy->move.y;
	pEnemy->move.y += (0.0f - pEnemy->move.y) * 0.05f;
	pEnemy->obj.rot.z += 0.2f;
}

//=====================================================================
// E008 | ボーナスエネミー
//=====================================================================
void Enemy008_Act(ENEMY* pEnemy)
{
	switch (pEnemy->nMode)
	{
	case 0:
		if (pEnemy->obj.pos.x < SCREEN_CENTER)
		{
			pEnemy->disapperFlags = OOS_RIGHT;
		}
		else
		{
			pEnemy->disapperFlags = OOS_LEFT;
			pEnemy->move *= -1;
		}
		pEnemy->originalColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pEnemy->nMode++;

	case 1:
		pEnemy->obj.pos.x += pEnemy->move.x;
		pEnemy->obj.rot.z += 0.3f;

		if (pEnemy->nCounterState % 5 == 0)
		{
			if (pEnemy->nCounterState % 10 == 0)
			{
				pEnemy->originalColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			}
			else
			{
				pEnemy->originalColor = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			}
		}

		break;
	}
}

void BonusEnemy_Died(ENEMY* pEnemy)
{
	STAGE* pStage = GetStage();

	BonusScore(pStage->nBonus);
	PlaySound(SOUND_LABEL_SE_HIT00);
	SetSpriteEffect(SPRITEEFFECTYPE_EXPLOSION, pEnemy->obj.pos, 1.0f, 0.0f);
	pEnemy->bUsed = false;

	pStage->nBonus *= 2;
}

//=====================================================================
// E009 | 丸2
//=====================================================================
void Enemy009_Act(ENEMY* pEnemy)
{
	PLAYER* pPlayer = GetPlayer();

	switch (pEnemy->nMode)
	{
	case 0:
		pEnemy->fShootRot = Angle(pEnemy->obj.pos, pPlayer->obj.pos);

		if (Magnitude(pEnemy->obj.pos, pPlayer->obj.pos) < 3.0f);
		{
			pEnemy->nMode++;
		}
		break;

	case 1:
		break;
	}

	pEnemy->obj.pos += Direction(pEnemy->fShootRot) * pEnemy->move.y;
	pEnemy->obj.rot.z += 0.1f;
}

//=====================================================================
// E010 | さんかく２
//=====================================================================
void Enemy010_Act(ENEMY* pEnemy)
{
	switch (pEnemy->nMode)
	{
	case 0:
		pEnemy->disapperFlags = OOS_LEFT | OOS_RIGHT;
		if (pEnemy->obj.pos.x < SCREEN_CENTER)
		{
			pEnemy->disapperFlags = OOS_RIGHT;
		}
		else
		{
			pEnemy->disapperFlags = OOS_LEFT;
		}

		pEnemy->nMode++;

	case 1:	// 出現
		pEnemy->obj.pos.x += pEnemy->move.x;
		pEnemy->move.x += (0.0f - pEnemy->move.x) * 0.05f;

		if (pEnemy->nCounterMode > E005_MOVE_INTERVAL01)
		{
			pEnemy->nMode++;
			pEnemy->nCounterMode = 0;
		}

		break;

	case 2:	// 弾発射
		if (pEnemy->nCounterShoot % 40 == 0)
		{
			pEnemy->nCounterShoot = 0;
			pEnemy->fShootRot = Angle(pEnemy->obj.pos, GetPlayer()->obj.pos);
			SetEnemyBullet(
				ENEMYBULLET_TYPE_001,
				pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E007_SHOOT_OFFSET,
				5,
				pEnemy->fShootRot);
			SetEnemyBullet(
				ENEMYBULLET_TYPE_001,
				pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E007_SHOOT_OFFSET,
				5,
				pEnemy->fShootRot + 0.4f);
			SetEnemyBullet(
				ENEMYBULLET_TYPE_001,
				pEnemy->obj.pos + Direction(pEnemy->fShootRot) * E007_SHOOT_OFFSET,
				5,
				pEnemy->fShootRot - 0.4f);
			pEnemy->nShot++;

			if (pEnemy->nShot > 10)
			{
				pEnemy->nMode++;
			}
		}
		break;

	case 3:	// 逃走
		pEnemy->move.x *= 1.05f;
		pEnemy->obj.pos.x += pEnemy->move.x;
		break;
	}

	pEnemy->obj.rot.z += 0.2f;
}

//=====================================================================
// ボス１
//=====================================================================
void Boss000_Act(ENEMY* pEnemy)
{
	if (pEnemy->obj.pos.y < BOSS000_MOVE_APPEAR)
	{
		pEnemy->obj.pos.y += pEnemy->move.y;
	}
	else
	{
		if (
			pEnemy->obj.pos.x < SCREEN_CENTER - BOSS000_MOVE_RANGE ||
			pEnemy->obj.pos.x > SCREEN_CENTER + BOSS000_MOVE_RANGE
			)
		{
			pEnemy->move *= -1;
		}
		pEnemy->obj.pos.x += pEnemy->move.x;
	}
	pEnemy->obj.rot.z += 0.1f;

	if (pEnemy->nCounterShoot % BOSS000_SHOOT_INTERVAL01 == 0)
	{
		if (pEnemy->nCounterShoot % BOSS000_SHOOT_INTERVAL02 == 0)
		{// 自機狙い
			SetEnemyBullet(
				ENEMYBULLET_TYPE_000,
				pEnemy->obj.pos,
				BOSS000_SHOOT_SPEED01,
				Angle(pEnemy->obj.pos, GetPlayer()->obj.pos),
				30
			);
			SetEnemyBullet(
				ENEMYBULLET_TYPE_000,
				pEnemy->obj.pos,
				BOSS000_SHOOT_SPEED01,
				Angle(pEnemy->obj.pos, GetPlayer()->obj.pos) + 0.4f,
				30
			);
			SetEnemyBullet(
				ENEMYBULLET_TYPE_000,
				pEnemy->obj.pos,
				BOSS000_SHOOT_SPEED01,
				Angle(pEnemy->obj.pos, GetPlayer()->obj.pos) - 0.4f,
				30
			);
		}

		pEnemy->fShootRot += 0.6f;
		SetEnemyBullet(
			ENEMYBULLET_TYPE_001,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot) * 30.0f,
			BOSS000_SHOOT_SPEED02,
			pEnemy->fShootRot,
			15
		);
		SetEnemyBullet(
			ENEMYBULLET_TYPE_001,
			pEnemy->obj.pos + Direction(pEnemy->fShootRot + D3DX_PI) * 30.0f,
			BOSS000_SHOOT_SPEED02,
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
	SetSpriteEffect(SPRITEEFFECTYPE_EXPLOSION, pEnemy->obj.pos + D3DXVECTOR3(fRandX, fRandY, 0.0f), 1.0f, 0.0f);

	// 爆発パーティクル
	SetParticle(info, pEnemy->obj.pos, 0, D3DX_PI * 2, 1, 10);

	if (pEnemy->nCounterState > 120)
	{// 消滅
		SetSpriteEffect(SPRITEEFFECTYPE_EXPLOSION, pEnemy->obj.pos, 4.0f, 0.0f);
		SetParticle(info, pEnemy->obj.pos, 0, D3DX_PI * 2, 1, 30);

		pEnemy->bUsed = false;
	}
}

void Boss010A_Act(ENEMY* pEnemy)
{
	static ENEMY* apBossPart[8];
	static float fGap = 1.0f;

	switch (pEnemy->nMode)
	{
	case 0:
		pEnemy->move = { 1.0f, 1.0f, 0.0f };
		pEnemy->bHitEnabled = false;
		pEnemy->bThroughBullet = true;
		pEnemy->obj.bVisible = false;
		pEnemy->bDamageEnabled = false;
		fGap = 0.0f;

		for (int nCount = 0; nCount < 8; nCount++)
		{
			apBossPart[nCount] = SetEnemy(
				ENEMYTYPE_BOSS001B,
				D3DXVECTOR3(0, -50, 0)
			);
			apBossPart[nCount]->bDamageEnabled = false;
			apBossPart[nCount]->bActEnabled = false;
		}

		pEnemy->nMode++;

	case 1:
		pEnemy->obj.pos.y += 3.0f;
		if (pEnemy->obj.pos.y > 250.0f)
		{
			pEnemy->nMode++;
		}
		break;

	case 2:
		fGap += 0.025f;
		if (fGap >= 1.0f)
		{
			fGap = 1.0f;
			pEnemy->bDamageEnabled = true;
			pEnemy->startPos = pEnemy->obj.pos;

			for (int nCount = 0; nCount < 8; nCount++)
			{
				apBossPart[nCount]->bDamageEnabled = true;
				apBossPart[nCount]->bActEnabled = true;
			}

			pEnemy->nMode++;
			pEnemy->nCounterMode = 0;
		}
		break;

	case 3:
		pEnemy->obj.rot.z += 0.1f;
		if (
			pEnemy->obj.pos.x < SCREEN_CENTER - 150 ||
			pEnemy->obj.pos.x > SCREEN_CENTER + 150
			)
		{
			pEnemy->move.x *= -1;
		}

		pEnemy->obj.pos.x += pEnemy->move.x;
		pEnemy->obj.pos.y = pEnemy->startPos.y + sin((float)pEnemy->nCounterMode * 0.01f) * 100.0f;

		break;
	}
	
	for (int nCount = 0; nCount < 8; nCount++)
	{
		apBossPart[nCount]->fShootRot = Angle(pEnemy->obj.pos, apBossPart[nCount]->obj.pos);
		apBossPart[nCount]->obj.pos = pEnemy->obj.pos + Direction(pEnemy->obj.rot.z + (D3DX_PI * 2.0f / 8.0f * nCount * fGap)) * 90.0f;
		if (apBossPart[nCount]->state == ENEMYSTATE_DAMAGED)
		{
			HitEnemy(pEnemy);
		}
	}
}

void Boss010A_Died(ENEMY* pEnemy)
{
	PLAYER* pPlayer = GetPlayer();

	pEnemy->bUsed = false;
	DestroyAllEnemy();
	if (BoxCollision(pEnemy->obj.pos, pEnemy->hitboxSize, pPlayer->obj.pos, pPlayer->hitBoxSize))
	{
		BonusScore(10000);
		AddScore(-2);
	}
}

void Boss010B_Act(ENEMY* pEnemy)
{
	if (pEnemy->nCounterShoot % 30 == 0)
	{
		pEnemy->nCounterShoot = 0;

		SetEnemyBullet(
			ENEMYBULLET_TYPE_000,
			pEnemy->obj.pos,
			7.0f,
			pEnemy->fShootRot,
			3
		);
		SetEnemyBullet(
			ENEMYBULLET_TYPE_001,
			pEnemy->obj.pos,
			3.0f,
			pEnemy->fShootRot,
			15
		);
	}
}
