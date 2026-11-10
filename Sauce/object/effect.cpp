//=========================================================
// 
// "エフェクトの処理"	[effect.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/effect.h"
#include "../../Header/scene/game.h"
#include "../../Header/system/manager.h"

//=========================================================
//マクロ定義
//=========================================================
#define MAX_EFFECT		(10000)		//エフェクトの最大数
#define EFFECT_SIZE		(10.0f)		//エフェクトの大きさ = 弾の大きさ
#define RADIUS_DIF		(0.2f)		//半径の減り
#define LIFE_DIF		(1)			//寿命の減り

//=========================================================
// 静的メンバ変数宣言
//=========================================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;	// テクスチャ
int CEffect::m_nCntAll = 0;

//=========================================================
// オブジェクトの生成処理
//=========================================================
CEffect* CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fRadiusSud, int nLife, int nLifeSud)
{
	CEffect* pEffect = NULL;

	if (pEffect == NULL)
	{
		// オブジェクトの生成
		pEffect = new CEffect;

		if (pEffect != NULL)
		{
			// オブジェクトの初期化処理
			pEffect->Init(pos, move, col, fRadius, fRadiusSud, nLife, nLifeSud);
			m_nCntAll++;

			return pEffect;
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

// パーティクル
void CEffect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, int nMinRand, int nMaxRand, float fRadius)
{
	// エフェクトを生成する数
	int nRandCreate = rand() % nMaxRand + nMinRand;		//10～40
	for (int nCount = 0; nCount < nRandCreate; nCount++)
	{
		float moveX = (-1.0f) + (float)rand() / RAND_MAX * ((1.0f) - (-1.0f));		// moveX	// -1.0f~1.0f
		float moveY = (-1.0f) + (float)rand() / RAND_MAX * ((0.0f) - (-1.0f));		// moveY
		if (fRadius == 0.0f)
		{
			fRadius = (3.0f) + (float)rand() / RAND_MAX * ((7.5f) - (3.0f));		// 半径
		}
		float fRadiusSud = (0.1f) + (float)rand() / RAND_MAX * ((0.5f) - (0.1f));	// 半径の減少
		Create(pos, D3DXVECTOR3(moveX, moveY, 0.0f), col, fRadius, fRadiusSud, 50, LIFE_DIF);
	}
}

// 穴掘り用エフェクト
void CEffect::Create(D3DXCOLOR col)
{
	for (int nCountX = 0; nCountX < FILED_X; nCountX++)
	{
		for (int nCountY = 0; nCountY < FILED_Y + 10; nCountY++)
		{
			if (nCountY % 15 == 0 && nCountX % 15 == 0)
			{
				float posX = nCountX * 1.0f;
				float posY = nCountY * 1.0f;

				Create(D3DXVECTOR3(posX, posY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, 12.0f, 0.0f, 50, 0);
			}
		}
	}
}

//=========================================================
// コンストラクタ
//=========================================================
CEffect::CEffect(PRIORITY nPriority) : CObjectBill(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_type = EFFECT_TYPE_MAX;					// 種類
	m_fRadius = 10.0f;							// 半径(大きさ)
	m_fRadiusSud = RADIUS_DIF;					// 半径
	m_nLife = 10;								// 寿命(表示時間)
	m_nLifeSud = LIFE_DIF;						// 寿命の減少
}

//=========================================================
// デストラクタ
//=========================================================
CEffect::~CEffect()
{
	m_nCntAll--;
}

//=========================================================
// テクスチャ読み込み
//=========================================================
HRESULT CEffect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\effect000.jpg", &m_pTexture);

	return S_OK;
}

//=========================================================
// テクスチャ破棄
//=========================================================
void CEffect::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CEffect::Init(void)
{
	return S_OK;
}
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fRadiusSud, int nLife, int nLifeSud)
{
	m_pos = pos;
	m_move = move;
	m_col = col;
	m_fRadius = fRadius;
	m_fRadiusSud = fRadiusSud;
	m_nLife = nLife;
	m_nLifeSud = nLifeSud;

	CObjectBill::BindTexture(m_pTexture);
	CObjectBill::Init(m_pos, m_fRadius, m_fRadius);
	CObjectBill::SetColor(m_col);

	return 0;
}

//=========================================================
// 終了処理
//=========================================================
void CEffect::Uninit(void)
{
	CObjectBill::Uninit();
}

//=========================================================
// 更新処理
//=========================================================
void CEffect::Update(void)
{
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ
	CInputMouse* pMouse = CManager::GetMouse();				// マウスへのポインタ

	//移動量の設定
	m_pos.x -= m_move.x;
	m_pos.y -= m_move.y;
	m_pos.z -= m_move.z;

	//半径の減り
	m_fRadius -= m_fRadiusSud;		//半径
	if (m_fRadius <= 0.0f)
	{
		m_fRadius = 0.0f;
	}

	//寿命の減り
	m_nLife -= m_nLifeSud;

	//寿命が尽きた
	if (m_nLife <= 0)
	{
		Uninit();
		return;
	}

	// 位置の更新
	CObjectBill::SetPosition(m_pos, m_fRadius, m_fRadius);
	CObjectBill::Update();

	// プレイヤーとの当たり判定
	if (true == CollisionPlayer())
	{
		return;
	}

	// デバック表示
	CDebugProc* pDebugProc = CManager::GetDebugProc();
	//pDebugProc->PrintDebugProc("\n\n**** エフェクト****");
	//pDebugProc->PrintDebugProc("\n< 寿命 > : (%d)", m_nLife);
	//pDebugProc->PrintDebugProc("\n< 総数 > : (%d)", m_nCntAll);
	//pDebugProc->PrintDebugProc("\n< 半径 > : (%f)", m_fRadius);
	//pDebugProc->PrintDebugProc("\n< 色 > : (a:%f)", m_col.a);
}

//=========================================================
// 描画処理
//=========================================================
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

	//αブレンディングを加算合成に設定(背景の黒を消す処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObjectBill::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=========================================================
// プレイヤーとの当たり判定処理
//=========================================================
bool CEffect::CollisionPlayer(void)
{
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	D3DXVECTOR3 PlayersclSize = CGame::GetPlayer()->GetDiameter();

	float dx = m_pos.x - PlayerPos.x;
	float dz = m_pos.y - PlayerPos.y;
	float distance = sqrt((dx * dx) + (dz * dz));	// 中心とキャラの距離
	distance = fabs(distance);	// 距離の正規化

	if (distance < PlayersclSize.x)
	{
		Uninit();
		return true;
	}
	return false;
}
