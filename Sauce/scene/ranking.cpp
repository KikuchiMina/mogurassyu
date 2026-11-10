////=========================================================
//// 
//// "ランキング画面処理"	[ranking.h]
//// Author : KikuchiMina
//// 
////=========================================================
//#include "ranking.h"
//#include "manager.h"
//
////=========================================================
////マクロ定義
////=========================================================
//
////=========================================================
//// 静的メンバ変数宣言
////=========================================================
////=== 2Dobject ===//
//CNumber* CRanking::m_pNumber = NULL;
//CScore* CRanking::m_pScore = NULL;
//
////=========================================================
//// コンストラクタ
////=========================================================
//CRanking::CRanking() : CScene(CScene::MODE_RANKING)
//{
//	m_nRankingData[MAX_RANKING] = {};	// 10位までのランキング
//	m_nNewScore = 110;					// ランキングに追加するスコア
//	m_nGetRank = 0;						// 受け取った順位
//}
//
////=========================================================
//// デストラクタ
////=========================================================
//CRanking::~CRanking()
//{
//}
//
////=========================================================
//// 生成処理
////=========================================================
//void CRanking::Create(void)
//{
//	//-------------------------------------
//	// スコアの生成
//	m_apScore[0] = CScore::Create(D3DXVECTOR3(450.0f, 200.0f, 0.0f), 40.0f, 60.0f);	// 8桁 2対3
//	m_apScore[1] = CScore::Create(D3DXVECTOR3(450.0f, 300.0f, 0.0f), 40.0f, 60.0f);
//	m_apScore[2] = CScore::Create(D3DXVECTOR3(450.0f, 400.0f, 0.0f), 40.0f, 60.0f);
//	m_apScore[3] = CScore::Create(D3DXVECTOR3(450.0f, 500.0f, 0.0f), 40.0f, 60.0f);
//	m_apScore[4] = CScore::Create(D3DXVECTOR3(450.0f, 600.0f, 0.0f), 40.0f, 60.0f);
//
//	// スコアの設定
//	for (int nCount = 0; nCount < OPEN_RANKING; nCount++)
//	{
//		m_apScore[nCount]->SetScore(m_nRankingData[nCount]);
//	}
//}
//
////=========================================================
//// テクスチャの読み込み処理
////=========================================================
//HRESULT CRanking::Load(void)
//{
//	// ナンバーテクスチャの読み込み
//	m_pNumber->Load();
//
//	return S_OK;
//}
//
////=========================================================
//// テクスチャの破棄処理
////=========================================================
//void CRanking::Unload(void)
//{
//	// ナンバーテクスチャの破棄
//	m_pNumber->Unload();
//}
//
////=========================================================
//// 初期化処理
////=========================================================
//HRESULT CRanking::Init(void)
//{
//	// スコアを取得
//	//m_nNewScore = CGame::
//
//	// ネットワーク通信の開始
//	NetworkRankingu();
//
//	// テクスチャの読み込み
//	HRESULT hresult = Load();
//	if (hresult == S_FALSE)
//	{
//		return S_FALSE;
//	}
//
//	// 生成処理
//	Create();
//
//	return S_OK;
//}
//
////=========================================================
//// 終了処理
////=========================================================
//void CRanking::Uninit(void)
//{
//	// テクスチャの破棄
//	Unload();
//}
//
////=========================================================
//// 更新処理
////=========================================================
//void CRanking::Update(void)
//{
//	CInputKeyboard* pInputKeyboard = CManager::GetKeyboard();
//	CRenderer* pRenderer = CManager::GetRenderer();
//
//	//-------------------------------------
//	// タイトルへ戻る
//	if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
//	{
//		// ゲームシーンの再読み込み
//		CFade::SetFade(CScene::MODE_TITLE);
//	}
//}
//
////=========================================================
//// 描画処理
////=========================================================
//void CRanking::Draw(void)
//{
//
//}
//
////=========================================================
//// ネットワーク通信
////=========================================================
//void CRanking::NetworkRankingu(void)
//{
//	HWND hWnd = CManager::GetWnd();
//
//	// メッセージ
//	int nID = IDYES;
//	//int nID = MessageBox(hWnd, "ネットワーク通信を開始しますか？", "ネットワークメッセージ", MB_YESNO);
//	if (nID == IDYES)
//	{
//		CTcpClient* pTcpClient = NULL;		// クライアントクラスの呼び出し
//		bool bLink = false;			// サーバーに接続できたかどうか
//
//		// これが無いとSETだけ通信されてGETの通信が出来ない
//		for (int nCount = COMMAND_TYPE_SET_RANKING; nCount < COMMAND_TYPE_MAX; nCount++)
//		{
//			//*********************************
//			// Winsockの初期化関数を実行
//			WSADATA wsaData;
//			int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);
//
//			// メッセージ
//			if (nErr != 0)
//			{ // 初期化に失敗した場合
//				nID = MessageBox(hWnd, "Winsockの初期化に失敗しました", "ネットワークメッセージ", MB_OK);
//			}
//
//			//*********************************
//			// クライアントの生成
//			if (pTcpClient == NULL)
//			{
//				pTcpClient = new CTcpClient;
//
//				if (pTcpClient != NULL)
//				{
//					//*********************************
//					// 接続先の設定
//					//char aData[20] = { "52.231.66.252" };	// 自分
//					char aData[20] = { "127.0.0.1" };
//					//nID = MessageBox(hWnd, "接続先のIPアドレスは[ 127.0.0.1 ]です", "ネットワークメッセージ", MB_OK);
//
//					// 接続処理
//					bLink = pTcpClient->Init(&aData[0], 22333);
//					if (bLink == true)
//					{
//						//nID = MessageBox(hWnd, "通信用のソケットを作成に成功しました", "ネットワークメッセージ", MB_OK);
//
//						//*********************************
//						// ランキング登録
//						if (nCount == COMMAND_TYPE_SET_RANKING)
//						{
//							SetRanking(pTcpClient);
//						}
//
//						//*********************************
//						// ランキング取得
//						if (nCount == COMMAND_TYPE_GET_RANKING)
//						{
//							GetRanking(pTcpClient);
//						}
//					}
//					else
//					{
//						nID = MessageBox(hWnd, "サーバーの接続に失敗しました", "ネットワークメッセージ", MB_OK);
//						break;
//					}
//
//					// クライアントの終了処理 + 破棄
//					pTcpClient->Uninit();
//					delete pTcpClient;
//					pTcpClient = NULL;
//				}
//			}
//
//			//*********************************
//			// Winsock終了処理
//			WSACleanup();
//		}
//	}
//	//nID = MessageBox(hWnd, "ネットワーク通信を終了しました", "ネットワークメッセージ", MB_OK);
//}
//
////=========================================================
//// ランキングを登録
////=========================================================
//void CRanking::SetRanking(CTcpClient* pTcpClient)
//{
//	COMMAND_TYPE type = COMMAND_TYPE_SET_RANKING;	// 種類
//	int nSetData = m_nNewScore;		// ランキングに追加する値
//	char aData[10];					// コピー用変数
//
//	// 1つ目のデータをコピー
//	memcpy(aData, &type, sizeof(type));
//	// 2つ目のデータをその直後にコピー
//	memcpy(aData + sizeof(type), &nSetData, sizeof(nSetData));
//
//	//*********************************
//	// データの送信処理
//	pTcpClient->Send(1, &aData);
//
//	//*********************************
//	// データ受信処理
//	int nRnaking = 0;					// ランキング順位受け取り用
//	pTcpClient->Recv(1, &nRnaking);		// ランキング順位受け取り
//	m_nGetRank = nRnaking;
//	//printf("\n%dが%dにランクインした!!", nSetData, nRnaking);
//}
//
////=========================================================
//// ランキングを取得
////=========================================================
//void CRanking::GetRanking(CTcpClient* pTcpClient)
//{
//	COMMAND_TYPE type = COMMAND_TYPE_GET_RANKING;	// 種類
//	int nRankingKData[MAX_RANKING] = {};			// ランキングを保存
//
//	//*********************************
//	// データの送信処理
//	pTcpClient->Send(1, &type);
//
//	//*********************************
//	// データ受信処理
//	pTcpClient->Recv(MAX_RANKING, &nRankingKData[0]);
//
//	printf("\n**** ランキング ****\n");
//	for (int nCount = 0; nCount < MAX_RANKING; nCount++)
//	{
//		// 受信後にエンディアン変換(host to network)
//		m_nRankingData[nCount] = ntohl(nRankingKData[nCount]);
//
//		// ランキング表示
//		printf("%d, %d\n", nCount + 1, m_nRankingData[nCount]);
//	}
//}
