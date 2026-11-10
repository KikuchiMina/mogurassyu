//=========================================================
// 
// "画面遷移処理"	[scene.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _SCENE_H_			//このマクロ定義がされてなかったら
#define _SCENE_H_			//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "../object/object.h"

// シーンクラス
class CScene
{
public:
	typedef enum
	{
		MODE_TITLE = 0,		// タイトル画面
		MODE_STAGESELECT,	// ステージ選択画面
		MODE_GAME,			// ゲーム画面
		MODE_CLEAR,			// ゲームクリア画面
		MODE_OVER,			// ゲームオーバー画面
		MODE_RANKING,		// ランキング画面
		MODE_MAX
	}MODE;

	CScene(MODE mode = MODE_TITLE);
	virtual ~CScene();
	static CScene* Create(MODE mode);
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	static MODE GetMode(void) { return m_mode; }

private:
	static MODE m_mode;	// モード
};

#endif // !_SCENE_H_