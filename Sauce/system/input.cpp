//=========================================================
// 
// "入力処理"	[input.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/system/input.h"
#include "../../Header/system/debugproc.h"
#include "../../Header/system/manager.h"
#define WINDOW_CLASS_NAME "MySampleWindowClass"

//=========================================================
// マクロ定義
//=========================================================
#define DEADZONE_LEFTSTICK		(0.1f)		// 左スティックのデッドゾーン
#define DEADZONE_RIGHTSTICK		(0.1f)		// 右スティックのデッドゾーン
#define MAX_PLAYER				(1)			// 最大人数

//=========================================================
// 静的メンバ変数宣言
//=========================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;		// DirectInputオブジェクトのポインタ

//=========================================================
// コンストラクタ
//=========================================================
CInput::CInput()
{
	m_pDevice = NULL;
}

//=========================================================
// デストラクタ
//=========================================================
CInput::~CInput()
{
}

//=========================================================
// 入力の初期化処理
//=========================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// DirectInputオブジェクトの作成
	if (m_pInput == NULL)
	{
		//DirectInputオブジェクトの作成
		if (FAILED(DirectInput8Create(hInstance,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(void**)&m_pInput,
			NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=========================================================
// 入力の終了処理
//=========================================================
void CInput::Uninit(void)
{
	// 入力デバイスの破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// DirectInputオブジェクトの破棄
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//=========================================================
// 入力の更新処理
//=========================================================
void CInput::Update(void)
{

}

//==================================================================================================================
//=========================================================
// キーボードコンストラクタ
//=========================================================
CInputKeyboard::CInputKeyboard()
{
	m_aKeyState[0] = {};			// プレス情報
	m_aKeyStateTrigger[0] = {};		// トリガー情報
	m_nKeyStateRepeat[0] = {};		// リピート情報
	m_aKeyStateRelease[0] = {};		// リリース情報
}

//=========================================================
// キーボードデストラクタ
//=========================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=========================================================
// キーボードの初期化処理
//=========================================================
HRESULT CInputKeyboard::Init (HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// 入力処理の初期化
	CInput::Init(hInstance, hWnd, bWindow);

	// 入力デバイス(キーボード)の設定
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を取得
	m_pDevice->Acquire();

	return S_OK;
}

//=========================================================
//キーボードの終了処理
//=========================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=========================================================
// キーボードの更新処理
//=========================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];			// キーボードの入力情報
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] & (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]));	// キーボードのリリース情報を保存
			m_aKeyStateTrigger[nCntKey] = ((aKeyState[nCntKey] ^ m_aKeyState[nCntKey]) & aKeyState[nCntKey]);	// キーボードのトリガー情報を保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey];															// キーボードのプレス情報を保存

			if (GetKeyboardPress(nCntKey))		//キーボードのリピート情報を保存
			{
				m_nKeyStateRepeat[nCntKey]++;
			}
			else
			{
				m_nKeyStateRepeat[nCntKey] = 0;
			}
		}
	}
	else
	{
		// デバック表示
		CDebugProc* pDebugProc = CManager::GetDebugProc();
		pDebugProc->PrintDebugProc("[ 入力デバイスからデータを取得できませんんでした！ふぁっく！ ]");

		m_pDevice->Acquire();			// キーボードへのアクセス権を取得
	}
}

//=========================================================
// キーボードのプレス情報を取得
//=========================================================
bool CInputKeyboard::GetKeyboardPress(int nKey)
{
	// 3項演算子
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=========================================================
// キーボードのトリガー情報を取得
//=========================================================
bool CInputKeyboard::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=========================================================
// キーボードのリリース情報を取得
//=========================================================
bool CInputKeyboard::GetKeyboardRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//=========================================================
// キーボードのリピート情報を取得
//=========================================================
bool CInputKeyboard::GetKeyboardRepeat(int nKey)
{
	if (m_nKeyStateRepeat[nKey] == 1 || m_nKeyStateRepeat[nKey] % 10 == 0 && m_nKeyStateRepeat[nKey] >= 30)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//==================================================================================================================
//=========================================================
// マウスコンストラクタ
//=========================================================
CInputMouse::CInputMouse()
{
	m_CurrentMouseState = {};	// マウスの入力情報
	m_PrevMouseState = {};		// マウスの過去の入力情報
	m_mousePos = {};			// マウスの位置
}

//=========================================================
// マウスデストラクタ
//=========================================================
CInputMouse::~CInputMouse()
{
}

//=========================================================
// マウスの入力の初期化処理
//=========================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// 入力デバイス(マウス)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse,
		&m_pDevice,
		NULL)))
	{
		//MyMathUtil::GenerateMessageBox(MB_ICONERROR, "Error (0)", "入力デバイス(マウス)の生成に失敗しました。");
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		//MyMathUtil::GenerateMessageBox(MB_ICONERROR, "Error (0)", "データフォーマットの設定に失敗しました。");
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		//MyMathUtil::GenerateMessageBox(MB_ICONERROR, "Error (0)", "協調モードの設定に失敗しました。");
		return E_FAIL;
	}

	// マウスへのアクセス権を獲得
	m_pDevice->Acquire();

	// ウィンドウハンドル(識別式)の代入
	m_hWnd = hWnd;

	return S_OK;
}

//=========================================================
// マウスの終了処理
//=========================================================
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

//=========================================================
// マウスの更新処理
//=========================================================
void CInputMouse::Update(void)
{
	BYTE aMouseState[NUM_MOUSE_MAX];			// マウスの入力情報

	// 更新前に過去の入力情報(マウス)を保存
	m_PrevMouseState = m_CurrentMouseState;

	// 現在のマウスの入力情報を取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_CurrentMouseState)))
	{
		for (int nCntMouse = 0; nCntMouse < NUM_MOUSE_MAX; nCntMouse++)
		{
			aMouseState[nCntMouse] = m_CurrentMouseState.rgbButtons[nCntMouse];
			m_aMouseStateTrigger[nCntMouse] = ((aMouseState[nCntMouse] ^ m_aMouseState[nCntMouse]) & aMouseState[nCntMouse]);
			m_aMouseStateRelease[nCntMouse] = (m_aMouseState[nCntMouse] & (m_aMouseState[nCntMouse] ^ aMouseState[nCntMouse]));
			m_aMouseState[nCntMouse] = aMouseState[nCntMouse];
		}
	}
	else
	{
		m_pDevice->Acquire();			// マウスへのアクセス権を取得
	}

	CInputMouse::GetMousePos();

	// デバック表示
	CDebugProc* pDebugProc = CManager::GetDebugProc();
	//pDebugProc->PrintDebugProc("\n%d\n%d", m_mousePos.x, m_mousePos.y);
}

//=========================================================
// マウスのプレス情報を取得
//=========================================================
bool CInputMouse::GetMousePress(int nButton)
{
	return (m_aMouseState[nButton] & 0x80) ? true : false;
}

//=========================================================
// マウスのトリガー情報を取得
//=========================================================
bool CInputMouse::GetMouseTrigger(int nButton)
{
	return (m_aMouseStateTrigger[nButton] & 0x80) ? true : false;
}

//=========================================================
// マウスのリリース情報を取得
//=========================================================
bool CInputMouse::GetMouseRelease(int nButton)
{
	return (m_aMouseStateRelease[nButton] & 0x80) ? true : false;
}

//=========================================================
// マウスの位置情報を取得
//=========================================================
POINT CInputMouse::GetMousePos(void)
{
	POINT mousePos;

	// マウスの位置を取得
	if (GetCursorPos(&mousePos) != TRUE)
	{// 取得失敗
		m_pDevice->Acquire();			// マウスへのアクセス権を取得
	}
	else
	{// 取得成功
		// 取得した座標をウィンドウ内の座標に変換
		ScreenToClient(m_hWnd, &mousePos);
		m_mousePos = mousePos;			// 現在の位置を保存
	}

	return m_mousePos;
}

//=========================================================================================================
// ジョイパッドの初期化処理
//=========================================================================================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// メモリのクリア
	for (int nCntPad = 0; nCntPad < MAX_PLAYER; nCntPad++)
	{
		memset(&m_Joypad.joykeyState, 0, sizeof(XINPUT_STATE));
		memset(&m_Joypad.joykeyStateTrigger, 0, sizeof(XINPUT_STATE));
		memset(&m_Joypad.vibration, 0, sizeof(XINPUT_VIBRATION));
	}

	// XInputのステートを設定(有効にする)
	XInputEnable(true);

	// 振動時間の初期化
	m_nTime = 10;

	return S_OK;
}

//=========================================================
// ジョイパッドの終了処理
//=========================================================
void CInputJoypad::Uninit(void)
{
	// XInputのステートを設定(無効にする)
	XInputEnable(false);
}

//=========================================================
// ジョイパッドの更新処理
//=========================================================
void CInputJoypad::Update(void)
{
	XINPUT_STATE joykeyState;							// ジョイパッドの入力情報

	// ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		// ジョイパッドのリリース情報を保存
		m_Joypad.joykeyStateRelease->Gamepad.wButtons = m_Joypad.joykeyState.Gamepad.wButtons & (m_Joypad.joykeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons);

		// ジョイパッドのトリガー情報を保存
		m_Joypad.joykeyStateTrigger.Gamepad.wButtons = (m_Joypad.joykeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & joykeyState.Gamepad.wButtons;

		// ジョイパッドの情報を保存
		m_Joypad.joykeyState = joykeyState;

		for (int nCntJoy = 0; nCntJoy < NUM_JOY_MAX; nCntJoy++)
		{
			// ジョイパッドのリピート情報
			if (GetJoypadPress((JOYKEY)nCntJoy))
			{
				m_Joypad.nJoykeyStateRepeat[nCntJoy]++;
			}
			else
			{
				m_Joypad.nJoykeyStateRepeat[nCntJoy] = 0;
			}
		}
	}

	// 振動時間の減り
	m_nTime--;

	if (m_nTime <= 0)
	{
		StopVibration();
		m_nTime = 0;
	}
}

//=========================================================
// ジョイパッドのプレス情報を取得
//=========================================================
bool CInputJoypad::GetJoypadPress(JOYKEY key)
{
	return(m_Joypad.joykeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=========================================================
// ジョイパッドのトリガー情報を取得
//=========================================================
bool CInputJoypad::GetJoypadTrigger(JOYKEY key)
{
	return(m_Joypad.joykeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=========================================================
// ジョイパッドのリリース情報を取得
//=========================================================
bool CInputJoypad::GetJoypadRelease(JOYKEY Key)
{
	return (m_Joypad.joykeyStateRelease->Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//=========================================================
// ジョイパッドのリピート情報を取得
//=========================================================
bool CInputJoypad::GetJoypadRepeat(JOYKEY key)
{
	if (m_Joypad.nJoykeyStateRepeat[key] == 1 ||
		m_Joypad.nJoykeyStateRepeat[key] % 10 == 0 &&
		m_Joypad.nJoykeyStateRepeat[key] >= 30)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=========================================================
// ジョイパッドの左スティック情報を取得
//=========================================================
bool CInputJoypad::GetJoypadStickLeft(JOYKEY key)
{

	if (JOYKEY_LEFT_STICK_LEFT == key)
	{
		return(m_Joypad.joykeyState.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? true : false;
	}
	else if (JOYKEY_LEFT_STICK_RIGHT == key)
	{
		return(m_Joypad.joykeyState.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? true : false;
	}
	if (JOYKEY_LEFT_STICK_UP == key)
	{
		return(m_Joypad.joykeyState.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? true : false;
	}
	else if (JOYKEY_LEFT_STICK_DOWN == key)
	{
		return(m_Joypad.joykeyState.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? true : false;
	}
	else
	{
		return false;
	}
}

//=======================================
// レフトジョイスティックの情報
//=======================================
bool CInputJoypad::GetJoypadLeftStick(D3DXVECTOR3* value)
{
	(*value).x = (float)m_Joypad.joykeyState.Gamepad.sThumbLX / 0x7fff;
	(*value).y = (float)-m_Joypad.joykeyState.Gamepad.sThumbLY / 0x7fff;
	(*value).z = -atan2f((float)-m_Joypad.joykeyState.Gamepad.sThumbLX, (float)m_Joypad.joykeyState.Gamepad.sThumbLY);

	// デッドゾーン
	if (-DEADZONE_LEFTSTICK <= (*value).x && (*value).x <= DEADZONE_LEFTSTICK &&
		-DEADZONE_LEFTSTICK <= (*value).y && (*value).y <= DEADZONE_LEFTSTICK)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//=======================================
// ライトジョイスティックの情報
//=======================================
bool CInputJoypad::GetJoypadRightStick(D3DXVECTOR3* value)
{
	if (value)
	{
		(*value).x = (float)m_Joypad.joykeyState.Gamepad.sThumbRX / 0x7fff;
		(*value).y = (float)-m_Joypad.joykeyState.Gamepad.sThumbRY / 0x7fff;
		(*value).z = -atan2f((float)-m_Joypad.joykeyState.Gamepad.sThumbRX, (float)m_Joypad.joykeyState.Gamepad.sThumbRY);
	}

	// デッドゾーン
	if (-DEADZONE_LEFTSTICK <= (*value).x && (*value).x <= DEADZONE_LEFTSTICK &&
		-DEADZONE_LEFTSTICK <= (*value).y && (*value).y <= DEADZONE_LEFTSTICK)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//=========================================================================================================
// 振動の情報
//=========================================================================================================
XINPUT_VIBRATION* CInputJoypad::GetXInput(void)
{
	return &m_Joypad.vibration;					// 振動情報の先頭アドレスを返す
}

//=========================================================
// 振動をスタートする処理
//=========================================================
void CInputJoypad::SetVibration(int nLeftMotor, int nRightMotor, int nTime)
{
	m_nTime = nTime;
	
	m_Joypad.vibration.wLeftMotorSpeed = nLeftMotor;		// use any value between 0-65535 here
	m_Joypad.vibration.wRightMotorSpeed = nRightMotor;		// use any value between 0-65535 here

	XInputSetState(0, &m_Joypad.vibration);
}

//=========================================================
// 振動をストップする処理
//=========================================================
void CInputJoypad::StopVibration(void)
{
	m_Joypad.vibration.wLeftMotorSpeed = 0;			// use any value between 0-65535 here
	m_Joypad.vibration.wRightMotorSpeed = 0;		// use any value between 0-65535 here

	XInputSetState(0, &m_Joypad.vibration);
}
