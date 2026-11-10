//=========================================================
// 
// "ブロックの処理"	[block.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _BLOCK_H_					//このマクロ定義がされてなかったら
#define _BLOCK_H_					//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "objectX.h"

// ブロッククラス
class  CBlock : public CObjectX
{
public:
	//ブロックの種類
	typedef enum
	{
		BLOCKTYPE_NORMAL = 0,		//普通
		BLOCKTYPE_MAX
	}BLOCKTYPE;

	CBlock(PRIORITY nPriority = PRIORITY_BLOCK);
	~ CBlock();
	static CBlock* Create(D3DXVECTOR3 pos, BLOCKTYPE type);
	static void Create(BLOCKTYPE type);
	HRESULT Init(D3DXVECTOR3 pos, BLOCKTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3* pos, D3DXVECTOR3 posOld, float* fHiegth);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	bool GetOnBlock(void) { return m_OnBlock; }

private:
	LPD3DXMESH m_pMash;				// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;		// マテリアルへのポインタ
	DWORD m_dwNumMat;				// マテリアルの数
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3 m_vtxMax, m_vtxMin;	// パーツの最大値 / 最小値
	D3DXVECTOR3 m_pos;				// 位置
	D3DXVECTOR3 m_scl;				// スケール
	BLOCKTYPE m_type;				// タイプ
	static int m_nNumAll;			// 総数
	static bool m_OnBlock;			// 上に乗っている
};

#endif // !_BLOCK_H_