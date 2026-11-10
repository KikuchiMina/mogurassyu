//=========================================================
// 
// "地面の処理"	[field.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/field.h"
#include "../../Header/scene/game.h"
#include "../../Header/system/main.h"
#include "../../Header/system/manager.h"

//=========================================================
// マクロ定義
//=========================================================
#define RING_COUNT		120			// 円周の分割数
#define LENGTH			100.0f		// 円の外周までの長さ

//=========================================================
// 静的メンバ変数宣言
//=========================================================
LPDIRECT3DTEXTURE9 CField::m_pTexture[FIELD_TEX_MAX] = {};

//=========================================================
// オブジェクトの生成処理
//=========================================================
CField* CField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, float fBack, FIELD_TEX type)
{
	CField* pField = NULL;

	if (pField == NULL)
	{
		// オブジェクトの生成
		pField = new CField;

		if (pField != NULL)
		{
			// オブジェクトの初期化処理
			pField->Init(pos, rot, fWidth, fHeight, fBack, type);

			// テクスチャの割り当て
			pField->BindTexture(m_pTexture[type]);

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
CField::CField(PRIORITY nPriority) : CObject3D(nPriority)
{
	m_nIdxTexture = 0;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
}

//=========================================================
// デストラクタ
//=========================================================
CField::~CField()
{
}

//=========================================================
//プレイヤーのテクスチャ読み込み
//=========================================================
HRESULT CField::Load(void)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	CTexture* pTexture = CManager::GetTexture();		// テクスチャの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\brick00.jpg", &m_pTexture[FIELD_TEX_WALL_00]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\brick01.jpg", &m_pTexture[FIELD_TEX_WALL_01]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\mato00.png", &m_pTexture[FIELD_TEX_MATO_00]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\mato01.png", &m_pTexture[FIELD_TEX_MATO_01]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\mato02.png", &m_pTexture[FIELD_TEX_MATO_02]);

	//m_nIdxTexture = pTexture->Register("data\\TEXTURE\\bg000.jpeg");

	return S_OK;
}

//=========================================================
//プレイヤーのテクスチャ破棄
//=========================================================
void CField::Unload(void)
{
	//テクスチャの破棄
	for (int nCount = 0; nCount < FIELD_TEX_MAX; nCount++)
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
HRESULT CField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, float fBack, FIELD_TEX type)
{
	// 初期化
	CObject3D::Init(pos, rot, fWidth, fHeight, fBack);
	m_pos = pos;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fBack = fBack;
	m_type = type;

	// 位置の強制
	/*D3DXVECTOR3 Pos0 = D3DXVECTOR3(-fWidth, fHeight, fBack);
	D3DXVECTOR3 Pos1 = D3DXVECTOR3(fWidth, 0.0f, fBack);
	D3DXVECTOR3 Pos2 = D3DXVECTOR3(-fWidth, fHeight, -fBack);
	D3DXVECTOR3 Pos3 = D3DXVECTOR3(fWidth, 0.0f, -fBack);
	CObject3D::SetPosition(Pos0, Pos1, Pos2, Pos3);*/

	return S_OK;
}

//=========================================================
// 地面の終了処理
//=========================================================
void CField::Uninit(void)
{
	CObject3D::Uninit();
}

//=========================================================
// 地面の更新処理
//=========================================================
void CField::Update(void)
{
	if (FIELD_TEX_MATO_00 <= m_type)
	{
		CDebugProc* pDebugProc = CManager::GetDebugProc();	// デバッグへのポインタ
		pDebugProc->PrintDebugProc("\n\n\n**** フィールド ****");
		pDebugProc->PrintDebugProc("\n< 位置 > : (%f, %f, %f)", m_pos.x, m_pos.y, m_pos.z);

		// 時間制限の設定
		CObject* pObject = CObject::GetTop(PRIORITY_TIME);	// 時間へのポインタ
		int _time = 100;
		while (pObject != NULL)
		{
			CTimer* pTimer = (CTimer*)pObject;				// 時間へのポインタ
			_time = pTimer->GetTime();	// 全て消去

			CObject* pObjectNextTime = pObject->GetNext();	// 次のオブジェクトを保存
			pObject = pObjectNextTime;						// 次のオブジェクトを代入
		}

		if (_time <= 3)
		{
			int nFotCount = RING_COUNT / 60;
			for (int nCount = 0; nCount < nFotCount; nCount++)
			{
				float fRot = ((-D3DX_PI * 2) / RING_COUNT) * m_LengthCnt;
				D3DXVECTOR3 posEffect;
				posEffect.x = m_pos.x + (sinf(fRot) * LENGTH);
				posEffect.y = m_pos.y;
				posEffect.z = m_pos.z + (cosf(fRot) * LENGTH);
				D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXCOLOR col;		//頂点カラー(r,g,b,a)
				switch (_time)
				{
				case 3:
					col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		//頂点カラー(r,g,b,a)
					break;
				case 2:
					col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);		//頂点カラー(r,g,b,a)
					break;
				case 1:
					col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);		//頂点カラー(r,g,b,a)
					break;
				}

				CEffect::Create(posEffect, move, col, 5.0f, 0.01f, 60, 1); // D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fRadiusSud, int nLife
				m_LengthCnt++;
			}
		}
	}

	CObject3D::Update();
}

//=========================================================
// 地面の描画処理
//=========================================================
void CField::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

	////αブレンディングを加算合成に設定(背景の黒を消す処理)
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject3D::Draw();

	////αブレンディングを元に戻す
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}