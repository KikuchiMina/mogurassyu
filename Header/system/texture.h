//=========================================================
// 
// "テクスチャの処理"	[texture.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _TEXTURE_H_		//このマクロ定義がされてなかったら
#define _TEXTURE_H_		//2重インクルード防止のマクロ定義

#include "main.h"

// マクロ定義
#define NUM_TEX		50		// テクスチャの総数

// テクスチャクラス
class CTexture
{
public:
	typedef enum
	{
		TEX_PLAYER = 0,
		TEX_ENEMY,
		TEX_FIELD,
		TEX_BULLET,
		TEX_MAX
	}TEX;

	CTexture();
	~CTexture();
	HRESULT Load(void);
	void Unload(void);
	int Register(const char* pFilename);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

private:
	LPDIRECT3DTEXTURE9 m_apTexture[NUM_TEX];	// テクスチャへのポインタ
	static int m_nNumAll;						// テクスチャの数
};

#endif // !_TEXTURE_H_