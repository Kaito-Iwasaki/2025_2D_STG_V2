//=====================================================================
//
// enemy.cpp�̃w�b�_�t�@�C�� [enemy.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

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
#define MAX_ENEMY		(256)

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************
typedef enum
{
	ENEMYTYPE_000 = 0,
	ENEMYTYPE_001,
	ENEMYTYPE_002,
	ENEMYTYPE_MAX
}ENEMYTYPE;

typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGED,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;
	bool bUsed;
	D3DXVECTOR3 startPos;

	D3DXVECTOR3 move;
	ENEMYTYPE type;
	ENEMYSTATE state;
	float fLife;
	int nShootInterval;

	int nCounterState;
	int nCounterShoot;
}ENEMY;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY* GetEnemy(void);
int GetEnemyLeft(void);
ENEMY* SetEnemy(ENEMYTYPE type, D3DXVECTOR3 pos);
void HitEnemy(ENEMY* pEnemy);

#endif