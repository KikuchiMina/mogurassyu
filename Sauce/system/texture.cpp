//=========================================================
// 
// "テクスチャの処理"	[texture.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/system/texture.h"
#include "../../Header/system/manager.h"

//=========================================================
// 静的メンバ変数宣言
//=========================================================
int CTexture::m_nNumAll = 0;

//=========================================================
// コンストラクタ
//=========================================================
CTexture::CTexture()
{
	m_nNumAll = 0;
	m_apTexture[NUM_TEX] = { NULL };
}

//=========================================================
// デストラクタ
//=========================================================
CTexture::~CTexture()
{
}

//=========================================================
// テクスチャの読み込み
//=========================================================
HRESULT CTexture::Load(void)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bg000.jpeg", &m_apTexture[TEX_FIELD]);

	return S_OK;
}

//=========================================================
// テクスチャの破棄
//=========================================================
void CTexture::Unload(void)
{
	for (int nCount = 0; nCount < NUM_TEX; nCount++)
	{
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//=========================================================
// テクスチャの登録処理
//=========================================================
int CTexture::Register(const char* pFilename)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	for (int nCount = 0; nCount < NUM_TEX; nCount++)
	{
		if (m_apTexture[nCount] == NULL)
		{
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pFilename, &m_apTexture[m_nNumAll]);
			break;
		}
		m_nNumAll++;	// カウントアップ
	}

	return m_nNumAll;
}

//=========================================================
// テクスチャのアドレス取得
//=========================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	return m_apTexture[nIdx];
}
