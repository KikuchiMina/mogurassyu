//=========================================================
// 
// "影の処理"	[shadow.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/shadow.h"
#include "../../Header/scene/game.h"
#include "../../Header/system/input.h"
#include "../../Header/system/main.h"
#include "../../Header/system/manager.h"

//=========================================================
// マクロ定義
//=========================================================
#define SHADOW_SIZE		10.0f	// 影の大きさ

//=========================================================
// 静的メンバ変数宣言
//=========================================================
LPDIRECT3DTEXTURE9 CShadow::m_pTexture = NULL;	// テクスチャ

//=========================================================
// オブジェクトの生成処理
//=========================================================
CShadow* CShadow::Create(D3DXVECTOR3 pos)
{
	CShadow* pShadow = NULL;

	if (pShadow == NULL)
	{
		// オブジェクトの生成
		pShadow = new CShadow;

		if (pShadow != NULL)
		{
			// オブジェクトの初期化処理
			pShadow->Init(pos);

			return pShadow;
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
CShadow::CShadow(PRIORITY nPriority) : CObject3D(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き(Z値を使用)
	m_fWidth = 0.0f;						// 幅
	m_fDepth = 0.0f;						// 奥行
}

//=========================================================
// デストラクタ
//=========================================================
CShadow::~CShadow()
{

}

//=========================================================
// テクスチャ読み込み
//=========================================================
HRESULT CShadow::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\shadow000.jpg", &m_pTexture);

	return S_OK;
}

//=========================================================
// テクスチャ破棄
//=========================================================
void CShadow::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CShadow::Init(void)
{
	return S_OK;
}
HRESULT CShadow::Init(D3DXVECTOR3 pos)
{
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

	CObject3D::BindTexture(m_pTexture);
	CObject3D::Init(m_pos, m_rot, SHADOW_SIZE, 0.0f, SHADOW_SIZE);

	return S_OK;
}

//=========================================================
// 影の終了処理
//=========================================================
void CShadow::Uninit(void)
{
	CObject3D::Uninit();
}

//=========================================================
// 更新処理
//=========================================================
void CShadow::Update(void)
{
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ
	CInputMouse* pMouse = CManager::GetMouse();				// マウスへのポインタ

	CObject3D::SetPosition(m_pos, SHADOW_SIZE, 0.0f, SHADOW_SIZE);
	CObject3D::Update();

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
void CShadow::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

	//αブレンディングを加算合成に設定(背景の黒を消す処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject3D::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=========================================================
// 位置の修正
//=========================================================
void CShadow::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
