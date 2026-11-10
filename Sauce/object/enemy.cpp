//=========================================================
// 
// "弾の処理"	[bullet.cpp]
// Author : KikuchiMina
// 
//=========================================================
// インクルードファイル
#include "../../Header/object/enemy.h"
#include "../../Header/system/manager.h"

//=========================================================
//マクロ定義
//=========================================================
#define ENEMY_RANGE		150		// 敵とプレイヤーが一定距離になったら反応する値
#define FILE_ENEMY		"data\\MODEL\\enemyCrystal\\motion.txt"	// 外部ファイル入力のファイル名

//=========================================================
// 静的メンバ変数宣言
//=========================================================
int CEnemy::m_nNumModel = 0;
int CEnemy::m_nNumAll = 0;

//=========================================================
// オブジェクトの生成処理
//=========================================================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy* pEnemy = NULL;

	if (pEnemy == NULL)
	{
		// オブジェクトの生成
		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{
			// オブジェクトの初期化処理
			pEnemy->Init(pos, rot);

			m_nNumAll++;

			return pEnemy;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

//=========================================================
// コンストラクタ
//=========================================================
CEnemy::CEnemy(PRIORITY nPriority) : CObject(nPriority)
{
	m_nNumModel = MAX_MODEL;
}

//=========================================================
// デストラクタ
//=========================================================
CEnemy::~CEnemy()
{
	m_nNumAll--;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CEnemy::Init(void)
{
	return S_OK;
}
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;

	// タイプの設定
	SetType(TYPE_ENEMY);

	//-------------------------------------
	// モデルの生成
	m_apModel[0] = CModel::Create("data\\MODEL\\enemyCrystal\\00_kaku0.x", D3DXVECTOR3(0.0f, 20.0, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[1] = CModel::Create("data\\MODEL\\enemyCrystal\\01_kaku.x", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[2] = CModel::Create("data\\MODEL\\enemyCrystal\\02_up.x", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[3] = CModel::Create("data\\MODEL\\enemyCrystal\\03_dwon.x", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[4] = CModel::Create("data\\MODEL\\enemyCrystal\\04_mawari1.x", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[5] = CModel::Create("data\\MODEL\\enemyCrystal\\05_mawari2.x", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_nNumModel = 6;

	// 親モデルの設定
	for (int nCount = 1; nCount < m_nNumModel; nCount++)	// 親はNULLにしたいため、1からfor文開始
	{
		m_apModel[nCount]->SetParent(m_apModel[0]);
	}

	//-------------------------------------
	////影を設定
	//g_nldxShadowPlayer = SetShadow();

	return 0;
}

//=========================================================
// 終了処理
//=========================================================
void CEnemy::Uninit(void)
{
	
}

//=========================================================
// 更新処理
//=========================================================
void CEnemy::Update(void)
{
	CCamera* pCamera = CManager::GetCamera();				// カメラへのポインタ
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ
	CInputMouse* pMouse = CManager::GetMouse();				// マウスへのポインタ
	D3DXVECTOR3 pPlayerPos;

	// プレイヤーのPos受け取り
	for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			// オブジェクトを取得
			CObject* pObj = GetObject(nCntPri, nCntObj);
	
			if (pObj != NULL)
			{
				// 種類を取得
				TYPE type = pObj->GetType();
	
				if (type == TYPE_PLAYER)
				{ // 種類が敵と重なった
					CPlayer* pPlayer = (CPlayer*)pObj;	// オブジェクトのポインタの取得
					pPlayerPos = pPlayer->GetPos();		// オブジェクトのposの取得
				}
			}
		}
	}

	//-------------------------------------
	// デバック
	CDebugProc* pDebugProc = CManager::GetDebugProc();
	int nNum = pDebugProc->GetNum();
	if (nNum == pDebugProc->DEBUG_PLAYER)
	{
		//対角線の長さを算出する
		float LengthX = pPlayerPos.x - m_pos.x;
		float LengthY = pPlayerPos.y - m_pos.y;
		float fLength = sqrtf((LengthX * LengthX) + (LengthY * LengthY) / 2.0f);

		if (0.0f <= fLength && fLength <= ENEMY_RANGE)
		{ // 敵がプレイヤーに反応する距離

		}

		// デバック表示
		CDebugProc* pDebugProc = CManager::GetDebugProc();
		//pDebugProc->PrintDebugProc("\n敵の位置\n[%f  %f  %f]\n", m_pos.x, m_pos.y, m_pos.z);
	}
}

//=========================================================
// 描画処理
//=========================================================
void CEnemy::Draw(void)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();	// デバイスへのポインタの取得
	D3DXMATRIX mtxRot, mtxTrans;						// 計算用マトリックス

	//-------------------------------------
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//-------------------------------------
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//-------------------------------------
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-------------------------------------
	// モデルの描画
	for (int nCount = 0; nCount < m_nNumModel; nCount++)
	{
		m_apModel[nCount]->Draw();
	}
}

//=========================================================
// ファイル処理
//=========================================================
bool CEnemy::Load(void)
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
	int nNumParts = 0;					// パーツ数
	int nCntParts = 0;					// 読み込んだパーツをカウントする
	int nCharIdx[MAX_MODEL] = {};		// キャラクターのインデックス
	int nCharParent[MAX_MODEL] = {};	// キャラクターの親インデックス
	D3DXVECTOR3 CharPos[MAX_MODEL] = {};// キャラクターの位置
	D3DXVECTOR3 CharRot[MAX_MODEL] = {};// キャラクターの向き

	// === モーション情報の変数を用意 === //
	int nCntMotion = 0;					// 読み込んだモーションをカウントする
	int nMotionrLoop = 0;				// モーションのループするかどうか
	int nNumMotionrKey = 0;				// モーションのキーの数
	int nCntMotionKeySet = 0;			// 読み込んだキーセットをカウントする
	int nMotionFrame = 0;				// モーションのフレーム
	int nNumMotionKey = 0;				// 読み込んだキーをカウントする
	D3DXVECTOR3 MotionPos = {};			// キーの位置
	D3DXVECTOR3 MotionRot = {};			// キーの向き

	// ファイルオープン
	pFile = fopen(FILE_ENEMY, "r");

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

				/*
				// テクスチャ数
				if (strstr(&Realize[0], "NUM_TEXTURE") != NULL)
				{// テクスチャの数を代入

					printf("[ テクスチャ数の読み込みに成功 ]\n");

					// 「=」の場所を見つける
					pEqual = strstr(Realize, "=");

					if (pEqual != NULL)
					{
						// アドレスを1こずらす
						pEqual++;

						// テクスチャの数を読み込む
						(void)sscanf(pEqual, "%d", &nNumTex);
						printf("読み込んだテクスチャの数 : %d\n\n", nNumTex);
					}

				}

				// テクスチャファイル名
				if (strstr(&Realize[0], "TEXTURE_FILENAME") != NULL)
				{// テクスチャのファイル名を代入

					if (nCntTexFile == 0)
					{
						printf("[ テクスチャファイル名の読み込みに成功 ]\n");
					}

					while (1)
					{
						if (nCntTexFile < nNumTex)
						{// 上で代入したテクスチャ数よりも、読み込んだファイル数が少ないとき
							// 「=」の場所を見つける
							pEqual = strstr(Realize, "=");

							if (pEqual != NULL)
							{
								// アドレスを1こずらす
								pEqual++;

								// テクスチャの数を読み込む
								(void)sscanf(pEqual, "%s", &cTexFileName[0]);

								// 書き出し
								printf("%d, %s\n", nCntTexFile, &cTexFileName[0]);

								// テクスチャの読み込み
								//m_apModel[nCntXFile]->Lead(g_XFileName[0]);

								// テクスチャのファイル名をカウント
								nCntTexFile++;
								break;
							}

						}
					}
				}
				*/

				// === モデル数 === //
				if (strstr(&Realize[0], "NUM_MODEL"))
				{// モデル数を代入

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

				// === モデルファイル名 === //
				if (strstr(&Realize[0], "MODEL_FILENAME") != NULL)
				{// モデルのファイル名を代入
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

								// 書き出し
								//printf("%d, %s\n", nCntModelFile, &cModelFileName[0]);

								// モデルファイル名をカウント
								nCntModelFile++;
								break;
							}

						}
					}
				}

				// === キャラクター情報 === //
				if (strstr(&Realize[0], "CHARACTERSET"))
				{
					while (1)
					{
						fgets(&Realize[0], sizeof Realize, pFile);	// 最初に比較用文字の読み込み

						if (CScript::JudgeComent(&Realize[0]) == true)
						{// 何かの情報を読み込む合図を取得したとき

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

								printf("[ %d / %d ]\n", nCntParts, nNumParts);

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
											{ // 読み込み終了
												//// モデルの設定
												//for (int nCount = 0; nCount < nNumParts; nCount++)
												//{
												//	m_apModel[nCharIdx[nCount]] = CModel::Create(cModelFileName[nCharIdx[nCount]], CharPos[nCharIdx[nCount]], CharRot[nCharIdx[nCount]]);
												//}
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
								//// 親モデルの設定
								//for (int nCount = 1; nCount < nNumParts; nCount++)	// 親はNULLにしたいため、1からfor文開始
								//{
								//	m_apModel[nCharIdx[nCount]]->SetParent(m_apModel[nCharParent[nCount]]);
								//}
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

		// モデルの総数の設定
		m_nNumModel = nNumParts;

		// モデルの設定
		for (int nCount = 0; nCount < nNumParts; nCount++)
		{
			m_apModel[nCharIdx[nCount]] = CModel::Create(cModelFileName[nCharIdx[nCount]], CharPos[nCharIdx[nCount]], CharRot[nCharIdx[nCount]]);
		}

		// 親モデルの設定
		for (int nCount = 1; nCount < nNumParts; nCount++)	// 親はNULLにしたいため、1からfor文開始
		{
			m_apModel[nCharIdx[nCount]]->SetParent(m_apModel[nCharParent[nCount]]);
		}

		return true;
	}
	else
	{ // ファイルオープン失敗時
		// デバック表示
		pDebugProc->PrintDebugProc("****ファイルオープン失敗****\n");
		return false;
	}
}
