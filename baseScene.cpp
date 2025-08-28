//=====================================================================
//
// 画面処理 [baseScene.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "baseScene.h"
#include "fade.h"
#include "game.h"
#include "another.h"

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
SCENE g_currentScene = SCENE_GAME;

//=====================================================================
// 初期化処理
//=====================================================================
void InitScene(void)
{
	InitFade(g_currentScene);
	//SetScene(g_currentScene);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitScene(void)
{
	UninitGame();
	UninitAnother();
	UninitFade();
}

//=====================================================================
// 更新処理
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
// 更新処理
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
// シーン設定処理
//=====================================================================
SCENE SetScene(SCENE nextScene)
{
	// 現在のシーンを終了
	switch (g_currentScene)
	{
	case SCENE_GAME:
		UninitGame();
		break;

	case SCENE_ANOTHER:
		UninitAnother();
		break;
	}

	// 新規シーンを初期化
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