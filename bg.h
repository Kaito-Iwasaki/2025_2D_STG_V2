//=====================================================================
//
// bg.cppのヘッダファイル [bg.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

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
#define MAX_BACKGROUND		(4)

#define BG_SCROLL_SPEED			(1.0f)
#define BG_SCROLL_OFFSET		(1.0f)
#define BG_SCROLL_MOVE_SCALE	(0.005f)

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;
}BACKGROUND;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitBackground(void);
void UninitBackground(void);
void UpdateBackground(void);
void DrawBackground(void);
BACKGROUND* GetBackground(void);
void SetBackgroundSpeed(float fSpeed);
void SetBackgroundSpeedMove(float fSpeed, float fScale);
float GetBackgroundSpeed(void);

#endif