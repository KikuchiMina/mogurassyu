//=========================================================
// 
// "プレイヤーの処理"	[player.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _SCORE_H_						// このマクロ定義がされてなかったら
#define _SCORE_H_						// 2重インクルード防止のマクロ定義

// インクルードファイル
#include "../system/main.h"
#include "number.h"

// マクロ定義
#define NUM_PLACE		(8)								//スコアの最大桁数

// オブジェクトクラス定義
class CScore : public CObject
{
public:
	CScore(PRIORITY nPriority = PRIORITY_NUM);
	~CScore();
	static CScore* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddScore(int nValue);
	void SetScore(int nValue);
	int GetScore(void) { return m_nScore; }

private:
	static CNumber* m_apNumber[NUM_PLACE];	// スコアの最大個数
	static int m_nScore;					// スコアの値
	int m_nNumber[NUM_PLACE];				// スコアを入れる値
	static int m_nNumScore;					// スコアの桁数
};

#endif // !_SCORE_H_
