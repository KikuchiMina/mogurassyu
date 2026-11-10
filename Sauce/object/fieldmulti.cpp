//=========================================================
// 
// "マルチテクスチャ地面の処理"	[fieldmulti.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/fieldmulti.h"
#include "../../Header/scene/game.h"
#include "../../Header/system/main.h"
#include "../../Header/system/manager.h"

//=========================================================
//マクロ定義
//=========================================================
#define TEX_MOVE	0.001f	// テクスチャの移動量

//=========================================================
// 静的メンバ変数宣言
//=========================================================
LPDIRECT3DTEXTURE9 CFieldMulti::m_pTexture[FIELD_MULTITEX_MAX] = {};
int CFieldMulti::m_nNumAll = 0;

//=========================================================
// オブジェクトの生成処理
//=========================================================
CFieldMulti* CFieldMulti::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fBack)
{
	CFieldMulti* pFieldMulti = NULL;

	if (pFieldMulti == NULL)
	{
		// オブジェクトの生成
		pFieldMulti = new CFieldMulti;

		if (pFieldMulti != NULL)
		{
			// オブジェクトの初期化処理
			pFieldMulti->Init(pos, fWidth, fHeight, fBack);

			return pFieldMulti;
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
CFieldMulti::CFieldMulti(PRIORITY nPriority) : CObject(nPriority)
{
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
}

//=========================================================
// デストラクタ
//=========================================================
CFieldMulti::~CFieldMulti()
{
}

//=========================================================
//プレイヤーのテクスチャ読み込み
//=========================================================
HRESULT CFieldMulti::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();;	// デバイスへのポインタの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\brick01.jpg", &m_pTexture[FIELD_MULTITEX_00]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\cloud00.png", &m_pTexture[FIELD_MULTITEX_01]);

	return S_OK;
}

//=========================================================
//プレイヤーのテクスチャ破棄
//=========================================================
void CFieldMulti::Unload(void)
{
	//テクスチャの破棄
	for (int nCount = 0; nCount < FIELD_MULTITEX_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=========================================================
// 地面の初期化処理
//=========================================================
HRESULT CFieldMulti::Init(void)
{
	return S_OK;
}
HRESULT CFieldMulti::Init(D3DXVECTOR3 pos, float fWidth, float fHeight, float fBack)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();;	// デバイスへのポインタの取得
	VERTEX_3D_MULTI* pVtx;									// 頂点情報へのポインタ

	//-------------------------------------
	// 代入
	m_pos = pos;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fBack = fBack;

	//-------------------------------------
	//頂点バッファの作成
	//-------------------------------------
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D_MULTI) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&m_pVtxBuff,
								NULL);

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//-------------------------------------
	//頂点処理の設定
	pVtx[0].pos.x = -m_fWidth;
	pVtx[1].pos.x = m_fWidth;
	pVtx[2].pos.x = -m_fWidth;
	pVtx[3].pos.x = m_fWidth;
	pVtx[0].pos.y = m_fHeight;
	pVtx[1].pos.y = m_fHeight;
	pVtx[2].pos.y = -m_fHeight;
	pVtx[3].pos.y = -m_fHeight;
	pVtx[0].pos.z = m_fBack;
	pVtx[1].pos.z = m_fBack;
	pVtx[2].pos.z = -m_fBack;
	pVtx[3].pos.z = -m_fBack;

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].texM = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].texM = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].texM = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].texM = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================================
// 地面の終了処理
//=========================================================
void CFieldMulti::Uninit(void)
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
// 地面の更新処理
//=========================================================
void CFieldMulti::Update(void)
{
	VERTEX_3D_MULTI* pVtx;									// 頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//-------------------------------------
	// テクスチャ座標の設定
	pVtx[0].tex.x += TEX_MOVE;
	pVtx[1].tex.x += TEX_MOVE;
	pVtx[2].tex.x += TEX_MOVE;
	pVtx[3].tex.x += TEX_MOVE;

	// テクスチャ座標の設定
	pVtx[0].texM.x -= TEX_MOVE;
	pVtx[1].texM.x -= TEX_MOVE;
	pVtx[2].texM.x -= TEX_MOVE;
	pVtx[3].texM.x -= TEX_MOVE;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================================
// 地面の描画処理
//=========================================================
void CFieldMulti::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得
	D3DXMATRIX mtxRot, mtxTrans;										// 計算用マトリックス

	//-------------------------------------
	// ワールドマトリックスの初期化
	//-------------------------------------
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-------------------------------------
	// マルチテクスチャの設定
	//-------------------------------------
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,							//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D_MULTI));			//頂点情報構造体のサイズ

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D_MULTI);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture[FIELD_MULTITEX_00]);
	pDevice->SetTexture(1, m_pTexture[FIELD_MULTITEX_01]);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,						//描画する最初の頂点インデックス
		2);						//プリミティブ(ポリゴン)数

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


}