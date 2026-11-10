//=========================================================
// 
// "フェード処理"	[fade.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _FADE_H_	//このマクロ定義がされてなかったら
#define _FADE_H_	//2重インクルード防止のマクロ定義

#include "scene.h"
#include "../system/main.h"

// マクロ定義
#define FADE_DOWN	// テクスチャ有りの処理
//#undef FADE_DOWN	// テクスチャ有りの処理を無効にする(有効にしたい時はこの行をコメントアウトする)

#ifdef FADE_DOWN
// フェードクラス
class CFade
{
public:
	//フェード構造体
	typedef enum
	{
		FADE_NONE = 0,		//何もしていない状態
		FADE_IN,			//フェードイン状態
		FADE_OUT,			//フェードアウト状態
		FADE_MAX
	}FADE;

	typedef enum
	{
		FADE_TEX_LPILL = 0,	// 左柱
		FADE_TEX_RPILL,		// 右柱
		FADE_TEX_LDOOR,		// 左襖
		FADE_TEX_RDOOR,		// 右襖
		FADE_TEX_MAX
	}FADE_TEX;

	CFade();
	~CFade();
	static CFade* Create(CScene::MODE mode);
	HRESULT Init(void);
	HRESULT Init(CScene::MODE modeNext);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetFade(CScene::MODE modeNext);
	FADE GetFade(void) { return m_fade; }
	HRESULT Load(void);
	void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[FADE_TEX_MAX];	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;	// 頂点バッファへのポインタ
	static FADE m_fade;					// フェードの状態
	static CScene::MODE m_modeNext;		// 次の画面(モード)
	static CScene* m_pScene;			// シーンへのポインタ
	static int m_OpenCnt;				// 扉のつっかえのカウント
	D3DXVECTOR3	m_pos[FADE_TEX_MAX];	// 位置
	static bool bDoor;					// ドアの開閉
};
#else
// フェードクラス
class CFade
{
public:
	//フェード構造体
	typedef enum
	{
		FADE_NONE = 0,		//何もしていない状態
		FADE_IN,			//フェードイン状態
		FADE_OUT,			//フェードアウト状態
		FADE_MAX
	}FADE;

	typedef enum
	{
		FADE_TEX_LPILL = 0,	// 左柱
		FADE_TEX_RPILL,		// 右柱
		FADE_TEX_LDOOR,		// 左襖
		FADE_TEX_RDOOR,		// 右襖
		FADE_TEX_MAX
	}FADE_TEX;

	CFade();
	~CFade();
	static CFade* Create(CScene::MODE mode);
	HRESULT Init(void);
	HRESULT Init(CScene::MODE modeNext);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetFade(CScene::MODE modeNext);
	FADE GetFade(void) { return m_fade; }

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[FADE_TEX_MAX];	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;	// 頂点バッファへのポインタ
	static FADE m_fade;					// フェードの状態
	static CScene::MODE m_modeNext;		// 次の画面(モード)
	static CScene* m_pScene;			// シーンへのポインタ
	static D3DXCOLOR m_color;			// ポリゴン(フェード)の色
	D3DXVECTOR3	m_pos[FADE_TEX_MAX];	// 位置
};
#endif // FADE_DOWN

#endif // !_FADE_H_