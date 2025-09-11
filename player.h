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
// ***** マクロ定義 *****
// 
//*********************************************************************
#define PLAYER_SPEED			(8.0f)
#define PLAYER_SHOOT_SPEED		(35.0f)
#define PLAYER_SHOOT_INTERVAL	(6)
#define PLAYER_HEAL_MAX			(2)
#define PLAYER_HEAL_SCALE		(0.0005f)
#define PLAYER_HITBOX_SIZE		D3DXVECTOR3(24.0f, 24.0f, 0.0f)


//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	PLAYERSTATE_NORMAL = 0,
	PLAYERSTATE_BLINK,
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
	D3DXVECTOR3 hitBoxSize;

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