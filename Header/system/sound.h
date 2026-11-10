//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Kikuchi Mina
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"
#include "script.h"

// マクロ定義
#define MAXDATA_SOUND	15	// 音データの最大数

// サウンドクラス
class CSound
{
public:
	// サウンド一覧
	typedef enum
	{
		SOUND_LABEL_BGMOP = 0,	// オープニング
		SOUND_LABEL_BGMED,		// エンディング
		SOUND_LABEL_BGMPS,		// ポーズ
		SOUND_LABEL_SE_JUMP,	// ジャンプ音
		SOUND_LABEL_SE_PAUSE,	// ポーズを開いた/閉じた音
		SOUND_LABEL_SE_SELECT,	// それぞれの選択音
		SOUND_LABEL_SE_WALK,	// 歩く音
		SOUND_LABEL_SE_DAMAGE,	// ダメージ音
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	CSound();
	~CSound();
	HRESULT Load(void);
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(void);
	void StopSound(SOUND_LABEL label);
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	IXAudio2* m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE* m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

	static char m_pFilename[MAXDATA_SOUND][MAX_FILECHAR];	// ファイル名
	static int m_nCntLoop[MAXDATA_SOUND];					// ループカウント
};

// サウンドの情報
//SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX] =
//{
//	{"data/BGM/OPENING.wav", 1},	// オープニング
//	{"data/BGM/ENDING.wav", 1},		// エンディング
//	{"data/BGM/PAUSE.wav", 1},		// ポーズ
//	{"data/SE/Jump.wav", 0},		// ジャンプ音
//	{"data/SE/Pause.wav", 0},		// ポーズを開いた/閉じた音
//	{"data/SE/Select.wav", 0},		// それぞれの選択音
//	{"data/SE/walk.wav", 0},		// 歩く音
//	{"data/SE/Damage.wav", 0},		// ダメージ音
//};

#endif
