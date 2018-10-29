//*****************************************************************************
//
//		リザルト
//												Author : Yusuke Seki
//*****************************************************************************
#include "result.h"
#include "list_LoadTexture_Result.h"

// シーン遷移
#include "GameManager.h"
#include "title.h"
#include "fade.h"

// 入力制御
#include "input.h"

// 背景
#include "Object2D.h"
#include "scene2D.h"

// テクスチャ
#include "TextureManager.h"
#include "texture.h"



//=============================================================================
//	基本的な関数
// 初期化
void Result::Init()
{
	List_LoadTexture_Result::LoadTextureAll(m_pTexture);

	// 背景の生成
	D3DXVECTOR3 pos((SCREEN_WIDTH >> 1), (SCREEN_HEIGHT >> 1), 0);
	D3DXVECTOR3 size((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0);
	//m_pScene2D = Scene2D::Create(pos, size);
	//m_pScene2D->SetTexture(m_pTexture[List_LoadTexture_Result::TEXTURE_NAME::BACKGROUND_TEXNAME]);

	m_pObject2D = Object2D::Create(pos, size);
	m_pObject2D->SetTexture(m_pTexture[List_LoadTexture_Result::TEXTURE_NAME::BACKGROUND_TEXNAME]->GetTexture());

}


// 終了
void Result::Uninit()
{
	// テクスチャの解放処理
	TextureManager::ReleaseAll();

}


// 更新
GameScene* Result::Update()
{
	// 入力デバイスの取得
	Input* pInput = GameManager::GetInput();

	// フェード状態の取得
	Fade* pFade = GameManager::GetFade();

	// 左クリックで次のシーンへ
	if (pInput->GetKeyboardTrigger(DIK_SPACE)) {
	//if (pInput->GetMouseTrigger(Input::MOUSEBUTTON_LEFT)) {
		// フェードアウト開始
		pFade->Start_FadeOut();
	}

	// フェードアウト終了した？
	if (pFade->Finish_FadeOut()) {
		// ゲームシーンをメインゲームへ
		return new Title;
	}

	return this;
}


// 描画
void Result::Draw()
{
}


// 解放
void Result::Release()
{
	// 終了処理
	Uninit();

	// メモリの解放
	delete this;
}

