////=========================================================
//// 
//// "メッシュオービット処理"	[meshorbit.cpp]
//// Author : KikuchiMina
//// 
////=========================================================
//#include "../../Header/object/meshorbit.h"
//#include "../../Header/system/manager.h"
//
////=========================================================
////マクロ定義
////=========================================================
//
////=========================================================
//// 静的メンバ変数宣言
////=========================================================
//
////=========================================================
//// オブジェクトの生成処理
////=========================================================
//CMeshOrbit* CMeshOrbit::Create(D3DXVECTOR3 pos)
//{
//	CMeshOrbit* pMeshOrbit = NULL;
//
//	if (pMeshOrbit == NULL)
//	{
//		// オブジェクトの生成
//		pMeshOrbit = new CMeshOrbit;
//
//		if (pMeshOrbit != NULL)
//		{
//			//// オブジェクトの初期化処理
//			//pObject3D->SetPosition(pos, fWidth, fHeight);
//			//pObject3D->SetMove(move);
//			//pObject3D->SetRot(rot);
//			pMeshOrbit->Init();
//
//			return pMeshOrbit;
//		}
//		else
//		{
//			return NULL;
//		}
//	}
//	else
//	{
//		return NULL;
//	}
//}
//
////=========================================================
//// コンストラクタ
////=========================================================
//CMeshOrbit::CMeshOrbit(PRIORITY nPriority):CObject(nPriority)
//{
//	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
//	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
//}
//
//
////=========================================================
//// デストラクタ
////=========================================================
//CMeshOrbit::~CMeshOrbit()
//{
//}
//
////=========================================================
//// 初期化処理
////=========================================================
//HRESULT CMeshOrbit::Init(void)
//{
//	return 0;
//}
//HRESULT CMeshOrbit::Init(D3DXVECTOR3 pos)
//{
//	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();;	// デバイスへのポインタの取得
//	VERTEX_3D* pVtx;									// 頂点情報へのポインタ
//
//	//-------------------------------------
//	// 代入
//
//	//-------------------------------------
//	//頂点バッファの作成
//	//-------------------------------------
//	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_3D,
//		D3DPOOL_MANAGED,
//		&m_pVtxBuff,
//		NULL);
//
//	//頂点バッファをロックし、頂点情報へのポインタを獲得
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	//-------------------------------------
//	//頂点処理の設定
//	pVtx[0].pos.x = -m_fWidth;
//	pVtx[1].pos.x = m_fWidth;
//	pVtx[2].pos.x = -m_fWidth;
//	pVtx[3].pos.x = m_fWidth;
//	pVtx[0].pos.y = m_fHeight;
//	pVtx[1].pos.y = m_fHeight;
//	pVtx[2].pos.y = 0.0f;
//	pVtx[3].pos.y = 0.0f;
//	pVtx[0].pos.z = m_fBack;
//	pVtx[1].pos.z = m_fBack;
//	pVtx[2].pos.z = -m_fBack;
//	pVtx[3].pos.z = -m_fBack;
//
//	m_posVtx[0] = pVtx[0].pos + m_pos;
//	m_posVtx[1] = pVtx[1].pos + m_pos;
//	m_posVtx[2] = pVtx[2].pos + m_pos;
//	m_posVtx[3] = pVtx[3].pos + m_pos;
//
//	//-------------------------------------
//	// ベクトルの設定
//	D3DXVECTOR3 vecA = pVtx[1].pos - pVtx[0].pos;
//	D3DXVECTOR3 vecB = pVtx[2].pos - pVtx[0].pos;
//	D3DXVECTOR3 vecC = pVtx[3].pos - pVtx[1].pos;
//	D3DXVECTOR3 vecD = pVtx[0].pos - pVtx[1].pos;
//	D3DXVECTOR3 vecE = pVtx[0].pos - pVtx[2].pos;
//	D3DXVECTOR3 vecF = pVtx[3].pos - pVtx[2].pos;
//	D3DXVECTOR3 vecG = pVtx[2].pos - pVtx[3].pos;
//	D3DXVECTOR3 vecH = pVtx[1].pos - pVtx[3].pos;
//
//	// 外積
//	D3DXVec3Cross(&m_nor[0], &vecA, &vecB);
//	D3DXVec3Cross(&m_nor[1], &vecC, &vecD);
//	D3DXVec3Cross(&m_nor[2], &vecE, &vecF);
//	D3DXVec3Cross(&m_nor[3], &vecG, &vecH);
//
//	// 正規化
//	D3DXVec3Normalize(&m_nor[0], &m_nor[0]);
//	D3DXVec3Normalize(&m_nor[1], &m_nor[1]);
//	D3DXVec3Normalize(&m_nor[2], &m_nor[2]);
//	D3DXVec3Normalize(&m_nor[3], &m_nor[3]);
//
//	// 法線ベクトルの設定
//	pVtx[0].nor = m_nor[0];
//	pVtx[1].nor = m_nor[1];
//	pVtx[2].nor = m_nor[2];
//	pVtx[3].nor = m_nor[3];
//
//	// 頂点カラーの設定
//	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//	// テクスチャ座標の設定
//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//	//頂点バッファをアンロック
//	m_pVtxBuff->Unlock();
//
//	return S_OK;
//}
//
////=========================================================
//// 終了処理
////=========================================================
//void CMeshOrbit::Uninit(void)
//{
//	//頂点バッファの破棄
//	if (m_pVtxBuff != NULL)
//	{
//		m_pVtxBuff->Release();
//		m_pVtxBuff = NULL;
//	}
//
//	// オブジェクト(自分自身)の破棄
//	CObject::Release();
//}
//
////=========================================================
//// 更新処理
////=========================================================
//void CMeshOrbit::Update(void)
//{
//	
//}
//
////=========================================================
//// 描画処理
////=========================================================
//void CMeshOrbit::Draw(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得
//
//	//-------------------------------------
//	//頂点バッファをデータストリームに設定
//	//-------------------------------------
//	pDevice->SetStreamSource(0,
//		m_pVtxBuff,							//頂点バッファへのポインタ
//		0,
//		sizeof(VERTEX_3D));					//頂点情報構造体のサイズ
//
//	//頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	////Zテストを無効にする
//	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
//	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//
//	//ポリゴンの描画
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
//		0,						//描画する最初の頂点インデックス
//		2);						//プリミティブ(ポリゴン)数
//
//	////Zテストを有効にする
//	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
//	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
//}
