//=====================================================================
// 
// ENEMYACT.cppのヘッダファイル [EnemyAct.h]
// Author:
//
//=====================================================================
#ifndef _ENEMYACT_H_
#define _ENEMYACT_H_		// 二重インクルード防止のマクロ

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"
#include "enemy.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void EnemyAct(ENEMY* pEnemy);

void Enemy000_Act(ENEMY* pEnemy);
void Enemy001_Act(ENEMY* pEnemy);
void Enemy002_Act(ENEMY* pEnemy);
void Enemy003_Act(ENEMY* pEnemy);
void Enemy004_Act(ENEMY* pEnemy);
void Enemy005_Act(ENEMY* pEnemy);
void Enemy006_Act(ENEMY* pEnemy);
void Enemy006_Died(ENEMY* pEnemy);
void Enemy007_Act(ENEMY* pEnemy);
void Enemy008_Act(ENEMY* pEnemy);
void Enemy009_Act(ENEMY* pEnemy);

void BonusEnemy_Died(ENEMY* pEnemy);

void Boss000_Act(ENEMY* pEnemy);
void Boss000_Died(ENEMY* pEnemy);

void Boss010A_Act(ENEMY* pEnemy);
void Boss010A_Died(ENEMY* pEnemy);

void Boss010B_Act(ENEMY* pEnemy);

#endif