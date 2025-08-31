//=====================================================================
//
// baseScene.cpp�̃w�b�_�t�@�C�� [baseScene.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _BASESCENE_H_
#define _BASESCENE_H_

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "main.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define GAME_SCREEN_WIDTH		(540.0f)
#define GAME_SCREEN_HEIGHT		(720.0f)
#define GAME_SCREEN_RECT		{ SCREEN_CENTER - GAME_SCREEN_WIDTH / 2, 0, SCREEN_CENTER + GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT};

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************
typedef enum
{
	SCENE_GAME = 0,
	SCENE_ANOTHER,
	SCENE_MAX
}SCENE;		// �V�[���񋓌^

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void InitScene(void);
void UninitScene(void);
void UpdateScene(void);
void DrawScene(void);
SCENE SetScene(SCENE nextScene, bool bStopSound = true);

#endif