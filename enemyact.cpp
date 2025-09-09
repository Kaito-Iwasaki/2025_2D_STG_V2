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
#include "player.h"
#include "util.h"
#include "enemybullet.h"

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void Enemy000_Act(ENEMY* pEnemy);
void Enemy001_Act(ENEMY* pEnemy);
void Enemy002_Act(ENEMY* pEnemy);
void Enemy003_Act(ENEMY* pEnemy);
void Enemy004_Act(ENEMY* pEnemy);
void Enemy005_Act(ENEMY* pEnemy);
void Enemy006_Act(ENEMY* pEnemy);
void Enemy007_Act(ENEMY* pEnemy);
void Enemy008_Act(ENEMY* pEnemy);
void Boss000_Act(ENEMY* pEnemy);

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
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
	Boss000_Act,
}; // 敵の挙動処理関数

//*********************************************************************
// 
// ***** 敵の挙動処理 *****
// 
//*********************************************************************
void EnemyAct(ENEMY* pEnemy)
{
	if (g_aActFunction[pEnemy->type] == NULL) return;

	g_aActFunction[pEnemy->type](pEnemy);
}

//*********************************************************************
// 
// ***** タイプ別敵の挙動処理 *****
// 
//*********************************************************************
//=====================================================================
// カニ
//=====================================================================
void Enemy000_Act(ENEMY* pEnemy)
{
	pEnemy->obj.pos += pEnemy->move;

	if (pEnemy->nShot > 30) return;

	if (pEnemy->nCounterShoot % pEnemy->nShootInterval == 0)
	{
		pEnemy->nCounterShoot = 0;
		pEnemy->fShootRot = RandRange(-100, 100) * 0.01f;
		if (SetEnemyBullet(
			ENEMYBULLET_TYPE_001,
			pEnemy->obj.pos,
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
}

//=====================================================================
// 黒
//=====================================================================
void Enemy002_Act(ENEMY* pEnemy)
{
	pEnemy->obj.pos.y += pEnemy->move.y + 1.0f;

	if (pEnemy->nShot > 1) return;

	if (pEnemy->nCounterShoot % pEnemy->nShootInterval == 0)
	{
		pEnemy->nCounterShoot = 0;
		pEnemy->fShootRot = Direction(pEnemy->obj.pos, GetPlayer()->obj.pos);
		if (SetEnemyBullet(
			ENEMYBULLET_TYPE_002,
			pEnemy->obj.pos,
			pEnemy->fShootSpeed + 5,
			pEnemy->fShootRot))
		{
			SetEnemyBullet(
				ENEMYBULLET_TYPE_002,
				pEnemy->obj.pos,
				pEnemy->fShootSpeed + 5,
				pEnemy->fShootRot + D3DX_PI / 2);
			SetEnemyBullet(
				ENEMYBULLET_TYPE_002,
				pEnemy->obj.pos,
				pEnemy->fShootSpeed + 5,
				pEnemy->fShootRot - D3DX_PI / 2);
			pEnemy->nShot++;
		}
	}

	pEnemy->move.y += (0.0f - pEnemy->move.y) * 0.05f;
}

//=====================================================================
// 魚
//=====================================================================
void Enemy003_Act(ENEMY* pEnemy)
{
	if (pEnemy->nCounterState == 1)
	{
		pEnemy->obj.pos.x = RandRange(640 - 250, 640 + 250);
		pEnemy->move.y = RandRange(pEnemy->move.y - 1, pEnemy->move.y + 1);
	}

	pEnemy->obj.pos.y += pEnemy->move.y;
}

//=====================================================================
// 貝
//=====================================================================
void Enemy004_Act(ENEMY* pEnemy)
{
	pEnemy->fShootRot = 4.0f;
	pEnemy->fShootRot = Direction(pEnemy->obj.pos, GetPlayer()->obj.pos);

		pEnemy->fShootRot = 4.0f;
		pEnemy->fShootRot = Direction(pEnemy->obj.pos, GetPlayer()->obj.pos);

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
		pEnemy->fShootRot = Direction(pEnemy->obj.pos, GetPlayer()->obj.pos);
		pEnemy->nShot = 0;
		pEnemy->nMode++;

	case 2:	// 弾発射
		if (pEnemy->nCounterMode % pEnemy->nShootInterval != 0) break;

		SetEnemyBullet(
			ENEMYBULLET_TYPE_001,
			pEnemy->obj.pos,
			pEnemy->fShootSpeed + 5.0f,
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
	pEnemy->obj.pos.y += 1.0f;
}

//=====================================================================
// おっさん
//=====================================================================
void Enemy007_Act(ENEMY* pEnemy)
{

}

//=====================================================================
// さんかく
//=====================================================================
void Enemy008_Act(ENEMY* pEnemy)
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
				Direction(pEnemy->obj.pos, GetPlayer()->obj.pos),
				30
			);
			SetEnemyBullet(
				ENEMYBULLET_TYPE_000,
				pEnemy->obj.pos,
				pEnemy->fShootSpeed + 2,
				Direction(pEnemy->obj.pos, GetPlayer()->obj.pos) + 0.4f,
				30
			);
			SetEnemyBullet(
				ENEMYBULLET_TYPE_000,
				pEnemy->obj.pos,
				pEnemy->fShootSpeed + 2,
				Direction(pEnemy->obj.pos, GetPlayer()->obj.pos) - 0.4f,
				30
			);
		}
		pEnemy->fShootRot += 0.5f;
		SetEnemyBullet(
			ENEMYBULLET_TYPE_001,
			pEnemy->obj.pos,
			pEnemy->fShootSpeed,
			pEnemy->fShootRot,
			15
		);
		SetEnemyBullet(
			ENEMYBULLET_TYPE_001,
			pEnemy->obj.pos,
			pEnemy->fShootSpeed,
			pEnemy->fShootRot + D3DX_PI,
			15
		);
	}
}