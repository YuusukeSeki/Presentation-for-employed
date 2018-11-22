// author : yusuke seki
// data   : 20181114
#include "Soldier.h"
#include "SoldierCommander.h"
#include "player.h"
#include "BasePoint.h"
#include "tower.h"
#include "castle.h"
#include "Collider.h"
#include "SoldierBullet.h"
#include "MainGame.h"
#include "collision.h"
#include "SoldierGauge.h"

const float Soldier::kBreakPower_ = 10.0f;
const float Soldier::kAttackPower_ = 50.0f;
const float Soldier::kMaxHp_ = 100.0f;;
const float Soldier::kSearchEnemyRange_ = 4.0f;
const float Soldier::kSearchFriendRange_ = 4.0f;

Soldier::Soldier() : Unit(TYPE_MODEL_SOLDIER)
{
	speed_ = 0.0f;
	currentHp_ = 0.0f;
	maxHp_ = 0.0f;
	attackPower_ = 0.0f;
	defencePower_ = 0.0f;
	breakPower_ = kBreakPower_;
	searchEnemyRange_ = 0.0f;
	searchFriendRange_ = 0.0f;
	parentCommander_ = nullptr;

	destination_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	frontAfterArriveDestination_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	targetBasePoint_ = nullptr;

	searchEnemyCollider_ = nullptr;
	searchFriendCollider_ = nullptr;

	Part* body = AddPart("body", "data/model/MainGame/soldier_body.x");

	Part* head = AddPart("head", "data/model/MainGame/soldier_head.x");
	head->SetParent(body);

	Part* arm_left = AddPart("arm_left", "data/model/MainGame/soldier_arm_left.x");
	arm_left->SetParent(body);

	Part* arm_right = AddPart("arm_right", "data/model/MainGame/soldier_arm_right.x");
	arm_right->SetParent(body);

	Part* leg_left = AddPart("leg_left", "data/model/MainGame/soldier_leg_left.x");
	leg_left->SetParent(body);

	Part* leg_right = AddPart("leg_right", "data/model/MainGame/soldier_leg_right.x");
	leg_right->SetParent(body);

	body->MovePosition(D3DXVECTOR3(0, 1, 0) * leg_right->GetSize().y);

	hpGauge_ = nullptr;
}

Soldier::~Soldier()
{
	Uninit();
}

void Soldier::CreateBuffer(const unsigned int& _numCreate)
{
	for (unsigned int i = 0; i < _numCreate; ++i)
	{
		Soldier* buf = new Soldier();
		buf->SetActive(false);
	}
}

Soldier* Soldier::Create(SoldierCommander* _soldierCommander)
{
	Soldier* soldier = new Soldier();
	soldier->Init(_soldierCommander);

	return soldier;
}

Soldier* Soldier::DynamicCreate(SoldierCommander* _soldierCommander)
{
	Soldier* soldier = FindNonActiveSoldier();

	if (soldier == nullptr)
	{
		soldier = Create(_soldierCommander);
	}
	else
	{
		soldier->Init(_soldierCommander);
	}

	return soldier;
}

void Soldier::Init(SoldierCommander* _soldierCommander)
{
	parentCommander_ = _soldierCommander;

	Unit::Init(parentCommander_->GetPosition(), parentCommander_->GetGroup());

	SetFront(parentCommander_->GetFront());
	speed_ = parentCommander_->GetSpeed();

	if (GetGroup() == Object::GROUP_A)
	{
		SetColor(255, 0, 0, 255);
		SetRotate(D3DXVECTOR3(0, 0, 0));
	}
	else if (GetGroup() == Object::GROUP_B)
	{
		SetColor(0, 0, 255, 255);
		SetRotate(D3DXVECTOR3(0, D3DXToRadian(180), 0));
	}

	GetObjectCollider()->SetRadius(GetHalfSize().x);

	maxHp_ = kMaxHp_;
	currentHp_ = kMaxHp_;
	attackPower_ = kAttackPower_;
	defencePower_ = 0.0f;
	searchEnemyRange_ = kSearchEnemyRange_;
	searchFriendRange_ = kSearchFriendRange_;

	destination_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	frontAfterArriveDestination_ = parentCommander_->GetFront();
	targetBasePoint_ = nullptr;

	if (searchEnemyCollider_ == nullptr)
	{
		searchEnemyCollider_ = Collider::Create(this);
	}
	else
	{
		searchEnemyCollider_->SetParentUnit(this);
	}
	searchEnemyCollider_->SetRadius(searchEnemyRange_);

	if (searchFriendCollider_ == nullptr)
	{
		searchFriendCollider_ = Collider::Create(this);
	}
	else
	{
		searchFriendCollider_->SetParentUnit(this);
	}
	searchFriendCollider_->SetRadius(searchFriendRange_);
	searchFriendCollider_->MovePosition(GetFront() * (searchFriendRange_ * 0.5f + GetHalfSize().z * 0.5f));

#ifdef _DEBUG
	searchEnemyCollider_->SetColor(1.0f, 0.3f, 0.3f, 0.5f);
	searchFriendCollider_->SetColor(0.3f, 0.3f, 1.0f, 0.5f);
	//searchEnemyCollider_->SetDraw(false);
	searchFriendCollider_->SetDraw(false);
#endif

	// hp gauge
	D3DXVECTOR3 gaugePosition = GetPosition();
	gaugePosition.y += GetSize().y * 1.2f;

	D3DXVECTOR3 gaugeSize(GetSize().x, GetSize().x * 0.25f, 0);

	hpGauge_ = SoldierGauge::DynamicCreate(gaugePosition, gaugeSize, MainGame::GetCamera(), this);
	hpGauge_->SetMaxHpPointer(&maxHp_);
	hpGauge_->SetCurrentHpPointer(&currentHp_);

	SetActive(true);
}

void Soldier::Uninit(void)
{
	Unit::Uninit();
}

void Soldier::Update(void)
{
	if (targetBasePoint_ == nullptr)
	{
		BasePoint* basePoint = SearchEnemyBasePoint();

		if (basePoint != nullptr)
		{
			parentCommander_->ReceiveReport(Report::FIND_ENEMY_BASEPOINT, this, basePoint);
		}
		else if (IsSearchEnemyUnit() == true)
		{
			parentCommander_->ReceiveReport(Report::FIND_ENEMY_UNIT, this);
		}
		else if (IsSearchFriendSoldier() == true)
		{
			parentCommander_->ReceiveReport(Report::FIND_FRIEND_UNIT, this);
		}
	}

}

void Soldier::Draw(void)
{
	Unit::Draw();
}

void Soldier::Stop()
{
	// –³ˆ—
}

void Soldier::Run()
{
	SetFront(parentCommander_->GetFront());

	MovePosition(GetFront(), speed_);
	SetRotateToPosition(GetPosition() + GetFront() * speed_);
}

void Soldier::FormLine()
{
	float distance = Distance3D(destination_, GetPosition());

	if (distance < speed_ * speed_)
	{
		SetPosition(destination_);

		SetRotateToPosition(GetPosition() + frontAfterArriveDestination_ * speed_);
	}
	else
	{
		D3DXVECTOR3 moveVector = destination_ - GetPosition();
		D3DXVec3Normalize(&moveVector, &moveVector);

		SetRotateToPosition(GetPosition() + moveVector * speed_);
		MovePosition(moveVector, speed_);
	}
}

void Soldier::ShotBullet()
{
	Unit* nearEnemy = FindNearEnemy();

	if (nearEnemy != nullptr)
	{
		Camera* camera = MainGame::GetCamera();

		SetRotateToPosition(nearEnemy->GetPosition());

		SoldierBullet::DynamicCreate(this, nearEnemy);
	}
}

void Soldier::AssaultBasePoint()
{
	D3DXVECTOR3 vector = targetBasePoint_->GetPosition() - GetPosition();
	D3DXVec3Normalize(&vector, &vector);

	MovePosition(vector * speed_);

	if (CollidedBasePoint() == true)
	{
		BreakBasePoint();
		Death();
	}
}

void Soldier::Death()
{
	parentCommander_->ReceiveReport(Report::DEATH, this);

	SetActive(false);
}

void Soldier::SetDestination(const D3DXVECTOR3& _destinationPoint)
{
	destination_ = _destinationPoint;
}

void Soldier::SetEndFront(const D3DXVECTOR3& _endFront)
{
	frontAfterArriveDestination_ = _endFront;
}

void Soldier::SetTargetBasePoint(BasePoint* _targetBasePoint)
{
	targetBasePoint_ = _targetBasePoint;
}

Collider* Soldier::GetSearchCollider()
{
	return searchEnemyCollider_;
}

void Soldier::ReceiveDamage(const float& _damage, Unit* _unit)
{
	if (_unit != nullptr)
	{
		//// ‚«”ò‚Î‚µˆ—
		//float speed = 5.0f;
		//D3DXVECTOR3 moveVector = GetPosition() - _unit->GetPosition();
		//D3DXVec3Normalize(&moveVector, &moveVector);

		//MovePosition(moveVector, speed);
	}

	currentHp_ -= _damage;

	if (currentHp_ <= 0.0f)
	{
		Death();
	}
}

void Soldier::SetActive(const bool& _isActive)
{
	if (searchEnemyCollider_ != nullptr)
	{
		searchEnemyCollider_->SetActive(_isActive);
	}

	if (searchFriendCollider_ != nullptr)
	{
		searchFriendCollider_->SetActive(_isActive);
	}

	if (hpGauge_ != nullptr)
	{
		hpGauge_->SetActive(_isActive);
	}

	Unit::SetActive(_isActive);
}

Soldier* Soldier::FindNonActiveSoldier()
{
	Soldier* soldier = (Soldier*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_MODEL_SOLDIER);

	if (soldier != nullptr)
	{
		for (;;)
		{
			if (soldier->GetActive() == false)
			{
				break;
			}

			soldier = (Soldier*)soldier->GetNextPointer();

			if (soldier == nullptr)
			{
				break;
			}
		}
	}

	return soldier;
}

Unit* Soldier::FindNearEnemy()
{
	Soldier* nearSoldier = FindNearEnemySoldier();
	Player* nearPlayer = FindNearEnemyPlayer();

	if (nearSoldier != nullptr && nearPlayer != nullptr)
	{
		float distance = Distance3D(GetPosition(), nearSoldier->GetPosition());

		if (distance < Distance3D(GetPosition(), nearPlayer->GetPosition()))
		{
			return (Unit*)nearSoldier;
		}
		else
		{
			return (Unit*)nearPlayer;
		}
	}
	else if (nearSoldier != nullptr && nearPlayer == nullptr)
	{
		return nearSoldier;
	}
	else if (nearSoldier == nullptr && nearPlayer != nullptr)
	{
		return nearPlayer;
	}
	else
	{
		return nullptr;
	}
}

Soldier* Soldier::FindNearEnemySoldier()
{
	Soldier* soldier = (Soldier*)Object::GetLDATA_HEAD(Object::TYPE_MODEL_SOLDIER);

	if (soldier != nullptr)
	{
		Soldier* nearEnemySoldier = nullptr;
		float minDistance = 0.0f;

		for (;;)
		{
			if (soldier->GetActive() == true && soldier->GetGroup() != GetGroup())
			{
				float distance = Distance3D(GetPosition(), soldier->GetPosition());

				if (distance < minDistance || nearEnemySoldier == nullptr)
				{
					minDistance = distance;
					nearEnemySoldier = soldier;
				}
			}

			soldier = (Soldier*)soldier->GetNextPointer();

			if (soldier == nullptr)
			{
				break;
			}
		}

		return nearEnemySoldier;

	}

	return nullptr;
}

Player* Soldier::FindNearEnemyPlayer()
{
	Player* player = (Player*)Object::GetLDATA_HEAD(Object::TYPE_MODEL_PLAYER);

	if (player == nullptr)
	{
		return nullptr;
	}
	else
	{
		Player* nearEnemyPlayer = nullptr;
		float minDistance = 0.0f;

		for (;;)
		{
			if (player->GetGroup() != GetGroup())
			{
				float distance = Distance3D(GetPosition(), player->GetPosition());

				if (distance < minDistance || nearEnemyPlayer == nullptr)
				{
					minDistance = distance;
					nearEnemyPlayer = player;
				}
			}

			player = (Player*)player->GetNextPointer();

			if (player == nullptr)
			{
				break;
			}
		}

		return nearEnemyPlayer;
	}
}

BasePoint* Soldier::SearchEnemyBasePoint()
{
	Tower* tower = SearchEnemyTower();

	if (tower != nullptr)
	{
		return tower;
	}

	Castle* castle = SearchEnemyCastle();

	if (castle != nullptr)
	{
		return castle;
	}

	return nullptr;
}

Tower* Soldier::SearchEnemyTower()
{
	Tower* tower = (Tower*)Object::GetLDATA_HEAD(TYPE_MODEL_TOWER);

	if (tower != nullptr)
	{
		for (;;) {

			if (tower->GetGroup() != GetGroup() && tower->IsBreak() == false)
			{
				if (searchEnemyCollider_->Collision(tower->GetObjectCollider()) == true)
				{
					return tower;
				}
			}

			tower = (Tower*)tower->GetNextPointer();

			if (tower == nullptr)
			{
				return nullptr;
			}
		}
	}

	return nullptr;

}

Castle* Soldier::SearchEnemyCastle()
{
	Castle* castle = (Castle*)Object::GetLDATA_HEAD(TYPE_MODEL_CASTLE);

	if (castle != nullptr)
	{
		for (;;) {

			if (castle->GetGroup() != GetGroup())
			{
				if (Collision_SphereToSphere(GetPosition(), searchEnemyRange_, castle->GetPosition(), castle->GetRadius()))
				{
					return castle;
				}
			}

			castle = (Castle*)castle->GetNextPointer();

			if (castle == nullptr)
			{
				return nullptr;
			}
		}
	}

	return nullptr;
}

bool Soldier::IsSearchEnemyUnit()
{
	if (IsSearchEnemySoldier() == true || IsSearchEnemyPlayer() == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Soldier::IsSearchEnemySoldier()
{
	Soldier* soldier = (Soldier*)Object::GetLDATA_HEAD(TYPE_MODEL_SOLDIER);

	if (soldier != nullptr)
	{
		for (;;)
		{
			if (soldier->GetActive() == true && soldier->GetGroup() != GetGroup())
			{
				if (searchEnemyCollider_->Collision(soldier->GetObjectCollider()) == true)
				{
					return true;
				}
			}

			soldier = (Soldier*)soldier->GetNextPointer();

			if (soldier == nullptr)
			{
				break;
			}
		}
	}

	return false;
}

bool Soldier::IsSearchEnemyPlayer()
{
	Player* player = (Player*)Object::GetLDATA_HEAD(TYPE_MODEL_PLAYER);

	if (player != nullptr)
	{
		for (;;)
		{
			if (player->GetGroup() != GetGroup())
			{
				if (searchEnemyCollider_->Collision(player->GetObjectCollider()) == true)
				{
					return true;
				}
			}

			player = (Player*)player->GetNextPointer();

			if (player == nullptr)
			{
				break;
			}
		}
	}

	return false;
}

bool Soldier::IsSearchFriendSoldier()
{
	SoldierCommander* soldierCommander = (SoldierCommander*)Object::GetLDATA_HEAD(TYPE_COMMANDER);

	if (soldierCommander != nullptr)
	{
		for (;;)
		{
			if (soldierCommander->GetActive() == true && soldierCommander->GetGroup() == GetGroup() && soldierCommander != parentCommander_)
			{
				if (soldierCommander->GetTargetRelayPoint() == parentCommander_->GetTargetRelayPoint())
				{
					if (searchFriendCollider_->Collision(soldierCommander->GetPosition()) == true)
					{
						return true;
					}
				}
			}

			soldierCommander = (SoldierCommander*)soldierCommander->GetNextPointer();

			if (soldierCommander == nullptr)
			{
				break;
			}
		}
	}

	return false;
}

bool Soldier::CollidedBasePoint()
{
	return targetBasePoint_->GetObjectCollider()->Collision(GetPosition());
}

void Soldier::BreakBasePoint()
{
	targetBasePoint_->ReceiveDamage(breakPower_, this);
}
