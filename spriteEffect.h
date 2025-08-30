//=====================================================================
//
// spriteeffect.cpp�̃w�b�_�t�@�C�� [spriteeffect.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _SPRITEEFFECT_H_
#define _SPRITEEFFECT_H_		// ��d�C���N���[�h�h�~�̃}�N��

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "main.h"
#include "baseObject.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define MAX_SPRITEEFFECT				(256)

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************
typedef enum
{
	SPRITEEFFECTYPE_EXPLOSION,
	SPRITEEFFECTTYPE_MAX
}SPRITEEFFECTTYPE;

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;
	SPRITEEFFECTTYPE type;
	bool bUsed;				// �g�p���Ă��邩�ǂ���
	int nAnimSpeed;
	int nCounterAnim;			// �A�j���[�V�����J�E���g
	int nPatternAnim;		// �A�j���[�V�����p�^�[��No
	int nMaxPattern;
}SPRITEEFFECT;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void InitSpriteEffect(void);
void UninitSpriteEffect(void);
void UpdateSpriteEffect(void);
void DrawSpriteEffect(void);
void SetSpriteEffect(SPRITEEFFECTTYPE type, D3DXVECTOR3 pos, float fScale);

#endif