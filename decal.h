//=====================================================================
//
// decal.cpp�̃w�b�_�t�@�C�� [decal.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _DECAL_H_
#define _DECAL_H_

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "baseObject.h"
#include "main.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define MAX_DECAL		(256)

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
typedef enum
{
	DECAL_LABEL_TITLE = 0,
	DECAL_LABEL_TUTORIAL,
	DECAL_LABEL_BG000,
	DECAL_LABEL_BG001,
	DECAL_LABEL_BG002,
	DECAL_LABEL_MAX
}DECAL_LABEL;

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;
	bool bUsed;
	DECAL_LABEL label;
	int nID;
}DECAL;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void InitDecal(void);
void UninitDecal(void);
void UpdateDecal(void);
void DrawDecal(void);
DECAL* GetDecal(void);
int SetDecal(DECAL_LABEL label, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);
void DeleteDecal(int nID);
void DeleteDecal(void);

#endif