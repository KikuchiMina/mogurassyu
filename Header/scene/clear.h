//=========================================================
// 
// "ゲームクリア画面処理"	[clear.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _CLEAR_H_			//このマクロ定義がされてなかったら
#define _CLEAR_H_			//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "../system/manager.h"
#include "../object/object2D.h"
#include "../object/bg.h"

// タイトルクラス
class CClear : public CScene
{
public:
	//タイトルの種類
	typedef enum
	{
		CLEAR_BG_000 = 0,		//背景000
		CLEAR_TITLE_000,		//タイトル000
		CLEAR_PRESS_PLAY,		//PLAY
		CLEAR_PRESS_TUTORIAL,	//TUTORIAL
		CLEAR_MAX
	} CLEAR;

	CClear();
	~CClear();
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

#endif // !_CLEAR_H_