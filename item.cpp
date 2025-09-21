//=====================================================================
//
// 画像オブジェクト [item.cpp]
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
#include "item.h"
#include "score.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE				D3DXVECTOR3(48.0f, 48.0f, 0.0f)
#define INIT_COLOR				D3DXCOLOR(0.0f, 0.0f, 0.0f,1.0f)

#define INIT_ITEM_SHOOT_INTERVAL	(20)

#define ITEM_COLOR_NORMAL		INIT_COLOR
#define ITEM_COLOR_DAMAGED		D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffItem[ITEMTYPE_MAX] = {};
ITEM g_aItem[MAX_ITEM] = {};

// 敵のテクスチャ
const char* g_aItemFileName[ITEMTYPE_MAX] = {
	// 通常
	"data\\TEXTURE\\item000.png",
};

//=====================================================================
// 初期化処理
//=====================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス
	VERTEX_2D* pVtx;							// 頂点情報
	ITEM* pItem = &g_aItem[0];

	// 構造体の初期化
	memset(pItem, 0, sizeof(ITEM) * MAX_ITEM);

	// テクスチャの読み込み
	for (int nCount = 0; nCount < ITEMTYPE_MAX; nCount++)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			g_aItemFileName[nCount],
			&g_pTexBuffItem[nCount]
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitItem(void)
{
	if (g_pVtxBuffItem != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}

	for (int nCount = 0; nCount < ITEMTYPE_MAX; nCount++)
	{
		if (g_pTexBuffItem[nCount] != NULL)
		{// テクスチャバッファの破棄
			g_pTexBuffItem[nCount]->Release();
			g_pTexBuffItem[nCount] = NULL;
		}
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateItem(void)
{
	ITEM* pItem = &g_aItem[0];
	PLAYER* pPlayer = GetPlayer();
	RECT rectScreen = GAME_SCREEN_RECT;

	for (int nCount = 0; nCount < MAX_ITEM; nCount++, pItem++)
	{
		if (pItem->bUsed == false) continue;

		bool bCollision = BoxCollision(pItem->obj.pos, pItem->hitboxSize, pPlayer->obj.pos, pPlayer->hitBoxSize);

		if (IsObjectOutOfScreen(pItem->obj, rectScreen, pItem->disapperFlags))
		{// 画面外に出たら削除
			pItem->bUsed = false;
			continue;
		}

		switch (pItem->type)
		{
		case ITEMTYPE_000:
			switch (pItem->nMode)
			{
			case 0:
				pItem->obj.pos.y -= 5.0f;
				if (pItem->nCounterState > 20)
				{
					pItem->nMode++;
					pItem->nCounterState = 0;
				}
				break;

			case 1:
				pItem->obj.pos.x = pItem->startPos.x + sin(pItem->nCounterState * 0.05f) * 100.0f;
				pItem->obj.pos.y += 1.0f;

				if (bCollision)
				{
					if (pPlayer->fLife == PLAYER_HEAL_MAX)
					{
						BonusScore(1000, "FULL SHIELD BONUS");
					}
					else
					{
						PlaySound(SOUND_LABEL_SE_HEAL);
						pPlayer->fLife += PLAYER_HEAL_MAX / 2.0f;
						Clampf(pPlayer->fLife, pPlayer->fLife, PLAYER_HEAL_MAX);
					}
					pItem->bUsed = false;
				}
				break;
			}
			break;
		}

		pItem->nCounterState++;
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	ITEM* pItem = &g_aItem[0];

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_ITEM; nCount++, pItem++, pVtx += 4)
	{
		// 頂点情報を設定
		SetVertexPos(pVtx, pItem->obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pItem->obj.color);
		SetVertexTexturePos(pVtx);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffItem->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャステージステートの適用
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);

	pItem = &g_aItem[0];
	for (int nCount = 0; nCount < MAX_ITEM; nCount++, pItem++)
	{
		if (pItem->obj.bVisible == true && pItem->bUsed == true)
		{// ポリゴン描画
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffItem[pItem->type]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}

	// テクスチャステージステートの解除
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

}

//=====================================================================
// オブジェクト取得処理
//=====================================================================
ITEM* GetItem(void)
{
	return &g_aItem[0];
}

//=====================================================================
// 敵の設定処理
//=====================================================================
ITEM* SetItem(ITEMTYPE type, D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ITEM* pItem = &g_aItem[0];
	for (int nCount = 0; nCount < MAX_ITEM; nCount++, pItem++)
	{
		if (pItem->bUsed == false)
		{
			memset(pItem, 0, sizeof(ITEM));
			pItem->bUsed = true;
			pItem->obj.pos = pos;
			pItem->startPos = pos;
			pItem->obj.size = INIT_SIZE;
			pItem->obj.rot = D3DXVECTOR3_ZERO;
			pItem->obj.color = INIT_COLOR;
			pItem->obj.bVisible = true;

			pItem->type = type;
			pItem->disapperFlags = OOS_BOTTOM;
			pItem->hitboxSize = pItem->obj.size;

			return pItem;
		}
	}

	return NULL;
}