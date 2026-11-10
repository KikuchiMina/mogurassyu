//=========================================================
// 
// "メッシュ壁の処理"	[meshwall.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _MESHWALL_H_					//このマクロ定義がされてなかったら
#define _MESHWALL_H_					//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "object3D.h"

// マクロ定義
#define MAX_VTX		100		// 頂点の最大数

// フィールドクラス
class CMeshWall : public CObject
{
public:
	CMeshWall(PRIORITY nPriority = PRIORITY_FIELD);
	~CMeshWall();
	static CMeshWall* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
	HRESULT Load(void);
	void Unload(void);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	float Collision(D3DXVECTOR3 posOld, D3DXVECTOR3 pos);
	float Collision(D3DXVECTOR3 posOld, D3DXVECTOR3 pos, float f);
	bool InField(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(int nIdx) { return m_posVtx[nIdx]; }

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;		// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3 m_pos;				// 位置
	D3DXVECTOR3 m_posPlayer;		// プレイヤーの位置保存用
	D3DXVECTOR3 m_rot;				// 位置
	D3DXVECTOR3 m_nor[MAX_VTX];		// 法線
	D3DXVECTOR3 m_posVtx[MAX_VTX];	// それぞれpVtxの位置
	float m_fWidth;					// 幅
	float m_fHeight;				// 高さ

	D3DXVECTOR2 m_Block;			// 分割数
	int m_nNumAllVtx;				// 頂点数の総数
	int m_nNumAllIdx;				// インデックス数の総数
	int m_nDiaNum;					// 斜めにするときのVtxの値
	float m_nDiaHeight;				// 斜めにするときの高さ
};

#endif // !_MESHWALL_H_