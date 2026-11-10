//=========================================================
// 
// "オブジェクト3Dの処理"	[object3D.h]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/object3D.h"
#include "../../Header/system/renderer.h"
#include "../../Header/system/manager.h"
#include "../../Header/system/input.h"

//=========================================================
//マクロ定義
//=========================================================
#define ASSIS		0.035f	// 補正係数
#define ASSIS_SWAY	0.01f	// 揺れる場合の補正係数
#define SWAY_WIDTH	0.25f	// 揺れる判定の幅
#define SWAY_BIG	0.3		// 揺れる時の左右判定の幅(大)
#define SWAY_SMO	0.2		// 揺れる時の左右判定の幅(小)

// 静的メンバ変数宣言
int CObject3D::m_nNumAll = 0;

//=========================================================
// オブジェクトの生成処理
//=========================================================
CObject3D* CObject3D::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fBack)
{
	CObject3D* pObject3D = NULL;

	if (pObject3D == NULL)
	{
		// オブジェクトの生成
		pObject3D = new CObject3D;

		if (pObject3D != NULL)
		{
			//// オブジェクトの初期化処理
			//pObject3D->SetPosition(pos, fWidth, fHeight);
			//pObject3D->SetMove(move);
			//pObject3D->SetRot(rot);
			pObject3D->Init();

			m_nNumAll++;

			return pObject3D;
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
CObject3D::CObject3D(PRIORITY nPriority):CObject(nPriority)
{
	m_pTexture = NULL;						// テクスチャ
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	m_nor[0] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_nor[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_nor[2] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_nor[3] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_bRight = false;
	m_bLeft = false;
	m_bSway = false;
}


//=========================================================
// デストラクタ
//=========================================================
CObject3D::~CObject3D()
{
	m_nNumAll--;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CObject3D::Init(void)
{
	return 0;
}
HRESULT CObject3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, float fBack)
{
	VERTEX_3D* pVtx;									// 頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();;	// デバイスへのポインタの取得

	//-------------------------------------
	// 代入
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fBack = fBack;
	m_pos = pos;
	m_rot = rot;

	//-------------------------------------
	//頂点バッファの作成
	//-------------------------------------
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
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
	pVtx[2].pos.y = 0.0f;
	pVtx[3].pos.y = 0.0f;
	pVtx[0].pos.z = m_fBack;
	pVtx[1].pos.z = m_fBack;
	pVtx[2].pos.z = -m_fBack;
	pVtx[3].pos.z = -m_fBack;

	m_posVtx[0] = pVtx[0].pos + m_pos;
	m_posVtx[1] = pVtx[1].pos + m_pos;
	m_posVtx[2] = pVtx[2].pos + m_pos;
	m_posVtx[3] = pVtx[3].pos + m_pos;

	//-------------------------------------
	// ベクトルの設定
	D3DXVECTOR3 vecA = pVtx[1].pos - pVtx[0].pos;
	D3DXVECTOR3 vecB = pVtx[2].pos - pVtx[0].pos;
	D3DXVECTOR3 vecC = pVtx[3].pos - pVtx[1].pos;
	D3DXVECTOR3 vecD = pVtx[0].pos - pVtx[1].pos;
	D3DXVECTOR3 vecE = pVtx[0].pos - pVtx[2].pos;
	D3DXVECTOR3 vecF = pVtx[3].pos - pVtx[2].pos;
	D3DXVECTOR3 vecG = pVtx[2].pos - pVtx[3].pos;
	D3DXVECTOR3 vecH = pVtx[1].pos - pVtx[3].pos;

	// 外積
	D3DXVec3Cross(&m_nor[0], &vecA, &vecB);
	D3DXVec3Cross(&m_nor[1], &vecC, &vecD);
	D3DXVec3Cross(&m_nor[2], &vecE, &vecF);
	D3DXVec3Cross(&m_nor[3], &vecG, &vecH);

	// 正規化
	D3DXVec3Normalize(&m_nor[0], &m_nor[0]);
	D3DXVec3Normalize(&m_nor[1], &m_nor[1]);
	D3DXVec3Normalize(&m_nor[2], &m_nor[2]);
	D3DXVec3Normalize(&m_nor[3], &m_nor[3]);

	// 法線ベクトルの設定
	pVtx[0].nor = m_nor[0];
	pVtx[1].nor = m_nor[1];
	pVtx[2].nor = m_nor[2];
	pVtx[3].nor = m_nor[3];

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

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CObject3D::Uninit(void)
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
void CObject3D::Update(void)
{
	VERTEX_3D* pVtx;		// 頂点情報へのポインタ
	
	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_posVtx[0] = pVtx[0].pos + m_pos;
	m_posVtx[1] = pVtx[1].pos + m_pos;
	m_posVtx[2] = pVtx[2].pos + m_pos;
	m_posVtx[3] = pVtx[3].pos + m_pos;

	////-------------------------------------
	////頂点処理の設定
	//pVtx[0].pos.x = m_pos.x + sinf(m_rot.y + fAngle) * fLength;
	//pVtx[0].pos.y = m_pos.y + sinf(m_rot.y + fAngle) * fLength;
	//pVtx[0].pos.z = m_pos.z + cosf(m_rot.y + fAngle) * fLength;
	//pVtx[1].pos.x = m_pos.x + sinf(m_rot.y + fAngle) * fLength;
	//pVtx[1].pos.y = m_pos.y + cosf(m_rot.y + fAngle) * fLength;
	//pVtx[1].pos.z = m_pos.z + sinf(m_rot.y + fAngle) * fLength;
	//pVtx[2].pos.x = m_pos.x + sinf(m_rot.y + fAngle) * fLength;
	//pVtx[2].pos.y = m_pos.y + cosf(m_rot.y + fAngle) * fLength;
	//pVtx[2].pos.z = m_pos.z + sinf(m_rot.y + fAngle) * fLength;
	//pVtx[3].pos.x = m_pos.x + sinf(m_rot.y + fAngle) * fLength;
	//pVtx[3].pos.y = m_pos.y + cosf(m_rot.y + fAngle) * fLength;
	//pVtx[3].pos.z = m_pos.z + sinf(m_rot.y + fAngle) * fLength;
	
	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	CDebugProc* pDebugProc = CManager::GetDebugProc();
	int nNum = pDebugProc->GetNum();
	if (nNum == pDebugProc->DEBUG_OBJECT3D)
	{
		// pDebugProc->PrintDebugProc("\n[ 3Dオブジェクト ]");
		// pDebugProc->PrintDebugProc("\n< 位置 >\n%f, %f, %f\n", m_pos.x, m_pos.y, m_pos.z);
	}
}

//=========================================================
// 描画処理
//=========================================================
void CObject3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得
	CTexture* pTexture = CManager::GetTexture();		// テクスチャの取得
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス

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
		m_pVtxBuff,							//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D));					//頂点情報構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,						//描画する最初の頂点インデックス
		2);						//プリミティブ(ポリゴン)数

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=========================================================
// 背景設定
//=========================================================
void CObject3D::SetPosition(D3DXVECTOR3 pos, float fWidth, float fHeight, float fBack)
{
	VERTEX_3D* pVtx;									//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_pos = pos;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fBack = fBack;

	//頂点座標の設定
	pVtx[0].pos.x = -m_fWidth;
	pVtx[1].pos.x = +m_fWidth;
	pVtx[2].pos.x = -m_fWidth;
	pVtx[3].pos.x = +m_fWidth;
	pVtx[0].pos.y = +m_fHeight;
	pVtx[1].pos.y = +m_fHeight;
	pVtx[2].pos.y = 0.0f;
	pVtx[3].pos.y = 0.0f;
	pVtx[0].pos.z = +m_fBack;
	pVtx[1].pos.z = +m_fBack;
	pVtx[2].pos.z = -m_fBack;
	pVtx[3].pos.z = -m_fBack;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

#if 0
//=========================================================
// ポリゴン3Dの外積・内積を使った地面との当たり判定
//=========================================================
float CObject3D::GetHeight(D3DXVECTOR3 posOld, D3DXVECTOR3 pos)
{
	VERTEX_3D* pVtx;			// 頂点情報へのポインタ
	float fHeight = 0.0f;		// 求める高さ

	//-------------------------------------
	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 vecLine12 = pVtx[1].pos - pVtx[2].pos;
	D3DXVECTOR3 vecLine30 = pVtx[0].pos - pVtx[3].pos;
	D3DXVECTOR3 vecLine1 = pVtx[2].pos - pVtx[0].pos;
	D3DXVECTOR3 vecLine2 = pVtx[0].pos - pVtx[1].pos;
	D3DXVECTOR3 vecLine3 = pVtx[1].pos - pVtx[3].pos;
	D3DXVECTOR3 vecLine4 = pVtx[3].pos - pVtx[2].pos;
	D3DXVECTOR3 vecToPos1 = pos - pVtx[0].pos;
	D3DXVECTOR3 vecToPos2 = pos - pVtx[1].pos;
	D3DXVECTOR3 vecToPos3 = pos - pVtx[3].pos;
	D3DXVECTOR3 vecToPos4 = pos - pVtx[2].pos;
	float vec12 = (vecLine12.z * vecToPos4.x) - (vecLine12.x * vecToPos4.z);
	float vec30 = (vecLine30.z * vecToPos1.x) - (vecLine30.x * vecToPos1.z);
	float vec1 = (vecLine1.z * vecToPos1.x) - (vecLine1.x * vecToPos1.z);
	float vec2 = (vecLine2.z * vecToPos2.x) - (vecLine2.x * vecToPos2.z);
	float vec3 = (vecLine3.z * vecToPos3.x) - (vecLine3.x * vecToPos3.z);
	float vec4 = (vecLine4.z * vecToPos4.x) - (vecLine4.x * vecToPos4.z);

	// プレイヤーが対象ポリゴンの内側にいる
	if (m_nor[0].y != 0.0f && m_nor[1].y != 0.0f && m_nor[2].y != 0.0f && m_nor[3].y != 0.0f)
	{
		if (vec1 < 0.0f && vec2 < 0.0f && vec3 < 0.0f && vec4 < 0.0f)
		{ // ポリゴン内にいる
			if (vec12 < 0.0f)
			{
				D3DXVECTOR3 vecPos = pos - pVtx[0].pos;
				fHeight = (-vecPos.x * m_nor[0].x - vecPos.z * m_nor[0].z) / m_nor[0].y + pVtx[0].pos.y;
			}
			else if (0.0f < vec12)
			{
				D3DXVECTOR3 vecPos = pos - pVtx[3].pos;
				fHeight = (-vecPos.x * m_nor[3].x - vecPos.z * m_nor[3].z) / m_nor[3].y + pVtx[3].pos.y;
			}
			if (vec30 < 0.0f)
			{
				D3DXVECTOR3 vecPos = pos - pVtx[2].pos;
				fHeight = (-vecPos.x * m_nor[2].x - vecPos.z * m_nor[2].z) / m_nor[2].y + pVtx[2].pos.y;
			}
			else if (0.0f < vec30)
			{
				D3DXVECTOR3 vecPos = pos - pVtx[1].pos;
				fHeight = (-vecPos.x * m_nor[1].x - vecPos.z * m_nor[1].z) / m_nor[1].y + pVtx[1].pos.y;
			}
		}
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return fHeight;
}

//=========================================================
// ポリゴン3Dの外積を使った草との当たり判定
//=========================================================
void CObject3D::Collision(D3DXVECTOR3 posOld, D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;	// 求める高さ
	float fRotDest = 0.0f;	// 目標の向き
	float fAssis = 0.0f;	// 補正係数

	D3DXVECTOR3 vecLine0 = m_posVtx[0] - m_posVtx[2];// 左
	D3DXVECTOR3 vecLine1 = m_posVtx[1] - m_posVtx[3];// 右
	D3DXVECTOR3 vecToPos0 = pos - m_posVtx[0];
	D3DXVECTOR3 vecToPos1 = pos - m_posVtx[1];
	D3DXVECTOR3 vecToPos2 = posOld - m_posVtx[0];
	D3DXVECTOR3 vecToPos3 = posOld - m_posVtx[1];
	float vec0 = (vecLine0.y * vecToPos0.x) - (vecLine0.x * vecToPos0.y);// pos
	float vec1 = (vecLine1.y * vecToPos1.x) - (vecLine1.x * vecToPos1.y);
	float vec2 = (vecLine0.y * vecToPos2.x) - (vecLine0.x * vecToPos2.y);// posOld
	float vec3 = (vecLine1.y * vecToPos3.x) - (vecLine1.x * vecToPos3.y);

	//if (m_nor[0].y != 0.0f && m_nor[1].y != 0.0f && m_nor[2].y != 0.0f && m_nor[3].y != 0.0f)
	if (m_nor[0].z != 0.0f && m_nor[1].z != 0.0f && m_nor[2].z != 0.0f && m_nor[3].z != 0.0f)
	{ // ベクトルが機能している
		
		//-------------------------------------
		// 右側から内に入った
		if (0.0f < vec3)
		{ // 前の位置が右側より外
			if (vec1 < 0.0f)
			{ // 今の位置が右より内
				m_bRight = true;
			}
		}

		// 進んだ距離に応じて向きを変える
		if (m_bRight == true)
		{
			fRotDest = D3DX_PI / 2;
			fAssis = ASSIS;
		}

		//-------------------------------------
		// 左側から内に入った
		if (vec2 < 0.0f)
		{ // 前の位置が左側より外
			if (0.0f < vec0)
			{ // 今の位置が左より内
				m_bLeft = true;
			}
		}

		// 進んだ距離に応じて向きを変える
		if (m_bLeft == true)
		{
			fRotDest = -D3DX_PI / 2;
			fAssis = ASSIS;
		}

		//-------------------------------------
		// 範囲外の時
		if (vec0 < 0.0f || 0.0f < vec1)
		{
			m_bRight = false;
			m_bLeft = false;

			if (-SWAY_WIDTH <= m_rot.z && m_rot.z <= SWAY_WIDTH)
			{
				fRotDest = Sway();
				fAssis = ASSIS_SWAY;
			}
			else
			{
				fRotDest = 0.0f;
				fAssis = ASSIS;
			}
		}
		
		float fRotDiff = fRotDest - m_rot.z;	// 目標までの差分 = 目標 - 今
		if (fRotDiff >= D3DX_PI)
		{
			fRotDiff -= D3DX_PI * 2;
		}
		if (fRotDiff <= -D3DX_PI)
		{
			fRotDiff += D3DX_PI * 2;
		}

		m_rot.z += fRotDiff * fAssis;

		if (m_rot.z >= D3DX_PI)
		{
			m_rot.z -= D3DX_PI * 2;
		}
		if (m_rot.z <= -D3DX_PI)
		{
			m_rot.z += D3DX_PI * 2;
		}
	}

	// デバック表示
	//CDebugProc* pDebugProc = CManager::GetDebugProc();
	//pDebugProc->PrintDebugProc("\n< 左 > %f\n", vec0);
}

//=========================================================
// 草を揺らす処理
//=========================================================
float CObject3D::Sway(void)
{
	// 揺れる処理
	if ((-SWAY_BIG <= m_rot.z && m_rot.z <= -SWAY_SMO) ||
		(SWAY_SMO <= m_rot.z && m_rot.z <= SWAY_BIG))
	{
		m_bSway = m_bSway ? false : true;
	}
	if (m_bSway == false)
	{
		float fRotDest = 0.3f;
		return fRotDest;
	}
	else
	{
		float fRotDest = -0.3f;
		return fRotDest;
	}
}

//=========================================================
// 位置の加算処理
//=========================================================
D3DXVECTOR3 CObject3D::AddMove(D3DXVECTOR3 move)
{
	m_pos += move;
	return m_pos;
}

//=========================================================
// 背景設定
//=========================================================
void CObject3D::SetPosition(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	VERTEX_3D* pVtx;									//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_pos = pos;
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	//頂点座標の設定
	pVtx[0].pos.x = -m_fWidth;
	pVtx[1].pos.x = +m_fWidth;
	pVtx[2].pos.x = -m_fWidth;
	pVtx[3].pos.x = +m_fWidth;
	pVtx[0].pos.y = -m_fHeight;
	pVtx[1].pos.y = -m_fHeight;
	pVtx[2].pos.y = +m_fHeight;
	pVtx[3].pos.y = +m_fHeight;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================================
// 位置設定(下の中心が頂点)
//=========================================================
void CObject3D::SetPosition(D3DXVECTOR3 pos, float fWidth, float fHeight, int nCnt)
{
	VERTEX_3D* pVtx;									//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_pos = pos;
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	//頂点座標の設定
	pVtx[0].pos.x = -m_fWidth;
	pVtx[1].pos.x = +m_fWidth;
	pVtx[2].pos.x = -m_fWidth;
	pVtx[3].pos.x = +m_fWidth;
	pVtx[0].pos.y = +m_fHeight;
	pVtx[1].pos.y = +m_fHeight;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================================
// 位置設定(それぞれを直に + 法線計算)
//=========================================================
void CObject3D::SetPosition(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3)
{
	VERTEX_3D* pVtx;									// 頂点情報へのポインタ
	
	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//-------------------------------------
	//頂点処理の設定
	pVtx[0].pos = pos0;
	pVtx[1].pos = pos1;
	pVtx[2].pos = pos2;
	pVtx[3].pos = pos3;

	m_posVtx[0] = pVtx[0].pos + m_pos;
	m_posVtx[1] = pVtx[1].pos + m_pos;
	m_posVtx[2] = pVtx[2].pos + m_pos;
	m_posVtx[3] = pVtx[3].pos + m_pos;

	//-------------------------------------
	// ベクトルの設定
	D3DXVECTOR3 vecA = pVtx[1].pos - pVtx[0].pos;
	D3DXVECTOR3 vecB = pVtx[2].pos - pVtx[0].pos;
	D3DXVECTOR3 vecC = pVtx[3].pos - pVtx[1].pos;
	D3DXVECTOR3 vecD = pVtx[0].pos - pVtx[1].pos;
	D3DXVECTOR3 vecE = pVtx[0].pos - pVtx[2].pos;
	D3DXVECTOR3 vecF = pVtx[3].pos - pVtx[2].pos;
	D3DXVECTOR3 vecG = pVtx[2].pos - pVtx[3].pos;
	D3DXVECTOR3 vecH = pVtx[1].pos - pVtx[3].pos;

	// 外積
	D3DXVec3Cross(&m_nor[0], &vecA, &vecB);
	D3DXVec3Cross(&m_nor[1], &vecC, &vecD);
	D3DXVec3Cross(&m_nor[2], &vecE, &vecF);
	D3DXVec3Cross(&m_nor[3], &vecG, &vecH);

	// 正規化
	D3DXVec3Normalize(&m_nor[0], &m_nor[0]);
	D3DXVec3Normalize(&m_nor[1], &m_nor[1]);
	D3DXVec3Normalize(&m_nor[2], &m_nor[2]);
	D3DXVec3Normalize(&m_nor[3], &m_nor[3]);

	//法線ベクトルの設定
	pVtx[0].nor = m_nor[0];
	pVtx[1].nor = m_nor[1];
	pVtx[2].nor = m_nor[2];
	pVtx[3].nor = m_nor[3];

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================================
// 色設定
//=========================================================
void CObject3D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D* pVtx;									//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================================
// テクスチャ設定
//=========================================================
void CObject3D::AddSetTexture(D3DXVECTOR2 tex)
{
	VERTEX_3D* pVtx;		// 頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex += tex;
	pVtx[1].tex += tex;
	pVtx[2].tex += tex;
	pVtx[3].tex += tex;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
#endif // 0

//=========================================================
// テクスチャの割り当て
//=========================================================
void CObject3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
