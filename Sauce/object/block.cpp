//=========================================================
// 
// "ブロックの処理"	[block.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/block.h"
#include "../../Header/scene/game.h"
#include "../../Header/system/manager.h"

//=========================================================
//マクロ定義
//=========================================================
#define MAX_BLOCK				(128)		//ブロックの最大数
#define BLOCK_X_SIZE			10.0f		//弾のサイズ
#define BLOCK_EDGE				20.0f		//画面四方
#define BLOCK_TEX_SAZE			25.0f		//テクスチャの元サイズ

//=========================================================
// 静的メンバ変数宣言
//=========================================================
int CBlock::m_nNumAll = 0;
bool CBlock::m_OnBlock = false;

//=========================================================
// オブジェクトの生成処理
//=========================================================
CBlock* CBlock::Create(D3DXVECTOR3 pos, BLOCKTYPE type)
{
	CBlock* pBlock = NULL;

	if (pBlock == NULL)
	{
		// オブジェクトの生成
		pBlock = new CBlock;

		if (pBlock != NULL)
		{
			// オブジェクトの初期化処理
			pBlock->Init(pos, type);

			m_nNumAll++;

			return pBlock;
		}
	}
	return pBlock;
}

void CBlock::Create(BLOCKTYPE type)
{
	float posX = -0.0f;
	float posY = -0.0f;

	for (int nCntB = 0; nCntB < 24; nCntB++)
	{ // 縦
		for (int nCntA = 0; nCntA < 43; nCntA++)
		{ // 横
			float moveX = posX + nCntA * 30.0f;
			float moveY = posY + nCntB * 30.0f;
			Create(D3DXVECTOR3(moveX, moveY, 100.0f), type);
		}
	}
}

//=========================================================
// コンストラクタ
//=========================================================
CBlock::CBlock(PRIORITY nPriority) : CObjectX(nPriority)
{
	m_pMash = NULL;		// メッシュ(頂点情報)へのポインタ
	m_pBuffMat = NULL;	// マテリアルへのポインタ
	m_dwNumMat = NULL;	// マテリアルの数
	m_pos = {};			// 位置
	m_scl = {};			// スケール
	m_mtxWorld = {};	// ワールドマトリックス
}

//=========================================================
// デストラクタ
//=========================================================
CBlock::~CBlock()
{
	m_nNumAll--;
}


//=========================================================
// 初期化処理
//=========================================================
HRESULT CBlock::Init(D3DXVECTOR3 pos, BLOCKTYPE type)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();	// デバイスへのポインタの取得
	HWND hWnd = CManager::GetWnd();

	//Xファイルの読み取り
	D3DXLoadMeshFromX("data\\MODEL\\block\\BlockAlive.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_dwNumMat, &m_pMash);

	if (m_pBuffMat == NULL)
	{
		int nID = MessageBox(hWnd, "data/MODEL/block/BlackBlock.x\nFile not found", "エラーメッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		return S_FALSE;
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

	//-------------------------------------
	// 位置の代入
	m_pos = pos;
	m_scl = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
	m_type = type;

	//-------------------------------------
	// 親クラスの初期化
	CObjectX::Init(m_pBuffMat, m_pMash, m_dwNumMat);
	CObjectX::SetScale(m_scl);

	//-------------------------------------
	// 種類の設定
	SetType(TYPE_OBJECTX);

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CBlock::Uninit(void)
{
	CObjectX::Uninit();
}

//=========================================================
// 更新処理
//=========================================================
void CBlock::Update(void)
{
	// デバック表示
	CDebugProc* pDebugProc = CManager::GetDebugProc();

	// オブジェクトの位置更新
	CObjectX::SetPosition(m_pos);
}

//=========================================================
// 描画処理
//=========================================================;
void CBlock::Draw(void)
{
	CObjectX::Draw();
}

//=========================================================
// プレイヤーとブロックの当たり判定
//=========================================================
bool CBlock::Collision(D3DXVECTOR3* pos, D3DXVECTOR3 posOld, float* fHiegth)
{
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	D3DXVECTOR3 posMax = m_pos + m_vtxMax;
	D3DXVECTOR3 posMin = m_pos + m_vtxMin;

	// 左右の範囲
	if (posMin.x <= (*pos).x && (*pos).x < posMax.x)
	{
		if ((*pos).y <= posMax.y && posMax.y <= posOld.y)
		{ // 上に乗っている
			(*fHiegth) = posMax.y;

			m_OnBlock = true;
			return true;
		}
	}

	// 上下の範囲
	if (posMin.y <=  (*pos).y && (*pos).y < posMax.y)
	{
		if (posOld.x <= posMin.x)
		{
			if (posMin.x <= (*pos).x)
			{
				(*pos).x = posMin.x;	// ブロックの左に位置を戻す

				return true;
			}
		}
		else if ((*pos).x <= posMax.x && posMax.x <= posOld.x)
		{ // 右から入った
			(*pos).x = posMax.x;	// ブロックの左に位置を戻す

			return true;
		}
	}

	return false;
}
