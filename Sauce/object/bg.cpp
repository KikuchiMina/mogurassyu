//=========================================================
// 
// "背景の処理"	[bg.cpp]
// Author : KikuchiMina
// 
//=========================================================
// インクルードファイル
#include "../../Header/object/bg.h"
#include "../../Header/object/player.h"
#include "../../Header/scene/game.h"
#include "../../Header/scene/scene.h"
#include "../../Header/system/manager.h"

//=========================================================
//マクロ定義
//=========================================================
#define TEX_X			0.2f		// xの移動量
#define TEX_Y			0.5f		// yの移動量
#define TEX_DIVI		10			// 分割数
#define TEX_STEP		5			// 上下分割
#define RUN_WIDTH		60.0f		// ランニングマンの中心からの幅
#define RUN_HEIGHT		100.0f		// ランニングマンの中心からの高さ

//=========================================================
// 静的メンバ変数宣言
//=========================================================
LPDIRECT3DTEXTURE9 CBg::m_pTexture[MAX_BG_TEX] = {};
int CBg::m_nReadyCnt = 0;
int CBg::m_nStartCnt = 0;
int CBg::m_nStartFin = 0;
bool CBg::m_bStartEnd = false;
bool CBg::m_bFinishEnd = true;

//=========================================================
// オブジェクトの生成処理
//=========================================================
CBg* CBg::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, BGTEX type)
{
	CBg* pBg = NULL;

	if (pBg == NULL)
	{
		// オブジェクトの生成
		pBg = new CBg;

		if (pBg != NULL)
		{
			// オブジェクトの初期化処理
			pBg->Init(pos, fWidth, fHeight, type);

			return pBg;
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
// ゲーム開始時の処理
//=========================================================
void CBg::GameCreate(void)
{
	Create(D3DXVECTOR3(-1280.0f, 270.0f, 0.0f), 1280.0f, 180.0f, BGTEX_GAME_OO);
	Create(D3DXVECTOR3(-600.0f, 310.0f, 0.0f), 600.0f, 100.0f, BGTEX_GAME_O1);
	Create(D3DXVECTOR3(-600.0f, 310.0f, 0.0f), 600.0f, 100.0f, BGTEX_GAME_O2);
	m_bStartEnd = false;
}

//=========================================================
// ゲーム終了時の処理
//=========================================================
void CBg::GameEndCreate(void)
{
	Create(D3DXVECTOR3(-1280.0f, 270.0f, 0.0f), 1280.0f, 180.0f, BGTEX_GAME_OO);
	Create(D3DXVECTOR3(-600.0f, 310.0f, 0.0f), 600.0f, 100.0f, BGTEX_GAME_O3);
	m_bFinishEnd = false;
}

//=========================================================
// コンストラクタ
//=========================================================
CBg::CBg(PRIORITY nPriority) : CObject2D(nPriority)
{
	//m_pTexture = NULL;						// テクスチャへのポインタ
	m_pVtxBuff = NULL;							// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(500.0f, 350.0f, 0.0f);	// 位置
	m_move = D3DXVECTOR3(0.2f, 0.5f, 0.0f);		// 移動量
	m_nCounterAnime = 0;						// アニメーションカウンター
	m_nPatternAnime = 0;						// アニメーションパターンNo.
}

//=========================================================
// デストラクタ
//=========================================================
CBg::~CBg()
{
}

//=========================================================
// テクスチャ読み込み
//=========================================================
HRESULT CBg::Load(const char* TexSet)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

	CScene::MODE modeType = CScene::GetMode();

	if (modeType == CScene::MODE_GAME)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\start_bg.png", &m_pTexture[BGTEX_GAME_OO]);
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ready.png", &m_pTexture[BGTEX_GAME_O1]);
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\start.png", &m_pTexture[BGTEX_GAME_O2]);
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\finish.png", &m_pTexture[BGTEX_GAME_O3]);
		D3DXCreateTextureFromFile(pDevice, TexSet, &m_pTexture[BGTEX_GAME]);
	}
	else
	{
		D3DXCreateTextureFromFile(pDevice, TexSet, &m_pTexture[BGTEX_GAME]);
	}

	return S_OK;
}

//=========================================================
// テクスチャ破棄
//=========================================================
void CBg::Unload(void)
{
	//テクスチャの破棄
	for (int nCount = 0; nCount < MAX_BG_TEX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=========================================================
//プレイヤーの初期化処理
//=========================================================
HRESULT CBg::Init(D3DXVECTOR3 pos, float fWidth, float fHeight, BGTEX type)
{
	CObject2D::Init(pos, fWidth, fHeight);
	CObject2D::BindTexture(m_pTexture[type]);

	m_type = type;
	m_pos = pos;
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	return S_OK;
}

//=========================================================
//プレイヤーの終了処理
//=========================================================
void CBg::Uninit(void)
{
	CObject2D::Uninit();
}

//=========================================================
//プレイヤーの更新処理
//=========================================================
void CBg::Update(void)
{
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ
	CInputMouse* pMouse = CManager::GetMouse();				// マウスへのポインタ
	bool bEnd = false;	// それぞれの処理が終わったかどうか

	// ゲーム開始処理
	if (m_bStartEnd == false/* && pKeyBoaed->GetKeyboardPress(DIK_RETURN) == true*/)
	{
		bEnd = StartMove();
		if (bEnd == true)
		{
			return;
		}
	}

	// ゲーム終了処理
	if (m_bFinishEnd == false)
	{
		FinishMove();
	}

	// 位置の更新
	CObject2D::SetPosition(m_pos, m_fWidth, m_fHeight);
	CObject2D::Update();
}

//=========================================================
//プレイヤーの描画処理
//=========================================================
void CBg::Draw(void)
{
	CObject2D::Draw();
}

//=========================================================
// 開始時の移動処理
//=========================================================
bool CBg::StartMove(void)
{
	// 背景
	if (m_nStartFin <= 0)
	{
		if (m_type == BGTEX_GAME_OO)
		{
			m_pos.x += 35.0f;

			if (0.0f < m_pos.x)
			{
				m_pos.x = 0.0f;	// 位置を戻す
			}

			m_nReadyCnt++;		// カウント
		}
	}

	// 用意
	if (10 <= m_nReadyCnt && m_type == BGTEX_GAME_O1)
	{
		m_pos.x += 35.0f;

		if (340.0f < m_pos.x)
		{
			m_pos.x = 340.0f;	// 位置を戻す
			m_nStartCnt++;		// カウント
		}
	}

	// スタート
	if (80 <= m_nStartCnt)
	{
		m_nReadyCnt = 0;	// カウントを戻す(下の処理のため

		if (m_type == BGTEX_GAME_O1)
		{
			m_pos.x += 35.0f;	// 移動
			if (1880.0f < m_pos.x)
			{
				m_pos.x = 1880.0f;
				Uninit();		// 削除
			}
		}
		if (m_type == BGTEX_GAME_O2)
		{
			m_pos.x += 35.0f;	// 移動
			if (340.0f < m_pos.x)
			{
				m_pos.x = 340.0f;	// 位置を戻す
				m_nStartFin++;		// カウント
			}
		}
	}

	// 撤退
	if (10 < m_nStartFin)
	{
		m_nStartCnt = 0;			// カウントを戻す(下の処理のため
		CGame::SetPause(false);		// ポーズ状態を解除する

		if (m_type == BGTEX_GAME_OO)
		{ // 背景
			m_pos.x += 75.0f;		// 移動
			if (2560.0f < m_pos.x)
			{
				m_pos.x = -1280.0f;	// 位置を戻す

				m_nStartCnt = 0;	// カウントを戻す
				m_nReadyCnt = 0;	// カウントを戻す
				m_nStartFin = 0;	// カウントを戻す
				m_bStartEnd = true;	// 終了したにする

				// フィールド設定
				CObject* pObject = CObject::GetTop(PRIORITY_FIELD);	// フィールドへのポインタ
				while (pObject != NULL)
				{
					CField* pField = (CField*)pObject;				// 的フィールドへのポインタ
					CField::FIELD_TEX _type = pField->GetType();	// フィールドの種類

					// 的テクスチャの場合
					if (_type != CField::FIELD_TEX_WALL_00 && _type != CField::FIELD_TEX_WALL_01)
					{
						pField->Uninit();		// 削除

						// ランダム
						float posX = (-FIELD_CREATE) + (float)rand() / RAND_MAX * ((FIELD_CREATE)-(-FIELD_CREATE));		// moveX
						float posZ = (-FIELD_CREATE) + (float)rand() / RAND_MAX * ((FIELD_CREATE)-(-FIELD_CREATE));		// moveY
						int type_rand = rand() % 10;

						// 種類の設定
						if (type_rand < FIELDTYPE_NUM)
						{
							_type = CField::FIELD_TEX_MATO_00;
						}
						else if (FIELDTYPE_NUM <= type_rand && type_rand <= FIELDTYPE_NEXT)
						{
							_type = CField::FIELD_TEX_MATO_01;
						}
						else
						{
							_type = CField::FIELD_TEX_MATO_02;
						}

						// 生成
						CField::Create(D3DXVECTOR3(posX, -0.1f, posZ), D3DXVECTOR3(0.0, 0.0f, 0.0f), 100.0f, 0.0f, 100.0f, _type);
						break;
					}

					CObject* pObjectNext = pObject->GetNext();	// 次のオブジェクトを保存
					pObject = pObjectNext;						// 次のオブジェクトを代入
				}

				Uninit();
				return true;
			}
		}

		if (m_type == BGTEX_GAME_O2)
		{ // スタート
			m_pos.x += 65.0f;	// 移動
			if (1880.0f < m_pos.x)
			{
				m_pos.x = 1880.0f;
				Uninit();
				return true;
			}
		}
	}

	return false;
}

//=========================================================
// 終了時の移動処理
//=========================================================
void CBg::FinishMove(void)
{
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ
	CGame::SetPause(true);	// ポーズ状態にする

	// 背景
	if (m_nStartFin <= 0)
	{
		if (m_type == BGTEX_GAME_OO)
		{
			m_pos.x += 35.0f;

			if (0.0f < m_pos.x)
			{
				m_pos.x = 0.0f;	// 位置を戻す
			}

			m_nReadyCnt++;		// カウント
		}
	}

	// 用意
	if (10 <= m_nReadyCnt && m_type == BGTEX_GAME_O3)
	{
		m_pos.x += 35.0f;

		if (340.0f < m_pos.x)
		{
			m_pos.x = 340.0f;	// 位置を戻す
			m_nStartFin++;
		}
	}

	if (20 <= m_nStartFin && pKeyBoaed->GetKeyboardPress(DIK_RETURN) == true)
	{
		m_nStartCnt = 0;		// カウントを戻す
		m_nReadyCnt = 0;		// カウントを戻す
		m_nStartFin = 0;		// カウントを戻す

		m_bFinishEnd = true;	// 終了した

		// ゲームシーンの再読み込み
		CFade::SetFade(CScene::MODE_TITLE);
	}
}
