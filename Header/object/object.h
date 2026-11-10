//=========================================================
// 
// "オブジェクトの処理"	[object.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _OBJECT_H_	//このマクロ定義がされてなかったら
#define _OBJECT_H_	//2重インクルード防止のマクロ定義

// インクルードファイル
#include "../system/main.h"

// マクロ定義
#define MAX_NUMBER		20		// ナンバーの最大個数

// オブジェクトクラス定義
class CObject
{
public:
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_OBJECTX,
		TYPE_NUM,
		TYPE_MAX
	}TYPE;

	// 優先順位
	typedef enum
	{
		PRIORITY_SCENE = 0,
		PRIORITY_BG,		// bg
		PRIORITY_MESHFIELD,	// field
		PRIORITY_FIELD,		// field
		PRIORITY_MULTITEX,	// MultiTexture
		PRIORITY_FIELDBG,	// fieldBg
		PRIORITY_SHADOW,	// shadow
		PRIORITY_OBJECT,	// object
		PRIORITY_BLOCK,		// block
		PRIORITY_EFFECT,	// effect
		PRIORITY_PLAYER,	// player
		PRIORITY_ENEMY,		// enemy
		PRIORITY_ITEM,		// item
		PRIORITY_MODELPAR,	// model particle
		PRIORITY_MESHRING,	// fieldring
		PRIORITY_NUM,		// number
		PRIORITY_UI,		// UI
		PRIORITY_STOCK,		// stock
		PRIORITY_PAUSE,		// pause
		PRIORITY_TIME,		// time
		PRIORITY_MAX
	}PRIORITY;

	//CObject();
	CObject(PRIORITY nPriority = PRIORITY_OBJECT);
	virtual ~CObject();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	void SetType(TYPE type) { m_type = type; }
	TYPE GetType(void) { return m_type; }
	//static CObject* GetObject(int nIdx) { return m_apObject[nIdx]; }
	static CObject* GetObject(int nPriority, int nIdx) { return m_apObject[nPriority][nIdx]; }
	static CObject* GetTop(int nPriority) { return m_apTop[nPriority]; }
	CObject* GetNext(void) { return m_pNext; }

protected:
	void Release(void);

private:
	//static CObject* m_apObject[MAX_OBJECT];
	static CObject* m_apObject[PRIORITY_MAX][MAX_OBJECT];		// [優先順位の総数][オブジェクトの最大数]
	static int m_nNumAll;					// オブジェクト総数
	int m_nID;								// 自分自身のID
	int m_nPriority;						// 優先順位
	TYPE m_type;							// 種類
	static CObject* m_apTop[PRIORITY_MAX];	// 先頭オブジェクトへのポインタ
	static CObject* m_apCur[PRIORITY_MAX];	// 最後尾オブジェクトへのポインタ
	CObject* m_pPrev;						// 前のオブジェクトへのポインタ
	CObject* m_pNext;						// 次のオブジェクトへのポインタ
	bool m_bDeath;							// 死亡フラグ
};

#endif // !_OBJECT_H_
