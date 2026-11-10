//=========================================================
// 
// "カメラの処理"	[camera.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/meshfield.h"
#include "../../Header/object/object.h"
#include "../../Header/scene/game.h"
#include "../../Header/system/camera.h"
#include "../../Header/system/manager.h"

//=========================================================
// マクロ定義
//=========================================================
#define POS_V_X		640.0f		// カメラのX座標
#define POS_V_Y		360.0f		// カメラのX座標
#define POS_V_Z		-350.0f		// カメラのX座標
#define POS_R_X		640.0f		// カメラのX座標
#define POS_R_Y		360.0f		// カメラのY座標
#define POS_R_Z		0.0f		// カメラのZ座標
#define ROT_X		0.0f		// カメラのX座標
#define ROT_Y		0.0f		// カメラのY座標
#define ROT_Z		0.0f		// カメラのZ座標
#define VIEW_SH		100.0f		// ビューボリュームの近平面までの距離
#define VIEW_LO		1000.0f		// ビューボリュームの遠平面までの距離
#define BFFOR_CLEAR	100.0f		// クリア前のカメラ制御

//=========================================================
// コンストラクタ
//=========================================================
CCamera::CCamera()
{
}

//=========================================================
// デストラクタ
//=========================================================
CCamera::~CCamera()
{
}

//=========================================================
// カメラの初期化処理
//=========================================================
void CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(POS_V_X, POS_V_Y, POS_V_Z);	// カメラ
	m_posR = D3DXVECTOR3(POS_R_X, POS_R_Y, POS_R_Z);	// 見てるとこ
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//固定
	m_rot = D3DXVECTOR3(ROT_X, ROT_Y, ROT_Z);

	m_ViewValShot = VIEW_SH;
	m_ViewValLong = VIEW_LO;
}

//=========================================================
// カメラの終了処理
//=========================================================
void CCamera::Uninit(void)
{

}

//=========================================================
// カメラの更新処理
//=========================================================
void CCamera::Update(void)
{
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ
	
	//対角線の長さを算出する
	m_fLengthCamera = sqrtf((POS_V_Z * POS_V_Z) + (POS_V_X * POS_V_X) / 2.0f);

	// デバック設定
	CDebugProc* pDebugProc = CManager::GetDebugProc();
	int nNum = pDebugProc->GetNum();
	if (nNum == pDebugProc->DEBUG_CAMERA)
	{ // カメラ手動操作
		//移動
#if 1
		if (pKeyBoaed->GetKeyboardPress(DIK_W) == true)
		{
			m_posR.x -= sinf(m_rot.y) * m_fLengthCamera * 0.01f;
			m_posR.z += cosf(m_rot.y) * m_fLengthCamera * 0.01f;
			m_posV.x -= sinf(m_rot.y) * m_fLengthCamera * 0.01f;
			m_posV.z += cosf(m_rot.y) * m_fLengthCamera * 0.01f;
		}
		else if (pKeyBoaed->GetKeyboardPress(DIK_S) == true)
		{
			m_posR.x += sinf(m_rot.y) * m_fLengthCamera * 0.01f;
			m_posR.z -= cosf(m_rot.y) * m_fLengthCamera * 0.01f;
			m_posV.x += sinf(m_rot.y) * m_fLengthCamera * 0.01f;
			m_posV.z -= cosf(m_rot.y) * m_fLengthCamera * 0.01f;
		}
#else
		if (pKeyBoaed->GetKeyboardPress(DIK_W) == true)
		{
			m_posR.y += 2.0f;
			m_posV.y += 2.0f;
		}
		else if (pKeyBoaed->GetKeyboardPress(DIK_S) == true)
		{
			m_posR.y -= 2.0f;
			m_posV.y -= 2.0f;
		}
#endif // 0
		if (pKeyBoaed->GetKeyboardPress(DIK_D) == true)
		{
			m_posR.x += cosf(m_rot.y) * m_fLengthCamera * 0.01f;
			m_posR.z += sinf(m_rot.y) * m_fLengthCamera * 0.01f;
			m_posV.x += cosf(m_rot.y) * m_fLengthCamera * 0.01f;
			m_posV.z += sinf(m_rot.y) * m_fLengthCamera * 0.01f;
		}
		else if (pKeyBoaed->GetKeyboardPress(DIK_A) == true)
		{
			m_posR.x -= cosf(m_rot.y) * m_fLengthCamera * 0.01f;
			m_posR.z -= sinf(m_rot.y) * m_fLengthCamera * 0.01f;
			m_posV.x -= cosf(m_rot.y) * m_fLengthCamera * 0.01f;
			m_posV.z -= sinf(m_rot.y) * m_fLengthCamera * 0.01f;
		}

		//注視点
		if (pKeyBoaed->GetKeyboardPress(DIK_Z) == true)
		{
			m_rot.y -= 0.01f;
			m_posV.x = m_posR.x + sinf(m_rot.y) * m_fLengthCamera;
			m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLengthCamera;
		}
		else if (pKeyBoaed->GetKeyboardPress(DIK_C) == true)
		{
			m_rot.y += 0.01f;
			m_posV.x = m_posR.x + sinf(m_rot.y) * m_fLengthCamera;
			m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLengthCamera;
		}
		if (pKeyBoaed->GetKeyboardPress(DIK_N) == true)
		{
			m_posV.y -= 2.0f;
		}
		if (pKeyBoaed->GetKeyboardPress(DIK_Y) == true)
		{
			m_posV.y += 2.0f;
		}

		//視点
		if (pKeyBoaed->GetKeyboardPress(DIK_T) == true)
		{
			//g_camera.posR.z += (g_camera.posV.z - sinf(g_camera.rot.z) * g_fLengthCamera) * 0.01;
			m_posR.y += 4.0f;
		}
		else if (pKeyBoaed->GetKeyboardPress(DIK_B) == true)
		{
			//g_camera.posR.z -= (g_camera.posV.z - sinf(g_camera.rot.z) * g_fLengthCamera) * 0.01;
			m_posR.y -= 4.0f;
		}
		if (pKeyBoaed->GetKeyboardPress(DIK_E) == true)
		{
			m_rot.y -= 0.01f;
			m_posR.x = m_posV.x - sinf(m_rot.y) * m_fLengthCamera;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLengthCamera;
		}
		else if (pKeyBoaed->GetKeyboardPress(DIK_Q) == true)
		{
			m_rot.y += 0.01f;
			m_posR.x = m_posV.x - sinf(m_rot.y) * m_fLengthCamera;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLengthCamera;
		}

		// ビューボリュームの近平面までの距離
		if (pKeyBoaed->GetKeyboardPress(DIK_3) == true)
		{
			m_ViewValShot += 10.0f;
		}
		else if (pKeyBoaed->GetKeyboardPress(DIK_4) == true)
		{
			m_ViewValShot -= 10.0f;
		}

		// ビューボリュームの遠平面までの距離
		if (pKeyBoaed->GetKeyboardPress(DIK_5) == true)
		{
			m_ViewValLong += 10.0f;
		}
		else if (pKeyBoaed->GetKeyboardPress(DIK_6) == true)
		{
			m_ViewValLong -= 10.0f;
		}

		//リセット
		if (pKeyBoaed->GetKeyboardPress(DIK_BACKSPACE) == true)
		{
			m_posR = D3DXVECTOR3(POS_R_X, POS_R_Y, POS_R_Z);
			m_posV = D3DXVECTOR3(POS_V_X, POS_V_Y, POS_V_Z);
			m_rot = D3DXVECTOR3(ROT_X, ROT_Y, ROT_Z);

			m_ViewValShot = VIEW_SH;
			m_ViewValLong = VIEW_LO;
		}

		// 正規化
		if (D3DX_PI <= m_rot.x)
		{
			m_rot.x -= D3DX_PI * 2;
		}
		if (m_rot.x <= -D3DX_PI)
		{
			m_rot.x += D3DX_PI * 2;
		}
		if (D3DX_PI <= m_rot.y)
		{
			m_rot.y -= D3DX_PI * 2;
		}
		if (m_rot.y <= -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2;
		}
		if (D3DX_PI <= m_rot.z)
		{
			m_rot.z -= D3DX_PI * 2;
		}
		if (m_rot.z <= -D3DX_PI)
		{
			m_rot.z += D3DX_PI * 2;
		}

		pDebugProc->PrintDebugProc("\n< 視点 >\n%f, %f, %f\n", m_posV.x, m_posV.y, m_posV.z);
		pDebugProc->PrintDebugProc("\n< 注視点 >\n%f, %f, %f\n", m_posR.x, m_posR.y, m_posR.z);	// 見てるとこ
		pDebugProc->PrintDebugProc("\n< 向き >\n%f, %f, %f\n", m_rot.x, m_rot.y, m_rot.z);
		pDebugProc->PrintDebugProc("\n< ビューボリュームの近平面までの距離 >\n%f\n", m_ViewValShot);
		pDebugProc->PrintDebugProc("\n< ビューボリュームの遠平面までの距離 >\n%f\n", m_ViewValLong);
		pDebugProc->PrintDebugProc("\n\n[ 操作 ]\n< 移動 > W A S D\n< 注視点 > Z C Y N\n< 視点 > Q E T B\n< ビューボリュームの近平面までの距離 > 3↑ 4↓\n< ビューボリュームの遠平面までの距離 > 5↑ 6↓\n< リセット > BACKSPACE");
	}

	if (nNum == pDebugProc->DEBUG_PLAYER && NULL)
	{ // プレイヤー追従

		CObject* pObject = CObject::GetTop(CObject::PRIORITY_PLAYER);	// プレイヤーへのポインタ
		D3DXVECTOR3 PlayerPos = {};
		while (pObject != NULL)
		{
			CPlayer* pPlayer = (CPlayer*)pObject;			// プレイヤーへのポインタ
			PlayerPos = pPlayer->GetPos();

			CObject* pObjectNext = pObject->GetNext();		// 次のオブジェクトを保存
			pObject = pObjectNext;							// 次のオブジェクトを代入
		}

		m_posR.x = PlayerPos.x + POS_R_X;
		m_posV.x = PlayerPos.x + POS_V_X;
		m_posR.z = PlayerPos.z + POS_R_Z;
		m_posV.z = PlayerPos.z + POS_V_Z;

#if 0
		if (fHeight != MIN_HEIGHT && 0 <= fHeight)
		{
			m_posR.y = fHeight + POS_R_Y;
			m_posV.y = fHeight + POS_V_Y;
			m_keep = fHeight;	// 高さを受け取った時の保存
		}
		else
		{
			float moveR = (PlayerPos.y + POS_R_Y) - m_posR.y;	// 目的-今の座標
			float moveV = (PlayerPos.y + POS_V_Y) - m_posV.y;
			 m_posR.y += moveR * 0.5f;	//元0.125
			 m_posV.y += moveV * 0.5f;	//元0.125
		}
#endif // 0

#if 0
		float RposY = POS_R_Y;	// カメラのどこにプレイヤーを捉えるか
		float VposY = POS_V_Y;	// カメラのどこにプレイヤーを捉えるか
		float moveR = 0.0f;
		float moveV = 0.0f;
		float move = 0.0f;
		if (fHeight != MIN_HEIGHT && 0 <= fHeight)
		{ // 地面の高さがある時
			moveR = fHeight;
			moveV = fHeight;
			m_keep = fHeight;	// 高さを受け取った時の保存
		
			if (PlayerPos.x != 0.0f)
			{ // 初期位置以外
				if (fHeight + 150.0f <= PlayerPos.y)
				{ // 地面の高さ+150.0f以上にプレイヤーが飛んだ時
					moveR = PlayerPos.y;
					moveV = PlayerPos.y;
		
					// カメラの中央に捉える
					RposY = 0.0f;
					VposY = 0.0f;
				}
			}
		}
		else
		{
			moveR = PlayerPos.y;
			moveV = PlayerPos.y;
		
			// カメラの中央に捉える
			if (m_keep )
			{
		
			}
			RposY = 0.0f;
			VposY = 0.0f;
		}
		
		moveR = (moveR + RposY) - m_posR.y;	// 目的-今の座標
		moveV = (moveV + VposY) - m_posV.y;
		m_posR.y += moveR * 0.25f;
		m_posV.y += moveV * 0.25f;

		m_posR.x = PlayerPos.x + POS_R_X;
		m_posV.x = PlayerPos.x + POS_V_X;
		m_posR.z = PlayerPos.z + POS_R_Z;
		m_posV.z = PlayerPos.z + POS_V_Z;

		if (PlayerPos.y < 0)
		{ // 地面より下に行った
			m_posR.y = m_keep + POS_R_Y;
			m_posV.y = m_keep + POS_V_Y;
		}

		//// 視点の位置設定
		//m_posR.x = PlayerPos.x;				// 見てるとこ
		////m_posR.y = PlayerPos.y;
		//m_posV.x = PlayerPos.x;				// カメラの座標
		////m_posV.y = PlayerPos.y;

		//********************************
		// ゴール前のカメラ制御
		CObject* pObjectC = CObject::GetTop(CObject::PRIORITY_CLEARBLOCK);	// プレイヤーへのポインタ
		D3DXVECTOR3 ClearBlockPos;
		while (pObjectC != NULL)
		{
			CClearBlock* pClearBlock = (CClearBlock*)pObjectC;			// プレイヤーへのポインタ
			ClearBlockPos = pClearBlock->GetPos();			// pos受け取り

			CObject* pObjectNext = pObjectC->GetNext();		// 次のオブジェクトを保存
			pObjectC = pObjectNext;							// 次のオブジェクトを代入
		}

		// プレイヤーとゴールが同じ高さにいるか
		if (ClearBlockPos.y - 25.0f <= PlayerPos.y && PlayerPos.y <= ClearBlockPos.y + 25.0f)
		{
			if (PlayerPos.x <= ClearBlockPos.x)
			{ // ゴールより左
				if (ClearBlockPos.x - BFFOR_CLEAR <= PlayerPos.x)
				{
					m_posV.x = ClearBlockPos.x - BFFOR_CLEAR;
					m_posR.x = ClearBlockPos.x - BFFOR_CLEAR;
				}
			}
			else
			{ // ゴールより右
				if (PlayerPos.x <= ClearBlockPos.x + BFFOR_CLEAR)
				{
					m_posV.x = ClearBlockPos.x + BFFOR_CLEAR;
					m_posR.x = ClearBlockPos.x + BFFOR_CLEAR;
				}
			}
		}
#endif // 0
	}
}

D3DXVECTOR3 CCamera::SetField(void)
{
	// 高さを取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	float fHeight = CGame::GetMeshField()->Collision(PlayerPos, PlayerPos);

	return D3DXVECTOR3(PlayerPos.x, fHeight, PlayerPos.z);
}
D3DXVECTOR3 CCamera::SetPlayer(void)
{
	// 高さを取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

	return PlayerPos;
}

//=========================================================
// カメラの設定処理
//=========================================================
void CCamera::SetCamera(void)
{
	CRenderer* renderer = CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();;	// デバイスへのポインタの取得

	//-------------------------------------
	//プロジェクションマトリクスの初期化
	//-------------------------------------
	D3DXMatrixIdentity(&m_mtxProjection);

	//-------------------------------------
	//プロジェクションマトリクスを作成
	//-------------------------------------
#if 1
	// 3D視点(射影行列)
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(45.0f),							//視野角
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		//アスペクト比
								10.0f,											//遠くまで見える最少の範囲
								1000.0f);										//最大
#else
	// 2D視点(正射影行列)
	D3DXMatrixOrthoLH(&m_mtxProjection,
						(float)SCREEN_WIDTH,	// ビューボリュームの幅
						(float)SCREEN_HEIGHT,	// ビューボリュームの高さ
						m_ViewValShot,			// ビューボリュームの近平面までの距離
						m_ViewValLong);			// ビューボリュームの遠平面までの距離
#endif // CAMERA_3D

	//プロジェクションマトリクスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//-------------------------------------
	//ビューションマトリクスの初期化
	//-------------------------------------
	D3DXMatrixIdentity(&m_mtxView);

	//ビューションマトリクスを作成
	D3DXMatrixLookAtLH(&m_mtxView,
						&m_posV,
						&m_posR,
						&m_vecU);

	//ビューションマトリクスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}
