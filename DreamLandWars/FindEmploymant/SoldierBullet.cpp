// author : yusuke seki
// data   : 20181121
#include "SoldierBullet.h"
#include "player.h"
#include "Soldier.h"
#include "collision.h"
#include "ObjectBillboard.h"
#include "MainGame.h"
#include "camera.h"

const float SoldierBullet::kDamage_ = 50.f;
const float SoldierBullet::kSpeed_ = 0.15f;
const unsigned int SoldierBullet::kEraseFrame_ = 240;

SoldierBullet::SoldierBullet() : Unit(Object::TYPE::TYPE_3D_SOLDIERBULLET)
{
	target_ = nullptr;
	vector_ = D3DXVECTOR3(0,0,0);
	isChase_ = false;
	remaineFrame_ = 0;
	objectBillboard_ = nullptr;
}

SoldierBullet::~SoldierBullet()
{
	Uninit();
}

void SoldierBullet::CreateBuffer(const unsigned int& _numCreate)
{
	for (unsigned int i = 0; i < _numCreate; ++i)
	{
		SoldierBullet* buf = new SoldierBullet();
		buf->SetActive(false);
	}
}

SoldierBullet* SoldierBullet::Create(Soldier* _parentSoldier, Unit* _target)
{
	SoldierBullet* soldierBullet = new SoldierBullet();
	soldierBullet->Init(_parentSoldier, _target);

	return soldierBullet;
}

SoldierBullet* SoldierBullet::DynamicCreate(Soldier* _parentSoldier, Unit* _target)
{
	SoldierBullet* soldierBullet = FindNonActiveSoldierBullet();

	if (soldierBullet == nullptr)
	{
		soldierBullet = Create(_parentSoldier, _target);
	}
	else
	{
		soldierBullet->Init(_parentSoldier, _target);
	}

	return soldierBullet;
}

void SoldierBullet::Init(Soldier* _parentSoldier, Unit* _target)
{
	D3DXVECTOR3 drawObjectSize(0.5f, 0.5f, 0);
	D3DXVECTOR3 drawObjectPosition(0, _parentSoldier->GetHalfSize().y, 0);

	Unit::Init(_parentSoldier->GetPosition(), _parentSoldier->GetGroup());

	target_ = _target;

	vector_ = target_->GetPosition() - GetPosition();
	D3DXVec3Normalize(&vector_, &vector_);

	isChase_ = true;

	remaineFrame_ = kEraseFrame_;

	objectBillboard_ = ObjectBillboard::DynamicCreate(drawObjectPosition, drawObjectSize, MainGame::GetCamera());
	objectBillboard_->SetParent(this);
	objectBillboard_->SetColor(_parentSoldier->GetColor().color);
	objectBillboard_->SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::SOLDIERBULLET)->GetTexture());

	GetObjectCollider()->SetRadius(objectBillboard_->GetHalfSize().x);

	SetActive(true);
}

void SoldierBullet::Uninit(void)
{
	Unit::Uninit();
}

void SoldierBullet::Update(void)
{
	CheckTargetBehave();

	ChangeVector();

	Move();

	CollisionTarget();

	UpdateTimer();
}

void SoldierBullet::Draw(void)
{
	Unit::Draw();
}

void SoldierBullet::SetActive(const bool& _isActive)
{
	objectBillboard_->SetActive(_isActive);

	Unit::SetActive(_isActive);
}

SoldierBullet* SoldierBullet::FindNonActiveSoldierBullet()
{
	SoldierBullet* soldierBullet = (SoldierBullet*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_3D_SOLDIERBULLET);

	if (soldierBullet != nullptr)
	{
		for (;;)
		{
			if (soldierBullet->GetActive() == false)
			{
				break;
			}

			soldierBullet = (SoldierBullet*)soldierBullet->GetNextPointer();

			if (soldierBullet == nullptr)
			{
				break;
			}
		}
	}

	return soldierBullet;
}

void SoldierBullet::CheckTargetBehave()
{
	if (isChase_ == true)
	{
		if (IsTargetBahave_Avoid() == true)
		{
			isChase_ = false;
		}
	}
}

void SoldierBullet::ChangeVector()
{
	if (isChase_ == true)
	{
		SetVectorToTarget();
	}
}

void SoldierBullet::Move()
{
	MovePosition(vector_ * kSpeed_);

	objectBillboard_->SetUpdateWorldMatrix(true);
}

void SoldierBullet::CollisionTarget()
{
	if (IsCollisionTarget() == true)
	{
		target_->ReceiveDamage(kDamage_, this);
		SetActive(false);
	}
}

void SoldierBullet::UpdateTimer()
{
	--remaineFrame_;

	if (remaineFrame_ == 0)
	{
		SetActive(false);
	}
}

bool SoldierBullet::IsTargetBahave_Avoid()
{
	if (target_->GetType() == Object::TYPE_MODEL_PLAYER)
	{
		Player* player = (Player*)target_;

		if (player->GetBehave() == Player::Behave::AVOID || player->GetBehave() == Player::Behave::HEAVY_AVOID)
		{
			return true;
		}
	}

	return false;
}

void SoldierBullet::SetVectorToTarget()
{
	vector_ = target_->GetPosition() - GetPosition();
	D3DXVec3Normalize(&vector_, &vector_);
}

bool SoldierBullet::IsCollisionTarget()
{
	if (target_->GetObjectCollider()->Collision(GetPosition()) == true)
	{
		return true;
	}

	return false;
}
