//=========================================================
// 
// "デバックの処理"	[debugproc.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _DEBUGPROC_H_		//このマクロ定義がされてなかったら
#define _DEBUGPROC_H_		//2重インクルード防止のマクロ定義

#include "main.h"

//=========================================================
// マクロ定義
//=========================================================
#define MAX_CHAR	2048	//最大文字数(不変)

//=========================================================
// デバッククラスの定義
//=========================================================
class CDebugProc
{
public:
	typedef enum
	{
		DEBUG_PLAYER = 0,	// プレイヤー
		DEBUG_CAMERA,		// カメラ
		DEBUG_LIGHT,		// ライト
		DEBUG_OBJECTX,		// オブジェクトX
		DEBUG_OBJECT2D,		// オブジェクト2D
		DEBUG_OBJECT3D,		// オブジェクト3D
		DEBUG_MAX
	}DEBUG;

	CDebugProc();
	~CDebugProc();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void PrintDebugProc(const char* fmt, ...);		//デバック情報を登録
	static int GetNum(void) { return m_Num; }

private:
	LPD3DXFONT m_pFont;				// フォントへのポインタ
	char m_aStrDebug[MAX_CHAR];		// 文字列(デバック情報)を格納するバッファ
	static int m_Num;				// デバックモードの番号
	static bool m_Debug;			// デバックを表示するかどうか
};

#endif // !_DEBUGPROC_H_