//	Author : Yusuke Seki
//	Author : 20181029

#include "main.h"
#include "GameManager.h"
#include "Title.h"
#include "MainGame.h"
#include "result.h"

#define CLASS_NAME	("class name")
#define WINDOW_NAME	("Dream Land Wars")
#define WindowStyle	(WS_OVERLAPPEDWINDOW ^ WS_MINIMIZEBOX ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME)

enum GAMESCENE
{
	TITLE,
	GAME,
	RESULT,
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FixCreateWindow(HINSTANCE hInstance, int nCmdShow);
D3DXVECTOR2 AdjustClientPosition(D3DXVECTOR2 *outWindowPosition, D3DXVECTOR2 windowSize);
bool InitializeGame(GAMESCENE initGameScene, HINSTANCE hInstance);
void MainRoop(MSG *msg);

HWND g_hwnd;
#ifdef _DEBUG
int	g_nCountFPS	= 0;
#endif

#ifdef _DEBUG
int GetFPS(void)
{
	return g_nCountFPS;
}
#endif

HWND GetHWnd()
{
	return g_hwnd;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	FixCreateWindow(hInstance, nCmdShow);

	if (InitializeGame(GAMESCENE::GAME, hInstance) == false)
	{
		return -1;
	}
	
	MainRoop(&msg);

	GameManager::Uninit();

	return (int)msg.wParam;
}

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

void FixCreateWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcex;
	D3DXVECTOR2 windowPosition(0, 0);
	D3DXVECTOR2 windowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	RegisterClassEx(&wcex);

	AdjustClientPosition(&windowPosition, windowSize);

	g_hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME, WindowStyle,
		(int)windowPosition.x,
		(int)windowPosition.y,
		(int)windowSize.x,
		(int)windowSize.y,
		NULL,
		NULL,
		hInstance, NULL);

	ShowWindow(g_hwnd, nCmdShow);
	UpdateWindow(g_hwnd);
}

D3DXVECTOR2 AdjustClientPosition(D3DXVECTOR2 *outWindowPosition, D3DXVECTOR2 windowSize)
{
	RECT clientRect = { 0, 0, (int)windowSize.x, (int)windowSize.y };
	RECT desktopRect;
	int windowWidth, windowHeight, desktopWidth, desktopHeight;

	// imaginate window rect
	AdjustWindowRect(&clientRect, WindowStyle, FALSE);
	windowWidth = clientRect.right - clientRect.left;
	windowHeight = clientRect.bottom - clientRect.top;

	// practical desktop rect
	GetWindowRect(GetDesktopWindow(), &desktopRect);
	desktopWidth = desktopRect.right - desktopRect.left;
	desktopHeight = desktopRect.bottom - desktopRect.top;

	// adjust window rect
	outWindowPosition->x = windowWidth  > desktopWidth ? 0 : (float)(desktopWidth - windowWidth) / 2;
	outWindowPosition->y = windowHeight > desktopHeight ? 0 : (float)(desktopHeight - windowHeight) / 2;

	return *outWindowPosition;
}

bool InitializeGame(GAMESCENE initGameScene, HINSTANCE hInstance)
{
	HRESULT hr;

	switch (initGameScene)
	{
		case TITLE:
			hr = GameManager::Init(hInstance, g_hwnd, TRUE, new Title);
			break;

		case GAME:
			hr = GameManager::Init(hInstance, g_hwnd, TRUE, new MainGame);
			break;

		case RESULT:
			hr = GameManager::Init(hInstance, g_hwnd, TRUE, new Result);
			break;

	}

	if (hr)
	{
		_MSGERROR("ゲームの初期化に失敗しました。ゲームを終了します。", "ERROR");
		return false;
	}

	return true;
}

void MainRoop(MSG *msg)
{
	DWORD dwCountFrame, dwCurrentTime, dwFPSLastTime, dwExecLastTime;

	dwCountFrame = 0;
	dwCurrentTime = 0;
	dwFPSLastTime = timeGetTime();
	dwExecLastTime = dwFPSLastTime;

	timeBeginPeriod(1);

	for (;;)
	{
		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg->message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(msg);
				DispatchMessage(msg);
			}
		}
		else {
			dwCurrentTime = timeGetTime();

#ifdef _DEBUG
			if (dwCurrentTime - dwFPSLastTime >= 500) {
				g_nCountFPS = (dwCountFrame * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwCountFrame = 0;
			}
#endif
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60)) {
				dwExecLastTime = dwCurrentTime;

				GameManager::Update();

				GameManager::Draw();

				dwCountFrame++;
			}
		}
	}

	timeEndPeriod(1);
}
