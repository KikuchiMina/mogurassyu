//=========================================================
// 
// "ポリゴンの処理"	[polygon.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _CAMERA_H_				//このマクロ定義がされてなかったら
#define _CAMERA_H_				//2重インクルード防止のマクロ定義

#include "main.h"

// カメラクラス
class CCamera
{
public:
	CCamera();
	~CCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	D3DXVECTOR3 SetField(void);
	D3DXVECTOR3 SetPlayer(void);
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	CCamera* GetCamera(int nIdx) { return m_pCamera; }
	float GetLength(void) { return m_fLengthCamera; }

private:
	CCamera* m_pCamera;			// カメラ情報
	D3DXVECTOR3 m_posV;			// 視点
	D3DXVECTOR3 m_posR;			// 注視点
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_vecU;			// 上方向ベクトル
	D3DXMATRIX m_mtxProjection;	// プロジェクションマトリクス
	D3DXMATRIX m_mtxView;		// ビューマトリクス
	float m_keep;				// 保存用Yの注視点
	float m_fLengthCamera;		// 対角線の長さ
	float m_ViewValShot;		// ビューボリュームの近平面までの距離
	float m_ViewValLong;		// ビューボリュームの遠平面までの距離
};

#endif // !_CAMERA_H_