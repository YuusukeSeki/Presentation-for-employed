//*****************************************************************************
//	
//	ゲーム管理
//	Author	:	Yusuke Seki
//	
//*****************************************************************************
#include "GameManager.h"
#include "renderer.h"
#include "GameScene.h"
#include "input.h"
#include "fade.h"
#include "Object.h"
#include "EffectManager.h"
#include "TextureManager.h"

#include "EventButton.h"


//----- 静的メンバ変数初期化 -----
GameScene* GameManager::m_pGameScene = nullptr;
Input*	   GameManager::m_pInput     = nullptr;
Fade*	   GameManager::m_pFade      = nullptr;
int        GameManager::_drawDebugInfo = 1;


//-----------------------------------------------------------------------------
//	初期化
//-----------------------------------------------------------------------------
HRESULT GameManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow, GameScene* pGameScene)
{
	// DirectX初期化
	if( FAILED( Renderer::Init( hWnd, bWindow ) ) ){ // できなかった
		// エラーメッセージの表示
		_MSGERROR("Failed Init DirectX!!", "");

		// 終わります
		return E_FAIL;
	}
	

	// ゲームシーン取得できた？
	if (pGameScene == nullptr) { // できてない
		// エラーメッセージ表示
		_MSGERROR("Cannot Get GameScene!!", "");

		// DirectX終了
		Renderer::Uninit();

		// 終わります
		return E_FAIL;
	}

	// ゲームシーンの取得
	m_pGameScene = pGameScene;

	// デバッグ文字
	OX::DebugFont::initialize(Renderer::GetDevice(), 2500, 1024);

	
	// 入力制御デバイス生成
	m_pInput = Input::Create(hInstance, hWnd);

	// 入力状態の更新
	m_pInput->Update();


	// ゲームシーンを初期化
	m_pGameScene->Init();


	// フェードの生成
	m_pFade = Fade::Create();


	// 正常終了
	return S_OK;
}


//-----------------------------------------------------------------------------
//	終了
//-----------------------------------------------------------------------------
void GameManager::Uninit()
{
	// フェードの解放処理
	m_pFade->Release();
	m_pFade = nullptr;

	// オブジェクトの解放処理
	Object::ReleaseAll();

	// エフェクトの解放処理
	EffectManager::ReleaseAll();
	
	// テクスチャの解放処理
	TextureManager::ReleaseAll();

	// ゲームシーンの解放処理
	m_pGameScene->Release();
	m_pGameScene = nullptr;

	// 入力デバイスの解放処理
	m_pInput->Uninit();
	m_pInput = nullptr;
	
	// デバック表示の解放処理
	OX::DebugFont::terminate();

	// DirectXの解放処理
	Renderer::Uninit();
}


//-----------------------------------------------------------------------------
//	更新
//-----------------------------------------------------------------------------
void GameManager::Update()
{
	// 入力デバイスの更新
	m_pInput->Update();

	// フェードの更新
	m_pFade->Update();

	// ゲームシーンの更新と取得
	GameScene* pGameScene = m_pGameScene->Update();

#ifdef _DEBUG
	// 倍速処理
	if (m_pInput->GetKeyboardPress(BUTTON_DOUBLE_SPEED)) {
		// オブジェクトの更新処理
		Object::UpdateAll();

		// エフェクトの更新処理
		EffectManager::UpdateAll();

	}
#endif

	// オブジェクトの更新処理
	Object::UpdateAll();

	// エフェクトの更新処理
	EffectManager::UpdateAll();

	// シーン遷移する？
	if (m_pGameScene == pGameScene) return; // しない

	// オブジェクトの解放処理
	Object::ReleaseAll();

	// エフェクトの解放処理
	EffectManager::ReleaseAll();

	// テクスチャの解放処理
	TextureManager::ReleaseAll();

	// 現在のゲームシーンの終了処理
	m_pGameScene->Release();

	// 新しいゲームシーンを取得
	m_pGameScene = pGameScene;

	// 新しいゲームシーンの初期化
	m_pGameScene->Init();

}


//-----------------------------------------------------------------------------
//	描画
//-----------------------------------------------------------------------------
void GameManager::Draw()
{
	// DirectX描画開始
	Renderer::DrawBegin();

	// オブジェクトの描画処理
	Object::DrawAll();
	
	// エフェクトの描画処理
	EffectManager::DrawAll();

	// ゲームシーン
	m_pGameScene->Draw();

	// フェードの描画
	m_pFade->Draw();

	// 描画
#ifdef _DEBUG
	if (m_pInput->GetKeyboardTrigger(BUTTON_DEBUG_DRAW)) {
		_drawDebugInfo ^= 1;
	}

	if (_drawDebugInfo) {
		OX::DebugFont::print(0, 0, 0xffff0000, "fps : %d", GetFPS());
		OX::DebugFont::draw(Renderer::GetDevice());
	}
#endif

	// DirectX描画終了
	Renderer::DrawEnd();

	// バッファのクリア
	OX::DebugFont::clear();

}

