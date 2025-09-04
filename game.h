//=====================================================================
//
// game.cpp�̃w�b�_�t�@�C�� [game.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "main.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define MAX_TIMELINE		(512)

//*********************************************************************
// 
// ***** �񋓌^��` *****
// 
//*********************************************************************
typedef enum
{
	GAMESTATE_NORMAL = 0,
	GAMESTATE_END,
	GAMESTATE_MAX
}GAMESTATE;

//*********************************************************************
// 
// ***** �\���̒�` *****
// 
//*********************************************************************
typedef struct
{
	bool bSet;

	int nType;
	int nNumEnemy;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOffset;
	D3DXVECTOR3 move;
	int nLife;
	int nWave;
	int nCountTime;
	int nCountTimeOffset;
}TIMELINE;

typedef struct
{
	bool bPaused;
	int nCurrentWave;
	int nCountGameState;
	int nCountTimeline;
	GAMESTATE state;
}STAGE;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetWave(int nWave);
void TogglePause(bool bPause);

#endif