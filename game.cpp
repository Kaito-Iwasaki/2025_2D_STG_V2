//=====================================================================
//
// ゲーム画面 [game.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "baseScene.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "font.h"
#include "stage_loader.h"

#include "decal.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "enemybullet.h"
#include "spriteEffect.h"
#include "score.h"
#include "pause.h"
#include "bg.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_WAVE		(10)
#define WAVE_START		(0)
#define WAVE_INTERVAL	(10)
#define FADE_START		(120)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
TIMELINE g_timeline[MAX_TIMELINE];
SOUND_LABEL g_CurrentSound = SOUND_LABEL_BGM_STAGE04;
STAGE g_stage;

//=====================================================================
// 初期化処理
//=====================================================================
void InitGame(void)
{
	InitFont();
	InitDecal();
	InitPlayer();
	InitBullet();
	InitEnemy();
	InitEnemyBullet();
	InitSpriteEffect();
	InitScore();
	InitPause();
	InitBackground();

	g_stage.bPaused = false;
	g_stage.nCountGameState = 0;
	g_stage.nCurrentWave = WAVE_START;
	g_stage.nCountTimeline = 0;
	g_stage.state = GAMESTATE_NORMAL;

	PlaySound(g_CurrentSound);

	SetDecal(
		DECAL_LABEL_NULL,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f),
		D3DXVECTOR3(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT, 0.0f),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)
	);

	LoadStage("data/STAGE/stage01.txt", &g_timeline[0]);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitGame(void)
{
	UninitFont();
	UninitDecal();
	UninitPlayer();
	UninitBullet();
	UninitEnemy();
	UninitEnemyBullet();
	UninitSpriteEffect();
	UninitScore();
	UninitPause();
	UninitBackground();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGame(void)
{
#if _DEBUG
	if (GetKeyboardTrigger(DIK_F1))
	{
		SetFade(SCENE_RESULT);
	}
#endif

	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START))
	{
		TogglePause(g_stage.bPaused ^ 1);
	}

	if (g_stage.bPaused == false)
	{
		UpdateFont();
		UpdatePlayer();
		UpdateBullet();
		UpdateEnemy();
		UpdateEnemyBullet();
		UpdateSpriteEffect();
		UpdateScore();
		UpdateBackground();

		for (int nCount = 0; nCount < MAX_TIMELINE; nCount++)
		{
			if (g_timeline[nCount].bSet == false) continue;
			if (g_timeline[nCount].nWave != g_stage.nCurrentWave) continue;
			if (g_timeline[nCount].nCountTime != g_stage.nCountTimeline) continue;

			SetEnemy(
				(ENEMYTYPE)g_timeline[nCount].nType,
				D3DXVECTOR3(g_timeline[nCount].pos.x, g_timeline[nCount].pos.y, 0.0f)
			);
		}

		switch (g_stage.state)
		{
		case GAMESTATE_NORMAL:
			if (GetEnemyLeft() < 1)
			{
				g_stage.nCountGameState++;
			}
			else
			{
				g_stage.nCountGameState = 0;
			}

			g_stage.nCountTimeline++;

			if (g_stage.nCountGameState > WAVE_INTERVAL)
			{
				SetWave(g_stage.nCurrentWave + 1);
			}
			break;

		case GAMESTATE_END:
			g_stage.nCountGameState++;

			if (g_stage.nCountGameState > FADE_START)
			{
				SetFade(SCENE_RESULT);
			}

			break;
		}
	}
	else
	{
		UpdatePause();
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGame(void)
{
	DrawBackground();
	DrawEnemy();
	DrawSpriteEffect();
	DrawEnemyBullet();
	DrawBullet();
	DrawPlayer();
	DrawFont();
	DrawScore();

	if (g_stage.bPaused)
	{
		DrawPause();
	}
}

//=====================================================================
// ウェーブ設定処理
//=====================================================================
void SetWave(int nWave)
{
	if (nWave > MAX_WAVE)
	{
		g_stage.state = GAMESTATE_END;
	}
	else
	{
		g_stage.nCurrentWave = nWave;
		g_stage.nCountGameState = 0;
		g_stage.nCountTimeline = 0;

		if (nWave == MAX_WAVE)
		{
			StopSound(g_CurrentSound);
			g_CurrentSound = SOUND_LABEL_BGM_BOSS00;
			PlaySound(g_CurrentSound);
		}
	}
}

//=====================================================================
// ポーズ処理
//=====================================================================
void TogglePause(bool bPause)
{
	if (GetFade() != FADE_NONE) return;	// フェード中は処理しない

	PlaySound(SOUND_LABEL_SE_PAUSE, 0.25f);
	g_stage.bPaused = bPause;

	if (g_stage.bPaused)
	{
		PauseSound(g_CurrentSound);
	}
	else
	{
		UnPauseSound(g_CurrentSound);
		SetPauseMenuCursor(0);
	}
}
