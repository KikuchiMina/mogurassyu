//=========================================================
// 
// "モデルの処理"	[model.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _MODEL_H_		//このマクロ定義がされてなかったら
#define _MODEL_H_		//2重インクルード防止のマクロ定義

#include "../system/main.h"

#define MAX_TEX		20	// テクスチャの最大数
#define MAX_MODEL	10	// モデルの最大数

// モデルクラス
class CModel
{
public:
	CModel();
	~CModel();
	static CModel* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static CModel* Create(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetParent(CModel* pModel) { m_pParent = pModel; }
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	HRESULT Init(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	HRESULT Lead(const char* pFilename);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CModel* GetModel(void) { return m_apModel; }
	D3DXVECTOR3 GetPos(void) { return m_posOri; }
	D3DXVECTOR3 GetRot(void) { return m_rotOri; }
	D3DXVECTOR3 GetMaxvtx(void) { return m_vtxMax; }
	D3DXVECTOR3 GetMinvtx(void) { return m_vtxMin; }
	void PosRot(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static int GetNumModel(void) { return m_nNumModel; }

private:
	static CModel* m_apModel;
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEX];	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;			// 頂点バッファへのポインタ
	LPD3DXMESH m_pMash;							// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;					// マテリアルへのポインタ
	DWORD m_dwNumMat;							// マテリアルの数
	D3DXVECTOR3 m_pos, m_posOri;				// 位置、元の位置
	D3DXVECTOR3 m_rot, m_rotOri;				// 向き、元の向き
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 m_vtxMax, m_vtxMin;				//パーツの最大値 / 最小値
	int m_nNumVtx;								// 頂点数を取得
	CModel* m_pParent;							// 親のモデルのポインタ
	static int m_nNumModel;						// 読み込んだ数
};

#endif // !_MODEL_H_