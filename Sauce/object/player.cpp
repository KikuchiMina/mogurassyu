//=========================================================
// 
// "プレイヤーの処理"	[player.cpp]
// Author : KikuchiMina
// 
//=========================================================
#include "../../Header/object/bg.h"
#include "../../Header/object/player.h"
#include "../../Header/object/effect.h"
#include "../../Header/object/shadow.h"
#include "../../Header/object/model particle.h"
#include "../../Header/scene/game.h"
#include "../../Header/system/manager.h"

//=========================================================
// マクロ定義
//=========================================================
#define PLAYER_RADIUS	10.0f	// プレイヤーの半径
#define GRAVITY_MOVE	1.0f	// 重力の強さ
#define BOUND_BALL		-0.8f	// 跳ね返る慣性の値
#define VALUE_MAX		0.20f	// 回転の上限
#define FILE_PLAYER		"data\\MODEL\\3Dchar\\motion.txt"	// 外部ファイル入力のファイル名

//=========================================================
// 静的メンバ変数宣言
//=========================================================
int CPlayer::m_nNumModel = 0;

//=========================================================
// オブジェクトの生成処理
//=========================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		// オブジェクトの生成
		pPlayer = new CPlayer;

		if (pPlayer != NULL)
		{
			// オブジェクトの初期化処理
			pPlayer->Init(pos, rot);

			// ファイル読み込み
			pPlayer->Load();

			return pPlayer;
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
CPlayer::CPlayer(PRIORITY nPriority) : CObject(nPriority)
{
	m_bDash = false;
	m_bLanding = false;
	m_bInput = true;
	m_bLookBack = false;

	m_nNumModel = 0;
	m_nCntEffect = 0;
	m_nCntInput = 0;
	m_nBrake = 2;
	m_nSetStock = 0;

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl = D3DXVECTOR3(1.7f, 1.7f, 1.7f);			// スケール
	m_vecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転軸(保存用)
	m_fValueRot = 0.0f;		// 回転角(回転量)
	m_fRotSpeed = 0.05f;	// 回転速度
	m_fInertia = 0.1f;		// 回転減衰
	m_scaledSize = D3DXVECTOR3(PLAYER_RADIUS * fabsf(m_scl.x), PLAYER_RADIUS * fabsf(m_scl.y), PLAYER_RADIUS * fabsf(m_scl.z));	// 半径のサイズ
}

//=========================================================
// デストラクタ
//=========================================================
CPlayer::~CPlayer()
{
	m_nNumModel = 0;
}

//=========================================================
// 地面の初期化処理
//=========================================================
HRESULT CPlayer::Init(void)
{
	return S_OK;
}
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = D3DXVECTOR3(FILED_X / 2, FILED_Y / 2, 0.0f);
	//m_pos = pos;
	m_rot = rot;
	m_state = STATE_NORMAL;

	// タイプの設定
	SetType(TYPE_PLAYER);

	//-------------------------------------
	// マトリックスの初期化
	D3DXMatrixIdentity(&m_mtxRot);

	//-------------------------------------
	// 影の生成
	float posShadow = m_pos.y - m_scaledSize.y;		// 位置-半径
	//m_pShadow = CShadow::Create(D3DXVECTOR3(m_pos.x, posShadow, m_pos.z));

	return 0;
}

//=========================================================
// プレイヤーの終了処理
//=========================================================
void CPlayer::Uninit(void)
{
}

//=========================================================
// プレイヤーの更新処理
//=========================================================
void CPlayer::Update(void)
{
	CCamera* pCamera = CManager::GetCamera();				// カメラへのポインタ
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ
	CInputMouse* pMouse = CManager::GetMouse();				// マウスへのポインタ
	CEnemy* pEnemy = CGame::GetEnemy();						// 敵へのポインタ
	D3DXVECTOR3 CameraRot = pCamera->GetRot();				// カメラの向きを取得

	//-------------------------------------
	// デバック
	CDebugProc* pDebugProc = CManager::GetDebugProc();
	int nNum = pDebugProc->GetNum();
	if (nNum == pDebugProc->DEBUG_PLAYER)
	{
		if (false == CGame::GetPause())
		{
			//-------------------------------------
			// デバック表示
			pDebugProc->PrintDebugProc("\n\n\n**** プレイヤー ****");

			//-------------------------------------
			// 移動処理
			Move();

			//-------------------------------------
			// クォータニオンの回転処理
			ValueRot();

			//-------------------------------------
			// 時間制限
			TimeUp();

			//-------------------------------------
			// エフェクトの生成カウント
			m_nCntEffect++;
			if (7 <= m_nCntEffect)
			{
				// エフェクトの生成
				D3DXCOLOR col = D3DXCOLOR(0.37f, 0.31f, 0.27f, 1.0f);		// 頂点カラー(r,g,b,a)
				for (int nCount = 0; nCount < 3; nCount++)
				{
					//CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, m_scaledSize.x + 5.0f, 0.0f, 100, 1);	// pos, move, col, Radius, sud, Life
				}

				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 頂点カラー(r,g,b,a)
				//CEffect::Create(m_pos, col, 1, 5, 3.0f);
				m_nCntEffect = 0;
			}

			//-------------------------------------
			// 位置の更新
			m_pos.x += m_move.x;
			m_pos.y += m_move.y;
			m_pos.z += m_move.z;

			//-------------------------------------
			// 回転速度の制限
			Debug();	// 値変更の処理

			if (m_fRotSpeed <= 0.00f)
			{
				m_fRotSpeed = 0.01f;
			}
			if (0.2f <= m_fRotSpeed)
			{
				m_fRotSpeed = 0.2f;
			}

			// 回転減衰
			m_fValueRot += (0.0f - m_fValueRot) * m_fInertia;

			// 制限
			if (m_fInertia <= 0.0f)
			{
				m_fInertia = 0.005f;
			}
			if (0.2f <= m_fInertia)
			{
				m_fInertia = 0.2f;
			}

			// 回転の制限
			/*if (m_fValueRot <= 0.0f)
			{
				m_fValueRot = 0.0f;
			}*/
			if (VALUE_MAX <= m_fValueRot)
			{ // 回転の上限
				m_fValueRot = VALUE_MAX;
				m_moveing = true;
			}

			//-------------------------------------
			// 位置の強制
			if (FILED_X < m_pos.x)
			{
				m_pos.x = FILED_X;
			}
			if (m_pos.x < 0.0f)
			{
				m_pos.x = 0.0f;
			}
			if (FILED_Y < m_pos.y)
			{
				m_pos.y = FILED_Y;
			}
			if (m_pos.y < 0.0f)
			{
				m_pos.y = 0.0f;
			}

			//// 地面に埋め込ませない
			//if (m_pos.y <= m_scaledSize.y)
			//{
			//	if (m_bJump == true)
			//	{
			//		D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//頂点カラー(r,g,b,a)
			//		CEffect::Create(m_pos, col, 30, 50, 0.0f);		// エフェクトの生成
			//	}
			//
			//	m_move.y = BOUND_BALL * m_move.y;	// 跳ね返り
			//	m_pos.y = m_scaledSize.y;			// 位置の強制
			//	m_bJump = false;					// ジャンプしていない
			//}

			// デバック表示
			pDebugProc->PrintDebugProc("\n< 位置 > : (%f, %f, %f)", m_pos.x, m_pos.y, m_pos.z);
			pDebugProc->PrintDebugProc("\n< 移動 > : (%f, %f, %f)", m_move.x, m_move.y, m_move.z);
			float data = sqrtf((m_move.x * m_move.x) + (m_move.z * m_move.z));
			pDebugProc->PrintDebugProc("\n< 移動 > : (%f)", data);
			pDebugProc->PrintDebugProc("\n< サイズ > : (%f, %f, %f)", m_scaledSize.x, m_scaledSize.y, m_scaledSize.z);
			//pDebugProc->PrintDebugProc("\n< 目的の回転軸 > : (%f, %f, %f)", m_Input.x, m_Input.y, m_Input.z);
			pDebugProc->PrintDebugProc("\n< 現在の回転軸 > : (%f, %f, %f)", m_vecAxis.x, m_vecAxis.y, m_vecAxis.z);
			pDebugProc->PrintDebugProc("\n< 回転量 > : (%f)", m_fValueRot);
			pDebugProc->PrintDebugProc("\n< 回転速度 > : [ O / L ] (%f)", m_fRotSpeed);
			pDebugProc->PrintDebugProc("\n< 回転減衰 > : [ I / K ] (%f)", m_fInertia);
			pDebugProc->PrintDebugProc("\n< ブレーキ > : [ U / J ] (%d)", m_nBrake);
			pDebugProc->PrintDebugProc("\n< 入力受付 > : (%d)", m_bInput);

			pDebugProc->PrintDebugProc("\n\n[ 位置確定 ] : [ ENTER ]");
			pDebugProc->PrintDebugProc("\n[ ジャンプ ] : [ SPACE ]");
			pDebugProc->PrintDebugProc("\n[ リセット ] : [ R ]");
			pDebugProc->PrintDebugProc("\n[ スケール ] : [ Up↑ DOWN↓ ] (%f)", m_scl.x);
			pDebugProc->PrintDebugProc("\n[ スコア満点 ] : 1000000");
		}
	}
}

//=========================================================
// プレイヤーの描画処理
//=========================================================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスへのポインタの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;				// 計算用マトリックス

	//-------------------------------------
	// 大きくしても暗くならない
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);	// 描画前(絶対)

	//-------------------------------------
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//-------------------------------------
	// スケールを反映
	D3DXMatrixScaling(&mtxScale, m_scl.x, m_scl.y, m_scl.z); 
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//-------------------------------------
	// クォータニオンの設定
#if 0
	// 回転軸における指定の回転角からクォータニオンを作成
	D3DXQuaternionRotationAxis(&m_quat, &m_vecAxisKeep, m_fValueRot);

	// クォータニオンから回転マトリックスを作成
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);

	// 保存した回転マトリックスと掛け合わせる
	D3DXMatrixMultiply(&m_mtxRot, &m_mtxRot, &mtxRot);
#endif // 0

	//-------------------------------------
	// 向きを反映
	// D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//-------------------------------------
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-------------------------------------
	// モデルの描画
	for (int nCount = 0; nCount < m_nNumModel; nCount++)
	{
		m_apModel[nCount]->Draw();
	}

	//-------------------------------------
	// スケールに応じた半径サイズ
	m_scaledSize = D3DXVECTOR3(PLAYER_RADIUS * fabsf(m_scl.x), PLAYER_RADIUS * fabsf(m_scl.y), PLAYER_RADIUS * fabsf(m_scl.z));
}

//=========================================================
// 移動処理
//=========================================================
void CPlayer::Move(void)
{
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ
	CInputJoypad* pJoypad = CManager::GetJoypad();			// ジョイパッドへのポインタ

	//-------------------------------------
	// 移動方向の入力
	D3DXVECTOR3 vecAxis = vec3_ZERO;	// 回転軸の代入用
	D3DXVECTOR3 stick = vec3_ZERO;									// 左スティックの情報

	if (pJoypad->GetJoypadLeftStick(&stick))
	{
		vecAxis.x = -stick.y;
		vecAxis.z = -stick.x;
		m_moveing = true;
	}

	if (pKeyBoaed->GetKeyboardPress(DIK_D) == true)
	{
		vecAxis.z = -1.0f;	// 回転軸
		m_moveing = true;
	}
	else if (pKeyBoaed->GetKeyboardPress(DIK_A) == true)
	{
		vecAxis.z = 1.0f;	// 回転軸
		m_moveing = true;
	}
	if (pKeyBoaed->GetKeyboardPress(DIK_W) == true)
	{
		vecAxis.x = 1.0f;	// 回転軸
		m_moveing = true;
	}
	else if (pKeyBoaed->GetKeyboardPress(DIK_S) == true)
	{
		vecAxis.x = -1.0f;	// 回転軸
		m_moveing = true;
	}

	// 動いてから
	if (m_moveing == true)
	{
		// ブレーキ処理
		if (pKeyBoaed->GetKeyboardTrigger(DIK_RETURN) == true || pJoypad->GetJoypadPress(JOYKEY_B) == true)
		{
			m_fValueRot -= m_fValueRot / m_nBrake;
			m_bInput = false;
		}
	}

	//-------------------------------------
	// 回転軸の正規化
	D3DXVec3Normalize(&m_vecAxis, &vecAxis);
	float moveDistance = sqrtf(m_vecAxis.x * m_vecAxis.x + m_vecAxis.z * m_vecAxis.z);	// vecAxisの平方根

	if (m_vecAxisKeep == D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{ // 何も入力されてない状態
		m_vecAxisKeep = m_vecAxis;	// 最初の移動はベクトルを使わない
	}

	//-------------------------------------
	// 逆方向かどうかを判定
	float fDot = D3DXVec3Dot(&m_vecAxisKeep, &m_vecAxis);
	if (fDot < -0.9f)
	{
		m_bLookBack = true;
	}
	else if (0.0f < fDot)
	{
		m_bLookBack = false;
	}

	////-------------------------------------
	//// 全てのキーが離されているか
	//if (moveDistance == 0.0f)
	//{ // 入力されていない
	//	if (m_moveing == true)
	//	{ // キー入力を離した
	//		if (m_fValueRot <= 0.075f)
	//		{
	//			m_bInput = false;		// 入力を受け付けない
	//			FieldLength();			// フィールドとプレイヤーの距離計算
	//		}
	//	}
	//}
}

//=========================================================
// クォータニオンの回転処理
//=========================================================
void CPlayer::ValueRot(void)
{
	CDebugProc* pDebugProc = CManager::GetDebugProc();	// デバッグへのポインタ

	//-------------------------------------
	// 玉のサイズ
	float move = m_fValueRot * m_scaledSize.x;			// 円周、移動量計算の省略

	//-------------------------------------
	// 角度の補正
	float fRotMove = atan2f(m_vecAxisKeep.x, m_vecAxisKeep.z);	// 現在の移動方向(角度)
	float fRotDest = atan2f(m_vecAxis.x, m_vecAxis.z);			// 目的の移動方向(角度)
	float fRotDiff = fRotDest - fRotMove;						// 目標までの差分
	D3DXVECTOR3 vecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転軸の代入用

	// 角度の値を修正
	fRotDiff = AngleNormalize(fRotDiff);
	
	// 移動方向の補正
	fRotMove += fRotDiff * 0.05f;

	// 角度の値を修正
	fRotMove = AngleNormalize(fRotMove);

	m_move.x = -cosf(fRotMove) * move;
	m_move.y = +sinf(fRotMove) * move;

	if (m_bLookBack == true)
	{
		m_fValueRot *= -1.0f;
		m_vecAxisKeep = m_vecAxis;
		m_bLookBack = false;
	}
	else
	{
		// 軸を90°回転させる
		if (m_vecAxis != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{ // 入力時に回転軸の保存
			m_vecAxisKeep.x = sinf(fRotMove);
			m_vecAxisKeep.z = cosf(fRotMove);
			m_fValueRot += m_fRotSpeed;		// 回転を足す
		}
		D3DXVec3Normalize(&m_vecAxisKeep, &m_vecAxisKeep);
	}

	pDebugProc->PrintDebugProc("\n< 現在の角度 > : (%f)", fRotMove);
	pDebugProc->PrintDebugProc("\n< 目的の角度 > : (%f)", fRotDest);
	pDebugProc->PrintDebugProc("\n< 目的までの差分 > : (%f)\n", fRotDiff);
}

//=========================================================
// 時間制限の処理
//=========================================================
void CPlayer::TimeUp(void)
{
	// 時間制限の設定
	CObject* pObjectTime = CObject::GetTop(PRIORITY_TIME);	// 時間へのポインタ
	while (pObjectTime != NULL)
	{
		CTimer* pTimer = (CTimer*)pObjectTime;				// 時間へのポインタ

		if (CTimer::TIMETYPE_UP == pTimer->GetType())
		{
			// 時間オーバーになったら
			if (pTimer->GetTime() <= 0)
			{
				// 入力を受け付けない
				m_bInput = false;
				m_moveing = true;
				break;
			}
		}

		CObject* pObjectNextTime = pObjectTime->GetNext();	// 次のオブジェクトを保存
		pObjectTime = pObjectNextTime;						// 次のオブジェクトを代入
	}
}

//=========================================================
// 角度の正規化処理
//=========================================================
float CPlayer::AngleNormalize(float fRot)
{
	if (D3DX_PI <= fRot)
	{
		fRot -= D3DX_PI * 2;
	}
	if (fRot <= -D3DX_PI)
	{
		fRot += D3DX_PI * 2;
	}

	return fRot;
}

//=========================================================
// プレイヤーとフィールドの処理
//=========================================================
void CPlayer::FieldLength(void)
{
	CDebugProc* pDebugProc = CManager::GetDebugProc();	// デバッグへのポインタ
	CObject* pObject = CObject::GetTop(PRIORITY_FIELD);	// フィールドへのポインタ
	while (pObject != NULL)
	{
		CField* pField = (CField*)pObject;				// 的フィールドへのポインタ
		CField::FIELD_TEX _type = pField->GetType();	// フィールドの種類

		// 的テクスチャの場合
		if (_type != CField::FIELD_TEX_WALL_00 && _type != CField::FIELD_TEX_WALL_01)
		{
			D3DXVECTOR3 posField = pField->GetPos();

			//float dx = m_pos.x - posField.x;
			//float dz = m_pos.z - posField.z;
			float dx = posField.x - m_pos.x;
			float dz = posField.z - m_pos.z;
			float distance = sqrt((dx * dx) + (dz * dz));	// 中心とキャラの距離
			distance = fabs(distance);	// 距離の正規化

			pDebugProc->PrintDebugProc("\n< 中心とキャラの距離 > : (%f)", distance);

			if (m_bInput == false)
			{ // 動きが止まった

				// カウント
				m_nCntInput++;

				//-------------------------------------
				// エフェクトの生成
				D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//頂点カラー(r,g,b,a)
				CEffect::Create(m_pos, col, 1, 10, 0.0f);	// pos, col,Min, Max

				if (100 <= m_nCntInput)
				{
					CMeshRing::Create(D3DXVECTOR3(m_pos.x, 1.0f, m_pos.z), 0.0f, 0.0f, D3DXVECTOR2(24, 1));

					//-------------------------------------
					// スコアの計算処理
					CreateScore(distance, _type);

					//-------------------------------------
					// フィールドの設定
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

					//-------------------------------------
					// 時間制限の設定
					CObject* pObjectTime = CObject::GetTop(PRIORITY_TIME);	// 時間へのポインタ
					while (pObjectTime != NULL)
					{
						CTimer* pTimer = (CTimer*)pObjectTime;				// 時間へのポインタ
						pTimer->Uninit();	// 全て消去

						CObject* pObjectNextTime = pObjectTime->GetNext();	// 次のオブジェクトを保存
						pObjectTime = pObjectNextTime;						// 次のオブジェクトを代入
					}

					// 生成
					CTimer::Create(D3DXVECTOR3(610.0f, 0.0f, 0.0f), 40.0f, 60.0f, 2, 10, CTimer::TIMETYPE_UP);	// pos,w,h,桁数,時間

					//-------------------------------------
					// 値のリセット
					m_vecAxisKeep = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					m_nCntInput = 0;	// カウントを戻す
					m_moveing = false;	// 動いていない
					m_bInput = true;	// 入力を待機中にする
				}
			}

			return;
		}

		CObject* pObjectNext = pObject->GetNext();	// 次のオブジェクトを保存
		pObject = pObjectNext;						// 次のオブジェクトを代入
	}
}

//=========================================================
// スコアの計算処理
//=========================================================
void CPlayer::CreateScore(float fDistance, int type)
{
	CScore* pScore = CGame::GetScore();		// スコアの受け取り
	CStock* pStock = CGame::GetStock();		// ストックの受け取り
	int _score = 0;							// スコアの代入
	CStock::STOCK stock;					// 円の外かどうか
	m_nSetStock++;	// カウント

	// 円の範囲内かどうか
	if (fDistance < 200.0f)
	{
		switch (type)
		{
		case CField::FIELD_TEX_MATO_00:
			if (fDistance <= 25.0f)
			{
				_score = 100;
				stock = CStock::STOCK_TRUE_R;
			}
			else if (25.0f < fDistance && fDistance <= 65.0f)
			{
				_score = 50;
				stock = CStock::STOCK_TRUE_Y;
			}
			else
			{
				_score = 10;
				stock = CStock::STOCK_TRUE_W;
			}
			break;
		case CField::FIELD_TEX_MATO_01:
			if (fDistance <= 15.0f)
			{
				_score = 100;
				stock = CStock::STOCK_TRUE_R;
			}
			else if (15.0f < fDistance && fDistance <= 65.0f)
			{
				_score = 50;
				stock = CStock::STOCK_TRUE_Y;
			}
			else
			{
				_score = 10;
				stock = CStock::STOCK_TRUE_W;
			}
			break;
		case CField::FIELD_TEX_MATO_02:
			_score = 100;
			stock = CStock::STOCK_TRUE_R;
			break;
		}
	}
	else
	{
		stock = CStock::STOCK_FALSE;
	}

	pStock->SetTex(m_nSetStock - 1, stock);	// ストックのテクスチャ変更
	pScore->AddScore(_score);				// スコアの増加

	// ストックの設定
	if (MAX_STOCK <= m_nSetStock)
	{
		CBg::GameEndCreate();
		return;
	}
}

//=========================================================
// 値変更の処理(debug)
//=========================================================
void CPlayer::Debug(void)
{
	CInputKeyboard* pKeyBoaed = CManager::GetKeyboard();	// キーボードへのポインタ

	//-------------------------------------
	// スケールの変更
	if (pKeyBoaed->GetKeyboardTrigger(DIK_UP) == true)
	{
		m_scl += D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	}
	else if (pKeyBoaed->GetKeyboardTrigger(DIK_DOWN) == true)
	{
		m_scl -= D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	}

	//-------------------------------------
	// 回転速度
	if (pKeyBoaed->GetKeyboardTrigger(DIK_O) == true)
	{
		m_fRotSpeed += 0.01f;
	}
	else if (pKeyBoaed->GetKeyboardTrigger(DIK_L) == true)
	{
		m_fRotSpeed -= 0.01f;
	}

	//-------------------------------------
	// 減衰の値変更
	if (pKeyBoaed->GetKeyboardTrigger(DIK_I) == true)
	{
		m_fInertia += 0.005f;
	}
	else if (pKeyBoaed->GetKeyboardTrigger(DIK_K) == true)
	{
		m_fInertia -= 0.005f;
	}

	//-------------------------------------
	// ブレーキ処理
	if (pKeyBoaed->GetKeyboardTrigger(DIK_U) == true)
	{
		m_nBrake++;
	}
	else if (pKeyBoaed->GetKeyboardTrigger(DIK_J) == true)
	{
		m_nBrake--;
	}
}

//=========================================================
// ファイル名の受け取り
//=========================================================
void CPlayer::ModelFileName(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_apModel[m_nNumModel] = CModel::Create(pFilename, pos, rot);
	m_nNumModel++;
}

//=========================================================
// プレイヤーのファイル処理
//=========================================================
bool CPlayer::Load(void)
{


#if 0
	// === 全項目共通で使用可能な変数 === //
	CDebugProc* pDebugProc = CManager::GetDebugProc();
	FILE* pFile = NULL;
	char Realize[MAX_FILECHAR];			// 判別用の変数
	char* pEqual = NULL;				// =を消去するためのポインタ

	//// === テクスチャ用の変数を用意 === //
	//int nNumTex = 0;					// テクスチャ数
	//int nCntTexFile = 0;				// 読み込んだテクスチャファイル名をカウントする
	//char cTexFileName[MAX_FILECHAR];	// テクスチャファイル名

	// === モデル用の変数を用意 ===	//
	int nNumModel = 0;					// モデル数
	int nCntModelFile = 0;				// 読み込んだXファイル名をカウントする
	char cModelFileName[MAX_MODEL][MAX_FILECHAR];	// Xファイル名

	// === キャラクター情報の変数を用意 === //
	int nNumParts = 0;					// パーツ数
	int nCntParts = 0;					// 読み込んだパーツをカウントする
	int nCharIdx[MAX_MODEL] = {};		// キャラクターのインデックス
	int nCharParent[MAX_MODEL] = {};	// キャラクターの親インデックス
	D3DXVECTOR3 CharPos[MAX_MODEL] = {};// キャラクターの位置
	D3DXVECTOR3 CharRot[MAX_MODEL] = {};// キャラクターの向き

	// === モーション情報の変数を用意 === //
	int nCntMotion = 0;					// 読み込んだモーションをカウントする
	int nMotionrLoop = 0;				// モーションのループするかどうか
	int nNumMotionrKey = 0;				// モーションのキーの数
	int nCntMotionKeySet = 0;			// 読み込んだキーセットをカウントする
	int nMotionFrame = 0;				// モーションのフレーム
	int nNumMotionKey = 0;				// 読み込んだキーをカウントする
	D3DXVECTOR3 MotionPos = {};			// キーの位置
	D3DXVECTOR3 MotionRot = {};			// キーの向き

	// ファイルオープン
	pFile = fopen(FILE_PLAYER, "r");

	if (pFile != NULL)
	{// ファイルを開いた
		// デバック表示
		pDebugProc->PrintDebugProc("****ファイルオープン成功****\n");

		while (1)
		{
			fgets(&Realize[0], sizeof Realize, pFile);	// 最初に比較用文字の読み込み

			if (CScript::JudgeComent(&Realize[0]) == true)
			{// 何かの情報を読み込む合図を取得したとき
				// スクリプト
				if (strcmp(&Realize[0], "SCRIPT") != NULL)
				{
					// デバック表示
					pDebugProc->PrintDebugProc("[ スクリプトの読み込みに成功 ]\n");
				}

				/*
				// テクスチャ数
				if (strstr(&Realize[0], "NUM_TEXTURE") != NULL)
				{// テクスチャの数を代入

					printf("[ テクスチャ数の読み込みに成功 ]\n");

					// 「=」の場所を見つける
					pEqual = strstr(Realize, "=");

					if (pEqual != NULL)
					{
						// アドレスを1こずらす
						pEqual++;

						// テクスチャの数を読み込む
						(void)sscanf(pEqual, "%d", &nNumTex);
						printf("読み込んだテクスチャの数 : %d\n\n", nNumTex);
					}

				}

				// テクスチャファイル名
				if (strstr(&Realize[0], "TEXTURE_FILENAME") != NULL)
				{// テクスチャのファイル名を代入

					if (nCntTexFile == 0)
					{
						printf("[ テクスチャファイル名の読み込みに成功 ]\n");
					}

					while (1)
					{
						if (nCntTexFile < nNumTex)
						{// 上で代入したテクスチャ数よりも、読み込んだファイル数が少ないとき
							// 「=」の場所を見つける
							pEqual = strstr(Realize, "=");

							if (pEqual != NULL)
							{
								// アドレスを1こずらす
								pEqual++;

								// テクスチャの数を読み込む
								(void)sscanf(pEqual, "%s", &cTexFileName[0]);

								// 書き出し
								printf("%d, %s\n", nCntTexFile, &cTexFileName[0]);

								// テクスチャの読み込み
								//m_apModel[nCntXFile]->Lead(g_XFileName[0]);

								// テクスチャのファイル名をカウント
								nCntTexFile++;
								break;
							}

						}
					}
				}
				*/

				// モデル数
				if (strstr(&Realize[0], "NUM_MODEL"))
				{// モデル数を代入

					printf("\n[ モデル数の読み込みに成功 ]\n");

					// 「=」の場所を見つける
					pEqual = strstr(Realize, "=");

					if (pEqual != NULL)
					{
						// アドレスを1こずらす
						pEqual++;

						// テクスチャの数を読み込む
						(void)sscanf(pEqual, "%d", &nNumModel);
					}

				}

				// モデルファイル名
				if (strstr(&Realize[0], "MODEL_FILENAME") != NULL)
				{// モデルのファイル名を代入
					while (1)
					{
						if (nCntModelFile < nNumModel)
						{// 上で代入したモデル数よりも、読み込んだファイル数が少ないとき
							// 「=」の場所を見つける
							pEqual = strstr(Realize, "=");

							if (pEqual != NULL)
							{
								// アドレスを1こずらす
								pEqual++;

								// モデルファイル名を読み込む
								(void)sscanf(pEqual, "%s", &cModelFileName[nCntModelFile][0]);

								// 書き出し
								//printf("%d, %s\n", nCntModelFile, &cModelFileName[0]);

								// モデルファイル名をカウント
								nCntModelFile++;
								break;
							}

						}
					}
				}

				// キャラクター情報
				if (strstr(&Realize[0], "CHARACTERSET"))
				{
					while (1)
					{
						fgets(&Realize[0], sizeof Realize, pFile);	// 最初に比較用文字の読み込み

						if (CScript::JudgeComent(&Realize[0]) == true)
						{// 何かの情報を読み込む合図を取得したとき

							// パーツ数
							if (strstr(&Realize[0], "NUM_PARTS") != NULL)
							{
								pEqual = strstr(Realize, "=");

								if (pEqual != NULL)
								{
									// アドレスを1こずらす
									pEqual++;

									// パーツ数を読み込む
									(void)sscanf(pEqual, "%d", &nNumParts);
									printf("パーツ数 : %d\n\n", nNumParts);
								}
							}

							// パーツの各設定
							if (strstr(&Realize[0], "PARTSSET") != NULL)
							{ // 読み込み開始

								printf("[ %d / %d ]\n", nCntParts, nNumParts);

								while (1)
								{
									fgets(&Realize[0], sizeof Realize, pFile);	// 最初に比較用文字の読み込み

									if (CScript::JudgeComent(&Realize[0]) == true)
									{// 何かの情報を読み込む合図を取得したとき
										if (nCntParts < nNumParts)
										{
											// インデックス
											if (strstr(&Realize[0], "INDEX") != NULL)
											{
												pEqual = strstr(Realize, "=");

												if (pEqual != NULL)
												{
													// アドレスを1こずらす
													pEqual++;

													// インデックスを読み込む
													(void)sscanf(pEqual, "%d", &nCharIdx[nCntParts]);
												}
											}

											// 親インデックス
											if (strstr(&Realize[0], "PARENT") != NULL)
											{
												pEqual = strstr(Realize, "=");

												if (pEqual != NULL)
												{
													// アドレスを1こずらす
													pEqual++;

													// 親インデックスを読み込む
													(void)sscanf(pEqual, "%d", &nCharParent[nCntParts]);
												}
											}

											// 位置
											if (strstr(&Realize[0], "POS") != NULL)
											{
												pEqual = strstr(Realize, "=");

												if (pEqual != NULL)
												{
													// アドレスを1こずらす
													pEqual++;

													// 位置を読み込む
													CScript::Recv(&Realize[0]);
													(void)sscanf(pEqual, "%f%f%f", &CharPos[nCntParts].x, &CharPos[nCntParts].y, &CharPos[nCntParts].z);
												}
											}

											// 向き
											if (strstr(&Realize[0], "ROT") != NULL)
											{
												pEqual = strstr(Realize, "=");

												if (pEqual != NULL)
												{
													// アドレスを1こずらす
													pEqual++;

													// 向きを読み込む
													CScript::Recv(&Realize[0]);
													(void)sscanf(pEqual, "%f%f%f", &CharRot[nCntParts].x, &CharRot[nCntParts].y, &CharRot[nCntParts].z);
												}
											}

											// 読み込みの終了
											if (strstr(&Realize[0], "END_PARTSSET") != NULL)
											{ // 読み込み終了
												//// モデルの設定
												//for (int nCount = 0; nCount < nNumParts; nCount++)
												//{
												//	m_apModel[nCharIdx[nCount]] = CModel::Create(cModelFileName[nCharIdx[nCount]], CharPos[nCharIdx[nCount]], CharRot[nCharIdx[nCount]]);
												//}
												nCntParts++;	// カウントアップ
												break;
											}
										}
									}
								}
							}

							// キャラクター情報の読み込みの終了
							if (strstr(&Realize[0], "END_CHARACTERSET") != NULL)
							{
								//// 親モデルの設定
								//for (int nCount = 1; nCount < nNumParts; nCount++)	// 親はNULLにしたいため、1からfor文開始
								//{
								//	m_apModel[nCharIdx[nCount]]->SetParent(m_apModel[nCharParent[nCount]]);
								//}
								break;
							}
						}
					}
				}
			}

			// スクリプト読み込みを終了する
			if (strstr(&Realize[0], "END_SCRIPT") != NULL)
			{
				// デバック表示
				CDebugProc* pDebugProc = CManager::GetDebugProc();
				pDebugProc->PrintDebugProc("[ スクリプト読み込みを終了します ]\n");
				break;
			}
		}
		// ファイルを閉じる
		fclose(pFile);

		// デバック表示
		pDebugProc->PrintDebugProc("[ ファイルを閉じました ]\n");

		// モデルの総数の設定
		m_nNumModel = nNumParts;

		// モデルの設定
		for (int nCount = 0; nCount < nNumParts; nCount++)
		{
			m_apModel[nCharIdx[nCount]] = CModel::Create(cModelFileName[nCharIdx[nCount]], CharPos[nCharIdx[nCount]], CharRot[nCharIdx[nCount]]);
		}

		// 親モデルの設定
		for (int nCount = 1; nCount < nNumParts; nCount++)	// 親はNULLにしたいため、1からfor文開始
		{
			m_apModel[nCharIdx[nCount]]->SetParent(m_apModel[nCharParent[nCount]]);
		}

		return true;
	}
	else
	{ // ファイルオープン失敗時
		// デバック表示
		pDebugProc->PrintDebugProc("****ファイルオープン失敗****\n");
		return false;
	}
#endif // 0

		return false;
}
