//=========================================================
// 
// "メイン処理"	[main.h]
// Author : KikuchiMina
// 
//=========================================================
#ifndef _MAIN_H_						// このマクロ定義がされてなかったら
#define _MAIN_H_						// 2重インクルード防止のマクロ定義

#include <Windows.h>
#include <stdio.h>
#include "d3dx9.h"						// 描画処理に必要
#define DIRECTINPUT_VERSION	(0x0800)	// ビルド時の警告対処用マクロ
#include "dinput.h"						// 入力処理に必要
#include "xaudio2.h"					// サウンド処理に必要
#include "xinput.h"						// ジョイパッド処理に必要
#include <time.h>						// ランダムに必要
#include <math.h>						// fabsに必要

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC        // メモリリーク検知範囲拡大マクロ
#endif
#include <stdlib.h>
#include <crtdbg.h>

//**
//  マクロ定義 
//**
#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)        // メモリリーク検知new
#endif

//=========================================================
//ライブラリのリンク
//=========================================================
#pragma comment(lib, "d3d9.lib")		// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")		// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib, "dxguid.lib")		// DirectXコンポーネント(部品)使用に必要
#pragma comment(lib, "winmm.lib")		// システム時刻取得に必要
#pragma comment(lib, "dinput8.lib")		// 入力処理に必要
#pragma comment(lib, "xinput.lib")		// ジョイパッド処理に必要

//=========================================================
// マクロ定義
//=========================================================
#define SCREEN_WIDTH			(1280)												// ウィンドウの幅
#define SCREEN_HEIGHT			(720)												// ウィンドウの高さ
#define FVF_VERTEX_2D			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// 頂点フォーマット
#define FVF_VERTEX_3D			(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		//頂点フォーマット3D
#define FVF_VERTEX_3D_MULTI		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2)		//頂点フォーマット3D(マルチテクスチャ)
#define MAX_OBJECT				(1000)												// オブジェクトの最大数
#define vec2_ZERO				D3DXVECTOR2(0.0f,0.0f)
#define vec3_ZERO				D3DXVECTOR3(0.0f,0.0f,0.0f)
#define FILED_X					(1280.0f)											// 移動制限の範囲(左右)
#define FILED_Y					(720.0f)											// 移動制限の範囲(下)

//=========================================================
// 頂点情報[2D]の構造体を定義
//=========================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標(x,y,z)
	float rhw;			// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;		// 頂点カラー(r,g,b,a)
	D3DXVECTOR2 tex;	// テクスチャ座標(x,y)
}VERTEX_2D;

//=========================================================
// 頂点情報[3D]の構造体を定義
//=========================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標(x,y,z)
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー(r,g,b,a)
	D3DXVECTOR2 tex;	// テクスチャ座標(x,y)
}VERTEX_3D;

//=========================================================
// 頂点情報[3D]の構造体を定義
//=========================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標(x,y,z)
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー(r,g,b,a)
	D3DXVECTOR2 tex;	// テクスチャ座標(x,y)
	D3DXVECTOR2 texM;	// マルチテクスチャ座標(x,y)
}VERTEX_3D_MULTI;

//=========================================================
// 前方宣言
//=========================================================
class CRenderer;
class CObject;

//=========================================================
// プロトタイプ宣言
//=========================================================
CRenderer* GetRenderer(void);			// レンダラーの取得
CObject* GetObject(void);				// オブジェクトの取得
#endif // !_MAIN_H_