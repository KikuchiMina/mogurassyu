//=========================================================
// 
// "オブジェクト2Dの処理"	[object.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/object2D.h"
#include "../../Header/system/renderer.h"
#include "../../Header/system/manager.h"
#include "../../Header/system/input.h"

//=========================================================
//マクロ定義
//=========================================================
#define TEX_X			0.2f		// xの移動量
#define TEX_Y			0.5f		// yの移動量
#define TEX_DIVI		10			// 分割数
#define TEX_STEP		5			// 上下分割

// 静的メンバ変数宣言
int CObject2D::m_nNumAll = 0;

//=========================================================
// オブジェクトの生成処理
//=========================================================
CObject2D* CObject2D::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CObject2D* pObject2D = NULL;

	if (pObject2D == NULL)
	{
		// オブジェクトの生成
		pObject2D = new CObject2D;

		if (pObject2D != NULL)
		{
			// オブジェクトの初期化処理
			pObject2D->Init();
			pObject2D->SetPosition(pos, fWidth, fHeight);

			m_nNumAll++;

			return pObject2D;
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
CObject2D::CObject2D(PRIORITY nPriority):CObject(nPriority)
{
	m_pTexture = NULL;						// テクスチャ
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	m_type = TYPE_NONE;						// 種類
	m_nCounterAnime = 0;					// アニメーションカウンター
	m_nPatternAnime = 0;					// アニメーションパターンNo.
	m_fWidth = 0;							// 幅サイズ
	m_fHeight = 0;							// 縦サイズ
}


//=========================================================
// デストラクタ
//=========================================================
CObject2D::~CObject2D()
{
	m_nNumAll--;
}

//=========================================================
// ポリゴン2Dの初期化処理
//=========================================================
HRESULT CObject2D::Init(void)
{
	VERTEX_2D* pVtx;									// 頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return 0;
}
HRESULT CObject2D::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// オブジェクトの初期化処理
	CObject2D::Init();
	CObject2D::SetPosition(pos, fWidth, fHeight);

	return S_OK;
}

//=========================================================
// ポリゴン2Dの終了処理
//=========================================================
void CObject2D::Uninit(void)
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
// ポリゴン2Dの更新処理
//=========================================================
void CObject2D::Update(void)
{
	//VERTEX_2D* pVtx;	//頂点情報へのポインタ

	////頂点バッファをロックし、頂点情報へのポインタを獲得
	//m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	////頂点処理の設定
	//pVtx[0].pos.x = m_pos.x;
	//pVtx[1].pos.x = m_pos.x + m_fWidth;
	//pVtx[2].pos.x = m_pos.x;
	//pVtx[3].pos.x = m_pos.x + m_fWidth;
	//pVtx[0].pos.y = m_pos.y;
	//pVtx[1].pos.y = m_pos.y;
	//pVtx[2].pos.y = m_pos.y + m_fHeight;
	//pVtx[3].pos.y = m_pos.y + m_fHeight;
	//pVtx[0].pos.z = 0.0f;
	//pVtx[1].pos.z = 0.0f;
	//pVtx[2].pos.z = 0.0f;
	//pVtx[3].pos.z = 0.0f;
	//
	////頂点バッファをアンロック
	//m_pVtxBuff->Unlock();
}

//=========================================================
// ポリゴン2Dの描画処理
//=========================================================
void CObject2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

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
}

//=========================================================
// ポリゴン2Dの位置設定(右上が頂点)
//=========================================================
void CObject2D::SetPosition(D3DXVECTOR3 pos, float fWidth, float fHeight)
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

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=========================================================
// ポリゴン2Dの位置設定(上の真ん中が頂点)
//=========================================================
void CObject2D::SetPosition(D3DXVECTOR3 pos, float fWidth, float fHeight, int nCnt)
{
	VERTEX_2D* pVtx;									//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_pos = pos;

	float width = m_fWidth / 2;

	//頂点処理の設定
	pVtx[0].pos.x = m_pos.x - width;
	pVtx[1].pos.x = m_pos.x + width;
	pVtx[2].pos.x = m_pos.x - width;
	pVtx[3].pos.x = m_pos.x + width;
	//pVtx[0].pos.y = ;
	//pVtx[1].pos.y = ;
	pVtx[2].pos.y = m_pos.y + m_fHeight;
	pVtx[3].pos.y = m_pos.y + m_fHeight;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================================
// ポリゴン2Dの色設定
//=========================================================
void CObject2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D* pVtx;									//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================================
// ポリゴン2Dの位置設定
//=========================================================
void CObject2D::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=========================================================
// ポリゴン2Dの位置設定
//=========================================================
void CObject2D::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=========================================================
// ポリゴン2Dのテクスチャの割り当て
//=========================================================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}