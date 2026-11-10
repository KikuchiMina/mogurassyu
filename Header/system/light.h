//=========================================================
// 
// "ライトの処理"	[light.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _LIGHT_H_					//このマクロ定義がされてなかったら
#define _LIGHT_H_					//2重インクルード防止のマクロ定義

#include "main.h"

//=========================================================
// マクロ定義
//=========================================================
#define MAX_LIGHT	3	//最低限3

// ライトクラス
class CLight
{
public:
	CLight();
	~CLight();
	void Init(void);
	void Uninit(void);
	void Update(void);

private:
	D3DLIGHT9 m_aLight[MAX_LIGHT];		// ライトの情報
	D3DXVECTOR3 m_vecDir;				// ライトの方向
};

#endif // !_LIGHT_H_