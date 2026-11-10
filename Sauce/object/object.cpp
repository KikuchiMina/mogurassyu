//=========================================================
// 
// "オブジェクトの処理"	[object.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/object.h"
#include "../../Header/system/manager.h"

//=========================================================
// マクロ定義
//=========================================================
#define LIST_STRUCT
#undef LIST_STRUCT			// リスト構造の処理を無効にする(有効にしたい時はこの行をコメントアウトする)

//=========================================================
// 静的メンバ変数宣言
//=========================================================
CObject* CObject::m_apObject[PRIORITY_MAX][MAX_OBJECT] = { NULL };	// 
int CObject::m_nNumAll = 0;											// オブジェクト総数
CObject* CObject::m_apTop[PRIORITY_MAX] = { NULL };		// 先頭オブジェクトへのポインタ
CObject* CObject::m_apCur[PRIORITY_MAX] = { NULL };		// 最後尾オブジェクトへのポインタ

//=========================================================
// コンストラクタ
//=========================================================
CObject::CObject(PRIORITY nPriority)
{
	m_nPriority = nPriority;	// 優先順位

#if LIST_STRUCT
	// リスト構造
	this->m_pPrev = m_apCur[m_nPriority];	// 最後尾の更新
	if (this->m_pPrev != NULL)
	{
		this->m_pPrev->m_pNext = this;
	}
	m_apCur[m_nPriority] = this;

	// 先頭がいない時
	if (m_apTop[m_nPriority] == NULL)
	{
		m_apTop[m_nPriority] = this;
	}

	// 死亡フラグを無くす
	this->m_bDeath = false;

	m_nNumAll++;	// 総数をカウント
#else
	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (m_apObject[m_nPriority][nCount] == NULL)
		{
			m_apObject[nPriority][nCount] = this;
			m_nID = nCount;				// 自分自身のID
			m_type = TYPE_NONE;			// 種類
			m_nNumAll++;				// オブジェクトの総数カウントアップ
			break;
		}
	}
#endif // LIST_STRUCT

	// オブジェクト
	/*for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (m_apObject[m_nPriority][nCount] == NULL)
		{
			m_apObject[m_nPriority][nCount] = this;
			m_nID = nCount;				// 自分自身のID
			m_type = TYPE_NONE;			// 種類
			m_bDeath = false;
			m_nNumAll++;				// オブジェクトの総数カウントアップ
			break;
		}
	}

	// 先頭オブジェクトの代入
	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (m_apObject[m_nPriority][nCount] != NULL)
		{
			m_apTop[m_nPriority] = m_apObject[m_nPriority][nCount];
			break;
		}
	}

	// 最後尾オブジェクトの代入
	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (m_apObject[m_nPriority][nCount] != NULL)
		{
			m_apCur[m_nPriority] = m_apObject[m_nPriority][nCount];
		}
	}

	// 次のオブジェクトの代入
	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (m_apObject[nPriority][nCount] != NULL)
		{
			for (int nNext = nCount + 1; nNext < MAX_OBJECT; nNext++)	// nNextはnCount + 1から始める
			{
				if (m_apObject[nPriority][nNext] != NULL)
				{ // 次のオブジェクトを保存
					m_apObject[nPriority][nCount]->m_pNext = m_apObject[nPriority][nNext];
					break;
				}
				else
				{
					m_apObject[nPriority][nCount]->m_pNext = NULL;
				}
			}
		}
	}

	// 前のオブジェクトの代入
	CObject* pObjectPrev = NULL;
	m_apObject[m_nPriority][0]->m_pPrev = NULL;	// 0より前はない
	for (int nCount = MAX_OBJECT - 1; 0 < nCount; nCount--)
	{
		if (m_apObject[nPriority][nCount] != NULL)
		{
			for (int nPrev = nCount - 1; -1 < nPrev; nPrev--)	// nPrevはnCountの続きから始める
			{
				if (m_apObject[nPriority][nPrev] != NULL)
				{
					m_apObject[nPriority][nCount]->m_pPrev = m_apObject[nPriority][nPrev];
				}
				else
				{
					m_apObject[nPriority][nCount]->m_pPrev = NULL;
				}
			}
		}
	}*/
}

//=========================================================
// デストラクタ
//=========================================================
CObject::~CObject()
{
}

//=========================================================
// ポリゴンの初期化処理
//=========================================================
HRESULT CObject::Init(void)
{
	return 0;
}

//=========================================================
// ポリゴンの終了処理
//=========================================================
void CObject::Uninit(void)
{

}

//=========================================================
// ポリゴンの更新処理
//=========================================================
void CObject::Update(void)
{

}

//=========================================================
// ポリゴンの描画処理
//=========================================================
void CObject::Draw(void)
{
	
}

//=========================================================
// 自分自身のポリゴンの破棄
//=========================================================
void CObject::Release(void)
{
#if LIST_STRUCT
	m_bDeath = true;
#else
	if (m_apObject[m_nPriority][m_nID] != NULL)
	{
		int nPriority = m_nPriority;
		int nID = m_nID;

		delete m_apObject[nPriority][nID];
		m_apObject[nPriority][nID] = NULL;
		m_nNumAll--;
	}
#endif // LIST_STRUCT
}

//=========================================================
// 全てのポリゴンの破棄
//=========================================================
void CObject::ReleaseAll(void)
{
#if LIST_STRUCT
	// リスト構造
	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		CObject* pObject = m_apTop[nCount];

		while (pObject != NULL)
		{
			CObject* pObjectNext = pObject->m_pNext;	// 次のオブジェクトを保存

			if (pObject->m_bDeath == false)
			{
				// 更新処理
				pObject->Release();
				pObject = NULL;
			}

			pObject = pObjectNext;	// 次のオブジェクトを代入
		}
	}

	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		CObject* pObject = m_apTop[nCount];

		while (pObject != NULL)
		{
			CObject* pObjectRrev = pObject->m_pPrev;	// 前のオブジェクトを保存
			CObject* pObjectNext = pObject->m_pNext;	// 次のオブジェクトを保存

			// 死亡フラグ
			if (pObject->m_bDeath == true)
			{
				if (m_apTop[nCount] != pObject && m_apCur[nCount] != pObject)
				{ // その他
					CObject* pObjectRrev = pObject->m_pPrev;	// 前のオブジェクトを保存
					CObject* pObjectNext = pObject->m_pNext;	// 次のオブジェクトを保存

					if (pObjectRrev != NULL)
					{
						pObjectNext->m_pPrev = pObjectRrev;			// 前のオブジェクトの次はオブジェクトの前
						pObjectRrev->m_pNext = pObjectNext;			// 次のオブジェクトの前はオブジェクトの前
					}
				}
				if (m_apTop[nCount] == pObject)
				{ // 先頭の入れ替え
					m_apTop[nCount] = pObject->m_pNext;

					// 次の人の前は存在しなくなる
					if (pObjectNext != NULL)
					{
						pObjectNext->m_pPrev = NULL;
					}
				}
				if (m_apCur[nCount] == pObject)
				{ // 最後尾の入れ替え
					m_apCur[nCount] = pObject->m_pPrev;

					// 前の人の次は存在しなくなる
					if (pObjectRrev != NULL)
					{
						pObjectRrev->m_pNext = NULL;
					}
				}

				delete pObject;
				pObject = NULL;
				m_nNumAll--;
			}

			pObject = pObjectNext;	// 次のオブジェクトを代入
		}
	}
#else
	for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
	{
		for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
		{
			if (m_apObject[nCntPri][nCount] != NULL)
			{
				m_apObject[nCntPri][nCount]->Release();
			}
		}
	}
#endif // LIST_STRUCT
}

//=========================================================
// 全てのポリゴンの更新処理
//=========================================================
void CObject::UpdateAll(void)
{
#if LIST_STRUCT
	int nCounter = 0;

	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		CObject* pObject = m_apTop[nCount];

		while (pObject != NULL)
		{
			CObject* pObjectNext = pObject->m_pNext;	// 次のオブジェクトを保存

			if (pObject->m_bDeath == false)
			{
				// 更新処理
				pObject->Update();
			}

			pObject = pObjectNext;	// 次のオブジェクトを代入
		}

		// カウント
		nCounter++;
	}

	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		CObject* pObject = m_apTop[nCount];

		while (pObject != NULL)
		{
			CObject* pObjectRrev = pObject->m_pPrev;	// 前のオブジェクトを保存
			CObject* pObjectNext = pObject->m_pNext;	// 次のオブジェクトを保存

			// 死亡フラグ
			if (pObject->m_bDeath == true)
			{
				if (m_apTop[nCount] != pObject && m_apCur[nCount] != pObject)
				{ // その他

					pObjectNext->m_pPrev = pObjectRrev;			// 前のオブジェクトの次はオブジェクトの前
					pObjectRrev->m_pNext = pObjectNext;			// 次のオブジェクトの前はオブジェクトの前
				}
				if (m_apTop[nCount] == pObject)
				{ // 先頭の入れ替え
					m_apTop[nCount] = pObject->m_pNext;

					// 次の人の前は存在しなくなる
					if (pObjectNext != NULL)
					{
						pObjectNext->m_pPrev = NULL;
					}
				}
				if (m_apCur[nCount] == pObject)
				{ // 最後尾の入れ替え
					m_apCur[nCount] = pObject->m_pPrev;

					// 前の人の次は存在しなくなる
					if (pObjectRrev != NULL)
					{
						pObjectRrev->m_pNext = NULL;
					}
				}

				delete pObject;
				pObject = NULL;
				m_nNumAll--;
			}

			pObject = pObjectNext;	// 次のオブジェクトを代入
		}
	}
#else
	for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
	{
		for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
		{
			if (m_apObject[nCntPri][nCount] != NULL)
			{
				m_apObject[nCntPri][nCount]->Update();
			}
		}
	}
#endif // LIST_STRUCT
}

//=========================================================
// 全てのポリゴンの描画処理
//=========================================================
void CObject::DrawAll(void)
{
#if LIST_STRUCT
	// リスト構造
	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		CObject* pObject = m_apTop[nCount];

		while (pObject != NULL)
		{
			CObject* pObjectNext = pObject->m_pNext;	// 次のオブジェクトを保存

			if (pObject->m_bDeath == false)
			{
				// 更新処理
				pObject->Draw();
			}

			pObject = pObjectNext;	// 次のオブジェクトを代入
		}
	}
#else
	for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
	{
		for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
		{
			if (m_apObject[nCntPri][nCount] != NULL)
			{
				m_apObject[nCntPri][nCount]->Draw();
			}
		}
	}
#endif // LIST_STRUCT
}

////=========================================================
//// プレイヤー情報を外部ファイルに書き出す 
////=========================================================
//void Save(void)
//{
//	// === 全項目共通で使用可能な変数 === //
//	CDebugProc* pDebugProc = CManager::GetDebugProc();
//	FILE* pFile = NULL;
//
//	//ファイルを開く
//	pFile = fopen(SAVE_FILE, "w");
//
//	if (pFile != NULL)
//	{// ファイルを開いた
//
//		fprintf(pFile, "#==============================================================================\n");
//		fprintf(pFile, "#\n");
//		fprintf(pFile, "# 『%s』スクリプトファイル[%s]\n", SAVE_FILENAME, SAVE_FILE);
//		fprintf(pFile, "# Author : Mina Kikuchi\n");
//		fprintf(pFile, "#\n");
//		fprintf(pFile, "#==============================================================================\n");
//		fprintf(pFile, "SCRIPT\n");
//
//		m_apObject[nCntPri][nCount]
//
//		//ファイルを閉じる
//		fclose(pFile);
//	}
//	else
//	{ // ファイルオープン失敗時
//		// デバック表示
//		pDebugProc->PrintDebugProc("****ファイルオープン失敗****\n");
//	}
//}
