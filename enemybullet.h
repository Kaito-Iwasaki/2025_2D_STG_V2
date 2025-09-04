//=====================================================================
//
// enemybullet.cpp�̃w�b�_�t�@�C�� [enemybullet.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _ENEMYBULLET_H_
#define _ENEMYBULLET_H_

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
#define MAX_ENEMYBULLET		(1024)

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************
typedef enum
{
	ENEMYBULLET_TYPE_000 = 0,
	ENEMYBULLET_TYPE_001,
	ENEMYBULLET_TYPE_002,
	ENEMYBULLET_TYPE_MAX
}ENEMYBULLET_TYPE;

typedef enum
{
	ENEMYBULLETSTATE_NORMAL = 0,
	ENEMYBULLETSTATE_DAMAGED,
	ENEMYBULLETSTATE_MAX
}ENEMYBULLETSTATE;

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;
	bool bUsed;
	float fSpeed;
	float fDirection;
	float fDamage;
	float fLife;
	ENEMYBULLET_TYPE type;
	ENEMYBULLETSTATE state;

	int nCounterState;
}ENEMYBULLET;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);
ENEMYBULLET* GetEnemyBullet(void);
bool SetEnemyBullet(ENEMYBULLET_TYPE type, D3DXVECTOR3 pos, float fSpeed, float fDirection, float fLife = 0);
void HitEnemyBullet(ENEMYBULLET* pEnemyBullet);

#endif