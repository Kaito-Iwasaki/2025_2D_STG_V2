//=====================================================================
//
// ゲーム画面 [result.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "baseScene.h"
#include "result.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "font.h"

#include "player.h"
#include "bullet.h"
#include "decal.h"

#include "util.h"
#include "score.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************

//=====================================================================
// 初期化処理
//=====================================================================
void InitResult(void)
{
	char aMessage[MAX_PATH];

	InitDecal();
	InitFont();

	SetDecal(
		DECAL_LABEL_NULL,
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXVECTOR3(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT, 0.0f),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)
	);

	if (GetPlayer()->fLife > 0)
	{
		sprintf(aMessage, "YOUR SCORE : %d", GetScore());

		SetFont(
			D3DXVECTOR3(640 - 270, SCREEN_HEIGHT / 2 - 50, 0.0f),
			D3DXVECTOR3(540, 200, 0.0f),
			D3DXVECTOR3_ZERO,
			"GAME CLEAR",
			DT_CENTER
		);
		SetFont(
			D3DXVECTOR3(640 - 270, SCREEN_HEIGHT / 2, 0.0f),
			D3DXVECTOR3(540, 200, 0.0f),
			D3DXVECTOR3_ZERO,
			&aMessage[0],
			DT_CENTER
		);
	}
	else
	{
		SetFont(
			D3DXVECTOR3(640 - 270, SCREEN_HEIGHT / 2 - 50, 0.0f),
			D3DXVECTOR3(540, 200, 0.0f),
			D3DXVECTOR3_ZERO,
			"GAME OVER",
			DT_CENTER
		);
	}


	PlaySound(SOUND_LABEL_SE_GAMEOVER);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitResult(void)
{
	UninitFont();
	UninitDecal();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START))
	{
		SetFade(SCENE_TITLE);
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawResult(void)
{
	DrawDecal();
	DrawFont();
}