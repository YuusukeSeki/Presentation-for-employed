////*****************************************************************************
////
//// サウンド処理 [sound.h]
//// Author : Yusuke Seki
////
////*****************************************************************************
//#ifndef _SOUND_H_
//#define _SOUND_H_
//
////----- インクルードファイル
//#include "main.h"
//
////----- 構造体の定義
//// サウンドファイル
//typedef enum
//{
//	SOUND_LABEL_BGM_TITLE01 = 0,	// BGM -- タイトル
//	SOUND_LABEL_SE_CLEAR01,			// SE -- イベント成功
//	SOUND_LABEL_MAX,
//} SOUND_LABEL;
//// サウンドパラメータ
//typedef struct
//{
//	char *pFilename;	// ファイル名
//	int nCntLoop;		// ループカウント
//} SOUNDPARAM;
//
////----- クラスの定義
//class Sound
//{
//public:
//	static HRESULT InitSound(HWND hWnd);
//	static void UninitSound(void);
//	static HRESULT PlaySound(SOUND_LABEL label);
//	static void StopSound(SOUND_LABEL label);
//	static void StopSound(void);
//	static void PlayClearSound( int arrayNum );
//	static void PlayDeathSound( int arrayNum );
//	static void SetVolume( SOUND_LABEL label , float volume );
//private:
//	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
//	static HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
//	static IXAudio2					*m_pXAudio2;						// XAudio2オブジェクトへのインターフェイス
//	static IXAudio2MasteringVoice	*m_pMasteringVoice;					// マスターボイス
//	static IXAudio2SourceVoice		*m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
//	static BYTE						*m_apDataAudio[SOUND_LABEL_MAX];	// オーディオデータ
//	static DWORD					m_aSizeAudio[SOUND_LABEL_MAX];		// オーディオデータサイズ
//	static SOUNDPARAM				m_aParam[SOUND_LABEL_MAX];			// 各音素材のパラメータ
//};
//
//#endif
