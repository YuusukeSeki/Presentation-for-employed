//*****************************************************************************
//
//		ゲームタイトル
//												Autohr : Yusuke Seki
//*****************************************************************************
#include "GameTitle.h"


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
GameTitle::GameTitle() :Object3D(Object::TYPE::TYPE_3D_GAMETITLE)
{
	// メンバ変数初期化

}


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
GameTitle::GameTitle(Object::TYPE type) :Object3D(type)
{
	// メンバ変数初期化

}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
GameTitle::~GameTitle()
{
	// 終了処理
	Uninit();

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
GameTitle* GameTitle::Create(D3DXVECTOR3& pos, D3DXVECTOR3& size)
{
	// 生成
	GameTitle* pGameTitle = new GameTitle;

	// 初期化
	pGameTitle->Init(pos, size);

	return pGameTitle;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void GameTitle::Init(D3DXVECTOR3& position, D3DXVECTOR3& size)
{
	// 継承データの初期化
	Object3D::Init(position, size);

}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void GameTitle::Uninit(void)
{
	// 継承データの終了
	Object3D::Uninit();

}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void GameTitle::Update(void)
{

}


//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void GameTitle::Draw(void)
{
	// 描画
	Object3D::Draw();

}


