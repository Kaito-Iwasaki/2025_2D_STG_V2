//=====================================================================
//
// ��ʏ��� [baseScene.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "sound.h"

#include "wallpaper.h"
#include "baseScene.h"
#include "fade.h"
#include "title.h"
#include "game.h"
#include "decal.h"

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
SCENE g_currentScene = SCENE_GAME;

//=====================================================================
// ����������
//=====================================================================
void InitScene(void)
{
	InitWallPaper();
	InitFade(g_currentScene);
}

//=====================================================================
// �I������
//=====================================================================
void UninitScene(void)
{
	UninitWallPaper();
	UninitTitle();
	UninitGame();
	UninitFade();
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateScene(void)
{
	switch (g_currentScene)
	{
	case SCENE_GAME:
		UpdateTitle();
		break;

	case SCENE_ANOTHER:
		UpdateGame();
		break;

	}

	UpdateFade();
}

//=====================================================================
// �X�V����
//=====================================================================
void DrawScene(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RECT rc = GAME_SCREEN_RECT;

	pDevice->SetScissorRect(&rc);

	DrawWallPaper();

	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);

	switch (g_currentScene)
	{
	case SCENE_GAME:
		DrawTitle();
		break;

	case SCENE_ANOTHER:
		DrawGame();
		break;
	}

	DrawFade();

	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

}

//=====================================================================
// �V�[���ݒ菈��
//=====================================================================
SCENE SetScene(SCENE nextScene, bool bStopSound)
{
	// ���݂̃V�[�����I��
	switch (g_currentScene)
	{
	case SCENE_GAME:
		UninitTitle();
		break;

	case SCENE_ANOTHER:
		UninitGame();
		break;
	}

	if (bStopSound)
	{
		StopSound();
	}

	// �V�K�V�[����������
	switch (nextScene)
	{
	case SCENE_GAME:
		InitTitle();
		break;

	case SCENE_ANOTHER:
		InitGame();
		break;
	}

	g_currentScene = nextScene;

	return g_currentScene;
}