////=========================================================
//// 
//// "ランキング画面処理"	[ranking.h]
//// Author : KikuchiMina
//// 
////=========================================================
//#ifndef _RANKING_H_			//このマクロ定義がされてなかったら
//#define _RANKING_H_			//2重インクルード防止のマクロ定義
//
//#include "../object/score.h"
//#include "../object/number.h"
//#include "scene.h"
//#include "../system/main.h"
//#include "../system/tcp_client.h"
//#include "../system/protocol_ranking.h"
//
//// マクロ定義
//#define OPEN_RANKING		5	// ランキングの表示数
//
//// タイトルクラス
//class CRanking : public CScene
//{
//public:
//	// 背景の種類
//	typedef enum
//	{
//		RANKING_BG_000 = 0,	// 背景
//		RANKING_MAX
//	} RANKING;
//
//	CRanking();
//	~CRanking();
//	void Create(void);
//	HRESULT Load(void);
//	void Unload(void);
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//
//	//=== network ===//
//	void NetworkRankingu(void);
//	void SetRanking(CTcpClient* pTcpClient);
//	void GetRanking(CTcpClient* pTcpClient);
//
//	//=== 2Dobject ===//
//	static CNumber* GetNumber(void) { return m_pNumber; }
//	static CScore* GetScore(void) { return m_pScore; }
//
//private:
//	//=== 2Dobject ===//
//	static CNumber* m_pNumber;
//	static CScore* m_pScore;
//
//	CScore* m_apScore[OPEN_RANKING];	// ランキングへのポインタ
//	int m_nRankingData[MAX_RANKING];	// 受け取った10位までのランキング
//	int m_nNewScore;					// ランキングに追加するスコア
//	int m_nGetRank;						// 受け取った順位
//};
//
//#endif // !_RANKING_H_