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
#include "util.h"
#include "ranking.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define GAME_START		(160)
#define FADE_START		(240)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
STAGE g_stage;

const char* g_aStageFileName[STAGETYPE_MAX] = {
	"data/STAGE/stage01.txt",
	"data/STAGE/stage02.txt",
};

//=====================================================================
// 初期化処理
//=====================================================================
void InitGame(STAGETYPE stagetype)
{
	InitFont();
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

	memset(&g_stage, 0, sizeof(g_stage));
	g_stage.bPaused = false;
	g_stage.nCountGameState = 0;
	g_stage.nCountWaveTime = 0;
	g_stage.nCurrentWave = 0;
	g_stage.nCountTimeline = 0;
	g_stage.nMaxWave = 0;
	g_stage.state = GAMESTATE_READY;
	g_stage.currentMusic = SOUND_LABEL_BGM_STAGE04;
	g_stage.nCountElapsed = 0;
	g_stage.nBonus = 500;

	PlaySound(g_stage.currentMusic);

	SetDecal(
		DECAL_LABEL_NULL,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f),
		D3DXVECTOR3(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT, 0.0f),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)
	);

	SetBackgroundSpeed(20.0f);
	SetBackgroundSpeedMove(3.0f, 0.001f);

	LoadStage(g_aStageFileName[stagetype], &g_stage);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitGame(void)
{
	UninitFont();
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
	char aMessage[MAX_PATH];

#if _DEBUG
	if (GetKeyboardTrigger(DIK_F1))
	{
		SetFade(SCENE_RESULT);
	}
#endif

	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START))
	{
		if (g_stage.state == GAMESTATE_RESULT) return;

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
		UpdateHealthbar();
		UpdateEffect();
		UpdateParticle();

		// タイムライン読み込み
		for (int nCount = 0; nCount < MAX_TIMELINE; nCount++, pTimeline++)
		{
			if (g_stage.state == GAMESTATE_READY) break;
			if (pTimeline->bSet == false) continue;
			if (pTimeline->nWave != g_stage.nCurrentWave) continue;
			if (pTimeline->nCountTime != g_stage.nCountTimeline) continue;


			switch (pTimeline->eventType)
			{
			case EVENTTYPE_SETENEMY:
				ENEMY* pEnemy;

				pEnemy = SetEnemy(
					(ENEMYTYPE)pTimeline->nType,
					D3DXVECTOR3(pTimeline->pos.x, pTimeline->pos.y, 0.0f)
				);

				if (Magnitude(pTimeline->move) != 0)
				{
					pEnemy->move = pTimeline->move;
				}

				if (pTimeline->bInversed == true)
				{
					pEnemy->move.x *= -1;
				}

				break;

			case EVENTTYPE_SETMUSIC:
				StopSound(g_stage.currentMusic);
				g_stage.currentMusic = SOUND_LABEL(pTimeline->nType);
				PlaySound(g_stage.currentMusic);

				break;

			case EVENTTYPE_SETBGSPEED:
				SetBackgroundSpeedMove(pTimeline->move.y, BG_SCROLL_MOVE_SCALE);
				break;
			}


		}

		g_stage.nCountGameState++;
		switch (g_stage.state)
		{
		case GAMESTATE_READY:
			if (g_stage.nCountGameState > GAME_START)
			{
				g_stage.state = GAMESTATE_NORMAL;
				g_stage.nCountGameState = 0;

			}
			break;

		case GAMESTATE_NORMAL:
			if (GetEnemyLeft() > 0)
			{
				g_stage.nCountGameState = 0;
			}

			g_stage.nCountTimeline++;

			if (g_stage.nCountGameState > g_stage.nWaveInterval &&
				g_stage.nCountTimeline > 60)
			{
				SetWave(g_stage.nCurrentWave + 1);
			}
			break;

		case GAMESTATE_END:
			if (g_stage.nCountGameState > FADE_START)
			{
				g_stage.nCountGameState = 0;
				g_stage.state = GAMESTATE_RESULT;

				sprintf(&aMessage[0], "YOUR SCORE : %d", GetScore());

				StopSound(g_stage.currentMusic);
				PlaySound(SOUND_LABEL_SE_GAMEOVER);

				SetFont(
					D3DXVECTOR3(640 - 270, SCREEN_HEIGHT / 2 - 50, 0.0f),
					D3DXVECTOR3(540, 200, 0.0f),
					D3DXVECTOR3_ZERO,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					40,
					"GAME CLEAR",
					DT_CENTER
				);
				SetFont(
					D3DXVECTOR3(640 - 270, SCREEN_HEIGHT / 2, 0.0f),
					D3DXVECTOR3(540, 200, 0.0f),
					D3DXVECTOR3_ZERO,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					40,
					&aMessage[0],
					DT_CENTER
				);
			}
			break;

		case GAMESTATE_RESULT:
			if (g_stage.nCountGameState == 240)
			{
				PlaySound(SOUND_LABEL_SE_HIT02);
				SetPlayerState(PLAYERSTATE_DISAPPEAR);
				SetVibration(30000, 30000, 60);
			}

			if (g_stage.nCountGameState == 300)
			{
				SetFade(SCENE_RANKING);
				SaveScore(GetScore());
			}
		}

		g_stage.nCountElapsed++;
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
	DrawFont();

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
		if (GetPlayer()->nCountHit == 0)
		{
			BonusScore(10000, "NO HIT BONUS");
		}
		g_stage.state = GAMESTATE_END;
	}
	else
	{
		g_stage.nCurrentWave = nWave;
		g_stage.nCountGameState = 0;
		g_stage.nCountTimeline = 0;
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
		PauseSound(g_stage.currentMusic);
	}
	else
	{
		UnPauseSound(g_stage.currentMusic);
		SetPauseMenuCursor(0);
	}
}

STAGE* GetStage(void)
{
	return &g_stage;
}

//=====================================================================
// ゲーム状態取得処理処理
//=====================================================================
GAMESTATE GetGameState(void)
{
	return g_stage.state;
}

//=====================================================================
// ゲーム状態取得処理処理
//=====================================================================
void SetGameState(GAMESTATE state)
{
	g_stage.state = state;
}

