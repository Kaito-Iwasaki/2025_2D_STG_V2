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

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_TIMELINE		(512)

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
	GAMESTATE_MAX
}GAMESTATE;

typedef enum
{
	EVENTTYPE_SETENEMY = 0,
	EVENTTYPE_SETBOSS,
	EVENTTYPE_MAX
}EVENTTYPE;

typedef enum
{
	STAGETYPE_01 = 0,
	STAGETYPE_02,
	STAGETYPE_MAX
}STAGETYPE;

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
}TIMELINE;

typedef struct
{
	bool bPaused;
	int nCurrentWave;
	int nCountGameState;
	int nCountTimeline;
	int nCountElapsed;
	int nMaxWave;
	int nWaveInterval;
	SOUND_LABEL currentMusic;
	GAMESTATE state;
	TIMELINE timeline[MAX_TIMELINE];
	STAGETYPE currentStage;
}STAGE;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitGame(STAGETYPE stagetype = STAGETYPE_01);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetWave(int nWave);
void TogglePause(bool bPause);
GAMESTATE GetGameState(void);
void SetGameState(GAMESTATE state);

#endif