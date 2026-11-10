//=========================================================
// 
// "プレイヤーのストック表示処理"	[stock.h]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/stock.h"
#include "../../Header/object/player.h"
#include "../../Header/scene/game.h"

//=========================================================
//マクロ定義
//=========================================================
#define WIDTH			50.0f	// ストック1つの幅
#define HEIGHT			50.0f	// ストック1つの幅
#define FALSE_START		3		// 背スタックのスタートの数

//=========================================================
// 静的メンバ変数宣言
//=========================================================
LPDIRECT3DTEXTURE9 CStock::m_pTexture[STOCK_MAX] = {};
CObject2D* CStock::m_apObject2D[MAX_STOCK] = {};
int CStock::m_nNumStock = 0;
int CStock::m_nNumAllTrue = 0;

//=========================================================
// オブジェクトの生成処理
//=========================================================
CStock* CStock::Create(D3DXVECTOR3 pos)
{
	CStock* pStock = NULL;

	if (pStock == NULL)
	{
		// オブジェクトの生成
		pStock = new CStock;

		if (pStock != NULL)
		{
			// オブジェクトの初期化処理
			pStock->Init(pos);

			return pStock;
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
CStock::CStock(PRIORITY nPriority) :CObject2D(nPriority)
{
	m_stock = STOCK_NONE;
}

//=========================================================
// デストラクタ
//=========================================================
CStock::~CStock()
{
	for (int nCntScore = 0; nCntScore < MAX_STOCK; nCntScore++)
	{
		if (m_apObject2D[nCntScore] != NULL)
		{
			m_apObject2D[nCntScore] = NULL;
		}
	}
	m_nNumAllTrue = 0;
}

//=========================================================
// テクスチャ読み込み
//=========================================================
HRESULT CStock::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stock_none.png", &m_pTexture[STOCK_NONE]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stock_true.png", &m_pTexture[STOCK_TRUE_R]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stock_trueYellow.png", &m_pTexture[STOCK_TRUE_Y]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stock_trueWhite.png", &m_pTexture[STOCK_TRUE_W]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stock_false.png", &m_pTexture[STOCK_FALSE]);

	return S_OK;
}

//=========================================================
// テクスチャ破棄
//=========================================================
void CStock::Unload(void)
{
	//テクスチャの破棄
	for (int nCount = 0; nCount < STOCK_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CStock::Init(void)
{
	return S_OK;
}
HRESULT CStock::Init(const D3DXVECTOR3 pos)
{
	for (int nCntScore = 0; nCntScore < MAX_STOCK; nCntScore++)
	{
		float fPosX = pos.x;
		fPosX += WIDTH * nCntScore;

		// ストックの初期化
		m_apObject2D[nCntScore] = m_apObject2D[nCntScore]->Create(D3DXVECTOR3(fPosX, pos.y, pos.z), WIDTH, HEIGHT);
		m_apObject2D[nCntScore]->BindTexture(m_pTexture[m_stock]);
	}

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CStock::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < MAX_STOCK; nCntScore++)
	{
		m_apObject2D[nCntScore]->Uninit();
		m_apObject2D[nCntScore] = NULL;
		m_nNumAllTrue--;
	}
}

//=========================================================
// 更新処理
//=========================================================
void CStock::Update(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();

	for (int nCntScore = 0; nCntScore < MAX_STOCK; nCntScore++)
	{
		m_apObject2D[nCntScore]->Update();
	}
}

//=========================================================
// 描画処理
//=========================================================
void CStock::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_STOCK; nCntScore++)
	{
		m_apObject2D[nCntScore]->Draw();
	}
}

//=========================================================
// テクスチャの設定処理
//=========================================================
void CStock::SetTex(int nIdx, STOCK stock)
{
	m_apObject2D[nIdx]->BindTexture(m_pTexture[stock]);
}
