//=====================================================================
//
// �e�I�u�W�F�N�g [bullet.cpp]
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
#include "collision.h"
#include "baseScene.h"
#include "baseObject.h"

#include "bullet.h"
#include "enemy.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\bullet000.png"

#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE_X				(4.0f)
#define INIT_SIZE_Y				(32.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffBullet = NULL;
BULLET g_aBullet[MAX_BULLET] = {};

//=====================================================================
// ����������
//=====================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X
	VERTEX_2D* pVtx;							// ���_���
	BULLET* pBullet = &g_aBullet[0];

	// �\���̂̏�����
	memset(pBullet, 0, sizeof(BULLET) * MAX_BULLET);
	for (int nCount = 0; nCount < MAX_BULLET; nCount++, pBullet++)
	{
		pBullet->obj.size = { INIT_SIZE_X, INIT_SIZE_Y, 0.0f };
		pBullet->obj.color = INIT_COLOR;
	}

	// �e�N�X�`���̓ǂݍ���
	if (TEXTURE_FILENAME)
	{// �e�N�X�`���쐬
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffBullet
		);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);
}

//=====================================================================
// �I������
//=====================================================================
void UninitBullet(void)
{
	if (g_pVtxBuffBullet != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

	if (g_pTexBuffBullet != NULL)
	{// �e�N�X�`���o�b�t�@�̔j��
		g_pTexBuffBullet->Release();
		g_pTexBuffBullet = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateBullet(void)
{
	BULLET* pBullet = &g_aBullet[0];
	ENEMY* pEnemy;
	RECT rectScreen = GAME_SCREEN_RECT;

	for (int nCountBullet = 0; nCountBullet < MAX_BULLET; nCountBullet++, pBullet++)
	{
		if (pBullet->bUsed == false) continue;		// ���g�p�̓G�Ȃ�X�L�b�v

		// �G���擾
		pEnemy = GetEnemy();

		if (IsObjectOutOfScreen(pBullet->obj, OOS_TOP, rectScreen))
		{// ��ʊO�ɏo����폜
			pBullet->bUsed = false;
			continue;
		}

		pBullet->obj.pos += D3DXVECTOR3(sinf(pBullet->fDirection), cosf(pBullet->fDirection), 0.0f) * pBullet->fSpeed;

		for (int nCountEnemy = 0; nCountEnemy < MAX_ENEMY; nCountEnemy++, pEnemy++)
		{
			if (pEnemy->bUsed && BoxCollision(pBullet->obj, pEnemy->obj))
			{
				HitEnemy(pEnemy);
				pBullet->bUsed = false;
			}
		}
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	BULLET* pBullet = &g_aBullet[0];

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_BULLET; nCount++, pBullet++, pVtx += 4)
	{
		// ���_����ݒ�
		SetVertexPos(pVtx, pBullet->obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pBullet->obj.color);
		SetVertexTexturePos(pVtx);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pBullet = &g_aBullet[0];
	for (int nCount = 0; nCount < MAX_BULLET; nCount++, pBullet++)
	{
		if (pBullet->bUsed == true && pBullet->obj.bVisible == true)
		{// �|���S���`��
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexBuffBullet);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//=====================================================================
// �I�u�W�F�N�g�擾����
//=====================================================================
BULLET* GetBullet(void)
{
	return &g_aBullet[0];
}

//=====================================================================
// �e�̐ݒ菈��
//=====================================================================
void SetBullet(D3DXVECTOR3 pos, float fSpeed, float fDirection)
{
	BULLET* pBullet = &g_aBullet[0];
	for (int nCount = 0; nCount < MAX_BULLET; nCount++, pBullet++)
	{
		if (pBullet->bUsed == false)
		{
			memset(pBullet, 0, sizeof(BULLET));
			pBullet->bUsed = true;
			pBullet->obj.pos = pos;
			pBullet->obj.size = { INIT_SIZE_X, INIT_SIZE_Y, 0.0f };
			pBullet->obj.color = INIT_COLOR;
			pBullet->fSpeed = fSpeed;
			pBullet->fDirection = fDirection;
			pBullet->fDamage = 1.0f;
			pBullet->obj.bVisible = true;

			break;
		}
	}
}
