//*****************************************************************************
//
//		チームゲージ
//													Author : Yusuke Seki
//*****************************************************************************
#include "TeamGauge.h"
#include "MainGame.h"
#include "list_LoadTexture_MainGame.h"
#include "Effect3D.h"

#include "Etc_Paramaters.h"


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
TeamGauge::TeamGauge(Object::TYPE type) : Object2D(type)
{
	// データのクリア
	m_life      = 0;	// 現在のチームゲージ量
	m_initWidth = 0.f;	// ポリゴンの初期幅

}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
TeamGauge::~TeamGauge()
{
	// 終了処理
	Uninit();

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
TeamGauge* TeamGauge::Create(D3DXVECTOR3& position, D3DXVECTOR3& size, Object::GROUP group)
{
	// 実体の生成
	TeamGauge* pTeamGauge = new TeamGauge;

	// 実体の初期化
	pTeamGauge->Init(position, size, group);

	// 生成されたタイマーのポインタを返す
	return pTeamGauge;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void TeamGauge::Init(D3DXVECTOR3& position, D3DXVECTOR3& size, Object::GROUP group)
{
	// データの初期化
	{
		// 継承データの初期化
		Object2D::Init(position, size);

		// 現在のチームゲージ量
		m_life = TEAMGAUGE_MAX_LIFE;

		// ポリゴンの初期幅
		m_initWidth = size.x;

		// 所属陣営
		SetGroup(group);

	}

}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void TeamGauge::Uninit(void)
{
	// 継承データの終了処理
	Object2D::Uninit();

}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void TeamGauge::Update(void)
{

}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void TeamGauge::Draw(void)
{
	// 描画処理
	Object2D::Draw();

}



//=============================================================================
// 増減
// ゲージ量の増減処理
void TeamGauge::MoveLife(float moveLife)
{
	// ゲージ量が0以下で無処理
	if (m_life <= 0) return;

	// 現在のゲージ量の更新
	m_life += moveLife;

	// 最大値からの割合の算出
	float percentage = m_life / TEAMGAUGE_MAX_LIFE;

	// ゲージの中心点と大きさの更新
	D3DXVECTOR3 size = GetSize();
	size.x = m_initWidth * percentage;
	D3DXVECTOR3 position = GetPosition();
	position.x -= (GetSize().x - size.x) * 0.5f;
	SetPosition(position);
	SetSize(size);

	// ゲージのUVサイズの更新
	D3DXVECTOR2 UVsize = GetUv_Size();
	UVsize.x = 1.0f * percentage;
	SetUv_Size(UVsize);

}

// true なら負け
bool TeamGauge::Lose() {

	// 【判定処理】残存ゲージ量の有無
	if (m_life <= 0) return true;	// 無し
	return false;					// 有り

}


