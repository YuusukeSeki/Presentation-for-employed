//*****************************************************************************
//
//	文字表示
//	Autohr	:	Yusuke Seki
//
//*****************************************************************************
#ifndef _FONT_H_
#define _FONT_H_

//----- インクルードファイル -----
#include "main.h"

//----- クラスの定義 -----
class Font
{
public:
	Font(){}
	~Font(){}
	static void Init(void);
	static void Uninit(void);
//	static void Draw(void);   // バッファ内の文字列を描画
	static void Draw(int color, const char* pText, ...); // 引数の文字列を描画
//	static void StoreText(const char* pText, ...); // 引数の文字列をバッファに格納
//	static void SetColor(int color);          // 文字列の色を指定色に変更
private:
	static LPD3DXFONT m_pFont;
	//static const int NUM_TEXT = 0x0000ffff; // 確保するバッファの領域数
	//static char m_str[NUM_TEXT];            // バッファ
	union TextColor{
		unsigned int color;
		unsigned char RGBA[4];
	};
	//static TextColor m_textColor; // 文字色
};

#endif