//=========================================================
// 
// "メイン処理"	[main.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/system/main.h"
#include "../../Header/system/manager.h"
//#include "timer.h"

// マクロ定義
#define CLASS_NAME			"WindowClass"	//ウィンドウクラスの名前
#define WINDOW_NAME			"就職作品Ｒ α版(クォータニオンゲーム [TKG])"	//ウィンドウの名前

// プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=========================================================
// メイン関数
//=========================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR LpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	// メモリリーク検知用のフラグを立てる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),							// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,									// ウィンドウのスタイル
		WindowProc,									// ウィンドウプロシージャ
		0,											// 0にする
		0,											// 0にする
		hInstance,									// インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),			// タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),				// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),					// クライアント領域の背景色
		NULL,										// メニューバー
		CLASS_NAME,									// ウィンドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)				// ファイルのアイコン
	};												   
													   
	HWND hWnd;										// ウィンドウハンドル(識別式)
	MSG msg;										// メッセージの格納

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };		// 画面サイズの構造体

	DWORD dwCurrentTime;							//現在時刻
	DWORD dwExecLastTime;							//最後に処理した時刻
	DWORD dwFrameCount = NULL;						//フレームカウント
	DWORD dwFPSLastTime = NULL;						//最後にFPSを計測した時刻

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウを生成
	hWnd = CreateWindowEx(0,		// 拡張ウィンドウスタイル
		CLASS_NAME,					// ウィンドウクラスの名前
		WINDOW_NAME,				// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,		// ウィンドウスタイル
		CW_USEDEFAULT,				// ウィンドウの左上x
		CW_USEDEFAULT,				// ウィンドウの左上y
		(rect.right - rect.left),	// ウィンドウの幅
		(rect.bottom - rect.top),	// ウィンドウの高さ
		NULL,						// 親ウィンドウのハンドル
		NULL,						// メニューハンドルまたは子ウィンドウ
		hInstance,					// インスタンスハンドル
		NULL);						// ウィンドウ作成データ
									   
	int nID;

	CManager* pManager = NULL;		// マネージャークラス呼び出し

	if (pManager == NULL)
	{
		// マネージャーの生成
		pManager = new CManager;

		if (pManager != NULL)
		{
			// マネージャーの初期化処理
			if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
			{ // 初期化が失敗した場合
				return -1;
			}

			// 分解能を設定
			timeBeginPeriod(1);

			dwCurrentTime = 0;
			dwExecLastTime = timeGetTime();

			// ウィンドウ表示
			ShowWindow(hWnd, nCmdShow);		// ウィンドウの表示状態を設定
			UpdateWindow(hWnd);				// クライアント領域を更新

			// バックグラウンドの描画


			// メッセージループ
			while (1)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
				{ // windowの処理
					if (msg.message == WM_QUIT)
					{ // WM_QUITメッセージを受け取ったらループを抜ける
						break;
					}
					else
					{
						// メッセージの設定
						TranslateMessage(&msg);			// 仮想メッセージを文字メッセージへ変換
						DispatchMessage(&msg);			// ウィンドプロシージャへメッセージを送出
					}
				}
				else
				{ // DirectXの処理
					dwCurrentTime = timeGetTime();		// 現在時刻を取得

					if ((dwCurrentTime - dwFPSLastTime) >= 500)
					{//0.5秒経過
						//FPSを計測
						int g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
						dwFPSLastTime = dwCurrentTime;		//FPSを測定した時刻を保存
						dwFrameCount = 0;					//フレームカウントをクリア
					}
					if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
					{ // 60分の1秒経過
						dwExecLastTime = dwCurrentTime;		// 処理開始の時刻[現在時刻]を保存

						//CTimer::AddScore(dwExecLastTime);

						// マネージャーの更新処理
						pManager->Update();

						// マネージャーの描画処理
						pManager->Draw();
					}
				}
			}

			// マネージャーの破棄
			if (pManager != NULL)
			{
				// マネージャーの終了処理
				pManager->Uninit();
				delete pManager;
				pManager = NULL;
			}

			// 分解能を戻す
			timeEndPeriod(1);

			// ウィンドウクラスの登録解除
			UnregisterClass(CLASS_NAME, wcex.hInstance);

			return (int)msg.wParam;
		}
		else
		{
			// 入力待機
			nID = MessageBox(hWnd, "レンダラーメモリの確保が出来ませんでした", "エラーメッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}

			// マネージャーの破棄
			if (pManager != NULL)
			{
				// マネージャーの終了処理
				pManager->Uninit();
				delete pManager;
				pManager = NULL;
			}
			return NULL;
		}
	}
	else
	{
		// 入力待機
		nID = MessageBox(hWnd, "レンダラーメモリの生成が出来ませんでした", "エラーメッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		return NULL;
	}
}

//=========================================================
// ウィンドウプロシージャ
//=========================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)		// 特定の処理を繰り返す
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:							// ウィンドウ破棄のメッセージ
		// MW_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:						// マウス左クリックのメッセージ
		// 対称ウィンドウにフォーカスを合わせる
		SetFocus(hWnd);
		break;

	case WM_KEYDOWN:												// キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:												//「ESC」キーが押された
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				// ウィンドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:													// 閉じるボタン押下のメッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			// ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;												// 0を返さないと終了してしまう
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=========================================================
// レンダラーの取得
//=========================================================
/*CRenderer* GetRenderer(void)
{
	return g_pRenderer;
}*/

//=========================================================
// オブジェクトの取得
//=========================================================
/*CObject* GetObject(void)
{
	CObject* g_pObject = NULL;	// レンダラークラス呼び出し
	return g_pObject;
}*/