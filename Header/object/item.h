//=========================================================
// 
// "ブロックの処理"	[item.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _ITEM_H_					//このマクロ定義がされてなかったら
#define _ITEM_H_					//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "objectX.h"

// ブロッククラス
class  CItem : public CObjectX
{
public:
	// アイテムの種類
	typedef enum
	{
		ITEM_00 = 0,	// 車
		ITEM_01,		// 車
	}ITEM;

	CItem(PRIORITY nPriority = PRIORITY_ITEM);
	~CItem();
	static CItem* Create(D3DXVECTOR3 pos, ITEM type);
	HRESULT Init(D3DXVECTOR3 pos, ITEM type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(void);
	D3DXVECTOR3 GetPos(void) { return m_pos; }

private:
	LPD3DXMESH m_pMash;				// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;		// マテリアルへのポインタ
	DWORD m_dwNumMat;				// マテリアルの数
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3 m_vtxMax, m_vtxMin;	// パーツの最大値 / 最小値
	D3DXVECTOR3 m_pos;				// 位置
	D3DXVECTOR3 m_move;				// 移動量
	ITEM m_type;					// 種類
	static int m_nNumAll;			// 総数
};

#endif // !_BLOCK_H_