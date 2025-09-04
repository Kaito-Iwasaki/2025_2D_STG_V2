//=====================================================================
//
// �Q�[����� [result.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
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
#include "ranking.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define FADE_START			(600)

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
int g_nCountStateResult = 0;


//=====================================================================
// ����������
//=====================================================================
void InitResult(void)
{
	char aMessage[MAX_PATH] = {};
	g_nCountStateResult = 0;

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
		SaveScore(GetScore());

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
// �I������
//=====================================================================
void UninitResult(void)
{
	UninitFont();
	UninitDecal();
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START))
	{
		SetFade(SCENE_RANKING);
	}

	g_nCountStateResult++;

	if (g_nCountStateResult > FADE_START)
	{
		SetFade(SCENE_RANKING);
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawResult(void)
{
	DrawDecal();
	DrawFont();
}