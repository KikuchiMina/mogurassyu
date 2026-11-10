//=========================================================
// 
// "メッシュシリンダーの処理"	[meshcylinder.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/meshcylinder.h"
#include "../../Header/scene/game.h"
#include "../../Header/system/main.h"
#include "../../Header/system/manager.h"

//=========================================================
// マクロ定義
//=========================================================
#define Length		100.0f		// 長さ

//=========================================================
// 静的メンバ変数宣言
//=========================================================
LPDIRECT3DTEXTURE9 CMeshCylinder::m_pTexture = {};

//=========================================================
// オブジェクトの生成処理
//=========================================================
CMeshCylinder* CMeshCylinder::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fBack, D3DXVECTOR2 Block)
{
	CMeshCylinder* pMeshCylinder = NULL;

	if (pMeshCylinder == NULL)
	{
		// オブジェクトの生成
		pMeshCylinder = new CMeshCylinder;

		if (pMeshCylinder != NULL)
		{
			// オブジェクトの初期化処理
			pMeshCylinder->Init(pos, fWidth, fHeight, fBack, Block);

			return pMeshCylinder;
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
CMeshCylinder::CMeshCylinder(PRIORITY nPriority) : CObject(nPriority)
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

	m_fWidth = 0;		// 幅
	m_fHeight = 0;		// 高さ
	m_fBack = 0;		// 奥行
	m_Block = {};		// 分割数
	m_nNumAllVtx = 0;	// 頂点数の総数
	m_nNumAllIdx = 0;	// インデックス数の総数
}

//=========================================================
// デストラクタ
//=========================================================
CMeshCylinder::~CMeshCylinder()
{
}

//=========================================================
//プレイヤーのテクスチャ読み込み
//=========================================================
HRESULT CMeshCylinder::Load(void)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	CTexture* pTexture = CManager::GetTexture();		// テクスチャの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bg000.jpeg", &m_pTexture);

	return S_OK;
}

//=========================================================
//プレイヤーのテクスチャ破棄
//=========================================================
void CMeshCylinder::Unload(void)
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
HRESULT CMeshCylinder::Init(void)
{
	return S_OK;
}
HRESULT CMeshCylinder::Init(D3DXVECTOR3 pos, float fWidth, float fHeight, float fBack, D3DXVECTOR2 Block)
{ // 地面
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	//-------------------------------------
	// 代入
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fBack = fBack;
	m_pos = pos;

	m_Block = Block;											// 分割数の指定
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

			//-------------------------------------
			// 座標
			pVtx[nVtxCnt].pos.x = sinf(fRot) * Length;
			pVtx[nVtxCnt].pos.y = -m_fHeight * nCnyY + (m_fHeight * (m_Block.y) / 2.0f);
			pVtx[nVtxCnt].pos.z = cosf(fRot) * Length;

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
void CMeshCylinder::Uninit(void)
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
void CMeshCylinder::Update(void)
{
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < m_nNumAllVtx; nCount++)
	{
		float posX = m_pos.x + pVtx[nCount].pos.x;
		float posY = m_pos.y + pVtx[nCount].pos.y;
		float posZ = m_pos.z + pVtx[nCount].pos.z;
		D3DXVECTOR3 _pos = D3DXVECTOR3(posX, posY, posZ);
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		//頂点カラー(r,g,b,a)

		// 頂点の場所にエフェクトを出現
		CEffect::Create(_pos, move, col, 5.0f, 0.01f, 2, 1); // D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fRadiusSud, int nLife
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================================
// 地面の描画処理
//=========================================================
void CMeshCylinder::Draw(void)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();	// デバイスへのポインタの取得
	D3DXMATRIX mtxRot, mtxTrans;						// 計算用マトリックス

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

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0, 0,
		m_nNumAllVtx, 0,														// 用意した頂点の数
		(((int)m_Block.x * (int)m_Block.y) * 2) + (((int)m_Block.y - 1) * 4));	// 描画するプリミティブ(ポリゴン)数(見えるポリゴン + 見えないポリゴン)
}
