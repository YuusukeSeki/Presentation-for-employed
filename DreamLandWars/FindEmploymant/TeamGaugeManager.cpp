//*****************************************************************************
//
//		チームゲージマネージャー
//													Author : Yusuke Seki
//*****************************************************************************
#include "TeamGaugeManager.h"
#include "MainGame.h"
#include "list_LoadTexture_MainGame.h"
#include "TeamGauge.h"


TeamGauge* TeamGaugeManager::m_pTeamGauge_YOURTEAM = nullptr;
TeamGauge* TeamGaugeManager::m_pTeamGauge_ENEMY    = nullptr;

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
TeamGaugeManager::TeamGaugeManager(Object::TYPE type) : Object2D(type)
{
	// データのクリア
	m_pLabel_YOURTEAM     = nullptr;	// 【ラベル】"YOUR TEAM"
	m_pLabel_ENEMY        = nullptr;	// 【ラベル】"ENEMY"
	m_pTeamGauge_YOURTEAM = nullptr;	// 【ゲージ】"YOUR TEAM"
	m_pTeamGauge_ENEMY    = nullptr;	// 【ゲージ】"ENEMY"

}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
TeamGaugeManager::~TeamGaugeManager()
{
	// 終了処理
	Uninit();

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
TeamGaugeManager* TeamGaugeManager::Create(D3DXVECTOR3& position, D3DXVECTOR3& size)
{
	// 実体の生成
	TeamGaugeManager* pTeamGaugeManager = new TeamGaugeManager;

	// 実体の初期化
	pTeamGaugeManager->Init(position, size);

	// 生成されたタイマーのポインタを返す
	return pTeamGaugeManager;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void TeamGaugeManager::Init(D3DXVECTOR3& position, D3DXVECTOR3& size)
{
	// データの初期化
	{
		// 継承データの初期化
		Object2D::Init(position, size);


		// ラベル："YOUR TEAM"
		size              = D3DXVECTOR3(GetSize().x * 0.1f                               , GetSize().y     * 0.525f, 0.f);
		position          = D3DXVECTOR3(GetPosition().x - GetHalfSize().x + size.x * 0.5f, GetPosition().y * 1.2f, 0.f);
		m_pLabel_YOURTEAM = Object2D::Create(position, size);
		m_pLabel_YOURTEAM->SetColor(0xff0000ff);
		m_pLabel_YOURTEAM->SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::TEXTURE_NAME::LABEL_YOURTEAM_TEXNAME)->GetTexture());

		// チームゲージ（自分）
		size                  = D3DXVECTOR3(GetSize().x * 0.85f                                                                     , m_pLabel_YOURTEAM->GetSize().y    , 0.f);
		position              = D3DXVECTOR3(m_pLabel_YOURTEAM->GetPosition().x + m_pLabel_YOURTEAM->GetHalfSize().x + size.x * 0.55f, m_pLabel_YOURTEAM->GetPosition().y, 0.f);
		m_pTeamGauge_YOURTEAM = TeamGauge::Create(position, size, Object::GROUP::GROUP_A);
		m_pTeamGauge_YOURTEAM->SetColor(0xff0000ff);
		m_pTeamGauge_YOURTEAM->SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::TEXTURE_NAME::TEAMGAUGE_TEXNAME)->GetTexture());

		// "ENEMY"
		size           = D3DXVECTOR3(GetSize().x * 0.10f                              , GetSize().y * 0.25f    , 0.f);
		position       = D3DXVECTOR3(GetPosition().x - GetHalfSize().x + size.x * 0.5f, GetPosition().y * 0.75f, 0.f);
		m_pLabel_ENEMY = Object2D::Create(position, size);
		m_pLabel_ENEMY->SetColor(0x00ffffff);
		m_pLabel_ENEMY->SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::TEXTURE_NAME::LABEL_ENEMY_TEXNAME)->GetTexture());

		// チームゲージ（相手）
		size               = D3DXVECTOR3(GetSize().x * 0.85f                                                               , GetSize().y * 0.30f            , 0.f);
		position           = D3DXVECTOR3(m_pLabel_ENEMY->GetPosition().x + m_pLabel_ENEMY->GetHalfSize().x + size.x * 0.55f, m_pLabel_ENEMY->GetPosition().y, 0.f);
		m_pTeamGauge_ENEMY = TeamGauge::Create(position, size, Object::GROUP::GROUP_B);
		m_pTeamGauge_ENEMY->SetColor(0x00ffffff);
		m_pTeamGauge_ENEMY->SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::TEXTURE_NAME::TEAMGAUGE_TEXNAME)->GetTexture());

	}


}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void TeamGaugeManager::Uninit(void)
{
	// 継承データの終了処理
	Object2D::Uninit();

}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void TeamGaugeManager::Update(void)
{

}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void TeamGaugeManager::Draw(void)
{
	// こいつ自身は描画しない
	return;

	// 描画処理
	Object2D::Draw();

}



//=============================================================================
// 増減

//=============================================================================
// 設定
