//=========================================================
// 
// "モデルの処理"	[model.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/model.h"
#include "../../Header/system/manager.h"

//=========================================================
// マクロ定義
//=========================================================
#define NUM_TEXTURE		100

//=========================================================
// 静的メンバ変数宣言
//=========================================================
int CModel::m_nNumModel = 0;			// 読み込んだ数
CModel* CModel::m_apModel = NULL;

//=========================================================
// オブジェクトの生成処理
//=========================================================
CModel* CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CModel* pObjectX = NULL;

	if (pObjectX == NULL)
	{
		// オブジェクトの生成
		pObjectX = new CModel;

		if (pObjectX != NULL)
		{
			// オブジェクトの初期化処理
			pObjectX->Init(pos, rot);

			return pObjectX;
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
CModel* CModel::Create(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CModel* pObjectX = NULL;

	if (pObjectX == NULL)
	{
		// オブジェクトの生成
		pObjectX = new CModel;

		if (pObjectX != NULL)
		{
			// オブジェクトの初期化処理
			pObjectX->Init(pFilename, pos, rot);

			return pObjectX;
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
// 親モデルの設定
//=========================================================
/*void CModel::SetParent(CModel* pModel)
{
	m_pParent = pModel;
}*/

//=========================================================
// コンストラクタ
//=========================================================
CModel::CModel()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	m_posOri = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_rotOri = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
}


//=========================================================
// デストラクタ
//=========================================================
CModel::~CModel()
{
}

//=========================================================
// ポリゴン2Dの初期化処理
//=========================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//-------------------------------------
	// 位置の設定
	m_pos = pos;
	m_posOri = pos;

	// 向きの設定
	m_rot = rot;
	m_rotOri = rot;

	m_nNumModel++;

	return S_OK;
}
HRESULT CModel::Init(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();	// デバイスへのポインタの取得

	//-------------------------------------
	// 位置の設定
	m_pos = pos;
	m_posOri = pos;

	// 向きの設定
	m_rot = rot;
	m_rotOri = rot;

	//-------------------------------------
	// Xファイルの読み取り
	D3DXLoadMeshFromX(pFilename, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_dwNumMat, &m_pMash);

	// マテリアルデータへのポインタを取得
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntModel = 0; nCntModel < (int)m_dwNumMat; nCntModel++)
	{
		// テクスチャが存在しているかどうか
		if (pMat[nCntModel].pTextureFilename != NULL)
		{
			// ファイル名を使用してテクスチャを読み込む
			D3DXCreateTextureFromFile(pDevice, pMat[nCntModel].pTextureFilename, &m_apTexture[nCntModel]);
		}
	}

	//-------------------------------------
	//頂点数を取得
	int nNumVtx = m_pMash->GetNumVertices();

	//頂点フォーマットのサイズを取得
	DWORD dwSizeFVF = D3DXGetFVFVertexSize(m_pMash->GetFVF());

	//頂点バッファをロック
	BYTE* pVtxBuff;		//頂点バッファのポインタ
	m_pMash->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

		//全ての頂点を比較してモデルの最大値/最小値を抜き出す
		if (vtx.x >= m_vtxMax.x)
		{
			m_vtxMax.x = vtx.x;
		}
		if (vtx.x <= m_vtxMin.x)
		{
			m_vtxMin.x = vtx.x;
		}

		if (vtx.y >= m_vtxMax.y)
		{
			m_vtxMax.y = vtx.y;
		}
		if (vtx.y <= m_vtxMin.y)
		{
			m_vtxMin.y = vtx.y;
		}

		if (vtx.z >= m_vtxMax.z)
		{
			m_vtxMax.z = vtx.z;
		}
		if (vtx.z <= m_vtxMin.z)
		{
			m_vtxMin.z = vtx.z;
		}

		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += dwSizeFVF;
	}

	//頂点バッファをアンロック
	m_pMash->UnlockVertexBuffer();

	m_nNumModel++;

	return S_OK;
}

//=========================================================
// テクスチャ読み込み
//=========================================================
HRESULT CModel::Lead(const char* pFilename)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();	// デバイスへのポインタの取得

	// Xファイルの読み取り
	D3DXLoadMeshFromX(pFilename, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_dwNumMat, &m_pMash);

	// マテリアルデータへのポインタを取得
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntModel = 0; nCntModel < (int)m_dwNumMat; nCntModel++)
	{
		// テクスチャが存在しているかどうか
		if (pMat[nCntModel].pTextureFilename != NULL)
		{
			// ファイル名を使用してテクスチャを読み込む
			D3DXCreateTextureFromFile(pDevice, pMat[nCntModel].pTextureFilename, &m_apTexture[nCntModel]);
		}
	}
	return S_OK;
}

//=========================================================
// ポリゴン2Dの終了処理
//=========================================================
void CModel::Uninit(void)
{
	//-------------------------------------
	// メッシュの破棄
	//-------------------------------------
	if (m_pMash != NULL)
	{
		m_pMash->Release();
		m_pMash = NULL;
	}

	//-------------------------------------
	// マテリアルの破棄
	//-------------------------------------
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//-------------------------------------
	// テクスチャの破棄
	//-------------------------------------
	for (int nCount = 0; nCount < NUM_TEXTURE; nCount++)
	{
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//=========================================================
// ポリゴン2Dの更新処理
//=========================================================
void CModel::Update(void)
{
}

//=========================================================
// ポリゴン2Dの描画処理
//=========================================================
void CModel::Draw(void)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();	// デバイスへのポインタの取得
	D3DXMATRIX mtxRot, mtxTrans;						// 計算用マトリックス
	D3DXMATRIX mtxParent;								// 親モデルのマトリックス
	D3DMATERIAL9 matDef;								// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;									// マテリアルデータのポインタ

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

	//-------------------------------------
	// 親のマトリックス掛け合わせる
	//-------------------------------------
	if (m_pParent != NULL)
	{ // 親が存在する
		// 親モデルのマトリックスを取得
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{ // 親が存在しない
		// 現在(最新)のマトリックスを取得[プレイヤーのマトリックス]
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 親のマトリックス掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//-------------------------------------
	//ワールドマトリックスの設定
	//-------------------------------------
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-------------------------------------
	//現在のマテリアルの取得
	//-------------------------------------
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntModel = 0; nCntModel < (int)m_dwNumMat; nCntModel++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntModel].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_apTexture[nCntModel]);

		//モデル(パーツ)の描画
		m_pMash->DrawSubset(nCntModel);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

void CModel::PosRot(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;
}
