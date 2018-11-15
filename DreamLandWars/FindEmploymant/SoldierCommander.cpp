// author : yusuke seki
// data   : 20181111
#include "SoldierCommander.h"
#include "soldier.h"
#include "collision.h"
#include "ObjectModel.h"
#include "ModelData.h"
#include "Etc_Paramaters.h"
#include "RelayPoint.h"

const unsigned int SoldierCommander::kInterval_ShotBullet_ = 300;
const float SoldierCommander::kLength_SoldierToSoldier = 10.0f;

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
	isActive_ = false;
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

	soldierList_.clear();

	soldierDestinationList_.clear();

	bulletTimer_ = 0;

	currentOrder_ = Order::FORM;

	isMarchedStart_ = false;

	isActive_ = true;
}

void SoldierCommander::Uninit()
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
	isActive_ = false;
}

void SoldierCommander::Update()
{
	if (isActive_ == false)
	{
		return;
	}

	if (isMarchedStart_ == true)
	{
		if (IsCorrectFormLine() == false)
		{
			if (IsChangeOrder(Order::FORM) == true)
			{
				ChangeOrder(Order::FORM);
			}
		}
		if (IsPathedTurnPoint() == true)
		{
			if (IsChangeOrder(Order::TURN) == true)
			{
				ChangeOrder(Order::TURN);
			}
		}

		ExecOrder();
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

void SoldierCommander::ReceiveReport(Soldier::Report _report, Soldier* _reporter, ObjectModel* _findBasePoint)
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

SoldierCommander* SoldierCommander::FindNonActiveSoldierCommander()
{
	SoldierCommander* soldierCommander = (SoldierCommander*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_COMMANDER);

	D3DXCreateSphere(, , , , , );

	// case is zero create
	if (soldierCommander != nullptr)
	{
		//SoldierCommander* next = (SoldierCommander*)soldierCommander->GetNextPointer();

		for (;;)
		{
			if (soldierCommander->GetIsActive() == false)
			{
				break;
			}

			soldierCommander = (SoldierCommander*)soldierCommander->GetNextPointer();

			if (soldierCommander == nullptr)
			{
				break;
			}

			//// 未使用なら兵士を生成して終了
			//if (!pCurrent->GetInstance()) {
			//	// 設定処理
			//	pCurrent->SetSoldierCommander_private(position, front, group);

			//	// 兵士を生成する
			//	pCurrent->SpawnSoldier();

			//	return pCurrent;
			//}

			//// 未使用領域が見つからなければ、新しく生成する
			//if (pNext == nullptr) {
			//	// 生成処理
			//	pNext = SoldierCommander::CreateBuffer();

			//	// 設定処理
			//	pNext->SetSoldierCommander_private(position, front, group);

			//	// 兵士を生成する
			//	pNext->SpawnSoldier();

			//	return pNext;
			//}

			//// ポインタをずらす
			//pCurrent = pNext;
			//pNext = (SoldierCommander*)pCurrent->GetNextPointer();

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

	for each (auto position in soldierDestinationList_)
	{
		position += front_ * speed_;
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

	isActive_ = false;
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
	for each (auto soldier in soldierList_)
	{
		soldier->AssaultBasePoint();
	}
}

void SoldierCommander::SoldierLine_Kill()
{
	for (unsigned int i = soldierList_.size(); i == 0; --i)
	{
		soldierList_[i - 1]->SelfDelete();
	}
}

void SoldierCommander::ReceiveReport_Death(Soldier* _soldier)
{
	RemoveSoldier(_soldier);

	if (soldierList_.size() == 0)
	{
		isActive_ = false;
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

void SoldierCommander::ReceiveReport_FindEnemyBasePoint(ObjectModel* _enemyBasePoint)
{
	if (IsChangeOrder(Order::ASSAULT_ENEMY_BASEPOINT) == true)
	{
		ChangeOrder(Order::ASSAULT_ENEMY_BASEPOINT);

		for each (auto soldier in soldierList_)
		{
			soldier->SetTargetBasePoint(_enemyBasePoint);
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

void SoldierCommander::ReceiveReport_None()
{
	if (IsChangeOrder(Order::RUN) == true)
	{
		ChangeOrder(Order::RUN);
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
			if ((currentOrder_ == Order::STOP) == true)
			{
				return true;
			}
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
			front_ = FindFront();
			right_ = FindRight();
			targetRelayPoint_ = FindNextRelayPoint();
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
	const float kErrorRange = 1.0f;

	for (unsigned int i = 0; i < soldierList_.size(); ++i)
	{
		float distance = Distance3D(soldierList_[i]->GetPosition(), soldierDestinationList_[i]);

		if (distance > kErrorRange)
		{
			return false;
		}
	}

	return true;
}

void SoldierCommander::ResetDestinationToSoldier()
{
	const float lengthSoldierLine = (soldierList_.size() - 1) * kLength_SoldierToSoldier;
	D3DXVECTOR3 leftEdgePosition = GetPosition() - right_ * lengthSoldierLine * 0.5f;
	D3DXVECTOR3 soldierDestination = leftEdgePosition;

	for (unsigned int i = 0; i < soldierList_.size(); ++i)
	{
		soldierDestination += right_ * kLength_SoldierToSoldier;
		soldierDestinationList_[i] = soldierDestination;
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
	float mostNearDistanceNextRelayPointFromOneSoldier = -1;

	position.x = targetRelayPoint_->GetPosition().x;
	position.y = targetRelayPoint_->GetPosition().y;

	for each (auto soldier in soldierList_)
	{
		float distance = Distance3D(nextPosition, soldier->GetPosition());

		if (mostNearDistanceNextRelayPointFromOneSoldier == -1 || mostNearDistanceNextRelayPointFromOneSoldier > distance)
		{
			mostNearDistanceNextRelayPointFromOneSoldier = distance;
			position.z = soldier->GetPosition().z;
		}
	}

	return position;
}

RelayPoint* SoldierCommander::FindNextRelayPoint()
{
	return targetRelayPoint_->GetNextRelayPoint(GetGroup());
}

D3DXVECTOR3 SoldierCommander::FindFront()
{
	D3DXVECTOR3 front;
	D3DXVECTOR3 currentRelayPosition = targetRelayPoint_->GetPosition();
	D3DXVECTOR3 nextRelayPosition = targetRelayPoint_->GetNextRelayPoint(GetGroup())->GetPosition();

	front = nextRelayPosition - currentRelayPosition;
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

