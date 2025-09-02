//=====================================================================
//
// �w�i���� [bg.cpp]
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
#include "util.h"

#include "bg.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define TEXTURE_NUM				(2)
#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE_X				(540 * TEXTURE_NUM)
#define INIT_SIZE_Y				(720 * TEXTURE_NUM)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

#define BG_SCROLL_SPEED			(3.0f)
#define BG_SCROLL_OFFSET		(1.0f)

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBackground = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffBackground[MAX_BACKGROUND] = {};
BACKGROUND g_aBackground[MAX_BACKGROUND] = {};

const char* g_aBackgroundFileName[MAX_BACKGROUND] = {
	NULL,
	"data\\TEXTURE\\bg000.png",
	"data\\TEXTURE\\bg001.png",
	"data\\TEXTURE\\bg002.png",
};

//=====================================================================
// ����������
//=====================================================================
void InitBackground(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X
	BACKGROUND* pBackground = &g_aBackground[0];

	// �\���̂̏�����
	memset(pBackground, 0, sizeof(BACKGROUND) * MAX_BACKGROUND);

	for (int nCount = 0; nCount < MAX_BACKGROUND; nCount++, pBackground++)
	{
		pBackground->obj.pos = { INIT_POS_X, INIT_POS_Y, 0.0f };
		pBackground->obj.size = { INIT_SIZE_X, INIT_SIZE_Y, 0.0f };
		pBackground->obj.color = INIT_COLOR;
		pBackground->obj.bVisible = true;

		if (nCount == 0)
		{
			pBackground->obj.color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		}

		D3DXCreateTextureFromFile(
			pDevice,
			g_aBackgroundFileName[nCount],
			&g_pTexBuffBackground[nCount]
		);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BACKGROUND,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBackground,
		NULL
	);
}

//=====================================================================
// �I������
//=====================================================================
void UninitBackground(void)
{
	if (g_pVtxBuffBackground != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffBackground->Release();
		g_pVtxBuffBackground = NULL;
	}

	for (int nCount = 0; nCount < MAX_BACKGROUND; nCount++)
	{
		if (g_pTexBuffBackground[nCount] != NULL)
		{// �e�N�X�`���o�b�t�@�̔j��
			g_pTexBuffBackground[nCount]->Release();
			g_pTexBuffBackground[nCount] = NULL;
		}
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateBackground(void)
{
	BACKGROUND* pBackground = &g_aBackground[0];

	for (int nCount = 0; nCount < MAX_BACKGROUND; nCount++, pBackground++)
	{
		pBackground->obj.pos.y += (nCount * BG_SCROLL_OFFSET);

		if (pBackground->obj.pos.y >= SCREEN_HEIGHT)
		{
			pBackground->obj.pos.y = 0;
		}
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawBackground(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	BACKGROUND* pBackground = &g_aBackground[0];

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffBackground->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_BACKGROUND; nCount++, pBackground++, pVtx += 4)
	{
		// ���_����ݒ�
		SetVertexPos(pVtx, pBackground->obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pBackground->obj.color);
		SetVertexTexturePos(pVtx,
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(1.0f * TEXTURE_NUM, 0.0f),
			D3DXVECTOR2(0.0f, 1.0f * TEXTURE_NUM),
			D3DXVECTOR2(1.0f * TEXTURE_NUM, 1.0f * TEXTURE_NUM)
		);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBackground->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBackground, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pBackground = &g_aBackground[0];
	for (int nCount = 0; nCount < MAX_BACKGROUND; nCount++, pBackground++)
	{
		if (pBackground->obj.bVisible == true)
		{// �|���S���`��
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexBuffBackground[nCount]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//=====================================================================
// �I�u�W�F�N�g�擾����
//=====================================================================
BACKGROUND* GetBackground(void)
{
	return &g_aBackground[0];
}