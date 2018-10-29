//*****************************************************************************
//
// フェード
// Author : Yusuke Seki
//
//*****************************************************************************
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

class Fade
{
public:
	// コンストラクタ / デストラクタ
	Fade();
	~Fade();

	// 関数
	static Fade* Create();
	void Release();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	// 設定
	void SetColor(unsigned char r = 0xff, unsigned char g = 0xff, unsigned char b = 0xff);

	// フェード
	void Start_FadeOut(unsigned int numFrame = 10); // フェードアウト開始
	bool Finish_FadeOut(); // フェードアウト終わった？


private:
	// 色
	union Color {
		unsigned int color;
		unsigned char rgba[4];
	};

	// 状態
	static const unsigned char FADE_NONE        = 0b00000000; // 状態：フェード処理無し
	static const unsigned char FADE_IN          = 0b00000001; // 状態：フェードイン
	static const unsigned char FADE_OUT         = 0b00000010; // 状態：フェードアウト
	static const unsigned char FADE_OUT_FINISH  = 0b00000100; // 状態：フェードアウト終了

	// 関数
	void UpdateVtxBuf();

	// 変数
	unsigned int m_numFrame; // フェードイン / アウトを完了するまでのフレーム数
	unsigned char m_FadeState; // 現在のフェード状態
	Color m_color;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;


};

#endif
