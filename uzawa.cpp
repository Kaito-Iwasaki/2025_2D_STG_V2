//=====================================================================
//
// 宇沢オブジェクト [uzawa.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "uzawa.h"
#include "input.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\uzawa.png"
#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE_X				(300.0f)
#define INIT_SIZE_Y				(600.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
UZAWA g_uzawa;

//=====================================================================
// 初期化処理
//=====================================================================
void InitUzawa(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス
	VERTEX_2D* pVtx;							// 頂点情報

	// 構造体の初期化
	memset(&g_uzawa, 0, sizeof(UZAWA));
	g_uzawa.obj.pos = { INIT_POS_X, INIT_POS_Y, 0.0f };
	g_uzawa.obj.size = { INIT_SIZE_X, INIT_SIZE_Y, 0.0f };
	g_uzawa.obj.color = INIT_COLOR;
	g_uzawa.obj.bVisible = true;

	g_uzawa.fSpeed = 10.0f;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE_FILENAME,
		&g_uzawa.obj.pTexBuff
	);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_uzawa.obj.pVtxBuff,
		NULL
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitUzawa(void)
{
	if (g_uzawa.obj.pVtxBuff != NULL)
	{// 頂点バッファの破棄
		g_uzawa.obj.pVtxBuff->Release();
		g_uzawa.obj.pVtxBuff = NULL;
	}

	if (g_uzawa.obj.pTexBuff != NULL)
	{// テクスチャバッファの破棄
		g_uzawa.obj.pTexBuff->Release();
		g_uzawa.obj.pTexBuff = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateUzawa(void)
{
	D3DXVECTOR3 direction = D3DXVECTOR3_ZERO;
	float fMagnitude;

	if (GetKeyboardPress(DIK_A))
	{
		direction.x -= 1;
	}
	if (GetKeyboardPress(DIK_D))
	{
		direction.x += 1;
	}
	if (GetKeyboardPress(DIK_W))
	{
		direction.y -= 1;
	}
	if (GetKeyboardPress(DIK_S))
	{
		direction.y += 1;
	}

	fMagnitude = sqrtf(direction.x * direction.x + direction.y * direction.y);
	if (fMagnitude == 0) fMagnitude = 1;

	g_uzawa.obj.pos += D3DXVECTOR3(direction.x / fMagnitude, direction.y / fMagnitude, 0.0f) * g_uzawa.fSpeed;
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawUzawa(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// デバイスの取得
	pDevice = GetDevice();

	if (g_uzawa.obj.bVisible == true)
	{// 表示状態
		// 頂点バッファをロックして頂点情報へのポインタを取得
		g_uzawa.obj.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点情報を設定
		SetVertexPos(pVtx, g_uzawa.obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, g_uzawa.obj.color);
		SetVertexTexturePos(pVtx);

		// 頂点バッファをアンロック
		g_uzawa.obj.pVtxBuff->Unlock();

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_uzawa.obj.pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_uzawa.obj.pTexBuff);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=====================================================================
// オブジェクト取得処理
//=====================================================================
UZAWA* GetUzawa(void)
{
	return &g_uzawa;
}
