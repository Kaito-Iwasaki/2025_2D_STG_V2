//=====================================================================
//
// �F��I�u�W�F�N�g [player.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "player.h"
#include "input.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\player000.png"

#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE_X				(64.0f)
#define INIT_SIZE_Y				(64.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
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
	g_player.obj.pos = { INIT_POS_X, INIT_POS_Y, 0.0f };
	g_player.obj.size = { INIT_SIZE_X, INIT_SIZE_Y, 0.0f };
	g_player.obj.color = INIT_COLOR;
	g_player.obj.bVisible = true;

	g_player.fSpeed = 10.0f;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE_FILENAME,
		&g_player.obj.pTexBuff
	);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_player.obj.pVtxBuff,
		NULL
	);
}

//=====================================================================
// �I������
//=====================================================================
void UninitPlayer(void)
{
	if (g_player.obj.pVtxBuff != NULL)
	{// ���_�o�b�t�@�̔j��
		g_player.obj.pVtxBuff->Release();
		g_player.obj.pVtxBuff = NULL;
	}

	if (g_player.obj.pTexBuff != NULL)
	{// �e�N�X�`���o�b�t�@�̔j��
		g_player.obj.pTexBuff->Release();
		g_player.obj.pTexBuff = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdatePlayer(void)
{
	D3DXVECTOR3 direction = D3DXVECTOR3_ZERO;
	float fMagnitude;

	// �L�[�{�[�h����
	if (GetKeyboardPress(DIK_A))
	{// ��
		direction.x -= 1;
	}
	if (GetKeyboardPress(DIK_D))
	{// �E
		direction.x += 1;
	}
	if (GetKeyboardPress(DIK_W))
	{// ��
		direction.y -= 1;
	}
	if (GetKeyboardPress(DIK_S))
	{// ��
		direction.y += 1;
	}

	// �����̑傫�������߂�
	fMagnitude = sqrtf(direction.x * direction.x + direction.y * direction.y);
	if (fMagnitude == 0) fMagnitude = 1;	// 0�Ȃ�1�ɕ␳

	// �ʒu���X�V
	g_player.obj.pos += D3DXVECTOR3(direction.x / fMagnitude, direction.y / fMagnitude, 0.0f) * g_player.fSpeed;
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

	if (g_player.obj.bVisible == true)
	{// �\�����
		// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
		g_player.obj.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_����ݒ�
		SetVertexPos(pVtx, g_player.obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, g_player.obj.color);
		SetVertexTexturePos(pVtx);

		// ���_�o�b�t�@���A�����b�N
		g_player.obj.pVtxBuff->Unlock();

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_player.obj.pVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_player.obj.pTexBuff);

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
