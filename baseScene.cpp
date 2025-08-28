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
#include "game.h"
#include "another.h"

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
	UninitGame();
	UninitAnother();
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
		UpdateGame();
		break;

	case SCENE_ANOTHER:
		UpdateAnother();
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
		DrawGame();
		break;

	case SCENE_ANOTHER:
		DrawAnother();
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
		UninitGame();
		break;

	case SCENE_ANOTHER:
		UninitAnother();
		break;
	}

	// �V�K�V�[����������
	switch (nextScene)
	{
	case SCENE_GAME:
		InitGame();
		break;

	case SCENE_ANOTHER:
		InitAnother();
		break;
	}

	g_currentScene = nextScene;

	return g_currentScene;
}