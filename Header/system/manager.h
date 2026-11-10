//=========================================================
// 
// "マネージャーの処理"	[manager.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _MANAGER_H_						//このマクロ定義がされてなかったら
#define _MANAGER_H_						//2重インクルード防止のマクロ定義

//=========================================================
// インクルードファイル
//=========================================================
#include "texture.h"
#include "debugproc.h"
#include "input.h"
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "light.h"
#include "script.h"
#include "sound.h"
#include "../object/field.h"
#include "../object/player.h"
#include "../object/enemy.h"
#include "../object/meshfield.h"
#include "../scene/scene.h"
#include "../scene/fade.h"
#include "../scene/pause.h"

//=========================================================
// マネージャークラス定義
//=========================================================
class CManager
{
public:
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HWND GetWnd(void) { return m_hWnd; }

	//=== バックグラウンド ===//
	HRESULT LoadBackground(void);
	static LPDIRECT3DTEXTURE9 GetBackground(void) { return m_background; }	// バックグラウンド用
	static LPD3DXSPRITE GetSprite(void) { return m_sprite; }				// バックグラウンド用

	//=== 2Dobject ===//
	static CTexture* GetTexture(void) { return m_pTexture; }
	//static CNumber* GetNumber(void) { return m_pNumber ; }
	//static CScore* GetScore(void) { return m_pScore ; }
	//static CTimer* GetTimer(void) { return m_pTimer ; }

	//=== 3Dobject ===//
	static CCamera* GetCamera(void) { return m_pCamera; }
	static CLight* GetLight(void) { return m_pLight; }
	static CObject3D* GetObject3D(void) { return m_pObject3D; }
	static CField* GetField(void) { return m_pField; }
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CEnemy* GetEnemy(void) { return m_pEnemy; }
	static CMeshField* GetMeshField(void) { return m_pMeshField; }
	//static C* Get(void) { return m_p ; }

	//=== system ===//
	static CInput* GetInput(void) { return m_pInput; }
	static CInputKeyboard* GetKeyboard(void) { return m_pInputKeyboard; }
	static CInputMouse* GetMouse(void) { return m_pInputMouse; }
	static CInputJoypad* GetJoypad(void) { return m_pInputJoypad; }
	static CRenderer* GetRenderer(void) { return m_pRenderer; }
	static CDebugProc* GetDebugProc(void) { return m_pDebugProc; }
	static CScript* GetScript(void) { return m_pScript; }
	static CScene* GetScene(void) { return m_pScene; }
	static CFade* GetFade(void) { return m_pFade; }
	static void SetMode(CScene::MODE mode);
	static CScene::MODE GetMode(void);
	static CSound* GetSound(void) { return m_pSound; }
	static CPause* GetPause(void) { return m_pPause; }
	static void SetPause(bool bPause) { m_bPause = bPause; }

private:
	//=== 2Dobject ===//
	static CTexture* m_pTexture;
	//static CNumber* GetNumber(void) { return m_pNumber; }
	//static CScore* GetScore(void) { return m_pScore; }
	//static CTimer* GetTimer(void) { return m_pTimer; }

	//=== 3Dobject ===//
	static CCamera* m_pCamera;
	static CLight* m_pLight;
	static CObject3D* m_pObject3D;
	static CField* m_pField;
	static CPlayer* m_pPlayer;
	static CEnemy* m_pEnemy;
	static CMeshField* m_pMeshField;

	//=== system ===//
	static CInput* m_pInput;
	static CInputKeyboard* m_pInputKeyboard;
	static CInputMouse* m_pInputMouse;
	static CInputJoypad* m_pInputJoypad;
	static CRenderer* m_pRenderer;
	static CDebugProc* m_pDebugProc;
	static CScript* m_pScript;
	static CScene* m_pScene;
	static CFade* m_pFade;
	static CSound* m_pSound;
	static CPause* m_pPause;

	//=== バックグラウンド ===//
	static LPDIRECT3DTEXTURE9 m_background;	// バックグラウンド用
	static LPD3DXSPRITE m_sprite;			// バックグラウンド用

	static bool m_bPause;			// ポーズ状態
	int	m_nCountFPS;				// FPSカウンタ
	static HINSTANCE m_hInstance;	// 保存用インスタンス
	static HWND m_hWnd;				// 
	static BOOL m_bWindow;			// 
};

#endif // !_MANAGER_H_
