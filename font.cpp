//=====================================================================
//
// �摜�I�u�W�F�N�g [font.cpp]
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

#include "font.h"

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
// ***** �\���� *****
// 
//*********************************************************************
typedef struct
{
	char aFontFileName[MAX_PATH];
	char aFontName[MAX_PATH];
}FONT_INFO;

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
FONT g_aFont[MAX_FONT] = {};

FONT_INFO g_aFontInfo[FONT_LABEL_MAX][MAX_PATH] = {
	{"x10y12pxDonguriDuel.ttf", "x10y12pxDonguriDuel" }
};

//=====================================================================
// ����������
//=====================================================================
void InitFont(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X
	VERTEX_2D* pVtx;							// ���_���
	FONT* pFont = &g_aFont[0];

	// �\���̂̏�����
	memset(pFont, 0, sizeof(FONT) * MAX_FONT);

	// �t�H���g��ǉ�
	for (int nCount = 0; nCount < FONT_LABEL_MAX; nCount++)
	{
		AddFontResourceEx(&g_aFontInfo[nCount]->aFontFileName[0], FR_PRIVATE, 0);
	}
}

//=====================================================================
// �I������
//=====================================================================
void UninitFont(void)
{
	for (int nCount = 0; nCount < MAX_FONT; nCount++)
	{
		if (g_aFont[nCount].font != NULL)
		{
			g_aFont[nCount].font->Release();
			g_aFont[nCount].font = NULL;
		}
	}

	for (int nCount = 0; nCount < FONT_LABEL_MAX; nCount++)
	{
		RemoveFontResourceEx(&g_aFontInfo[nCount]->aFontFileName[0], FR_PRIVATE, 0);
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateFont(void)
{

}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawFont(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	FONT* pFont = &g_aFont[0];

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	pFont = &g_aFont[0];
	for (int nCount = 0; nCount < MAX_FONT; nCount++, pFont++)
	{
		if (pFont != NULL && pFont->obj.bVisible && pFont->bUsed)
		{// �e�L�X�g�`��
			RECT rect;
			float fMagnitude;

			rect = {
				(long)pFont->obj.pos.x,
				(long)pFont->obj.pos.y,
				(long)(pFont->obj.pos.x + pFont->obj.size.x),
				(long)(pFont->obj.pos.y + pFont->obj.size.y),
			};

			pFont->font->DrawText(NULL, pFont->aText, -1, &rect, DT_LEFT, D3DXCOLOR(255, 0, 0, 255));
		}
	}
}

//=====================================================================
// �I�u�W�F�N�g�擾����
//=====================================================================
FONT* GetFont(void)
{
	return &g_aFont[0];
}

//=====================================================================
// �摜�̐ݒ菈��
//=====================================================================
FONT* SetFont(FONT_LABEL label, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, const char* aText)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	FONT* pFont = &g_aFont[0];
	for (int nCount = 0; nCount < MAX_FONT; nCount++, pFont++)
	{
		if (pFont->font == NULL)
		{
			memset(pFont, 0, sizeof(FONT));
			pFont->bUsed = true;
			pFont->obj.pos = pos;
			pFont->obj.size = size;
			pFont->obj.color = INIT_COLOR;
			pFont->nID = nCount;
			pFont->obj.bVisible = true;
			strcpy(&pFont->aText[0], aText);

			D3DXFONT_DESC fontDesc = {
				40,								// Height
				0,								// Width (0 = ����)
				FW_NORMAL,						// Weight
				1,								// MipLevels
				FALSE,							// Italic
				DEFAULT_CHARSET,				// CharSet
				OUT_DEFAULT_PRECIS,				// OutputPrecision
				CLIP_DEFAULT_PRECIS,			// Quality
				DEFAULT_PITCH | FF_DONTCARE,	// PitchAndFamily
				"x10y12pxDonguriDuel"			// FaceName�i.ttf���j
			};

			D3DXCreateFontIndirect(pDevice, &fontDesc, &pFont->font);

			return pFont;
		}
	}

	return NULL;
}

//=====================================================================
// �摜�̍폜�����i�w��j
//=====================================================================
void DeleteFont(int nID)
{
	memset(&g_aFont[nID], 0, sizeof(FONT));
}

//=====================================================================
// �摜�̍폜�����i�ꊇ�j
//=====================================================================
void DeleteFont(void)
{
	memset(&g_aFont[0], 0, sizeof(FONT) * MAX_FONT);
}
