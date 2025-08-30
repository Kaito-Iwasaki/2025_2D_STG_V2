//=====================================================================
//
// spriteeffect.cppのヘッダファイル [spriteeffect.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _SPRITEEFFECT_H_
#define _SPRITEEFFECT_H_		// 二重インクルード防止のマクロ

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"
#include "baseObject.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_SPRITEEFFECT				(256)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	SPRITEEFFECTYPE_EXPLOSION,
	SPRITEEFFECTTYPE_MAX
}SPRITEEFFECTTYPE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;
	SPRITEEFFECTTYPE type;
	bool bUsed;				// 使用しているかどうか
	int nAnimSpeed;
	int nCounterAnim;			// アニメーションカウント
	int nPatternAnim;		// アニメーションパターンNo
	int nMaxPattern;
}SPRITEEFFECT;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitSpriteEffect(void);
void UninitSpriteEffect(void);
void UpdateSpriteEffect(void);
void DrawSpriteEffect(void);
void SetSpriteEffect(SPRITEEFFECTTYPE type, D3DXVECTOR3 pos, float fScale);

#endif