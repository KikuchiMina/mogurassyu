//=========================================================
// 
// "背景の処理"	[bg.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _BG_H_						// このマクロ定義がされてなかったら
#define _BG_H_						// 2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "object2D.h"

// マクロ定義
#define MAX_BG_TEX		10		// テクスチャの最大数

// 背景クラス
class CBg : public CObject2D
{
public:
	typedef enum
	{ // ゲーム中
		BGTEX_GAME_OO = 0,	// スタート背景
		BGTEX_GAME_O1,		// ready
		BGTEX_GAME_O2,		// start
		BGTEX_GAME_O3,		// finish
		BGTEX_TITLE,	// タイトル
		BGTEX_GAME,		// ゲーム
		BGTEX_CLEAR,	// クリア
		BGTEX_OVER,		// ゲームオーバー
		BGTEX_MAX
	}BGTEX;

	CBg(PRIORITY nPriority = PRIORITY_BG);
	~CBg();
	static CBg* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, BGTEX type);
	static void GameCreate(void);
	static void GameEndCreate(void);
	HRESULT Load(const char* TexSet);
	void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight, BGTEX type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool StartMove(void);
	void FinishMove(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_BG_TEX];	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;					// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;		// 目標位置
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_rot;		// 向き
	BGTEX m_type;			// 種類
	float m_fWidth;			// 幅
	float m_fHeight;		// 高さ
	static int m_nReadyCnt;	// readyを出すタイミング
	static int m_nStartCnt;	// startを出すタイミング
	static int m_nStartFin;	// startを出すタイミング
	int m_nCounterAnime;	// アニメーションカウンター
	int m_nPatternAnime;	// アニメーションパターンNo.
	static bool m_bStartEnd;	// スタート処理が終わったかどうか
	static bool m_bFinishEnd;	// 終了処理が終わったかどうか
};

#endif // !_BG_H_
