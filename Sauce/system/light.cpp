//=========================================================
// 
// "ライトの処理"	[light.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/system/light.h"
#include "../../Header/system/main.h"
#include "../../Header/system/manager.h"

#define DIRECTOPN0	(D3DXVECTOR3(0.2f, -0.8f, 0.4f))		// ライトの方向
#define DIRECTOPN1	(D3DXVECTOR3(-0.2f, 0.9f, -0.4f))		// ライトの方向
#define DIRECTOPN2	(D3DXVECTOR3(0.9f, 0.1f, 0.4f))			// ライトの方向

//=========================================================
// コンストラクタ
//=========================================================
CLight::CLight()
{
	m_vecDir = {};
}

//=========================================================
// デストラクタ
//=========================================================
CLight::~CLight()
{
}

//=========================================================
// カメラの初期化処理
//=========================================================
void CLight::Init(void)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	//-------------------------------------
	//ライトの情報をクリアする
	//-------------------------------------
	ZeroMemory(&m_aLight[0], sizeof(D3DLIGHT9));

	//ライトの種類を設定
	m_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	m_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトの方向を設定
	m_vecDir = D3DXVECTOR3(0.4f, -0.8f, 0.4f);
	m_vecDir = DIRECTOPN0;
	//m_vecDir = D3DXVECTOR3(0.5f, -0.4f, -0.8f);
	D3DXVec3Normalize(&m_vecDir, &m_vecDir);			//ベクトルを正規化する

	m_aLight[0].Direction = m_vecDir;

	//ライトを設定する
	pDevice->SetLight(0, &m_aLight[0]);

	//ライトを有効にする
	pDevice->LightEnable(0, TRUE);

	//-------------------------------------
	//ライトの情報をクリアする
	//-------------------------------------
	ZeroMemory(&m_aLight[1], sizeof(D3DLIGHT9));

	//ライトの種類を設定
	m_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	m_aLight[1].Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	//ライトの方向を設定
	m_vecDir = D3DXVECTOR3(-0.4f, -0.8f, -0.4f);
	m_vecDir = DIRECTOPN1;
	D3DXVec3Normalize(&m_vecDir, &m_vecDir);			//ベクトルを正規化する

	m_aLight[1].Direction = m_vecDir;

	//ライトを設定する
	pDevice->SetLight(1, &m_aLight[1]);

	//ライトを有効にする
	pDevice->LightEnable(1, TRUE);

	//-------------------------------------
	//ライトの情報をクリアする
	//-------------------------------------
	ZeroMemory(&m_aLight[2], sizeof(D3DLIGHT9));

	//ライトの種類を設定
	m_aLight[2].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	m_aLight[2].Diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);

	//ライトの方向を設定
	m_vecDir = D3DXVECTOR3(-0.0f, -0.8f, 0.4f);
	m_vecDir = DIRECTOPN2;
	D3DXVec3Normalize(&m_vecDir, &m_vecDir);			//ベクトルを正規化する

	m_aLight[2].Direction = m_vecDir;

	//ライトを設定する
	pDevice->SetLight(2, &m_aLight[2]);

	//ライトを有効にする
	pDevice->LightEnable(2, TRUE);
}

//=========================================================
// カメラの終了処理
//=========================================================
void CLight::Uninit(void)
{

}

//=========================================================
// カメラの更新処理
//=========================================================
void CLight::Update(void)
{
	CRenderer* renderer = CManager::GetRenderer();			// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();		// デバイスへのポインタの取得
	CDebugProc* pDebugProc = CManager::GetDebugProc();		// デバッグへのポインタの取得
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ

	//-------------------------------------
	// デバック
	int nNum = pDebugProc->GetNum();
	if (nNum == pDebugProc->DEBUG_LIGHT)
	{
		//-------------------------------------
		// 移動
		if (pKeyBoaed->GetKeyboardTrigger(DIK_Q) == true)
		{
			m_vecDir.x += 0.1f;

			// ライトの方向を設定
			D3DXVec3Normalize(&m_vecDir, &m_vecDir);			//ベクトルを正規化する
		}
		if (pKeyBoaed->GetKeyboardTrigger(DIK_A) == true)
		{
			m_vecDir.x -= 0.1f;

			// ライトの方向を設定
			D3DXVec3Normalize(&m_vecDir, &m_vecDir);			//ベクトルを正規化する
		}
		if (pKeyBoaed->GetKeyboardTrigger(DIK_W) == true)
		{
			m_vecDir.y += 0.1f;

			// ライトの方向を設定
			D3DXVec3Normalize(&m_vecDir, &m_vecDir);			//ベクトルを正規化する
		}
		if (pKeyBoaed->GetKeyboardTrigger(DIK_S) == true)
		{
			m_vecDir.y -= 0.1f;

			// ライトの方向を設定
			D3DXVec3Normalize(&m_vecDir, &m_vecDir);			//ベクトルを正規化する
		}
		if (pKeyBoaed->GetKeyboardTrigger(DIK_E) == true)
		{
			m_vecDir.z += 0.1f;

			// ライトの方向を設定
			D3DXVec3Normalize(&m_vecDir, &m_vecDir);			//ベクトルを正規化する
		}
		if (pKeyBoaed->GetKeyboardTrigger(DIK_D) == true)
		{
			m_vecDir.z -= 0.1f;

			// ライトの方向を設定
			D3DXVec3Normalize(&m_vecDir, &m_vecDir);			//ベクトルを正規化する
		}

		m_aLight[0].Direction = m_vecDir;

		//-------------------------------------
		// デバック表示
		pDebugProc->PrintDebugProc("\n< ライトの方向 >\n%f, %f, %f\n", m_vecDir.x, m_vecDir.y, m_vecDir.z);
	}
}