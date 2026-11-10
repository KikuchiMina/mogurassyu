//=========================================================
// 
// "レンダラーの処理"	[renderer.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/object.h"
#include "../../Header/system/renderer.h"
#include "../../Header/system/debugproc.h"
#include "../../Header/system/manager.h"

//=========================================================
// 静的メンバ変数宣言
//=========================================================
float CRenderer::m_backgroundOffset = 0.0f;

//=========================================================
// コンストラクタ
//=========================================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;			// Direct3Dオブジェクトへのポインタ
	m_pD3DDevice = NULL;	// Direct3Dデバイスへのポインタ
	m_move = -0.5f;
}

//=========================================================
// デストラクタ
//=========================================================
CRenderer::~CRenderer()
{
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// パラメータのゼロクリア
																	   
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面サイズの幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// ゲーム画面サイズの高さ
	d3dpp.BackBufferFormat = d3ddm.Format;							// バックバッファの形式
	d3dpp.BackBufferCount = 1;										// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;							// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;										// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュモード
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル

	// Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,				// 描画処理と頂点処理をハードウェアで行う
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,			// 描画処理とをハードウェア、頂点処理をCPUで行う
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		// 描画処理と頂点処理をCPUで行う
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングの設定
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// アルファブレンドの設定
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// レンダリングターゲット用のテクスチャの生成
	m_pD3DDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT,	// 映し出される範囲の設定
								1,
								D3DUSAGE_RENDERTARGET,
								D3DFMT_A8R8G8B8,
								D3DPOOL_DEFAULT,
								&m_pTextureMT,
								NULL);

	// テクスチャレンダリング用インターフェースの生成
	m_pTextureMT->GetSurfaceLevel(0, &m_pRenderMT);

	// テクスチャレンダリング用Zバッファの生成
	m_pD3DDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT,	// 映し出される範囲の設定
											D3DFMT_D16,
											D3DMULTISAMPLE_NONE,
											0,
											TRUE,
											&m_pZBuffMT,
											NULL);

	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;

	// 現在のレンダリングターゲットを取得(保存)
	m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

	// 現在のZバッファを取得(保存)
	m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

	// レンダリングターゲットを'生成したテクスチャ'に設定
	m_pD3DDevice->SetRenderTarget(0, m_pRenderMT);

	// Zバッファを'生成したテクスチャ'に設定
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

	// レンダリングターゲット用テクスチャのクリア
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// レンダリングターゲットを元に戻す
	m_pD3DDevice->SetRenderTarget(0, pRenderDef);

	// Zバッファを元に戻す
	m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);

	// テクスチャレンダリング用ビューポートの設定
	m_viewportMT.X = 0;
	m_viewportMT.Y = 0;
	m_viewportMT.Width = SCREEN_WIDTH;
	m_viewportMT.Height = SCREEN_HEIGHT;
	m_viewportMT.MinZ = 0.0f;
	m_viewportMT.MaxZ = 1.0f;

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CRenderer::Uninit(void)
{
	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	// レンダリングターゲット用テクスチャの破棄
	if (m_pTextureMT != NULL)
	{
		m_pTextureMT->Release();
		m_pTextureMT = NULL;
	}

	// テクスチャレンダリング用インターフェースの破棄
	if (m_pRenderMT != NULL)
	{
		m_pRenderMT->Release();
		m_pRenderMT = NULL;
	}

	// テクスチャレンダリング用Zバッファの破棄
	if (m_pZBuffMT != NULL)
	{
		m_pZBuffMT->Release();
		m_pZBuffMT = NULL;
	}
}

//=========================================================
// 更新処理
//=========================================================
void CRenderer::Update(void)
{
	// 全てのオブジェクトの更新処理
	CObject::UpdateAll();
}

//=========================================================
// 描画処理
//=========================================================
void CRenderer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();	// デバイスへのポインタの取得
	CDebugProc* pDebugProc = CManager::GetDebugProc();
	CFade* pFade = CManager::GetFade();
	HRESULT hr;

	// 画面クリア(バックバッファとZバッファのクリア)
	m_pD3DDevice->Clear(0, NULL,
						(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
						D3DCOLOR_RGBA(244, 190, 155, 255), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{ // 描画開始が成功した場合

		////Zテストを無効にする
		//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		//
		//// 背景を描画
		//DrawBackground();
		//
		////Zテストを有効にする
		//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// 背景描画中は深度テストとZ書き込みを無効化
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// 背景を描画
		DrawBackground();

		// 3D描画用の設定に戻す
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// 全てのオブジェクトの描画処理
		CObject::DrawAll();

		//デバック表示
		pDebugProc->Draw();

		// フェードの描画処理
		if (pFade != NULL)
		{
			pFade->Draw();
		}

		// 描画終了
		hr = m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	hr = m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=========================================================
// レンダリングターゲットの変更
//=========================================================
void CRenderer::ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU)
{
	D3DXMATRIX mtxView, mtxProjection;

	// レンダリングターゲットを'生成したテクスチャ'に設定
	m_pD3DDevice->SetRenderTarget(0, m_pRenderMT);

	// Zバッファを'生成したテクスチャ'に設定
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

	// テクスチャレンダリング用のビューポートを設定
	m_pD3DDevice->SetViewport(&m_viewportMT);

	//-------------------------------------
	// ビューマトリクスの初期化
	//-------------------------------------
	D3DXMatrixIdentity(&mtxView);

	// ビューマトリクスの生成
	D3DXMatrixLookAtLH(&mtxView, &posV, &posR, &vecU);

	// ビューマトリクスの設定
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxView);

	//-------------------------------------
	// プロジェクションマトリクスの初期化
	//-------------------------------------
	D3DXMatrixIdentity(&mtxProjection);

	// プロジェクションマトリクスの生成
	float fAspect = (float)m_viewportMT.Width / (float)m_viewportMT.Height;
	D3DXMatrixLookAtLH(&mtxProjection, &posV, &posR, &vecU);

	// プロジェクションマトリクスの設定
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

//=========================================================
// バックグラウンドテクスチャを描画
//=========================================================
void CRenderer::DrawBackground(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得
	LPDIRECT3DTEXTURE9 _background = CManager::GetBackground();
	LPD3DXSPRITE _sprite = CManager::GetSprite();
	
	if (_background == NULL || _sprite == NULL)
	{ // 読み込まれていない
		return;
	}

#if 1
	D3DSURFACE_DESC desc;
	if (FAILED(_background->GetLevelDesc(0, &desc)))
		return;

	// 画面サイズに合わせた拡大率
	D3DXMATRIX mat;
	D3DXMatrixScaling(&mat,
					(float)(SCREEN_WIDTH * 2) / desc.Width,
					(float)SCREEN_HEIGHT / desc.Height,
					1.0f);

	// 背景を少しずつ左へ移動
	m_backgroundOffset += m_move;

	// 画面幅を超えたら位置を戻す
	if (m_backgroundOffset <= -SCREEN_WIDTH || 0.0f < m_backgroundOffset)
	{
		m_move *= -1.0f;
	}

	_sprite->Begin(D3DXSPRITE_ALPHABLEND);

	// 拡大率と移動量を設定
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate,
		m_backgroundOffset,
		0.0f,
		0.0f);

	// 拡大してから移動
	D3DXMATRIX transform = mat * translate;
	_sprite->SetTransform(&transform);

	// 背景画像を描画
	D3DXVECTOR3 position(0.0f, 0.0f, 0.0f);

	_sprite->Draw(_background,
					NULL,
					NULL,
					&position,
					D3DCOLOR_XRGB(255, 255, 255));

	_sprite->End();
#else

	// スクロール量
	m_backgroundOffset += 0.001f;

	// 1.0fを超えたら戻す
	if (m_backgroundOffset >= 1.0f)
	{
		m_backgroundOffset -= 1.0f;
	}

	float offset = m_backgroundOffset;

	// UV座標を設定
	VERTEX_2D vertex[] =
	{
		{
			D3DXVECTOR3(0, 0, 0),
			0.0f,
			D3DXCOLOR(1, 1, 1, 1),
			D3DXVECTOR2(0, offset)
		},
		{
			D3DXVECTOR3(SCREEN_WIDTH, 0, 0),
			0.0f,
			D3DXCOLOR(1, 1, 1, 1),
			D3DXVECTOR2(1, offset)
		},
		{
			D3DXVECTOR3(0, SCREEN_HEIGHT, 0),
			0.0f,
			D3DXCOLOR(1, 1, 1, 1),
			D3DXVECTOR2(0, 1.0f + offset)
		},
		{
			D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
			0.0f,
			D3DXCOLOR(1, 1, 1, 1),
			D3DXVECTOR2(1, 1.0f + offset)
		},
	};

	// テクスチャ設定
	pDevice->SetTexture(0, _background);

	// UV座標を繰り返す
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// 頂点設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 描画
	pDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2,
		vertex,
		sizeof(VERTEX_2D)
	);

	// テクスチャ解除
	pDevice->SetTexture(0, NULL);
#endif // 0

}
