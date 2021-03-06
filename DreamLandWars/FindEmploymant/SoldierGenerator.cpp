// author : yusuke seki
// data   : 20181111
#include "SoldierGenerator.h"
#include "SoldierCommander.h"
#include "Soldier.h"
#include "RelayPoint.h"
#include "BasePoint.h"

const unsigned int SoldierGenerator::kInterval_CreateOneSoldier_ = 20;

SoldierGenerator::SoldierGenerator(const unsigned int _kInterval_CreateSoldierLine_First, const unsigned int _kNumSoldier_First
	, const unsigned int _kInterval_CreateSoldierLine_Subsequent, const unsigned int _kNumSoldier_Subsequent)
	: kInterval_CreateSoldierLine_First_(_kInterval_CreateSoldierLine_First), kNumSoldier_First_(_kNumSoldier_First)
	, kInterval_CreateSoldierLine_Subsequent_(_kInterval_CreateSoldierLine_Subsequent), kNumSoldier_Subsequent_(_kNumSoldier_Subsequent)
	,Object(Object::TYPE::TYPE_GENERATOR)
{
	parentObject_ = nullptr;
	timer_ = 0;
	countCreateSoldier_ = 0;
	isFirstCreate_ = false;
	isUpdateTimer_ = false;
	soldierCommander_ = nullptr;
}

SoldierGenerator::~SoldierGenerator()
{
	Uninit();
}

SoldierGenerator* SoldierGenerator::Create(BasePoint* _parentObject
	, const unsigned int _kInterval_CreateSoldierLine_First, const unsigned int _kNumSoldier_First
	, const unsigned int _kInterval_CreateSoldierLine_Subsequent, const unsigned int _kNumSoldier_Subsequent)
{
	SoldierGenerator* soldierGenerator = new SoldierGenerator(_kInterval_CreateSoldierLine_First, _kNumSoldier_First, _kInterval_CreateSoldierLine_Subsequent, _kNumSoldier_Subsequent);
	soldierGenerator->Init(_parentObject);

	return soldierGenerator;
}

void SoldierGenerator::Init(BasePoint* _parentObject)
{
	parentObject_ = _parentObject;

	SetPosition(_parentObject->GetPosition());

	timer_ = kInterval_CreateSoldierLine_First_;
	
	countCreateSoldier_ = 0;
	
	isFirstCreate_ = true;
	
	isUpdateTimer_ = false;
	
	SetGroup(parentObject_->GetGroup());
	
	nextRelayPoint_ = parentObject_->GetNextRelayPoint();
	
	soldierCommander_ = nullptr;
}

void SoldierGenerator::Uninit()
{
	SetActive(false);
}

void SoldierGenerator::Update()
{
	if (isUpdateTimer_ == true)
	{
		UpdateTimer();

		if (IsAlertTimer() == true)
		{
			CreateSoldier();

			if (IsFilledSoldier() == true)
			{
				soldierCommander_->MarchedStart();
				soldierCommander_ = nullptr;
				countCreateSoldier_ = 0;
				ResetTimer(kInterval_CreateSoldierLine_Subsequent_);
				isFirstCreate_ = false;
			}
			else
			{
				ResetTimer(kInterval_CreateOneSoldier_);
			}
		}
	}
}

void SoldierGenerator::SetNextRelayPoint(RelayPoint* _relayPoint)
{
	nextRelayPoint_ = _relayPoint;
}

RelayPoint* SoldierGenerator::GetNextRelayPoint()
{
	return nextRelayPoint_;
}

const unsigned int SoldierGenerator::GetMaxNumSoldierInOneSoldierLine()
{
	if (kNumSoldier_First_ > kNumSoldier_Subsequent_)
	{
		return kNumSoldier_First_;
	}
	else
	{
		return kNumSoldier_Subsequent_;
	}
}

void SoldierGenerator::SetIsUpdateTimer(const bool& _isUpdateTimer)
{
	isUpdateTimer_ = _isUpdateTimer;
}

void SoldierGenerator::ReceiveReport_BreakedParent()
{
	if (soldierCommander_ != nullptr)
	{
		soldierCommander_->MarchedStart();
		soldierCommander_ = nullptr;
	}

	Release();
}

void SoldierGenerator::UpdateTimer()
{
	timer_ = timer_ == 0 ? 0 : timer_ - 1;
}

bool SoldierGenerator::IsAlertTimer()
{
	return timer_ == 0 ? true : false;
}

void SoldierGenerator::CreateSoldier()
{
	if (soldierCommander_ == nullptr)
	{
		FindSoldierCommander();
	}

	soldierCommander_->AddSoldier();

	++countCreateSoldier_;
}

bool SoldierGenerator::IsFilledSoldier()
{
	const unsigned int limitCreateSoldier = isFirstCreate_ == true ? kNumSoldier_First_ : kNumSoldier_Subsequent_;

	if (countCreateSoldier_ >= limitCreateSoldier)
	{
		return true;
	}

	return false;
}

void SoldierGenerator::ResetTimer(const unsigned int _timer)
{
	timer_ = _timer;
}

void SoldierGenerator::FindSoldierCommander()
{
	soldierCommander_ = SoldierCommander::DynamicCreate(this);
}
