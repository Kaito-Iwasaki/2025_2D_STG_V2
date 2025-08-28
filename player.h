//=====================================================================
//
// player.cppのヘッダファイル [player.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "baseObject.h"
#include "main.h"

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	PLAYERSTATE_NORMAL = 0,
	PLAYERSTATE_APPEAR,
	PLAYERSTATE_DAMAGED,
	PLAYERSTATE_DIED,
	PLAYERSTATE_END,
	PLAYERSTATE_MAX
}PLAYERSTATE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;
	int nTexPattern;
	int nCounterPattern;
	
	PLAYERSTATE state;
	float fSpeed;
	float fShootSpeed;
	float fLife;
	float fCharge;

	int nCounterState;
	int nCounterShoot;

}PLAYER;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER* GetPlayer(void);
void HitPlayer(void);

#endif