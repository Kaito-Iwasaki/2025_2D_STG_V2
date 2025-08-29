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
#include "baseScene.h"
#include "fade.h"
#include "title.h"
#include "game.h"

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
	InitFade(g_currentScene);
	//SetScene(g_currentScene);
}

//=====================================================================
// �I������
//=====================================================================
void UninitScene(void)
{
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
}

//=====================================================================
// �V�[���ݒ菈��
//=====================================================================
SCENE SetScene(SCENE nextScene)
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