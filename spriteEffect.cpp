//=====================================================================
// 
// �X�v���C�g�G�t�F�N�g [spriteEffect.cpp]
// Author : Kaito Iwaskai
//
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "spriteEffect.h"
#include "util.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define INIT_SIZE		D3DXVECTOR3(64.0f, 64.0f, 0.0f)

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
typedef struct
{
	const char* aSpriteEffectFilename;
	int nMaxPattern;
	int nAnimSpeed;
}SPRITEEFFECT_INFO;


//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSpriteEffect = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffSpriteEffect[MAX_SPRITEEFFECT] = {};
SPRITEEFFECT g_aSpriteEffect[MAX_SPRITEEFFECT];

SPRITEEFFECT_INFO g_aSpriteEffectInfo[SPRITEEFFECTTYPE_MAX] = {
	{"data\\TEXTURE\\explosion000.png", 5, 3},
};


//=====================================================================
// 
// ***** ���������� *****
// 
//=====================================================================
void InitSpriteEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	memset(&g_aSpriteEffect[0], 0, sizeof(SPRITEEFFECT) * MAX_SPRITEEFFECT);

	for (int nCount = 0; nCount < SPRITEEFFECTTYPE_MAX; nCount++)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			g_aSpriteEffectInfo[nCount].aSpriteEffectFilename,
			&g_pTexBuffSpriteEffect[nCount]
		);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_SPRITEEFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSpriteEffect,
		NULL
	);
}

//=====================================================================
// 
// ***** �I������ *****
// 
//=====================================================================
void UninitSpriteEffect(void)
{
	memset(&g_aSpriteEffect[0], 0, sizeof(SPRITEEFFECT) * MAX_SPRITEEFFECT);

	if (g_pVtxBuffSpriteEffect != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffSpriteEffect->Release();
		g_pVtxBuffSpriteEffect = NULL;
	}

	for (int nCount = 0; nCount < SPRITEEFFECTTYPE_MAX; nCount++)
	{
		if (g_pTexBuffSpriteEffect[nCount] != NULL)
		{// �e�N�X�`���o�b�t�@�̔j��
			g_pTexBuffSpriteEffect[nCount]->Release();
			g_pTexBuffSpriteEffect[nCount] = NULL;
		}
	}
}

//=====================================================================
// 
// ***** �X�V���� *****
// 
//=====================================================================
void UpdateSpriteEffect(void)
{
	SPRITEEFFECT* pSpriteEffect = &g_aSpriteEffect[0];

	for (int nCount = 0; nCount < MAX_SPRITEEFFECT; nCount++, pSpriteEffect++)
	{
		if (pSpriteEffect->bUsed == false) continue;

		pSpriteEffect->nCounterAnim++;

		if (pSpriteEffect->nCounterAnim % pSpriteEffect->nAnimSpeed == 0)
		{
			pSpriteEffect->nPatternAnim++;
		}

		if (pSpriteEffect->nPatternAnim > pSpriteEffect->nMaxPattern - 1)
		{
			pSpriteEffect->bUsed = false;
			continue;
		}
	}
}

//=====================================================================
// 
// ***** �`�揈�� *****
// 
//=====================================================================
void DrawSpriteEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;
	SPRITEEFFECT* pSpriteEffect = &g_aSpriteEffect[0];

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffSpriteEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_SPRITEEFFECT; nCount++, pSpriteEffect++, pVtx += 4)
	{
		// ���_����ݒ�
		SetVertexPos(pVtx, pSpriteEffect->obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pSpriteEffect->obj.color);
		SetVertexTexturePos(pVtx, pSpriteEffect->nPatternAnim, pSpriteEffect->nMaxPattern);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffSpriteEffect->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffSpriteEffect, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pSpriteEffect = &g_aSpriteEffect[0];
	for (int nCount = 0; nCount < MAX_SPRITEEFFECT; nCount++, pSpriteEffect++)
	{
		if (pSpriteEffect->obj.bVisible == true && pSpriteEffect->bUsed == true)
		{// �|���S���`��
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexBuffSpriteEffect[pSpriteEffect->type]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//=====================================================================
// 
// ***** �X�v���C�g�G�t�F�N�g�ݒ菈�� *****
// 
//=====================================================================
void SetSpriteEffect(SPRITEEFFECTTYPE type, D3DXVECTOR3 pos, float fScale)
 {
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SPRITEEFFECT* pSpriteEffect = &g_aSpriteEffect[0];

	for (int nCount = 0; nCount < MAX_SPRITEEFFECT; nCount++, pSpriteEffect++)
	{
		if (pSpriteEffect->bUsed == true) continue;

		memset(&g_aSpriteEffect[0], 0, sizeof(SPRITEEFFECT) * MAX_SPRITEEFFECT);

		pSpriteEffect->bUsed = true;
		pSpriteEffect->type = type;
		pSpriteEffect->obj.pos = pos;
		pSpriteEffect->obj.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pSpriteEffect->obj.size = INIT_SIZE * fScale;
		pSpriteEffect->obj.bVisible = true;
		pSpriteEffect->nMaxPattern = g_aSpriteEffectInfo[type].nMaxPattern;
		pSpriteEffect->nAnimSpeed = g_aSpriteEffectInfo[type].nAnimSpeed;

		break;
	}
}