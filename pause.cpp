//=====================================================================
// 
// �|�[�Y��ʏ��� [pause.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "pause.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include "game.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define TEXTURE_POS_X			(SCREEN_WIDTH / 2)
#define TEXTURE_POS_Y			(SCREEN_HEIGHT / 2 - 100)
#define TEXTURE_SIZE			(1)
#define TEXTURE_SCALE_X			(500)
#define TEXTURE_SCALE_Y			(100)

#define COLOR_SELECTED			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_DESELECTED		D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f)

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DTEXTURE9 g_apTexturePause[PAUSE_MENU_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
PAUSE g_menu;

const char* g_aPauseTexFileName[PAUSE_MENU_MAX] = {
	"data\\TEXTURE\\pause000.png",
	"data\\TEXTURE\\pause001.png",
	"data\\TEXTURE\\pause002.png"
};

//=====================================================================
// ����������
//=====================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	g_menu.obj.pos = D3DXVECTOR3(TEXTURE_POS_X, TEXTURE_POS_Y, 0.0f);
	g_menu.nSelect = 0;

	for (int nCount = 0; nCount < PAUSE_MENU_MAX; nCount++)
	{// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			g_aPauseTexFileName[nCount],
			&g_apTexturePause[nCount]
		);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * PAUSE_MENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL
	);
}

//=====================================================================
// �I������
//=====================================================================
void UninitPause(void)
{
	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		// �e�N�X�`���̔j��
		if (g_apTexturePause[nCntPause] != NULL)
		{
			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdatePause(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) ||
		GetJoypadTrigger(JOYKEY_A)
		)
	{
		switch (g_menu.nSelect)
		{
		case PAUSE_MENU_CONTINUE:
			TogglePause(false);
			break;

		case PAUSE_MENU_RETRY:
			SetFade(SCENE_GAME);
			break;

		case PAUSE_MENU_QUIT:
			SetFade(SCENE_TITLE);
			break;
		}
	}

	if (GetKeyboardRepeat(DIK_W) || GetJoypadRepeat(JOYKEY_UP))
	{
		g_menu.nSelect--;
	}
	if (GetKeyboardRepeat(DIK_S) || GetJoypadRepeat(JOYKEY_DOWN))
	{
		g_menu.nSelect++;
	}
	g_menu.nSelect %= 3;
	if (g_menu.nSelect < 0)
	{
		g_menu.nSelect = PAUSE_MENU_MAX - 1;
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < PAUSE_MENU_MAX; nCount++, pVtx += 4)
	{
		// ���_����ݒ�
		SetVertexPos(pVtx,
			D3DXVECTOR3(g_menu.obj.pos.x - TEXTURE_SCALE_X / 2, g_menu.obj.pos.y + (100 * nCount) - TEXTURE_SCALE_Y / 2, 0.0f),
			D3DXVECTOR3(g_menu.obj.pos.x + TEXTURE_SCALE_X / 2, g_menu.obj.pos.y + (100 * nCount) - TEXTURE_SCALE_Y / 2, 0.0f),
			D3DXVECTOR3(g_menu.obj.pos.x - TEXTURE_SCALE_X / 2, g_menu.obj.pos.y + (100 * nCount) + TEXTURE_SCALE_Y / 2, 0.0f),
			D3DXVECTOR3(g_menu.obj.pos.x + TEXTURE_SCALE_X / 2, g_menu.obj.pos.y + (100 * nCount) + TEXTURE_SCALE_Y / 2, 0.0f)
		);
		SetVertexRHW(pVtx, 1.0f);
		if (nCount == g_menu.nSelect)
		{
			SetVertexColor(pVtx, COLOR_SELECTED);
		}
		else
		{
			SetVertexColor(pVtx, COLOR_DESELECTED);
		}
		SetVertexTexturePos(pVtx);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}

//=====================================================================
// �|�[�Y���j���[�J�[�\���w�菈��
//=====================================================================
void SetPauseMenuCursor(int nCursor)
{
	g_menu.nSelect = nCursor;
}