//=========================================================
// 
// "レンダラーの処理"	[renderer.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _RENDERER_H_						//このマクロ定義がされてなかったら
#define _RENDERER_H_						//2重インクルード防止のマクロ定義

// インクルードファイル
#include "main.h"

// レンダラークラス定義
class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU);
	LPDIRECT3DTEXTURE9 GetTextureMT(void) { return m_pTextureMT; }
	void DrawBackground(void);

private:
	LPDIRECT3D9			m_pD3D;			// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9	m_pD3DDevice;	// Direct3Dデバイスへのポインタ
	LPDIRECT3DTEXTURE9	m_pTextureMT;	// レンダリングターゲット用テクスチャ
	LPDIRECT3DSURFACE9	m_pRenderMT;	// テクスチャレンダリング用インターフェース
	LPDIRECT3DSURFACE9	m_pZBuffMT;		// テクスチャレンダリング用Zバッファ
	D3DVIEWPORT9		m_viewportMT;	// テクスチャレンダリング用ビューポート
	float m_move;						// バックグラウンドの移動量
	static float m_backgroundOffset;	// バックグラウンドの移動した値
};

#endif // !_RENDERER_H_
