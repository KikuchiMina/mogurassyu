//=========================================================
// 
// "オブジェクトビルボードの処理"	[objectBillborad.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _OBJECTBILL_H_						// このマクロ定義がされてなかったら
#define _OBJECTBILL_H_						// 2重インクルード防止のマクロ定義

// インクルードファイル
#include "../system/main.h"
#include "object.h"

// オブジェクトクラス定義
class CObjectBill : public CObject
{
public:
	CObjectBill(PRIORITY nPriority = PRIORITY_UI);
	~CObjectBill();
	static CObjectBill* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void SetColor(D3DXCOLOR col);
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	D3DXVECTOR3 GetPosision(void) { return m_pos; }

protected:
	D3DXVECTOR3 m_pos;							// 位置

private:
	LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_rot;					// 向き
	D3DXCOLOR m_col;					// 色
	float m_fWidth;						// 幅サイズ
	float m_fHeight;					// 縦サイズ
	static int m_nNumAll;
};

#endif // !_OBJECTBILL_H_
