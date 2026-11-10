//=========================================================
// 
// "ポーズ処理"	[pause.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _PAUSE_H_				//このマクロ定義がされてなかったら
#define _PAUSE_H_				//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "../object/object2D.h"

// ポーズクラス
class CPause : public CObject2D
{
public:
	//ポーズメニュー
	typedef enum
	{
		PAUSE_MENU_RESTART = 0,		// 最初から
		PAUSE_MENU_STAGESELECT,		// ステージ選択
		PAUSE_MENU_RETITLE,			// タイトルに戻る
		PAUSE_MENU_CLOSE,			// 閉じる
		PAUSE_MENU_MAX
	}PAUSE_MENU;

	CPause();
	//CPause(PRIORITY nPriority = PRIORITY_PAUSE);
	~CPause();
	static CPause* Create(void);
	HRESULT Load(void);
	void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[PAUSE_MENU_MAX];	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;						// 頂点バッファへのポインタ
	static D3DXVECTOR3 m_pos[PAUSE_MENU_MAX];				// 位置
	static PAUSE_MENU m_type;								// 種類
	static int m_nData;
};

#endif // !_PAUSE_H_