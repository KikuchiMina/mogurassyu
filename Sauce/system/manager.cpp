//=========================================================
// 
// "マネージャーの処理"	[manager.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/object.h"
#include "../../Header/object/objectX.h"
#include "../../Header/scene/title.h"
#include "../../Header/scene/game.h"
#include "../../Header/system/manager.h"

//=========================================================
// マクロ定義
//=========================================================
#define RUN_WIDTH		50.0f		// プレイヤーの中心からの幅
#define RUN_HEIGHT		50.0f		// プレイヤーの中心からの高さ
#define SOUND
#undef SOUND

//=========================================================
// 静的メンバ変数宣言
//=========================================================
//=== 2Dobject ===//
CTexture* CManager::m_pTexture = NULL;
//CNumber * CManager::m_pNumber = NULL;
//CScore * CManager::m_pScore = NULL;
//CTimer* CManager::m_pTimer = NULL;

//=== 3Dobject ===//
CCamera* CManager::m_pCamera = NULL;
CLight* CManager::m_pLight = NULL;
CObject3D* CManager::m_pObject3D = NULL;
//CField* CManager::m_pField = NULL;
//CWirePoint* CManager::m_pWirePoint = NULL;
//CBillboard* CManager::m_pBillboard = NULL;
//CPlayer* CManager::m_pPlayer = NULL;
//CEnemy* CManager::m_pEnemy = NULL;
//CGrass* CManager::m_pGrass = NULL;
//CMeshField* CManager::m_pMeshField = NULL;
//CMeshBg* CManager::m_pMeshBg = NULL;
//CBullet* CManager::m_pBullet = NULL;

//=== system ===//
CInput* CManager::m_pInput = NULL;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CInputMouse* CManager::m_pInputMouse = NULL;
CInputJoypad* CManager::m_pInputJoypad = NULL;
CRenderer* CManager::m_pRenderer = NULL;
CDebugProc* CManager::m_pDebugProc = NULL;
CScript* CManager::m_pScript = NULL;
CScene* CManager::m_pScene = NULL;
CFade* CManager::m_pFade = NULL;
CSound* CManager::m_pSound = NULL;
CPause* CManager::m_pPause;

//=== バックグラウンド ===//
LPDIRECT3DTEXTURE9 CManager::m_background = NULL;
LPD3DXSPRITE CManager::m_sprite = NULL;

bool CManager::m_bPause = false;
HINSTANCE CManager::m_hInstance = NULL;
HWND CManager::m_hWnd = NULL;
BOOL CManager::m_bWindow = NULL;

//=========================================================
// コンストラクタ
//=========================================================
CManager::CManager()
{
	m_nCountFPS = 0;			//FPSカウンタ
}

//=========================================================
// デストラクタ
//=========================================================
CManager::~CManager()
{
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	srand((unsigned int)time(NULL));

	// 保存
	m_hInstance = hInstance;
	m_hWnd = hWnd;
	m_bWindow = bWindow;

	// レンダラーの生成
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;

		if (m_pRenderer != NULL)
		{
			// レンダラーの初期化処理
			if (FAILED(m_pRenderer->Init(hInstance, hWnd, TRUE)))
			{ // 初期化が失敗した場合
				return -1;
			}
		}
	}

	// バックグラウンドを描画
	HRESULT hr = LoadBackground();
	if (FAILED(hr))
	{
		// エラー処理
	}

	// キーボードの生成
	if (m_pInputKeyboard == NULL)
	{
		m_pInputKeyboard = new CInputKeyboard;

		if (m_pInputKeyboard != NULL)
		{
			// キーボードの初期化処理
			if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd, TRUE)))
			{
				MessageBox(hWnd, "InitKeyboardに失敗しました。", "Error (0)", MB_ICONERROR);
				return E_FAIL;
			}
		}
	}

	// マウスの生成
	if (m_pInputMouse == NULL)
	{
		m_pInputMouse = new CInputMouse;

		if (m_pInputMouse != NULL)
		{
			// マウスの初期化処理
			if (FAILED(m_pInputMouse->Init(hInstance, hWnd, TRUE)))
			{
				MessageBox(hWnd, "InitKeyboardに失敗しました。", "Error (0)", MB_ICONERROR);
				return E_FAIL;
			}
		}
	}

	// ジョイパッドの生成
	if (m_pInputJoypad == NULL)
	{
		m_pInputJoypad = new CInputJoypad;

		if (m_pInputJoypad != NULL)
		{
			// マウスの初期化処理
			if (FAILED(m_pInputJoypad->Init(hInstance, hWnd, TRUE)))
			{
				MessageBox(hWnd, "InitKeyboardに失敗しました。", "Error (0)", MB_ICONERROR);
				return E_FAIL;
			}
		}
	}

	// デバックの生成
	if (m_pDebugProc == NULL)
	{
		m_pDebugProc = new CDebugProc;

		if (m_pDebugProc != NULL)
		{
			// デバックの初期化処理
			m_pDebugProc->Init();
		}
	}

	// カメラの生成
	if (m_pCamera == NULL)
	{
		m_pCamera = new CCamera;

		if (m_pCamera != NULL)
		{
			// カメラの初期化処理
			m_pCamera->Init();
		}
	}

	// ライトの生成
	if (m_pLight == NULL)
	{
		m_pLight = new CLight;

		if (m_pLight != NULL)
		{
			// ライトの初期化処理
			m_pLight->Init();
		}
	}

	// サウンドの読み込み
	m_pSound->Load();

#ifdef SOUND
	// サウンドの生成
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;

		if (m_pSound != NULL)
		{
			// キーボードの初期化処理
			if (FAILED(m_pSound->Init(hWnd)))
			{
				MessageBox(hWnd, "InitSoundに失敗しました。", "Error (0)", MB_ICONERROR);
				return E_FAIL;
			}
		}
	}
#endif // SOUND

	//// ポーズの生成
	//m_pPause->Load();	// テクスチャ読み込み
	//if (m_pPause == NULL)
	//{
	//	m_pPause = new CPause;
	//
	//	if (m_pPause != NULL)
	//	{
	//		// ポーズの初期化処理
	//		m_pPause->Init();
	//	}
	//}

	//// テクスチャの生成
	//if (m_pTexture == NULL)
	//{
	//	m_pTexture = new CTexture;
	//
	//	if (m_pTexture != NULL)
	//	{
	//		//デバックの初期化処理
	//		m_pTexture->Load();
	//	}
	//}

	// 全てのオブジェクトの破棄
	CObject::ReleaseAll();

	// シーンの生成
	m_pScene = CScene::Create(CScene::MODE_GAME);

#ifdef FADE_DOWN
	// フェードのテクスチャ読み込み
	m_pFade->Load();
#endif // FADE_DOWN

	// フェードの生成
	m_pFade = CFade::Create(CScene::MODE_GAME);

	//// ジョイパッドの初期化処理
	//if (FAILED(InitJoypad()))
	//{
	//	MessageBox(hWnd, "InitJoypadに失敗しました。", "Error (1)", MB_ICONERROR);
	//	return E_FAIL;
	//}
	//
	//// サウンドの初期化処理
	//if (FAILED(InitSound(hWnd)))
	//{
	//	MessageBox(hWnd, "InitSoundに失敗しました。", "Error (3)", MB_ICONERROR);
	//	return E_FAIL;
	//}

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CManager::Uninit(void)
{
	//// シーンの破棄
	//if (m_pScene != NULL)
	//{
	//	m_pScene->Uninit();
	//	/*delete m_pScene;
	//	m_pScene = NULL;*/		// ReleaseAllで消すから、deleteとNULLはしなくていい
	//}

	// 全てのオブジェクトの破棄(+終了処理)
	CObject::ReleaseAll();

	//// テクスチャの破棄
	//if (m_pTexture != NULL)
	//{
	//	// テクスチャの終了処理
	//	m_pTexture->Unload();
	//	delete m_pTexture;
	//	m_pTexture = NULL;
	//}

#ifdef FADE_DOWN
	// フェードのテクスチャ破棄
	m_pFade->Unload();
#endif // FADE_DOWN

	// ポーズの破棄
	//if (m_pPause != NULL)
	//{
	//	// ライトの終了処理
	//	m_pPause->Uninit();
	//	delete m_pPause;
	//	m_pPause = NULL;
	//}
	//m_pPause->Unload();	// テクスチャ破棄

	// サウンドの破棄
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	// ライトの破棄
	if (m_pLight != NULL)
	{
		// ライトの終了処理
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	// ライトの破棄
	if (m_pLight != NULL)
	{
		// ライトの終了処理
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	// カメラの破棄
	if (m_pCamera != NULL)
	{
		// カメラの終了処理
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//デバックの破棄
	if (m_pDebugProc != NULL)
	{
		// デバックの終了処理
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	// ジョイパッドの破棄
	if (m_pInputJoypad != NULL)
	{
		// マウスの終了処理
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	// マウスの破棄
	if (m_pInputMouse != NULL)
	{
		// マウスの終了処理
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// キーボードの破棄
	if (m_pInputKeyboard != NULL)
	{
		// キーボードの終了処理
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// レンダラーの破棄
	if (m_pRenderer != NULL)
	{
		// レンダラーの終了処理
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// バックグラウンド用テクスチャデータの破棄
	if (m_sprite != NULL)
	{
		m_sprite->Release();
		m_sprite = NULL;
	}
	if (m_background != NULL)
	{
		m_background->Release();
		m_background = NULL;
	}
}

//=========================================================
// 更新処理
//=========================================================
void CManager::Update(void)
{
	// シーンの更新処理
	//m_pScene->Update();

	//-------------------------------------
	// デバック表示
	CDebugProc* pDebugProc = CManager::GetDebugProc();
	int nNum = CDebugProc::GetNum();
	//pDebugProc->PrintDebugProc("**** シーン切り替え ****\nF2,ゲーム  F3,タイトル  F4,クリア  F5,ゲームオーバー\n\n");
	//pDebugProc->PrintDebugProc("**** モード切り替え  %d / %d ****\n0, プレイヤー 1,カメラ 2, ライト 3~5, オブジェクトX-2D-3D\n", nNum, CDebugProc::DEBUG_MAX);

	// マウスの更新処理
	m_pInputMouse->Update();

	// キーボードの更新処理
	m_pInputKeyboard->Update();

	// ジョイパッドの更新処理
	m_pInputJoypad->Update();

	//デバックの更新処理
	m_pDebugProc->Update();

	// カメラの更新処理
	m_pCamera->Update();

	// シーンの更新処理
	m_pScene->Update();

	//-------------------------------------
	// ポーズ状態のON/OFFを切り替え
	if (m_pInputKeyboard->GetKeyboardTrigger(DIK_P) == true)
	{ // Pキーが押された
		m_bPause = m_bPause ? false : true;
	}
	if (m_bPause == true)
	{ // ポーズ状態がONの時
		//m_pPause->Update();
	}
	else
	{ // ポーズ状態がOFFの時
		// ライトの更新処理
		m_pLight->Update();

		// レンダラーの更新処理
		m_pRenderer->Update();
	}

	// シーンの変更
	if (m_pInputKeyboard->GetKeyboardTrigger(DIK_F2) == true)
	{
		m_pFade->SetFade(CScene::MODE_GAME);
	}
	if (m_pInputKeyboard->GetKeyboardTrigger(DIK_F3) == true)
	{
		m_pFade->SetFade(CScene::MODE_TITLE);
	}
	if (m_pInputKeyboard->GetKeyboardTrigger(DIK_F4) == true)
	{
		m_pFade->SetFade(CScene::MODE_CLEAR);
	}
	if (m_pInputKeyboard->GetKeyboardTrigger(DIK_F5) == true)
	{
		m_pFade->SetFade(CScene::MODE_OVER);
	}

	// フェードの更新処理
	m_pFade->Update();
}

//=========================================================
// 描画処理
//=========================================================
void CManager::Draw(void)
{
	if (m_bPause == true)
	{ // ポーズ状態がONの時
		//m_pPause->Draw();
	}
	else
	{ // ポーズ状態がOFFの時

		// シーンの描画処理
		m_pScene->Draw();

		// レンダラーの描画処理
		m_pRenderer->Draw();		// お前いなきゃいけない存在だよ
	}
}

//==================================================================================================================
// 画面(モード)遷移処理
//==================================================================================================================
void CManager::SetMode(CScene::MODE mode)
{
	// サウンドの停止

	// 現在のモードの破棄
	if (m_pScene != NULL)
	{
		m_pScene->Uninit();
	}

	// 全てのオブジェクトの破棄
	CObject::ReleaseAll();
	m_pScene = NULL;

	// 新しいシーンの生成
	m_pScene = CScene::Create(mode);
}

//=========================================================
// バックグラウンドテクスチャの読み込み
//=========================================================
HRESULT CManager::LoadBackground(void)
{ // Direct3D 9のデバイスを初期化した後に呼ぶ
	LPDIRECT3DDEVICE9 pDevice = GetRenderer()->GetDevice();	// デバイスへのポインタの取得
	HRESULT hr;

#if 1
	hr = D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\soilLoop.png", &m_background);
#else
	hr = D3DXCreateTextureFromFile(pDevice, NULL, &m_background);
#endif // 0

	// 読み込めているかどうか
	if (FAILED(hr))
	{
		return hr;
	}

	hr = D3DXCreateSprite(pDevice, &m_sprite);

	return hr;
}
