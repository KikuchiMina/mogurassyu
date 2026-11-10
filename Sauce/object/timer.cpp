//=========================================================
// 
// "弾の処理"	[bullet.cpp]
// Author : KikuchiMina
// 
//=========================================================
// インクルードファイル
#include "../../Header/object/timer.h"
#include "../../Header/object/number.h"
#include "../../Header/scene/game.h"
#include "../../Header/system/manager.h"

//=========================================================
//マクロ定義
//=========================================================
#define NUM_WIDTH	60.0f
#define TIME		10		// ゲーム時間

//=========================================================
// 静的メンバ変数宣言
//=========================================================
//CNumber* CTimer::m_apNumber[NUM_TIME] = { NULL };
//int CTimer::m_nTime = NULL;
//int CTimer::m_nTimer[NUM_TIME] = { NULL };
//int CTimer::m_nNumTimer = NUM_TIME;

//=========================================================
// 敵オブジェクトの生成処理
//=========================================================
CTimer* CTimer::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNum, int nTime, TIMETYPE type)
{
	CTimer* pTimer = NULL;

	if (pTimer == NULL)
	{
		// オブジェクトの生成
		pTimer = new CTimer;

		if (pTimer != NULL)
		{
			// オブジェクトの初期化処理
			pTimer->Init(pos, fWidth, fHeight, nNum, nTime, type);

			return pTimer;
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
CTimer::CTimer(PRIORITY nPriority) :CObject(nPriority)
{
	m_nTimeCnt = 0;
	m_nTime = 0;
}

//=========================================================
// デストラクタ
//=========================================================
CTimer::~CTimer()
{
	m_nTime = 0;
}

//=========================================================
// 敵の初期化処理
//=========================================================
HRESULT CTimer::Init(void)
{
	return S_OK;
}
HRESULT CTimer::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nNum, int nTime, TIMETYPE type)
{
	m_nNumTimer = nNum;	// 桁数
	m_nTime = nTime;	// タイマーの値
	m_type = type;		// 種類

	for (int nCntScore = 0; nCntScore < m_nNumTimer; nCntScore++)
	{
		float fPosX = pos.x;
		fPosX += fWidth * nCntScore;

		// スコアの生成
		m_apNumber[nCntScore] = m_apNumber[nCntScore]->Create(D3DXVECTOR3(fPosX, pos.y, pos.z), fWidth, fHeight);

		// 透明度の変更
		if (m_type == TIMETYPE_MAIN)
		{
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
			m_apNumber[nCntScore]->SetColor(col);
		}
	}

	return S_OK;
}

//=========================================================
// 敵の終了処理
//=========================================================
void CTimer::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < m_nNumTimer; nCntScore++)
	{
		m_apNumber[nCntScore]->Uninit();
	}

	// オブジェクト(自分自身)の破棄
	CObject::Release();
}

//=========================================================
// 敵の更新処理
//=========================================================
void CTimer::Update(void)
{
	if (false == CGame::GetPause())
	{
		m_nTimeCnt++;

		if (m_nTimeCnt % 60 == 0 && 0 < m_nTimeCnt)
		{ // 1秒経過した
			m_nTime--;

			if (m_nTime == 3)
			{
				Create(D3DXVECTOR3(340.0f, 135.0f, 0.0f), 300.0f, 450.0f, 2, 3, TIMETYPE_MAIN);	// pos,w,h,桁数,時間
			}
			if (m_nTime < 0)
			{ // タイムの減算
				m_nTime = 0;

				// ゲームシーンの再読み込み
				//CFade::SetFade(CScene::MODE_RANKING);

				Uninit();
				return;
			}
		}

		//スコアの値
		int nBdata = 10;	//m_nTimer[2] = m_nTime % 10 / 1;
		int nSdata = 1;
		int _nNumTimer = m_nNumTimer - 1;
		for (int nCntScore = _nNumTimer; -1 < nCntScore; nCntScore--)
		{
			m_nTimer[nCntScore] = m_nTime % nBdata / nSdata;
			nBdata *= 10;
			nSdata *= 10;
		}

		for (int nCntScore = 0; nCntScore < m_nNumTimer; nCntScore++)
		{
			m_apNumber[nCntScore]->SetNumber(m_nTimer[nCntScore]);
			m_apNumber[nCntScore]->Update();
		}
	}
}

//=========================================================
// 敵の描画処理
//=========================================================
void CTimer::Draw(void)
{
	for (int nCntScore = 0; nCntScore < m_nNumTimer; nCntScore++)
	{
		m_apNumber[nCntScore]->Draw();
	}
}
