//=========================================================
// 
// "プレイヤーの処理"	[player.cpp]
// Author : KikuchiMina
// 
//=========================================================
// インクルードヘッダファイル
#include "../../Header/system/script.h"
#include "../../Header/system/manager.h"

//=========================================================
// マクロ定義
//=========================================================
#define MODEL_TXT	"data\\Scripts\\motion.txt"	// 外部ファイル

//=========================================================
// 静的メンバ変数宣言
//=========================================================
FILE* CScript::m_pFile = NULL;	// ファイルの読み込み
CModel* CScript::m_apModel[MAX_MODEL] = { NULL };

//=========================================================
// コンストラクタ
//=========================================================
CScript::CScript()
{
	m_pFile = NULL;
	m_apModel[MAX_MODEL] = { NULL };
}

//=========================================================
// デストラクタ
//=========================================================
CScript::~CScript()
{
}

//=========================================================
// ファイル名の読み込み
//=========================================================
HRESULT CScript::FileLoad(void)
{
	m_pFile = fopen(MODEL_TXT, "r");	//ファイルの読み込み・開く

	return S_OK;
}

//=========================================================
// スクリプト読み込み
//=========================================================
bool CScript::Load(void)
{
	// === 全項目共通で使用可能な変数 === //
	CDebugProc* pDebugProc = CManager::GetDebugProc();
	FILE* pFile = NULL;
	char Realize[MAX_FILECHAR];			// 判別用の変数
	char* pEqual = NULL;				// =を消去するためのポインタ

	// === テクスチャ用の変数を用意 === //
	int nNumTex = 0;					// テクスチャ数
	int nCntTexFile = 0;				// 読み込んだテクスチャファイル名をカウントする
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

	// ファイルオープン
	pFile = fopen(MODEL_TXT, "r");

	if (pFile != NULL)
	{// ファイルを開いた
		// デバック表示
		pDebugProc->PrintDebugProc("****ファイルオープン成功****\n");

		while (1)
		{
			fgets(&Realize[0], sizeof Realize, pFile);	// 最初に比較用文字の読み込み

			if (JudgeComent(&Realize[0]) == true)
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

				// モデル数
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
						printf("読み込んだモデル数 : %d\n\n", nNumModel);
					}

				}

				// モデルファイル名
				if (strstr(&Realize[0], "MODEL_FILENAME") != NULL)
				{// モデルのファイル名を代入

					if (nCntModelFile == 0)
					{
						printf("[ モデルファイル名の読み込みに成功 ]\n");
					}

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

								// モデルファイル名の読み込み
								//m_apModel[nCntModelFile]->Lead(&cModelFileName[nCntModelFile][0]);

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
					if (nCntParts == 0)
					{
						printf("\n[ キャラクター情報の読み込みに成功 ]\n");
					}

					while (1)
					{
						fgets(&Realize[0], sizeof Realize, pFile);	// 最初に比較用文字の読み込み

						if (JudgeComent(&Realize[0]) == true)
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

									if (JudgeComent(&Realize[0]) == true)
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
													Recv(&Realize[0]);
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
													Recv(&Realize[0]);
													(void)sscanf(pEqual, "%f%f%f", &CharRot[nCntParts].x, &CharRot[nCntParts].y, &CharRot[nCntParts].z);
												}
											}

											// 読み込みの終了
											if (strstr(&Realize[0], "END_PARTSSET") != NULL)
											{ // 読み込み終了
												//m_apModel[nCharIdx[nCntParts]]->Create(&cModelFileName[nCntParts][0], CharPos[nCntParts], CharRot[nCntParts]);

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
									m_apModel[nCharIdx[nCount]] = CModel::Create(&cModelFileName[nCharIdx[nCount]][0], CharPos[nCharIdx[nCount]], CharRot[nCharIdx[nCount]]);
								}

								// 親モデルの設定
								for (int nCount = 1; nCount < nNumParts; nCount++)	// 親はNULLにしたいため、1からfor文開始
								{
									m_apModel[nCharIdx[nCount]]->SetParent(m_apModel[nCharIdx[nCount]]);
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

		//// 親モデルの設定
		//for (int nCount = 1; nCount < nNumParts; nCount++)	// 親はNULLにしたいため、1からfor文開始
		//{
		//	m_apModel[nCount]->SetParent(m_apModel[nCharIdx[nCount]]);
		//}

		return true;
	}
	else
	{ // ファイルオープン失敗時
		// デバック表示
		CDebugProc* pDebugProc = CManager::GetDebugProc();
		pDebugProc->PrintDebugProc("****ファイルオープン失敗****\n");
		return false;
	}
}

// =================================================
// スクリプト読み込みでの特定の文字消去処理
// =================================================
void CScript::Recv(char* data)
{
	char aStrAll[MAX_FILECHAR] = {};	// 文字列の連結を入れる
	char* cc = NULL;					// 個々の文字の読み取り
	char aChar[MAX_FILECHAR] = {};		// 個々の文字を入れる値
	
	for (char* c = (char*)data; *c != '\0'; c++)
	{ // 文字列を1文字ずつ読み込む
		switch (*c)
		{
		case 'f':
			c++;
			sprintf(&aChar[0], " ");
			//sprintf(&aChar[0], " ", c);
			break;
		default:
			sprintf(&aChar[0], "%c", *c);
			break;
		}

		// 文字列の連結
		strcat(aStrAll, aChar);
	}

	// 文字列の代入
	strcpy((char*)data, aStrAll);
}

// =================================================
// スクリプト読み込みでのコメントアウトの処理
// =================================================
bool CScript::JudgeComent(char* pStr)
{
	// アドレス取得用のchar型のポインタを用意
	char* pTrash = {};

	if (strncmp(pStr, "#", 1) != 0 && strncmp(pStr, "\n", 1) != 0)
	{ // 文頭が、コメントアウト、改行でなかった場合
		// #があるかを判別
		pTrash = strstr(pStr, "#");

		if (pTrash != NULL)
		{ // #が存在する
			// それ以降の文字列を消去
			strcpy(pTrash, "");
		}

		while (1)
		{ // tabが存在する場合回し続ける
			// tabのアドレスを取得
			pTrash = strstr(pStr, "\t");

			if (pTrash != NULL)
			{ // tabが存在する

				if (pTrash + 1 == NULL)
				{ // 文末にあった時
					// 後ろを空白にする
					strcpy(pTrash + 1, "");
				}
				else
				{ // 文頭にあった時
					// tab以降の文字列をtabがあったところに代入
					strcpy(pTrash, pTrash + 1);
				}
			}
			else if (pTrash == NULL)
			{ // tabが存在しなくなったとき
				break;
			}
		}

		return true;
	}
	else
	{ // コメントアウト、改行が文頭だった場合
		return false;
	}
}
