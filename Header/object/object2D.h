//=========================================================
// 
// "オブジェクト2Dの処理"	[object.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _OBJECT2D_H_						// このマクロ定義がされてなかったら
#define _OBJECT2D_H_						// 2重インクルード防止のマクロ定義

// インクルードファイル
#include "../system/main.h"
#include "object.h"

// オブジェクトクラス定義
class CObject2D : public CObject
{
public:
	CObject2D(PRIORITY nPriority = PRIORITY_OBJECT);
	~CObject2D();
	static CObject2D* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void SetPosition(D3DXVECTOR3 pos, float fWidth, float fHeight, int nCnt);
	void SetColor(D3DXCOLOR col);
	void SetMove(D3DXVECTOR3 move);
	void SetRot(D3DXVECTOR3 rot);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	D3DXVECTOR3 GetPosision(void) { return m_pos; }

protected:
	D3DXVECTOR3 m_pos;							// 位置

private:
	LPDIRECT3DTEXTURE9	m_pTexture;				// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;			// 頂点バッファへのポインタ
	D3DXVECTOR3 m_move;							// 移動量
	D3DXVECTOR3 m_rot;							// 向き
	TYPE m_type;								// 種類
	int m_nCounterAnime;						// アニメーションカウンター
	int m_nPatternAnime;						// アニメーションパターンNo.
	float m_fWidth;								// 幅サイズ
	float m_fHeight;							// 縦サイズ
	static int m_nNumAll;
};

#endif // !_OBJECT2D_H_
