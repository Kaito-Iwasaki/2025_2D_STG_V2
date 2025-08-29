//=====================================================================
//
// �摜�I�u�W�F�N�g [enemy.cpp]
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

#include "player.h"
#include "enemy.h"
#include "enemybullet.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE_X				(48.0f)
#define INIT_SIZE_Y				(48.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

#define INIT_ENEMY_LIFE				(10)
#define INIT_ENEMY_SHOOT_INTERVAL	(20)

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffEnemy[ENEMYTYPE_MAX] = {};
ENEMY g_aEnemy[MAX_ENEMY] = {};

const char* g_aEnemyFileName[ENEMYTYPE_MAX] = {
	"data\\TEXTURE\\enemy000.png",
	"data\\TEXTURE\\enemy001.png",
};

//=====================================================================
// ����������
//=====================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X
	VERTEX_2D* pVtx;							// ���_���
	ENEMY* pEnemy = &g_aEnemy[0];

	// �\���̂̏�����
	memset(pEnemy, 0, sizeof(ENEMY) * MAX_ENEMY);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL
	);
}

//=====================================================================
// �I������
//=====================================================================
void UninitEnemy(void)
{
	if (g_pVtxBuffEnemy != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

	for (int nCount = 0; nCount < ENEMYTYPE_MAX; nCount++)
	{
		if (g_pTexBuffEnemy[nCount] != NULL)
		{// �e�N�X�`���o�b�t�@�̔j��
			g_pTexBuffEnemy[nCount]->Release();
			g_pTexBuffEnemy[nCount] = NULL;
		}
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateEnemy(void)
{
	ENEMY* pEnemy = &g_aEnemy[0];

	for (int nCount = 0; nCount < MAX_ENEMY; nCount++, pEnemy++)
	{
		if (pEnemy->bUsed == false) continue;

		switch (pEnemy->type)
		{
		case ENEMYTYPE_000:
			pEnemy->nCounterShoot++;
			if (pEnemy->nCounterShoot % pEnemy->nShootInterval == 0)
			{
				pEnemy->nCounterShoot = 0;
				SetEnemyBullet(ENEMYBULLET_TYPE_001, pEnemy->obj.pos, 5.0f, Direction(pEnemy->obj.pos, GetPlayer()->obj.pos));
			}

			break;

		case ENEMYTYPE_001:

			break;
		}
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	ENEMY* pEnemy = &g_aEnemy[0];

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_ENEMY; nCount++, pEnemy++, pVtx += 4)
	{
		// ���_����ݒ�
		SetVertexPos(pVtx, pEnemy->obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pEnemy->obj.color);
		SetVertexTexturePos(pVtx);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemy->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pEnemy = &g_aEnemy[0];
	for (int nCount = 0; nCount < MAX_ENEMY; nCount++, pEnemy++)
	{
		if (pEnemy->obj.bVisible == true && pEnemy->bUsed == true)
		{// �|���S���`��
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexBuffEnemy[pEnemy->type]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//=====================================================================
// �I�u�W�F�N�g�擾����
//=====================================================================
ENEMY* GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=====================================================================
//�G�̐��擾����
//=====================================================================
int GetEnemyLeft(void)
{
	ENEMY* pEnemy = &g_aEnemy[0];
	int nLeft = 0;

	for (int nCount = 0; nCount < MAX_ENEMY; nCount++, pEnemy++)
	{
		if (pEnemy->bUsed == true)
		{
			nLeft++;
		}
	}

	return nLeft;
}

//=====================================================================
// �G�̐ݒ菈��
//=====================================================================
ENEMY* SetEnemy(ENEMYTYPE type, D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ENEMY* pEnemy = &g_aEnemy[0];
	for (int nCount = 0; nCount < MAX_ENEMY; nCount++, pEnemy++)
	{
		if (pEnemy->bUsed == false)
		{
			if (g_pTexBuffEnemy[type] == NULL)
			{// �e�N�X�`���̐���
				D3DXCreateTextureFromFile(
					pDevice,
					g_aEnemyFileName[type],
					&g_pTexBuffEnemy[type]
				);
			}

			memset(pEnemy, 0, sizeof(ENEMY));
			pEnemy->bUsed = true;
			pEnemy->obj.pos = pos;
			pEnemy->obj.size = { INIT_SIZE_X, INIT_SIZE_Y, 0.0f };
			pEnemy->obj.rot = D3DXVECTOR3_ZERO;
			pEnemy->obj.color = INIT_COLOR;
			pEnemy->obj.bVisible = true;

			pEnemy->fLife = INIT_ENEMY_LIFE;
			pEnemy->nShootInterval = INIT_ENEMY_SHOOT_INTERVAL;
			pEnemy->type = type;

			return pEnemy;
		}
	}

	return NULL;
}

void HitEnemy(ENEMY* pEnemy)
{
	pEnemy->fLife -= 1;
	
	if (pEnemy->fLife <= 0)
	{
		pEnemy->bUsed = false;
	}
}