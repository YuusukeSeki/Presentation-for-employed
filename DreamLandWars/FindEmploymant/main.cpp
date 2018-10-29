//*****************************************************************************
//	GM31 授業１コマ目
//	Author : Yusuke Seki
//*****************************************************************************

//---------- インクルードファイル
#include "main.h"
#include "GameManager.h"
#include "Title.h"
#include "MainGame.h"

//---------- マクロ定義
#define CLASS_NAME	("class name")	// ウィンドウクラスの名前
#define WINDOW_NAME	("Dream Land Wars")	// ウィンドウの名前
#define WindowStyle		(WS_OVERLAPPEDWINDOW ^ WS_MINIMIZEBOX ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME)	// ウィンドウスタイル

//---------- プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ウィンドウプロシージャ

//---------- グローバル宣言
#ifdef _DEBUG
int	g_nCountFPS	= 0;	// FPSカウンター
#endif
HWND g_hwnd;


//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	HWND hWnd;
	WNDCLASSEX wcex;
	RECT ClientRect, DesktopRect;
	int WindowWidth, WindowHeight, DesktopWidth, DesktopHeight, WindowX, WindowY;
	HRESULT hr;
	MSG msg;
	DWORD dwCountFrame, dwCurrentTime, dwFPSLastTime, dwExecLastTime;


	//----- ウィンドウの作成 -----
	// ウィンドウクラスの設定
	wcex = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0, 0, hInstance, NULL, LoadCursor(NULL,IDC_ARROW),
			 (HBRUSH)(COLOR_WINDOW+1), NULL, CLASS_NAME, NULL };
	RegisterClassEx(&wcex);

	// クライアント領域の設定
	ClientRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect( &ClientRect, WindowStyle, FALSE );
	WindowWidth  = ClientRect.right  - ClientRect.left;
	WindowHeight = ClientRect.bottom - ClientRect.top;

	// デスクトップ領域の取得
	GetWindowRect( GetDesktopWindow(), &DesktopRect );
	DesktopWidth  = DesktopRect.right  - DesktopRect.left;
	DesktopHeight = DesktopRect.bottom - DesktopRect.top;

	// 実際のクライアント領域の算出
	WindowX = WindowWidth  > DesktopWidth  ? 0 : ( DesktopWidth  - WindowWidth  ) / 2;
	WindowY = WindowHeight > DesktopHeight ? 0 : ( DesktopHeight - WindowHeight ) / 2;

	// ウィンドウの実体化
	hWnd = CreateWindowEx( 0, CLASS_NAME, WINDOW_NAME, WindowStyle, WindowX, WindowY,
						   WindowWidth, WindowHeight, NULL, NULL, hInstance, NULL );
	g_hwnd = hWnd;

	// ウィンドウの表示
	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

	
	//----- ゲームの初期化 -----
	// ゲームマネージャーの初期化
	//hr = GameManager::Init(hInstance, hWnd, TRUE, new Title);
	hr = GameManager::Init(hInstance, hWnd, TRUE, new MainGame);

	// 結果は？
	if (hr) { // 失敗しました
		// エラーメッセージの表示
		_MSG("終了します");

		// 終わります
		return -1;
	}

	// 分解能設定
	timeBeginPeriod(1);

	// FPS設定
	dwCountFrame		= 0;				// フレームカウンター
	dwCurrentTime		= 0;				// 現在時刻
	dwFPSLastTime		= timeGetTime();	// 最後にFPSを更新した時刻
	dwExecLastTime	= dwFPSLastTime;	// 最後にゲーム処理をした時刻

	//----- メインループ -----
	for(;;){
		// メッセージの取得
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) != 0 ){	// WndProcの設定
			if(msg.message == WM_QUIT){	// 終了命令の取得で、ループ脱出
				break;
			}else{	// 命令・処理の取得
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		// ゲームループ
		else{
			// 現在時刻の取得
			dwCurrentTime = timeGetTime();
	
#ifdef _DEBUG
			// FPSの更新
			if( dwCurrentTime - dwFPSLastTime >= 500 ){
				g_nCountFPS		= ( dwCountFrame * 1000 ) / ( dwCurrentTime-dwFPSLastTime );
				dwFPSLastTime	= dwCurrentTime;
				dwCountFrame	= 0;
			}
#endif
			// ゲーム処理
			if((dwCurrentTime - dwExecLastTime) >= (1000/60)){
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				GameManager::Update();

				// 描画処理
				GameManager::Draw();

				dwCountFrame++;
			}// end... ゲーム処理
		}// end... ゲームループ
	}// end... メインループ

	// 分解能の解放
	timeEndPeriod(1);


	//----- ゲーム終了 -----
	GameManager::Uninit();

	return (int)msg.wParam;
}

//=============================================================================
//	クライアント領域の設定
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT nID;	// ID取得

	switch(uMsg)
	{
	// ウィンドウクリエイト
	case WM_CREATE:
		break;

	// キー入力
	case WM_KEYDOWN:
		switch(wParam){
		case VK_ESCAPE:	// 実行ファイル終了確認
			nID = MessageBox( hWnd, "終了しますか？" , "終了確認" , MB_YESNO | MB_ICONINFORMATION );
			if(nID == IDYES){ DestroyWindow(hWnd); }
			break;
		}
		break;
	
	// 終了確認
	case WM_CLOSE:
		nID = MessageBox( hWnd, "終了しますか？" , "終了確認" , MB_YESNO | MB_ICONINFORMATION );
		if(nID != IDYES){ return 0; }
		break;

	// 終了処理
	case WM_DESTROY:
		PostQuitMessage(0);	// メインループ脱出
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 既定の処理を提供
}

#ifdef _DEBUG
//=============================================================================
//	FPS取得
//=============================================================================
int GetFPS(void)
{
	return g_nCountFPS;
}
#endif


HWND GetHWnd()
{
	return g_hwnd;
}

