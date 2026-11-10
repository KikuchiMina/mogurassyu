//=========================================================
// 
// "メッシュリングの処理"	[meshring.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/meshring.h"
#include "../../Header/scene/game.h"
#include "../../Header/system/main.h"
#include "../../Header/system/manager.h"

//=========================================================
// マクロ定義
//=========================================================
#define MAX_OUT		400.0f		// 最大の長さ

//=========================================================
// 静的メンバ変数宣言
//=========================================================
LPDIRECT3DTEXTURE9 CMeshRing::m_pTexture = {};

//=========================================================
// オブジェクトの生成処理
//=========================================================
CMeshRing* CMeshRing::Create(D3DXVECTOR3 pos, float fInSide, float fOutSide, D3DXVECTOR2 Block)
{
	CMeshRing* pMeshRing = NULL;

	if (pMeshRing == NULL)
	{
		// オブジェクトの生成
		pMeshRing = new CMeshRing;

		if (pMeshRing != NULL)
		{
			// オブジェクトの初期化処理
			pMeshRing->Init(pos, fInSide, fOutSide, Block);

			return pMeshRing;
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
CMeshRing::CMeshRing(PRIORITY nPriority) : CObject(nPriority)
{
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_mtxWorld = {};
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き

	for (int nCount = 0; nCount < MAX_VTX; nCount++)
	{
		m_nor[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 法線
		m_posVtx[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置保存
	}

	m_fInSide = 0;		// 幅
	m_fOutSide = 0;		// 高さ
	m_Block = {};		// 分割数
	m_nNumAllVtx = 0;	// 頂点数の総数
	m_nNumAllIdx = 0;	// インデックス数の総数
}

//=========================================================
// デストラクタ
//=========================================================
CMeshRing::~CMeshRing()
{
}

//=========================================================
//プレイヤーのテクスチャ読み込み
//=========================================================
HRESULT CMeshRing::Load(void)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	CTexture* pTexture = CManager::GetTexture();		// テクスチャの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\gradation001.jpg", &m_pTexture);

	return S_OK;
}

//=========================================================
//プレイヤーのテクスチャ破棄
//=========================================================
void CMeshRing::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=========================================================
// 地面の初期化処理
//=========================================================
HRESULT CMeshRing::Init(void)
{
	return S_OK;
}
HRESULT CMeshRing::Init(D3DXVECTOR3 pos, float fInSide, float fOutSide,D3DXVECTOR2 Block)
{ // 地面
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	//-------------------------------------
	// 代入
	m_fInSide = fInSide;
	m_fOutSide = fOutSide;
	m_pos = pos;

	m_Block = D3DXVECTOR2(Block.x, 2);											// 分割数の指定
	m_nNumAllVtx = (int)(m_Block.x + 1) * (int)(m_Block.y + 1);	// 頂点数の計算
	m_nNumAllIdx = (((int)m_Block.x * (int)m_Block.y) * 2) + (((int)m_Block.y - 1) * 4) + 2;

	//-------------------------------------
	// 頂点情報へのポインタ
	//-------------------------------------
	VERTEX_3D* pVtx;

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumAllVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//-------------------------------------
	//頂点処理の設定
	for (int nCnyY = 0; nCnyY < ((int)m_Block.y + 1); nCnyY++)
	{
		for (int nCnyX = 0; nCnyX < ((int)m_Block.x + 1); nCnyX++)
		{
			int nVtxCnt = nCnyX + (nCnyY * ((int)m_Block.x + 1));

			float fRot = ((D3DX_PI * 2) / (int)m_Block.x) * nCnyX;

			if (D3DX_PI <= fRot)
			{
				fRot -= D3DX_PI * 2;
			}
			if (fRot <= -D3DX_PI)
			{
				fRot += D3DX_PI * 2;
			}

			float Length = 100.0f;
			if (nCnyY == 0)
			{ // 偶数
				Length = m_fOutSide;
			}
			else
			{
				Length = m_fInSide;
			}

			//-------------------------------------
			// 座標
			pVtx[nVtxCnt].pos.x = sinf(fRot) * Length;
			pVtx[nVtxCnt].pos.y = 10.0f;
			pVtx[nVtxCnt].pos.z = cosf(fRot) * Length;

			/*float m_fHeight = 100.0f;
			pVtx[nVtxCnt].pos.x = sinf(fRot) * Length;
			pVtx[nVtxCnt].pos.y = -m_fHeight * nCnyY + (m_fHeight * (m_Block.y) / 2.0f);
			pVtx[nVtxCnt].pos.z = cosf(fRot) * Length;*/

			// 座標の保存
			m_posVtx[nVtxCnt].x = pVtx[nVtxCnt].pos.x + m_pos.x;
			m_posVtx[nVtxCnt].y = pVtx[nVtxCnt].pos.y + m_pos.y;
			m_posVtx[nVtxCnt].z = pVtx[nVtxCnt].pos.z + m_pos.z;

			//-------------------------------------
			// 法線ベクトルの設定
			pVtx[nVtxCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_nor[nVtxCnt] = pVtx[nVtxCnt].nor;

			//-------------------------------------
			// 頂点カラーの設定
			pVtx[nVtxCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//-------------------------------------
			//テクスチャ座標の設定
			pVtx[nVtxCnt].tex = D3DXVECTOR2((float)nCnyX, (float)nCnyY);
		}
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//-------------------------------------
	// インデックス情報へのポインタ
	//-------------------------------------
	WORD* pIdx;

	// 頂点バッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumAllIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// 頂点バッファをロックし、頂点番号データへのポインタを獲得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//-------------------------------------
	// 番号設定
	int PrigonCnt = 0;
	for (int nIdxCnt = 0; nIdxCnt < (m_nNumAllIdx / 2); nIdxCnt++)
	{
		if ((nIdxCnt + 1) % ((int)m_Block.x + 2) == 0)
		{
			pIdx[0] = nIdxCnt - (PrigonCnt + 1);
			pIdx[1] = ((int)m_Block.x + 1) + nIdxCnt - PrigonCnt;
			PrigonCnt++;
		}
		else
		{
			pIdx[0] = ((int)m_Block.x + 1) + nIdxCnt - PrigonCnt;
			pIdx[1] = nIdxCnt - PrigonCnt;
		}
		pIdx += 2;
	}

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=========================================================
// 地面の終了処理
//=========================================================
void CMeshRing::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// オブジェクト(自分自身)の破棄
	CObject::Release();
}

//=========================================================
// 地面の更新処理
//=========================================================
void CMeshRing::Update(void)
{
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_fOutSide += 18.0f;
	m_fInSide += 15.0f;

	if (MAX_OUT <= m_fOutSide)
	{ // 制限
		m_fOutSide = MAX_OUT;

		if (m_fOutSide <= m_fInSide)
		{ // 外周と内周が重なったら削除
			Uninit();
			return;
		}
	}

	for (int nCnyY = 0; nCnyY < ((int)m_Block.y + 1); nCnyY++)
	{
		for (int nCnyX = 0; nCnyX < ((int)m_Block.x + 1); nCnyX++)
		{
			int nVtxCnt = nCnyX + (nCnyY * ((int)m_Block.x + 1));

			// 中心からの向き設定
			float fRot = ((D3DX_PI * 2) / (int)m_Block.x) * nCnyX;
			if (D3DX_PI <= fRot)
			{
				fRot -= D3DX_PI * 2;
			}
			if (fRot <= -D3DX_PI)
			{
				fRot += D3DX_PI * 2;
			}

			// 中心からの長さ設定
			float Length = 100.0f;
			if (nCnyY == 0)
			{ // 偶数
				Length = m_fOutSide;
			}
			else
			{
				Length = m_fInSide;
			}

			//-------------------------------------
			// 座標
			pVtx[nVtxCnt].pos.x = sinf(fRot) * Length;
			pVtx[nVtxCnt].pos.y = 10.0f;
			pVtx[nVtxCnt].pos.z = cosf(fRot) * Length;
		}
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================================
// 地面の描画処理
//=========================================================
void CMeshRing::Draw(void)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();	// デバイスへのポインタの取得
	D3DXMATRIX mtxRot, mtxTrans;						// 計算用マトリックス

	//αブレンディングを加算合成に設定(背景の黒を消す処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//-------------------------------------
	//ワールドマトリックスの初期化
	//-------------------------------------
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-------------------------------------
	//頂点バッファをデータストリームに設定
	//-------------------------------------
	pDevice->SetStreamSource(0,
		m_pVtxBuff,						// 頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D));				// 頂点情報構造体のサイズ

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	////Zテストを無効にする(描画優先)
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//
	//	//アルファテストを有効
	//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0, 0,
		m_nNumAllVtx, 0,														// 用意した頂点の数
		(((int)m_Block.x * (int)m_Block.y) * 2) + (((int)m_Block.y - 1) * 4));	// 描画するプリミティブ(ポリゴン)数(見えるポリゴン + 見えないポリゴン)
	
	//	//アルファテストを無効
	//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	//
	////Zテストを有効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
