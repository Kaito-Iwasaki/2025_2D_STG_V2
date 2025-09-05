//=====================================================================
//
// プレイヤーオブジェクト [player.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "input.h"
#include "sound.h"
#include "util.h"
	
#include "baseScene.h"
#include "collision.h"
#include "player.h"
#include "bullet.h"
#include "enemybullet.h"
#include "spriteEffect.h"
#include "fade.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\player000.png"
#define NUM_TEXTURE				(2)

#define INIT_POS				{SCREEN_WIDTH / 2, SCREEN_HEIGHT/ 2 + 200, 0.0f}
#define INIT_SIZE				{64.0f, 64.0f, 0.0f}
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void SetPlayerState(PLAYERSTATE state);

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffPlayer = NULL;
PLAYER g_player;

//=====================================================================
// 初期化処理
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス

	// 構造体の初期化
	memset(&g_player, 0, sizeof(PLAYER));
	g_player.obj.pos = INIT_POS;
	g_player.obj.size = INIT_SIZE;
	g_player.obj.color = INIT_COLOR;
	g_player.obj.bVisible = true;

	g_player.state = PLAYERSTATE_DAMAGED;
	g_player.fSpeed = INIT_PLAYER_SPEED;
	g_player.fShootSpeed = INIT_SHOOT_SPEED;
	g_player.fLife = INIT_PLAYER_LIFE;
	g_player.fCharge = INIT_PLAYER_CHARGE;
	g_player.hitBoxSize = PLAYER_HITBOX_SIZE;

	// テクスチャの読み込み
	if (TEXTURE_FILENAME)
	{// テクスチャ作成
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffPlayer
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitPlayer(void)
{
	if (g_pVtxBuffPlayer != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	if (g_pTexBuffPlayer != NULL)
	{// テクスチャバッファの破棄
		g_pTexBuffPlayer->Release();
		g_pTexBuffPlayer = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdatePlayer(void)
{
	RECT rectScreen = GAME_SCREEN_RECT;
	D3DXVECTOR3 direction = D3DXVECTOR3_ZERO;
	float fMagnitude;
	XINPUT_GAMEPAD* pGamepad = &GetJoypad()->Gamepad;

	// ***** 状態別処理 *****
	g_player.nCounterState++;
	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:
		g_player.obj.bVisible = true;
		if (g_player.fLife < PLAYER_HEAL_MAX)
		{
			g_player.fLife += PLAYER_HEAL_SCALE;
		}
		break;

	case PLAYERSTATE_APPEAR:
		if (g_player.nCounterState > 60)
		{
			SetPlayerState(PLAYERSTATE_NORMAL);
		}
		break;

	case PLAYERSTATE_DAMAGED:
		if (g_player.nCounterState % 3 == 0)
		{
			g_player.obj.bVisible ^= 1;
		}

		if (g_player.nCounterState > 120)
		{
			SetPlayerState(PLAYERSTATE_NORMAL);
		}
		break;

	case PLAYERSTATE_DIED:
		g_player.obj.bVisible = false;
		if (g_player.nCounterState > 120)
		{
			SetPlayerState(PLAYERSTATE_END);
		}
		return;

	case PLAYERSTATE_END:
		SetFade(SCENE_RESULT);
		return;
	}

	// ***** 移動 *****
	// キーボード&ジョイパッド操作
	if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LEFT) || GetJoystickPress(JOYSTICK_L_LEFT))
	{// 左
		direction.x -= 1;
	}
	if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RIGHT) || GetJoystickPress(JOYSTICK_L_RIGHT))
	{// 右
		direction.x += 1;
		//g_player.obj.bInversed = false;
	}
	if (GetKeyboardPress(DIK_W) || GetJoypadPress(JOYKEY_UP) || GetJoystickPress(JOYSTICK_L_UP))
	{// 上
		direction.y -= 1;
	}
	if (GetKeyboardPress(DIK_S) || GetJoypadPress(JOYKEY_DOWN) || GetJoystickPress(JOYSTICK_L_DOWN))
	{// 下
		direction.y += 1;
	}

	// 移動方向に応じてテクスチャパターンを設定
	g_player.nTexPattern = Clamp(direction.x, -1, 1);
	g_player.obj.bInversed = g_player.nTexPattern == -1;

	// 方向の大きさを求める
	fMagnitude = sqrtf(direction.x * direction.x + direction.y * direction.y);

	if (fMagnitude != 0)
	{// 位置を更新
		g_player.obj.pos += D3DXVECTOR3(direction.x / fMagnitude, direction.y / fMagnitude, 0.0f) * g_player.fSpeed;
	}

	// 位置制限
	Clampf(&g_player.obj.pos.x, rectScreen.left + g_player.obj.size.x / 2, rectScreen.right - g_player.obj.size.x / 2);
	Clampf(&g_player.obj.pos.y, rectScreen.top + g_player.obj.size.y / 2, rectScreen.bottom - g_player.obj.size.y / 2);

	// ***** ショット *****
	g_player.nCounterShoot++;
	if ((GetKeyboardPress(DIK_SPACE) || GetJoypadPress(JOYKEY_A)) && g_player.nCounterShoot % INIT_SHOOT_INTERVAL == 0)
	{// 弾撃ち
		g_player.nCounterShoot = 0;
		PlaySound(SOUND_LABEL_SE_SHOOT, 0.1f);
		SetBullet(g_player.obj.pos + D3DXVECTOR3(5, -5, 0), g_player.fShootSpeed, D3DX_PI);
		SetBullet(g_player.obj.pos + D3DXVECTOR3(-5, -5, 0), g_player.fShootSpeed, D3DX_PI);
		SetBullet(g_player.obj.pos + D3DXVECTOR3(15, 0, 0), g_player.fShootSpeed, D3DX_PI);
		SetBullet(g_player.obj.pos + D3DXVECTOR3(-15, 0, 0), g_player.fShootSpeed, D3DX_PI);
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	SetVertexPos(pVtx, g_player.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_player.obj.color);
	SetVertexTexturePos(pVtx, g_player.nTexPattern, NUM_TEXTURE, g_player.obj.bInversed);

	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_player.obj.bVisible == true)
	{// ポリゴン描画
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffPlayer);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=====================================================================
// オブジェクト取得処理
//=====================================================================
PLAYER* GetPlayer(void)
{
	return &g_player;
}

void HitPlayer(void)
{
	// 以下の状態では判定しない
	if (g_player.state == PLAYERSTATE_APPEAR)	return;		// 出現直後
	if (g_player.state == PLAYERSTATE_DAMAGED)	return;		// ダメージ直後
	if (g_player.state == PLAYERSTATE_DIED)		return;		// 死亡状態
	if (g_player.state == PLAYERSTATE_END)		return;		// ゲーム終了状態

	g_player.fLife -= 1;

	if (g_player.fLife <= 0)
	{
		PlaySound(SOUND_LABEL_SE_HIT02);
		SetVibration(60000, 60000, 50);
		SetSpriteEffect(SPRITEEFFECTYPE_EXPLOSION, g_player.obj.pos, 1.5f);
		SetPlayerState(PLAYERSTATE_DIED);
	}
	else
	{
		PlaySound(SOUND_LABEL_SE_HIT01);
		SetVibration(40000, 40000, 50);
		SetPlayerState(PLAYERSTATE_DAMAGED);
	}
}

void SetPlayerState(PLAYERSTATE state)
{
	g_player.state = state;
	g_player.nCounterState = 0;
}
