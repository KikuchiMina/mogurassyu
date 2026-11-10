//=========================================================
// 
// "Xファイルの処理"	[objectX.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/objectX.h"
#include "../../Header/system/manager.h"

//=========================================================
// マクロ定義
//=========================================================

//=========================================================
// 静的メンバ変数宣言
//=========================================================
int CObjectX::m_nNumAll = 0;

//=========================================================
// オブジェクトの生成処理
//=========================================================
CObjectX* CObjectX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	CObjectX* pObjectX = NULL;

	if (pObjectX == NULL)
	{
		// オブジェクトの生成
		pObjectX = new CObjectX;

		if (pObjectX != NULL)
		{
			// オブジェクトの初期化処理
			pObjectX->Init();

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
// コンストラクタ
//=========================================================
CObjectX::CObjectX(PRIORITY nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// スケール
}


//=========================================================
// デストラクタ
//=========================================================
CObjectX::~CObjectX()
{
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CObjectX::Init(void)
{
	return 0;
}
HRESULT CObjectX::Init(LPD3DXBUFFER pBuffMat, LPD3DXMESH pMash, DWORD dwNumMat)
{
	m_pBuffMat = pBuffMat;
	m_pMash = pMash;
	m_dwNumMat = dwNumMat;

	//CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	//LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();	// デバイスへのポインタの取得

	//D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();//マテリアルデータへのポインタを取得

	//for (int nCntModel = 0; nCntModel < (int)m_dwNumMat; nCntModel++)
	//{
	//	//テクスチャが存在しているかどうか
	//	if (pMat[nCntModel].pTextureFilename != NULL)
	//	{
	//		//ファイル名を使用してテクスチャを読み込む
	//		D3DXCreateTextureFromFile(pDevice, pMat[nCntModel].pTextureFilename, &m_apTexture[nCntModel]);
	//	}
	//}

	m_nNumAll++;
	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CObjectX::Uninit(void)
{
	m_nNumAll--;

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

	// オブジェクト(自分自身)の破棄
	CObject::Release();
}

//=========================================================
// 更新処理
//=========================================================
void CObjectX::Update(void)
{
	
}

//=========================================================
// 描画処理
//=========================================================
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// 計算用マトリックス
	D3DMATERIAL9 matDef;					// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;						// マテリアルデータのポインタ

	//-------------------------------------
	// 大きくしても暗くならない
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);	// 描画前(絶対)

	//-------------------------------------
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//-------------------------------------
	// スケールを反映
	D3DXMatrixScaling(&mtxScale, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
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

//=========================================================
// テクスチャの割り当て
//=========================================================
void CObjectX::BindTexture(LPDIRECT3DTEXTURE9 pTexture[NUM_TEXTURE])
{
	for (int nCount = 0; nCount < NUM_TEXTURE; nCount++)
	{
		m_apTexture[nCount] = pTexture[nCount];
	}
}