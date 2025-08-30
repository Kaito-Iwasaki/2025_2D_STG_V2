//=====================================================================
//
// ゲーム画面 [game.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "baseScene.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "font.h"

#include "decal.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "enemybullet.h"
#include "spriteEffect.h"

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
void InitGame(void)
{
	InitFont();
	InitDecal();
	InitPlayer();
	InitBullet();
	InitEnemy();
	InitEnemyBullet();
	InitSpriteEffect();

	SetDecal(
		DECAL_LABEL_NULL,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)
	);

	SetEnemy(ENEMYTYPE_000, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 200, 0.0f));
	SetEnemy(ENEMYTYPE_000, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 400, 0.0f));
	SetEnemy(ENEMYTYPE_000, D3DXVECTOR3(SCREEN_WIDTH / 2 + 100, 0 - 400, 0.0f));
	SetEnemy(ENEMYTYPE_000, D3DXVECTOR3(SCREEN_WIDTH / 2 - 100, 0 - 400, 0.0f));
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitGame(void)
{
	UninitFont();
	UninitDecal();
	UninitPlayer();
	UninitBullet();
	UninitEnemy();
	UninitEnemyBullet();
	UninitSpriteEffect();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGame(void)
{
	UpdateFont();
	UpdateDecal();
	UpdatePlayer();
	UpdateBullet();
	UpdateEnemy();
	UpdateEnemyBullet();
	UpdateSpriteEffect();

	if (GetKeyboardTrigger(DIK_RETURN))
	{
		SetFade(SCENE_GAME);
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGame(void)
{
	DrawDecal();
	DrawSpriteEffect();
	DrawEnemy();
	DrawEnemyBullet();
	DrawBullet();
	DrawPlayer();
	DrawFont();
}