// author : yusuke seki
// data   : 20181116
#include "tower.h"
#include "collision.h"
#include "SoldierCommander.h"
#include "ObjectBillboard.h"
#include "MainGame.h"

const unsigned int Tower::kNumSoldier_First_ = 9;
const unsigned int Tower::kNumSoldier_Subsequent_ = 5;
const float Tower::kMaxHp_ = 100.0f;

Tower::Tower(const Object::TYPE& _type) : BasePoint(_type, kNumSoldier_First_, kNumSoldier_Subsequent_)
{
	maxHp_ = 0.0f;
	currentHp_ = 0.0f;
	icon_ = nullptr;
	hpGauge_ = nullptr;
}

Tower::~Tower()
{
	Uninit();
}

Tower* Tower::Create(const D3DXVECTOR3& _position, const Object::GROUP& _group, RelayPoint* _nextRelayPoint)
{
	Tower* tower = new Tower(Object::TYPE::TYPE_MODEL_TOWER);
	tower->Init(_position, _group, _nextRelayPoint);

	return tower;
}

void Tower::Init(const D3DXVECTOR3& _position, const Object::GROUP& _group, RelayPoint* _nextRelayPoint)
{
	BasePoint::Init(_position, _group, _nextRelayPoint);

	AddPart("tower", "data/model/MainGame/tower05.x");
	SetBrowedRange(GetHalfSize().x + 3.5f);

	maxHp_ = kMaxHp_;
	currentHp_ = maxHp_;

	releaseTimer_ = 120;

	if (icon_ == nullptr)
	{
		D3DXVECTOR3 iconPosition, iconSize;
		iconSize = D3DXVECTOR3(496, 406, 0) * 0.01f;
		iconPosition = GetPosition();
		iconPosition.y += GetSize().y + iconSize.y * 0.5f;

		icon_ = ObjectBillboard::Create(iconPosition, iconSize, MainGame::GetCamera());
		icon_->SetColor(GetColor().color);
		icon_->SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::ICON_TOWER)->GetTexture());
	}
}

void Tower::Uninit(void)
{
	BasePoint::Uninit();
}

void Tower::Update(void)
{
	if (IsBreak() == true)
	{
		--releaseTimer_;

		if (releaseTimer_ == 0)
		{
			icon_->Release();
			GetBrowedCollider()->Release();
			GetObjectCollider()->Release();
			ReleaseParts();
			Release();
		}
	}
}

void Tower::Draw(void)
{
	BasePoint::Draw();
}

void Tower::ReceiveDamage(const float& _damage, Unit* _unit)
{
	if (IsBreak() == false)
	{
		currentHp_ -= _damage;

		if (IsBreak() == true)
		{
			SendIsBrakedSoldierGenerator();
		}
	}
}

float Tower::GetMaxHp()
{
	return maxHp_;
}

float Tower::GetCurrentHp()
{
	return currentHp_;
}

bool Tower::IsBreak()
{
	if (currentHp_ <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//bool Tower::BrowTower(float breakPower)
//{
//	m_life -= breakPower;
//
//	if (m_life < 0)
//	{
//		breakPower = breakPower + m_life;
//	}
//
//	m_pLifeGauge->MoveLife(breakPower);
//
//	TeamGaugeManager::GetTeamGauge_ENEMY()->MoveLife(-breakPower);
//
//	if (m_life <= 0)
//	{
//		TeamGaugeManager::GetTeamGauge_ENEMY()->MoveLife(-50);
//
//		m_pHold      ->ReleaseThis();	// "HOLD"
//		m_pLifeGauge ->Release();		// 体力ゲージ
//		m_pIcon      ->Release();		// アイコン
//
//		Release();
//
//		return true;
//	}
//	return false;
//
//}
//
//void Tower::MovePosition(D3DXVECTOR3& movePosition)
//{
//	BasePoint::MovePosition(movePosition);
//
//	m_pHold->MovePosition(movePosition);
//	m_pLifeGauge->MovePosition(movePosition);
//	m_pIcon->MovePosition(movePosition);
//}
//
//bool Tower::CollisionBrowRange(D3DXVECTOR3& position)
//{
//	if (Distance3D(GetPosition(), position) <= m_browRange * m_browRange)
//		return true;
//
//	return false;
//}
//
