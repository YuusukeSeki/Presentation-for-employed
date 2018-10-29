//*****************************************************************************
//	DirectX設定クラス[renderer.cpp]
//	Autohr	:	Yusuke Seki
//*****************************************************************************

//---------- インクルードファイル
#include "renderer.h"
#include "player.h"

//---------- 静的メンバ変数の初期化
LPDIRECT3D9			Renderer::m_pD3D		= NULL;
LPDIRECT3DDEVICE9	Renderer::m_pD3DDevice	= NULL;
#ifdef _DEBUG
LPD3DXFONT			Renderer::m_pFont		= NULL;
#endif

//=============================================================================
// 初期化
//=============================================================================
HRESULT Renderer::Init( HWND hWnd, BOOL bWindow )
{
	D3DPRESENT_PARAMETERS d3dpp = {};
    D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL){
		MessageBox( hWnd, "Direct3Dオブジェクトが作れませんでした", "ERROR!!", MB_ICONERROR );
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))){
		MessageBox( hWnd, "ディスプレイモードが取得できませんでした", "ERROR!!", MB_ICONERROR );
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount			 = 1;							// バックバッファの数
	d3dpp.BackBufferWidth			 = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			 = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			 = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect				 = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil	 = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	 = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed					 = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// デバイスの生成
	if(FAILED(m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp, &m_pD3DDevice))){
		if(FAILED(m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp, &m_pD3DDevice))){
			if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF, hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &m_pD3DDevice))){
				MessageBox( hWnd, "デバイスが作れませんでした", "ERROR!!", MB_ICONERROR );
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);					// Zライト有効
	//m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);		// ワイヤーフレームで表示

	// サンプラーステートの設定
	//m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	//m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,D3DTOP_MODULATE);		// 
	//m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);	// アルファブレンディング処理(初期値はD3DTOP_SELECTARG1)
	//m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// 最初のアルファ引数(初期値はD3DTA_TEXTURE、テクスチャがない場合はD3DTA_DIFFUSE)
	//m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		// ２番目のアルファ引数(初期値はD3DTA_CURRENT)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);	// 
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);	// 



#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

	return S_OK;
}

//=============================================================================
//	終了
//=============================================================================
void Renderer::Uninit(void)
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if(m_pFont != NULL){
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif

	// デバイスの破棄
	if(m_pD3DDevice != NULL){
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if(m_pD3D != NULL){
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=============================================================================
//	描画開始
//=============================================================================
HRESULT Renderer::DrawBegin(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 255, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	return m_pD3DDevice->BeginScene();
}

//=============================================================================
//	描画終了
//=============================================================================
void Renderer::DrawEnd(void)
{
#ifdef _DEBUG
	//DrawFPS();
#endif

	// Direct3Dによる描画の終了
	m_pD3DDevice->EndScene();

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイス取得
//=============================================================================
LPDIRECT3DDEVICE9 Renderer::GetDevice()
{
	return m_pD3DDevice;
}

#ifdef _DEBUG
//=============================================================================
// FPS表示
//=============================================================================
void Renderer::DrawFPS(void)
{
	RECT rect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	char aStr[0x00000fff];

	sprintf(&aStr[0],
		"FPS:%d\n"
		, GetFPS());

	m_pFont->DrawText( NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA( 255, 0, 0, 255 ));

	return;

	sprintf(&aStr[0],
		"【その他】                 \n"
		"Enter:次のシーンへ         \n"
		"T:タイマーを動かす / 止める\n"
		"P:スコアを上げる           \n"
		"O:スコアを下げる           \n"
	);

	m_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 0, 0, 255));
}
#endif


