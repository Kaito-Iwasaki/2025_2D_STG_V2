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
#include "healthbar.h"
#include "effect.h"
#include "particle.h"
#include "keylogger.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define GAME_START		(160)
#define FADE_START		(200)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
STAGE g_stage;
SOUND_LABEL g_CurrentSound = SOUND_LABEL_BGM_STAGE04;
int g_nElapsedFrame = 0;

//=====================================================================
// 初期化処理
//=====================================================================
void InitGame(void)
{
	InitPlayer();
	InitBullet();
	InitEnemy();
	InitEnemyBullet();
	InitSpriteEffect();
	InitScore();
	InitPause();
	InitBackground();
	InitHealthbar();
	InitEffect();
	InitParticle();

	g_stage.bPaused = false;
	g_stage.nCountGameState = 0;
	g_stage.nCurrentWave = 0;
	g_stage.nCountTimeline = 0;
	g_stage.nMaxWave = 0;
	g_stage.state = GAMESTATE_READY;
	g_CurrentSound = SOUND_LABEL_BGM_STAGE04;
	g_nElapsedFrame = 0;

	PlaySound(g_CurrentSound);

	SetDecal(
		DECAL_LABEL_NULL,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f),
		D3DXVECTOR3(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT, 0.0f),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)
	);

	SetBackgroundSpeed(20.0f);
	SetBackgroundSpeedMove(1.0f, 0.001f);

	LoadStage("data/STAGE/stage01.txt", &g_stage);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitGame(void)
{
	UninitPlayer();
	UninitBullet();
	UninitEnemy();
	UninitEnemyBullet();
	UninitSpriteEffect();
	UninitScore();
	UninitPause();
	UninitBackground();
	UninitHealthbar();
	UninitEffect();
	UninitParticle();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGame(void)
{
	TIMELINE* pTimeline = &g_stage.timeline[0];

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
		UpdatePlayer();
		UpdateBullet();
		UpdateEnemy();
		UpdateEnemyBullet();
		UpdateSpriteEffect();
		UpdateScore();
		UpdateBackground();
		UpdateHealthbar();
		UpdateEffect();
		UpdateParticle();

		for (int nCount = 0; nCount < MAX_TIMELINE; nCount++, pTimeline++)
		{
			if (g_stage.state == GAMESTATE_READY) break;
			if (pTimeline->bSet == false) continue;
			if (pTimeline->nWave != g_stage.nCurrentWave) continue;
			if (pTimeline->nCountTime != g_stage.nCountTimeline) continue;

			ENEMY* pEnemy;

			pEnemy = SetEnemy(
				(ENEMYTYPE)pTimeline->nType,
				D3DXVECTOR3(pTimeline->pos.x, pTimeline->pos.y, 0.0f)
			);

			if (pTimeline->bInversed == true)
			{
				pEnemy->move.x *= -1;
			}
		}

		switch (g_stage.state)
		{
		case GAMESTATE_READY:
			g_stage.nCountGameState++;
			if (g_stage.nCountGameState > GAME_START)
			{
				g_stage.state = GAMESTATE_NORMAL;
				g_stage.nCountGameState = 0;

			}
			break;

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

			if (g_stage.nCountGameState > g_stage.nWaveInterval)
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

		g_nElapsedFrame++;
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
	DrawEffect();
	DrawPlayer();
	DrawScore();
	DrawHealthbar();

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
	if (nWave > g_stage.nMaxWave)
	{
		g_stage.state = GAMESTATE_END;
	}
	else
	{
		g_stage.nCurrentWave = nWave;
		g_stage.nCountGameState = 0;
		g_stage.nCountTimeline = 0;

		if (nWave == g_stage.nMaxWave)
		{
			SetBackgroundSpeedMove(10.0f, BG_SCROLL_MOVE_SCALE);
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

//=====================================================================
// ゲーム状態設定処理
//=====================================================================
GAMESTATE GetGameState(void)
{
	return g_stage.state;
}
