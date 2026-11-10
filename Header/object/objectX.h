//=========================================================
// 
// "Xファイルの処理"	[objectX.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _OBJECTX_H_				// このマクロ定義がされてなかったら
#define _OBJECTX_H_				// 2重インクルード防止のマクロ定義

// インクルードファイル
#include "../system/main.h"
#include "object.h"

#define NUM_TEXTURE		100		//読み込むテクスチャの最大数

// オブジェクトクラス定義
class CObjectX : public CObject
{
public:
	CObjectX(PRIORITY nPriority = PRIORITY_OBJECT);
	~CObjectX();
	static CObjectX* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, float fWidth, float fHeight);
	HRESULT Init(void);
	HRESULT Init(LPD3DXBUFFER m_pBuffMat, LPD3DXMESH m_pMash, DWORD m_dwNumMat);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(D3DXVECTOR3 rot){ m_rot = rot; }
	void SetScale(D3DXVECTOR3 scl) { m_scl = scl; }
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture[NUM_TEXTURE]);
	D3DXVECTOR3 GetPosision(void) { return m_pos; }

protected:

private:
	LPDIRECT3DTEXTURE9	m_apTexture[NUM_TEXTURE];	//テクスチャへのポインタ
	LPD3DXMESH m_pMash;			// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;	// マテリアルへのポインタ
	DWORD m_dwNumMat;			// マテリアルの数
	D3DXVECTOR3 m_pos;			// 移動量
	D3DXVECTOR3 m_move;			// 移動量
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_scl;			// スケール
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	static int m_nNumAll;		// 総数
	float m_fWidth;				// 幅
	float m_fHeight;			// 高さ
};

#endif // !_OBJECTX_H_
