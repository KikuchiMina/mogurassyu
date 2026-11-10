//=========================================================
// 
// "影の処理"	[shadow.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _SHADOW_H_		//このマクロ定義がされてなかったら
#define _SHADOW_H_		//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "object3D.h"

// 影の構造体
class CShadow : public CObject3D
{
public:
	CShadow(PRIORITY nPriority = PRIORITY_SHADOW);
	~CShadow();
	static CShadow* Create(D3DXVECTOR3 pos);
	HRESULT Load(void);
	void Unload(void);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		//頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_rot;		//向き
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
	float m_fWidth;			//幅
	float m_fDepth;			//奥行
};

#endif // !_SHADOW_H_