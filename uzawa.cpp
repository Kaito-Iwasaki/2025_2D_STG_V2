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
#include "input.h"
#include "sound.h"
#include "collision.h"

#include "uzawa.h"
#include "player.h"

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUzawa;
LPDIRECT3DTEXTURE9 g_pTexBuffUzawa;
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
	if (TEXTURE_FILENAME)
	{// テクスチャ作成
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffUzawa
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUzawa,
		NULL
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitUzawa(void)
{
	if (g_pVtxBuffUzawa != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffUzawa->Release();
		g_pVtxBuffUzawa = NULL;
	}

	if (g_pTexBuffUzawa != NULL)
	{// テクスチャバッファの破棄
		g_pTexBuffUzawa->Release();
		g_pTexBuffUzawa = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateUzawa(void)
{
	if (GetPlayer()->obj.bVisible && BoxCollision(g_uzawa.obj, GetPlayer()->obj))
	{
		g_uzawa.obj.color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		g_uzawa.obj.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

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

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffUzawa->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	SetVertexPos(pVtx, g_uzawa.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_uzawa.obj.color);
	SetVertexTexturePos(pVtx);

	// 頂点バッファをアンロック
	g_pVtxBuffUzawa->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUzawa, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_uzawa.obj.bVisible == true)
	{// ポリゴン描画
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffUzawa);

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
