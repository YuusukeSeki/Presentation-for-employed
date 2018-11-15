//*****************************************************************************
//	
//	�Q�[���Ǘ�[manager.h]
//	Author	:	Yusuke Seki
//	
//*****************************************************************************
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

//----- �C���N���[�h�t�@�C�� -----
#include "main.h"


//----- �O���錾 -----
class GameScene;
class Input;
class Fade;

//----- �N���X�̒�` -----
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
	static GameScene* m_pGameScene; // ���݂̃V�[��
	static Input* m_pInput;
	static Fade* m_pFade;
	static int _drawDebugInfo;	// �f�o�b�O���̕`��؂�ւ��X�C�b�`

	static HINSTANCE instanceHandle_;
	static HWND windowHandle_;
};


#endif
