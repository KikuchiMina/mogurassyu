//=========================================================
// 
// "マルチテクスチャ地面の処理"	[fieldmulti.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _FIELDMULTI_H_					//このマクロ定義がされてなかったら
#define _FIELDMULTI_H_					//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "object.h"

// フィールドクラス
class CFieldMulti : public CObject
{
public:
	typedef enum
	{
		FIELD_MULTITEX_00 = 0,	// 壁フィールドテクスチャ
		FIELD_MULTITEX_01,		// 壁フィールドテクスチャ
		FIELD_MULTITEX_MAX
	}FIELD_MULTITEX;

	CFieldMulti(PRIORITY nPriority = PRIORITY_MULTITEX);
	~CFieldMulti();
	static CFieldMulti* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fBack);
	HRESULT Load(void);
	void Unload(void);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight, float fBack);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPos(void) { return m_pos; }

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[FIELD_MULTITEX_MAX];	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;							// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_rot;		// 向き
	DWORD m_dwNumMat;		// マテリアルの数
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	FIELD_MULTITEX m_type;	// 種類
	float m_fWidth;			// 幅
	float m_fHeight;		// 高さ
	float m_fBack;			// 奥行
	static int m_nNumAll;	// 総数
};

#endif // !_FIELDMULTI_H_