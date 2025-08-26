//=====================================================================
//
// 宇沢オブジェクト [player.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "player.h"
#include "input.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\player000.png"

#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE_X				(64.0f)
#define INIT_SIZE_Y				(64.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
PLAYER g_player;

//=====================================================================
// 初期化処理
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス
	VERTEX_2D* pVtx;							// 頂点情報

	// 構造体の初期化
	memset(&g_player, 0, sizeof(PLAYER));
	g_player.obj.pos = { INIT_POS_X, INIT_POS_Y, 0.0f };
	g_player.obj.size = { INIT_SIZE_X, INIT_SIZE_Y, 0.0f };
	g_player.obj.color = INIT_COLOR;
	g_player.obj.bVisible = true;

	g_player.fSpeed = 10.0f;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE_FILENAME,
		&g_player.obj.pTexBuff
	);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_player.obj.pVtxBuff,
		NULL
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitPlayer(void)
{
	if (g_player.obj.pVtxBuff != NULL)
	{// 頂点バッファの破棄
		g_player.obj.pVtxBuff->Release();
		g_player.obj.pVtxBuff = NULL;
	}

	if (g_player.obj.pTexBuff != NULL)
	{// テクスチャバッファの破棄
		g_player.obj.pTexBuff->Release();
		g_player.obj.pTexBuff = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdatePlayer(void)
{
	D3DXVECTOR3 direction = D3DXVECTOR3_ZERO;
	float fMagnitude;

	// キーボード操作
	if (GetKeyboardPress(DIK_A))
	{// 左
		direction.x -= 1;
	}
	if (GetKeyboardPress(DIK_D))
	{// 右
		direction.x += 1;
	}
	if (GetKeyboardPress(DIK_W))
	{// 上
		direction.y -= 1;
	}
	if (GetKeyboardPress(DIK_S))
	{// 下
		direction.y += 1;
	}

	// 方向の大きさを求める
	fMagnitude = sqrtf(direction.x * direction.x + direction.y * direction.y);
	if (fMagnitude == 0) fMagnitude = 1;	// 0なら1に補正

	// 位置を更新
	g_player.obj.pos += D3DXVECTOR3(direction.x / fMagnitude, direction.y / fMagnitude, 0.0f) * g_player.fSpeed;
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

	if (g_player.obj.bVisible == true)
	{// 表示状態
		// 頂点バッファをロックして頂点情報へのポインタを取得
		g_player.obj.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点情報を設定
		SetVertexPos(pVtx, g_player.obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, g_player.obj.color);
		SetVertexTexturePos(pVtx);

		// 頂点バッファをアンロック
		g_player.obj.pVtxBuff->Unlock();

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_player.obj.pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_player.obj.pTexBuff);

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
