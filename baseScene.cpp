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
#include "sound.h"

#include "wallpaper.h"
#include "baseScene.h"
#include "fade.h"
#include "title.h"
#include "game.h"
#include "decal.h"
#include "result.h"

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
SCENE g_currentScene = SCENE_TITLE;

//=====================================================================
// 初期化処理
//=====================================================================
void InitScene(void)
{
	InitWallPaper();
	InitFade(g_currentScene);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitScene(void)
{
	UninitWallPaper();
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
	case SCENE_TITLE:
		UpdateTitle();
		break;

	case SCENE_GAME:
		UpdateGame();
		break;

	case SCENE_RESULT:
		UpdateResult();
		break;

	}

	UpdateFade();
}

//=====================================================================
// 更新処理
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
	case SCENE_TITLE:
		DrawTitle();
		break;

	case SCENE_GAME:
		DrawGame();
		break;

	case SCENE_RESULT:
		DrawResult();
		break;
	}

	DrawFade();

	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

}

//=====================================================================
// シーン設定処理
//=====================================================================
SCENE SetScene(SCENE nextScene, bool bStopSound)
{
	// 現在のシーンを終了
	switch (g_currentScene)
	{
	case SCENE_TITLE:
		UninitTitle();
		break;

	case SCENE_GAME:
		UninitGame();
		break;

	case SCENE_RESULT:
		UninitResult();
		break;
	}

	if (bStopSound)
	{
		StopSound();
	}

	// 新規シーンを初期化
	switch (nextScene)
	{
	case SCENE_TITLE:
		InitTitle();
		break;

	case SCENE_GAME:
		InitGame();
		break;

	case SCENE_RESULT:
		InitResult();
		break;
	}

	g_currentScene = nextScene;

	return g_currentScene;
}