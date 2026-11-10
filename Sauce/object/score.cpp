//=========================================================
// 
// "弾の処理"	[bullet.cpp]
// Author : KikuchiMina
// 
//=========================================================
// インクルードファイル
#include "../../Header/object/score.h"
#include "../../Header/object/number.h"
#include "../../Header/system/manager.h"

//=========================================================
//マクロ定義
//=========================================================
#define NUM_WIDTH	60.0f

//=========================================================
// 静的メンバ変数宣言
//=========================================================
CNumber* CScore::m_apNumber[NUM_PLACE] = { NULL };
int CScore::m_nScore = 0;
int CScore::m_nNumScore = 0;

//=========================================================
// 敵オブジェクトの生成処理
//=========================================================
CScore* CScore::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CScore* pScore = NULL;

	if (pScore == NULL)
	{
		// オブジェクトの生成
		pScore = new CScore;

		if (pScore != NULL)
		{
			// オブジェクトの初期化処理
			pScore->Init(pos, fWidth, fHeight);

			return pScore;
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
CScore::CScore(PRIORITY nPriority) :CObject(nPriority)
{
	m_nScore = 0;
	m_nNumScore = NUM_PLACE;
}

//=========================================================
// デストラクタ
//=========================================================
CScore::~CScore()
{
}

//=========================================================
// 敵の初期化処理
//=========================================================
HRESULT CScore::Init(void)
{
	return S_OK;
}
HRESULT CScore::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	m_nNumScore = NUM_PLACE;

	for (int nCntScore = 0; nCntScore < m_nNumScore; nCntScore++)
	{
		float fPosX = pos.x;
		fPosX += fWidth * nCntScore;

		// スコアの生成
		m_apNumber[nCntScore] = m_apNumber[nCntScore]->Create(D3DXVECTOR3(fPosX, pos.y, pos.z), fWidth, fHeight);
	}

	return S_OK;
}

//=========================================================
// 敵の終了処理
//=========================================================
void CScore::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < m_nNumScore; nCntScore++)
	{
		m_apNumber[nCntScore]->Uninit();
	}
}

//=========================================================
// 敵の更新処理
//=========================================================
void CScore::Update(void)
{
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ
	int nAddCnt = 0;

	for (int nCntScore = 0; nCntScore < m_nNumScore; nCntScore++)
	{
		m_apNumber[nCntScore]->Update();
	}

	if (pKeyBoaed->GetKeyboardPress(DIK_0) == true)
	{
		nAddCnt++;
	}
	if (pKeyBoaed->GetKeyboardPress(DIK_9) == true)
	{
		nAddCnt += 10;
	}
	if (pKeyBoaed->GetKeyboardPress(DIK_8) == true)
	{
		nAddCnt += 100;
	}
	if (pKeyBoaed->GetKeyboardPress(DIK_7) == true)
	{
		nAddCnt += 1000;
	}
	if (pKeyBoaed->GetKeyboardPress(DIK_6) == true)
	{
		nAddCnt += 10000;
	}
	if (pKeyBoaed->GetKeyboardPress(DIK_5) == true)
	{
		nAddCnt += 100000;
	}
	if (pKeyBoaed->GetKeyboardPress(DIK_4) == true)
	{
		nAddCnt += 1000000;
	}
	if (pKeyBoaed->GetKeyboardPress(DIK_3) == true)
	{
		nAddCnt += 10000000;
	}
}

//=========================================================
// 敵の描画処理
//=========================================================
void CScore::Draw(void)
{
	for (int nCntScore = 0; nCntScore < m_nNumScore; nCntScore++)
	{
		m_apNumber[nCntScore]->Draw();
	}
}

//=========================================================
// スコアの加算処理
//=========================================================
void CScore::AddScore(int nValue)
{
	//スコアの加算
	m_nScore += nValue;

	SetScore(m_nScore);
}

//=========================================================
// スコアの上書き処理
//=========================================================
void CScore::SetScore(int nValue)
{
	//スコアの加算
	m_nScore = nValue;

	//スコアの値
	m_nNumber[0] = m_nScore % 100000000 / 10000000;
	m_nNumber[1] = m_nScore % 10000000 / 1000000;
	m_nNumber[2] = m_nScore % 1000000 / 100000;
	m_nNumber[3] = m_nScore % 100000 / 10000;
	m_nNumber[4] = m_nScore % 10000 / 1000;
	m_nNumber[5] = m_nScore % 1000 / 100;
	m_nNumber[6] = m_nScore % 100 / 10;
	m_nNumber[7] = m_nScore % 10 / 1;

	for (int nCntScore = 0; nCntScore < m_nNumScore; nCntScore++)
	{
		m_apNumber[nCntScore]->SetNumber(m_nNumber[nCntScore]);
	}
}
