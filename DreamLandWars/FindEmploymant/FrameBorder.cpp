//*****************************************************************************
//
//		枠線
//													Author : Yusuke Seki
//*****************************************************************************
#include "FrameBorder.h"
#include "timer.h"
#include "TeamGaugeManager.h"
#include "TeamLevel.h"
#include "MainGame.h"
#include "list_LoadTexture_MainGame.h"


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
FrameBorder::FrameBorder(Object::TYPE type) : Object2D(type)
{
	// データのクリア
	m_pTimer     = nullptr;	// 【子】タイマー
	m_pTeamGaugeManager = nullptr;	// 【子】チームゲージ
	m_pTeamLevel = nullptr;	// 【子】チームレベル

}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
FrameBorder::~FrameBorder()
{
	// 終了処理
	Uninit();

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
FrameBorder* FrameBorder::Create(D3DXVECTOR3& position, D3DXVECTOR3& size)
{
	// 実体の生成
	FrameBorder* pFrameBorder = new FrameBorder;

	// 実体の初期化
	pFrameBorder->Init(position, size);

	// 生成された枠線のポインタを返す
	return pFrameBorder;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void FrameBorder::Init(D3DXVECTOR3& position, D3DXVECTOR3& size)
{
	// データの初期化
	{
		// 継承データの初期化
		Object2D::Init(position, size);

		// タイマーの生成
		size     = D3DXVECTOR3(GetSize().x * 0.25f, GetSize().y * 0.6f, 0.f);
		position = D3DXVECTOR3(GetPosition().x - GetHalfSize().x + GetSize().x * 0.01f, GetPosition().y * 1.1f, 0.f);
		m_pTimer = Timer::Create(position, size);
		m_pTimer->SetTexture_Number(MainGame::GetTexture(List_LoadTexture_MainGame::TEXTURE_NAME::NUMBER_TEXNAME)->GetTexture());
		m_pTimer->SetTexture_Colon(MainGame::GetTexture(List_LoadTexture_MainGame::TEXTURE_NAME::COLON_TEXNAME)->GetTexture());
		m_pTimer->SetTimer(7, 0);

		// チームゲージの生成
		size                = D3DXVECTOR3(GetSize().x * 0.6f, GetSize().y * 0.65f, 0.f);
		position            = D3DXVECTOR3(GetPosition().x - GetHalfSize().x + GetSize().x * 0.02f + GetSize().x * 0.25f + size.x * 0.5f, GetPosition().y * 1.1f, 0.f);
		m_pTeamGaugeManager = TeamGaugeManager::Create(position, size);

		// チームレベルの生成
		//size         = D3DXVECTOR3((GetSize().x * 0.2f), (GetSize().y * 0.6f), 0.f);
		//position     = D3DXVECTOR3((m_pTeamGauge->GetPosition().x + m_pTeamGauge->GetHalfSize().x + size.x * 0.5f), m_pTeamGauge->GetPosition().y , 0.f);
		//m_pTeamLevel = TeamLevel::Create(position, size);

	}


}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void FrameBorder::Uninit(void)
{
	// 継承データの終了処理
	Object2D::Uninit();

}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void FrameBorder::Update(void)
{
	// タイマー
	m_pTimer->Update();
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void FrameBorder::Draw(void)
{
	// 描画処理
	Object2D::Draw();

}



//=============================================================================
// 増減

//=============================================================================
// 設定
