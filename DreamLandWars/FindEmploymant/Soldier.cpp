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

const std::string Soldier::kSoldierModelPath_ = "data/model/MainGame/soldier03.x";
const float Soldier::kBreakPower_ = 10.0f;
const float Soldier::kAttackPower_ = 50.0f;
const float Soldier::kMaxHp_ = 100.0f;;
const float Soldier::kSearchRange_ = 10.0f;

Soldier::Soldier() : Unit(TYPE_MODEL_SOLDIER)
{
	speed_ = 0.0f;
	currentHp_ = 0.0f;
	maxHp_ = 0.0f;
	attackPower_ = 0.0f;
	defencePower_ = 0.0f;
	breakPower_ = kBreakPower_;
	searchRange_ = 0.0f;
	parentCommander_ = nullptr;

	destination_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	frontAfterArriveDestination_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	targetBasePoint_ = nullptr;

	objectCollider_ = nullptr;
	searchCollider_ = nullptr;
}

Soldier::~Soldier()
{
	Uninit();
}

void Soldier::CreateBuffer(const unsigned int& _numCreate)
{
	for (unsigned int i = 0; i < _numCreate; ++i)
	{
		new Soldier();
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

	SetPosition(parentCommander_->GetPosition());
	SetFront(parentCommander_->GetFront());
	speed_ = parentCommander_->GetSpeed();
	SetGroup(parentCommander_->GetGroup());

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

	maxHp_ = kMaxHp_;
	currentHp_ = kMaxHp_;
	attackPower_ = kAttackPower_;
	defencePower_ = 0.0f;
	searchRange_ = kSearchRange_;

	destination_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	frontAfterArriveDestination_ = parentCommander_->GetFront();
	targetBasePoint_ = nullptr;

	objectCollider_ = Collider::Create(this);

	searchCollider_ = Collider::Create(this);
	searchCollider_->SetRadius(searchRange_);
}

void Soldier::Uninit(void)
{
	Unit::Uninit();
}

void Soldier::Update(void)
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
	else
	{
		parentCommander_->ReceiveReport(Report::NONE, this);
	}
}

void Soldier::Draw(void)
{

}

void Soldier::Stop()
{
	// ñ≥èàóù
}

void Soldier::Run()
{
	MovePosition(GetFront(), speed_);
}

void Soldier::FormLine()
{
	const float kErrorDistance = 1.0f;
	float distance = Distance3D(destination_, GetPosition());

	if (distance < speed_ * speed_)
	{
		SetPosition(destination_);
	}
	else
	{
		D3DXVECTOR3 moveVector = destination_ - GetPosition();
		D3DXVec3Normalize(&moveVector, &moveVector);

		MovePosition(moveVector, speed_);
	}
}

void Soldier::ShotBullet()
{
	Unit* nearEnemy = FindNearEnemy();

	if (nearEnemy != nullptr)
	{
		Camera* camera = MainGame::GetPlayer(0)->GetCamera();

		SetRotateToPosition(nearEnemy->GetPosition());
		SoldierBullet::SetBullet(GetPosition(), nearEnemy, camera);
	}
}

void Soldier::AssaultBasePoint()
{
	if (CollidedBasePoint() == true)
	{
		BreakBasePoint();
		SelfDelete();
	}
}

void Soldier::SelfDelete()
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

void Soldier::SetTargetBasePoint(ObjectModel* _targetBasePoint)
{
	targetBasePoint_ = _targetBasePoint;
}

void Soldier::ReceiveDamage(const float& _damage, const D3DXVECTOR3& _bulletPosition)
{
	float speed = 5.0f;
	D3DXVECTOR3 moveVector = GetPosition() - _bulletPosition;
	D3DXVec3Normalize(&moveVector, &moveVector);

	MovePosition(moveVector, speed);

	currentHp_ -= _damage;

	if (currentHp_ <= 0.0f)
	{
		parentCommander_->ReceiveReport(Report::DEATH, this);
		SetActive(false);
	}
}

void Soldier::SetTargetBasePoint(ObjectModel* _targetBasePoint)
{
	targetBasePoint_ = _targetBasePoint;
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

	if (soldier == nullptr)
	{
		return nullptr;
	}
	else
	{
		Soldier* nearEnemySoldier = nullptr;
		float minDistance = 0.0f;

		for (;;)
		{
			if (soldier->GetActive() == true && soldier->GetGroup() != GetGroup() && soldier->GetBehave() != Soldier::Behave::DEAD)
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

	if (tower == nullptr)
	{
		return nullptr;
	}
	else
	{
		for (;;) {

			if (tower->GetGroup() != GetGroup())
			{
				if (Collision_SphereToSphere(GetPosition(), searchRange_, tower->GetPosition(), tower->GetRadius()))
				{
					break;
				}
			}

			tower = (Tower*)tower->GetNextPointer();

			if (tower == nullptr)
			{
				break;
			}
		}

		return tower;
	}
}

Castle* Soldier::SearchEnemyCastle()
{
	Castle* castle = (Castle*)Object::GetLDATA_HEAD(TYPE_MODEL_CASTLE);

	if (castle == nullptr)
	{
		return nullptr;
	}
	else
	{
		for (;;) {

			if (castle->GetGroup() != GetGroup())
			{
				if (Collision_SphereToSphere(GetPosition(), searchRange_, castle->GetPosition(), castle->GetRadius()))
				{
					break;
				}
			}

			castle = (Castle*)castle->GetNextPointer();

			if (castle == nullptr)
			{
				break;
			}
		}

		return castle;
	}
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
				if (searchCollider_->Collision(soldier->GetObjectCollider()) == true)
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
				if (searchCollider_->Collision(player->GetObjectCollider()) == true)
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

}

bool Soldier::CollidedBasePoint()
{
	const float kRadius = 0.0001f;

	if (Collision_SphereToSphere(GetPosition(), kRadius, targetBasePoint_->GetPosition(), targetBasePoint_->GetRadius()) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Soldier::BreakBasePoint()
{
	if (targetBasePoint_->GetType() == Object::TYPE::TYPE_MODEL_CASTLE)
	{
		//dynamic_cast<Castle*>(m_pAssaultBasePoint)->
	}
	else
	{
		if (dynamic_cast<Tower*>(targetBasePoint_)->BrowTower(breakPower_))
		{
			parentCommander_->ReceiveReport(Report::DEATH);
			return;
		}
	}
}
