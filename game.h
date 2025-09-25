//=====================================================================
//
// game.cppのヘッダファイル [game.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"
#include "sound.h"
#include "item.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_TIMELINE		(1024)

//*********************************************************************
// 
// ***** 列挙型定義 *****
// 
//*********************************************************************
typedef enum
{
	GAMESTATE_READY = 0,
	GAMESTATE_NORMAL,
	GAMESTATE_CONTINUE,
	GAMESTATE_END,
	GAMESTATE_RESULT,
	GAMESTATE_MAX
}GAMESTATE;

typedef enum
{
	EVENTTYPE_SETENEMY = 0,
	EVENTTYPE_SETMUSIC,
	EVENTTYPE_SETBGSPEED,
	EVENTTYPE_MAX
}EVENTTYPE;

typedef enum
{
	STAGETYPE_01_EASY = 0,
	STAGETYPE_01_NORMAL,
	STAGETYPE_MAX
}STAGETYPE;

typedef enum
{
	DIFFCULITY_EASY = 0,
	DIFFCULITY_NORMAL,
	DIFFCULITY_MAX
}DIFFCULITY;

//*********************************************************************
// 
// ***** 構造体定義 *****
// 
//*********************************************************************
typedef struct
{
	bool bSet;
	EVENTTYPE eventType;

	int nType;
	int nNumEnemy;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOffset;
	D3DXVECTOR3 move;
	int nLife;
	int nWave;
	int nCountTime;
	int nCountTimeOffset;
	bool bInversed;
	ITEMTYPE item;
}TIMELINE;

typedef struct
{
	bool bPaused;
	int nCurrentWave;
	int nCountGameState;
	int nCountWaveTime;
	int nCountTimeline;
	int nCountElapsed;
	int nMaxWave;
	int nWaveInterval;
	SOUND_LABEL currentMusic;
	GAMESTATE state;
	TIMELINE timeline[MAX_TIMELINE];
	STAGETYPE currentStage;
	int nBonus;
	DIFFCULITY diffculity;
}STAGE;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetWave(int nWave);
void TogglePause(bool bPause);
STAGE* GetStage(void);
GAMESTATE GetGameState(void);
void SetGameState(GAMESTATE state);
void SetGameStage(STAGETYPE stagetype,  DIFFCULITY diffculity);

#endif