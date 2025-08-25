//=====================================================================
// 
// 入力処理 [input.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "input.h"

// マクロ定義
#define NUM_KEY_MAX		(256)	// 最大キー数
#define REPEAT_START	(30)	// リピート開始までのカウント
#define REPEAT_INTERVAL	(5)		// リピート毎のカウント

// グローバル変数宣言
LPDIRECTINPUT8 g_pInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;

BYTE g_aKeyState[NUM_KEY_MAX];
BYTE g_aKeyTriggerState[NUM_KEY_MAX];
BYTE g_aKeyReleaseState[NUM_KEY_MAX];
int g_aKeyRepeatState[NUM_KEY_MAX];

XINPUT_STATE g_joyKeyState;
XINPUT_STATE g_joyKeyTriggerState;
XINPUT_STATE g_joyKeyReleaseState;
int g_aJoyKeyRepeatState[JOYKEY_MAX];
XINPUT_VIBRATION g_vibration;

//=====================================================================
// 
// ***** キーボード *****
//
//=====================================================================
//=====================================================================
// 初期化処理
//=====================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DIrectInputオブジェクトの作成
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInput,
		NULL
	)))
	{
		return E_FAIL;
	}

	// 入力デバイスの生成
	if (FAILED(g_pInput->CreateDevice(
		GUID_SysKeyboard,
		&g_pDevKeyboard,
		NULL
	)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	)))
	{
		return E_FAIL;
	}

	// キーボードのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitKeyboard(void)
{
	// 入力デバイス（キーボード）破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	// キーボードの入力情報
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyTriggerState[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			g_aKeyReleaseState[nCntKey] = g_aKeyState[nCntKey] & (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]);
			g_aKeyState[nCntKey] = aKeyState[nCntKey];

			if (GetKeyboardPress(nCntKey))
			{
				g_aKeyRepeatState[nCntKey] += 1;
			}
			else
			{
				g_aKeyRepeatState[nCntKey] = 0;
			}
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();	// キーボードのアクセス権利を再取得
	}
}

//=====================================================================
// キーボードのプレス情報を取得
//=====================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//=====================================================================
// キーボードのトリガー情報を取得
//=====================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyTriggerState[nKey] & 0x80) ? true : false;
}

//=====================================================================
// キーボードのリリース情報を取得
//=====================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyReleaseState[nKey] & 0x80) ? true : false;
}

//=====================================================================
// キーボードのリピート情報を取得
//=====================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyRepeatState[nKey] == 1 || g_aKeyRepeatState[nKey] >= REPEAT_START && g_aKeyRepeatState[nKey] % REPEAT_INTERVAL == 0) ? true : false;
}

//=====================================================================
// 
// ***** ジョイパッド *****
//
//=====================================================================
//=====================================================================
// 初期化処理
//=====================================================================
HRESULT InitJoypad(void)
{
	// メモリのクリア
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));
	memset(&g_vibration, 0, sizeof(XINPUT_VIBRATION));

	// XINPUTのステートを有効にする
	XInputEnable(true);

	return S_OK;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitJoypad(void)
{
	// XInputのステートを無効
	XInputEnable(false);
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateJoypad(void)
{
	XINPUT_STATE joyKeyState;

	// ジョイパッドの状態
	if (XInputGetState(0, &joyKeyState) == ERROR_SUCCESS)
	{
		g_joyKeyTriggerState.Gamepad.wButtons = (g_joyKeyState.Gamepad.wButtons ^ joyKeyState.Gamepad.wButtons)
			& joyKeyState.Gamepad.wButtons;
		g_joyKeyReleaseState.Gamepad.wButtons = g_joyKeyState.Gamepad.wButtons & (g_joyKeyState.Gamepad.wButtons
			^ joyKeyState.Gamepad.wButtons);
		g_joyKeyState = joyKeyState;

		for (int nCntKey = 0; nCntKey < JOYKEY_MAX; nCntKey++)
		{
			if (GetJoypadPress((JOYKEY)nCntKey))
			{
				g_aJoyKeyRepeatState[nCntKey] += 1;
			}
			else
			{
				g_aJoyKeyRepeatState[nCntKey] = 0;
			}
		}
	}
}

//=====================================================================
// ジョイパッドの取得
//=====================================================================
XINPUT_STATE* GetJoypad(void)
{
	return &g_joyKeyState;
}

//=====================================================================
// ジョイパッドのプレス情報を取得
//=====================================================================
bool GetJoypadPress(JOYKEY key)
{
	return (g_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=====================================================================
// ジョイパッドのトリガー情報を取得
//=====================================================================
bool GetJoypadTrigger(JOYKEY key)
{
	return (g_joyKeyTriggerState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=====================================================================
// ジョイパッドのリリース情報を取得
//=====================================================================
bool GetJoypadRelease(JOYKEY key)
{
	return (g_joyKeyReleaseState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=====================================================================
// ジョイパッドのリピート情報を取得
//=====================================================================
bool GetJoypadRepeat(JOYKEY key)
{
	return (
		g_aJoyKeyRepeatState[key] == 1 ||
		g_aJoyKeyRepeatState[key] >= REPEAT_START &&
		g_aJoyKeyRepeatState[key] % REPEAT_INTERVAL == 0
		) ? true : false;
}

//=====================================================================
// 振動情報の設定処理
//=====================================================================
void SetVibration(WORD wLeftMotorSpeed, WORD wRightMotorSpeed)
{
	g_vibration.wLeftMotorSpeed = wLeftMotorSpeed; // use any value between 0-65535 here
	g_vibration.wRightMotorSpeed = wRightMotorSpeed; // use any value between 0-65535 here
	XInputSetState(0, &g_vibration);
}