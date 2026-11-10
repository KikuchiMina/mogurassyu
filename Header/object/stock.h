//=========================================================
// 
// "プレイヤーのストック表示処理"	[stock.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _STOCK_H_					//このマクロ定義がされてなかったら
#define _STOCK_H_					//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "object2D.h"

// マクロ定義
#define MAX_STOCK	10	// ストックの最大数

// 体力バークラス
class CStock : public CObject2D
{
public:
	typedef enum
	{
		STOCK_NONE = 0,	// 無し
		STOCK_TRUE_R,	// 正
		STOCK_TRUE_Y,	// 正
		STOCK_TRUE_W,	// 正
		STOCK_FALSE,	// 誤
		STOCK_MAX
	}STOCK;

	CStock(PRIORITY nPriority = PRIORITY_STOCK);
	~CStock();
	static CStock* Create(D3DXVECTOR3 pos);
	HRESULT Load(void);
	void Unload(void);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetTex(int nIdx, STOCK stock);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[STOCK_MAX];	// テクスチャへのポインタ
	static CObject2D* m_apObject2D[MAX_STOCK];			// ストックの最大個数
	STOCK m_stock;								// ストックのタイプ
	static int m_nNumAll;						// スタックの総数
	static int m_nNumStock;						// 今のストック
	static int m_nNumAllTrue;					// trueの総数
};

#endif // !_STOCK_H_