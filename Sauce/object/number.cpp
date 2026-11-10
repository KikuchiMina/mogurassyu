//=========================================================
// 
// "ナンバー処理"	[number.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/number.h"
#include "../../Header/system/manager.h"

//=========================================================
//マクロ定義
//=========================================================

//=========================================================
// 静的メンバ変数宣言
//=========================================================
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//=========================================================
// オブジェクトの生成処理
//=========================================================
CNumber* CNumber::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CNumber* pNumber = NULL;

	if (pNumber == NULL)
	{
		// オブジェクトの生成
		pNumber = new CNumber;

		if (pNumber != NULL)
		{
			// オブジェクトの初期化処理
			pNumber->Init();
			pNumber->SetPosition(pos, fWidth, fHeight);

			return pNumber;
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
CNumber::CNumber()
{
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ
	m_nCounterAnime = 0;	// アニメーションカウンター
	m_nPatternAnime = 0;	// アニメーションパターンNo.
	m_fWidth = 0;			// 幅サイズ
	m_fHeight = 0;			// 縦サイズ
}

//=========================================================
// デストラクタ
//=========================================================
CNumber::~CNumber()
{
}

//=========================================================
// 敵のテクスチャ読み込み
//=========================================================
HRESULT CNumber::Load(void)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Number001.png", &m_pTexture);

	return S_OK;
}

//=========================================================
// 敵のテクスチャ破棄
//=========================================================
void CNumber::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=========================================================
// ナンバーの初期化処理
//=========================================================
HRESULT CNumber::Init(void)
{
	VERTEX_2D* pVtx;									//頂点情報へのポインタ
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点処理の設定
	pVtx[0].pos.x = m_pos.x;
	pVtx[1].pos.x = m_pos.x + m_fWidth;
	pVtx[2].pos.x = m_pos.x;
	pVtx[3].pos.x = m_pos.x + m_fWidth;
	pVtx[0].pos.y = m_pos.y;
	pVtx[1].pos.y = m_pos.y;
	pVtx[2].pos.y = m_pos.y + m_fHeight;
	pVtx[3].pos.y = m_pos.y + m_fHeight;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return 0;
}

//=========================================================
// ナンバーの終了処理
//=========================================================
void CNumber::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクト(自分自身)の破棄
	CObject::Release();
}

//=========================================================
// ナンバーの更新処理
//=========================================================
void CNumber::Update(void)
{
	//// デバック表示
	//CDebugProc* pDebugProc = CManager::GetDebugProc();
	//pDebugProc->PrintDebugProc("\nスコアの位置\n[%f  %f  %f]", m_pos.x, m_pos.y, m_pos.z);
}

//=========================================================
// ナンバーの描画処理
//=========================================================
void CNumber::Draw(void)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	// Zバッファを無効化
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// アルファブレンドを有効に設定(透明度を下げるため)
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,				// 頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D));		// 頂点情報構造体のサイズ

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
							0,						// 描画する最初の頂点インデックス
							2);						// プリミティブ(ポリゴン)数

	// アルファブレンドを元に戻す
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// Zバッファを有効化
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

//=========================================================
// ナンバーの設定処理
//=========================================================
void CNumber::SetPosition(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	VERTEX_2D* pVtx;									//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_pos = pos;

	//頂点処理の設定
	pVtx[0].pos.x = m_pos.x;
	pVtx[1].pos.x = m_pos.x + m_fWidth;
	pVtx[2].pos.x = m_pos.x;
	pVtx[3].pos.x = m_pos.x + m_fWidth;
	pVtx[0].pos.y = m_pos.y;
	pVtx[1].pos.y = m_pos.y;
	pVtx[2].pos.y = m_pos.y + m_fHeight;
	pVtx[3].pos.y = m_pos.y + m_fHeight;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================================
// ナンバーの設定処理
//=========================================================
void CNumber::SetColor(D3DXCOLOR col)
{
	VERTEX_2D* pVtx;									//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================================
// スコアの設定処理
//=========================================================
void CNumber::SetNumber(int nNumber)
{
	VERTEX_2D* pVtx;			//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex.x = 0.1f * nNumber;
	pVtx[1].tex.x = 0.1f * nNumber + 0.1f;
	pVtx[2].tex.x = 0.1f * nNumber;
	pVtx[3].tex.x = 0.1f * nNumber + 0.1f;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
