//=====================================================================
//
// ゲーム画面 [title.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "baseScene.h"
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "player.h"
#include "bullet.h"
#include "decal.h"

#include "util.h"
#include "bg.h"
#include "font.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define FADE_START					(800)
#define INTRO_END					(310)

#define PLAYER_START_Y		(SCREEN_HEIGHT + 50)
#define PLAYER_GOAL_Y		(SCREEN_VCENTER + 100)
#define LOGO_START_Y		(-200)
#define LOGO_GOAL_Y			(100)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
int g_nCountStateTitle = 0;
TITLESTATE g_stateTitle = TITLESTATE_INTRO;

DECAL* g_DecalPlayer;
DECAL* g_DecalLogo;
FONT* g_FontStart;

//=====================================================================
// 初期化処理
//=====================================================================
void InitTitle(void)
{
	InitDecal();
	InitBackground();
	InitFont();

	g_nCountStateTitle = 0;
	g_stateTitle = TITLESTATE_INTRO;

	SetBackgroundSpeed(7.0f);

	PlaySound(SOUND_LABEL_BGM_TITLE00);

	g_DecalPlayer = SetDecal(
		DECAL_LABEL_PLAYER,
		D3DXVECTOR3(SCREEN_CENTER, PLAYER_START_Y, 0.0f),
		D3DXVECTOR3(64, 64, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
	);

	g_DecalLogo = SetDecal(
		DECAL_LABEL_LOGO,
		D3DXVECTOR3(SCREEN_CENTER, LOGO_START_Y, 0.0f),
		D3DXVECTOR3(600, 200, 0) * 0.8f,
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
	);

	g_FontStart = SetFont(
		D3DXVECTOR3(GAME_SCREEN_START, 520.0f, 0.0f),
		D3DXVECTOR3(GAME_SCREEN_WIDTH, 400.0f, 0.0f),
		D3DXVECTOR3_ZERO,
		"PRESS ENTER\nOR\nPRESS START",
		DT_CENTER
	);
	g_FontStart->obj.bVisible = false;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitTitle(void)
{
	UninitDecal();
	UninitBackground();
	UninitFont();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateTitle(void)
{
	UpdateBackground();
	UpdateFont();
	UpdateDecal();


	switch (g_stateTitle)
	{
	case TITLESTATE_INTRO:
		g_DecalPlayer->obj.pos.y -= 1.0f;
		if (g_DecalPlayer->obj.pos.y <= PLAYER_GOAL_Y)
		{
			g_DecalPlayer->obj.pos.y = PLAYER_GOAL_Y;
		}

		g_DecalLogo->obj.pos.y += 1.0f;
		if (g_DecalLogo->obj.pos.y >= LOGO_GOAL_Y)
		{
			g_DecalLogo->obj.pos.y = LOGO_GOAL_Y;
		}

		if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START) || g_nCountStateTitle > INTRO_END)
		{
			g_nCountStateTitle = 0;
			g_stateTitle = TITLESTATE_NORMAL;
			g_FontStart->obj.bVisible = true;
		}
		break;

	case TITLESTATE_NORMAL:
		g_DecalPlayer->obj.pos.y = PLAYER_GOAL_Y;
		g_DecalLogo->obj.pos.y = LOGO_GOAL_Y;

		if (g_nCountStateTitle % 30 == 0)
		{
			g_FontStart->obj.bVisible ^= 1;
		}

		if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START))
		{
			PlaySound(SOUND_LABEL_SE_HIT02);
			g_nCountStateTitle = 0;
			g_stateTitle = TITLESTATE_START;
		}

		if (g_nCountStateTitle > FADE_START)
		{
			SetFade(SCENE_RANKING);
		}

		break;

	case TITLESTATE_START:
		if (g_nCountStateTitle % 3 == 0)
		{
			g_FontStart->obj.bVisible ^= 1;
		}

		if (g_nCountStateTitle > 60)
		{
			SetFade(SCENE_TUTORIAL, false);
		}

		g_DecalPlayer->obj.pos.y -= 15.0f;

		break;
	}
	g_nCountStateTitle++;
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawTitle(void)
{
	DrawBackground();
	DrawDecal();
	DrawFont();
}