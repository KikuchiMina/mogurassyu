//=========================================================
// 
// "地面の処理"	[field.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _FIELD_H_					//このマクロ定義がされてなかったら
#define _FIELD_H_					//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "object3D.h"

// フィールドクラス
class CField : public CObject3D
{
public:
	typedef enum
	{
		FIELD_TEX_WALL_00 = 0,	// 壁フィールドテクスチャ
		FIELD_TEX_WALL_01,		// 壁フィールドテクスチャ
		FIELD_TEX_MATO_00,		// 的フィールドテクスチャ
		FIELD_TEX_MATO_01,		// 的フィールドテクスチャ
		FIELD_TEX_MATO_02,		// 的フィールドテクスチャ
		FIELD_TEX_MAX
	}FIELD_TEX;
	CField(PRIORITY nPriority = PRIORITY_FIELD);
	~CField();
	static CField* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, float fBack, FIELD_TEX type);
	HRESULT Load(void);
	void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, float fBack, FIELD_TEX type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	FIELD_TEX GetType(void) { return m_type; }

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[FIELD_TEX_MAX];	// テクスチャへのポインタ
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 向き
	FIELD_TEX m_type;	// 種類
	float m_fWidth;		// 幅
	float m_fHeight;	// 高さ
	float m_fBack;		// 奥行
	int m_nIdxTexture;	// テクスチャの指定
	int m_LengthCnt;	// 円作成のカウント
};

#endif // !_FIELD_H_