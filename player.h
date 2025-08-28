//=====================================================================
//
// player.cpp�̃w�b�_�t�@�C�� [player.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "baseObject.h"
#include "main.h"

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************
typedef enum
{
	PLAYERSTATE_NORMAL = 0,
	PLAYERSTATE_APPEAR,
	PLAYERSTATE_DAMAGED,
	PLAYERSTATE_DIED,
	PLAYERSTATE_END,
	PLAYERSTATE_MAX
}PLAYERSTATE;

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;
	int nTexPattern;
	int nCounterPattern;
	
	PLAYERSTATE state;
	float fSpeed;
	float fShootSpeed;
	float fLife;
	float fCharge;

	int nCounterState;
	int nCounterShoot;

}PLAYER;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER* GetPlayer(void);
void HitPlayer(void);

#endif