//=========================================================
// 
// "デバックの処理"	[debugproc.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include<stdio.h>			// sprintf使うため
#include "../../Header/system/debugproc.h"
#include "../../Header/system/manager.h"

//=========================================================
// マクロ定義
//=========================================================

//=========================================================
// 静的メンバ変数宣言
//=========================================================
int CDebugProc::m_Num = 0;
bool CDebugProc::m_Debug = true;

//=========================================================
// コンストラクタ
//=========================================================
CDebugProc::CDebugProc()
{
	m_pFont = NULL;			// フォントへのポインタ
	m_aStrDebug[0] = {};	// 文字列(デバック情報)を格納するバッファ

	//デバック情報バッファのクリア
	memset(&m_aStrDebug[0], 0, sizeof m_aStrDebug);
}

//=========================================================
// デストラクタ
//=========================================================
CDebugProc::~CDebugProc()
{
	m_pFont = NULL;			// フォントへのポインタ
	m_aStrDebug[0] = {};	// 文字列(デバック情報)を格納するバッファ

	//デバック情報バッファのクリア
	memset(&m_aStrDebug[0], 0, sizeof m_aStrDebug);
}

//=========================================================
// デバックの初期化処理
//=========================================================
void CDebugProc::Init(void)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	//デバック表示用フォントの作成
	D3DXCreateFont(pDevice,
					18, 0, 0, 0,
					FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS,
					DEFAULT_QUALITY,
					DEFAULT_PITCH,
					"terminal", &m_pFont);
}

//=========================================================
// デバックの終了処理
//=========================================================
void CDebugProc::Uninit(void)
{
	//デバック表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//=========================================================
// デバックの更新処理
//=========================================================
void CDebugProc::Update(void)
{
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ
	
	if (pKeyBoaed->GetKeyboardTrigger(DIK_F1) == true)
	{ // F1キーが押された
		m_Debug = m_Debug ? false : true;
	}

	// デバックモードの切り替え
	if (pKeyBoaed->GetKeyboardTrigger(DIK_1) == true)
	{
		m_Num--;
	}
	else if (pKeyBoaed->GetKeyboardTrigger(DIK_2) == true)
	{
		m_Num++;
	}

	if (m_Num < DEBUG_PLAYER)
	{
		m_Num = DEBUG_MAX;
	}
	else if (m_Num > DEBUG_MAX)
	{
		m_Num = DEBUG_PLAYER;
	}
}

//=========================================================
// デバックの描画処理
//=========================================================
void CDebugProc::Draw(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	int D3DCOLOR_A = 0;

	if (m_Debug == true)
	{
		D3DCOLOR_A = 255;
	}

	//テキストの描画
	m_pFont->DrawText(NULL, &m_aStrDebug[0], -1, &rect, DT_LEFT,
		D3DCOLOR_RGBA(255, 255, 255, D3DCOLOR_A));

	//デバック情報バッファのクリア
	memset(&m_aStrDebug[0], 0, sizeof m_aStrDebug);
}

//=========================================================
// デバック情報を登録
//=========================================================
void CDebugProc::PrintDebugProc(const char* fmt, ...)
{
#if 0

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	int     x = 0;
	float	f = 0.0f;
	char* cc = NULL;
	char aChar[MAX_CHAR] = {};

	va_list list;
	va_start(list, fmt);

	for (char* c = (char*)fmt; *c != '\0'; c++)
	{
		switch (*c)
		{
		case '%':
			c++;
			switch (*c)
			{
			case 's':
				cc = va_arg(list, char*);
				sprintf(&aChar[0], "%s", cc);
				break;
			case 'f':
				f = (float)va_arg(list, double);
				sprintf(&aChar[0], "%.3f", f);
				break;
			case 'd':
				x = va_arg(list, int);
				sprintf(&aChar[0], "%d", x);
				break;
			}
			break;
		default:
			sprintf(&aChar[0], "%c", *c);
			break;
		}

		//文字列の連結
		strcat(m_aStrDebug, aChar);
	}

	//デバック情報バッファのクリア
	memset(&aChar[0], 0, sizeof aChar);
#endif // 0
}