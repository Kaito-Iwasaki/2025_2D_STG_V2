//=====================================================================
//result
// �`���[�g���A�� [tutorial.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "baseScene.h"
#include "tutorial.h"
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
TUTORIALSTATE g_stateTutorial = TUTORIALSTATE_NORMAL;
int g_nCountStateTutorial = 0;

FONT* g_FontStartTutorial = NULL;

//=====================================================================
// ����������
//=====================================================================
void InitTutorial(void)
{
	g_stateTutorial = TUTORIALSTATE_NORMAL;
	g_nCountStateTutorial = 0;

	InitDecal();
	InitFont();

	SetDecal(
		DECAL_LABEL_TUTORIAL,
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXVECTOR3(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT, 0.0f),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
	);

	g_FontStartTutorial = SetFont(
		D3DXVECTOR3(GAME_SCREEN_START, 580.0f, 0.0f),
		D3DXVECTOR3(GAME_SCREEN_WIDTH, 400.0f, 0.0f),
		D3DXVECTOR3_ZERO,
		"PRESS ENTER\nOR\nPRESS START",
		DT_CENTER
	);
}

//=====================================================================
// �I������
//=====================================================================
void UninitTutorial(void)
{
	UninitFont();
	UninitDecal();
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateTutorial(void)
{
	switch (g_stateTutorial)
	{
	case TUTORIALSTATE_NORMAL:
		if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START))
		{
			if (GetFade() != FADE_NONE) break;

			g_stateTutorial = TUTORIALSTATE_START;
			g_FontStartTutorial->obj.bVisible = true;
			PlaySound(SOUND_LABEL_SE_HIT02);

			SetFade(SCENE_GAME);

			break;
		}

		if (g_nCountStateTutorial % 30 == 0)
		{
			g_FontStartTutorial->obj.bVisible ^= 1;
		}

		break;

	case TUTORIALSTATE_START:
		if (g_nCountStateTutorial % 3 == 0)
		{
			g_FontStartTutorial->obj.bVisible ^= 1;
		}

		break;
	}


	g_nCountStateTutorial++;
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawTutorial(void)
{
	DrawDecal();
	DrawFont();
}