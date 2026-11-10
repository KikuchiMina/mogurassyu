//=========================================================
// 
// "モデルのパーティクル処理"	[model particle.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _MODELPARTICLE_H_					//このマクロ定義がされてなかったら
#define _MODELPARTICLE_H_					//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "objectX.h"

//エフェクト構造大の定義
class CModelParticle : public CObjectX
{
public:
	// アイテムの種類
	typedef enum
	{
		MODELPAR_00 = 0,	// box
		MODELPAR_01,		// 頭
		MODELPAR_02,		// 地球
		MODELPAR_MAX
	}MODELPAR;

	CModelParticle(PRIORITY nPriority = PRIORITY_ITEM);
	~CModelParticle();
	static CModelParticle* Create(D3DXVECTOR3 pos, MODELPAR type);
	static void Create(D3DXVECTOR3 pos, MODELPAR type, int nRandMax);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos, MODELPAR type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(void);
	D3DXVECTOR3 GetPos(void) { return m_pos; }

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MODELPAR_MAX][NUM_TEXTURE];	//テクスチャへのポインタ
	static LPD3DXMESH m_pMash[MODELPAR_MAX];			// メッシュ(頂点情報)へのポインタ
	static LPD3DXBUFFER m_pBuffMat[MODELPAR_MAX];		// マテリアルへのポインタ
	static DWORD m_dwNumMat[MODELPAR_MAX];				// マテリアルの数
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3 m_vtxMax, m_vtxMin;	// パーツの最大値 / 最小値
	D3DXVECTOR3 m_pos;				// 位置
	D3DXVECTOR3 m_move;				// 移動量
	D3DXVECTOR3 m_rot;				// 向き
	D3DXVECTOR3 m_scl;				// スケール
	D3DXVECTOR3 m_scaledSize;		// モデルの半径サイズ
	MODELPAR m_type;				// 種類
	static int m_nNumAll;			// 総数
	float m_posKeepY;				// 位置の保存
	bool m_bJump;					// ジャンプしたかどうか
};

#endif // !_MODELPARTICLE_H_