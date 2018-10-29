//*****************************************************************************
//	DirectX設定クラス[renderer.h]
//	Autohr	:	Yusuke Seki
//*****************************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//---------- インクルードファイル
#include "main.h"

//---------- クラスの定義
class Renderer{
private:
	static LPDIRECT3D9			m_pD3D;
	static LPDIRECT3DDEVICE9	m_pD3DDevice;
#ifdef _DEBUG
	static LPD3DXFONT			m_pFont;
#endif
public:
	Renderer(){}
	~Renderer(){}
	static HRESULT				Init( HWND hWnd, BOOL bWindow );	// 初期化
	static void					Uninit(void);						// 終了
	static HRESULT				DrawBegin(void);					// 描画開始
	static void					DrawEnd(void);						// 描画終了
	static LPDIRECT3DDEVICE9	GetDevice(void);					// デバイスの取得
#ifdef _DEBUG
	static void					DrawFPS(void);						// FPSの表示
#endif
};

#endif
