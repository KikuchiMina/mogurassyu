//=========================================================
// 
// "プレイヤーの処理"	[player.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _ENEMY_H_						// このマクロ定義がされてなかったら
#define _ENEMY_H_						// 2重インクルード防止のマクロ定義

// インクルードファイル
#include "../system/main.h"
#include "object.h"
#include "model.h"

// オブジェクトクラス定義
class CEnemy : public CObject
{
public:
	// モデルの種類
	typedef enum
	{
		MODELTYPE_BODY = 0,		// 胴体
		MODELTYPE_HEAD,			// 頭
		MODELTYPE_ARMR,			// 腕R
		MODELTYPE_ARML,			// 腕L
		MODELTYPE_LEGR,			// 足R
		MODELTYPE_LEGL,			// 足L
		MODELTYPE_SWORD,		// 剣
		MODELTYPE_MAX
	}MODELTYPE;

	CEnemy(PRIORITY nPriority = PRIORITY_ENEMY);
	~CEnemy();
	static CEnemy* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Load(void);
	int GetNumAll(void) { return m_nNumAll; };
	D3DXVECTOR3 GetPos(void) { return m_pos; }

private:
	D3DXVECTOR3	m_pos, m_posOld;	// 位置
	D3DXVECTOR3	m_move;				// 移動量
	D3DXVECTOR3	m_rot, m_rotDest;	// 向き
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	static int m_nldxShadow;		// 対象の影のインデックス(番号)
	static int m_nNumAll;			// 敵の総数
	int m_nLife;					// 消えるまでのカウント

	//=== オブジェクトへのポインタ ===//
	CModel* m_apModel[MAX_MODEL];	// モデル(パーツ)へのポインタ
	static int m_nNumModel;			// モデルの総数
};

#endif // !_ENEMY_H_
