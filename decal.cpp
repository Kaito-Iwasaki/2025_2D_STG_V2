//=====================================================================
//
// �摜�I�u�W�F�N�g [decal.cpp]
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

#include "decal.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE_X				(8.0f)
#define INIT_SIZE_Y				(32.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDecal = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffDecal[DECAL_LABEL_MAX] = {};
DECAL g_aDecal[MAX_DECAL] = {};

const char* g_aDecalFileName[DECAL_LABEL_MAX] = {
	"data\\TEXTURE\\title000.png",
	"data\\TEXTURE\\title000.png",
	"data\\TEXTURE\\bg100.png",
	"data\\TEXTURE\\bg101.png",
	"data\\TEXTURE\\bg102.png"
};

//=====================================================================
// ����������
//=====================================================================
void InitDecal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X
	VERTEX_2D* pVtx;							// ���_���
	DECAL* pDecal = &g_aDecal[0];

	// �\���̂̏�����
	memset(pDecal, 0, sizeof(DECAL) * MAX_DECAL);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_DECAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDecal,
		NULL
	);
}

//=====================================================================
// �I������
//=====================================================================
void UninitDecal(void)
{
	if (g_pVtxBuffDecal != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffDecal->Release();
		g_pVtxBuffDecal = NULL;
	}

	for (int nCount = 0; nCount < DECAL_LABEL_MAX; nCount++)
	{
		if (g_pTexBuffDecal[nCount] != NULL)
		{// �e�N�X�`���o�b�t�@�̔j��
			g_pTexBuffDecal[nCount]->Release();
			g_pTexBuffDecal[nCount] = NULL;
		}
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateDecal(void)
{

}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawDecal(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	DECAL* pDecal = &g_aDecal[0];

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffDecal->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_DECAL; nCount++, pDecal++, pVtx += 4)
	{
		// ���_����ݒ�
		SetVertexPos(pVtx, pDecal->obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pDecal->obj.color);
		SetVertexTexturePos(pVtx);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffDecal->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffDecal, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDecal = &g_aDecal[0];
	for (int nCount = 0; nCount < MAX_DECAL; nCount++, pDecal++)
	{
		if (pDecal->obj.bVisible == true && pDecal->bUsed == true)
		{// �|���S���`��
			// �e�N�X�`���̐ݒ�
			if (pDecal->label != DECAL_LABEL_NULL)
			{
				pDevice->SetTexture(0, g_pTexBuffDecal[pDecal->label]);
			}
			else
			{
				pDevice->SetTexture(0, NULL);
			}

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//=====================================================================
// �I�u�W�F�N�g�擾����
//=====================================================================
DECAL* GetDecal(void)
{
	return &g_aDecal[0];
}

//=====================================================================
// �摜�̐ݒ菈��
//=====================================================================
int SetDecal(DECAL_LABEL label, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	DECAL* pDecal = &g_aDecal[0];
	for (int nCount = 0; nCount < MAX_DECAL; nCount++, pDecal++)
	{
		if (pDecal->bUsed == false)
		{
			if (label != DECAL_LABEL_NULL && g_pTexBuffDecal[label] == NULL)
			{
				D3DXCreateTextureFromFile(
					pDevice,
					g_aDecalFileName[label],
					&g_pTexBuffDecal[label]
				);
			}

			memset(pDecal, 0, sizeof(DECAL));
			pDecal->bUsed = true;
			pDecal->obj.pos = pos;
			pDecal->obj.size = size;
			pDecal->obj.color = col;
			pDecal->label = label;
			pDecal->nID = nCount;
			pDecal->obj.bVisible = true;

			return nCount;
		}
	}

	return -1;
}

//=====================================================================
// �摜�̍폜�����i�w��j
//=====================================================================
void DeleteDecal(int nID)
{
	memset(&g_aDecal[nID], 0, sizeof(DECAL));
}

//=====================================================================
// �摜�̍폜�����i�ꊇ�j
//=====================================================================
void DeleteDecal(void)
{
	memset(&g_aDecal[0], 0, sizeof(DECAL) * MAX_DECAL);
}
