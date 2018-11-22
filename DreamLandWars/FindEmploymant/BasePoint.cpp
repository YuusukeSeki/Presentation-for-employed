// author : yusuke seki
// data   : 20181115
#include "BasePoint.h"
#include "player.h"
#include "SoldierGenerator.h"
#include "Hold.h"
#include "LifeGauge.h"
#include "Icon.h"

const unsigned int BasePoint::kNumFrame_CreateSoldierLine_First_ = 180;
const unsigned int BasePoint::kNumFrame_CreateSoldierLine_Subsequent_ = 900;

BasePoint::BasePoint(const Object::TYPE& _type, const unsigned int& _kNumSoldier_First, const unsigned int& _kNumSoldier_Subsequen)
	: Unit(_type), kNumSoldier_First_(_kNumSoldier_First), kNumSoldier_Subsequent_(_kNumSoldier_Subsequen)
{
	soldierGenerator_ = nullptr;

	browedRange_ = 0.0f;
	browedCollider_ = nullptr;

	nextRelayPoint_ = nullptr;
}

BasePoint::~BasePoint()
{
	Uninit();
}

void BasePoint::Release()
{
	Unit::Release();
}

void BasePoint::Init(const D3DXVECTOR3& _position, const Object::GROUP& _group, RelayPoint* _nextRelayPoint)
{
	Unit::Init(_position, _group);

	nextRelayPoint_ = _nextRelayPoint;

	if (GetGroup() == Object::GROUP::GROUP_A)
	{
		SetRotate(D3DXVECTOR3(0, D3DXToRadian(0), 0));
		SetFront(D3DXVECTOR3(0, 0, 1));
		SetColor(255, 64, 64, 255);
	}
	else
	{
		SetRotate(D3DXVECTOR3(0, D3DXToRadian(180), 0));
		SetFront(D3DXVECTOR3(0, 0, -1));
		SetColor(64, 64, 255, 255);
	}

	if (browedCollider_ == nullptr)
	{
		browedCollider_ = Collider::Create(this);
	}

#ifdef _DEBUG
	browedCollider_->SetColor(0.3f, 1.0f, 0.3f, 0.5f);
	//browedCollider_->SetDraw(false);
#endif

	if (soldierGenerator_ == nullptr)
	{
		soldierGenerator_ = SoldierGenerator::Create(this
			, kNumFrame_CreateSoldierLine_First_, kNumSoldier_First_
			, kNumFrame_CreateSoldierLine_Subsequent_, kNumSoldier_Subsequent_);
	}
	else
	{
		soldierGenerator_->Init(this);
	}

	soldierGenerator_->SetIsUpdateTimer(true);
}

void BasePoint::Uninit()
{
	Unit::Uninit();
}

void BasePoint::Update()
{

}

void BasePoint::Draw()
{
	Unit::Draw();
}

void BasePoint::SetBrowedRange(const float& _browedRange)
{
	browedRange_ = _browedRange;

	browedCollider_->SetRadius(browedRange_);
}

Collider* BasePoint::GetBrowedCollider()
{
	return browedCollider_;
}

RelayPoint* BasePoint::GetNextRelayPoint()
{
	return nextRelayPoint_;
}

bool BasePoint::IsBrowedRange(Player* _player)
{
	if (browedCollider_->Collision(_player->GetObjectCollider()) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void BasePoint::SendIsBrakedSoldierGenerator()
{
	if (soldierGenerator_ != nullptr)
	{
		soldierGenerator_->ReceiveReport_BreakedParent();
		soldierGenerator_ = nullptr;
	}

	browedCollider_->Uninit();

	GetObjectCollider()->Uninit();
}
