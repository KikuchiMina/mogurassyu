//=========================================================
// 
// "ゲーム画面処理"	[game.h]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/scene/game.h"

//=========================================================
// マクロ定義
//=========================================================
#define FILE_SCRIPT_NAME	"data\\Scripts\\Script.txt"		// 開くファイル名
#define GAME_BG_NAME		"data\\TEXTURE\\soilLoop.png"	// ゲーム背景名
#define STAGE_00		// ステージ0の処理
//#undef STAGE_00			// ステージ0の処理を無効にする(有効にしたい時はこの行をコメントアウトする)
#define STAGE_01		// ステージ1の処理
#undef STAGE_01			// ステージ1の処理を無効にする(有効にしたい時はこの行をコメントアウトする)

//=========================================================
// 静的メンバ変数宣言
//=========================================================
//bool CGame::m_bPause = NULL;

//=== 2Dobject ===//
CBg* CGame::m_pBg = NULL;
CNumber* CGame::m_pNumber = NULL;
CScore* CGame::m_pScore = NULL;
CTimer* CGame::m_pTimer = NULL;
CStock* CGame::m_pStock = NULL;

//=== 3Dobject ===//
CField* CGame::pField = NULL;
CPlayer* CGame::pPlayer = NULL;
CEnemy* CGame::pEnemy = NULL;
//CMeshWall* CGame::pMeshWall = NULL;
CMeshField* CGame::pMeshField = NULL;
CMeshRing* CGame::m_pMeshRing = NULL;
CMeshCylinder* CGame::m_pMeshCylinder = NULL;
CShadow* CGame::pShadow = NULL;
CFieldMulti* CGame::m_pFieldMulti = NULL;

//=== objectX ===//
CBlock* CGame::m_pBlock = NULL;
CItem* CGame::m_pItem = NULL;
CModelParticle* CGame::m_pModelParticle = NULL;

//=== objectBillboard ===//
CEffect* CGame::m_pEffect = NULL;

bool CGame::m_bPause = false;		// ゲームスタートの合図
int CGame::SetObjectNum = 0;
int CGame::m_nCreateCnt = 0;

//=========================================================
// コンストラクタ
//=========================================================
CGame::CGame() : CScene(CScene::MODE_GAME)
{
}

//=========================================================
// デストラクタ
//=========================================================
CGame::~CGame()
{
}

//=========================================================
// 生成処理
//=========================================================
void CGame::Create(void)
{
	// スクリプトの読み込み
	//m_pScript->Load();

	LoadFile();

	//m_pBlock->Create(CBlock::BLOCKTYPE_NORMAL);

	//CBg::GameCreate();

	m_pBg = CBg::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CBg::BGTEX_GAME);

	// エフェクトの生成
	D3DXCOLOR col = D3DXCOLOR(0.37f, 0.31f, 0.27f, 1.0f);		// 頂点カラー(r,g,b,a)
	//m_pEffect = CEffect::Create(D3DXVECTOR3(FILED_X / 2, FILED_Y / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, 25.0f, 0.0f, 100, 0);	// pos, move, col, Radius, sud, Life
	//m_pEffect->Create(col);

	// フィールドの生成
	//pMeshField = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f, 100.0f, D3DXVECTOR2(6, 1), 4, 15.0f, 100.0f);
	//pMeshField = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f, D3DXVECTOR2(13, 1));

	// メッシュリングの生成
	//m_pMeshRing = CMeshRing::Create(D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, D3DXVECTOR2(24, 1));

	// メッシュシリンダー
	//m_pMeshCylinder = CMeshCylinder::Create(D3DXVECTOR3(0.0f, 50.0f, 0.0f), 100.0f, 100.0f, 100.0f, D3DXVECTOR2(8, 1));

	// タイマーの生成
	//m_pTimer = CTimer::Create(D3DXVECTOR3(610.0f, 0.0f, 0.0f), 40.0f, 60.0f, 2, 10, CTimer::TIMETYPE_UP);	// pos,w,h,桁数,時間, type
}

//=========================================================
// テクスチャの読み込み処理
//=========================================================
HRESULT CGame::Load(void)
{
	// 背景の読み込み
	m_pBg->Load(GAME_BG_NAME);

	//// フィールドの読み込み
	//pField->Load();

	//// フィールドの読み込み
	//pMeshField->Load();

	//// マルチテクスチャフィールドの読み込み
	//m_pFieldMulti->Load();

	//// メッシュリングの読み込み
	//m_pMeshRing->Load();

	//// メッシュシリンダーの読み込み
	//m_pMeshCylinder->Load();

	//// 影の読み込み
	//pShadow->Load();

	// エフェクトの読み込み
	m_pEffect->Load();

	//// ナンバーテクスチャの読み込み
	//m_pNumber->Load();

	//// ストックの読み込み
	//m_pStock->Load();

	return S_OK;
}

//=========================================================
// テクスチャの破棄処理
//=========================================================
void CGame::Unload(void)
{
	// 背景の破棄
	m_pBg->Unload();

	//// フィールドの破棄
	//pField->Unload();

	//// メッシュフィールドの破棄
	//pMeshField->Uninit();

	//// マルチテクスチャフィールドの破棄
	//m_pFieldMulti->Unload();

	//// メッシュリングの破棄
	//m_pMeshRing->Unload();

	//// メッシュシリンダーの破棄
	//m_pMeshCylinder->Unload();

	//// 影の破棄
	//pShadow->Unload();

	// エフェクトの破棄
	m_pEffect->Unload();

	//// ナンバーテクスチャの破棄
	//m_pNumber->Unload();

	//// ストックの破棄
	//m_pStock->Unload();
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CGame::Init(void)
{
	// テクスチャの読み込み
	HRESULT hresult = Load();
	if (hresult == S_FALSE)
	{
		return S_FALSE;
	}

	// 生成処理
	Create();

#ifdef SOUND
	//サウンドの再生
	CSound* _pSound = CManager::GetSound();
	_pSound->PlaySound(CSound::SOUND_LABEL_BGMOP);
#endif // SOUND

	return S_FALSE;
}

//=========================================================
// 終了処理
//=========================================================
void CGame::Uninit(void)
{
	// テクスチャの破棄
	Unload();

#ifdef SOUND
	//サウンドの停止
	CSound* _pSound = CManager::GetSound();
	_pSound->StopSound();
#endif // SOUND
}

//=========================================================
// 更新処理
//=========================================================
void CGame::Update(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetKeyboard();
	CRenderer* pRenderer = CManager::GetRenderer();
	CLight* pLight = CManager::GetLight();
	CCamera* pCamera = CManager::GetCamera();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	D3DXVECTOR3 PlayerRot = CGame::GetPlayer()->GetRot();

	//-------------------------------------
	// ゲームリトライ
	if (pInputKeyboard->GetKeyboardTrigger(DIK_R) == true)
	{
		// ゲームシーンの再読み込み
		CFade::SetFade(CScene::MODE_GAME);
	}

	// 配置
	if (pInputKeyboard->GetKeyboardTrigger(DIK_LEFT) == true)
	{
		SetObjectNum--;
		if (SetObjectNum < 0)
		{
			SetObjectNum = SETOBJECT_MAX;
		}
	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_RIGHT) == true)
	{
		SetObjectNum++;
		if (SETOBJECT_MAX < SetObjectNum)
		{
			SetObjectNum = SETOBJECT_GRASS;
		}
	}

	// カウントアップ
	m_nCreateCnt++;

	//-------------------------------------
	// デバック
	CDebugProc* pDebugProc = CManager::GetDebugProc();
	//pDebugProc->PrintDebugProc("\n[ 操作 ]\n< 設置 > ENTER\n< 設置オブジェクト変更 >← →\n");
	//switch (SetObjectNum)
	//{
	//case SETOBJECT_GRASS:	// 草
	//	pDebugProc->PrintDebugProc(" 草\n");
	//	break;
	//case SETOBJECT_CUBE:	// キューブ
	//	pDebugProc->PrintDebugProc(" キューブ\n");
	//	break;
	//case SETOBJECT_FIELD:	// フィールド
	//	pDebugProc->PrintDebugProc(" フィールド\n");
	//	break;
	//case SETOBJECT_ENEMY:	// 敵
	//	pDebugProc->PrintDebugProc(" 敵\n");
	//	break;
	//case SETOBJECT_BULLET:	// 弾
	//	pDebugProc->PrintDebugProc(" 弾\n");
	//	break;
	//case SETOBJECT_MAX:		// 敵
	//	pDebugProc->PrintDebugProc(" なし\n");
	//	break;
	//}
}

//=========================================================
// 描画処理
//=========================================================
void CGame::Draw(void)
{
	CRenderer* pRenderer = CManager::GetRenderer();
	CCamera* pCamera = CManager::GetCamera();

	// カメラの更新処理
	pCamera->SetCamera();

	//// レンダラーの描画処理
	//pRenderer->Draw();
}

//=========================================================
// ファイル読み込み
//=========================================================
bool CGame::LoadFile(void)
{
	// === 全項目共通で使用可能な変数 === //
	CDebugProc* pDebugProc = CManager::GetDebugProc();
	FILE* pFile = NULL;
	char Realize[MAX_FILECHAR];			// 判別用の変数
	char* pEqual = NULL;				// =を消去するためのポインタ

	//// === テクスチャ用の変数を用意 === //
	//int nNumTex = 0;					// テクスチャ数
	//int nCntTexFile = 0;				// 読み込んだテクスチャファイル名をカウントする
	//char cTexFileName[MAX_FILECHAR];	// テクスチャファイル名

	// === モデル用の変数を用意 ===	//
	int nNumModel = 0;					// モデル数
	int nCntModelFile = 0;				// 読み込んだXファイル名をカウントする
	char cModelFileName[MAX_MODEL][MAX_FILECHAR];	// Xファイル名

	// === キャラクター情報の変数を用意 === //
	D3DXVECTOR3 CharSetPos = {};// キャラクターの位置
	D3DXVECTOR3 CharSetRot = {};// キャラクターの向き
	int nNumParts = 0;					// パーツ数
	int nCntParts = 0;					// 読み込んだパーツをカウントする
	int nCharIdx[MAX_MODEL] = {};		// キャラクターのインデックス
	int nCharParent[MAX_MODEL] = {};	// キャラクターの親インデックス
	D3DXVECTOR3 CharPos[MAX_MODEL] = {};// キャラクターの位置
	D3DXVECTOR3 CharRot[MAX_MODEL] = {};// キャラクターの向き

	// ファイルオープン
	pFile = fopen(FILE_SCRIPT_NAME, "r");

	if (pFile != NULL)
	{// ファイルを開いた
		// デバック表示
		pDebugProc->PrintDebugProc("****ファイルオープン成功****\n");

		while (1)
		{
			fgets(&Realize[0], sizeof Realize, pFile);	// 最初に比較用文字の読み込み

			if (CScript::JudgeComent(&Realize[0]) == true)
			{// 何かの情報を読み込む合図を取得したとき
				// スクリプト
				if (strcmp(&Realize[0], "SCRIPT") != NULL)
				{
					// デバック表示
					pDebugProc->PrintDebugProc("[ スクリプトの読み込みに成功 ]\n");
				}

				// キャラクター情報
				if (strstr(&Realize[0], "CHARACREATE") != NULL)
				{
					while (1)
					{
						fgets(&Realize[0], sizeof Realize, pFile);	// 最初に比較用文字の読み込み

						if (CScript::JudgeComent(&Realize[0]) == true)
						{// 何かの情報を読み込む合図を取得したとき
							// 位置
							if (strstr(&Realize[0], "POS") != NULL)
							{
								pEqual = strstr(Realize, "=");

								if (pEqual != NULL)
								{
									// アドレスを1こずらす
									pEqual++;

									// 位置を読み込む
									CScript::Recv(&Realize[0]);
									(void)sscanf(pEqual, "%f%f%f", &CharSetPos.x, &CharSetPos.y, &CharSetPos.z);
								}
							}

							// 向き
							if (strstr(&Realize[0], "ROT") != NULL)
							{
								pEqual = strstr(Realize, "=");

								if (pEqual != NULL)
								{
									// アドレスを1こずらす
									pEqual++;

									// 向きを読み込む
									CScript::Recv(&Realize[0]);
									(void)sscanf(pEqual, "%f%f%f", &CharSetRot.x, &CharSetRot.y, &CharSetRot.z);
								}
							}

							// 読み込みの終了
							if (strstr(&Realize[0], "END_CHARACREATE") != NULL)
							{
								pPlayer = CPlayer::Create(CharSetPos, CharSetRot);			// スタート地点
								//pPlayer = CPlayer::Create(D3DXVECTOR3(20.0f, 20.0f, 20.0f), CharSetRot);			// スタート地点
								break;
							}
						}
					}
				}

				// モデル数
				if (strstr(&Realize[0], "NUM_MODEL"))
				{ // モデル数を代入

					printf("\n[ モデル数の読み込みに成功 ]\n");

					// 「=」の場所を見つける
					pEqual = strstr(Realize, "=");

					if (pEqual != NULL)
					{
						// アドレスを1こずらす
						pEqual++;

						// テクスチャの数を読み込む
						(void)sscanf(pEqual, "%d", &nNumModel);
					}

				}

				// モデルファイル名
				if (strstr(&Realize[0], "MODEL_FILENAME") != NULL)
				{ // モデルのファイル名を代入
					while (1)
					{
						if (nCntModelFile < nNumModel)
						{// 上で代入したモデル数よりも、読み込んだファイル数が少ないとき
							// 「=」の場所を見つける
							pEqual = strstr(Realize, "=");

							if (pEqual != NULL)
							{
								// アドレスを1こずらす
								pEqual++;

								// モデルファイル名を読み込む
								(void)sscanf(pEqual, "%s", &cModelFileName[nCntModelFile][0]);

								// モデルファイル名をカウント
								nCntModelFile++;
								break;
							}

						}
					}
				}

				// キャラクター情報
				if (strstr(&Realize[0], "CHARACTERSET"))
				{
					while (1)
					{
						fgets(&Realize[0], sizeof Realize, pFile);	// 最初に比較用文字の読み込み

						if (CScript::JudgeComent(&Realize[0]) == true)
						{ // 何かの情報を読み込む合図を取得したとき
							// パーツ数
							if (strstr(&Realize[0], "NUM_PARTS") != NULL)
							{
								pEqual = strstr(Realize, "=");

								if (pEqual != NULL)
								{
									// アドレスを1こずらす
									pEqual++;

									// パーツ数を読み込む
									(void)sscanf(pEqual, "%d", &nNumParts);
									printf("パーツ数 : %d\n\n", nNumParts);
								}
							}

							// パーツの各設定
							if (strstr(&Realize[0], "PARTSSET") != NULL)
							{ // 読み込み開始
								while (1)
								{
									fgets(&Realize[0], sizeof Realize, pFile);	// 最初に比較用文字の読み込み

									if (CScript::JudgeComent(&Realize[0]) == true)
									{// 何かの情報を読み込む合図を取得したとき
										if (nCntParts < nNumParts)
										{
											// インデックス
											if (strstr(&Realize[0], "INDEX") != NULL)
											{
												pEqual = strstr(Realize, "=");

												if (pEqual != NULL)
												{
													// アドレスを1こずらす
													pEqual++;

													// インデックスを読み込む
													(void)sscanf(pEqual, "%d", &nCharIdx[nCntParts]);
												}
											}

											// 親インデックス
											if (strstr(&Realize[0], "PARENT") != NULL)
											{
												pEqual = strstr(Realize, "=");

												if (pEqual != NULL)
												{
													// アドレスを1こずらす
													pEqual++;

													// 親インデックスを読み込む
													(void)sscanf(pEqual, "%d", &nCharParent[nCntParts]);
												}
											}

											// 位置
											if (strstr(&Realize[0], "POS") != NULL)
											{
												pEqual = strstr(Realize, "=");

												if (pEqual != NULL)
												{
													// アドレスを1こずらす
													pEqual++;

													// 位置を読み込む
													CScript::Recv(&Realize[0]);
													(void)sscanf(pEqual, "%f%f%f", &CharPos[nCntParts].x, &CharPos[nCntParts].y, &CharPos[nCntParts].z);
												}
											}

											// 向き
											if (strstr(&Realize[0], "ROT") != NULL)
											{
												pEqual = strstr(Realize, "=");

												if (pEqual != NULL)
												{
													// アドレスを1こずらす
													pEqual++;

													// 向きを読み込む
													CScript::Recv(&Realize[0]);
													(void)sscanf(pEqual, "%f%f%f", &CharRot[nCntParts].x, &CharRot[nCntParts].y, &CharRot[nCntParts].z);
												}
											}

											// 読み込みの終了
											if (strstr(&Realize[0], "END_PARTSSET") != NULL)
											{
												nCntParts++;	// カウントアップ
												break;
											}
										}
									}
								}
							}

							// キャラクター情報の読み込みの終了
							if (strstr(&Realize[0], "END_CHARACTERSET") != NULL)
							{
								for (int nCount = 0; nCount < nNumParts; nCount++)
								{
									pPlayer->ModelFileName(cModelFileName[nCharIdx[nCount]], CharPos[nCharIdx[nCount]], CharRot[nCharIdx[nCount]]);			// スタート地点
								}
								break;
							}
						}
					}
				}
			}

			// スクリプト読み込みを終了する
			if (strstr(&Realize[0], "END_SCRIPT") != NULL)
			{
				// デバック表示
				CDebugProc* pDebugProc = CManager::GetDebugProc();
				pDebugProc->PrintDebugProc("[ スクリプト読み込みを終了します ]\n");
				break;
			}
		}
		// ファイルを閉じる
		fclose(pFile);

		// デバック表示
		pDebugProc->PrintDebugProc("[ ファイルを閉じました ]\n");

		//// モデルの総数の設定
		//m_nNumModel = nNumParts;

		//// モデルの設定
		//for (int nCount = 0; nCount < nNumParts; nCount++)
		//{
		//	m_apModel[nCharIdx[nCount]] = CModel::Create(cModelFileName[nCharIdx[nCount]], CharPos[nCharIdx[nCount]], CharRot[nCharIdx[nCount]]);
		//}

		//// 親モデルの設定
		//for (int nCount = 1; nCount < nNumParts; nCount++)	// 親はNULLにしたいため、1からfor文開始
		//{
		//	m_apModel[nCharIdx[nCount]]->SetParent(m_apModel[nCharParent[nCount]]);
		//}

		return true;
	}
	else
	{ // ファイルオープン失敗時
		// デバック表示
		pDebugProc->PrintDebugProc("****ファイルオープン失敗****\n");
		return false;
	}

	return false;
}
