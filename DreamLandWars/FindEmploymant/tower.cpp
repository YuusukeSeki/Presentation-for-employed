//*****************************************************************************
//
//		塔
//													Autohr : Yusuke Seki
//*****************************************************************************
// base
#include "tower.h"

// fw
#include "collision.h"

// child
#include "Hold.h"
#include "LifeGauge.h"
#include "Icon.h"
#include "SoldierCommander.h"

// etc...
#include "player.h"
#include "TeamGaugeManager.h"
#include "TeamGauge.h"

// resource
#include "MainGame.h"
#include "list_LoadTexture_MainGame.h"
#include "Etc_Paramaters.h"



//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Tower::Tower() : ObjectModel(Object::TYPE::TYPE_MODEL_TOWER)
{
	// メンバ変数初期化
	m_life      = 0;	// 体力
	m_browRange = 0.f;	// 殴れる範囲

	m_pHold      = nullptr;	// "HOLD"
	m_pLifeGauge = nullptr;	// 体力ゲージ
	m_pIcon      = nullptr;	// アイコン
	
}


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Tower::Tower(Object::TYPE type) : ObjectModel(type)
{
	// メンバ変数初期化
	m_life      = 0;	// 体力
	m_browRange = 0.f;	// 殴れる範囲

	m_pHold      = nullptr;	// "HOLD"
	m_pLifeGauge = nullptr;	// 体力ゲージ
	m_pIcon      = nullptr;	// アイコン
	
}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Tower::~Tower()
{
	Uninit();

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
Tower* Tower::Create(D3DXVECTOR3& position, const char* FileName, Object::GROUP group)
{
	Tower* pTower = new Tower(Object::TYPE::TYPE_MODEL_TOWER);
	pTower->Init(position, FileName, group);

	return pTower;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void Tower::Init(D3DXVECTOR3& position, const char* FileName, Object::GROUP group)
{
	// データの設定
	ObjectModel::Init(position, FileName);	// 継承データの初期化
	m_life      = TOWER_MAX_LIFE;	// 体力
	m_browRange = GetSize().x * 1.05f;		// 殴れる範囲
	SetGroup(group);
	
	// 色の設定
	if      (GetGroup() == Object::GROUP::GROUP_A) SetColor(0xff0000a0);	// 赤色
	else if (GetGroup() == Object::GROUP::GROUP_B) SetColor(0x0000ffa0);	// 青色

	m_cntFrame = 1500;	// 兵士生成用カウンター

	// get camera of this client
	// ※下記は暫定処理
	Camera* camera = MainGame::GetPlayer(0)->GetCamera();

	// ポリゴンの生成
	m_pLifeGauge = LifeGauge::Create(this, camera);	// 体力ゲージ
	m_pHold      = Hold::Create(this, camera);		// "HOLD"
	m_pIcon      = Icon::Create(m_pLifeGauge->GetPosition() + D3DXVECTOR3(0, m_pLifeGauge->GetSize().y * 1.f, 5),
								D3DXVECTOR3(m_pLifeGauge->GetSize().x * 0.3f, m_pLifeGauge->GetSize().x * 0.3f, 0), camera, group);	// アイコン
	
	m_pIcon->SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::ICON_CASTLE)->GetTexture());

	
}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Tower::Uninit(void)
{
	// 継承データの終了処理
	ObjectModel::Uninit();

}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void Tower::Update(void)
{
	m_cntFrame++;

	if (m_cntFrame >= INTERVAL_CREATE_SOLDIER) {

		Commander::SetCommander(GetPosition(), GetFront(), GetGroup());
		m_cntFrame = 0;

	}

	// "HOLD"表示判定
	{
		Player* pPlayer = (Player*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_MODEL_PLAYER);

		if (pPlayer == nullptr) return;

		Player* pCurrent = pPlayer;
		Player* pNext    = (Player*)pCurrent->GetNextPointer();

		for (;;) {

			if (pCurrent->GetGroup() != GetGroup()) {

				float length = Distance3D(pCurrent->GetPosition(), GetPosition());

				if (length <= (m_browRange) * (m_browRange)) m_pHold->SetDrawHold(true);
				else                                                       m_pHold->SetDrawHold(false);

			}

			pCurrent = pNext;

			if (pCurrent == nullptr) break;

			pNext = (Player*)pCurrent->GetNextPointer();

		}

	}


}


//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void Tower::Draw(void)
{
	// 描画処理は親元に丸投げ
	ObjectModel::Draw();

}


// 体力を減らす
// true - 拠点が壊れた : false - 拠点が壊れてない
bool Tower::BrowTower(float breakPower)
{
	m_life -= breakPower;

	if (m_life < 0)
	{
		breakPower = breakPower + m_life;
	}

	m_pLifeGauge->MoveLife(breakPower);

	TeamGaugeManager::GetTeamGauge_ENEMY()->MoveLife(-breakPower);

	if (m_life <= 0)
	{
		TeamGaugeManager::GetTeamGauge_ENEMY()->MoveLife(-50);

		m_pHold      ->ReleaseThis();	// "HOLD"
		m_pLifeGauge ->Release();		// 体力ゲージ
		m_pIcon      ->Release();		// アイコン

		Release();

		return true;
	}
	return false;

}

// 座標の移動
void Tower::MovePosition(D3DXVECTOR3& movePosition)
{
	ObjectModel::MovePosition(movePosition);

	m_pHold->MovePosition(movePosition);
	m_pLifeGauge->MovePosition(movePosition);
	m_pIcon->MovePosition(movePosition);

}

// true なら塔を殴れる範囲内
bool Tower::CollisionBrowRange(D3DXVECTOR3& position)
{
	if (Distance3D(GetPosition(), position) <= m_browRange * m_browRange)
		return true;

	return false;

}

