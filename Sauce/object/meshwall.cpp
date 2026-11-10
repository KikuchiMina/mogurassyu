//=========================================================
// 
// "メッシュ壁の処理"	[meshwall.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/meshwall.h"
#include "../../Header/scene/game.h"
#include "../../Header/system/main.h"
#include "../../Header/system/manager.h"

//=========================================================
// マクロ定義
//=========================================================
//#define BLOC_X	30	// 分割数X
//#define BLOC_Y	1	// 分割数Y

//=========================================================
// 静的メンバ変数宣言
//=========================================================
LPDIRECT3DTEXTURE9 CMeshWall::m_pTexture = {};

//=========================================================
// オブジェクトの生成処理
//=========================================================
CMeshWall* CMeshWall::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CMeshWall* pField = NULL;

	if (pField == NULL)
	{
		// オブジェクトの生成
		pField = new CMeshWall;

		if (pField != NULL)
		{
			// オブジェクトの初期化処理
			pField->Init(pos, fWidth, fHeight);

			return pField;
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
CMeshWall::CMeshWall(PRIORITY nPriority) : CObject(nPriority)
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
	m_Block = {};		// 分割数
	m_nNumAllVtx = 0;	// 頂点数の総数
	m_nNumAllIdx = 0;	// インデックス数の総数
	m_nDiaNum = 0;		// 斜めの値
}

//=========================================================
// デストラクタ
//=========================================================
CMeshWall::~CMeshWall()
{
}

//=========================================================
//プレイヤーのテクスチャ読み込み
//=========================================================
HRESULT CMeshWall::Load(void)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	CTexture* pTexture = CManager::GetTexture();		// テクスチャの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bg000.jpeg", &m_pTexture);
	//D3DXCreateTextureFromFile(pDevice, NULL, &m_pTexture);

	return S_OK;
}

//=========================================================
//プレイヤーのテクスチャ破棄
//=========================================================
void CMeshWall::Unload(void)
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
HRESULT CMeshWall::Init(void)
{
	return S_OK;
}
HRESULT CMeshWall::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	//-------------------------------------
	// 代入
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_pos = pos;

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

			//-------------------------------------
			// 座標
			//pVtx[nVtxCnt].pos.x = m_fWidth * nCnyX - (m_fWidth * (m_Block.x) / 2.0f);	// 中心が0
			pVtx[nVtxCnt].pos.x = m_fWidth * nCnyX - (m_fWidth * (m_Block.x) / 2.0f) + ((m_fWidth * (m_Block.x) / 2.0f) - m_fWidth);	// 中心合わせ + 半分ズレ + 幅
			pVtx[nVtxCnt].pos.y = -m_fHeight * nCnyY + (m_fHeight * (m_Block.y) / 2.0f);
			pVtx[nVtxCnt].pos.z = 0.0f;

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
void CMeshWall::Uninit(void)
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
void CMeshWall::Update(void)
{
	
}

//=========================================================
// 地面の描画処理
//=========================================================
void CMeshWall::Draw(void)
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

//=========================================================
// 地面の外積・内積を使った地面との当たり判定
//=========================================================
float CMeshWall::Collision(D3DXVECTOR3 posOld, D3DXVECTOR3 pos)
{
	float fHeight = MIN_HEIGHT;		// 求める高さ

	for (int nCount = 0; nCount < (int)m_Block.x; nCount++)
	{
		int Number0 = nCount;
		int Number1 = Number0 + 1;
		int Number2 = Number0 + ((int)m_Block.x + 1);
		int Number3 = Number2 + 1;

		//-------------------------------------
		// ベクトルの設定
		D3DXVECTOR3 vecA = m_posVtx[Number1] - m_posVtx[Number0];//
		D3DXVECTOR3 vecB = m_posVtx[Number2] - m_posVtx[Number0];
		D3DXVECTOR3 vecC = m_posVtx[Number3] - m_posVtx[Number1];//
		D3DXVECTOR3 vecD = m_posVtx[Number0] - m_posVtx[Number1];
		D3DXVECTOR3 vecE = m_posVtx[Number0] - m_posVtx[Number2];//
		D3DXVECTOR3 vecF = m_posVtx[Number3] - m_posVtx[Number2];
		D3DXVECTOR3 vecG = m_posVtx[Number2] - m_posVtx[Number3];//
		D3DXVECTOR3 vecH = m_posVtx[Number1] - m_posVtx[Number3];

		// 外積
		D3DXVec3Cross(&m_nor[Number0], &vecA, &vecB);
		D3DXVec3Cross(&m_nor[Number1], &vecC, &vecD);
		D3DXVec3Cross(&m_nor[Number2], &vecE, &vecF);
		D3DXVec3Cross(&m_nor[Number3], &vecG, &vecH);

		// 正規化
		D3DXVec3Normalize(&m_nor[Number0], &m_nor[Number0]);
		D3DXVec3Normalize(&m_nor[Number1], &m_nor[Number1]);
		D3DXVec3Normalize(&m_nor[Number2], &m_nor[Number2]);
		D3DXVec3Normalize(&m_nor[Number3], &m_nor[Number3]);

		//-------------------------------------
		// ベクトルの設定
		D3DXVECTOR3 vecLine30 = m_posVtx[Number3] - m_posVtx[Number0];
		D3DXVECTOR3 vecToPos1 = pos - m_posVtx[Number0];
		D3DXVECTOR3 vecToPos2 = pos - m_posVtx[Number1];
		D3DXVECTOR3 vecToPos3 = pos - m_posVtx[Number3];
		D3DXVECTOR3 vecToPos4 = pos - m_posVtx[Number2];
		float vec30 = (vecLine30.z * vecToPos3.x) - (vecLine30.x * vecToPos3.z);
		float vec1 = (vecA.z * vecToPos1.x) - (vecA.x * vecToPos1.z);
		float vec2 = (vecC.z * vecToPos2.x) - (vecC.x * vecToPos2.z);
		float vec3 = (vecG.z * vecToPos3.x) - (vecG.x * vecToPos3.z);
		float vec4 = (vecE.z * vecToPos4.x) - (vecE.x * vecToPos4.z);

		// プレイヤーが対象ポリゴンの内側にいる
		if (m_nor[Number0].y != 0.0f && m_nor[Number1].y != 0.0f && m_nor[Number2].y != 0.0f && m_nor[Number3].y != 0.0f)
		{
			if (0.0f <= vec1 && 0.0f <= vec2 && 0.0f <= vec3 && 0.0f <= vec4)
			{ // ポリゴン内にいる
				int Num = -1;		// 代入する値の初期化

				if (vec30 <= 0.0f)
				{ // 右
					Num = Number1;
				}
				else if (0.0f <= vec30)
				{ // 左
					Num = Number2;
				}

				if (0 <= Num)
				{
					D3DXVECTOR3 vecPos = pos - m_posVtx[Num];
					fHeight = (-vecPos.x * m_nor[Num].x - vecPos.z * m_nor[Num].z) / m_nor[Num].y + m_posVtx[Num].y;
					break;
				}
			}
		}
	}
	
	return fHeight;
}

//=========================================================
// 地面の外積・内積を使った地面との当たり判定
//=========================================================
bool CMeshWall::InField(D3DXVECTOR3 pos)
{
	m_posPlayer = pos;

	if (m_posVtx[0].x <= pos.x && pos.x <= m_posVtx[(int)m_Block.x].x)
	{
		return true;
	}

	return false;
}
