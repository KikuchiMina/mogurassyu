//=========================================================
// 
// "フェード処理"	[fade.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/scene/fade.h"
#include "../../Header/scene/scene.h"
#include "../../Header/system/main.h"
#include "../../Header/system/manager.h"

//=========================================================
//マクロ定義
//=========================================================
#ifdef FADE_DOWN
#define FADE_STOP		10			// フェードを一度止める時間
#define FADE_MOVE		40.0f		// move値
#define FADE_TIMEING	100			// フェードを開始するカウント
#else
#define FADE_POLYGON	0.05f		// ポリゴンの変化数(D3DXCOLOR)
#endif

#define WIDTH_PILL		190.0f		// 柱の幅
#define WIDTH_DOOR		450.0f		// 襖の幅

//=========================================================
// 静的メンバ変数宣言
//=========================================================
LPDIRECT3DTEXTURE9 CFade::m_pTexture[FADE_TEX_MAX] = {};
CFade::FADE CFade::m_fade = FADE_NONE;					// フェードの状態
CScene::MODE CFade::m_modeNext = CScene::MODE_TITLE;	// 次の画面(モード)
#ifdef FADE_DOWN
int CFade::m_OpenCnt = 0;								// カウント
bool CFade::bDoor = true;								// ドアの開閉
#else
D3DXCOLOR CFade::m_color = {};							// ポリゴン(フェード)の色
#endif

//=========================================================
// 生成処理
//=========================================================
CFade* CFade::Create(CScene::MODE mode)
{
	CFade* pFade = NULL;

	if (pFade == NULL)
	{
		// オブジェクトの生成
		pFade = new CFade;

		if (pFade != NULL)
		{
			//m_pScene = CScene::Create(mode);

			// オブジェクトの初期化処理
			pFade->Init(mode);

			return pFade;
		}
	}
	return pFade;
}

//=========================================================
// コンストラクタ
//=========================================================
CFade::CFade()
{
	m_pVtxBuff = NULL;
}

//=========================================================
// デストラクタ
//=========================================================
CFade::~CFade()
{
}

#ifdef FADE_DOWN
//=========================================================
// テクスチャ読み込み
//=========================================================
HRESULT CFade::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pill.jpg", &m_pTexture[FADE_TEX_LPILL]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pill.jpg", &m_pTexture[FADE_TEX_RPILL]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\doorL.png", &m_pTexture[FADE_TEX_LDOOR]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\doorR.png", &m_pTexture[FADE_TEX_RDOOR]);

	return S_OK;
}

//=========================================================
// テクスチャ破棄
//=========================================================
void CFade::Unload(void)
{
	//テクスチャの破棄
	for (int nCount = 0; nCount < FADE_TEX_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CFade::Init(void)
{
	return S_OK;
}
HRESULT CFade::Init(CScene::MODE modeNext)
{
	VERTEX_2D* pVtx;													// 頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * FADE_TEX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//初期位置
	m_fade = FADE_IN;			// フェードインの状態に
	m_modeNext = modeNext;		// 次の画面(モード)を設定
	m_pos[FADE_TEX_LDOOR] = D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f, 0.0f);
	m_pos[FADE_TEX_RDOOR] = D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f, 0.0f);
	m_pos[FADE_TEX_LPILL] = D3DXVECTOR3(m_pos[FADE_TEX_LDOOR].x - WIDTH_DOOR, 0.0f, 0.0f);
	m_pos[FADE_TEX_RPILL] = D3DXVECTOR3(m_pos[FADE_TEX_RDOOR].x + WIDTH_DOOR, 0.0f, 0.0f);

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < FADE_TEX_MAX; nCount++)
	{
		// 頂点処理の設定
		switch (nCount)
		{
		case FADE_TEX_LPILL:
			pVtx[0].pos.x = m_pos[nCount].x - WIDTH_PILL;
			pVtx[1].pos.x = m_pos[nCount].x;
			pVtx[2].pos.x = m_pos[nCount].x - WIDTH_PILL;
			pVtx[3].pos.x = m_pos[nCount].x;
			break;
		case FADE_TEX_RPILL:
			pVtx[0].pos.x = m_pos[nCount].x;
			pVtx[1].pos.x = m_pos[nCount].x + WIDTH_PILL;
			pVtx[2].pos.x = m_pos[nCount].x;
			pVtx[3].pos.x = m_pos[nCount].x + WIDTH_PILL;
			break;
		case FADE_TEX_LDOOR:
			pVtx[0].pos.x = m_pos[nCount].x - WIDTH_DOOR;
			pVtx[1].pos.x = m_pos[nCount].x;
			pVtx[2].pos.x = m_pos[nCount].x - WIDTH_DOOR;
			pVtx[3].pos.x = m_pos[nCount].x;
			break;
		case FADE_TEX_RDOOR:
			pVtx[0].pos.x = m_pos[nCount].x;
			pVtx[1].pos.x = m_pos[nCount].x + WIDTH_DOOR;
			pVtx[2].pos.x = m_pos[nCount].x;
			pVtx[3].pos.x = m_pos[nCount].x + WIDTH_DOOR;
			break;
		}
		pVtx[0].pos.y = m_pos[nCount].y;
		pVtx[1].pos.y = m_pos[nCount].y;
		pVtx[2].pos.y = m_pos[nCount].y + SCREEN_HEIGHT;
		pVtx[3].pos.y = m_pos[nCount].y + SCREEN_HEIGHT;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.z = 0.0f;

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 黒いポリゴン(フェード)を不透明にしておく(表示)
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CFade::Uninit(void)
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
void CFade::Update(void)
{
	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)			// 開く
		{ // フェードイン状態
			if (bDoor == true)
			{ // ドアが完全に閉まってるなら開始
				// フェード始め
				if (m_OpenCnt == 0)
				{
					m_pos[FADE_TEX_LDOOR].x -= FADE_MOVE;
					m_pos[FADE_TEX_RDOOR].x += FADE_MOVE;
				}

				// 一度止まる
				if (m_pos[FADE_TEX_LDOOR].x <= m_pos[FADE_TEX_LPILL].x)
				{ // ドアが木枠まで開いた
					m_OpenCnt++;
				}

				// 再度動く
				if (FADE_STOP <= m_OpenCnt)
				{
					m_pos[FADE_TEX_LDOOR].x -= FADE_MOVE;
					m_pos[FADE_TEX_RDOOR].x += FADE_MOVE;
					m_pos[FADE_TEX_LPILL].x -= FADE_MOVE;
					m_pos[FADE_TEX_RPILL].x += FADE_MOVE;
				}

				// ドアが完全に開いた
				if (m_pos[FADE_TEX_LDOOR].x < 0)
				{
					m_OpenCnt = 0;			// カウントを戻す
					m_fade = FADE_NONE;		// 何もしていない状態にする
					bDoor = false;
				}
			}
			else
			{
				m_fade = FADE_OUT;
			}
		}
		else if (m_fade == FADE_OUT)	// 閉じる
		{//フェードアウト状態
			if (bDoor == false)
			{ // ドアが完全に開いているなら開始
				if (FADE_TIMEING == m_OpenCnt)
				{
					m_OpenCnt = 0;			// カウントを戻す
					m_fade = FADE_IN;		// フェードイン状態にする
					bDoor = true;

					//モード設定(次の画面に移行)
					CManager::SetMode(m_modeNext);
				}
				else
				{

					// フェード始め
					if (m_OpenCnt == 0)
					{
						m_pos[FADE_TEX_LDOOR].x += FADE_MOVE;
						m_pos[FADE_TEX_RDOOR].x -= FADE_MOVE;
						m_pos[FADE_TEX_LPILL].x += FADE_MOVE;
						m_pos[FADE_TEX_RPILL].x -= FADE_MOVE;
					}

					// 一度止まる
					if (0 <= m_pos[FADE_TEX_LPILL].x - WIDTH_PILL)
					{ // ドアが木枠まで閉じた
						m_OpenCnt++;
					}

					// 再度動く
					if (FADE_STOP <= m_OpenCnt && m_OpenCnt < FADE_TIMEING)
					{
						m_pos[FADE_TEX_LDOOR].x += FADE_MOVE;
						m_pos[FADE_TEX_RDOOR].x -= FADE_MOVE;
					}

					// ドアが完全に閉じた
					if (SCREEN_WIDTH / 2 == m_pos[FADE_TEX_LDOOR].x && m_pos[FADE_TEX_RDOOR].x == SCREEN_WIDTH / 2)
					{
						// 位置の補正
						m_pos[FADE_TEX_LDOOR] = D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f, 0.0f);
						m_pos[FADE_TEX_RDOOR] = D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f, 0.0f);
						m_pos[FADE_TEX_LPILL] = D3DXVECTOR3(m_pos[FADE_TEX_LDOOR].x - WIDTH_DOOR, 0.0f, 0.0f);
						m_pos[FADE_TEX_RPILL] = D3DXVECTOR3(m_pos[FADE_TEX_RDOOR].x + WIDTH_DOOR, 0.0f, 0.0f);

						m_OpenCnt = FADE_TIMEING;
					}
				}
			}
			else
			{
				m_fade = FADE_IN;
			}
		}

		//頂点バッファをロックし、頂点情報へのポインタを獲得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCount = 0; nCount < FADE_TEX_MAX; nCount++)
		{
			// 頂点処理の設定
			switch (nCount)
			{
			case FADE_TEX_LPILL:
				pVtx[0].pos.x = m_pos[nCount].x - WIDTH_PILL;
				pVtx[1].pos.x = m_pos[nCount].x;
				pVtx[2].pos.x = m_pos[nCount].x - WIDTH_PILL;
				pVtx[3].pos.x = m_pos[nCount].x;
				break;
			case FADE_TEX_RPILL:
				pVtx[0].pos.x = m_pos[nCount].x;
				pVtx[1].pos.x = m_pos[nCount].x + WIDTH_PILL;
				pVtx[2].pos.x = m_pos[nCount].x;
				pVtx[3].pos.x = m_pos[nCount].x + WIDTH_PILL;
				break;
			case FADE_TEX_LDOOR:
				pVtx[0].pos.x = m_pos[nCount].x - WIDTH_DOOR;
				pVtx[1].pos.x = m_pos[nCount].x;
				pVtx[2].pos.x = m_pos[nCount].x - WIDTH_DOOR;
				pVtx[3].pos.x = m_pos[nCount].x;
				break;
			case FADE_TEX_RDOOR:
				pVtx[0].pos.x = m_pos[nCount].x;
				pVtx[1].pos.x = m_pos[nCount].x + WIDTH_DOOR;
				pVtx[2].pos.x = m_pos[nCount].x;
				pVtx[3].pos.x = m_pos[nCount].x + WIDTH_DOOR;
				break;
			}

			pVtx += 4;
		}

		//頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}
}

//=========================================================
// 描画処理
//=========================================================
void CFade::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,				// 頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D));		// 頂点情報構造体のサイズ

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < FADE_TEX_MAX; nCount++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture[nCount]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								nCount * 4,				//描画する最初の頂点インデックス
								2);						//プリミティブ(ポリゴン)数
	}
}

//=========================================================
// 設定
//=========================================================
void CFade::SetFade(CScene::MODE modeNext)
{
	m_fade = FADE_OUT;		// フェードの状態
	m_modeNext = modeNext;	// 次の画面(モード)
}

#else

//=========================================================
// 初期化処理
//=========================================================
HRESULT CFade::Init(void)
{
	return S_OK;
}
HRESULT CFade::Init(CScene::MODE modeNext)
{
	VERTEX_2D* pVtx;									// 頂点情報へのポインタ
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//初期位置
	m_fade = FADE_IN;								//フェードインの状態に
	m_modeNext = modeNext;							//次の画面(モード)を設定
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//黒いポリゴン(フェード)を不透明にしておく

	//頂点バッファをロックし、頂点情報へのポインタを獲得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点処理の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = m_color;
	pVtx[1].col = m_color;
	pVtx[2].col = m_color;
	pVtx[3].col = m_color;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================================
//フェードの終了処理
//=========================================================
void CFade::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=========================================================
//フェードの更新処理
//=========================================================
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{//フェードイン状態
			m_color.a -= FADE_POLYGON;		//ポリゴンを透明にしていく

			if (m_color.a <= 0.0f)
			{
				m_color.a = 0.0f;
				m_fade = FADE_NONE;				//何もしていない状態にする
			}
		}
		else if (m_fade == FADE_OUT)
		{//フェードアウト状態
			m_color.a += FADE_POLYGON;		//ポリゴンを不透明にしていく

			if (m_color.a >= 1.0f)
			{
				m_color.a = 1.0f;
				m_fade = FADE_IN;				//フェードイン状態にする

				//モード設定(次の画面に移行)
				CManager::SetMode(m_modeNext);
			}
		}

		//頂点バッファをロックし、頂点情報へのポインタを獲得
		VERTEX_2D* pVtx;						//頂点情報へのポインタ
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = m_color;
		pVtx[1].col = m_color;
		pVtx[2].col = m_color;
		pVtx[3].col = m_color;

		//頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}
}

//=========================================================
//フェードの描画処理
//=========================================================
void CFade::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,			//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D));		//頂点情報構造体のサイズ

//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);	//NULLを消すな!!!!

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,						//描画する最初の頂点インデックス
		2);						//プリミティブ(ポリゴン)数
}

//=========================================================
//フェードの設定
//=========================================================
void CFade::SetFade(CScene::MODE modeNext)
{
	m_fade = FADE_OUT;							//フェードの状態
	m_modeNext = modeNext;							//次の画面(モード)
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//黒いポリゴン(フェード)を透明にしておく
}

#endif // FADE_DOWN
