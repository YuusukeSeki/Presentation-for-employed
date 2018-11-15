//*****************************************************************************
//
//		タイトル
//												Author : Yusuke Seki
//*****************************************************************************
#include "Title.h"
#include "list_LoadTexture_Title.h"

// Sequence
#include "GameManager.h"
#include "MainGame.h"
#include "fade.h"

// FrameWork
#include "camera.h"
#include "light.h"

// Input
#include "input.h"

// Object
#include "SkyDome.h"		// Back Ground
#include "GameTitle.h"		// Title Name
#include "TouchScreen.h"	// Tell Go to Next Scene
#include "Object2D.h"

// Resource
#include "TextureManager.h"
#include "texture.h"
#include "ModelData.h"

#include "EventButton.h"


//-----------------------------------------------------------------------------
//	初期化
//-----------------------------------------------------------------------------
void Title::Init()
{
	// Load Texture
	List_LoadTexture_Title::LoadTextureAll(_texture);

	// Create Camera
	{
		_camera = Camera::Create(D3DXVECTOR3(0.f, 0.f, -1.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 1.f, 0.f), 1.f, 1000.f);
	}

	// Create Light
	{
		_light = Light::Create();
		_light->SetVectorDirection(_camera->GetVecZ() * 1);
	}

	// Create Sky Dome
	{
		_skydome = SkyDome::Create(_camera->GetPosEye(), MODELDATA_SKYDOME);
	}

	// Create Game Title
	{
		// base paramater
		D3DXVECTOR3 bigSize(1, 1, 0), smallSize(bigSize * 0.75f);	// size of big or small character
		float posZ = _camera->GetPosEye().z + 5;					// position of Z
		float character_interval = smallSize.x * -0.05f;			// length of between character
		D3DXVECTOR2 numUV(1.0f / 6, 1.0f / 6);						// demarcation of texture(x, y)

		// game title(contain border)
		{
			for (int i = 0; i < NUM_CHARACTER_TITLENAME; i++)
			{
				// big character
				if (i == 0 || i == 5 || i == 9) {
					if (i == 0) {
						// create
						_gameTitle[0] = GameTitle::Create(D3DXVECTOR3(-3.625f + bigSize.x * 0.5f, 1.f, posZ), bigSize);			// Dream	4.00
						_gameTitle[5] = GameTitle::Create(D3DXVECTOR3(-3.625f + 4.f + bigSize.x * 0.5f, 1.0f, posZ), bigSize);	// Land		3.25
						_gameTitle[9] = GameTitle::Create(D3DXVECTOR3(-1.625f + bigSize.x * 0.5f, 0.f, posZ), bigSize);			// Wars		3.25
						_gameTitle_Border[0] = GameTitle::Create(_gameTitle[0]->GetPosition(), _gameTitle[0]->GetSize());		// Dream	4.00
						_gameTitle_Border[5] = GameTitle::Create(_gameTitle[5]->GetPosition(), _gameTitle[5]->GetSize());		// Land		3.25
						_gameTitle_Border[9] = GameTitle::Create(_gameTitle[9]->GetPosition(), _gameTitle[9]->GetSize());		// Wars		3.25

						// setting texture
						_gameTitle[0]->SetTexture(_texture[List_LoadTexture_Title::TEXT_TEXNAME]->GetTexture());
						_gameTitle[5]->SetTexture(_texture[List_LoadTexture_Title::TEXT_TEXNAME]->GetTexture());
						_gameTitle[9]->SetTexture(_texture[List_LoadTexture_Title::TEXT_TEXNAME]->GetTexture());
						_gameTitle_Border[0]->SetTexture(_texture[List_LoadTexture_Title::TEXTBORDER_TEXNAME]->GetTexture());
						_gameTitle_Border[5]->SetTexture(_texture[List_LoadTexture_Title::TEXTBORDER_TEXNAME]->GetTexture());
						_gameTitle_Border[9]->SetTexture(_texture[List_LoadTexture_Title::TEXTBORDER_TEXNAME]->GetTexture());

					}
				}
				// small character
				else {
					// create
					_gameTitle[i] = GameTitle::Create(D3DXVECTOR3(	_gameTitle[i - 1]->GetPosition().x + _gameTitle[i - 1]->GetHalfSize().x + smallSize.x * 0.5f + character_interval,
																	_gameTitle[i - 1]->GetPosition().y - _gameTitle[i - 1]->GetHalfSize().y + smallSize.y * 0.5f,
																	posZ), smallSize);
					_gameTitle_Border[i] = GameTitle::Create(_gameTitle[i]->GetPosition(), _gameTitle[i]->GetSize());

					// setting texture
					_gameTitle[i]->SetTexture(_texture[List_LoadTexture_Title::TEXTSMALL_TEXNAME]->GetTexture());
					_gameTitle_Border[i]->SetTexture(_texture[List_LoadTexture_Title::TEXTSMALLBORDER_TEXNAME]->GetTexture());
					
				}

				// UV size
				_gameTitle[i]->SetUv_Size(numUV);
				_gameTitle_Border[i]->SetUv_Size(numUV);

			}

			// UV start point
			_gameTitle[0] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 3, numUV.y * 0));	// D
			_gameTitle[1] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 5, numUV.y * 2));	// r
			_gameTitle[2] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 4, numUV.y * 0));	// e
			_gameTitle[3] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 0, numUV.y * 0));	// a
			_gameTitle[4] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 0, numUV.y * 2));	// m
			_gameTitle[5] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 5, numUV.y * 1));	// L
			_gameTitle[6] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 0, numUV.y * 0));	// a
			_gameTitle[7] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 1, numUV.y * 2));	// n
			_gameTitle[8] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 3, numUV.y * 0));	// d
			_gameTitle[9] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 4, numUV.y * 3));	// W
			_gameTitle[10]->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 0, numUV.y * 0));	// a
			_gameTitle[11]->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 5, numUV.y * 2));	// r
			_gameTitle[12]->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 1, numUV.y * 4));	// s

			// border common paramater
			for (int i = 0; i < NUM_CHARACTER_TITLENAME; i++)
			{
				// UV start point
				_gameTitle_Border[i]->SetUv_StartPoint(_gameTitle[i]->GetUv_StartPoint());

				// color
				_gameTitle_Border[i]->SetColor(0x000000ff);

			}

		}

	}

	// Create Touch Screen
	{
		// base paramater
		D3DXVECTOR3 size(0.3f, 0.3f, 0);			// size of polygon
		float posZ = _camera->GetPosEye().z + 5;	// position of Z
		float character_interval = size.x * -0.00f;		// length of between character
		D3DXVECTOR2 numUV(1.0f / 6, 1.0f / 6);		// demarcation of texture(x, y)


		// touch screen
		{
			for (int i = 0; i < NUM_CHARACTER_TOUCHSCREEN; i++)
			{
				// head character
				if (i == 0 || i == 5) {
					if (i == 0) {
						// create
						_touchScreen[0] = TouchScreen::Create(D3DXVECTOR3(-1.8f, -1.f, posZ), size);			// TOUCH	1.5
						_touchScreen[5] = TouchScreen::Create(D3DXVECTOR3(size.x * 0.5f, -1.f, posZ), size);	// SCREEN	1.8

					}
				}
				// small character
				else {
					// create
					_touchScreen[i] = TouchScreen::Create(D3DXVECTOR3(	_touchScreen[i - 1]->GetPosition().x + _touchScreen[i - 1]->GetSize().x + character_interval,
																		_touchScreen[i - 1]->GetPosition().y, posZ), size);

				}

				// setting texture
				_touchScreen[i]->SetTexture(_texture[List_LoadTexture_Title::TEXT_TEXNAME]->GetTexture());

				// UV size
				_touchScreen[i]->SetUv_Size(numUV);

			}

			// UV start point
			_touchScreen[0] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 1, numUV.y * 3));	// T
			_touchScreen[1] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 2, numUV.y * 2));	// O
			_touchScreen[2] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 2, numUV.y * 3));	// U
			_touchScreen[3] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 2, numUV.y * 0));	// C
			_touchScreen[4] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 1, numUV.y * 1));	// H
			_touchScreen[5] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 0, numUV.y * 3));	// S
			_touchScreen[6] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 2, numUV.y * 0));	// C
			_touchScreen[7] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 5, numUV.y * 2));	// R
			_touchScreen[8] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 4, numUV.y * 0));	// E
			_touchScreen[9] ->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 4, numUV.y * 0));	// E
			_touchScreen[10]->SetUv_StartPoint(D3DXVECTOR2(numUV.x * 1, numUV.y * 2));	// N

		}

	}

	tutorialNumber = 0;

	_tutorial = Object2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0), D3DXVECTOR3(SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.9f, 0));
	_tutorial->SetTexture(_texture[List_LoadTexture_Title::TUTORIAL01]->GetTexture());
	_tutorial->SetDraw(false);

}


//-----------------------------------------------------------------------------
//	終了
//-----------------------------------------------------------------------------
void Title::Uninit()
{
	// Delete Light
	_light->Release();
	_light = nullptr;

	// Delete Camera
	_camera->Release();
	_camera = nullptr;

	// Unload Texture
	TextureManager::ReleaseAll();

}


//-----------------------------------------------------------------------------
//	更新
//-----------------------------------------------------------------------------
GameScene* Title::Update()
{
	// 入力デバイスの取得
	Input* pInput = GameManager::GetInput();

	// フェード状態の取得
	Fade* pFade = GameManager::GetFade();

	
	// Rotate Sky Dome
	_skydome->MoveRotate(D3DXVECTOR3(0, 0.002f, 0));


	// To Go Next Scene by Press Key
	if (pInput->GetKeyboardTrigger(BUTTON_NEXT_SCENE) || pInput->GetMouseTrigger(Input::MOUSEBUTTON_LEFT)) {
		if (tutorialNumber == 0) {
			_tutorial->SetDraw(true);
		}
		else if (tutorialNumber == 1) {
			_tutorial->SetTexture(_texture[List_LoadTexture_Title::TUTORIAL02]->GetTexture());
		}
		else if (tutorialNumber >= 2) {
			// フェードアウト開始
			pFade->Start_FadeOut();
		}

		tutorialNumber++;
	}


	// フェードアウト終了した？
	if(pFade->Finish_FadeOut()){
		// ゲームシーンをメインゲームへ
		return new MainGame;
	}

	return this;
}


//-----------------------------------------------------------------------------
//	描画
//-----------------------------------------------------------------------------
void Title::Draw()
{
}


//-----------------------------------------------------------------------------
//	解放
//-----------------------------------------------------------------------------
void Title::Release()
{
	// 終了処理
	Uninit();

	// メモリの解放
	delete this;
}

