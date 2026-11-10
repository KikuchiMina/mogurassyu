//=========================================================
// 
// "オブジェクト2Dの処理"	[object.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/objectBillboard.h"
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
int CObjectBill::m_nNumAll = 0;

//=========================================================
// 生成処理
//=========================================================
CObjectBill* CObjectBill::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CObjectBill* pObjectBill = NULL;

	if (pObjectBill == NULL)
	{
		// オブジェクトの生成
		pObjectBill = new CObjectBill;

		if (pObjectBill != NULL)
		{
			// オブジェクトの初期化処理
			pObjectBill->Init(pos, fWidth, fHeight);

			m_nNumAll++;

			return pObjectBill;
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
CObjectBill::CObjectBill(PRIORITY nPriority):CObject(nPriority)
{
	m_pTexture = NULL;						// テクスチャ
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// 色
	m_fWidth = 0;							// 幅サイズ
	m_fHeight = 0;							// 縦サイズ
}


//=========================================================
// デストラクタ
//=========================================================
CObjectBill::~CObjectBill()
{
	m_nNumAll--;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CObjectBill::Init(void)
{
	return 0;
}
HRESULT CObjectBill::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// オブジェクトの初期化処理
	m_pos = pos;
	//m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	
	VERTEX_3D* pVtx;									// 頂点情報へのポインタ
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x -= m_fWidth;
	pVtx[1].pos.x += m_fWidth;
	pVtx[2].pos.x -= m_fWidth;
	pVtx[3].pos.x += m_fWidth;
	pVtx[0].pos.y += m_fHeight;
	pVtx[1].pos.y += m_fHeight;
	pVtx[2].pos.y -= m_fHeight;
	pVtx[3].pos.y -= m_fHeight;

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CObjectBill::Uninit(void)
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
// 更新処理
//=========================================================
void CObjectBill::Update(void)
{
	
}

//=========================================================
// 描画処理
//=========================================================
void CObjectBill::Draw(void)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();	// デバイスへのポインタの取得
	D3DXMATRIX mtxTrans;								//計算用マトリックス
	D3DXMATRIX mtxView;									//ビューマトリックス

	//-------------------------------------
	//ワールドマトリックスの初期化
	//-------------------------------------
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリクスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);	//逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-------------------------------------
	//頂点バッファをデータストリームに設定
	//-------------------------------------
	pDevice->SetStreamSource(0,
		m_pVtxBuff,							//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D));					//頂点情報構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//Zテストを無効にする(描画優先)
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		//アルファテストを有効
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				0,						//描画する最初の頂点インデックス
				2);						//プリミティブ(ポリゴン)数

		//アルファテストを無効
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=========================================================
// 位置設定(中心が頂点)
//=========================================================
void CObjectBill::SetPosition(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	m_pos = pos;
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	VERTEX_3D* pVtx;									//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = -m_fWidth;
	pVtx[1].pos.x = +m_fWidth;
	pVtx[2].pos.x = -m_fWidth;
	pVtx[3].pos.x = +m_fWidth;
	pVtx[0].pos.y = +m_fHeight;
	pVtx[1].pos.y = +m_fHeight;
	pVtx[2].pos.y = -m_fHeight;
	pVtx[3].pos.y = -m_fHeight;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================================
// 色設定
//=========================================================
void CObjectBill::SetColor(D3DXCOLOR col)
{
	m_col = col;
	
	VERTEX_3D* pVtx;									//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================================
// テクスチャの割り当て
//=========================================================
void CObjectBill::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}