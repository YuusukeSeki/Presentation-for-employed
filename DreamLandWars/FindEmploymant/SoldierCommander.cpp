// author : yusuke seki
// data   : 20181111
#include "SoldierCommander.h"
#include "soldier.h"
#include "collision.h"
#include "BasePoint.h"
#include "ModelData.h"
#include "Etc_Paramaters.h"
#include "RelayPoint.h"

const unsigned int SoldierCommander::kInterval_ShotBullet_ = 300;
const float SoldierCommander::kLength_SoldierToSoldier_ = 2.5f;
const float SoldierCommander::kSpeed_SoldierRun_ = 0.2f;

SoldierCommander::SoldierCommander() : Object(Object::TYPE::TYPE_COMMANDER)
{
	currentOrder_ = Order::STOP;
	soldierGenerator_ = nullptr;
	front_ = D3DXVECTOR3(0, 0, 0);
	right_ = D3DXVECTOR3(0, 0, 0);
	speed_ = 0.0f;
	targetRelayPoint_ = nullptr;
	vectorToReleyPoint_ = D3DXVECTOR3(0, 0, 0);
	targetBasePoint_ = nullptr;
	soldierList_.clear();
	soldierDestinationList_.clear();
	bulletTimer_ = 0;
	isMarchedStart_ = false;
}

SoldierCommander::~SoldierCommander()
{
	Uninit();
}

void SoldierCommander::CreateBuffer(const unsigned int& _numCreate)
{
	for (unsigned int i = 0; i < _numCreate; ++i)
	{
		new SoldierCommander();
	}
}

SoldierCommander* SoldierCommander::Create(SoldierGenerator* _soldierGenerator)
{
	SoldierCommander* soldierCommander = new SoldierCommander();
	soldierCommander->Init(_soldierGenerator);

	return soldierCommander;
}

SoldierCommander* SoldierCommander::DynamicCreate(SoldierGenerator* _soldierGenerator)
{
	SoldierCommander* soldierCommander = FindNonActiveSoldierCommander();

	if (soldierCommander == nullptr)
	{
		soldierCommander = Create(_soldierGenerator);
	}
	else
	{
		soldierCommander->Init(_soldierGenerator);
	}

	return soldierCommander;
}

void SoldierCommander::Init(SoldierGenerator* _soldierGenerator)
{
	soldierGenerator_ = _soldierGenerator;

	SetPosition(soldierGenerator_->GetPosition());

	SetGroup(soldierGenerator_->GetGroup());

	targetRelayPoint_ = soldierGenerator_->GetNextRelayPoint();

	front_ = FindFront();

	right_ = FindRight();

	vectorToReleyPoint_ = front_;

	targetBasePoint_ = nullptr;

	speed_ = kSpeed_SoldierRun_;

	soldierList_.clear();

	soldierDestinationList_.clear();

	bulletTimer_ = 0;

	currentOrder_ = Order::FORM;

	isMarchedStart_ = false;

	SetActive(true);
}

void SoldierCommander::Uninit()
{
	for each (auto soldier in soldierList_)
	{
		soldier->Uninit();
	}
	soldierList_.clear();

	soldierDestinationList_.clear();

	SetActive(false);
}

void SoldierCommander::Update()
{
	if (isMarchedStart_ == true)
	{
		if (targetBasePoint_ == nullptr)
		{
			if (IsPathedTurnPoint() == true)
			{
				if (IsChangeOrder(Order::TURN) == true)
				{
					ChangeOrder(Order::TURN);
				}
			}
			else if (IsCorrectFormLine() == false)
			{
				if (IsChangeOrder(Order::FORM) == true)
				{
					ChangeOrder(Order::FORM);
				}
			}

			ExecOrder();

			ChangeOrder(Order::RUN);
		}
		else
		{
			if (targetBasePoint_->IsBreak() == true)
			{
				Order_Death();
				return;
			}
			else
			{
				ExecOrder();
			}
		}
	}
	else
	{
		Order_Form();
	}

	UpdateBulletTimer();
}

void SoldierCommander::AddSoldier()
{
	soldierList_.push_back(Soldier::DynamicCreate(this));

	soldierDestinationList_.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	ResetDestinationToSoldier();

	SetDestinationToSoldier();
}

void SoldierCommander::MarchedStart()
{
	isMarchedStart_ = true;

	ChangeOrder(Order::RUN);
}

void SoldierCommander::ReceiveReport(Soldier::Report _report, Soldier* _reporter, BasePoint* _findBasePoint)
{
	switch (_report)
	{
		case Soldier::Report::DEATH:
			ReceiveReport_Death(_reporter);
			break;

		case Soldier::Report::FIND_FRIEND_UNIT:
			ReceiveReport_FindFriendUnit();
			break;

		case Soldier::Report::FIND_ENEMY_UNIT:
			ReceiveReport_FindEnemyUnit();
			break;

		case Soldier::Report::FIND_ENEMY_BASEPOINT:
			ReceiveReport_FindEnemyBasePoint(_findBasePoint);
			break;

		case Soldier::Report::BREAK_ENEMY_BASEPOINT:
			ReceiveReport_BreakBasePoint();
			break;
	}
}

D3DXVECTOR3 SoldierCommander::GetFront()
{
	return front_;
}

float SoldierCommander::GetSpeed()
{
	return speed_;
}

RelayPoint* SoldierCommander::GetTargetRelayPoint()
{
	return targetRelayPoint_;
}

SoldierCommander* SoldierCommander::FindNonActiveSoldierCommander()
{
	SoldierCommander* soldierCommander = (SoldierCommander*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_COMMANDER);

	if (soldierCommander != nullptr)
	{
		for (;;)
		{
			if (soldierCommander->GetActive() == false)
			{
				break;
			}

			soldierCommander = (SoldierCommander*)soldierCommander->GetNextPointer();

			if (soldierCommander == nullptr)
			{
				break;
			}
		}
	}

	return soldierCommander;
}

void SoldierCommander::ExecOrder()
{
	switch (currentOrder_)
	{
		case Order::STOP:
			Order_Stop();
			break;

		case Order::FORM:
			Order_Form();
			break;

		case Order::RUN:
			Order_Run();
			break;

		case Order::TURN:
			Order_Turn();
			break;

		case Order::ATTACK_ENEMY_UNIT:
			Order_AttackEnemyUnit();
			break;

		case Order::ASSAULT_ENEMY_BASEPOINT:
			Order_AssaultEnemyBasePoint();
			break;

		case Order::DEATH:
			Order_Death();
			break;
	}
}

void SoldierCommander::Order_Stop()
{
	SoldierLine_Stop();
}

void SoldierCommander::Order_Form()
{
	SoldierLine_FormLine();
}

void SoldierCommander::Order_Run()
{
	SoldierLine_Run();

	MovePosition(front_ * speed_);

	for (unsigned int i = 0; i < soldierDestinationList_.size(); ++i)
	{
		soldierDestinationList_[i] += front_ * speed_;
	}
}

void SoldierCommander::Order_Turn()
{
	SoldierLine_Turn();
}

void SoldierCommander::Order_AttackEnemyUnit()
{
	if (IsAlartBulletTimer() == true)
	{
		SoldierLine_ShotBullet();

		ResetBulletTimer(kInterval_ShotBullet_);
	}
}

void SoldierCommander::Order_AssaultEnemyBasePoint()
{
	SoldierLine_AssaultBasePoint();
}

void SoldierCommander::Order_Death()
{
	SoldierLine_Kill();

	SetActive(false);
}

void SoldierCommander::SoldierLine_Stop()
{
	for each (auto soldier in soldierList_)
	{
		soldier->Stop();
	}
}

void SoldierCommander::SoldierLine_FormLine()
{
	SetDestinationToSoldier();

	for each (auto soldier in soldierList_)
	{
		soldier->FormLine();
	}
}

void SoldierCommander::SoldierLine_Run()
{
	for each (auto soldier in soldierList_)
	{
		soldier->Run();
	}
}

void SoldierCommander::SoldierLine_Turn()
{
	SetDestinationToSoldier();

	for each (auto soldier in soldierList_)
	{
		soldier->SetEndFront(front_);
	}
}

void SoldierCommander::SoldierLine_ShotBullet()
{
	for each (auto soldier in soldierList_)
	{
		soldier->ShotBullet();
	}
}

void SoldierCommander::SoldierLine_AssaultBasePoint()
{
	for (unsigned int i = 0; i < soldierList_.size(); ++i)
	{
		soldierList_[i]->AssaultBasePoint();
	}
}

void SoldierCommander::SoldierLine_Kill()
{
	for (unsigned int i = soldierList_.size(); i > 0; --i)
	{
		soldierList_[i - 1]->Death();
	}
}

void SoldierCommander::ReceiveReport_Death(Soldier* _soldier)
{
	RemoveSoldier(_soldier);

	if (soldierList_.size() == 0)
	{
		SetActive(false);
	}
	else
	{
		ResetDestinationToSoldier();
		SetDestinationToSoldier();
	}
}

void SoldierCommander::ReceiveReport_FindFriendUnit()
{
	if (IsChangeOrder(Order::STOP) == true)
	{
		ChangeOrder(Order::STOP);
	}
}

void SoldierCommander::ReceiveReport_FindEnemyUnit()
{
	if (IsChangeOrder(Order::ATTACK_ENEMY_UNIT) == true)
	{
		ChangeOrder(Order::ATTACK_ENEMY_UNIT);
	}
}

void SoldierCommander::ReceiveReport_FindEnemyBasePoint(BasePoint* _enemyBasePoint)
{
	if (IsChangeOrder(Order::ASSAULT_ENEMY_BASEPOINT) == true)
	{
		ChangeOrder(Order::ASSAULT_ENEMY_BASEPOINT);

		targetBasePoint_ = _enemyBasePoint;

		for each (auto soldier in soldierList_)
		{
			soldier->SetTargetBasePoint(targetBasePoint_);
		}
	}
}

void SoldierCommander::ReceiveReport_BreakBasePoint()
{
	if (IsChangeOrder(Order::DEATH) == true)
	{
		ChangeOrder(Order::DEATH);
	}
}

bool SoldierCommander::IsChangeOrder(const Order& _changeOrder)
{
	if (currentOrder_ == _changeOrder)
	{
		return true;
	}

	switch (_changeOrder)
	{
		case Order::STOP:
			if ((currentOrder_ == Order::RUN) == true)
			{
				return true;
			}
			break;

		case Order::FORM:
			if ((currentOrder_ == Order::STOP || currentOrder_ == Order::RUN || currentOrder_ == Order::ATTACK_ENEMY_UNIT) == true)
			{
				return true;
			}
			break;

		case Order::RUN:
			// RUN 状態には、毎フレームのExecOrder()後の強制変更以外には変更しない
			break;

		case Order::TURN:
			if ((currentOrder_ == Order::STOP || currentOrder_ == Order::RUN) == true)
			{
				return true;
			}
			break;

		case Order::ATTACK_ENEMY_UNIT:
			if ((currentOrder_ == Order::STOP || currentOrder_ == Order::RUN) == true)
			{
				return true;
			}
			break;

		case Order::ASSAULT_ENEMY_BASEPOINT:
			if ((currentOrder_ == Order::STOP  || currentOrder_ == Order::RUN || currentOrder_ == Order::FORM || currentOrder_ == Order::ATTACK_ENEMY_UNIT) == true)
			{
				return true;
			}
			break;

		case Order::DEATH:
			return true;

		default:
			_MSGERROR("範囲外の値が要求されました", "SoldierCommander::IsChangeOrder");
			break;
	}

	return false;
}

void SoldierCommander::ChangeOrder(const Order& _changeOrder)
{
	if (currentOrder_ == _changeOrder)
	{
		return;
	}

	currentOrder_ = _changeOrder;

	switch (currentOrder_)
	{
		case Order::STOP:
			break;

		case Order::FORM:
			break;

		case Order::RUN:
			break;

		case Order::TURN:
			SetPosition(FindPositionAfterTurned());
			targetRelayPoint_ = targetRelayPoint_->GetNextRelayPoint(GetGroup());
			front_ = FindFront();
			right_ = FindRight();
			ResetDestinationToSoldier();
			break;

		case Order::ATTACK_ENEMY_UNIT:
			break;

		case Order::ASSAULT_ENEMY_BASEPOINT:
			break;

		case Order::DEATH:
			break;

		default:
			_MSGERROR("範囲外の値が要求されました", "SoldierCommander::IsChangeOrder");
			break;
	}
}

void SoldierCommander::UpdateBulletTimer()
{
	bulletTimer_ = bulletTimer_ == 0 ? 0 : bulletTimer_ - 1;
}

bool SoldierCommander::IsAlartBulletTimer()
{
	if (bulletTimer_ == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SoldierCommander::RemoveSoldier(Soldier* _soldier)
{
	for (unsigned int i = 0; i < soldierList_.size(); ++i)
	{
		if (soldierList_[i] == _soldier)
		{
			soldierList_.erase(soldierList_.begin() + i);
			break;
		}
	}

	soldierDestinationList_.pop_back();
}

void SoldierCommander::ResetBulletTimer(const unsigned int& _time)
{
	bulletTimer_ = _time;
}

bool SoldierCommander::IsCorrectFormLine()
{
	for (unsigned int i = 0; i < soldierList_.size(); ++i)
	{
		float distance = Distance3D(soldierList_[i]->GetPosition(), soldierDestinationList_[i]);

		if (distance > 0)
		{
			return false;
		}
	}

	return true;
}

void SoldierCommander::ResetDestinationToSoldier()
{
	float maxLength = kLength_SoldierToSoldier_ * 8;
	float soldierToSoldier = maxLength / (soldierList_.size() + 1);

	D3DXVECTOR3 destination = soldierList_.size() % 2 != 0
		? GetPosition()
		: GetPosition() - (right_ * soldierToSoldier * 0.5f);

	for (unsigned int i = 0; i < soldierList_.size(); ++i)
	{
		destination = i % 2 != 0
			? destination + (right_ * soldierToSoldier * (float)i)
			: destination - (right_ * soldierToSoldier * (float)i);

		soldierDestinationList_[i] = destination;
	}
}

void SoldierCommander::SetDestinationToSoldier()
{
	for (unsigned int i = 0; i < soldierList_.size(); ++i)
	{
		soldierList_[i]->SetDestination(soldierDestinationList_[i]);
	}
}

bool SoldierCommander::IsPathedTurnPoint()
{
	float result;
	D3DXVECTOR3 currentVectorToRelayPoint = targetRelayPoint_->GetPosition() - GetPosition();
	D3DXVec3Normalize(&currentVectorToRelayPoint, &currentVectorToRelayPoint);

	result = D3DXVec3Dot(&currentVectorToRelayPoint, &vectorToReleyPoint_);

	if (result <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

D3DXVECTOR3 SoldierCommander::FindPositionAfterTurned()
{
	D3DXVECTOR3 position(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 nextPosition = targetRelayPoint_->GetNextRelayPoint(GetGroup())->GetPosition();
	float mostFrontSoldierPosZ = -1;

	position.x = targetRelayPoint_->GetPosition().x;
	position.y = targetRelayPoint_->GetPosition().y;

	for each (auto soldier in soldierList_)
	{
		float distance = Distance3D(nextPosition, soldier->GetPosition());

		if (mostFrontSoldierPosZ == -1 || mostFrontSoldierPosZ > distance)
		{
			mostFrontSoldierPosZ = distance;
			position.z = soldier->GetPosition().z;
		}
	}

	return position;
}

D3DXVECTOR3 SoldierCommander::FindFront()
{
	D3DXVECTOR3 front;
	D3DXVECTOR3 currentPosition = GetPosition();
	D3DXVECTOR3 destination = targetRelayPoint_->GetPosition();

	front = destination - currentPosition;
	D3DXVec3Normalize(&front, &front);

	return front;
}

D3DXVECTOR3 SoldierCommander::FindRight()
{
	D3DXVECTOR3 right;
	D3DXVECTOR3 front = front_;
	D3DXVECTOR3 up(0, 1, 0);

	D3DXVec3Cross(&right, &up, &front);
	D3DXVec3Normalize(&right, &right);

	return right;
}
