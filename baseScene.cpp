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
#include "title.h"
#include "game.h"

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
	UninitTitle();
	UninitGame();
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
		UpdateTitle();
		break;

	case SCENE_ANOTHER:
		UpdateGame();
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
		DrawTitle();
		break;

	case SCENE_ANOTHER:
		DrawGame();
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
		UninitTitle();
		break;

	case SCENE_ANOTHER:
		UninitGame();
		break;
	}

	// 新規シーンを初期化
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