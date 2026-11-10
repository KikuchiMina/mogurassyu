//=========================================================
// 
// "プレイヤーの処理"	[player.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _PLAYER_H_		//このマクロ定義がされてなかったら
#define _PLAYER_H_		//2重インクルード防止のマクロ定義

// インクルードヘッダファイル
#include "object.h"
#include "model.h"
#include "shadow.h"
#include "../system/main.h"
#include "../system/script.h"

#define MAX_NAME		125		// 最大文字
#define MAX_BULLET		128		// 弾の最大数
#define FIELD_CREATE	300.0f	// フィールドを生成する範囲
#define FIELDTYPE_NUM	6		// type00
#define FIELDTYPE_NEXT	8		// type01

// プレイヤークラス
class CPlayer : public CObject
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

	//プレイヤーの状態
	typedef enum
	{
		STATE_APPEAR = 0,	// 出現状態(点滅)
		STATE_NORMAL,		// 通常状態
		STATE_WAIT,			// 出現待ち状態
		STATE_BOSS,			// ボス戦状態
		STATE_DEATE,		// 死亡状態
		STATE_STAGECLEAR,	// ステージクリア状態
		STATE_CLEAR,		// クリア状態
		STATE_MAX
	}STATE;

	CPlayer(PRIORITY nPriority = PRIORITY_PLAYER);
	~CPlayer();
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Move(void);
	void ValueRot(void);
	void TimeUp(void);
	void FieldLength(void);
	void CreateScore(float fDistance, int type);
	bool Load(void);
	void Debug(void);
	float AngleNormalize(float fRot);
	void ModelFileName(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	bool GetMoving(void) { return m_moveing; }
	float GetValueRot(void) { return m_fValueRot; }
	D3DXVECTOR3 GetDiameter(void) { return m_scaledSize; }

private:
	D3DXVECTOR3	m_pos;				// 位置
	D3DXVECTOR3	m_move;				// 移動量
	D3DXVECTOR3	m_rot, m_rotDest;	// 向き
	D3DXVECTOR3 m_scl;				// スケール
	D3DXVECTOR3 m_scaledSize;		// モデルの半径サイズ
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	STATE m_state;					// プレイヤーの状態
	int m_nCntEffect;				// エフェクト生成間隔
	int m_nCntInput;				// 入力時間
	int m_nBrake;					// ブレーキ
	int m_nSetStock;				// ストックのカウント
	float m_fRotMove;				// 向いている角度
	bool m_moveing;					// 動いているかどうか
	bool m_bInput;					// 入力をしてもいいかどうか
	bool m_bJump;					// ジャンプしたかどうか
	bool m_bDash;					// ダッシュしたかどうか
	bool m_bLanding;				// 着地してるかどうか
	bool m_bLookBack;				// 切り替えしをするかどうか

	//=== クォータニオンに関する変数 ===//
	D3DXMATRIX m_mtxRot;			// 回転マトリックス(保存用)
	D3DXQUATERNION m_quat;			// クォータニオン
	D3DXVECTOR3	m_vecAxis;			// 回転軸
	D3DXVECTOR3	m_vecAxisKeep;		// 回転軸(保存用)
	float m_fValueRot;				// 回転角(回転量)
	float m_fRotSpeed;				// 回転速度
	float m_fInertia;				// 回転減衰

	//=== オブジェクトへのポインタ ===//
	CShadow* m_pShadow;				// 影へのポインタ
	CModel* m_apModel[MAX_MODEL];	// モデル(パーツ)へのポインタ
	static int m_nNumModel;			// モデルの総数
};

#endif // !_CUBE_H_