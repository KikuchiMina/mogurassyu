//=========================================================
// 
// "ブロックの処理"	[item.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/item.h"
#include "../../Header/object/score.h"
#include "../../Header/scene/game.h"
#include "../../Header/system/manager.h"

//=========================================================
//マクロ定義
//=========================================================
#define MOVEY_RAND_MIN		1.0f	// ランダムの範囲(-1.0よりも小さく、1.0よりも大きい値に設定)
#define MOVEY_RAND_MAX		5.0f	// ランダムの範囲(-5.0～5.0の間)
#define FILED_MOVE			100.0f	// 移動制限の範囲

//=========================================================
// 静的メンバ変数宣言
//=========================================================
int CItem::m_nNumAll = 0;

//=========================================================
// オブジェクトの生成処理
//=========================================================
CItem* CItem::Create(D3DXVECTOR3 pos, ITEM type)
{
	CItem* pItem = NULL;

	if (pItem == NULL)
	{
		// オブジェクトの生成
		pItem = new CItem;

		if (pItem != NULL)
		{
			// オブジェクトの初期化処理
			pItem->Init(pos, type);

			m_nNumAll++;

			return pItem;
		}
	}
	return pItem;
}

//=========================================================
// コンストラクタ
//=========================================================
CItem::CItem(PRIORITY nPriority) : CObjectX(nPriority)
{
	m_pMash = NULL;		// メッシュ(頂点情報)へのポインタ
	m_pBuffMat = NULL;	// マテリアルへのポインタ
	m_dwNumMat = NULL;	// マテリアルの数
	m_pos = {};			// 位置
	m_move = {};		// 移動量
	m_mtxWorld = {};	// ワールドマトリックス
}

//=========================================================
// デストラクタ
//=========================================================
CItem::~CItem()
{
	m_nNumAll--;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, ITEM type)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得
	HWND hWnd = CManager::GetWnd();

	m_pos = pos;
	m_type = type;

	//-------------------------------------
	// Xファイルの読み取り
	if (m_type == ITEM_00)
	{
		D3DXLoadMeshFromX("data\\MODEL\\car000.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_dwNumMat, &m_pMash);

		if (m_pBuffMat == NULL)
		{
			int nID = MessageBox(hWnd, "data/MODEL/car000.x\nFile not found", "エラーメッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			return S_FALSE;
		}
	}
	else if (m_type == ITEM_01)
	{
		D3DXLoadMeshFromX("data\\MODEL\\01_head.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_dwNumMat, &m_pMash);

		if (m_pBuffMat == NULL)
		{
			int nID = MessageBox(hWnd, "data/MODEL/01_head.x\nFile not found", "エラーメッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			return S_FALSE;
		}
	}
	else
	{
		return S_FALSE;
	}

	//-------------------------------------
	// 頂点数を取得
	int nNumVtx = m_pMash->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	DWORD dwSizeFVF = D3DXGetFVFVertexSize(m_pMash->GetFVF());

	// 頂点バッファをロック
	BYTE* pVtxBuff;		// 頂点バッファのポインタ
	m_pMash->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// 頂点座標の代入

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

	// 頂点バッファをアンロック
	m_pMash->UnlockVertexBuffer();

	//-------------------------------------
	// 親クラスの初期化
	CObjectX::Init(m_pBuffMat, m_pMash, m_dwNumMat);

	//-------------------------------------
	// 移動量の設定
	if (m_type == ITEM_00)
	{
		float moveX = (-MOVEY_RAND_MAX) + (float)rand() / RAND_MAX * ((MOVEY_RAND_MAX)-(-MOVEY_RAND_MAX));	// moveのランダム
		float moveZ = (-MOVEY_RAND_MAX) + (float)rand() / RAND_MAX * ((MOVEY_RAND_MAX)-(-MOVEY_RAND_MAX));	// moveのランダム
		m_move = D3DXVECTOR3(moveX, 0.0f, moveZ);	// 移動量の設定
	}
	if (m_type == ITEM_01)
	{
		while (-MOVEY_RAND_MIN < m_move.y && m_move.y < MOVEY_RAND_MIN)
		{ // ↑の場合回り続ける
			float move = (-MOVEY_RAND_MAX) + (float)rand() / RAND_MAX * ((MOVEY_RAND_MAX)-(-MOVEY_RAND_MAX));	// moveのランダム
			m_move.y = move;	// 移動量の設定
		}
	}

	//-------------------------------------
	// 種類の設定
	SetType(TYPE_OBJECTX);

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CItem::Uninit(void)
{
	CObjectX::Uninit();
}

//=========================================================
// 更新処理
//=========================================================
void CItem::Update(void)
{
	// デバック表示
	CDebugProc* pDebugProc = CManager::GetDebugProc();
	bool bMAX = false;	// 位置が規定を超えたか否か

	if (m_type == ITEM_01)
	{
		//-------------------------------------
		// 移動の制限
		if (0.0f < m_move.y)
		{ // m_move.yが＋の場合
			if (m_move.y < m_pos.y)
			{ // 位置が規定よりも大きくなった
				bMAX = true;
			}
		}
		else if (m_move.y < 0.0f)
		{ // m_move.yがーの場合
			if (m_pos.y < m_move.y)
			{ // 位置が規定よりも小さく
				bMAX = true;
			}
		}

		// 位置が規定を超えたか否か
		if (bMAX == true)
		{
			m_pos.y = m_move.y;	// 位置の強制
			m_move.y *= -1.0f;	// 移動量の反転
		}

		//-------------------------------------
		// 位置の更新
		m_pos.y += m_move.y * 0.025f;

		//pDebugProc->PrintDebugProc("\n**** アイテム ****");
		//pDebugProc->PrintDebugProc("\n< 移動量Y > : (%f)", m_move.y);
	}

	if (m_type == ITEM_00)
	{
		//-------------------------------------
		// 位置を更新
		m_pos.x += m_move.x;
		m_pos.z += m_move.z;

		//-------------------------------------
		// フィールドでの位置強制
		float distance = 10.0f;
		if (FILED_MOVE - distance < m_pos.x)
		{
			m_pos.x = FILED_MOVE - distance;
			m_move.x *= -1.0f;
		}
		if (m_pos.x < -FILED_MOVE + distance)
		{
			m_pos.x = -FILED_MOVE + distance;
			m_move.x *= -1.0f;
		}
		if (FILED_MOVE - distance < m_pos.z)
		{
			m_pos.z = FILED_MOVE - distance;
			m_move.z *= -1.0f;
		}
		if (m_pos.z < -FILED_MOVE + distance)
		{
			m_pos.z = -FILED_MOVE + distance;
			m_move.z *= -1.0f;
		}
	}

	//-------------------------------------
	// オブジェクトの位置更新
	CObjectX::SetPosition(m_pos);

	//-------------------------------------
	// 当たり判定
	Collision();
}

//=========================================================
// 描画処理
//=========================================================;
void CItem::Draw(void)
{
	CObjectX::Draw();
}

//=========================================================
// プレイヤーとアイテムの当たり判定
//=========================================================
bool CItem::Collision(void)
{
	CScore* pScore = CGame::GetScore();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();				// Playerの位置
	D3DXVECTOR3 PlayerDiameter = CGame::GetPlayer()->GetDiameter();		// PlayerのMax+Min
	D3DXVECTOR3 range = ((m_vtxMax - m_vtxMin) + PlayerDiameter) / 2;	// 半径

	float dx = PlayerPos.x - m_pos.x;
	float dz = PlayerPos.z - m_pos.z;
	float distance = sqrt((dx * dx) + (dz * dz));	// アイテムとキャラの距離

	if (distance <= range.x || distance <= range.z)
	{
		// エフェクトの生成
		D3DXCOLOR col = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f);		//頂点カラー(r,g,b,a)
		CEffect::Create(m_pos, col, 10, 30, 0.0f);

		// スコア増加
		pScore->AddScore(200);
		Uninit();
	}

	return false;
}
