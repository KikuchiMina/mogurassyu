//=========================================================
// 
// "スクリプト読み込み処理"	[script.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _SCRIPT_H_		//このマクロ定義がされてなかったら
#define _SCRIPT_H_		//2重インクルード防止のマクロ定義

#include "main.h"
#include "../object/model.h"

// マクロ定義
#define MAX_FILECHAR	125		// 最大文字

// キューブクラス
class CScript
{
public:
	CScript();
	~CScript();
	HRESULT FileLoad(void);
	bool Load(void);
	static void Recv(char* data);
	static bool JudgeComent(char* pStr);

private:
	static FILE* m_pFile;					// ファイルの読み込み
	static CModel* m_apModel[MAX_MODEL];	// モデル(パーツ)へのポインタ
};

#endif // !_FILE_H_