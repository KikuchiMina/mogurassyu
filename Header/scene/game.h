//=========================================================
// 
// "ゲーム画面処理"	[game.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _GAME_H_			//このマクロ定義がされてなかったら
#define _GAME_H_			//2重インクルード防止のマクロ定義

#include "../system/main.h"
#include "../system/manager.h"
#include "../object/number.h"
#include "../object/score.h"
#include "../object/timer.h"
#include "../object/block.h"
#include "../object/item.h"
#include "../object/effect.h"
#include "../object/shadow.h"
#include "../object/model particle.h"
#include "../object/field.h"
#include "../object/fieldmulti.h"
#include "../object/meshring.h"
#include "../object/meshcylinder.h"
#include "../object/stock.h"
#include "../object/bg.h"

// マクロ定義
#define MIN_HEIGHT	-1000.0f	// プレイヤーの高さの初期値

// タイトルクラス
class CGame : public CScene
{
public:
	// 設置オブジェクトの種類
	typedef enum
	{
		SETOBJECT_GRASS = 0,// 草
		SETOBJECT_CUBE,		// キューブ
		SETOBJECT_FIELD,	// フィールド
		SETOBJECT_ENEMY,	// 敵
		SETOBJECT_BULLET,	// 弾
		SETOBJECT_MAX
	}SETOBJECT;

	CGame();
	~CGame();
	void Create(void);
	HRESULT Load(void);
	void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool LoadFile(void);
	static bool GetPause(void) { return m_bPause; }
	static void SetPause(bool bPause) { m_bPause = bPause; }

	//=== 2Dobject ===//
	static CBg* GetBg(void) { return m_pBg; }
	static CNumber* GetNumber(void) { return m_pNumber; }
	static CScore* GetScore(void) { return m_pScore; }
	static CTimer* GetTimer(void) { return m_pTimer; }
	static CStock* GetStock(void) { return m_pStock; }

	//=== 3Dobject ===//
	static CField* GetField(void) { return pField; }
	static CPlayer* GetPlayer(void) { return pPlayer; }
	static CEnemy* GetEnemy(void) { return pEnemy; }
	static CMeshField* GetMeshField(void) { return pMeshField; }
	static CMeshRing* GetMeshRing(void) { return m_pMeshRing; }
	static CMeshCylinder* GetMeshCylinder(void) { return m_pMeshCylinder; }
	static CShadow* GetShadow(void) { return pShadow; }
	static CFieldMulti* GETFieldMulti(void) { return m_pFieldMulti; }

	//=== objectX ===//
	static CBlock* GetBlock(void) { return m_pBlock; }
	static CItem* GetItem(void) { return m_pItem; }
	static CModelParticle* GetModelParticle(void) { return m_pModelParticle; }

	//=== objectBillboard ===//
	static CEffect* GetEffect(void) { return m_pEffect; }

private:
	//=== 2Dobject ===//
	static CBg* m_pBg;
	static CNumber* m_pNumber;
	static CScore* m_pScore;
	static CTimer* m_pTimer;
	static CStock* m_pStock;

	//=== 3Dobject ===//
	static CField* pField;
	static CPlayer* pPlayer;
	static CEnemy* pEnemy;
	static CMeshField* pMeshField;
	static CMeshRing* m_pMeshRing;
	static CMeshCylinder* m_pMeshCylinder;
	static CShadow* pShadow;
	static CFieldMulti* m_pFieldMulti;

	//=== objectX ===//
	static CBlock* m_pBlock;
	static CItem* m_pItem;
	static CModelParticle* m_pModelParticle;

	//=== objectBillboard ===//
	static CEffect* m_pEffect;

	static bool m_bPause;		// ポーズ
	static int SetObjectNum;	// オブジェクト設置の種類
	static int m_nCreateCnt;	// 生成カウント
};

#endif // !_GAME_H_
