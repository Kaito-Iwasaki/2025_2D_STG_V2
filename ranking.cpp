//=====================================================================
//
// �����L���O��� [ranking.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "baseScene.h"
#include "ranking.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "font.h"

#include "player.h"
#include "bullet.h"
#include "decal.h"

#include "util.h"
#include "bg.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define RANKING_FILEPATH		"data\\ranking.bin"
#define MAX_PLACE				(5)

#define INIT_RANKING			{ 10000, 5000, 3000, 2000, 1000 }

#define FADE_START				(600)

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
int compare(const void* arg1, const void* arg2);

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
int g_aRanking[MAX_PLACE] = INIT_RANKING;
int g_nCountStateRanking = 0;

//=====================================================================
// ����������
//=====================================================================
void InitRanking(void)
{
	char aString[MAX_PATH] = {};
	g_nCountStateRanking = 0;

	InitBackground();
	InitFont();

	PlaySound(SOUND_LABEL_BGM_BGM00);

	LoadBin(RANKING_FILEPATH, &g_aRanking[0], sizeof(int), MAX_PLACE);

	qsort(&g_aRanking[0], MAX_PLACE, sizeof(int), compare);

	SetFont(
		D3DXVECTOR3(GAME_SCREEN_START, 100.0f, 0.0f),
		D3DXVECTOR3(GAME_SCREEN_WIDTH, 100.0f, 0.0f),
		D3DXVECTOR3_ZERO,
		"RANKING",
		DT_CENTER
	);

	for (int nCount = 0; nCount < MAX_PLACE; nCount++)
	{
		sprintf(&aString[0], "%d", g_aRanking[nCount]);

		SetFont(
			D3DXVECTOR3(GAME_SCREEN_START, 250.0f + (nCount * 50.0f), 0.0f),
			D3DXVECTOR3(GAME_SCREEN_WIDTH, 100.0f, 0.0f),
			D3DXVECTOR3_ZERO,
			&aString[0],
			DT_CENTER
		);
	}
}

//=====================================================================
// �I������
//=====================================================================
void UninitRanking(void)
{
	UninitBackground();
	UninitFont();
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateRanking(void)
{
	UpdateBackground();
	UpdateFont();

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START))
	{
		SetFade(SCENE_TITLE);
	}

	g_nCountStateRanking++;

	if (g_nCountStateRanking > FADE_START)
	{
		SetFade(SCENE_TITLE);
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawRanking(void)
{
	DrawBackground();
	DrawFont();
}

void SaveScore(int nScore)
{
	qsort(&g_aRanking[0], MAX_PLACE, sizeof(int), compare);

	for (int nCount = 0; nCount < MAX_PLACE; nCount++)
	{
		if (g_aRanking[nCount] < nScore)
		{
			g_aRanking[nCount] = nScore;

			SaveBin(RANKING_FILEPATH, &g_aRanking[0], sizeof(int), MAX_PLACE);

			break;
		}
	}
}

//=====================================================================
// qsort�֐��̔��菈��
//=====================================================================
int compare(const void* arg1, const void* arg2)
{
	return (int)arg1 - (int)arg2;
}