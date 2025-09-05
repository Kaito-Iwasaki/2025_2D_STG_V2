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
	
#include "baseScene.h"
#include "collision.h"
#include "player.h"
#include "bullet.h"
#include "enemybullet.h"
#include "spriteEffect.h"
#include "fade.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\player000.png"
#define NUM_TEXTURE				(2)

#define INIT_POS				{SCREEN_WIDTH / 2, SCREEN_HEIGHT/ 2 + 200, 0.0f}
#define INIT_SIZE				{64.0f, 64.0f, 0.0f}
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void SetPlayerState(PLAYERSTATE state);

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

	// �\���̂̏�����
	memset(&g_player, 0, sizeof(PLAYER));
	g_player.obj.pos = INIT_POS;
	g_player.obj.size = INIT_SIZE;
	g_player.obj.color = INIT_COLOR;
	g_player.obj.bVisible = true;

	g_player.state = PLAYERSTATE_DAMAGED;
	g_player.fSpeed = INIT_PLAYER_SPEED;
	g_player.fShootSpeed = INIT_SHOOT_SPEED;
	g_player.fLife = INIT_PLAYER_LIFE;
	g_player.fCharge = INIT_PLAYER_CHARGE;
	g_player.hitBoxSize = PLAYER_HITBOX_SIZE;

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
	RECT rectScreen = GAME_SCREEN_RECT;
	D3DXVECTOR3 direction = D3DXVECTOR3_ZERO;
	float fMagnitude;
	XINPUT_GAMEPAD* pGamepad = &GetJoypad()->Gamepad;

	// ***** ��ԕʏ��� *****
	g_player.nCounterState++;
	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:
		g_player.obj.bVisible = true;
		if (g_player.fLife < PLAYER_HEAL_MAX)
		{
			g_player.fLife += PLAYER_HEAL_SCALE;
		}
		break;

	case PLAYERSTATE_APPEAR:
		if (g_player.nCounterState > 60)
		{
			SetPlayerState(PLAYERSTATE_NORMAL);
		}
		break;

	case PLAYERSTATE_DAMAGED:
		if (g_player.nCounterState % 3 == 0)
		{
			g_player.obj.bVisible ^= 1;
		}

		if (g_player.nCounterState > 120)
		{
			SetPlayerState(PLAYERSTATE_NORMAL);
		}
		break;

	case PLAYERSTATE_DIED:
		g_player.obj.bVisible = false;
		if (g_player.nCounterState > 120)
		{
			SetPlayerState(PLAYERSTATE_END);
		}
		return;

	case PLAYERSTATE_END:
		SetFade(SCENE_RESULT);
		return;
	}

	// ***** �ړ� *****
	// �L�[�{�[�h&�W���C�p�b�h����
	if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LEFT) || GetJoystickPress(JOYSTICK_L_LEFT))
	{// ��
		direction.x -= 1;
	}
	if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RIGHT) || GetJoystickPress(JOYSTICK_L_RIGHT))
	{// �E
		direction.x += 1;
		//g_player.obj.bInversed = false;
	}
	if (GetKeyboardPress(DIK_W) || GetJoypadPress(JOYKEY_UP) || GetJoystickPress(JOYSTICK_L_UP))
	{// ��
		direction.y -= 1;
	}
	if (GetKeyboardPress(DIK_S) || GetJoypadPress(JOYKEY_DOWN) || GetJoystickPress(JOYSTICK_L_DOWN))
	{// ��
		direction.y += 1;
	}

	// �ړ������ɉ����ăe�N�X�`���p�^�[����ݒ�
	g_player.nTexPattern = Clamp(direction.x, -1, 1);
	g_player.obj.bInversed = g_player.nTexPattern == -1;

	// �����̑傫�������߂�
	fMagnitude = sqrtf(direction.x * direction.x + direction.y * direction.y);

	if (fMagnitude != 0)
	{// �ʒu���X�V
		g_player.obj.pos += D3DXVECTOR3(direction.x / fMagnitude, direction.y / fMagnitude, 0.0f) * g_player.fSpeed;
	}

	// �ʒu����
	Clampf(&g_player.obj.pos.x, rectScreen.left + g_player.obj.size.x / 2, rectScreen.right - g_player.obj.size.x / 2);
	Clampf(&g_player.obj.pos.y, rectScreen.top + g_player.obj.size.y / 2, rectScreen.bottom - g_player.obj.size.y / 2);

	// ***** �V���b�g *****
	g_player.nCounterShoot++;
	if ((GetKeyboardPress(DIK_SPACE) || GetJoypadPress(JOYKEY_A)) && g_player.nCounterShoot % INIT_SHOOT_INTERVAL == 0)
	{// �e����
		g_player.nCounterShoot = 0;
		PlaySound(SOUND_LABEL_SE_SHOOT, 0.1f);
		SetBullet(g_player.obj.pos + D3DXVECTOR3(5, -5, 0), g_player.fShootSpeed, D3DX_PI);
		SetBullet(g_player.obj.pos + D3DXVECTOR3(-5, -5, 0), g_player.fShootSpeed, D3DX_PI);
		SetBullet(g_player.obj.pos + D3DXVECTOR3(15, 0, 0), g_player.fShootSpeed, D3DX_PI);
		SetBullet(g_player.obj.pos + D3DXVECTOR3(-15, 0, 0), g_player.fShootSpeed, D3DX_PI);
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
	// �ȉ��̏�Ԃł͔��肵�Ȃ�
	if (g_player.state == PLAYERSTATE_APPEAR)	return;		// �o������
	if (g_player.state == PLAYERSTATE_DAMAGED)	return;		// �_���[�W����
	if (g_player.state == PLAYERSTATE_DIED)		return;		// ���S���
	if (g_player.state == PLAYERSTATE_END)		return;		// �Q�[���I�����

	g_player.fLife -= 1;

	if (g_player.fLife <= 0)
	{
		PlaySound(SOUND_LABEL_SE_HIT02);
		SetVibration(60000, 60000, 50);
		SetSpriteEffect(SPRITEEFFECTYPE_EXPLOSION, g_player.obj.pos, 1.5f);
		SetPlayerState(PLAYERSTATE_DIED);
	}
	else
	{
		PlaySound(SOUND_LABEL_SE_HIT01);
		SetVibration(40000, 40000, 50);
		SetPlayerState(PLAYERSTATE_DAMAGED);
	}
}

void SetPlayerState(PLAYERSTATE state)
{
	g_player.state = state;
	g_player.nCounterState = 0;
}
