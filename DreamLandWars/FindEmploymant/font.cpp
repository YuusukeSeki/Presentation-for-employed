//*****************************************************************************
//
//	文字表示
//	Autohr	:	Yusuke Seki
//
//*****************************************************************************
#include "font.h"
#include "renderer.h"
#include <string>

//----- 静的変数初期化 -----
LPD3DXFONT Font::m_pFont = nullptr;
//char Font::m_str[NUM_TEXT] = {};
//Font::TextColor Font::m_textColor;

//=============================================================================
// 初期化
//=============================================================================
void Font::Init(void)
{
	// D3Dデバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);

	// 色の設定
	//m_textColor.color = 0x00ff00ff;
}

//=============================================================================
// 終了
//=============================================================================
void Font::Uninit(void)
{
	// 生成したフォントを解放
	if (m_pFont != nullptr) {
		m_pFont->Release();
	}

	// バッファ内部を初期化
	//m_str[0] = '\0';

	// テキスト色を初期化
	//m_textColor.color = 0x00ff00ff;
}

//=============================================================================
// 文字列をバッファに格納
//=============================================================================
//void Font::StoreText(const char* pText, ...)
//{
//	// 引数の文字をバッファに格納
//	strcat_s(m_str, pText);
//	strcat_s(m_str, "\n");
//}

//=============================================================================
// バッファ内の文字を描画
//=============================================================================
//void Font::Draw(void)
//{
//	// 変数宣言
//	char r = m_textColor.RGBA[3]; // 赤
//	char g = m_textColor.RGBA[2]; // 緑
//	char b = m_textColor.RGBA[1]; // 青
//	char a = m_textColor.RGBA[0]; // α値
//
//	// クライアント領域取得
//	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	// 描画
//	m_pFont->DrawText(nullptr, &m_str[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(r, g, b, a));
//
//	// バッファ内部を初期化
//	m_str[0] = '\0';
//}

//=============================================================================
// 引数の文字を描画
//=============================================================================
void Font::Draw(int color, const char* pText, ...)
{
	// 変数宣言
	TextColor tColor;
	tColor.color = color;
	char r = tColor.RGBA[3]; // 赤
	char g = tColor.RGBA[2]; // 緑
	char b = tColor.RGBA[1]; // 青
	char a = tColor.RGBA[0]; // α値
	char str[256];

	// 可変長引数の代入
	va_list ap;
	va_start(ap, pText);
	vsprintf(str,pText, ap);
	va_end(ap);

	// クライアント領域取得
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// 描画
	m_pFont->DrawText(nullptr, &str[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(r, g, b, a));
}

//=============================================================================
// 色を設定
//=============================================================================
//void Font::SetColor(int color)
//{
//	m_textColor.color = color;
//}
