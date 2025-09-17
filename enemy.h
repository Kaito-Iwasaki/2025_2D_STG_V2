//=====================================================================
//
// enemy.cppのヘッダファイル [enemy.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "baseObject.h"
#include "main.h"
#include "game.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_ENEMY		(256)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	ENEMYTYPE_000 = 0,
	ENEMYTYPE_001,
	ENEMYTYPE_002,
	ENEMYTYPE_003,
	ENEMYTYPE_004,
	ENEMYTYPE_005,
	ENEMYTYPE_006,
	ENEMYTYPE_007,
	ENEMYTYPE_008,
	ENEMYTYPE_009,
	ENEMYTYPE_BOSS000,
	ENEMYTYPE_BOSS001A,
	ENEMYTYPE_BOSS001B,
	ENEMYTYPE_MAX
}ENEMYTYPE;

typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGED,
	ENEMYSTATE_DIED,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct ENEMY
{
	BASEOBJECT obj;
	bool bUsed;
	D3DXVECTOR3 startPos;
	bool bActEnabled;
	bool bHitEnabled;
	bool bThroughBullet;

	D3DXVECTOR3 move;
	ENEMYTYPE type;
	ENEMYSTATE state;
	int nMode;
	float fLife;
	int nShot;
	int nShootInterval;
	float fShootSpeed;
	float fShootRot;
	int nScore;
	DWORD disapperFlags;
	D3DXVECTOR3 hitboxSize;

	int nCounterState;
	int nCounterShoot;
	int nCounterMode;

	void (*pfDied)(ENEMY* pEnemy);
}ENEMY;

typedef struct
{
	D3DXVECTOR3 size;
	D3DXVECTOR3 move;
	
	float fLife;
	int nScore;
	void (*pfDied)(ENEMY* pEnemy);
}ENEMYINFO;


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY* GetEnemy(void);
int GetEnemyLeft(void);
ENEMY* SetEnemy(ENEMYTYPE type, D3DXVECTOR3 pos);
ENEMY* SetEnemy(TIMELINE* pTimeline);
void HitEnemy(ENEMY* pEnemy, float fDamage = 1.0f);
void DestroyAllEnemy();

#endif