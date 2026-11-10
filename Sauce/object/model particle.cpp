//=========================================================
// 
// "モデルのパーティクル処理"	[model particle.h]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/model particle.h"
#include "../../Header/system/main.h"
#include "../../Header/system/manager.h"

//=========================================================
//マクロ定義
//=========================================================
#define MAX_EFFECT		(10000)		//エフェクトの最大数
#define EFFECT_SIZE		(10.0f)		//エフェクトの大きさ = 弾の大きさ
#define RADIUS_DIF		(0.2f)		//半径の減り
#define LIFE_DIF		(1)			//寿命の減り

#define MOVEY_RAND_MIN	1.0f		// ランダムの範囲(-1.0よりも小さく、1.0よりも大きい値に設定)
#define MOVEY_RAND_MAX	5.0f		// ランダムの範囲(-5.0～5.0の間)
#define FILED_MOVE		100.0f		// 移動制限の範囲
#define PLAYER_RADIUS	10.0f		// プレイヤーの半径		// 仮置き
#define BOUND_BALL		-0.8f		// 跳ね返る慣性の値

//=========================================================
// 静的メンバ変数宣言
//=========================================================
int CModelParticle::m_nNumAll = 0;
LPDIRECT3DTEXTURE9 CModelParticle::m_apTexture[MODELPAR_MAX][NUM_TEXTURE] = {};	//テクスチャへのポインタ
LPD3DXMESH CModelParticle::m_pMash[MODELPAR_MAX] = {};			// メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER CModelParticle::m_pBuffMat[MODELPAR_MAX] = {};		// マテリアルへのポインタ
DWORD CModelParticle::m_dwNumMat[MODELPAR_MAX] = {};			// マテリアルの数

//=========================================================
// オブジェクトの生成処理
//=========================================================
CModelParticle* CModelParticle::Create(D3DXVECTOR3 pos, MODELPAR type)
{
	CModelParticle* pItem = NULL;

	if (pItem == NULL)
	{
		// オブジェクトの生成
		pItem = new CModelParticle;

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

void CModelParticle::Create(D3DXVECTOR3 pos, MODELPAR type, int nRandMax)
{
	// モデルパーティクルの読み込み
	Load();

	// エフェクトを生成する数
	int nRandCreate = rand() % nRandMax + 20;		//20～nRandMax
	for (int nCount = 0; nCount < nRandCreate; nCount++)
	{
		float AddX = (-1.0f) + (float)rand() / RAND_MAX * ((1.0f) - (-1.0f));		// moveX	// -1.0f~1.0f
		float AddY = (-1.0f) + (float)rand() / RAND_MAX * ((1.0f) - (-1.0f));		// moveX
		float AddZ = (-1.0f) + (float)rand() / RAND_MAX * ((1.0f) - (-1.0f));		// moveX
		D3DXVECTOR3 Add = D3DXVECTOR3(AddX, AddY, AddZ);
		Create(pos + Add, type);
	}
}

//=========================================================
// コンストラクタ
//=========================================================
CModelParticle::CModelParticle(PRIORITY nPriority) : CObjectX(nPriority)
{
	m_pos = {};			// 位置
	m_move = {};		// 移動量
	m_mtxWorld = {};	// ワールドマトリックス

	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);			// スケール
	m_scaledSize = D3DXVECTOR3(PLAYER_RADIUS * fabsf(m_scl.x), PLAYER_RADIUS * fabsf(m_scl.y), PLAYER_RADIUS * fabsf(m_scl.z));	// 半径のサイズ
	m_posKeepY = 0.0f;
	m_bJump = false;
}

//=========================================================
// デストラクタ
//=========================================================
CModelParticle::~CModelParticle()
{
	m_nNumAll--;
}

//=========================================================
// Xファイルの読み込み
//=========================================================
HRESULT CModelParticle::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得
	HWND hWnd = CManager::GetWnd();

	//-------------------------------------
	// Xファイルの読み取り
	D3DXLoadMeshFromX("data\\MODEL\\box000.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[MODELPAR_00], NULL, &m_dwNumMat[MODELPAR_00], &m_pMash[MODELPAR_00]);
	if (m_pBuffMat[MODELPAR_00] == NULL)
	{
		int nID = MessageBox(hWnd, "data/MODEL/box000.x\nFile not found", "エラーメッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		return S_FALSE;
	}
	else
	{
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat[MODELPAR_00]->GetBufferPointer();//マテリアルデータへのポインタを取得

		for (int nCntModel = 0; nCntModel < (int)m_dwNumMat[MODELPAR_00]; nCntModel++)
		{
			//テクスチャが存在しているかどうか
			if (pMat[nCntModel].pTextureFilename != NULL)
			{
				//ファイル名を使用してテクスチャを読み込む
				D3DXCreateTextureFromFile(pDevice, pMat[nCntModel].pTextureFilename, &m_apTexture[MODELPAR_00][nCntModel]);
			}
		}
	}

	D3DXLoadMeshFromX("data\\MODEL\\01_head.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[MODELPAR_01], NULL, &m_dwNumMat[MODELPAR_01], &m_pMash[MODELPAR_01]);
	if (m_pBuffMat[MODELPAR_01] == NULL)
	{
		int nID = MessageBox(hWnd, "data/MODEL/01_head.x\nFile not found", "エラーメッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		return S_FALSE;
	}
	else
	{
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat[MODELPAR_01]->GetBufferPointer();//マテリアルデータへのポインタを取得

		for (int nCntModel = 0; nCntModel < (int)m_dwNumMat[MODELPAR_01]; nCntModel++)
		{
			//テクスチャが存在しているかどうか
			if (pMat[nCntModel].pTextureFilename != NULL)
			{
				//ファイル名を使用してテクスチャを読み込む
				D3DXCreateTextureFromFile(pDevice, pMat[nCntModel].pTextureFilename, &m_apTexture[MODELPAR_01][nCntModel]);
			}
		}
	}

	D3DXLoadMeshFromX("data\\MODEL\\earth000.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[MODELPAR_02], NULL, &m_dwNumMat[MODELPAR_02], &m_pMash[MODELPAR_02]);
	if (m_pBuffMat[MODELPAR_02] == NULL)
	{
		int nID = MessageBox(hWnd, "data/MODEL/earth000.x\nFile not found", "エラーメッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		return S_FALSE;
	}
	else
	{
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat[MODELPAR_02]->GetBufferPointer();//マテリアルデータへのポインタを取得

		for (int nCntModel = 0; nCntModel < (int)m_dwNumMat[MODELPAR_02]; nCntModel++)
		{
			//テクスチャが存在しているかどうか
			if (pMat[nCntModel].pTextureFilename != NULL)
			{
				//ファイル名を使用してテクスチャを読み込む
				D3DXCreateTextureFromFile(pDevice, pMat[nCntModel].pTextureFilename, &m_apTexture[MODELPAR_02][nCntModel]);
			}
		}
	}

	return S_OK;
}
//=========================================================
// Xファイルの破棄
//=========================================================
void CModelParticle::Unload(void)
{
	for (int nCount = 0; nCount < MODELPAR_MAX; nCount++)
	{
		//-------------------------------------
		// メッシュの破棄
		//-------------------------------------
		if (m_pMash[nCount] != NULL)
		{
			m_pMash[nCount]->Release();
			m_pMash[nCount] = NULL;
		}

		//-------------------------------------
		// マテリアルの破棄
		//-------------------------------------
		if (m_pBuffMat[nCount] != NULL)
		{
			m_pBuffMat[nCount]->Release();
			m_pBuffMat[nCount] = NULL;
		}

		//-------------------------------------
		// テクスチャの破棄
		//-------------------------------------
		for (int nCountTex = 0; nCountTex < NUM_TEXTURE; nCountTex++)
		{
			if (m_apTexture[nCount][nCountTex] != NULL)
			{
				m_apTexture[nCount][nCountTex]->Release();
				m_apTexture[nCount][nCountTex] = NULL;
			}
		}

	}
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CModelParticle::Init(D3DXVECTOR3 pos, MODELPAR type)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

	m_posKeepY = pos.y;
	m_pos = pos;
	m_pos.y = 0.0f;
	m_type = type;

	//-------------------------------------
	// 頂点数を取得
	int nNumVtx = m_pMash[type]->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	DWORD dwSizeFVF = D3DXGetFVFVertexSize(m_pMash[type]->GetFVF());

	// 頂点バッファをロック
	BYTE* pVtxBuff;		// 頂点バッファのポインタ
	m_pMash[type]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

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
	m_pMash[type]->UnlockVertexBuffer();

	//-------------------------------------
	// ランダム設定
	while (-MOVEY_RAND_MIN < m_move.y && m_move.y < MOVEY_RAND_MIN)
	{ // ↑の場合回り続ける
		float moveX = (-MOVEY_RAND_MAX) + (float)rand() / RAND_MAX * ((MOVEY_RAND_MAX)-(-MOVEY_RAND_MAX));	// moveのランダム
		float moveY = (-MOVEY_RAND_MAX) + (float)rand() / RAND_MAX * ((MOVEY_RAND_MAX)-(-MOVEY_RAND_MAX));	// moveのランダム
		float moveZ = (-MOVEY_RAND_MAX) + (float)rand() / RAND_MAX * ((MOVEY_RAND_MAX)-(-MOVEY_RAND_MAX));	// moveのランダム
		m_move = D3DXVECTOR3(moveX, moveY, moveZ);	// 移動量の設定
	}

	// 向き
	float rotX = (-D3DX_PI) + (float)rand() / RAND_MAX * ((D3DX_PI)-(-D3DX_PI));	// moveのランダム
	float rotY = (-D3DX_PI) + (float)rand() / RAND_MAX * ((D3DX_PI)-(-D3DX_PI));	// moveのランダム
	float rotZ = (-D3DX_PI) + (float)rand() / RAND_MAX * ((D3DX_PI)-(-D3DX_PI));	// moveのランダム
	m_rot = D3DXVECTOR3(rotX, rotY, rotZ);

	// スケール
	float scl = (1.0f) + (float)rand() / RAND_MAX * ((MOVEY_RAND_MAX)-(1.0f));	// moveのランダム
	m_scl = D3DXVECTOR3(scl, scl, scl);	// 移動量の設定

	//-------------------------------------
	// 親クラスの初期化
	CObjectX::Init(m_pBuffMat[type], m_pMash[type], m_dwNumMat[type]);
	CObjectX::BindTexture(m_apTexture[type]);
	CObjectX::SetPosition(m_pos);
	CObjectX::SetRotation(m_rot);
	CObjectX::SetScale(m_scl);

	//-------------------------------------
	// 種類の設定
	SetType(TYPE_OBJECTX);

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CModelParticle::Uninit(void)
{
	CObjectX::Uninit();
}

//=========================================================
// 更新処理
//=========================================================
void CModelParticle::Update(void)
{
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ
	CInputMouse* pMouse = CManager::GetMouse();				// マウスへのポインタ

	//-------------------------------------
	// 位置の更新
	CObjectX::SetPosition(m_pos);
	CObjectX::SetRotation(m_rot);
	CObjectX::SetScale(m_scl);
	CObjectX::Update();

	//-------------------------------------
	//常に重力を加える
	m_move.y -= 1.0f;

	// ジャンプ
	if (m_bJump == false && m_pos.y <= m_posKeepY)
	{
		m_move.y += 15.0f;
		m_bJump = true;	// ジャンプした
	}

	//-------------------------------------
	// 位置を更新
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;

	// 位置の減衰
	m_move.x += (0.0f - m_move.x) * 0.025f;	//元0.125
	m_move.y += (0.0f - m_move.y) * 0.025f;	//元0.125
	m_move.z += (0.0f - m_move.z) * 0.025f;	//元0.125

	// 地面に埋め込ませない
	if (m_pos.y <= m_scaledSize.y)
	{
		m_move.y = BOUND_BALL * m_move.y;	// 跳ね返り
		m_pos.y = m_scaledSize.y;			// 位置の強制
	}

	//-------------------------------------
	// 向きの回転
	D3DXVECTOR3 rotAdd = D3DXVECTOR3(0.05f, 0.05f, 0.05f);
	m_rot += rotAdd;

	//-------------------------------------
	//半径の減り
	D3DXVECTOR3 sclSub = D3DXVECTOR3(0.05f, 0.05f, 0.05f);
	m_scl -= sclSub;
	if (m_scl.x <= 0.0f)
	{
		m_scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Uninit();
	}

	// デバック表示
	CDebugProc* pDebugProc = CManager::GetDebugProc();
	//pDebugProc->PrintDebugProc("\n\n**** エフェクト****");
	//pDebugProc->PrintDebugProc("\n< 寿命 > : (%d)", m_nLife);
	//pDebugProc->PrintDebugProc("\n< 半径 > : (%f)", m_fRadius);
	//pDebugProc->PrintDebugProc("\n< 色 > : (a:%f)", m_col.a);
}

//=========================================================
// 描画処理
//=========================================================
void CModelParticle::Draw(void)
{
	CObjectX::Draw();

	// スケールに応じた半径サイズ
	m_scaledSize = D3DXVECTOR3(PLAYER_RADIUS * fabsf(m_scl.x), PLAYER_RADIUS * fabsf(m_scl.y), PLAYER_RADIUS * fabsf(m_scl.z));
}
