//=========================================================
// 
// "タイトル画面処理"	[title.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _TITLE_H_			//このマクロ定義がされてなかったら
#define _TITLE_H_			//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "../system/manager.h"
#include "../object/object2D.h"
#include "../object/bg.h"

// タイトルクラス
class CTitle : public CScene
{
public:
	//タイトルの種類
	typedef enum
	{
		TITLE_BG_000 = 0,		//背景000
		TITLE_TITLE_000,		//タイトル000
		TITLE_PRESS_PLAY,		//PLAY
		TITLE_PRESS_TUTORIAL,	//TUTORIAL
		TITLE_PRESS_CAT,		//猫
		TITLE_MAX
	} TITLE;

	CTitle();
	~CTitle();
	void Create(void);
	HRESULT Load(void);
	void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//=== 2Dobject ===//
	static CBg* GetBg(void) { return pBg; }

	static int GetData(void) { return m_Data; }

private:
	//=== 2Dobject ===//
	static CBg* pBg;		// 背景へのポインタ

	static int m_Data;						// カウント
};

#endif // !_TITLE_H_