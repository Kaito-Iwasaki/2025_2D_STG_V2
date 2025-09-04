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
#define MAX_DECAL		(8)

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************
typedef enum
{
	DECAL_LABEL_NULL = -1,
	DECAL_LABEL_TITLE,
	DECAL_LABEL_TUTORIAL,
	DECAL_LABEL_BG000,
	DECAL_LABEL_BG001,
	DECAL_LABEL_BG002,
	DECAL_LABEL_WALLPAPER01,
	DECAL_LABEL_PLAYER,
	DECAL_LABEL_LOGO,
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
DECAL* SetDecal(DECAL_LABEL label, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col);
void DeleteDecal(int nID);
void DeleteDecal(void);

#endif