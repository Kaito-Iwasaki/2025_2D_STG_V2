//=====================================================================
//
// �G�e�I�u�W�F�N�g [enemybullet.cpp]
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

#include "enemybullet.h"
#include "player.h"
#include "spriteEffect.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\enemybullet000.png"

#define INIT_POS				D3DXVECTOR3_ZERO
#define INIT_SIZE				D3DXVECTOR3(16.0f, 16.0f, 16.0f)
#define INIT_COLOR				D3DXCOLOR(0.0f, 0.0f, 0.0f,1.0f)

#define ENEMYBULLET_COLOR_NORMAL		INIT_COLOR
#define ENEMYBULLET_COLOR_DAMAGED		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)


//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemyBullet = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffEnemyBullet[ENEMYBULLET_TYPE_MAX] = {};
ENEMYBULLET g_aEnemyBullet[MAX_ENEMYBULLET] = {};

const char* g_aEnemyBulletFileName[ENEMYBULLET_TYPE_MAX] = {
	"data\\TEXTURE\\enemybullet000.png",
	"data\\TEXTURE\\enemybullet001.png",
};

//=====================================================================
// ����������
//=====================================================================
void InitEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X
	VERTEX_2D* pVtx;							// ���_���
	ENEMYBULLET* pEnemyBullet = &g_aEnemyBullet[0];

	// �\���̂̏�����
	memset(pEnemyBullet, 0, sizeof(ENEMYBULLET) * MAX_ENEMYBULLET);
	for (int nCount = 0; nCount < MAX_ENEMYBULLET; nCount++, pEnemyBullet++)
	{
		pEnemyBullet->obj.pos = D3DXVECTOR3_ZERO;
		pEnemyBullet->obj.size = D3DXVECTOR3_ZERO;
		pEnemyBullet->obj.color = INIT_COLOR;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMYBULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemyBullet,
		NULL
	);
}

//=====================================================================
// �I������
//=====================================================================
void UninitEnemyBullet(void)
{
	memset(&g_aEnemyBullet[0], 0, sizeof(ENEMYBULLET) * MAX_ENEMYBULLET);

	if (g_pVtxBuffEnemyBullet != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffEnemyBullet->Release();
		g_pVtxBuffEnemyBullet = NULL;
	}

	for (int nCount = 0; nCount < ENEMYBULLET_TYPE_MAX; nCount++)
	{
		if (g_pTexBuffEnemyBullet[nCount] != NULL)
		{// �e�N�X�`���o�b�t�@�̔j��
			g_pTexBuffEnemyBullet[nCount]->Release();
			g_pTexBuffEnemyBullet[nCount] = NULL;
		}
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateEnemyBullet(void)
{
	ENEMYBULLET* pEnemyBullet = &g_aEnemyBullet[0];
	PLAYER* pPlayer = GetPlayer();

	for (int nCount = 0; nCount < MAX_ENEMYBULLET; nCount++, pEnemyBullet++)
	{
		if (pEnemyBullet->bUsed == false) continue;		// ���g�p�̓G�Ȃ�X�L�b�v

		if (pEnemyBullet->fLife <= 0)
		{// �e���S
			SetSpriteEffect(SPRITEEFFECTYPE_EXPLOSION, pEnemyBullet->obj.pos, 0.8f);
			pEnemyBullet->bUsed = false;
			continue;
		}

		if (IsObjectOutOfScreen(pEnemyBullet->obj, OOS_BOTTOM | OOS_TOP | OOS_RIGHT | OOS_LEFT))
		{// ��ʊO�ɏo����폜
			pEnemyBullet->bUsed = false;
			continue;
		}

		if (BoxCollision(pEnemyBullet->obj.pos, pEnemyBullet->obj.size, pPlayer->obj.pos, pPlayer->hitBoxSize))
		{// �v���C���[�Ƃ̏Փ˔���
			HitPlayer();
			pEnemyBullet->bUsed = false;
			continue;
		}

		switch (pEnemyBullet->state)
		{
		case ENEMYBULLETSTATE_NORMAL:
			pEnemyBullet->obj.color = ENEMYBULLET_COLOR_NORMAL;
			break;

		case ENEMYBULLETSTATE_DAMAGED:
			pEnemyBullet->obj.color = ENEMYBULLET_COLOR_DAMAGED;

			if (pEnemyBullet->nCounterState % 1 == 0)
			{
 				pEnemyBullet->state = ENEMYBULLETSTATE_NORMAL;
			}
			break;
		}
		pEnemyBullet->nCounterState++;

		switch (pEnemyBullet->type)
		{
		case ENEMYBULLET_TYPE_001:
			if (pEnemyBullet->nCounterState % 2 == 0)
			{
				pEnemyBullet->obj.rot.z += D3DX_PI * 0.125f;
			}
			break;

		default:
			break;
		}

		pEnemyBullet->obj.pos += D3DXVECTOR3(sin(pEnemyBullet->fDirection), cos(pEnemyBullet->fDirection), 0.0f) * pEnemyBullet->fSpeed;
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	ENEMYBULLET* pEnemyBullet = &g_aEnemyBullet[0];

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffEnemyBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_ENEMYBULLET; nCount++, pEnemyBullet++, pVtx += 4)
	{
		// ���_����ݒ�
		SetVertexPos(pVtx, pEnemyBullet->obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pEnemyBullet->obj.color);
		SetVertexTexturePos(pVtx, pEnemyBullet->obj.bInversed);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemyBullet->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemyBullet, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);

	pEnemyBullet = &g_aEnemyBullet[0];
	for (int nCount = 0; nCount < MAX_ENEMYBULLET; nCount++, pEnemyBullet++)
	{
		if (pEnemyBullet->bUsed == true && pEnemyBullet->obj.bVisible == true)
		{// �|���S���`��
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexBuffEnemyBullet[pEnemyBullet->type]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}

	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
}

//=====================================================================
// �I�u�W�F�N�g�擾����
//=====================================================================
ENEMYBULLET* GetEnemyBullet(void)
{
	return &g_aEnemyBullet[0];
}

//=====================================================================
// �G�e�̐ݒ菈��
//=====================================================================
void SetEnemyBullet(ENEMYBULLET_TYPE type, D3DXVECTOR3 pos, float fSpeed, float fDirection)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYBULLET* pEnemyBullet = &g_aEnemyBullet[0];

	for (int nCount = 0; nCount < MAX_ENEMYBULLET; nCount++, pEnemyBullet++)
	{
		if (pEnemyBullet->bUsed == false)
		{
			if (g_pTexBuffEnemyBullet[type] == NULL)
			{
				D3DXCreateTextureFromFile(
					pDevice,
					g_aEnemyBulletFileName[type],
					&g_pTexBuffEnemyBullet[type]
				);
			}

			memset(pEnemyBullet, 0, sizeof(ENEMYBULLET));
			pEnemyBullet->bUsed = true;
			pEnemyBullet->obj.pos = pos;
			pEnemyBullet->obj.size = INIT_SIZE;
			pEnemyBullet->obj.color = INIT_COLOR;
			pEnemyBullet->fSpeed = fSpeed;
			pEnemyBullet->fDirection = fDirection;
			pEnemyBullet->fDamage = 1.0f;
			pEnemyBullet->fLife = 10.0f;
			pEnemyBullet->type = type;
			pEnemyBullet->state = ENEMYBULLETSTATE_NORMAL;
			pEnemyBullet->obj.bVisible = true;

			break;
		}
	}
}

void HitEnemyBullet(ENEMYBULLET* pEnemyBullet)
{
	pEnemyBullet->fLife -= 1;

	if (pEnemyBullet->fLife <= 0)
	{
		SetSpriteEffect(SPRITEEFFECTYPE_EXPLOSION, pEnemyBullet->obj.pos, 1.0f);
		pEnemyBullet->bUsed = false;
	}
	else
	{
		pEnemyBullet->state = ENEMYBULLETSTATE_DAMAGED;
		pEnemyBullet->nCounterState = 0;
	}
}