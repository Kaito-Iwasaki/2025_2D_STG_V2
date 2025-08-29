//=====================================================================
//
// 宇沢オブジェクト [wallpaper.cpp]
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

#include "wallpaper.h"
#include "player.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\title001.png"

#define INIT_POS				{SCREEN_WIDTH / 2, SCREEN_HEIGHT/ 2, 0.0f}
#define INIT_SIZE				{SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f}
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

#define INIT_PLAYER_SPEED		(10.0f)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWallPaper = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffWallPaper = NULL;
WALLPAPER g_wallpaper;

//=====================================================================
// 初期化処理
//=====================================================================
void InitWallPaper(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス

	// 構造体の初期化
	memset(&g_wallpaper, 0, sizeof(WALLPAPER));
	g_wallpaper.obj.pos = INIT_POS;
	g_wallpaper.obj.size = INIT_SIZE;
	g_wallpaper.obj.color = INIT_COLOR;
	g_wallpaper.obj.bVisible = true;

	// テクスチャの読み込み
	if (TEXTURE_FILENAME)
	{// テクスチャ作成
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffWallPaper
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWallPaper,
		NULL
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitWallPaper(void)
{
	memset(&g_wallpaper, 0, sizeof(WALLPAPER));

	if (g_pVtxBuffWallPaper != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffWallPaper->Release();
		g_pVtxBuffWallPaper = NULL;
	}

	if (g_pTexBuffWallPaper != NULL)
	{// テクスチャバッファの破棄
		g_pTexBuffWallPaper->Release();
		g_pTexBuffWallPaper = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateWallPaper(void)
{

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawWallPaper(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffWallPaper->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	SetVertexPos(pVtx, g_wallpaper.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_wallpaper.obj.color);
	SetVertexTexturePos(pVtx);

	// 頂点バッファをアンロック
	g_pVtxBuffWallPaper->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffWallPaper, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_wallpaper.obj.bVisible == true)
	{// ポリゴン描画
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffWallPaper);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=====================================================================
// オブジェクト取得処理
//=====================================================================
WALLPAPER* GetWallPaper(void)
{
	return &g_wallpaper;
}
