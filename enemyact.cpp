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

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
void (*g_aShootFunction[ENEMYTYPE_MAX])(ENEMY* pEnemy) = {
	Enemy000_Act,
	Enemy001_Act,
	Enemy002_Act,
	Enemy003_Act,
	Enemy004_Act,
	Enemy005_Act,
}; // 敵の挙動処理関数

//*********************************************************************
// 
// ***** 敵の挙動処理 *****
// 
//*********************************************************************
void EnemyAct(ENEMY* pEnemy)
{
	g_aShootFunction[pEnemy->type](pEnemy);
}

//*********************************************************************
// 
// ***** タイプ別敵の挙動処理 *****
// 
//*********************************************************************
void Enemy000_Act(ENEMY* pEnemy)
{
	pEnemy->obj.pos += pEnemy->move;

	if (pEnemy->nShot < 1) return;

	if (pEnemy->nCounterShoot % pEnemy->nShootInterval == 0)
	{
		pEnemy->nCounterShoot = 0;
		pEnemy->fShootRot = RandRange(-100, 100) * 0.01f;
		SetEnemyBullet(
			ENEMYBULLET_TYPE_001,
			pEnemy->obj.pos,
			pEnemy->fShootSpeed,
			pEnemy->fShootRot);
	}
}

void Enemy001_Act(ENEMY* pEnemy)
{
	pEnemy->obj.pos.x = pEnemy->startPos.x + sin((float)pEnemy->nCounterState * pEnemy->move.x) * pEnemy->move.z;
	pEnemy->obj.pos.y += pEnemy->move.y;
}

void Enemy002_Act(ENEMY* pEnemy)
{
	
}

void Enemy003_Act(ENEMY* pEnemy)
{
	
}

void Enemy004_Act(ENEMY* pEnemy)
{

}

void Enemy005_Act(ENEMY* pEnemy)
{

}