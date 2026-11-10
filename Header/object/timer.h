//=========================================================
// 
// "タイマーの処理"	[timer.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _TIMER_H_						// このマクロ定義がされてなかったら
#define _TIMER_H_						// 2重インクルード防止のマクロ定義

// インクルードファイル
#include "../system/main.h"
#include "number.h"

// マクロ定義
#define NUM_TIME		(8)								// 制限時間の最大桁数

// オブジェクトクラス定義
class CTimer : public CObject
{
public:
	// モデルの種類
	typedef enum
	{
		TIMETYPE_UP = 0,	// 上に表示
		TIMETYPE_MAIN,		// 画面全体に表示
		TIMETYPE_MAX
	}TIMETYPE;

	CTimer(PRIORITY nPriority = PRIORITY_TIME);
	~CTimer();
	static CTimer* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNum, int nTime, TIMETYPE type);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNum, int nTime, TIMETYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	int GetTime(void) { return m_nTime; }
	TIMETYPE GetType(void) { return m_type; }

private:
	CNumber* m_apNumber[NUM_TIME];	// タイマーの最大個数
	TIMETYPE m_type;		// 種類
	int m_nTime;			// タイマーの値
	int m_nTimer[NUM_TIME];	// タイマーを入れる値
	int m_nNumTimer;		// タイマーの桁数
	int m_nTimeCnt;			// タイマー計測
};

#endif // !_TIMER_H_
