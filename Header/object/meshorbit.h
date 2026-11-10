////=========================================================
//// 
//// "メッシュオービット処理"	[meshorbit.h]
//// Author : KikuchiMina
//// 
////=========================================================
//#ifndef _MESHORBIT_H_						// このマクロ定義がされてなかったら
//#define _MESHORBIT_H_						// 2重インクルード防止のマクロ定義
//
//#include "../system/main.h"
//#include "object.h"
//
//// マクロ定義
//#define MAX_VTX		100		// 頂点の最大数
//
//// オブジェクトクラス定義
//class CMeshOrbit : public CObject
//{
//public:
//	CMeshOrbit(PRIORITY nPriority = PRIORITY_OBJECT);
//	~CMeshOrbit();
//	static CMeshOrbit* Create(D3DXVECTOR3 pos);
//	HRESULT Init(void);
//	HRESULT Init(D3DXVECTOR3 pos);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//
//private:
//	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;	// 頂点バッファへのポインタ
//	D3DMATRIX* m_pMtxParent;			// 親マトリックス
//	D3DXVECTOR3 m_aOffset[2];			// 両端のオフセット
//	D3DXCOLOR m_aCol[2];				// 両端の基準の色
//	D3DXVECTOR3 m_aPosPoint[MAX_VTX];	// 計算後の各頂点座標
//	D3DXCOLOR m_aColPlint[MAX_VTX];		// 計算後の各頂点カラー
//	D3DXVECTOR3 m_pos;					// 位置
//};
//
//#endif // !_OBJECT3D_H_
