//=========================================================
// 
// "ゲームオーバー画面処理"	[over.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _OVER_H_			//このマクロ定義がされてなかったら
#define _OVER_H_			//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "../system/manager.h"
#include "../object/object2D.h"
#include "../object/bg.h"

// タイトルクラス
class COver : public CScene
{
public:
	//タイトルの種類
	typedef enum
	{
		OVER_BG_000 = 0,		//背景000
		OVER_TITLE_000,			//タイトル000
		OVER_PRESS_PLAY,		//PLAY
		OVER_PRESS_TUTORIAL,	//TUTORIAL
		OVER_MAX
	} OVER;

	COver();
	~COver();
	void Create(void);
	HRESULT Load(void);
	void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//=== 2Dobject ===//
	static CBg* GetBg(void) { return pBg; }

private:
	//=== 2Dobject ===//
	static CBg* pBg;		// 背景へのポインタ

};

#endif // !_OVER_H_