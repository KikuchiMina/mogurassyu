//=========================================================
// 
// "メッシュリングの処理"	[meshring.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _MESHRING_H_					//このマクロ定義がされてなかったら
#define _MESHRING_H_					//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "object3D.h"

// マクロ定義
#define MAX_VTX		100		// 頂点の最大数

// フィールドクラス
class CMeshRing : public CObject
{
public:
	CMeshRing(PRIORITY nPriority = PRIORITY_MESHRING);
	~CMeshRing();
	static CMeshRing* Create(D3DXVECTOR3 pos, float fInSide, float fOutSide, D3DXVECTOR2 Block);
	HRESULT Load(void);
	void Unload(void);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, float fInSide, float fOutSide, D3DXVECTOR2 Block);
	void Uninit(void);
	void Update(void);
	void Draw(void);

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
	float m_fInSide;				// 内側の長さ
	float m_fOutSide;				// 外側の長さ
};

#endif // !_FIELD_H_