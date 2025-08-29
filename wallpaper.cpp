//=====================================================================
//
// �F��I�u�W�F�N�g [wallpaper.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "input.h"
#include "sound.h"
#include "collision.h"

#include "wallpaper.h"
#include "player.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\title001.png"

#define INIT_POS				{SCREEN_WIDTH / 2, SCREEN_HEIGHT/ 2, 0.0f}
#define INIT_SIZE				{SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f}
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

#define INIT_PLAYER_SPEED		(10.0f)

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWallPaper = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffWallPaper = NULL;
WALLPAPER g_wallpaper;

//=====================================================================
// ����������
//=====================================================================
void InitWallPaper(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X

	// �\���̂̏�����
	memset(&g_wallpaper, 0, sizeof(WALLPAPER));
	g_wallpaper.obj.pos = INIT_POS;
	g_wallpaper.obj.size = INIT_SIZE;
	g_wallpaper.obj.color = INIT_COLOR;
	g_wallpaper.obj.bVisible = true;

	// �e�N�X�`���̓ǂݍ���
	if (TEXTURE_FILENAME)
	{// �e�N�X�`���쐬
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffWallPaper
		);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWallPaper,
		NULL
	);
}

//=====================================================================
// �I������
//=====================================================================
void UninitWallPaper(void)
{
	memset(&g_wallpaper, 0, sizeof(WALLPAPER));

	if (g_pVtxBuffWallPaper != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffWallPaper->Release();
		g_pVtxBuffWallPaper = NULL;
	}

	if (g_pTexBuffWallPaper != NULL)
	{// �e�N�X�`���o�b�t�@�̔j��
		g_pTexBuffWallPaper->Release();
		g_pTexBuffWallPaper = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateWallPaper(void)
{

}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawWallPaper(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffWallPaper->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	SetVertexPos(pVtx, g_wallpaper.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_wallpaper.obj.color);
	SetVertexTexturePos(pVtx);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffWallPaper->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffWallPaper, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_wallpaper.obj.bVisible == true)
	{// �|���S���`��
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexBuffWallPaper);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=====================================================================
// �I�u�W�F�N�g�擾����
//=====================================================================
WALLPAPER* GetWallPaper(void)
{
	return &g_wallpaper;
}
