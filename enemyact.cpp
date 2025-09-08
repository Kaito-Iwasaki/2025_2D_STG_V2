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

void EnemyAct_00(ENEMY* pEnemy);

void (*g_aShootFunction[ENEMYTYPE_MAX])(ENEMY* pEnemy) = {
	EnemyAct_00,
};

void EnemyAct(ENEMY* pEnemy)
{
	g_aShootFunction[0](pEnemy);
}

void EnemyAct_00(ENEMY* pEnemy)
{
	pEnemy->obj.pos.y += pEnemy->move.y;
}