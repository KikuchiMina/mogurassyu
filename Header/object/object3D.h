//=========================================================
// 
// "オブジェクト3Dの処理"	[object3D.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _OBJECT3D_H_						// このマクロ定義がされてなかったら
#define _OBJECT3D_H_						// 2重インクルード防止のマクロ定義

// インクルードファイル
#include "../system/main.h"
#include "object.h"

// オブジェクトクラス定義
class CObject3D : public CObject
{
public:
	CObject3D(PRIORITY nPriority = PRIORITY_OBJECT);
	~CObject3D();
	static CObject3D* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fBack);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, float fBack);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos, float fWidth, float fHeight, float fBack);
	void SetPosition(D3DXVECTOR3 pos, float fWidth, float fHeight, int nCnt);
	void SetPosition(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3);
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetColor(D3DXCOLOR col);
	void AddSetTexture(D3DXVECTOR2 tex);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	float GetHeight(D3DXVECTOR3 posOld, D3DXVECTOR3 pos);
	void Collision(D3DXVECTOR3 posOld, D3DXVECTOR3 pos);
	float Sway(void);
	D3DXVECTOR3 AddMove(D3DXVECTOR3 move);
	D3DXVECTOR3 GetPosision(void) { return m_pos; }

protected:

private:
	LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャへのポインタ
	int m_nIdxTexture;					// テクスチャの指定
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_move;			// 移動量
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_nor[4];		// 法線
	D3DXVECTOR3 m_posVtx[4];	// それぞれpVtxの位置
	static int m_nNumAll;		// 総数
	float m_fWidth;				// 幅
	float m_fHeight;			// 高さ
	float m_fBack;				// 奥行
	bool m_bRight;				// 右から入った時
	bool m_bLeft;				// 左から入った時
	bool m_bSway;				// 揺れる動き
};

#endif // !_OBJECT3D_H_
