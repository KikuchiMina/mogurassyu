//=========================================================
// 
// "エフェクトの処理"	[effect.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _EFFECT_H_					//このマクロ定義がされてなかったら
#define _EFFECT_H_					//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "objectBillboard.h"

//エフェクト構造大の定義
class CEffect : public CObjectBill
{
public:
	// エフェクトの種類
	typedef enum
	{
		EFFECT_TYPE_BG = 0,	// 地面用
		EFFECT_TYPE_,		// 地面用
		EFFECT_TYPE_MAX
	}EFFECT_TYPE;

	CEffect(PRIORITY nPriority = PRIORITY_EFFECT);
	~CEffect();
	static CEffect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fRadiusSud, int nLife, int nLifeSud);
	static void Create(D3DXVECTOR3 pos, D3DXCOLOR col, int nMinRand, int nMaxRand, float fRadius);
	static void Create(D3DXCOLOR col);
	HRESULT Load(void);
	void Unload(void);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fRadiusSud, int nLife, int nLifeSud);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool CollisionPlayer(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャへのポインタ
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_move;		// 移動量
	D3DXCOLOR m_col;		// 色
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	EFFECT_TYPE m_type;		// 種類
	float m_fRadius;		// 半径(大きさ)
	float m_fRadiusSud;		// 半径の減少
	int m_nLife;			// 寿命(表示時間)
	int m_nLifeSud;			// 寿命の減少
	static int m_nCntAll;	// 総数
};

#endif // !_EFFECT_H_