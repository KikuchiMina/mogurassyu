//=========================================================
// 
// "メッシュシリンダーの処理"	[meshcylinder.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _MESHCYLINDER_H_					//このマクロ定義がされてなかったら
#define _MESHCYLINDER_H_					//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "object3D.h"

// マクロ定義
#define MAX_VTX		100		// 頂点の最大数

// フィールドクラス
class CMeshCylinder : public CObject
{
public:
	CMeshCylinder(PRIORITY nPriority = PRIORITY_MESHFIELD);
	~CMeshCylinder();
	static CMeshCylinder* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fBack, D3DXVECTOR2 Block);
	HRESULT Load(void);
	void Unload(void);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight, float fBack, D3DXVECTOR2 Block);
	void Uninit(void);
	void Update(void);
	void Draw(void);
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
	D3DXVECTOR2 m_Block;			// 分割数
	int m_nNumAllVtx;				// 頂点数の総数
	int m_nNumAllIdx;				// インデックス数の総数
	float m_fWidth;					// 幅
	float m_fHeight;				// 高さ
	float m_fBack;					// 高さ
};

#endif // !_MESHCYLINDER_H_