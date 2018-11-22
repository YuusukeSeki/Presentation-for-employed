//*****************************************************************************
//	
//	ゲーム管理[manager.h]
//	Author	:	Yusuke Seki
//	
//*****************************************************************************
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

//----- インクルードファイル -----
#include "main.h"


//----- 前方宣言 -----
class GameScene;
class Input;
class Fade;

//----- クラスの定義 -----
class GameManager
{
public:
	GameManager() {}
	~GameManager() {}
	static HRESULT Init( HINSTANCE hInstance, HWND hWnd, BOOL bWindow, GameScene* pGameScene);
	static void	   Uninit();
	static void	   Update();
	static void	   Draw();
	static Input*  GetInput() { return m_pInput; }
	static Fade*   GetFade() { return m_pFade; }
	static HINSTANCE GetInstanceHandle() { return instanceHandle_; }
	static HWND GetWindowHandle() { return windowHandle_; }

private:
	static GameScene* m_pGameScene; // 現在のシーン
	static Input* m_pInput;
	static Fade* m_pFade;
	static int _drawDebugInfo;	// デバッグ情報の描画切り替えスイッチ

	static HINSTANCE instanceHandle_;
	static HWND windowHandle_;
};


#endif
