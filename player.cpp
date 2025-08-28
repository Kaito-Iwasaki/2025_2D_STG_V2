//=====================================================================
//
// �v���C���[�I�u�W�F�N�g [player.cpp]
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
#include "player.h"
#include "bullet.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\player000.png"
#define NUM_TEXTURE				(2)

#define INIT_POS				{SCREEN_WIDTH / 2, SCREEN_HEIGHT/ 2, 0.0f}
#define INIT_SIZE				{64.0f, 64.0f, 0.0f}
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

#define INIT_PLAYER_SPEED		(5.0f)
#define INIT_SHOOT_SPEED		(17.0f)
#define INIT_SHOOT_INTERVAL		(5)

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffPlayer = NULL;
PLAYER g_player;

//=====================================================================
// ����������
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X
	VERTEX_2D* pVtx;							// ���_���

	// �\���̂̏�����
	memset(&g_player, 0, sizeof(PLAYER));
	g_player.obj.pos = INIT_POS;
	g_player.obj.size = INIT_SIZE;
	g_player.obj.color = INIT_COLOR;
	g_player.obj.bVisible = true;

	g_player.fSpeed = INIT_PLAYER_SPEED;
	g_player.fShootSpeed = INIT_SHOOT_SPEED;

	// �e�N�X�`���̓ǂݍ���
	if (TEXTURE_FILENAME)
	{// �e�N�X�`���쐬
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffPlayer
		);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL
	);
}

//=====================================================================
// �I������
//=====================================================================
void UninitPlayer(void)
{
	memset(&g_player, 0, sizeof(PLAYER));

	if (g_pVtxBuffPlayer != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	if (g_pTexBuffPlayer != NULL)
	{// �e�N�X�`���o�b�t�@�̔j��
		g_pTexBuffPlayer->Release();
		g_pTexBuffPlayer = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdatePlayer(void)
{
	D3DXVECTOR3 direction = D3DXVECTOR3_ZERO;
	float fMagnitude;

	// ***** ��ԕʏ��� *****
	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:

		break;

	case PLAYERSTATE_APPEAR:

		break;

	case PLAYERSTATE_DAMAGED:

		break;

	case PLAYERSTATE_DIED:

		break;
	}
	g_player.nCounterState++;

	// ***** �ړ� *****
	if (GetKeyboardPress(DIK_A))
	{// ��
		direction.x -= 1;
		g_player.obj.bInversed = true;
	}
	if (GetKeyboardPress(DIK_D))
	{// �E
		direction.x += 1;
		g_player.nTexPattern = 1;
		g_player.obj.bInversed = false;
	}
	if (GetKeyboardPress(DIK_W))
	{// ��
		direction.y -= 1;
	}
	if (GetKeyboardPress(DIK_S))
	{// ��
		direction.y += 1;
	}

	g_player.nTexPattern = direction.x;

	// �����̑傫�������߂�
	fMagnitude = sqrtf(direction.x * direction.x + direction.y * direction.y);

	if (fMagnitude != 0)
	{// �ʒu���X�V
		g_player.obj.pos += D3DXVECTOR3(direction.x / fMagnitude, direction.y / fMagnitude, 0.0f) * g_player.fSpeed;
	}

	// �ʒu����
	Clampf(&g_player.obj.pos.x, 0 + g_player.obj.size.x / 2, SCREEN_WIDTH - g_player.obj.size.x / 2);
	Clampf(&g_player.obj.pos.y, 0 + g_player.obj.size.y / 2, SCREEN_HEIGHT - g_player.obj.size.y / 2);

	// ***** �V���b�g *****
	g_player.nCounterShoot++;
	if (GetKeyboardPress(DIK_SPACE) && g_player.nCounterShoot % INIT_SHOOT_INTERVAL == 0)
	{// �e����
		g_player.nCounterShoot = 0;
		SetBullet(g_player.obj.pos, g_player.fShootSpeed, D3DX_PI);
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	SetVertexPos(pVtx, g_player.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_player.obj.color);
	SetVertexTexturePos(pVtx, g_player.nTexPattern, NUM_TEXTURE, g_player.obj.bInversed);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_player.obj.bVisible == true)
	{// �|���S���`��
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexBuffPlayer);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=====================================================================
// �I�u�W�F�N�g�擾����
//=====================================================================
PLAYER* GetPlayer(void)
{
	return &g_player;
}

void HitPlayer(void)
{
	if (PLAYERSTATE_APPEAR)		return;
	if (PLAYERSTATE_DAMAGED)	return;
	if (PLAYERSTATE_DIED)		return;
	if (PLAYERSTATE_END)		return;

	g_player.fLife -= 1;

	if (g_player.fLife <= 0)
	{
		g_player.state = PLAYERSTATE_DIED;
	}
	else
	{
		PLAYERSTATE_DAMAGED;
	}

	g_player.nCounterState = 0;
}
