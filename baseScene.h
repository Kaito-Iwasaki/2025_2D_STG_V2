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
#define GAME_SCREEN_START		(SCREEN_CENTER - GAME_SCREEN_WIDTH / 2)
#define GAME_SCREEN_END			(SCREEN_CENTER + GAME_SCREEN_WIDTH / 2)
#define GAME_SCREEN_RECT		{ SCREEN_CENTER - GAME_SCREEN_WIDTH / 2, 0, SCREEN_CENTER + GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT};

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************
typedef enum
{
	SCENE_TITLE = 0,
	SCENE_TUTORIAL,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_RANKING,
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