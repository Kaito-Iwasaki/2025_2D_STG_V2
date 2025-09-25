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
#include "effect.h"
#include "particle.h"
#include "game.h"

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

#define COLOR_SELECTED		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_DESELECTED	D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
int g_nCountStateTitle = 0;
int g_nCounterTitle = 0;
TITLESTATE g_stateTitle = TITLESTATE_INTRO;
int nDifSelect = DIFFCULITY_EASY;

DECAL* g_DecalPlayer;
DECAL* g_DecalLogo;
FONT* g_FontStart;
FONT* g_FontCredit;
FONT* g_FontEasy;
FONT* g_FontNormal;

//=====================================================================
// 初期化処理
//=====================================================================
void InitTitle(void)
{
	InitDecal();
	InitBackground();
	InitFont();
	InitEffect();
	InitParticle();

	g_nCounterTitle = 0;
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
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		40,
		"PRESS ENTER\nOR\nPRESS START",
		DT_CENTER
	);
	g_FontStart->obj.bVisible = false;

	g_FontCredit = SetFont(
		D3DXVECTOR3(GAME_SCREEN_START, 690.0f, 0.0f),
		D3DXVECTOR3(GAME_SCREEN_WIDTH, 400.0f, 0.0f),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		25,
		"(C) 2025 KAITO IWASAKI ",
		DT_CENTER
	);
	g_FontCredit->obj.bVisible = false;

	g_FontEasy = SetFont(
		D3DXVECTOR3(SCREEN_CENTER - 150.0f, 645.0f, 0.0f),
		D3DXVECTOR3(150.0f, 400.0f, 0.0f),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		35,
		"EASY",
		DT_CENTER
	);
	g_FontEasy->obj.bVisible = false;

	g_FontNormal = SetFont(
		D3DXVECTOR3(SCREEN_CENTER, 645.0f, 0.0f),
		D3DXVECTOR3(150.0f, 400.0f, 0.0f),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		35,
		"NORMAL",
		DT_CENTER
	);
	g_FontNormal->obj.bVisible = false;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitTitle(void)
{
	UninitDecal();
	UninitBackground();
	UninitFont();
	UninitEffect();
	UninitParticle();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateTitle(void)
{
	UpdateBackground();
	UpdateFont();
	UpdateDecal();
	UpdateEffect();
	UpdateParticle();

	EFFECTINFO info;

	g_FontEasy->obj.color = COLOR_DESELECTED;
	g_FontNormal->obj.color = COLOR_DESELECTED;

	if (nDifSelect == DIFFCULITY_EASY)
	{
		g_FontEasy->obj.color = COLOR_SELECTED;
	}
	else if (nDifSelect == DIFFCULITY_NORMAL)
	{
		g_FontNormal->obj.color = COLOR_SELECTED;
	}

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
			g_FontCredit->obj.bVisible = true;
			g_FontEasy->obj.bVisible = true;
			g_FontNormal->obj.bVisible = true;
		}
		break;

	case TITLESTATE_NORMAL:
		g_DecalPlayer->obj.pos.y = PLAYER_GOAL_Y;
		g_DecalLogo->obj.pos.y = LOGO_GOAL_Y;

		if (g_nCountStateTitle % 30 == 0)
		{
			g_FontStart->obj.bVisible ^= 1;
		}

		if (GetFade() != FADE_NONE) break;

		if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START))
		{
			PlaySound(SOUND_LABEL_SE_HIT02);
			g_nCountStateTitle = 0;
			g_stateTitle = TITLESTATE_START;
			SetVibration(30000, 30000, 60);
			g_FontEasy->obj.bVisible = false;
			g_FontNormal->obj.bVisible = false;
		}

		if (
			GetKeyboardRepeat(DIK_A) ||
			GetKeyboardRepeat(DIK_D) ||
			GetKeyboardRepeat(DIK_SPACE) || 
			GetKeyboardRepeat(DIK_LEFT) || 
			GetKeyboardRepeat(DIK_RIGHT) ||
			GetJoypadRepeat(JOYKEY_A) ||
			GetJoypadRepeat(JOYKEY_BACK) ||
			GetJoypadRepeat(JOYKEY_LEFT) ||
			GetJoypadRepeat(JOYKEY_RIGHT) ||
			GetJoystickRepeat(JOYSTICK_L_LEFT) ||
			GetJoystickRepeat(JOYSTICK_L_RIGHT)
			)
		{
			nDifSelect = (nDifSelect + 1) % DIFFCULITY_MAX;
			PlaySound(SOUND_LABEL_SE_DECISION);
			g_nCounterTitle = 0;
		}

		if (g_nCounterTitle > FADE_START)
		{
			SetFade(SCENE_RANKING);
		}

		g_nCounterTitle++;

		break;

	case TITLESTATE_START:
		if (g_nCountStateTitle % 3 == 0)
		{
			g_FontStart->obj.bVisible ^= 1;

			switch (nDifSelect)
			{
			case DIFFCULITY_EASY:
				g_FontEasy->obj.bVisible ^= 1;
				break;

			case DIFFCULITY_NORMAL:
				g_FontNormal->obj.bVisible ^= 1;
				break;
			}
		}

		if (g_nCountStateTitle > 60)
		{
			SetFade(SCENE_TUTORIAL, false);
		}

		g_DecalPlayer->obj.pos.y -= 15.0f;

		info.fSpeed = 1.0f;
		info.fRotSpeed = 0.05f;
		info.fMaxScale = 0.3f;
		info.nMaxLife = 300;
		info.col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);

		SetParticle(
			info,
			g_DecalPlayer->obj.pos,
			0,
			0.8f,
			1,
			6
		);

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
	DrawEffect();
	DrawDecal();
	DrawFont();
}

DIFFCULITY GetDiffculity(void)
{
	return (DIFFCULITY)nDifSelect;
}
