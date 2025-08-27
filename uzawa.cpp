//=====================================================================
//
// �F��I�u�W�F�N�g [uzawa.cpp]
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

#include "uzawa.h"
#include "player.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\uzawa.png"

#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE_X				(300.0f)
#define INIT_SIZE_Y				(600.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUzawa;
LPDIRECT3DTEXTURE9 g_pTexBuffUzawa;
UZAWA g_uzawa;

//=====================================================================
// ����������
//=====================================================================
void InitUzawa(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X
	VERTEX_2D* pVtx;							// ���_���

	// �\���̂̏�����
	memset(&g_uzawa, 0, sizeof(UZAWA));
	g_uzawa.obj.pos = { INIT_POS_X, INIT_POS_Y, 0.0f };
	g_uzawa.obj.size = { INIT_SIZE_X, INIT_SIZE_Y, 0.0f };
	g_uzawa.obj.color = INIT_COLOR;
	g_uzawa.obj.bVisible = true;

	g_uzawa.fSpeed = 10.0f;

	// �e�N�X�`���̓ǂݍ���
	if (TEXTURE_FILENAME)
	{// �e�N�X�`���쐬
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffUzawa
		);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUzawa,
		NULL
	);
}

//=====================================================================
// �I������
//=====================================================================
void UninitUzawa(void)
{
	if (g_pVtxBuffUzawa != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffUzawa->Release();
		g_pVtxBuffUzawa = NULL;
	}

	if (g_pTexBuffUzawa != NULL)
	{// �e�N�X�`���o�b�t�@�̔j��
		g_pTexBuffUzawa->Release();
		g_pTexBuffUzawa = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateUzawa(void)
{
	if (GetPlayer()->obj.bVisible && BoxCollision(g_uzawa.obj, GetPlayer()->obj))
	{
		g_uzawa.obj.color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		g_uzawa.obj.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawUzawa(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffUzawa->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	SetVertexPos(pVtx, g_uzawa.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_uzawa.obj.color);
	SetVertexTexturePos(pVtx);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffUzawa->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUzawa, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_uzawa.obj.bVisible == true)
	{// �|���S���`��
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexBuffUzawa);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=====================================================================
// �I�u�W�F�N�g�擾����
//=====================================================================
UZAWA* GetUzawa(void)
{
	return &g_uzawa;
}
