//=========================================================
// 
// "ポーズ処理"	[pause.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/timer.h"
#include "../../Header/scene/fade.h"
#include "../../Header/scene/game.h"
#include "../../Header/scene/pause.h"
#include "../../Header/system/input.h"
#include "../../Header/system/sound.h"

//=========================================================
//マクロ定義
//=========================================================
#define PAUSE_WIDTH		100.0f		// 幅
#define PAUSE_HEIGHT	100.0f		// 高さ

//=========================================================
// 静的メンバ変数宣言
//=========================================================
LPDIRECT3DTEXTURE9 CPause::m_apTexture[PAUSE_MENU_MAX] = {};
D3DXVECTOR3 CPause::m_pos[PAUSE_MENU_MAX] = {};
CPause::PAUSE_MENU CPause::m_type = PAUSE_MENU_RESTART;
int CPause::m_nData = 0;

//=========================================================
// オブジェクトの生成処理(背景)
//=========================================================
CPause* CPause::Create()
{
	CPause* pPause = NULL;

	if (pPause == NULL)
	{
		// オブジェクトの生成
		pPause = new CPause;

		if (pPause != NULL)
		{
			// オブジェクトの初期化処理
			pPause->Init();

			return pPause;
		}
	}
	return pPause;
}


//=========================================================
// コンストラクタ
//=========================================================
//CPause::CPause(PRIORITY nPriority) :CObject2D(nPriority)
CPause::CPause()
{
	m_pVtxBuff = NULL;					// 頂点バッファへのポインタ
}

//=========================================================
// デストラクタ
//=========================================================
CPause::~CPause()
{
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	m_nData = 0;
}

//=========================================================
// テクスチャ読み込み
//=========================================================
HRESULT CPause::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Pouse Restart.png", &m_apTexture[PAUSE_MENU_RESTART]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Pouse StageSelect.png", &m_apTexture[PAUSE_MENU_STAGESELECT]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Pouse Retitle.png", &m_apTexture[PAUSE_MENU_RETITLE]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Pouse Close.png", &m_apTexture[PAUSE_MENU_CLOSE]);

	return S_OK;
}

//=========================================================
// テクスチャ破棄
//=========================================================
void CPause::Unload(void)
{
	//テクスチャの破棄
	for (int nCount = 0; nCount < PAUSE_MENU_MAX; nCount++)
	{
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CPause::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得
	VERTEX_2D* pVtx;								//頂点情報へのポインタ

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4/* * PAUSE_MENU_MAX*/,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_pos[0] = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	//頂点処理の設定
	pVtx[0].pos.x = m_pos[0].x;
	pVtx[1].pos.x = m_pos[0].x + PAUSE_WIDTH;
	pVtx[2].pos.x = m_pos[0].x;
	pVtx[3].pos.x = m_pos[0].x + PAUSE_WIDTH;
	pVtx[0].pos.y = m_pos[0].y;
	pVtx[1].pos.y = m_pos[0].y;
	pVtx[2].pos.y = m_pos[0].y + PAUSE_HEIGHT;
	pVtx[3].pos.y = m_pos[0].y + PAUSE_HEIGHT;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	/*for (int nCount = 0; nCount < PAUSE_MENU_MAX; nCount++)
	{
		//位置の設定
		pVtx[0].pos = D3DXVECTOR3(100.0f, nCount * 50.0f + 100.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(200.0f, nCount * 50.0f + 100.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(100.0f, nCount * 50.0f + 200.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(200.0f, nCount * 50.0f + 200.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;
	}*/

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CPause::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=========================================================
// 更新処理
//=========================================================
void CPause::Update(void)
{
	VERTEX_2D* pVtx;										// 頂点情報へのポインタ
	CFade::FADE pFade = CManager::GetFade()->GetFade();		// フェード情報の取得
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ
	CInputMouse* pMouse = CManager::GetMouse();				// マウスへのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標を進める
	pVtx += m_nData * 4;
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//W・Sキーが押された
	if (m_nData >= -1 || m_nData <= PAUSE_MENU_MAX)
	{
		if (/*pKeyBoaed->GetJoypadRepeat(JOYKEY_UP) == true || */
			pKeyBoaed->GetKeyboardRepeat(DIK_W) == true || pKeyBoaed->GetKeyboardRepeat(DIK_UP) == true)
		{//Wキーが押された
			m_nData--;

			if (m_nData < PAUSE_MENU_RESTART)
			{
				m_nData = PAUSE_MENU_CLOSE;
			}
		}
		if (/*pKeyBoaed->GetJoypadRepeat(JOYKEY_DOWN) == true || */
			pKeyBoaed->GetKeyboardRepeat(DIK_S) == true || pKeyBoaed->GetKeyboardRepeat(DIK_DOWN) == true)
		{//Sキーが押された
			m_nData++;

			if (PAUSE_MENU_CLOSE < m_nData)
			{
				m_nData = PAUSE_MENU_RESTART;
			}
		}

		//頂点バッファをロックし、頂点情報へのポインタを獲得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標を進める
		pVtx += m_nData * 4;
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}

	// Pキーが押された
	if (/*pKeyBoaed->GetJoypadTrigger(JOYKEY_BACK) == true || */pKeyBoaed->GetKeyboardTrigger(DIK_P) == true)
	{
		//頂点バッファをロックし、頂点情報へのポインタを獲得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
			pVtx += 4;
		}
		m_nData = 0;

		//頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}

	// ENTERキーが押された
	if (/*pKeyBoaed->GetJoypadPress(JOYKEY_A) == true || */pKeyBoaed->GetKeyboardPress(DIK_RETURN) == true)
	{
		switch (m_nData)
		{
		case PAUSE_MENU_RESTART:
			CFade::SetFade(CScene::MODE_GAME);
			break;
		case PAUSE_MENU_STAGESELECT:
			break;
		case PAUSE_MENU_RETITLE:
			CFade::SetFade(CScene::MODE_TITLE);
			break;
		case PAUSE_MENU_CLOSE:
			//SetEnablePause(false);
			break;
		}
	}
}

//=========================================================
//ポーズの描画処理
//=========================================================
void CPause::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,				// 頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D));		// 頂点情報構造体のサイズ

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_apTexture[0]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,						// 描画する最初の頂点インデックス
		2);						// プリミティブ(ポリゴン)数

	/*for (int nCount = 0; nCount < PAUSE_MENU_MAX; nCount++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, m_apTexture[nCount]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			nCount * 4,				// 描画する最初の頂点インデックス
			2);						// プリミティブ(ポリゴン)数
	}*/
}

//=========================================================
// 移動処理
//=========================================================
void UpdateMove(void)
{
	//VERTEX_2D* pVtx;								//頂点情報へのポインタ

	////頂点バッファをロックし、頂点情報へのポインタを獲得
	//g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	//for (int nCntPouse = 0; nCntPouse < MAX_POUSE; nCntPouse++)
	//{
	//	if (pVtx[0].pos.x >= 600.0f)
	//	{
	//		//位置の設定
	//		pVtx[0].pos.x -= 40.0f;
	//		pVtx[1].pos.x -= 40.0f;
	//		pVtx[2].pos.x -= 40.0f;
	//		pVtx[3].pos.x -= 40.0f;
	//	}
	//	pVtx += 4;
	//}

	////頂点バッファをアンロック
	//g_pVtxBuffPause->Unlock();
}
