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

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_WAVE		(5)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
TIMELINE g_timeline[MAX_TIMELINE];
SOUND_LABEL g_CurrentSound = SOUND_LABEL_BGM_STAGE02;
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
	
	g_stage.bPaused = false;
	g_stage.nCountGameState = 0;
	g_stage.nCurrentWave = 0;
	g_stage.nCountTimeline = 0;

	PlaySound(g_CurrentSound);

	SetDecal(
		DECAL_LABEL_NULL,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
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
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START))
	{
		PlaySound(SOUND_LABEL_SE_PAUSE, 0.25f);
		g_stage.bPaused ^= 1;

		if (g_stage.bPaused)
		{
			PauseSound(g_CurrentSound);
		}
		else
		{
			UnPauseSound(g_CurrentSound);
		}
	}

	if (g_stage.bPaused == false)
	{
		UpdateFont();
		UpdateDecal();
		UpdatePlayer();
		UpdateBullet();
		UpdateEnemy();
		UpdateEnemyBullet();
		UpdateSpriteEffect();
		UpdateScore();

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

		if (GetEnemyLeft() < 1)
		{
			g_stage.nCountGameState++;
		}
		else
		{
			g_stage.nCountGameState = 0;
		}

		g_stage.nCountTimeline++;

		if (g_stage.nCountGameState > 100)
		{
			SetWave(g_stage.nCurrentWave + 1);
		}
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGame(void)
{
	DrawDecal();
	DrawEnemy();
	DrawSpriteEffect();
	DrawEnemyBullet();
	DrawBullet();
	DrawPlayer();
	DrawFont();
	DrawScore();
}

void SetWave(int nWave)
{
	if (nWave > MAX_WAVE)
	{
		SetFade(SCENE_TITLE);
	}
	else
	{
		g_stage.nCurrentWave = nWave;
		g_stage.nCountGameState = 0;
		g_stage.nCountTimeline = 0;
	}
}
