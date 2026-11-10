//=========================================================
// 
// "ナンバー処理"	[number.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _NUMBER_H_						// このマクロ定義がされてなかったら
#define _NUMBER_H_						// 2重インクルード防止のマクロ定義

// インクルードファイル
#include "../system/main.h"
#include "object.h"

// オブジェクトクラス定義
class CNumber : CObject
{
public:
	CNumber();
	~CNumber();
	static CNumber* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
	HRESULT Load(void);
	void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void SetColor(D3DXCOLOR col);
	void SetNumber(int nNumber);

protected:
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_rot;	// 向き
	D3DXCOLOR m_col;	// カラー

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// 頂点バッファへのポインタ
	int m_nCounterAnime;					// アニメーションカウンター
	int m_nPatternAnime;					// アニメーションパターンNo.
	float m_fWidth;							// 幅サイズ
	float m_fHeight;						// 縦サイズ
};

#endif // !_NUMBER_H_
