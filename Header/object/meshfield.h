//=========================================================
// 
// "メッシュフィールドの処理"	[meshfield.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _MESHFIELD_H_					//このマクロ定義がされてなかったら
#define _MESHFIELD_H_					//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "object3D.h"

// マクロ定義
#define MAX_VTX		100		// 頂点の最大数

// フィールドクラス
class CMeshField : public CObject
{
public:
	typedef enum
	{
		FIELD_WIDTH = 0,	// 地面
		FIELD_WALL,			// 壁
	}FIELD;

	CMeshField(PRIORITY nPriority = PRIORITY_MESHFIELD);
	~CMeshField();
	static CMeshField* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fBack, D3DXVECTOR2 Block, int nDiaNum, float nDiaHeight, float fDiffHeight);
	static CMeshField* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, D3DXVECTOR2 Block);
	HRESULT Load(void);
	void Unload(void);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight, float fBack, D3DXVECTOR2 Block, int nDiaNum, float nDiaHeight, float fDiffHeight);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, D3DXVECTOR2 Block);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	float Collision(D3DXVECTOR3 posOld, D3DXVECTOR3 pos);
	float Collision(D3DXVECTOR3 posOld, D3DXVECTOR3 pos, float f);
	bool InField(D3DXVECTOR3 pos);
	float CollisionCamera(void);
	D3DXVECTOR3 GetPos(int nIdx) { return m_posVtx[nIdx]; }

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
	float m_fWidth;					// 幅
	float m_fHeight;				// 高さ
	float m_fBack;					// 奥行
	FIELD m_type;					// フィールドの種類

	D3DXVECTOR2 m_Block;			// 分割数
	int m_nNumAllVtx;				// 頂点数の総数
	int m_nNumAllIdx;				// インデックス数の総数
	int m_nDiaNum;					// 斜めにするときのVtxの値
	float m_nDiaHeight;				// 斜めにするときの高さ
};

#endif // !_FIELD_H_